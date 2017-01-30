////////////////////////////////////////////////////////////////////////////////
// Copyright: (C) 2016 CyberExtruder.com Inc.
//                     www.cyberextruder.com
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CX_AUREUS_H_
#define CX_AUREUS_H_
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// some simple definitions to ensure the type sizes for Aureus SDKs
#ifndef cx_real
#define cx_real float
#endif

#ifndef cx_byte
#define cx_byte unsigned char
#endif

#ifdef WIN32

#ifndef cx_int
#define cx_int __int32
#endif

#ifndef cx_uint
#define cx_uint unsigned cx_int
#endif

#else
#ifndef cx_int
#define cx_int int32_t
#endif

#ifndef cx_uint
#define cx_uint uint32_t
#endif

#endif
/////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////
// These parameters affect the face detection algorithm(s) in the SDK.
// The top/left/height/width describe an image window in which to search
// for faces. The are normalized (0..1) paramters with the origin top left.
// Smaller windows result in faster detections as long as the heads are 
// inside the window.
// 
// min/max proportions limit the sizes of heads/faces to be found, larger
// values of m_min_height_prop result in faster detections. A typical set
// of values might be:
//
// search whole image
// top = 0.001
// left = 0.001
// height = 0.999
// width = 0.999
//
// reasonably small heads
// m_min_height_prop = 0.2
// m_max_height_prop = 0.8
//
/////////////////////////////////////////////////////////////////////
struct CX_DetectionParams
{
  cx_real    m_top;
  cx_real    m_left;
  cx_real    m_height;
  cx_real    m_width;
  cx_real    m_min_height_prop;
  cx_real    m_max_height_prop;
};
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a structure for passing in images via RAM
struct CX_RAM_Image
{
  cx_byte*    mp_pixels;
  cx_uint     m_rows;
  cx_uint     m_cols;
  cx_uint     m_type;    // 0=RGB, 1=RGBA, 2=Gray, 3=BGR, 4=BGRA
  cx_uint     m_origin;  // 0=top left,  1=bottom left
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
struct CX_VECTOR2D
{
  float x, y;
};
////////////////////////////////////////////////////////////////////////////////
struct CX_POLY2D
{
  bool m_is_front;
  int  m_elems;
  int  m_indices[5];
};
////////////////////////////////////////////////////////////////////////////////
struct CX_MESH2D
{
  int  m_num_verts;
  CX_VECTOR2D m_verts[700];
  int  m_num_polys;
  CX_POLY2D   m_polys[700];
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
struct CX_VECTOR3D
{
  float x, y, z;
};
////////////////////////////////////////////////////////////////////////////////
struct CX_POLYVERTEX
{
  int v, t, n; // vertex, texture, normal
};
////////////////////////////////////////////////////////////////////////////////
struct CX_POLY3D
{
  int            m_elems;
  CX_POLYVERTEX  m_indices[5];
};
////////////////////////////////////////////////////////////////////////////////
struct CX_RGBA
{
  unsigned char r, g, b, a;
};
////////////////////////////////////////////////////////////////////////////////
struct CX_MESH3D
{
  // 3D vertices
  int  m_num_verts;
  CX_VECTOR3D m_verts[1500];
  // 2D uv (texture) coordinates (normalized 0..1)
  int  m_num_tex;
  CX_VECTOR2D m_tex[1500];
  // 3D vertex normals
  int  m_num_norms;
  CX_VECTOR3D m_norms[1500];
  // 3D polygons
  int  m_num_polys;
  CX_POLY3D   m_polys[1500];
  // texture map, pixels and size (origin always bottom left)
  CX_RGBA     m_pixels[65536];
  cx_uint     m_rows;
  cx_uint     m_cols;
  // 3D mesh extents, min and max values
  CX_VECTOR3D m_extent_verts;
  CX_VECTOR3D m_min_verts;
  CX_VECTOR3D m_max_verts;
};
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a structure to contain information about detected/tracked heads
struct CX_HeadInfo
{
  // an index refering to the tracked head, after performing tracking
  // this index will remain the same throughout tracked frames
  // this value is undertermined upon detection, it only has meaning after tracking
  cx_int   m_head_id;

