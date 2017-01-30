// Leon: File created by me

#include "xmlhandling.h"

using namespace std;

bool WordToBool(char* value)
{
	if (!value)
	{
		printf("Value passed into WordToBool is NULL\n");
		return false;
	}
	if (value[0] == 't' || value[0] == 'T')
	{
		return true;
	}
	else if (value[0] == 'f' || value[0] == 'F')
	{
		return false;
	}
	else
	{
		printf("Value passed into WordToBool is neither true nor false\n");
		return false;
	}
}

bool GetValueOfTag(TiXmlElement *settings, char* tag, char* value)
{
	TiXmlElement *element = settings->FirstChildElement(tag);
	if (!element)
	{
		return false;
	}
	TiXmlNode *valueNode = element->FirstChild();
	if (!valueNode)
	{
		return false;
	}
	TiXmlText *valueText = valueNode->ToText();
	if (!valueText)
	{
		return false;
	}
	if (!valueText->Value())
	{
		return false;
	}
	strcpy(value, valueText->Value());
	return true;
}

void GetEmailConfig(Camera *camera)
{
	char tag[1024];
	char value[1024];

	CX_VIDEO p_video = camera->m_video;
	char* configPath = camera->m_configPath;
	int cameraindex = camera->m_cameraindex;
	EmailConfig *email = camera->m_email;

	printf("\nEmail configuration for camera %d\n", cameraindex);

	// Open the camera xml file
	TiXmlDocument config(configPath);
	if (!config.LoadFile())
	{
		printf("Could not load file %s\n", configPath);
		return;
	}

	// Obtain email settings tag
	TiXmlElement *root = config.FirstChildElement();
	if (!root)
	{
		printf("Could not obtain SOAP-ENV:Envelope\n");
		return;
	}

	strcpy(tag, "emailConfiguration");
	TiXmlElement *emailConfiguration = root->FirstChildElement(tag);
	if (!emailConfiguration)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}
	strcpy(tag, "emailSettings");
	TiXmlElement *emailSettings = emailConfiguration->FirstChildElement(tag);
	if (!emailSettings)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	// Set email settings

	strcpy(tag, "sender");
	if (!GetValueOfTag(emailSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(email->m_sender, value);
		printf("%s = %s\n", tag, email->m_sender);
	}

	strcpy(tag, "password");
	if (!GetValueOfTag(emailSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(email->m_password, value);
		printf("%s = %s\n", tag, email->m_password);
	}

	strcpy(tag, "receiver");
	if (!GetValueOfTag(emailSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(email->m_receiver, value);
		printf("%s = %s\n", tag, email->m_receiver);
	}

	strcpy(tag, "contents");
	if (!GetValueOfTag(emailSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(email->m_message, value);
		printf("%s = %s\n", tag, email->m_message);
	}
}

void GetCameraInfo(Camera *camera)
{
	char tag[1024];
	char value[1024];

	CX_VIDEO p_video = camera->m_video;
	char* configPath = camera->m_configPath;
	int cameraindex = camera->m_cameraindex;

	printf("\nCamera info for camera %d\n", cameraindex);

	// Open the camera xml file
	TiXmlDocument config(configPath);
	if (!config.LoadFile())
	{
		printf("Could not load file %s\n", configPath);
		return;
	}

	TiXmlElement *root = config.FirstChildElement();
	if (!root)
	{
		printf("Could not obtain SOAP-ENV:Envelope tag\n");
		return;
	}

	strcpy(tag, "cameraConfiguration");
	TiXmlElement *cameraConfiguration = root->FirstChildElement(tag);
	if (!cameraConfiguration)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	// Move down to record number for this camera
	strcpy(tag, "recordNumber");
	TiXmlElement *recordNumber = cameraConfiguration->FirstChildElement(tag);
	if (!recordNumber)
	{
		printf("Could not obtain first %s\n", tag);
		return;
	}
	for (int i = 1; i < cameraindex; i++)
	{
		recordNumber = recordNumber->NextSiblingElement(tag);
		if (!recordNumber)
		{
			printf("Could not obtain %d-th %s\n", i + 1, tag);
			return;
		}
	}

	// Make sure camera index in xml matches camera here
	strcpy(tag, "cameraIndex");
	if (!GetValueOfTag(recordNumber, tag, value))
	{
		printf("Could not get %s\n", tag);
		return;
	}
	else
	{
		int index = atoi(value);
		printf("camera index in config file: %d\n", index);
		if (index != cameraindex)
		{
			printf("Index %d does not match\n", index);
		}
	}

	// Get data settings tag for this camera index
	TiXmlElement *cameraMetaDataSettings = recordNumber->FirstChildElement("cameraMetaDataSettings");
	if (!cameraMetaDataSettings)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	// Get camera data

	strcpy(tag, "location");
	if (!GetValueOfTag(cameraMetaDataSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(camera->m_location, value);
		printf("%s = %s\n", tag, camera->m_location);
	}

	strcpy(tag, "CameraID");
	if (!GetValueOfTag(cameraMetaDataSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(camera->m_cameraid, value);
		printf("Camera %d: %s = %s\n", cameraindex, tag, camera->m_cameraid);
	}

	strcpy(tag, "URL");
	if (!GetValueOfTag(cameraMetaDataSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(camera->m_url, value);
		printf("%s = %s\n", tag, camera->m_url);
	}
}

void GetControlSettings(Camera *camera)
{
	char msg[1024];

	char tag[1024];
	char value[1024];

	CX_VIDEO p_video = camera->m_video;
	char* configPath = camera->m_configPath;
	int cameraindex = camera->m_cameraindex;

	printf("\nControl settings for camera %d\n", cameraindex);

	// Open the camera xml file
	TiXmlDocument config(configPath);
	if (!config.LoadFile())
	{
		printf("Could not load file %s\n", configPath);
		return;
	}

	TiXmlElement *root = config.FirstChildElement();
	if (!root)
	{
		printf("Could not obtain SOAP-ENV:Envelope\n");
		return;
	}

	strcpy(tag, "cameraConfiguration");
	TiXmlElement *cameraConfiguration = root->FirstChildElement(tag);
	if (!cameraConfiguration)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	// Move down to record number for this camera
	strcpy(tag, "recordNumber");
	TiXmlElement *recordNumber = cameraConfiguration->FirstChildElement(tag);
	if (!recordNumber)
	{
		printf("Could not obtain first %s\n", tag);
		return;
	}
	for (int i = 1; i < cameraindex; i++)
	{
		recordNumber = recordNumber->NextSiblingElement(tag);
		if (!recordNumber)
		{
			printf("Could not obtain %d-th %s\n", i + 1, tag);
			return;
		}
	}

	// Make sure camera index in xml matches camera here
	strcpy(tag, "cameraIndex");
	if (!GetValueOfTag(recordNumber, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		int index = atoi(value);
		if (index != cameraindex)
		{
			printf("Index %d does not match\n", index);
		}
	}

	// Control settings

	TiXmlElement *controlSettings = recordNumber->FirstChildElement("videoStreamControlSettings");
	if (!controlSettings)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	int version;
	float face_detect_top;
	float face_detect_left;
	float face_detect_height;
	float face_detect_width;
	float face_detect_min_height_prop;
	float face_detect_max_height_prop;
	int frame_interval;
	int reduce_frame_step;
	bool generate_templates;
	bool perform_ranking;
	float verification_thresh;

	strcpy(tag, "version");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		version = atoi(value);
		printf("%s = %d\n", tag, version);
	}

	strcpy(tag, "face_detect_top");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_top = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_top);
	}

	strcpy(tag, "face_detect_left");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_left = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_left);
	}

	strcpy(tag, "face_detect_height");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_height = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_height);
	}

	strcpy(tag, "face_detect_width");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_width = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_width);
	}

	strcpy(tag, "face_detect_min_height_prop");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_min_height_prop = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_min_height_prop);
	}

	strcpy(tag, "face_detect_max_height_prop");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		face_detect_max_height_prop = (float)atof(value);
		printf("%s = %f\n", tag, face_detect_max_height_prop);
	}

	strcpy(tag, "frame_interval");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		frame_interval = atoi(value);
		printf("%s = %d\n", tag, frame_interval);
	}

	strcpy(tag, "reduce_frame_step");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		reduce_frame_step = atoi(value);
		printf("%s = %d\n", tag, reduce_frame_step);
	}

	strcpy(tag, "generate_templates");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		generate_templates = WordToBool(value);
		printf("%s = %d\n", tag, generate_templates);
	}

	strcpy(tag, "perform_ranking");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		perform_ranking = WordToBool(value);
		printf("%s = %d\n", tag, perform_ranking);
	}

	strcpy(tag, "verification_thresh");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		verification_thresh = (float)atof(value);
		printf("%s = %f\n", tag, verification_thresh);
	}

	// Assign the settings
	if (!CX_SetDetectionParameters(p_video, face_detect_top, face_detect_left, face_detect_height, face_detect_width, face_detect_min_height_prop, face_detect_max_height_prop, msg))
	{
		printf("FAILED to set detection parameters:\n%s\n\n", msg);
	}
	if (0 == CX_SetFrameInterval(p_video, frame_interval, msg))
	{
		printf("FAILED to set frame interval:\n%s\n\n", msg);
	}
	if (0 == CX_SetFrameReductionStep(p_video, reduce_frame_step, msg))
	{
		printf("FAILED to set frame reductions step:\n%s\n\n", msg);
	}
	if (0 == CX_SetGenerateTemplatesFlag(p_video, generate_templates, msg))
	{
		printf("FAILED to set generate templates flag:\n%s\n\n", msg);
	}
	if (0 == CX_SetPerformGalleryFRFlag(p_video, perform_ranking, msg))
	{
		printf("FAILED to set perform ranking flag:\n%s\n\n", msg);
	}
	/*if (0 == CX_SetVerificationThreshold(p_video, verification_thresh, msg))
	{
		printf("FAILED to set verification threshold for camera %d:\n%s\n\n", cameraindex, msg);
	}*/
}

