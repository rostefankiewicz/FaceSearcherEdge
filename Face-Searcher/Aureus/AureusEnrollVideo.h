////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_VIDEO_ENROLL_H_
#define AUREUS_VIDEO_ENROLL_H_
////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// include main Aureus header
#include "Aureus.h"
#include "AureusVideo.h"
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// Aureus provides a CX_ENROLL_VIDEO object which can be used for live enrollment
// from a video stream (usually a camera). Please not that whilst you can set up and 
// run a CX_ENROLL_VIDEO it is unadvisable to do so unless you are alos implementing 
// some form of GUI feedback as the CX_ENROLL_VIDEO object will be looking for people
// in the stream to enroll and will wait (potentially for ever) if a person's head
// is not suitably positioned in the frame.
//
// An CX_ENROLL_VIDEO provides captioned feedback and an oval directly on the output
// callback frame. A user must place there head such that it fills the oval. The oval
// will then move to a different position and the process is repeated for 3 positions.
// After successfully capturing 3 positions Aureus will enroll the person into the
// gallery as a new person.
//
// CX_ENROLL_VIDEO will not work unless you have switched on the template generation
// and perform gallery FR flags. You also need an FR license.
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_CreateEnrollVideo
// 
// Description:   Call this function to create an Aureus Enroll Video Object. 
//                An Enroll Video object facilitates Live Enrollment from a video camera.
//
// Parameters:		[in] p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL if the upon failure (e.g. NULL parameters) check 
//                msg for more details
//                Returns a CX_ENROLL_VIDEO pointer upon success, you will need this pointer for
//                all subsequent enroll video processing calls (below).
//
//
// NOTE: Do NOT free/delete the returned pointer.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_ENROLL_VIDEO CXAPI CX_CreateEnrollVideo(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_StartEnrollmentStream
// 
// Description:   Call this function to begin live enrollment.
//                There are 2 types of enrollment video stream: a USB camera and IP camera.
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateEnrollVideo
//            		[in] stream_type  The stream type: 1 = USB, 2 = IP camera
//                [in] connection_info The stream connection information,
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
// Stream type and connection info:
//
// 1 = stream_type USB, connection info = "0" or "1" etc, for USB pin number
// 2 = stream_type IP camera, connection info = the IP camera's url, with all login & pwd info
//
// Note: to obtain the processed results you must set a frame callback, explained below.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_StartEnrollmentStream(CX_ENROLL_VIDEO p_video, cx_uint stream_type, const char* connection_info, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_StopEnrollmentStream
// 
// Description:   Call this function to stop processing an enrollment video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateEnrollVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_StopEnrollmentStream(CX_ENROLL_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_PauseEnrollmentStream
// 
// Description:   Call this function to pause processing an enrollment video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateEnrollVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_PauseEnrollmentStream(CX_ENROLL_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_UnPauseEnrollmentStream
// 
// Description:   Call this function to un-pause a previously paused enrollment video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateEnrollVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                Errors can be: p_video was incorrect/missing, or the enrollment video stream
//                was not previously paused.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_UnPauseEnrollmentStream(CX_ENROLL_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Type:					CX_ENROLL_FRAME_CALLBACK
// 
// Parameters:		[in]  p_pixels       A pointer to the frame pixels (always RGBA, always origin bottom left, always row major)
//                [in]  rows           The number of rows in the frame
//                [in]  cols           The number of columns in the frame
//                [in]  status         if 0 then enrollment is still processing, if 1 then it has finished, if -1 there was an error
//                [in]  err_msg        A string containing the error message if one occurs
//								[in]	p_object			 A pointer to your object that your function can operate on/use, or NULL
// 
// Description:		signature of the enroll frame callback function that will provide the frames
//                for a live enroll
// 
//                The frame image denoted by p_pixels, rows, cols will always be an RGBA
//                image with the origin at the bottom left and will always be row major
//
//                Check the status flag in your handler, if set to 1 then there has
//                been a successful enrollment, if you have obtained DB info (e.g. from
//                a call to CX_GetNumGalleryPeople()) then you will need to call the appropriate
//                functions again to ensure your app is up-to-date and in sync with Aureus.
//                If enrollment is still processing then status will be zero.
//                If Aureus encounted and error (e.g. the collected images failed to enroll)
//                then status will be set to -1 and err_msg will contain the error message.
//
// NOTE: The parameters passed into the callback via this definition will be valid
// until the callback returns. Do not free/delete err_msg or p_pixels.
//
//////////////////////////////////////////////////////////////////////////////////////
typedef void(*CX_ENROLL_FRAME_CALLBACK) (
  cx_byte*      p_pixels,
  cx_uint       rows,
  cx_uint       cols,
  cx_uint       frame_num,
  cx_int        status,
  const char*   err_msg,
  void*         p_object);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetEnrollFrameCallBack
// 
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateEnrollVideo
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		The Aureus SDK provides a live enrollment video stream.
//                You need to display/use the resulting images and results to do so you 
//                need to set a callback function. Use this function to register your output 
//                callback function. 
//                The parameters of the call back function are described above.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetEnrollFrameCallBack(CX_ENROLL_VIDEO p_video, CX_ENROLL_FRAME_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetEnrollStreamTerminatedCallBack
// 
// Parameters:		[in] p_video  a pointer to the enroll video object created by CX_CreateEnrollVideo
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		The Aureus SDK allows live enrollment via a video stream, it is possible
//                that the video stream may unexpectedly terminate. If you wish to capture
//                this information in your code you must register a callback using this
//                function
//                The parameters of the call back function are described in AureusVideo.h
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetEnrollStreamTerminatedCallBack(CX_ENROLL_VIDEO p_video, CX_STREAM_TERMINATED_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


#endif // AUREUS_VIDEO_ENROLL_H_
