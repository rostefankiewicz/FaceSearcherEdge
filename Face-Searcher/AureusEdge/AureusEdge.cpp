//////////////////////////////////////////////////////////////////////////////////////
// AureusEdge.cpp : Defines the entry point for the console application.
//////////////////////////////////////////////////////////////////////////////////////
// Copyright: (C) 2016 CyberExtruder.com Inc.
//                     www.cyberextruder.com
//
// DISCLAIMER:
//
// THIS SOFTWARE IS PROVIDED “AS IS” AND “WITH ALL FAULTS” AND WITHOUT
// WARRANTY OF ANY KIND.
// CUSTOMER AGREES THAT THE USE OF THIS SOFTWARE IS AT CUSTOMER'S RISK
// CYBEREXTRUDER MAKES NO WARRANTY OF ANY KIND TO CUSTOMER OR ANY THIRD 
// PARTY, EXPRESS, IMPLIED OR STATUTORY, WITH RESPECT TO THE THIS 
// SOFTWARE, OPERATION OF THE SOFTWARE, OR OUTPUT OF OR RESULTS OBTAINED 
// FROM THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY 
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OPERABILITY OR 
// NON-INFRINGEMENT AND ALL SUCH WARRANTIES ARE HEREBY EXCLUDED BY 
// CYBEREXTRUDER AND WAIVED BY CUSTOMER.
//////////////////////////////////////////////////////////////////////////////////////
//
// This example code shows how to use Aureus for an edge device, e.g. performing
// detection, tracking and saving the results to a file and/or positing them
// to an internet address.
//
//////////////////////////////////////////////////////////////////////////////////////


// Leon
#include "includes.h"
#include "filehandling.h"
#include "xmlhandling.h"
#include "Camera.h"


#ifndef WIN32
#include <unistd.h>
#endif


// Leon: For tinyxml
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif



void DoSleep(int millisec)
{
#ifdef WIN32
	Sleep(millisec);
#else
	usleep(millisec * 1000);
#endif
}


// Leon: Set custom read stream
istream mycin(std::cin.rdbuf());
// Leon: Switch to this stream to restart unexpectedly terminated streams
istringstream restartStream("-2");


// Leon
void GetCommandInput(int& command)
{
	mycin >> command;
	// Return to cin buffer
	mycin.rdbuf(std::cin.rdbuf());
}


// Leon: Set the connection info tag of the camera structure
void SetConnectionInfo(Camera *camera)
{
	// Media stream
	if (camera->m_type == 0)
	{
		// Have an option of which video file you want for media stream
		vector<char*> video_paths;
		video_paths.push_back("C:/Allevate/Face-Searcher/Videos/one.wmv");
		video_paths.push_back("C:/Allevate/Face-Searcher/Videos/two.wmv");
		video_paths.push_back("C:/Allevate/Face-Searcher/Videos/three.wmv");
		video_paths.push_back("C:/Allevate/Face-Searcher/Videos/four.wmv");
		video_paths.push_back("C:/Allevate/Face-Searcher/Videos/five.wmv");

		int videonumber;
		printf("Pick video file (integer >= 1, look at SetConnectionInfo): ");
		GetCommandInput(videonumber);
		while (videonumber < 1 || videonumber > video_paths.size())
		{
			printf("Index out of range\n");
			printf("Pick video file (integer > 0, look at SetConnectionInfo): ");
			GetCommandInput(videonumber);
		}

		strcpy(camera->m_connection_info, video_paths[videonumber - 1]);
	}

	// USB stream
	else if (camera->m_type == 1)
	{
		char usb_pin[1024];
		printf("Pick usb pin: ");
		cin >> usb_pin;

		strcpy(camera->m_connection_info, usb_pin);
	}

	// IP stream
	else if (camera->m_type == 2)
	{
		strcpy(camera->m_connection_info, camera->m_url);
	}
}