void GetResultsSettings(Camera *camera)
{
	char msg[1024];

	char tag[1024];
	char value[1024];

	CX_VIDEO p_video = camera->m_video;
	char* configPath = camera->m_configPath;
	int cameraindex = camera->m_cameraindex;

	printf("\nResults settings for camera %d\n", cameraindex);

	// Open the camera xml file
	TiXmlDocument config(configPath);
	if (!config.LoadFile())
	{
		printf("Could not load file %s\n", configPath);
		return;
	}

	TiXmlElement *root = config.FirstChildElement();
	if (!root)
	{
		printf("Could not obtain SOAP-ENV:Envelope\n");
		return;
	}

	strcpy(tag, "cameraConfiguration");
	TiXmlElement *cameraConfiguration = root->FirstChildElement(tag);
	if (!cameraConfiguration)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	// Move down to record number for this camera
	strcpy(tag, "recordNumber");
	TiXmlElement *recordNumber = cameraConfiguration->FirstChildElement(tag);
	if (!recordNumber)
	{
		printf(" Could not obtain first %s\n", tag);
		return;
	}
	for (int i = 1; i < cameraindex; i++)
	{
		recordNumber = recordNumber->NextSiblingElement(tag);
		if (!recordNumber)
		{
			printf("Could not obtain %d-th %s\n", i + 1, tag);
			return;
		}
	}

	// Make sure camera index in xml matches camera here
	strcpy(tag, "cameraIndex");
	if (!GetValueOfTag(recordNumber, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		int index = atoi(value);
		if (index != cameraindex)
		{
			printf("Index %d does not match\n", index);
		}
	}

	// Results settings

	strcpy(tag, "videoStreamResultsSettings");
	TiXmlElement *controlSettings = recordNumber->FirstChildElement("videoStreamResultsSettings");
	if (!controlSettings)
	{
		printf("Could not obtain %s\n", tag);
		return;
	}

	CX_ResultsSettings res;

	strcpy(tag, "save_xml_results");
	if (!GetValueOfTag(controlSettings, "save_xml_results", value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_save_xml_results = WordToBool(value);
		printf("%s = %d\n", tag, res.m_save_xml_results);
	}

	strcpy(tag, "save_xml_folder");
	if (!GetValueOfTag(controlSettings, "save_xml_folder", value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_save_xml_folder, value);
		strcpy(camera->m_save_xml_folder, res.m_save_xml_folder);
		printf("%s = %s\n", tag, res.m_save_xml_folder);
	}

	strcpy(tag, "remove_files");
	if (!GetValueOfTag(controlSettings, "remove_files", value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_remove_files = WordToBool(value);
		printf("%s = %d\n", tag, res.m_remove_files);
	}

	strcpy(tag, "post_xml");
	if (!GetValueOfTag(controlSettings, "post_xml", value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_post_xml = WordToBool(value);
		printf("%s = %d\n", tag, res.m_post_xml);
	}

	strcpy(tag, "post_url");
	if (!GetValueOfTag(controlSettings, "post_url", value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_post_url, value);
		printf("%s = %s\n", tag, res.m_post_url);
	}

	strcpy(tag, "save_images");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_save_images = WordToBool(value);
		printf("%s = %d\n", tag, res.m_save_images);
	}

	strcpy(tag, "save_images_folder");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_save_images_folder, value);
		printf("%s = %s\n", tag, res.m_save_images_folder);
	}

	strcpy(tag, "save_only_verified");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_save_only_verified = WordToBool(value);
		printf("%s = %d\n", tag, res.m_save_only_verified);
	}

	strcpy(tag, "root_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_root_tag, value);
		printf("%s = %s\n", tag, res.m_root_tag);
	}

	strcpy(tag, "person_name");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_person_name = WordToBool(value);
		printf("%s = %d\n", tag, res.m_person_name);
	}

	strcpy(tag, "person_name_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_person_name_tag, value);
		printf("%s = %s\n", tag, res.m_person_name_tag);
	}

	strcpy(tag, "person_id");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_person_id = WordToBool(value);
		printf("%s = %d\n", tag, res.m_person_id);
	}

	strcpy(tag, "person_id_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_person_id_tag, value);
		printf("%s = %s\n", tag, res.m_person_id_tag);
	}

	strcpy(tag, "processed_frames");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_processed_frames = WordToBool(value);
		printf("%s = %d\n", tag, res.m_processed_frames);
	}

	strcpy(tag, "processed_frames_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_processed_frames_tag, value);
		printf("%s = %s\n", tag, res.m_processed_frames_tag);
	}

	strcpy(tag, "head_id");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_head_id = WordToBool(value);
		printf("%s = %d\n", tag, res.m_head_id);
	}

	strcpy(tag, "head_id_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_head_id_tag, value);
		printf("%s = %s\n", tag, res.m_head_id_tag);
	}

	strcpy(tag, "stream_type");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_stream_type = WordToBool(value);
		printf("%s = %d\n", tag, res.m_stream_type);
	}

	strcpy(tag, "stream_type_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_stream_type_tag, value);
		printf("%s = %s\n", tag, res.m_stream_type_tag);
	}

	strcpy(tag, "stream_connection_info");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_stream_connection_info = WordToBool(value);
		printf("%s = %d\n", tag, res.m_stream_connection_info);
	}

	strcpy(tag, "stream_connection_info_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_stream_connection_info_tag, value);
		printf("%s = %s\n", tag, res.m_stream_connection_info_tag);
	}

	strcpy(tag, "stream_index");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_stream_index = WordToBool(value);
		printf("%s = %d\n", tag, res.m_stream_index);
	}

	strcpy(tag, "stream_index_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_stream_index_tag, value);
		printf("%s = %s\n", tag, res.m_stream_index_tag);
	}

	strcpy(tag, "verification_threshold");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_verification_threshold = WordToBool(value);
		printf("%s = %d\n", tag, res.m_verification_threshold);
	}

	strcpy(tag, "verification_threshold_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_verification_threshold_tag, value);
		printf("%s = %s\n", tag, res.m_verification_threshold_tag);
	}

	strcpy(tag, "frame_number");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_frame_number = WordToBool(value);
		printf("%s = %d\n", tag, res.m_frame_number);
	}

	strcpy(tag, "frame_number_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_frame_number_tag, value);
		printf("%s = %s\n", tag, res.m_frame_number_tag);
	}

	strcpy(tag, "utc_time");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_utc_time = WordToBool(value);
		printf("%s = %d\n", tag, res.m_utc_time);
	}

	strcpy(tag, "utc_time_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_utc_time_tag, value);
		printf("%s = %s\n", tag, res.m_utc_time_tag);
	}

	strcpy(tag, "ranked_results");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_ranked_results = WordToBool(value);
		printf("%s = %d\n", tag, res.m_ranked_results);
	}

	strcpy(tag, "ranked_results_n");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_ranked_results_n = atoi(value);
		printf("%s = %d\n", tag, res.m_ranked_results_n);
	}

	strcpy(tag, "ranked_results_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_ranked_results_tag, value);
		printf("%s = %s\n", tag, res.m_ranked_results_tag);
	}

	strcpy(tag, "matched_person_name");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_person_name = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_person_name);
	}

	strcpy(tag, "matched_person_name_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_person_name_tag, value);
		printf("%s = %s\n", tag, res.m_matched_person_name_tag);
	}

	strcpy(tag, "matched_person_id");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_person_id = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_person_id);
	}

	strcpy(tag, "matched_person_id_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_person_id_tag, value);
		printf("%s = %s\n", tag, res.m_matched_person_id_tag);
	}

	strcpy(tag, "matched_image_id");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_image_id = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_image_id);
	}

	strcpy(tag, "matched_image_id_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_image_id_tag, value);
		printf("%s = %s\n", tag, res.m_matched_image_id_tag);
	}

	strcpy(tag, "date_time_stamp");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_date_time_stamp = WordToBool(value);
		printf("%s = %d\n", tag, res.m_date_time_stamp);
	}

	strcpy(tag, "date_time_stamp_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_date_time_stamp_tag, value);
		printf("%s = %s\n", tag, res.m_date_time_stamp_tag);
	}

	strcpy(tag, "confidence_measure");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_confidence_measure = WordToBool(value);
		printf("%s = %d\n", tag, res.m_confidence_measure);
	}

	strcpy(tag, "confidence_measure_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_confidence_measure_tag, value);
		printf("%s = %s\n", tag, res.m_confidence_measure_tag);
	}

	strcpy(tag, "focus_measure");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_focus_measure = WordToBool(value);
		printf("%s = %d\n", tag, res.m_focus_measure);
	}

	strcpy(tag, "focus_measure_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_focus_measure_tag, value);
		printf("%s = %s\n", tag, res.m_focus_measure_tag);
	}

	strcpy(tag, "eye_positions");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_eye_positions = WordToBool(value);
		printf("%s = %d\n", tag, res.m_eye_positions);
	}

	strcpy(tag, "eye_positions_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_eye_positions_tag, value);
		printf("%s = %s\n", tag, res.m_eye_positions_tag);
	}

	strcpy(tag, "face_box");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_face_box = WordToBool(value);
		printf("%s = %d\n", tag, res.m_face_box);
	}

	strcpy(tag, "face_box_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_face_box_tag, value);
		printf("%s = %s\n", tag, res.m_face_box_tag);
	}

	strcpy(tag, "match_score");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_match_score = WordToBool(value);
		printf("%s = %d\n", tag, res.m_match_score);
	}

	strcpy(tag, "match_score_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_match_score_tag, value);
		printf("%s = %s\n", tag, res.m_match_score_tag);
	}

	strcpy(tag, "matched_status");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_status = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_status);
	}

	strcpy(tag, "matched_status_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_status_tag, value);
		printf("%s = %s\n", tag, res.m_matched_status_tag);
	}

	strcpy(tag, "tracked_image");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_tracked_image = WordToBool(value);
		printf("%s = %d\n", tag, res.m_tracked_image);
	}

	strcpy(tag, "tracked_image_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_tracked_image_tag, value);
		printf("%s = %s\n", tag, res.m_tracked_image_tag);
	}

	strcpy(tag, "matched_person_thumbnail");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_person_thumbnail = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_person_thumbnail);
	}

	strcpy(tag, "matched_person_thumbnail_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_person_thumbnail_tag, value);
		printf("%s = %s\n", tag, res.m_matched_person_thumbnail_tag);
	}

	strcpy(tag, "matched_thumbnail");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		res.m_matched_thumbnail = WordToBool(value);
		printf("%s = %d\n", tag, res.m_matched_thumbnail);
	}

	strcpy(tag, "matched_thumbnail_tag");
	if (!GetValueOfTag(controlSettings, tag, value))
	{
		printf("Could not get %s\n", tag);
	}
	else
	{
		strcpy(res.m_matched_thumbnail_tag, value);
		printf("%s = %s\n", tag, res.m_matched_thumbnail_tag);
	}

	// Add camera id tag
	if (0 == CX_AddUserTagToResults(p_video, "CameraID", camera->m_cameraid, msg))
	{
		printf("Failed to set camera id tag\n%s\n", msg);
	}

	// Set results parameters
	if (0 == CX_SetALLResultsParameters(p_video, res, msg))
	{
		printf("Failed to set results params\n%s\n", msg);
	}
}