  // the image size 
  cx_int   m_rows;
  cx_int   m_cols;

  // the head location in normalized coordinates with origin at bottom left
  bool    m_head_ok;
  cx_real m_head_bl_x, m_head_bl_y, m_head_tr_x, m_head_tr_y;

  // the face location in normalized coordinates with origin at bottom left
  bool    m_face_ok;
  cx_real m_face_bl_x, m_face_bl_y, m_face_tr_x, m_face_tr_y;

  // a confidence measure with respect to face recognition, higher = better
  // this is constructed from measures of internal Aureus model fitting, 3D pose
  // estimation and face resolution
  cx_real m_confidence;

  // a focus measure with respect to face recognition, higher = better
  // this is only available after tracking and is undefined if
  //  m_has_annotation_set is false.
  cx_real m_focus;

  // a fit measure that denotes how well the internal models have fit, lower = better
  // this is only available after tracking and is undefined if
  //  m_has_annotation_set is false.
  cx_real m_fit;

  // the 3D pose of the detected head, this is only available after tracking
  // and is undefined if m_has_annotation_set is false.
  cx_real m_rot_x;
  cx_real m_rot_y;
  cx_real m_rot_z;

  // if true, then the head has an associated annotation set
  // if false then it does not
  bool m_has_annotation_set;

  // the frame number relevent to this head info
  cx_int m_frame_number;

  // a flag that denotes whether the head info data is valid and being tracked for the current frame
  bool   m_valid;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this struct is used to pass gallery information about a single person
struct CX_PersonInfo
{
  // a unique person identifier
  cx_int m_person_id;

  // the name of the person in the gallery
  char   m_person_name[1024];

