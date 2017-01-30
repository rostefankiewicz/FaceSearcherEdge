////////////////////////////////////////////////////////////////////////////////
//
// File: Aureus.h 
//
// Copyright: (C) 2016 CyberExtruder.com Inc. 
//                     www.cyberextruder.com
//
// Description:  This file contains the definitions for functions that
//               are exported by this DLL. 
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_VFR_H_
#define AUREUS_VFR_H_
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// Definition to enable exporting/importing all relevent functions
/////////// windows //////////////////////////
#ifdef WIN32
  #ifdef AUREUS_EXPORTS
    #define CXVFRSDK __declspec( dllexport )
  #else
    #define CXVFRSDK __declspec( dllimport )
#endif
///////// linux assumes gnu version >= 4.0 /////////////////////////////
#else
  #define CXVFRSDK __attribute__ ((visibility ("default")))
#endif
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
  #ifndef EXTERN_C
    #define EXTERN_C    extern "C"
  #endif
#else
  #ifndef EXTERN_C
    #define EXTERN_C    extern
  #endif
#endif
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
  #ifndef CXAPI
    //#define CXAPI      __cdecl
    #define CXAPI      __stdcall
  #endif
#else // linux
  #ifndef CXAPI
    #define CXAPI
  #endif
#endif
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// Various useful Aureus definitions and structures
#include "CX_Aureus.h"
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Aureus SDK Objects 
//////////////////////////////////////////////////////////////////////////////////////
// All Aureus SDK objects are represented as a void pointer, they are defined 
// here for convenience so that the functions that use them are more clearly defined.
// Note: all Aureus Objects are run time type checked to ensure they are used correctly.
//////////////////////////////////////////////////////////////////////////////////////
typedef void* CX_AUREUS;
typedef void* CX_VIDEO;
typedef void* CX_HEAD;
typedef void* CX_HEAD_LIST;
typedef void* CX_HEAD_DATA;
typedef void* CX_HEAD_DATA_LIST;
typedef void* CX_ANNOTATION;
typedef void* CX_ANNOTATION_SET;
typedef void* CX_IMAGE;
typedef void* CX_ENROLL_VIDEO;
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// Error messages:
// This Aureus SDK is re-entrant thread safe, for this reason ALL error messages are
// provided as a string parameter. You must provide enough space in the string for those
// messages to be correctly placed to it. We recommend setting the string size to 1024 bytes
// just to make sure.
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetMachineID
// 
// Description:   Gets the machine Identifier, you will need this to generate a license key
// 
// Parameters:		[in/out] msg a string large enough to contain error messages
// 
// Returns:       0 upon failure, msg will contain the error
//                1 upon success, msg will contain the machine ID
//
// NOTE:          Since this is a very simple function the only error that can occurr is
//                is that you did not pass in a valid msg with enough memory space
//                in which case msg probably will not contain the error message. In this case
//                the error message will be printed to std out.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetMachineID(char* msg);
//////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_CreateAureus
// 
// Description:   Call this function first after loading the DLL. 
//
// Returns:       Returns a pointer to and Aureus object, you will need to pass this
//                pointer into the other functions below. This enables your code to be
//                straight forwardly re-entrant thread safe.
//
// Returns:       NULL upon failure, check msg for information.
//                A CX_AUREUS object upon success.
//
// Note: Don't forget to Free the object returned when you have finished with it
//       use CX_FreeAureus() to unload the object and free the memory.
//
// Note: You can create only a single CX_AUREUS object per machine, a subsequent call to
//       this function will return an error. You can however create as many video objects
//       as you are licensed for
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_AUREUS CXAPI CX_CreateAureus(char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_FreeAureus
// 
// Description:   Call this function when you have finished all processing associated
//                with the Aureus object. 
//
// Parameters:		[in]	p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in/out] msg a string large enough to contain error messages
//
// Returns:       1 upon sucess, 0 upon failure check msg for more details
//               
// Notes:         Failure can be caused by passing in a NULL pointer, or a pointer
//                that is not an Aureus object returned from CX_CreateAureus().
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_FreeAureus(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_Initialize
// 
// Description:   Call this function once after loading the DLL and creating your Aureus 
//                object. Provide a directory from which the SDK can load the required 
//                Aureus data files.
//                The directory should contain the Aureus data files, e.g. Detectors.data.
//                and also the Aureus license file.
//
// Parameters:		[in]	p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//                [in]  load_gallery set to 1 if you want Aureus to load a gallery zero otherwise
//            		[in]	dir     The folder containing the Aureus data file(s)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 if if the initialization failed, 1 upon success.
//                Returns 1 if the SDK is already initialized;
//
//                The usual reasons for failure are that the data files are not found
//                or the license is incorrect, check msg for more info.
//
// Note: Aureus can detect and track people, generate FR templates and match them to a gallery.
// However, if you do not wish to load a gallery (e.g. just perform detection & tracking, or perform 1:1 FR)
// then set the load_gallery flag to zero.
// You must have an appropriate license to enable video stream processing and FR.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_Initialize(CX_AUREUS p_aureus, int load_gallery, const char* dir, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetVerbose
//
// Description:   The SDK will print information to stdout if verbose is switched on.
// 
// Parameters:		[in] p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in] use_verbose  If 1, then use verbose otherwise do not
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       0 = failure, 1 = success (check msg for more details)
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetVerbose(CX_AUREUS p_aureus, int use_verbose, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetLicenseInfo
// 
// Description:   Gets the current license information
// 
// Parameters:		[in]	p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       0 on an error, check msg upon failure for a description of the error.
//                1 upon success, msg will contain the license info
// 
//Note: CX_Initialize must be called first
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetLicenseInfo(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetLicenseParameters
// 
// Description:   Gets the current license parameters
// 
// Parameters:		[in]	p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//                [in/out] num_video_streams a pointer to an integer which will contain the
//                         licensed number of video streams, must not be NULL.
//                [in/out] image_enabled a pointer to an integer flag which will show whether
//                         Aureus is licensed for image input, must not be NULL.
//                [in/out] fr_enabled a pointer to an integer flag which will show whether
//                         Aureus is licensed for Facial Recognition, must not be NULL.
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       0 on an error, check msg upon failure for a description of the error.
//                1 upon success, msg will contain a string describing the remaining license 
//                time and the flags will contain the license information
// 
// Note: CX_Initialize must be called first
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetLicenseParameters(CX_AUREUS p_aureus, int* num_video_streams, int* image_enabled, int* fr_enabled, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetVersion
// 
// Description:   Gets the current Aureus version
// 
// Parameters:		[in]	p_aureus  a pointer to the Aureus object created by CX_CreateAureus
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       1 upon success, msg contains the current version of Aureus
//                0 upon failure, check msg for more details
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetVersion(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////









#endif  // AUREUS_VFR_H_