bool AddCameraIDToXml(Camera* camera, const char* xmlName)
{
	char* cameraID = camera->m_cameraid;
	char* save_xml_folder = camera->m_save_xml_folder;
	char xmlPath[1024];
	strcpy(xmlPath, save_xml_folder);
	strcat(xmlPath, xmlName);

	TiXmlDocument imageXml(xmlPath);
	if (!imageXml.LoadFile())
	{
		printf("Could not load image xml: %s\n", xmlPath);
		return false;
	}

	TiXmlElement *envelope = imageXml.FirstChildElement("SOAP-ENV:Envelope");
	if (!envelope)
	{
		printf("Could not obtain SOAP-ENV:Envelope tag of image xml: %s\n", xmlPath);
		return false;
	}

	TiXmlElement *body = envelope->FirstChildElement("SOAP-ENV:Body");
	if (!body)
	{
		printf("Could not obtain SOAP-ENV:Body tag of image xml: %s\n", xmlPath);
		return false;
	}

	TiXmlElement *CustomerInfoRequest = body->FirstChildElement("CustomerInfoRequest");
	if (!CustomerInfoRequest)
	{
		printf("Could not obtain CustomerInfoRequest tag of image xml: %s\n", xmlPath);
		return false;
	}

	// Insert CameraID into xml
	TiXmlElement *cameraIDElement = new TiXmlElement("CameraID");
	TiXmlText *cameraIDText = new TiXmlText(cameraID);
	cameraIDElement->LinkEndChild(cameraIDText);
	CustomerInfoRequest->LinkEndChild(cameraIDElement);

	char newXmlPath[1024];
	strcpy(newXmlPath, save_xml_folder);
	char newXmlName[1024];
	strcpy(newXmlName, "id_");
	strcat(newXmlName, xmlName);
	strcat(newXmlPath, newXmlName);

	// Save new document
	if (!imageXml.SaveFile(newXmlPath))
	{
		printf("Unable to save new xml with camera id from image xml: %s\n", xmlPath);
		return false;
	}

	// Delete original xml if successful
	remove(xmlPath);

	return true;
}

