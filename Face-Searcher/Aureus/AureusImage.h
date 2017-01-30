////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_IMAGES_H_
#define AUREUS_IMAGES_H_
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// include Aureus headers
#include "Aureus.h"
#include "AureusHeads.h"
#include "AureusGallery.h"
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetTemplateSize
// 
// Description:   Call this function get the size of a template for the currently loaded FR Engine
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the size (in bytes) of a template for the currently loaded FR Engine.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetTemplateSize(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GenerateTemplate
// 
// Description:   Call this function to generate a template using the currently loaded FR Engine
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_im a pointer to an image from which to generate the template
//                [in] fdp the face detection parameters (see CX_Aureus.h for more info)
//                [in/out] p_template a pointer to an array of bytes
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. Upon success, p_template will contain the template. You
//                must ensure that p_template has enough allocated memory to store the template.
//                Use CX_GetTemplateSize() to determine how much memory to allocate.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GenerateTemplate(CX_AUREUS p_aureus, CX_RAM_Image* p_im, CX_DetectionParams fdp, cx_byte* p_template, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GenerateTemplateAndFaceMeasures
// 
// Description:   Call this function to generate a template using the currently loaded FR Engine
//                also to obtain face location, confidence and focus measures.
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_im a pointer to an image from which to generate the template
//                [in] fdp the face detection parameters (see CX_Aureus.h for more info)
//                [in/out] face_, pointers to cx_reals, upon sucess they will contain the face location
//                [in/out] conf, a pointer to a cx_real, upon sucess it will contain the conficense measure
//                [in/out] focus, a pointer to a cx_real, upon sucess it will contain a measure of focus for the detected face
//                [in/out] p_template a pointer to an array of bytes
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. Upon success, p_template will contain the template. You
//                must ensure that p_template has enough allocated memory to store the template.
//                Use CX_GetTemplateSize() to determine how much memory to allocate.
//
// NOTE: Thereturned face location has normalized coordinates with the origin bottom left, you
// will have to manipulate the values to obtain the face location wrt the p_im origin if it
// has origin top left.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GenerateTemplateAndFaceMeasures(CX_AUREUS p_aureus, CX_RAM_Image* p_im, CX_DetectionParams fdp, cx_byte* p_template, 
  cx_real* face_bl_x, cx_real* face_bl_y, cx_real* face_tr_x, cx_real* face_tr_y,
  cx_real* conf, cx_real* focus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GenerateTemplateFromPackedAnnotations
// 
// Description:   Call this function to generate a template using the currently loaded FR Engine
//                and a prefitted packed annotation set.
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_im a pointer to an image from which to generate the template
//                [in] p_packed_annos a pointer to a packed annotations set (see AureusHead.h for more info)
//                [in/out] p_template a pointer to an array of bytes
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. Upon success, p_template will contain the template. You
//                must ensure that p_template has enough allocated memory to store the template.
//                Use CX_GetTemplateSize() to determine how much memory to allocate.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GenerateTemplateFromPackedAnnotations(CX_AUREUS p_aureus, CX_RAM_Image* p_im, cx_byte* p_packed_annos, cx_byte* p_template, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GenerateMultiTemplate
// 
// Description:   Call this function to generate a multi FR template from one or more images
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_ims an array of image pointers from which to generate the template
//                [int] n_images the number of image pointers in p_ims
//                [in] fdp the face detection parameters (see CX_Aureus.h for more info)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a pointer to a CX_MultFRTemplate multi FR template upon success. 
//
// NOTE: Use this function to create templates from a set of multiple images of the same person.
// Aureus will attempt to create a template for each image AND also create a combined template.
// The last template in the returned structure will contain the combined template.
// If there is only a single image then a combined template will not be generated.
// You can match any of the templates produced using CX_MatchFRtemplates() (in AureusGallery.h).
//
// NOTE: There may be one or more failures to generate a template, check the number of templates
// generated in the returned pointer. Notice that if a combined template has been generated (which
// will be the case if two or more templates were successfully generated) then the m_combined_exists
// flag will be set and the last template will be the combined one. So for example, if you passed
// in 3 images and Aureus successfully generated 2 templates it will also have generated the 
// combined template and hence the returned structure will have 3 templates with the m_combined_exists
// flag set to true. In this case there would have been one failure, check msg for more details
// of any failures.
//
//
// NOTE: Use CX_FreeMultiFRTemplate() when you are finished with the returned pointer.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_MultFRTemplate* CXAPI CX_GenerateMultiTemplate(CX_AUREUS p_aureus, CX_RAM_Image** p_ims, int n_images, CX_DetectionParams fdp, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_FreeMultiFRTemplate
// 
// Description:   Call this function to free a previously generated multi FR template
//
// Parameters:		[in] p_mt the previously generated multi template
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. 
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_FreeMultiFRTemplate(CX_MultFRTemplate* p_mt, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_Generate3DMesh
// 
// Description:   Call this function to generate a 3D mesh from a given image
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_im a pointer to an image from which to generate the template
//                [in] fdp the face detection parameters (see CX_Aureus.h for more info)
//                [in/out] p_mesh a pointer to a 3D Mesh structure
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. Upon success, p_mesh will contain the 3D mesh. 
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_Generate3DMesh(CX_AUREUS p_aureus, CX_RAM_Image* p_im, CX_DetectionParams fdp, CX_MESH3D* p_mesh, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GeneratePoseCorrectedImage
// 
// Description:   Call this function to generate a pose corrected image from a given image
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] p_im a pointer to an image from which to generate the template
//                [in] fdp the face detection parameters (see CX_Aureus.h for more info)
//                [in] width, the width of the output image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a CX_IMAGE upon success, use CX_FreeImage() after you have finished with the image
//
// Notes: This function will generate a pose corrected image from the given input image
// the image will be USO standard tokenized, hence the output height will be = width / 0.75
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_GeneratePoseCorrectedImage(CX_AUREUS p_aureus, CX_RAM_Image* p_im, CX_DetectionParams fdp, cx_uint width, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_CopyImage
// 
// Description:   Call this function to copy an image into a different format
//
// Parameters:		[in] p_src_image the src image from which to copy
//                [in] dst_im_type, the type of output image you require, // 0=RGB, 1=RGBA, 2=Gray, 3=BGR, 4=BGRA
//                [in] dst_origin, the origin of the output image, // 0=top left,  1=bottom left
//                [out] p_dst_image, a pointer to the pixels of your output image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success. 
//
// Note: You MUST allocate enough memory in p_dst_pixels to perform the conversion
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_CopyImage(CX_RAM_Image* p_src_image, int dst_im_type, int dst_origin, cx_byte* p_dst_pixels, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_FreeImage
// 
// Description:   Free's a CX_IMAGE that has been allocated on the heap
// 
// Parameters:		[in]	p_im  the CX_IMAGE to be freed
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       1 upon success
//                0 upon failure, check msg for more details
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_FreeImage(CX_IMAGE p_im, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


#endif // AUREUS_IMAGES_H_
