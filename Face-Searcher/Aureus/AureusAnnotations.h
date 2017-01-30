////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_ANNOTATIONS_H_
#define AUREUS_ANNOTATIONS_H_
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// include Aureus headers
#include "Aureus.h"
#include "AureusHeads.h"
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// A CX_ANNOTATION_SET is an object containing 1 or more CX_ANNOTATIONs. Each
// CX_ANNOTATION contains a list of normalized 2D points, a label and 
// a flag denoting whether the CX_ANNOTATION is open or closed.
//
// So, for example, a CX_ANNOTATION might have a label called "RIGHT EYE" with the
// open flag set to false and 8 points deliniating the eye.
//
// Note: All annotation points are normalized (0...1) with respect to the image and
// they always represent points with origin at the bottom left of the image.
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetHeadAnnotationSet
// 
// Description:   Call this function get the annotation set associated with a given CX_HEAD
//
// Parameters:		[in] p_head a pointer to a CX_HEAD
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//                Also returned NULL if the head does not contain an annotation set
//
//                Returns a pointer to an annotation set upon success.
//
// NOTE:          Do not free the returned pointer
//
//                An annotation set is a list of annotations, an annotation is a list
//                of normalized (wrt the image) 2D points.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_ANNOTATION_SET CXAPI CX_GetHeadAnnotationSet(CX_HEAD p_head, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotationSetSize
// 
// Parameters:		[in] p_anno_set    The CX_ANNOTATION_SET
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				The number of CX_ANNOTATIONs in the set upon success
//                -1 upon error, check msg for more information
// 
// Description:		This allows you to get number of annotations in the given annotation set.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetAnnotationSetSize(CX_ANNOTATION_SET p_anno_set, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotation
// 
// Parameters:		[in] p_anno_set    The CX_ANNOTATION_SET
//                [in] index         The index of the annotation within the set
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				A pointer to the indexed CX_ANNOTATION in the set upon success
//                0 upon error, check msg for more information
// 
// Description:		This  allows you to get the annotation at the given index in the given annotation set.
//
// Note:          Do not free/delete the returned pointer
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_ANNOTATION CXAPI CX_GetAnnotation(CX_ANNOTATION_SET p_anno_set, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotationSize
// 
// Parameters:		[in] p_anno    The CX_ANNOTATION
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				The number of 2D points in the annotation upon success
//                -1 upon error, check msg for more information
// 
// Description:		This allows you to get number of normalized 2D points in the given annotation.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetAnnotationSize(CX_ANNOTATION p_anno, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotationPoint
// 
// Parameters:		[in] p_anno    The CX_ANNOTATION
//                [in] index     The index of the 2D point within the annotation
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				The indexed 2D point in the annotation upon success
//                0 upon error, check msg for more information
// 
// Description:		This allows you to get the normalized 2D point at the given index in the 
//                given annotation.
//                The 2D vector has coordinates normalized (0..1) to the image width
//                and height respectively with a bottom left origin.
//
// Note:          Do not free/delete the returned pointer
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK float* CXAPI CX_GetAnnotationPoint(CX_ANNOTATION p_anno, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotationOpen
// 
// Parameters:		[in] p_anno    The CX_ANNOTATION
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				1 if the annotation was open, 0 if it was closed
//                -1 upon error, check msg for more information
// 
// Description:		This allows you to get the open/closed flag for the given annotation.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetAnnotationOpen(CX_ANNOTATION p_anno, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_GetAnnotationLabel
// 
// Parameters:		[in] p_anno    The CX_ANNOTATION
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:				A string denoting the lable of the annotation
//                0 upon error, check msg for more information
// 
// Description:		This allows you to get the string label for the given annotation.
//           
// Note:          Do not free/delete the returned pointer
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK const char* CXAPI CX_GetAnnotationLabel(CX_ANNOTATION p_anno, char* msg);
//////////////////////////////////////////////////////////////////////////////////////






#endif  // AUREUS_ANNOTATIONS_H_