bool AddCameraIDToXmls(Camera* camera)
{
	char* cameraID = camera->m_cameraid;
	char* save_xml_folder = camera->m_save_xml_folder;

	// Replace forward slashes with backward slashes
	char save_xml_folder_backslash[1024];
	int i = 0;
	int j = 0;
	while (save_xml_folder[i] != '\0')
	{
		if (save_xml_folder[i] == '/')
		{
			// Since \ is escape character, need to escape it
			save_xml_folder_backslash[j] = '\\';
			j++;
			i++;
		}
		else
		{
			save_xml_folder_backslash[j++] = save_xml_folder[i++];
		}
		save_xml_folder_backslash[j] = '\0';
	}

	// The command will create a file containing a list of the xmls
	char xmlsListPath[1024];
	strcpy(xmlsListPath, save_xml_folder);
	strcat(xmlsListPath, "xmls_without_cameraid.txt");
	char xmlsListPath_backslash[1024];
	strcpy(xmlsListPath_backslash, save_xml_folder_backslash);
	strcat(xmlsListPath_backslash, "xmls_without_cameraid.txt");

	// Call the command to create the list of xmls
	char command[1024];
	strcpy(command, "dir /b ");
	strcat(command, save_xml_folder_backslash);
	strcat(command, "Stream* > ");
	strcat(command, xmlsListPath_backslash);
	system(command);

	// Now iterate through the file names
	ifstream xmlsList;
	xmlsList.open(xmlsListPath);
	if (!xmlsList)
	{
		printf("Could not open list of xmls: %s\n", xmlsListPath);
		return false;
	}
	string xmlName;
	while (xmlsList >> xmlName)
	{
		// Add camera id to this one file
		if (!AddCameraIDToXml(camera, xmlName.c_str()))
		{
			printf("Could not add camera id to xml %s in %s\n", xmlName.c_str(), save_xml_folder);
			return false;
		}
	}

	// Close the file so it can be deleted
	xmlsList.close();

	// Delete the list
	sprintf(command, "del %s", xmlsListPath_backslash);
	system(command);

	return true;
}