// Simple frame call back
void FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, void* p_object)
{
	// prints all data
	//PrintHeadListData(frame_num, p_head_list);

	// just print num of heads being tracked
	char msg[1204];

	// Leon: Pass in the camera
	Camera *camera = (Camera*)p_object;

	int n_heads = CX_GetHeadListSize(p_head_list, msg);
	if (n_heads < 0) printf("Frame %d Error: %s\n", frame_num, msg);
	else
	{
		//printf("FRAME %d, Tracking %d HEADS rows=%d cols=%d     \r", frame_num, n_heads, rows, cols);
		if (camera->m_showProgress)
		{
			printf("FRAME %d, Tracking %d HEADS     \r", frame_num, n_heads);

			// or print more detailed info (including FR if switched on)
			//PrintHeadListData(frame_num, p_head_list, true);
		}
	}

}


// Leon: Forward declare these since it is used in UnexpectedTerminationCallback

void StartCamera(Camera *camera);
void StopCamera(Camera *camera);


// Simple unexpected stream termination call back
void UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object)
{
	printf("bitch\n");

	Camera *camera = (Camera*)p_object;

	camera->m_unexpectedlyterminated = true;

	EmailConfig *email = camera->m_email;
	char cameraInfo[1024];
	//sprintf(message, "Unexpected stream termination, stream type = %d, connection_info = %s\n", stream_type, connection_info);
	RecordCameraInfo(camera, cameraInfo);
	fprintf(stderr, cameraInfo);
	strcpy(email->m_message, cameraInfo);
	strcat(email->m_message, "\n");
	SendEmail(email);

	// Restart stream after 10s
	printf("Restarting camera %d\n\n", camera->m_cameraindex);
	DoSleep(5 * 1000);

}


// Leon
bool CreateCamera(CX_AUREUS p_aureus, vector<Camera*>& cameras)
{
	char msg[1024];

	size_t streamindex = cameras.size() + 1;

	// create a video object
	printf("Creating CX_VIDEO object %d\n", streamindex);
	CX_VIDEO p_video = CX_CreateVideo(p_aureus, msg);
	if (!p_video)
	{
		printf("%s\n", msg);
		printf("Freeing Aureus\n");
		if (!CX_FreeAureus(p_aureus, msg))
		{
			printf("Failed to free Aureus:\n%s\n", msg);
		}
		else
		{
			printf("Successfully freed Aureus\n");
		}
		return false;
	}
	printf("Successfully created CX_VIDEO\n");

	// Leon: Force every frame
	if (!CX_SetForceEveryFrame(p_video, 1, msg))
	{
		printf("%s\n", msg);
	}

	// Get settings from user
	Camera *camera = new Camera;

	camera->m_aureus = p_aureus;
	camera->m_video = p_video;

	camera->m_streamindex = (int)streamindex;

	// Camera is not running yet, of course
	camera->m_running = false;

	camera->m_unexpectedlyterminated = false;

	printf("\nChoose a camera index for stream %d: ", camera->m_streamindex);
	GetCommandInput(camera->m_cameraindex);
	if (camera->m_cameraindex == -1)
	{
		delete camera;
		return false;
	}
	else if (camera->m_cameraindex < 0)
	{
		printf("Camera index %d is invalid\n", camera->m_cameraindex);
		delete camera;
		return false;
	}

	// Leon: Get path to camera configuration
	strcpy(camera->m_configPath, "C:/Allevate/Face-Searcher/CameraConfiguration.xml");

	// Leon: Make new EmailConfig for camera
	camera->m_email = new EmailConfig;

	// Leon: Email config for camera
	GetEmailConfig(camera);

	// Leon: Info about the camera
	GetCameraInfo(camera);

	// Leon: The settings for the camera
	GetControlSettings(camera);

	// Leon: What is stored in the image xmls
	GetResultsSettings(camera);

	printf("\nChoose stream type for camera %d (0 media, 1 usb, 2 ip): ", camera->m_cameraindex);
	GetCommandInput(camera->m_type);
	if (camera->m_type == -1)
	{
		delete camera;
		return false;
	}
	if (camera->m_type < 0 || camera->m_type > 2)
	{
		printf("Media type %d is invalid\n", camera->m_type);
		delete camera;
		return false;
	}

	// Leon: Set the connection info field of the camera
	SetConnectionInfo(camera);

	// Set frame callback
	printf("Setting frame callback for camera %d, which is in stream %d\n\n", camera->m_cameraindex, camera->m_streamindex);
	if (0 == CX_SetFrameCallBack(camera->m_video, FrameCallBack, (void*)camera, msg))
	{
		printf("%s\n", msg);
		delete camera;
		return false;
	}

	// Do something if stream unexpectedly terminates
	printf("Setting unexpected termination callback for camera %d, which is in stream %d\n\n", camera->m_cameraindex, camera->m_streamindex);
	if (0 == CX_SetStreamTerminatedCallBack(camera->m_video, UnexpectedTerminationCallBack, (void*)camera, msg))
	{
		printf("%s\n", msg);
		delete camera;
		return false;
	}

	// Disable output from frame callback
	camera->m_showProgress = false;

	// Leon: Add to camera vector
	cameras.push_back(camera);

	return true;
}


