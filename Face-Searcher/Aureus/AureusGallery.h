////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUREUS_GALLERY_H_
#define AUREUS_GALLERY_H_
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// include Aureus headers
#include "Aureus.h"
#include "AureusHeads.h"
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// Aureus can be used for FR in many ways. One of those ways is to have Aureus maintain
// it's own gallery. The gallery implementation is small and fast and flexible.
// The gallery maintains tables of people, images and FR templates. There can be more
// than one table of FR templates if there are more than one FR engine present.
// To use a particular engine you need to seelect that engine via Aureus.
//
// The Aureus gallery provides a thumnail image for every person and every image. This
// facilitates very fast display of live FR information direct to a GUI window.
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetNumFRengines
// 
// Description:   Call this function get the number of FR Engines currently loaded
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns number of FR Engines upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetNumFRengines(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetFRname
// 
// Description:   Call this function get the name of FR Engine at index
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] index the index of the FR Engine from which to obtain the name
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a string denoting the FR engine name upon success.
//
// NOTE: The index must be within the boundary provided by CX_GetNumFRengines();
//       Do not free/delete the returned pointer
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK const char* CXAPI CX_GetFRname(CX_AUREUS p_aureus, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetFRnumTemplates
// 
// Description:   Call this function get the number of templates for the FR Engine at index
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] index, the index of the FR Engine from which to obtain the number of templates
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the number of templates in the gallery for the indexed FR engine upon success.
//
// NOTE: The index must be within the boundary provided by CX_GetNumFRengines();
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetFRnumTemplates(CX_AUREUS p_aureus, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_SelectFRengine
// 
// Description:   Call this function select a currently loaded FR Engine, upon success
//                this engine will be used until you select a different one.
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] index, the index of the FR Engine to select
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
// NOTE: The index must be within the boundary provided by CX_GetNumFRengines();
//       
// NOTE: Any video stream processing will be stopped. You will have to re-start the stream(s)
//       
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SelectFRengine(CX_AUREUS p_aureus, int index, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ChangeGallery
// 
// Description:   Call this function select a different gallery. The Gallery file MUST
//                be in the Installation FR folder to enable it to pick up the FR Engine
//                plugin DLLs
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] gal_name the full path and filename of the gallery
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
// NOTE: Any video stream processing will be stopped. You will have to re-start the stream(s)
//       
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ChangeGallery(CX_AUREUS p_aureus, const char* gal_name, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetNumGalleryPeople
// 
// Description:   Call this function get the number of people in the currently loaded gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns number of people in the gallery upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetNumGalleryPeople(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetNumGalleryImages
// 
// Description:   Call this function get the number of images in the currently loaded gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns number of images in the gallery upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetNumGalleryImages(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetPersonInfo
// 
// Description:   Call this function to get the person info associated with one person in the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] index: the index of the person in the gallery
//                [in/out] person_info: upon successful return, will contain the person info
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon sucess.
//
// Notes: the parameter "index" refers the to index in a list of people e.g. in range
// 0...the value returned by CX_GetNumGalleryPeople(). Alternatively CX_GetPersonIndex() will
// also provide a valid index.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetPersonInfo(CX_AUREUS p_aureus, int index, CX_PersonInfo* person_info, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetPersonIndex
// 
// Description:   Call this function to get the person index associated with one person in the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] person_id: the unique person identifier from the gallery
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the person index upon sucess. 
//
// Notes: the person index is a integer index to a list of people, this is not the same as
// the person_id which is generated by the gallery.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_GetPersonIndex(CX_AUREUS p_aureus, int person_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_FillPersonInfo
// 
// Description:   Call this function to fill the person info in *person_info. The person id field must be correct
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in/out] person_info: upon successful return, will contain the all person info
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon sucess.
//
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_FillPersonInfo(CX_AUREUS p_aureus, CX_PersonInfo* person_info, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetPersonThumbnail
// 
// Description:   Call this function to get the thumbnail image associated with person_id
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] person_id the unique person id from which to obtain the thumbnail image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a CX_IMAGE upon sucess.
//
// Note: The returned image is allocated on the Heap, you MUST call CX_FreeImage(p_im) to
//       free the memory when you have finished with it.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_GetPersonThumbnail(CX_AUREUS p_aureus, int person_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ChangePersonName
// 
// Description:   Call this function to change the name of the person with person_id
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] person_id the unique person id of the person whose name is to be changed
//                [in] new_name the new name of the person (must NOT be NULL)
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon sucess.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ChangePersonName(CX_AUREUS p_aureus, int person_id, const char* new_name, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetImageThumbnail
// 
// Description:   Call this function to get the thumbnail image associated with image_id
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] image_id the unique image id from which to obtain the thumbnail image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a CX_IMAGE upon sucess.
//
// Note: The returned image is allocated on the Heap, you MUST call CX_FreeImage(p_im) to
//       free the memory when you have finished with it.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_GetImageThumbnail(CX_AUREUS p_aureus, int image_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_GetGalleryImage
// 
// Description:   Call this function to get the image associated with image_id
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] image_id the unique image id from which to obtain the image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns NULL upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns a CX_IMAGE upon sucess.
//
// Note: The returned image is allocated on the Heap, you MUST call CX_FreeImage(p_im) to
//       free the memory when you have finished with it.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK CX_IMAGE CXAPI CX_GetGalleryImage(CX_AUREUS p_aureus, int image_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_ApplyFR
// 
// Description:   Call this function apply the currently selected FR Engine to the gallery
//                to obtain a ranked list of results
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] p_im the input image to which FR will be applied
//                [in/out] p_rank_results a pointer to an array of CX_RankItem's, will contain
//                         the ranked FR results upon success
//                [in] array_size size of the array pointer to by p_rank_results
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the number of elements inserted into the array upon sucess.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_ApplyFR(CX_AUREUS p_aureus, CX_RAM_Image* p_im, CX_RankItem* p_rank_results, int array_size, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddNewPerson
// 
// Description:   Call this function add a new person to the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] name a string denoting the person's name, must not be NULL
//            		[in] p_im the input image that will be used for this person
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the unique person id of the newly added person upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddNewPerson(CX_AUREUS p_aureus, const char* name, CX_RAM_Image* p_im, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddNewPersonMultiImage
// 
// Description:   Call this function add a new person to the gallery using multiple images
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] name a string denoting the person's name
//            		[in] p_ims a pointer to an array of input image pointers that will be used for this person
//                [in] n_images num of images pointed to by p_im
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the unique person id of the newly added person upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddNewPersonMultiImage(CX_AUREUS p_aureus, const char* name, CX_RAM_Image** p_ims, int n_images, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddNewPersonFromHead
// 
// Description:   Call this function add a new person to the gallery using a CX_HEAD
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] name a string denoting the person's name
//            		[in] p_head the CX_HEAD that will be enrolled as a new person
//                [in] n_images the max number of images to enroll
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns -1 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the unique person id of the newly added person upon success.
//
// Notes: You can Live enroll a tracked CX_HEAD from a CX_VIDEO frame callback using this
// function. Set n_images to the max num of images you wish to use to enroll, more than 10
// will be ignored, zero or negative will use all images.
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddNewPersonFromHead(CX_AUREUS p_aureus, const char* name, CX_HEAD p_head, int n_images, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_DeletePerson
// 
// Description:   Call this function delete a person from the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] person_id the unique identifier of the person obtained from the gallery
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_DeletePerson(CX_AUREUS p_aureus, int person_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddImageToPerson
// 
// Description:   Call this function add an image to a person in the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] person_id the unique identifier of the person obtained from the gallery
//                [in] p_im a pointer to an image
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddImageToPerson(CX_AUREUS p_aureus, int person_id, CX_RAM_Image* p_im, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_AddImagesToPerson
// 
// Description:   Call this function add multiple images to a person in the gallery
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] person_id the unique identifier of the person obtained from the gallery
//            		[in] p_ims a pointer to an array of input image pointers that will be used for this person
//                [in] n_images the number of images in the array
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_AddImagesToPerson(CX_AUREUS p_aureus, int person_id, CX_RAM_Image** p_ims, int n_images, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_DeleteImage
// 
// Description:   Call this function delete an image from the gallery. If the image is the only
//                image belonging to a person then the person will also be deleted
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//                [in] image_id the unique identifier of the image obtained from the gallery
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns 0 upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns 1 upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_DeleteImage(CX_AUREUS p_aureus, int image_id, char* msg);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_UpdateFR
// 
// Description:   Call this function to update the FR tables in the DB, in some cases they
//                may not be up to date: e.g. if a new plugin DLL has just been added then
//                you will need to update the FR tables.
//
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       -1 for incorrect parameters, 0 = total failure, 1 = total success, 2 = partial success (some templates may not have been generated)
//                upon error msg will contain the error messages, WARNING this can
//                be large, potentially as large as one error message for every image
//                in the DB, please set the size of msg accordingly.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_UpdateFR(CX_AUREUS p_aureus, char* msg);
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 
// Type:					CX_UPDATE_FR_CALLBACK
// 
// Parameters:		[in]	msg     an informative message on the current update status, do NOT free the pointer
//                [in]  total   the total count
//                [in]  current the current count (range = 0...total)
//                [in]  p_object       Allows passing of data, can be NULL
// 
// Description:		Signature of the update FR callback function that will provide 
//                information regarding the current status of the update.
//                If current<0 this means that the update has finished
// 
//////////////////////////////////////////////////////////////////////////////////////
typedef void(*CX_UPDATE_FR_CALLBACK) (const char* msg, cx_int total, cx_int current, void* p_object);
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:			CX_SetUpdateFRCallBack
// 
// Parameters:		[in] p_aureus a CX_AUREUS pointer
//            		[in] p_func         The function that will be called
//                [in] p_object       Allows passing of data, can be NULL
//            		[in/out] msg        a string large enough to contain error messages
// 
// Returns:				1 upon success
//                0 upon error
// 
// Description:		When CX_UpdateFR() is called you can obtain the progress by setting
//                this callback function.
//                The parameters of the call back function are described above.
//           
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK int CXAPI CX_SetUpdateFRCallBack(CX_AUREUS p_aureus, CX_UPDATE_FR_CALLBACK p_func, void* p_object, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
// 
// Function:  		CX_MatchFRtemplates
// 
// Description:   Call this function match two FR templates and return a match score.
//                You must have the correct FR engine currently selected when matching AND 
//                the templates must correspond to that FR engine
//
// Parameters:		[in] p_aureus a CX_AUREUS object
//                [in] p_t1 a pointer to the first template
//                [in] t1_size the size (in bytes) of the first template
//                [in] p_t2 a pointer to the second template
//                [in] t2_size the size (in bytes) of the second template
//            		[in/out] msg a string large enough to contain error messages
// 
// Returns:       Returns negative upon failure (e.g. incorrect parameters) check msg for more details.
//
//                Returns the match score upon success.
//
//////////////////////////////////////////////////////////////////////////////////////
EXTERN_C CXVFRSDK cx_real CXAPI CX_MatchFRtemplates(CX_AUREUS p_aureus,  
                cx_byte* p_t1, int t1_size, cx_byte* p_t2, int t2_size, char* msg);
//////////////////////////////////////////////////////////////////////////////////////




#endif  // AUREUS_GALLERY_H_