  // the number of images associated with the person in the gallery
  cx_int m_num_images;
  // the unique image id's, one for each associated image
  cx_int m_image_ids[1024];

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CX_RankItem
{
  // the person ID from the gallery, this means the actual person: so in a list of CX_RankItem's
  // there might be more than one element having the same person_id because there were more
  // than one image per person; alternatively a list of CX_RankItem's might contain rank info
  // combined from many processed images, hence more than one element might have the same person_id
  cx_int  m_person_id;

  // this is the name of the person defined by the unique id m_person_id
  char    m_person_name[1024];

  // this is the image id from the gallery (same rules as person_id above wrt multiple processed images)
  cx_int  m_image_id;

  // this is the FR match score with range from zero (no match) to 1 (perfect match)
  cx_real m_score;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define AUREUS_TAG_SIZE 50
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CX_ResultsSettings
{
  // if set then xml files will be saved to the given folder, one xml file per tracked head
  bool m_save_xml_results;      // default = false
  char m_save_xml_folder[1024]; // default = InstallFolder/POST
  // if true then any saved xml files will be removed after a certain time period (circa 5s, subject to change)
  bool m_remove_files;          // default = true

  // if set then the xml files will be posted to the given url
  bool m_post_xml;         // default  = false
  char m_post_url[1024];   // default = http://localhost:81

  // if set then the tracked head image and it's highest ranked matched image
  // will be saved to the given folder
  bool m_save_images;              // default = false
  char m_save_images_folder[1024]; // default = InstallFolder/Results/Images

  bool m_save_only_verified;  // if true then only verified matches will be saved (default=true)

  // if xml files are saved, the contents are controlled by the following flags
  // each one has a tag that will be used in the xml file, you can edit/alter the tags
  // to suit your own schema
  char m_root_tag[AUREUS_TAG_SIZE]; // defaults to CustomerInfoRequest

  bool m_person_name;           // the name of the rank1 matched person
  char m_person_name_tag[AUREUS_TAG_SIZE]; // defaults to externalId

  bool m_person_id;             // the unique DB ID of the rank1 matched person
  char m_person_id_tag[AUREUS_TAG_SIZE]; // defaults to person_id

  bool m_processed_frames;      // the number of processed frames for the tracked head
  char m_processed_frames_tag[AUREUS_TAG_SIZE]; // defaults to ProcessedFrames

  bool m_head_id;               // the head id
  char m_head_id_tag[AUREUS_TAG_SIZE]; // defaults to head_id

  bool m_stream_type;           // a string (MediaFile, USBcamera, IPcamera)
  char m_stream_type_tag[AUREUS_TAG_SIZE]; // defaults to stream_type

  bool m_stream_connection_info; // a string containing the connection info (e.g. 0 for USB pin zero, or the IP camera url or the full path and file name of a media file)
  char m_stream_connection_info_tag[AUREUS_TAG_SIZE]; // defaults to conn_info

  bool m_stream_index;          // the index of the video stream
  char m_stream_index_tag[AUREUS_TAG_SIZE]; // defaults to stream_index

  bool m_verification_threshold;// the verification threshold used at the time
  char m_verification_threshold_tag[AUREUS_TAG_SIZE]; // defaults to VerificationThresh

  bool m_frame_number;          // the frame number at which the result occurred
  char m_frame_number_tag[AUREUS_TAG_SIZE]; // defaults to FrameNumber

  bool m_utc_time;              // coordinated universal time
  char m_utc_time_tag[AUREUS_TAG_SIZE]; // defaults to utcTime

  bool m_ranked_results;
  cx_uint m_ranked_results_n;     // the max number of ranked FR results (default = 1)
  char m_ranked_results_tag[AUREUS_TAG_SIZE]; // defaults to NumOfRankedResults

  // for each ranked result
  bool m_matched_person_name;   // the name of the ranked matched person
  char m_matched_person_name_tag[AUREUS_TAG_SIZE]; // defaults to externalId

  bool m_matched_person_id;     // the unique DB ID of the ranked matched person
  char m_matched_person_id_tag[AUREUS_TAG_SIZE]; // defaults to person_id

  bool m_matched_image_id;      // the unique DB ID of the ranked matched image
  char m_matched_image_id_tag[AUREUS_TAG_SIZE]; // defaults to image_id

  bool m_date_time_stamp;       // a date and time stamp
  char m_date_time_stamp_tag[AUREUS_TAG_SIZE]; // defaults to DateTime

  bool m_confidence_measure;    // the confidence measure of the tracked person's image (higher the better)
  char m_confidence_measure_tag[AUREUS_TAG_SIZE]; // defaults to Confidence

  bool m_focus_measure;         // the focus measure of the tracked person's image (higher the better)
  char m_focus_measure_tag[AUREUS_TAG_SIZE]; // defaults to Focus

  bool m_eye_positions;         // the eye locations in the tracked person's image
  char m_eye_positions_tag[AUREUS_TAG_SIZE]; // defaults to EyePositions

  bool m_face_box;              // the detected face box in the tracked person's image
  char m_face_box_tag[AUREUS_TAG_SIZE]; // defaults to FaceBox

  bool m_match_score;           // the score between the two matches
  char m_match_score_tag[AUREUS_TAG_SIZE]; // defaults to MatchScore

  bool m_matched_status;        // VERIFIED if the score >=verification threshold, UNVERIFIED if not
  char m_matched_status_tag[AUREUS_TAG_SIZE]; // defaults to matchStatus

  bool m_tracked_image;         // the tracked person's image
  char m_tracked_image_tag[AUREUS_TAG_SIZE]; // defaults to capturedImage

  bool m_matched_person_thumbnail; // the thumbnail of the person in the DB which resulted in the match
  char m_matched_person_thumbnail_tag[AUREUS_TAG_SIZE]; // defaults to MatchedPersonImage

  bool m_matched_thumbnail;     // the DB thumbnail image to which it was matched
  char m_matched_thumbnail_tag[AUREUS_TAG_SIZE]; // defaults to MatchedImage
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CX_FRTemplate
{
  cx_int   m_size;      // the size of the FR template
  cx_byte* mp_template; // the FR template data
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CX_MultFRTemplate
{
  int m_n_templates;                    // number of templates in this structure
  CX_FRTemplate* mp_templates;          // the templates
  bool m_combined_exists;               // if true then the last template is a combined template
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif  // CX_AUREUS_H_
