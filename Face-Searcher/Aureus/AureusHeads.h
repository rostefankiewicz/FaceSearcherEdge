////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_HEADS_H_
#define AUREUS_HEADS_H_
////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// include main Aureus header
#include "Aureus.h"
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// During video processing Aureus detects and tracks heads. It also performs FR on those
// heads (if switched on). The video frame callback provides a list of the current 
// tracked heads.
// Each head has a current head data object and a sorted list of the "best" head data
// objects. Each head data object contains tracking information, confidence scores,
// focus measurements, FR templates (if used) and FR rank match information (if FR used).
//
//////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadListSize
// 
// Description:   Call this function get the number of heads in a given head list returned by 
//                CX_ProcessFrame() or from withint the frame callback.
//
// Parameters:		[in] p_head_list a pointer to a CX_HEAD_LIST 
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the number heads in the list upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetHeadListSize(CX_HEAD_LIST p_head_list, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHead
// 
// Description:   Call this function get a head in a given head list
//
// Parameters:		[in] p_head_list a pointer to a CX_HEAD_LIST 
//                [in] index, the index into the head list (will return error if out of range)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the head indexed in the list upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD CXAPI CX_GetHead(CX_HEAD_LIST p_head_list, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadInfo
// 
// Description:   Call this function get the data associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//                [out] p_data a pointer to a head info structure that will contain the data upon return
//                      this parameter MUST NOT be NULL
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetHeadInfo(CX_HEAD p_head, CX_HeadInfo* p_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadID
// 
// Description:   Call this function get the unique head id for a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the head index upon success.
//
// NOTE: The head id is unique to this tracked head, other heads will have different ids
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetHeadID(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadMarkedForDeletion
// 
// Description:   Calling this function returns a bool that denotes that the head
//                will be deleted on the next frame. This is your last chance to 
//                use the head.
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns 1 if the head will be deleted next frame or 0 otherwise.
//
// NOTE: Aureus maintains a list of tracked heads, as a head dissapears i.e. is occluded for
// long enough or leaves the filed of view of the camera it will be deleted from the list.
// Prior to deletion the head will be marked for deletion. Take this opportunity to use the
// head before is is deleted on the next frame. For example, you may wish to enroll the head
// as a new person, or copy the head for later use.
// A head marked for deletion is no longer being actively tracked and is not current in this
// frame.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetHeadMarkedForDeletion(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetNumHeadEnrollmentImages
// 
// Description:   Call this function determine the number of enrollment images present in a CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns number of enrollment images upon sucess (can be zero).
//
// Note: This is useful if you wish to enroll this head, you can determine how many images
// are withint the head that are capable of enrollment.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetNumHeadEnrollmentImages(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetCurrentHeadData
// 
// Description:   Call this function get the most current CX_HEAD_DATA associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a pointer to a CX_HEAD_DATA upon success.
//
// Note: The current CE_HEAD_DATA is the "most current" it is still possible that this
// CE_HEAD_DATA came from a previous frame.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD_DATA CXAPI CX_GetCurrentHeadData(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadFRRankedResults
// 
// Description:   Call this function get the current ranked FR results associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//                [in/out] p_rank_results a pointer to an array of CX_RankItem's, will contain
//                         the ranked FR results upon success
//                [in] array_size size of the array pointer to by p_rank_results
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the number of results placed in the p_rank_results array upon success.
//
// Note: To obtain this information you must have generate templates switched on, also 
// you must have perform gallery switched on and have loaded a non empty gallery.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetHeadFRRankedResults(CX_HEAD p_head, CX_RankItem* p_rank_results, int array_size, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetMeshOverlay
// 
// Description:   Call this function get the the mesh overlay associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//                Also returned NULL if the head does not contain a mesh overlay
//
//                Returns a pointer to a mesh overlay upon success.
//
// NOTE:          Do not free the returned pointer
//
//                A mesh overlay is a set of 2D vertices referenced by a set of
//                polygons. Each polygon can have up to 5 vertices and a flag
//                denoting whether the polygon is visible or unseen.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_MESH2D* CXAPI CX_GetMeshOverlay(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadFit
// 
// Description:   Call this function get the model fit measure associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns negative upon failure (e.g. incorrect parameters) check msg for more details.
//                Also returns negative if the head does not contain an annotation set
//
// NOTE:          The returned fit is a measure of how well the internal Aureus models
//                have fit to the current image, lower values are better.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_GetHeadFit(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_Get3DMesh
// 
// Description:   Call this function get the the 3D mesh associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//                Also returned NULL if the head does not contain a 3D mesh
//
//                Returns a pointer to a 3D mesh upon success.
//
// NOTE:          Do not free the returned pointer
//
//                A 3D mesh overlay is a set of 3D vertices, normals and texture
//                coordinates referenced by a set of polygons. Each polygon can 
//                have up to 5 vertices.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_MESH3D* CXAPI CX_Get3DMesh(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetPCimage
// 
// Description:   Call this function get the the pose corrected image associated 
//                with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//                Also returned NULL if the head does not contain a 3D mesh
//
//                Returns a pointer to an image upon success.
//
// NOTE:          Do not free the returned pointer
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_GetPCimage(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetRankedHeadDataList
// 
// Description:   Call this function get the sorted list of CX_HEAD_DATAs associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a pointer to a CX_HEAD_DATA_LIST upon success.
//
// Note: this function provides access to the list of "best" CX_HEAD_DATAs associated
// with this CX_HEAD
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD_DATA_LIST CXAPI CX_GetRankedHeadDataList(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataListSize
// 
// Description:   Call this function get the number of elements in a CX_HEAD_DATA_LIST
//
// Parameters:		[in] p_head_data_list a pointer to a CX_HEAD_DATA_LIST
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns number of elements in p_head_data_list upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_HeadDataListSize(CX_HEAD_DATA_LIST p_head_data_list, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadData
// 
// Description:   Call this function get an element in a CX_HEAD_DATA_LIST
//
// Parameters:		[in] p_head_data_list a pointer to a CX_HEAD_DATA_LIST
//                [in] index the index of the element in the CX_HEAD_DATA_LIST
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a pointer to a CX_HEAD_DATA at index in p_head_data_list upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD_DATA CXAPI CX_GetHeadData(CX_HEAD_DATA_LIST p_head_data_list, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataConfidence
// 
// Description:   Call this function get the confidence measure of a CX_HEAD_DATA
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns negative upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a cx_real indicating the confidence measure (higher = better).
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_HeadDataConfidence(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataFocus
// 
// Description:   Call this function get the focus measure of a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns negative upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the focus measure associated with p_head_data.
//                Higher focus measures indicate head images that are more infocus than lower measures
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_HeadDataFocus(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataFit
// 
// Description:   Call this function get the fit measure of a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns negative upon failure (e.g. incorrect parameters) check msg for more details
//                Also returns negative if there is no annotation set for the head data object
//                Returns the fit measure associated with p_head_data.
//                Lower fit measures indicate better internal model fitting than higher values
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_HeadDataFit(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataDateTime
// 
// Description:   Call this function get the date and time stamp of a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the date and time stamp (Month_Day_Year_Hour_Min_Sec_Millisecond) associated with p_head_data
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK const char* CXAPI CX_HeadDataDateTime(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataImage
// 
// Description:   Call this function get the image inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a CX_IMAGE pointer to the image inside p_head_data.
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_HeadDataImage(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ImageData
// 
// Description:   Call this function get image data from a CX_IMAGE object
//
// Parameters:		[in] p_image a CX_IMAGE pointer
//                [in,out] rows a pointer to a cx_uint, it will contain the num of image rows upon success
//                [in,out] cols a pointer to a cx_uint, it will contain the num of image cols upon success
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a cx_byte pointer to the image pixels.
//
// NOTE:          The image ia ALWAYS a row major, RGBA image with the origin at the bottom left
//
// Note:          Do not free/delete the returned pointer
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_byte* CXAPI CX_ImageData(CX_IMAGE p_image, cx_uint* rows, cx_uint* cols, char* msg);
//////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataFRTemplate
// 
// Description:   Call this function get the FR template inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//                [in/out] p_template_size a pointer to an int variable that will 
//                         contain the size of the returned FR template, must NOT be NULL
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a byte pointer to the FR template inside p_head_data.
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_byte* CXAPI CX_HeadDataFRTemplate(CX_HEAD_DATA p_head_data, int* p_template_size, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataAnnotations
// 
// Description:   Call this function get the annotations set inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//                [in/out] num_annotations a pointer to an int variable that will 
//                         contain the number of annotations in the returned annotation
//                         set, must NOT be NULL
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the CX_ANNOTATION_SET inside p_head_data upon success
//
// Note: Do not free/delete the returned pointer
// Note: Not all CX_HEAD_DATA objects contain valid annotation sets, a returned NULL
// pointer can mean that the image contained within the head data object was not suitable
// to produce an annotation set of landmark points.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_ANNOTATION_SET CXAPI CX_HeadDataAnnotations(CX_HEAD_DATA p_head_data, int* num_annotations, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AnnotationsByteArraySize
// 
// Description:   Call this function get the size of an annotations set stored as a packed byte array
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the size (in bytes) of an annotations set stored as a packed byte array.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AnnotationsByteArraySize(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataAnnotationsAsByteArray
// 
// Description:   Call this function get the annotations set inside a CX_HEAD_DATA object
//                This function differs from CX_HeadDataAnnotations in that it packs the
//                annotations set into a byte array.
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//                [in/out] p_array a pointer to a byte array that will 
//                         contain the annotations set as a byte array set, must NOT be NULL
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details
//                Returns the 1  upon success
//
// Note: Not all CX_HEAD_DATA objects contain valid annotation sets, a returned zero
// value can mean that the image contained within the head data object was not suitable
// to produce an annotation set of landmark points.
//
// Note: You must pass in an array of bytes large enough to hold the packed annotations set
// you can obtain the required size of such an array from the function CX_AnnotationsByteArraySize
// described above.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_HeadDataAnnotationsAsByteArray(CX_HEAD_DATA p_head_data, cx_byte* p_array, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataMeshOverlay
// 
// Description:   Call this function get the mesh overlay inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a CX_MESH2D pointer to the mesh overlay inside p_head_data.
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_MESH2D* CXAPI CX_HeadDataMeshOverlay(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadData3DMesh
// 
// Description:   Call this function get the 3D mesh inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a CX_MESH3D pointer to the 3D mesh inside p_head_data.
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_MESH3D* CXAPI CX_HeadData3DMesh(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_HeadDataPCimage
// 
// Description:   Call this function get the pose corrected image inside a CX_HEAD_DATA object
//
// Parameters:		[in] p_head_data a pointer to a CX_HEAD_DATA
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a CX_IMAGE pointer to the pose corrected image inside p_head_data.
//
// Note: Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_HeadDataPCimage(CX_HEAD_DATA p_head_data, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_CopyHead
// 
// Description:   Call this function get a copy of a CX_HEAD. It is your responsibility to
//                free the associated memory using CX_FreeHead() when you have finished with it.
//
// Parameters:		[in] p_head the CX_HEAD from which to make a copy
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details
//                Returns a copy of p_head upon success.
//
// Note: Use CX_FreeHead() to free the associated memory when you have finished using the
// copy.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_HEAD CXAPI CX_CopyHead(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////
// frees a previously copied CX_HEAD, returns 1 upon success, 0 on failure
EXTERN_C CXVFRSDK int CXAPI CX_FreeHead(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////






#endif  // AUREUS_HEADS_H_

