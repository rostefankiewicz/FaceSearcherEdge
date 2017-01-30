////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_VIDEO_H_
#define AUREUS_VIDEO_H_
////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// include main Aureus header
#include "Aureus.h"
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_CreateVideo
// 
// Description:   Call this function to create an Aureus Video Object. All video processing
//                requires an Aureus Video object. Since Aureus allows multiple video stream 
//                processing you may create as many video objects as you are licensed for.
//
// Parameters:		[in] p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL if the upon failure (e.g. NULL parameters or you have reached 
//                limit of video objects allowed by your license) check 
//                msg for more details
//                Returns a CX_VIDEO pointer upon success, you will need this pointer for
//                all subsequent video processing calls (below).
//
//
// NOTE: Do NOT free/delete the returned pointer.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_VIDEO CXAPI CX_CreateVideo(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetVideoVersion
// 
// Description:   Call this function to obtain version of the Aureus Video decoding. 
//
// Parameters:		[in] p_video  a pointer to the CX_VIDEO object created by CX_CreateVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 if the upon failure (e.g. NULL parameters ) check 
//                msg for more details
//                Returns a 1 upon success, msg will contain the video version
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetVideoVersion(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////////////////
// The following 3 functions facilitate sequential frame processing. They allow you
// to pass in each frame and obtain the processed output
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_DetectHeads
// 
// Description:   Call this function to detect heads for an image frame. The
//                detection parameters set with CX_SetDetectionParameters() will be used.
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_im   An image pointer, NULL will return and error
//                [in] frame_number, the current frame number in the image sequence (required for tracking)
//                [in/out] p_head_info_array an array that will contain the detection results on return
//                [in] n_array_size size of p_head_info_array
//                [in] use_face_detector, if 1 then only face detection will occurr otherwise head followed by face detection will occurr
//                [in] use_fail_safe, if 1 (and use_face_detector=1) then a slower fail safe face detector will be used if no faces are detected
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. NULL parameters, or caught an
//                exception reading pixels in image or copying results to p_head_info_array) check 
//                msg for more details
//
//                Returns -2 if more heads were detected than n_array_size would 
//                contain, in this case p_head_info_array is still valid but contains a
//                n_array_size detected heads
//
//                Returns the number of detected heads (can be zero), the head detection info
//                will be contained in p_head_info_array.
//
// Note: Aureus is optimised for processing video streams at speed via the use of head (and shoulders)
// and face detection rather than just face detection. This enables a significant speed advantage in many
// scenarios since a head & shoulders is larger than a face. However, depending on your frames you
// may wish to use only face detection; if so set use_face_detector to 1. This usually results
// in better detection for difficult images with a speed trade off. Also note that if you set this
// flag you will need to adjust the detection parameters (CX_SetDetectionParameters()) to enable
// Aureus to look for faces at a smaller minimum height proportion (since faces are smaller).
// 
// Note: If you set use_face_detector=1 you might also want to set use_fail_safe=1. If set, then
// Aureus will apply a slower fail safe face detector is no faces are found. This results in yet 
// more robust detection for difficult images, however is is significantly slower.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_DetectHeads(CX_VIDEO p_video, CX_RAM_Image* p_im, int frame_number,
  CX_HeadInfo* p_head_info_array, int n_array_size, int use_face_detector, int use_fail_safe, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ProcessFrameFromDetectedHeads
// 
// Description:   Call this function to track heads in the given image frame using
//                the previously detected heads (obtained with CX_DetectHeads() above). .
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_im   An image pointer, NULL will return and error
//                [in] frame_number the current frame number in the image sequence (required for tracking)
//                [in] p_head_info_array an array that contains the detected heads from CX_DetectHeads()
//                [in] n_heads number of heads in array (NOT the array size)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. NULL parameters, or caught an
//                exception reading pixels in image, or head array is corrupted) check msg
//                for more details
//
//                Returns a pointer to a Head List upon success: Do NOT free the returned pointer.
//
// NOTE:          The returned CX_HEAD_LIST and ALL it's internal variables are valid until 
//                this function (or the generic ProcessFrame) is called again. If you use a 
//                previously returned CX_HEAD_LIST the behaviour is undefined.
//
// NOTE:          Please also review the use_face_detector parameter in CX_DetectHeads() above.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD_LIST CXAPI CX_ProcessFrameFromDetectedHeads(CX_VIDEO p_video,
  CX_RAM_Image* p_im, int frame_number, CX_HeadInfo* p_head_info_array, int n_heads, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ProcessFrame
// 
// Description:   Call this function to detect and track heads for an image frame. The
//                detection parameters set with CX_SetDetectionParameters() will be used.
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_im   An image pointer, NULL will return and error
//                [in] frame_number the current frame number in the image sequence (required for tracking)
//                [in] use_face_detector, if 1 then only face detection will occurr otherwise head followed by face detection will occurr
//                [in] use_fail_safe, if 1 (and use_face_detector=1) then a slower fail safe face detector will be used if no faces are detected
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. NULL parameters, or caught an
//                exception reading pixels in image) check msg for more details
//
//                Returns a pointer to a Head List upon success: Do NOT free the returned pointer.
//
// NOTE:          The returned CX_HEAD_LIST and ALL it's internal variables are valid until 
//                this function is called again. If you use the a previously returned CX_HEAD_LIST
//                the behaviour is undefined.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD_LIST CXAPI CX_ProcessFrame(CX_VIDEO p_video,
  CX_RAM_Image* p_im, int frame_number, int use_face_detector, int use_fail_safe, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ResetProcessing
// 
// Description:   Call this function to reset the processing for this video object.
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. NULL parameters, or incorrect p_video parameter)
//                check msg for more details
//                Return 1 on success
//
// NOTE:          Before processing any sequential image set you should call this
//                function so that Aureus does not assume that the next set of sequential
//                images are a continuation of a previous set.
//                For connections to video streams (see below) this is called automatically.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ResetProcessing(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// These functions allow you to connect Aureus to a video stream and process it.
// You obtain the proccessed results via a callback function which you must set
// to use the results.
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ProcessStream
// 
// Description:   Call this function to process a video stream. The detection parameters 
//                set with CX_SetDetectionParameters() will be used.
//                There are 3 types of video stream, Media File, USB camera and IP camera.                  
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] stream_type  The stream type: 0= File, 1 = USB, 2 = IP camera
//                [in] connection_info The stream connection information,
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
// Stream type and connection info:
//
// 0 = stream_type File, connection info = full path and file name of media file
// 1 = stream_type USB, connection info = "0" or "1" etc, for USB pin number
// 2 = stream_type IP camera, connection info = the IP camera's url, with all login & pwd info
//
// Note: to obtain the processed results you must set a frame callback, explained below.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ProcessStream(CX_VIDEO p_video, cx_uint stream_type, const char* connection_info, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_StopStream
// 
// Description:   Call this function to stop processing a video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_StopStream(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_PauseStream
// 
// Description:   Call this function to pause processing a video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_PauseStream(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_UnPauseStream
// 
// Description:   Call this function to un-pause a previously paused video stream. 
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure check msg for more details, 1 upon sucess.
//                Errors can be: p_video was incorrect/missing, or the video stream
//                was not previously paused.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_UnPauseStream(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Type:					CX_FRAME_CALLBACK
// 
// Parameters:		[in]	p_head_list    A pointer to a head list, do NOT free the pointer
//                [in]  head_list_size The number of heads in the head list
//                [in]  p_pixels       A pointer to the frame pixels (always RGBA, always origin bottom left, always row major)
//                [in]  rows           The number of rows in the frame
//                [in]  cols           The number of columns in the frame
//								[in]	p_object			 A pointer to your object that your function can operate on, or NULL
// 
// Description:		signature of the frame callback function that will provide processed head data
// 
//                The frame image denoted by p_pixels, rows, cols will always be an RGBA
//                image with the origin at the bottom left and will always be row major
//
// NOTE: The parameters passed into the callback via this definition will be valid
// until the callback returns.
//////////////////////////////////////////////////////////////////////////////////////
typedef void(*CX_FRAME_CALLBACK) (CX_HEAD_LIST  p_head_list,
  cx_uint       head_list_size,
  cx_byte*      p_pixels,
  cx_uint       rows,
  cx_uint       cols,
  cx_uint       frame_num,
  void*         p_object);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetFrameCallBack
// 
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		The Aureus SDK allows the processing of video streams.
//                If you wish to display/use the resulting images and results you need 
//                to set a callback function. Use this function to register your output 
//                callback function. 
//                The parameters of the call back function are described above.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetFrameCallBack(CX_VIDEO p_video, CX_FRAME_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Type:					CX_STREAM_TERMINATED_CALLBACK
// 
// Parameters:		[in]	stream_type     The stream type, 0 = media file, 1 = USB, 2 = IP
//                [in]  connection_info The connection info (e.g. "0" for USB pin zero, or "C:/Video.wmv" for media file etc.)
//								[in]	p_object			 A pointer to your object that your function can use, or NULL
// 
// Description:		signature of the stream termination callback function that will be called if the 
//                stream unexpectedly terminate (e.g. the camera loses power).
// 
//
// NOTE: The parameters passed into the callback via this definition will be valid
// until the callback returns.
// This callback will only occur if the stream terminates unexpectedly, e.g. a camera
// connection or poer being lost. Whn called it will provide the information of the
// stream type and connection. This will allows you to debug/restart the connection.
//
//////////////////////////////////////////////////////////////////////////////////////
typedef void(*CX_STREAM_TERMINATED_CALLBACK) (cx_int stream_type, const char* connection_info, void* p_object);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetStreamTerminatedCallBack
// 
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		The Aureus SDK allows the processing of video streams, it is possible
//                that the video stream may unexpectedly terminate. If you wish to capture
//                this information in your code you must register a callback using this
//                function
//                The parameters of the call back function are described above.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetStreamTerminatedCallBack(CX_VIDEO p_video, CX_STREAM_TERMINATED_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Type:					CX_MEDIA_FILE_FINISHED_CALLBACK
// 
// Parameters:		[in]	frame_num     The current (last) frame number
//								[in]	p_object			A pointer to your object that your function can use, or NULL
// 
// Description:		signature of the media file finished callback function that will be called when
//                the media file stream has finished processing.
// 
// Notes: If you are processing a media file you may wish to programatically know when
//        the processing has finished. If you set this callback it will be called when the
//        stream processing has finished. Note, the callback may not happen imediately upon
//        processing the last frame, however it will called within 0.5 seconds.
//
//////////////////////////////////////////////////////////////////////////////////////
typedef void(*CX_MEDIA_FILE_FINISHED_CALLBACK) (cx_uint frame_num, void* p_object);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetMediaFileFinishedCallBack
// 
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		If you are processing a media file you may wish to programatically know when
//                they processing has finished. If you set this callback it will be called when the
//                stream processing has finished. Note, the callback may not happen imediately upon
//                processing the last frame, however it will called within 0.5 seconds.
//                The parameters of the call back function are described above.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetMediaFileFinishedCallBack(CX_VIDEO p_video, CX_MEDIA_FILE_FINISHED_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetPeopleCount
// 
// Description:   Call this function get the number of active valid heads being processed
//                in the given video object.
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the number active currently valid heads in the list upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetPeopleCount(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetFrameProcessTime
// 
// Description:   Call this function get the time Aureus took to process a frame.
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1.0f upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the time in seconds to process a frame upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_GetFrameProcessTime(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetVerificationThreshold
// 
// Description:   Call this function to set the verification threshold for p_video
//
// Parameters:		[in] p_video the CX_VIDEO for which to set the verification_threshold
//                [in] verification_threshold the verification threshold value
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: The verification threshold is only relevent if you are performing FR and saving the
// results, in which case it will be used to mark them as verified/identified or not.
// Set a verification threshold between zero and 1, default value is 0.73.
//
// If you set a threshold outside the range 0..1 an error is returned.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetVerificationThreshold(CX_VIDEO p_video, cx_real verification_threshold, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current verification threshold or -1.0f on error
EXTERN_C CXVFRSDK cx_real CXAPI CX_GetVerificationThreshold(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetGenerateTemplatesFlag
// 
// Description:   Call this function to enable/disable template generation for p_video
//
// Parameters:		[in] p_video the CX_VIDEO from which to set the generates templates flag
//                [in] generate_templates, the flag value
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus is modular, it can be used for the following:
// 1) detection and tracking
// 2) Generating FR templates (which for example could then be used for 1:1 FR)
// 3) Performing ranked FR on a gallery (see AureusGallery.h for details)
// 4) Live enrolling either via the separate CX_ENROLL_VIDEO stream object or directly in a CX_VIDEO object
//
// If you wish to manipulate whether this CX_VIDEO generate templates use this function to
// set the controlling flag. Note, if set to false then any subsequent FR e.g. matching
// against a gallery will be automatically turned off since a template must be generated
// to allow matching to be performed.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetGenerateTemplatesFlag(CX_VIDEO p_video, int generate_templates, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current flag status or -1 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetGenerateTemplatesFlag(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetPerformGalleryFRFlag
// 
// Description:   Call this function to enable/disable FR matching to the gallery for p_video
//
// Parameters:		[in] p_video the CX_VIDEO from which to set the perform gallery FR flag
//                [in] perform_gallery_fr the flag value
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus is modular, it can be used for the following:
// 1) detection and tracking
// 2) Generating FR templates (which for example could then be used for 1:1 FR)
// 3) Performing ranked FR on a gallery (see AureusGallery.h for details)
// 4) Live enrolling either via the separate CX_ENROLL_VIDEO stream object or directly in a CX_VIDEO object
//
// If you wish to manipulate whether this CX_VIDEO performs FR matching to the gallery
// use this function to set the controlling flag. Note, you must have the generate templates
// flag switched on since a template must be generated to allow matching to be performed.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetPerformGalleryFRFlag(CX_VIDEO p_video, int perform_gallery_fr, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current flag status or -1 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetPerformGalleryFRFlag(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetFrameInterval
// 
// Description:   Call this function to set the frame interval for a CX_VIDEO
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] frame_interval the desired frame interval
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Setting the frame interval to zero or 1 will cause every single frame of a video
// stream to be processed. Setting the interval to 2 will process every other frame etc.
// In certain situations it is beneficial to set the frame interval higher (e.g. 2 or 3) 
// since Aureus can still detect, track and identify with that interval whilst maintaining
// a faster overall processing speed. Typical scenarios are on a lower powered system with
// a reasobaly high density of heads throughput in the scene.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetFrameInterval(CX_VIDEO p_video, int frame_interval, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -1 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetFrameInterval(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetFrameReductionStep
// 
// Description:   Call this function to set a frame reduction step for a CX_VIDEO
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] frame_reduction_step the desired frame reduction step
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: frame_reduction_step controls the reduction of the frame size (only for detection).
// The Aureus head detections will perform down to a very small resolution (circa 25 
// pixels from the chin to the top of the head). Thus one can increase detection speed by reducing
// the size of the frame, this parameter controls that. If set to -1 (or zero) then Aureus
// will automatically choose a frame reduction step. 
//
// This setting works in conjunction with min_height_prop (in CX_SetDetectionParameters()) to 
// achieve the highest frame rate possible. For example setting min_height_prop to 0.175 for 
// frames with 720 pixels in height will result in a frame step of 3 if Aureus chooses 
// automaticaly (frame_reduction_step = -1)
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetFrameReductionStep(CX_VIDEO p_video, int frame_reduction_step, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -10 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetFrameReductionStep(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetForceEveryFrame
// 
// Description:   Call this function to set the force_every_frame flag for a CX_VIDEO
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] flag the value of the flag (0=false, 1=true)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Behind the scenes Aureus loads images whilst processing continues on a separate
// thread. The end result is a far smoother and faster processing pipeline. However, sometimes
// this can result in a frame being skipped here and there (depending on processing power
// and detection set up and frame size and number of people in the frame). Under certain
// circumstances you may wish to force Aureus to process every single frame, use this
// flag to signal Aureus to do that.
//
// Note that if Aureus is forced to process every frame it effectively waits for the frame
// to be loaded, then processes it. This results in a slower throughput of frames (often
// significantly slower) and for any stream type other than media files it will can be
// significantly slower since the camera will put out frames at a regular frame period and
// Aureus will load them and processe them in one function. If this flag is false then the
// loading and processing happen in parallel hence the processing of camera frames can be
// significantly faster and smoother.
// Setting this flag to true really only has relevance for media files where Aureus can get
// a new frame after finishing processing the previous frame.
//
// When processing media files Aureus can grab and decode frames significantly faster
// than the time it takes to proces them (circa 100 fps), hence when this flag is 
// false (default) Aureus will grab frames at a maximum frame rate of approximately 30 fps 
// regardless of resolution.
//
// Default is false, it is highly recomended that this flag remains false unless you have
// a need for each media file frame to be processed (e.g. performing detection and tracking
// without FR on a set of media files for post processing using FR later).
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetForceEveryFrame(CX_VIDEO p_video, int flag, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -10 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetForceEveryFrame(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetGenerateMeshOverlayFlag
// 
// Description:   Call this function to set the generate mesh overlay flag for a CX_VIDEO
//                If set to true, a call to CX_GetMeshOverlay() will return the mesh
//                overlay for a given tracked head
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] flag the value of the flag (0=false, 1=true)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus is capable of creating mesh overlays for tracked heads, note, this is not
// necessary for the correct functioning of Aureus FR, however you may wish to view/use the 
// overlaid meshes. This will consume more CPU per frame.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetGenerateMeshOverlayFlag(CX_VIDEO p_video, int flag, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -10 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetGenerateMeshOverlayFlag(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetGenerate3DMeshFlag
// 
// Description:   Call this function to set the generate 3D mesh flag for a CX_VIDEO
//                If set to true, a call to CX_Get3DMesh() will return the mesh
//                for a given tracked head
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] flag the value of the flag (0=false, 1=true)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus is capable of creating 3D meshes for tracked heads, note, this is not
// necessary for the correct functioning of Aureus FR, however you may wish to view/use the 
// meshes. This will consume more CPU per frame (also more that a 2D mesh overlay).
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetGenerate3DMeshFlag(CX_VIDEO p_video, int flag, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -10 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetGenerate3DMeshFlag(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetGeneratePoseCorrectionFlag
// 
// Description:   Call this function to set the generate pose correction flag for a CX_VIDEO
//                If set to true, a call to CX_GetPCimage() will return a pose corrected
//                image for a given tracked head
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] flag the value of the flag (0=false, 1=true)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus is capable of creating pose corrected images for tracked heads, note, this is
// not necessary for the correct functioning of Aureus FR, however you may wish to view/use the 
// images. This will consume more CPU per frame (also more that a 3D mesh overlay).
//
// To obtain a pose corrected image, you mush also set the 3D mesh generation flag to true
// using the CX_SetGenerate3DMeshFlag() function above
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetGeneratePoseCorrectionFlag(CX_VIDEO p_video, int flag, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// returns the current value or -10 on error
EXTERN_C CXVFRSDK int CXAPI CX_GetGeneratePoseCorrectionFlag(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetDetectionParameters
// 
// Description:   Call this function to set the head detection parameters.
//                These parameters affect the face detection algorithms in the SDK.
//                The top/left/height/width describe an image window in which to search
//                for faces. The are normalized (0..1) paramters with the origin top left.
//                Smaller windows result in faster detections as long as the heads are 
//                inside the window.
//                Note: if you do not call this the default values (show below) will be used.
//                Note: the setting controlled by this function will remain fixed until
//                you call it again, regardless or processing a new video stream.
// 
//                The min/max proportions limit the sizes of heads/faces to be found, larger
//                values of min_height_prop result in faster detections. 
//
//                A typical set of values might be:
//
//                search whole image
//                top = 0.0
//                left = 0.0
//                height = 1.0
//                width = 1.0
//
//                reasonably small heads
//                min_height_prop = 0.2
//                max_height_prop = 0.8
//
//
// Parameters:		[in] p_video  a pointer to the video object created by CX_CreateVideo
//                Other parameters described above
//            		[in/out] msg a string large enough to contain error messages
//
// 
// Returns:       Returns 0 if the upon failure check msg for more details
//
// Defaults:
//                top = 0.0
//                left = 0.0
//                height = 1.0
//                width = 1.0
//
//                min_height_prop = 0.2
//                max_height_prop = 0.4
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetDetectionParameters(CX_VIDEO p_video,
  float top, float left, float height, float width, float min_height_prop, float max_height_prop,
  char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetDetectionParameters
// 
// Description:   Call this function to get the head detection parameters. You must pass
//                in a pointer to an array of floats, the array must be at least 6 floats.
// 
//
// Parameters:		[in] p_video the CX_VIDEO from which to get the people count
//                [in] fdp a pointer to at least 6 floats
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Upon success fdp will contain the detection parameters in this order:
// 0 = top, 1 = left, 2 = height, 3 = width, 4 = min_height_prop, 5 = max_height_prop
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetDetectionParameters(CX_VIDEO p_video, float* fdp, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetResultsParameters
// 
// Description:   Call this function to set the various parameters that control how results 
//                can be saved for this video stream.
// 
//
// Parameters:		[in] p_video the CX_VIDEO from which to set the results
//                [in] results a CX_ResultsSettings variable
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus enables the automatic saving/posting of results from video processing. See
// the definition of CX_ResultsSettings to learn more about what can be controlled
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetResultsParameters(CX_VIDEO p_video, CX_ResultsSettings results, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// sets p_results to the current results setting for p_video
//
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
EXTERN_C CXVFRSDK int CXAPI CX_GetResultsParameters(CX_VIDEO p_video, CX_ResultsSettings* p_results, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// sets p_results to the default results setting for p_video
//
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
EXTERN_C CXVFRSDK int CXAPI CX_GetDefaultResultsParameters(CX_VIDEO p_video, CX_ResultsSettings* p_results, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddUserTagToResults
// 
// Description:   Call this function to add your own tag & value pair to the saved results 
//                for this video stream.
// 
//
// Parameters:		[in] p_video the CX_VIDEO to which that tag is to be added
//                [in] tag_name a string representing the XML tag name
//                [in] tag_value a string containing the tage value
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus enables the automatic saving/posting of results from video processing. See
// the definition of CX_ResultsSettings to learn more about what can be controlled, this
// function allows you to add your own tags to the saved XML results
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddUserTagToResults(CX_VIDEO p_video, const char* tag_name, const char* tag_value, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ClearUserTagToResults
// 
// Description:   Call this function to remove all user tag & value pairs
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ClearUserResultsTags(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetNumUserResultsTags
// 
// Description:   Call this function to find out how many user tag & value pairs
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns number of user tag value pairs upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetNumUserResultsTags(CX_VIDEO p_video, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetUserTagName
// 
// Description:   Call this function to find out the tag name of a particular tag & value pair
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//                [in] index: the index of the user tag/value pair
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the tag name of the tag value pair at index upon success.
//
// NOTE! Do not free or delete the returned pointer
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK const char* CXAPI CX_GetUserTagName(CX_VIDEO p_video, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetUserTagName
// 
// Description:   Call this function to change the tag name of a particular tag & value pair
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//                [in] index: the index of the user tag/value pair
//                [in] tag_name: the name you want to use for the tag of the "index" tag/value pair
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetUserTagName(CX_VIDEO p_video, int index, const char* tag_name, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetUserTagValue
// 
// Description:   Call this function to find out the tag value of a particular tag & value pair
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//                [in] index: the index of the user tag/value pair
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the tag value of the tag value pair at index upon success.
//
// NOTE! Do not free or delete the returned pointer
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK const char* CXAPI CX_GetUserTagValue(CX_VIDEO p_video, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetUserTagValue
// 
// Description:   Call this function to change the tag name of a particular tag & value pair
//                for this video stream.
// 
//
// Parameters:		[in] p_video the relevant CX_VIDEO object
//                [in] index: the index of the user tag/value pair
//                [in] tag_value: the value you want to use for the value of the "index" tag/value pair
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetUserTagValue(CX_VIDEO p_video, int index, const char* tag_value, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SetALLResultsParameters
// 
// Description:   Call this function to set the various parameters that control how results 
//                can be saved for all video streams.
// 
//
// Parameters:		[in] p_aureus the CX_AURUES object
//                [in] results a CX_ResultsSettings variable
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
// Notes: Aureus enables the automatic saving/posting of results from video processing. See
// the definition of CX_ResultsSettings to learn more about what can be controlled.
//
// You can set individual video save results settings or you can call this function
// to save all video stream save results settings to the same values.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetALLResultsParameters(CX_AUREUS p_aureus, CX_ResultsSettings results, char* msg);
//////////////////////////////////////////////////////////////////////////////////////






#endif  // AUREUS_VIDEO_H_