// Leon: Start camera stream
void StartCamera(Camera *camera)
{
	char msg[1024];

	// Camera has already been started
	if (camera->m_running)
	{
		printf("Camera %d has already been started\n\n", camera->m_cameraindex);
		return;
	}

	// Start processing the stream
	printf("PROCESSING camera %d, which is in stream %d\n\n", camera->m_cameraindex, camera->m_streamindex);
	if (0 == CX_ProcessStream(camera->m_video, camera->m_type, camera->m_connection_info, msg))
	{
		printf("%s\n", msg);
		printf("Inside start camera\n");
		return;
	}

	camera->m_running = true;
}


// Leon
void StartSpecifiedCamera(vector<Camera*>& cameras)
{
	int cameraindex;
	size_t num_cameras = cameras.size();

	printf("Enter camera index to start (0 to start all): ");
	GetCommandInput(cameraindex);
	if (cameraindex == -1)
	{
		return;
	}
	else if (cameraindex < 0 || cameraindex > num_cameras)
	{
		printf("Camera index outside bounds\n");
		return;
	}

	// Start all cameras
	if (cameraindex == 0)
	{
		for (Camera *camera : cameras)
		{
			StartCamera(camera);
		}
		return;
	}

	// Find camera
	Camera *chosenCamera = NULL;

	for (Camera *camera : cameras)
	{
		if (camera->m_cameraindex == cameraindex)
		{
			chosenCamera = camera;
		}
	}

	// Camera does not exist
	if (!chosenCamera)
	{
		printf("Camera %d does not exist\n", cameraindex);
		return;
	}

	StartCamera(chosenCamera);
}


// Leon: Stop camera at user's request
void StopCamera(Camera *camera)
{
	char msg[1024];

	// Camera has already been stopped
	if (!camera->m_running)
	{
		printf("Camera %d has already been stopped\n\n", camera->m_cameraindex);
		return;
	}

	// Stop the stream
	camera->m_running = false;
	printf("STOPPING camera %d, which is in stream %d\n\n", camera->m_cameraindex, camera->m_streamindex);
	if (0 == CX_StopStream(camera->m_video, msg))
	{
		printf("%s\n", msg);
		return;
	}
}


void StopSpecifiedCamera(vector<Camera*>& cameras)
{
	int cameraindex;
	size_t num_cameras = cameras.size();


	printf("Enter camera index to stop (0 to stop all): ");
	GetCommandInput(cameraindex);
	if (cameraindex == -1)
	{
		return;
	}
	else if (cameraindex < 0 || cameraindex > num_cameras)
	{
		printf("Camera index outside bounds\n");
		return;
	}
	
	// Start all cameras
	if (cameraindex == 0)
	{
		for (Camera *camera : cameras)
		{
			StopCamera(camera);
		}
		return;
	}

	// Find camera
	Camera *chosenCamera = NULL;

	for (Camera *camera : cameras)
	{
		if (camera->m_cameraindex == cameraindex)
		{
			chosenCamera = camera;
		}
	}

	// Camera does not exist
	if (!chosenCamera)
	{
		printf("Camera %d does not exist\n", cameraindex);
		return;
	}

	StopCamera(chosenCamera);
}


// Leon
void RestartUnexpectedlyTerminatedStreams(vector<Camera*>& cameras)
{
	for (Camera *camera : cameras)
	{
		camera->m_unexpectedlyterminated = true;
		if (camera->m_unexpectedlyterminated)
		{
			camera->m_running = false;
			StartCamera(camera);
			camera->m_unexpectedlyterminated = false;
		}
	}
}


// Leon
void ToggleShowProgress(vector<Camera*>& cameras, int cameraindex)
{
	size_t num_cameras = cameras.size();
	
	// Find camera
	Camera *chosenCamera = NULL;

	for (Camera *camera : cameras)
	{
		if (camera->m_cameraindex == cameraindex)
		{
			chosenCamera = camera;
		}
	}

	// Camera does not exist
	if (!chosenCamera)
	{
		printf("Camera %d does not exist\n", cameraindex);
		return;
	}

	// Toggle
	chosenCamera->m_showProgress = !chosenCamera->m_showProgress;
}


// Leon
void ShowLicenseInfo(CX_AUREUS p_aureus)
{
	char msg[1024];

	int command;

	printf("\n");
	printf("License info\n");
	printf("-1: Return to main menu\n");
	printf("0: Number of streams authorized by license\n");
	printf("1: Time until license becomes invalid\n");

	printf("Enter command: ");
	GetCommandInput(command);

	switch (command)
	{
	case -1:
		return;
	case 0:
		int num_streams, image_enabled, fr_enabled;
		CX_GetLicenseParameters(p_aureus, &num_streams, &image_enabled, &fr_enabled, msg);
		printf("Number of licensed video streams = %d\n", num_streams);
		break;
	case 1:
		CX_GetLicenseInfo(p_aureus, msg);
		printf("Time remaining: %s\n", msg);
		break;
	}
}


// Leon
void ShowCameraInfo(vector<Camera*>& cameras)
{
	size_t num_cameras = cameras.size();
	char* message = new char[num_cameras * 1024];
	strcpy(message, "");

	for (Camera *camera : cameras)
	{
		char cameraInfo[1024];
		RecordCameraInfo(camera, cameraInfo);
		strcat(message, cameraInfo);
		strcat(message, "\n");
	}

	printf(message);

	delete[] message;
}


// Leon
void DisplayCommands()
{
	printf("\n");
	printf("Command Legend\n");
	printf("-1: Return to main menu\n");
	printf("0: Quit program\n");
	printf("1: Add camera stream\n");
	printf("2: Start camera\n");
	printf("3: Stop camera\n");
	printf("4: License info\n");
	printf("5: Info of camera streams\n");
	printf("\n");
}


// Leon
void UserInteraction(CX_AUREUS p_aureus, vector<Camera*>& cameras)
{
	int command;
	while (true)
	{
		DisplayCommands();
		printf("Enter a command: ");

		// Get the command
		GetCommandInput(command);

		printf("\n");

		switch (command)
		{
		case -2:
			RestartUnexpectedlyTerminatedStreams(cameras);
			break;
		case -1:
			break;
		case 0:
			return;
		case 1:
			CreateCamera(p_aureus, cameras);
			break;
		case 2:
			StartSpecifiedCamera(cameras);
			break;
		case 3:
			StopSpecifiedCamera(cameras);
			break;
		case 4:
			ShowLicenseInfo(p_aureus);
			break;
		case 5:
			ShowCameraInfo(cameras);
			break;
		default:
			printf("Invalid command\n");
			break;
		}
	}
}


int main(int argc, char* argv[])
{
	char msg[1024];

	int load_gallery = 0; // instruct Aureus NOT to load the FR gallery
	bool load_FR_engines = false; // instruct Aureus NOT to load FR engines
	// For this example program fr is switched off since we are just detecting and tracking, 
	bool use_fr = false;
	int generate_templates = 0; // no need to generate templates
	int perform_gallery = 0; // no need to perform FR ranking (in fact we can't since we are not generating templates)

	// just a bit of feedback to std out
	printf("----------------\n");
	if (load_FR_engines) printf("Loading FR engines\n");
	else printf("Not loading FR engines\n");
	if (use_fr) printf("Using FR\n");
	else printf("Not using FR\n");
	if (load_gallery) printf("Loading Gallery\n");
	else printf("Not loading gallery\n");
	printf("----------------\n");

	// first thing we do is demonstrate getting the machine ID
	if (!CX_GetMachineID(msg))
	{
		printf("Failed to get machine ID:\n%s\n", msg);
	}
	else
	{
		printf("Machine ID = %s\n", msg);
	}

	CX_AUREUS p_aureus = CX_CreateAureus(msg);
	if (!p_aureus)
	{
		printf("Failed to create Aureus Object\n%s\n", msg);
		return 0;
	}
	printf("Created Aureus\n");

	int use_verbose = 1; // can be useful if you have (somehow) lost required files
	(void)CX_SetVerbose(p_aureus, use_verbose, msg);

	// print out the Aureus version
	if (!CX_GetVersion(p_aureus, msg)) printf("Failed to get Aureus Version:\n%s\n", msg);
	else printf("Aureus Version = %s\n", msg);

	// initialize and process video frames
	// 
	// this is the default installation path for Aureus, obviously this will need
	// to be altered if you install anywhere else
#ifdef WIN32
#ifdef IS_32_BIT
	const char* install_dir = "C:/Allevate/Face-Searcher32"; // this is the default folder for 32 bit Aureus
#else
	const char* install_dir = "C:/Allevate/Face-Searcher";
#endif
#else
#ifdef IS_32_BIT
	const char* install_dir = "/home/tim/Allevate/Face-Searcher32"; // this is the default folder for 32 bit Aureus
#else
	const char* install_dir = "/home/tim/Allevate/Face-Searcher";
#endif
#endif

	printf("Initializing Aureus from %s\n", install_dir);
	if (CX_Initialize(p_aureus, load_gallery, install_dir, msg))
	{
		printf("Successful initialization\n");
	}
	// Initialization failed
	else
	{
		printf("Failed to Initialize Aureus:\n%s\n", msg);
	}
	// print license info
	printf("------- License Info --------\n");
	(void)CX_GetLicenseInfo(p_aureus, msg);
	printf("License Info: %s\n", msg);
	int num_streams, image_enabled, fr_enabled;
	(void)CX_GetLicenseParameters(p_aureus, &num_streams, &image_enabled, &fr_enabled, msg);
	printf("Number of licensed video streams = %d\n", num_streams);
	printf("Image enabled = %d\n", image_enabled);
	printf("FR Enabled = %d\n", fr_enabled);
	printf("-----------------------------\n");


	// Leon: Set the parameters for each stream
	vector<Camera*> cameras;

	// Let user interact with program
	UserInteraction(p_aureus, cameras);

	// Leon: Destroy cameras
	for (Camera *camera : cameras)
	{
		delete camera->m_email;
		delete camera;
	}

	// Leon: Free aureus
	printf("Freeing aureus\n");
	if (!CX_FreeAureus(p_aureus, msg))
	{
		printf("Failed to free Aureus:\n%s\n", msg);
	}
	else
	{
		printf("Successfully freed Aureus\n");
	}

	while (true); // Leon: Close the application manually


	// uncomment to test that the video stream licenses are renewed
	// after a call to CX_FreeAureus
	
	printf("------ Testing Video Stream License Renewal ------\n");
	p_aureus = CX_CreateAureus(msg);
	if (CX_Initialize(p_aureus, load_gallery, install_dir, msg))
	{
	printf("Successful initialization\n");
	}
	CX_VIDEO p_video2 = CX_CreateVideo(p_aureus, msg);
	if (!p_video2)
	{
	printf("%s\n", msg);
	return 0;
	}
	printf("Created CX_VIDEO\n");
	if (!CX_FreeAureus(p_aureus, msg)) printf("Failed to free Aureus:\n%s\n", msg);
	else printf("Successfully freed Aureus\n");
	
	return 0;
}
