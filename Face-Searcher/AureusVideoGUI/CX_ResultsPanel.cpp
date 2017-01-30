///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/textdlg.h"
#include <wx/notebook.h>
#ifdef WIN32
#include "wx/msw/checkbox.h"
#include "wx/msw/textctrl.h"
#include "wx/msw/button.h"
#include "wx/msw/statbox.h"
#else
#include "wx/gtk/checkbox.h"
#include "wx/gtk/textctrl.h"
#include "wx/gtk/button.h"
#include "wx/gtk/statbox.h"
#endif
#include "CX_ResultsPanel.h"
#include "AureusVideoGUI.h"
#include "CX_VideoPanel.h"
#include "CX_VideoDialog.h"
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
static const long ID_RESULTS_SAVE_XML = wxNewId();
static const long ID_RESULTS_SAVE_XML_FOLDER = wxNewId();
static const long ID_RESULTS_XML_BROWSE = wxNewId();
static const long ID_RESULTS_REMOVE_FILES = wxNewId();
static const long ID_RESULTS_ONLY_VERIFIED = wxNewId();

static const long ID_RESULTS_POST_XML = wxNewId();
static const long ID_RESULTS_POST_XML_URL = wxNewId();

static const long ID_RESULTS_SAVE_IMAGES_FOLDER = wxNewId();
static const long ID_RESULTS_IMAGES_BROWSE = wxNewId();
static const long ID_RESULTS_SAVE_IMAGES = wxNewId();

static const long ID_RESULTS_ROOT = wxNewId();
static const long ID_RESULTS_ROOT_TAG = wxNewId();
static const long ID_RESULTS_ROOT_DEFAULT = wxNewId();

static const long ID_RESULTS_PERSON_NAME = wxNewId();
static const long ID_RESULTS_PERSON_NAME_TAG = wxNewId();
static const long ID_RESULTS_PERSON_NAME_DEFAULT = wxNewId();

static const long ID_RESULTS_PERSON_ID = wxNewId();
static const long ID_RESULTS_PERSON_ID_TAG = wxNewId();
static const long ID_RESULTS_PERSON_ID_DEFAULT = wxNewId();

static const long ID_RESULTS_PROCESSED_FRAMES = wxNewId();
static const long ID_RESULTS_PROCESSED_FRAMES_TAG = wxNewId();
static const long ID_RESULTS_PROCESSED_FRAMES_DEFAULT = wxNewId();

static const long ID_RESULTS_HEAD_ID = wxNewId();
static const long ID_RESULTS_HEAD_ID_TAG = wxNewId();
static const long ID_RESULTS_HEAD_ID_DEFAULT = wxNewId();

static const long ID_RESULTS_STREAM_TYPE = wxNewId();
static const long ID_RESULTS_STREAM_TYPE_TAG = wxNewId();
static const long ID_RESULTS_STREAM_TYPE_DEFAULT = wxNewId();

static const long ID_RESULTS_STREAM_CONNECTION = wxNewId();
static const long ID_RESULTS_STREAM_CONNECTION_TAG = wxNewId();
static const long ID_RESULTS_STREAM_CONNECTION_DEFAULT = wxNewId();

static const long ID_RESULTS_STREAM_INDEX = wxNewId();
static const long ID_RESULTS_STREAM_INDEX_TAG = wxNewId();
static const long ID_RESULTS_STREAM_INDEX_DEFAULT = wxNewId();

static const long ID_RESULTS_VTHRESH = wxNewId();
static const long ID_RESULTS_VTHRESH_TAG = wxNewId();
static const long ID_RESULTS_VTHRESH_DEFAULT = wxNewId();

static const long ID_RESULTS_FRAME_NUM = wxNewId();
static const long ID_RESULTS_FRAME_NUM_TAG = wxNewId();
static const long ID_RESULTS_FRAME_NUM_DEFAULT = wxNewId();

static const long ID_RESULTS_UTC_TIME = wxNewId();
static const long ID_RESULTS_UTC_TIME_TAG = wxNewId();
static const long ID_RESULTS_UTC_TIME_DEFAULT = wxNewId();

static const long ID_RESULTS_RANK_N = wxNewId();
static const long ID_RESULTS_RANK_N_EDIT = wxNewId();
static const long ID_RESULTS_RANK_N_TAG = wxNewId();
static const long ID_RESULTS_RANK_N_DEFAULT = wxNewId(); 

static const long ID_RESULTS_MATCHED_PERSON_NAME = wxNewId();
static const long ID_RESULTS_MATCHED_PERSON_NAME_TAG = wxNewId();
static const long ID_RESULTS_MATCHED_PERSON_NAME_DEFAULT = wxNewId();

static const long ID_RESULTS_MATCHED_PERSON_ID = wxNewId();
static const long ID_RESULTS_MATCHED_PERSON_ID_TAG = wxNewId();
static const long ID_RESULTS_MATCHED_PERSON_ID_DEFAULT = wxNewId();

static const long ID_RESULTS_MATCHED_IMAGE_ID = wxNewId();
static const long ID_RESULTS_MATCHED_IMAGE_ID_TAG = wxNewId();
static const long ID_RESULTS_MATCHED_IMAGE_ID_DEFAULT = wxNewId();

static const long ID_RESULTS_DATE_TIME = wxNewId();
static const long ID_RESULTS_DATE_TIME_TAG = wxNewId();
static const long ID_RESULTS_DATE_TIME_DEFAULT = wxNewId();

static const long ID_RESULTS_CONFIDENCE = wxNewId();
static const long ID_RESULTS_CONFIDENCX_TAG = wxNewId();
static const long ID_RESULTS_CONFIDENCX_DEFAULT = wxNewId();

static const long ID_RESULTS_FOCUS = wxNewId();
static const long ID_RESULTS_FOCUS_TAG = wxNewId();
static const long ID_RESULTS_FOCUS_DEFAULT = wxNewId();

static const long ID_RESULTS_EYE_POS = wxNewId();
static const long ID_RESULTS_EYE_POS_TAG = wxNewId();
static const long ID_RESULTS_EYE_POS_DEFAULT = wxNewId();

static const long ID_RESULTS_FACX_BOX = wxNewId();
static const long ID_RESULTS_FACX_BOX_TAG = wxNewId();
static const long ID_RESULTS_FACX_BOX_DEFAULT = wxNewId();

static const long ID_RESULTS_MATCH_SCORE = wxNewId();
static const long ID_RESULTS_MATCH_SCORE_TAG = wxNewId();
static const long ID_RESULTS_MATCH_SCORE_DEFAULT = wxNewId();

static const long ID_RESULTS_MATCHED_STATUS = wxNewId();
static const long ID_RESULTS_MATCHED_STATUS_TAG = wxNewId();
static const long ID_RESULTS_MATCHED_STATUS_DEFAULT = wxNewId();

static const long ID_RESULTS_TRACKED_IM = wxNewId();
static const long ID_RESULTS_TRACKED_IM_TAG = wxNewId();
static const long ID_RESULTS_TRACKED_IM_DEFAULT = wxNewId();

static const long ID_RESULTS_PERSON = wxNewId();
static const long ID_RESULTS_PERSON_TAG = wxNewId();
static const long ID_RESULTS_PERSON_DEFAULT = wxNewId();

static const long ID_RESULTS_IMAGE = wxNewId();
static const long ID_RESULTS_IMAGE_TAG = wxNewId();
static const long ID_RESULTS_IMAGE_DEFAULT = wxNewId();



static const long ID_RESULTS_SET_ALL = wxNewId();
static const long ID_RESULTS_OK = wxNewId();
static const long ID_RESULTS_CANCEL = wxNewId();
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_ResultsPanel, wxPanel)
//EVT_SIZE(CX_ResultsPanel::OnSize)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
wxStaticBoxSizer* CX_ResultsPanel::CreateTagItem(wxWindow* parent, const char* title, wxWindowID check_ID, wxWindowID editID, wxWindowID defaultEditID, bool large)
{
  wxSize tag_size(75, 20);
  wxSize reset_size(35, 20);
  if (large)
  {
    tag_size = wxSize(130, 20);
    reset_size = wxSize(40, 20);
  }

  wxStaticBoxSizer* sz = new wxStaticBoxSizer(wxHORIZONTAL, parent, title);
  wxCheckBox* c = new wxCheckBox(sz->GetStaticBox(), check_ID, "");
  wxTextCtrl* f = new wxTextCtrl(sz->GetStaticBox(), editID, wxEmptyString, wxDefaultPosition, tag_size);
  wxButton* d = new wxButton(sz->GetStaticBox(), defaultEditID, "Reset", wxDefaultPosition, reset_size);
  sz->Add(c, 0, wxALIGN_LEFT | wxEXPAND, 1, NULL);
  sz->Add(f, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(d, 0, wxALIGN_LEFT | wxEXPAND, 1);
  return sz;
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
wxBoxSizer* CX_ResultsPanel::CreateCompact()
{
  // you can use either a single SetScrollbars() call or these 2 functions,
  // usually using them is better because you normally won't need to change
  // the scroll rate in the future and the sizer can be used to update the
  // virtual size automatically
  SetScrollRate(10, 10);
  SetVirtualSize(750, 1000);

  mp_mainframe = NULL;
  mp_vd_parent = NULL;

  cx_int space = 5;
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *content = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML Content");
  wxBoxSizer* v1 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* v2 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* v3 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* v4 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* v5 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer* info = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML Info");

  //////////// XML /////////////////
  wxStaticBoxSizer* xml_folder_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML Folder");
  wxTextCtrl* xml_folder_edit = new wxTextCtrl(this, ID_RESULTS_SAVE_XML_FOLDER, wxEmptyString, wxDefaultPosition, wxSize(110, 20));
  wxButton* xml_browse_button = new wxButton(this, ID_RESULTS_XML_BROWSE, "Browse", wxDefaultPosition, wxSize(50, 20));
  xml_folder_sizer->Add(xml_folder_edit, 0, wxALIGN_LEFT, 1);
  xml_folder_sizer->Add(xml_browse_button, 0, wxALIGN_RIGHT, 1);


  wxBoxSizer *xml_checks = new wxBoxSizer(wxVERTICAL);
  wxCheckBox* save_xml = new wxCheckBox(this, ID_RESULTS_SAVE_XML, "Save XML");
  wxCheckBox* remove_files = new wxCheckBox(this, ID_RESULTS_REMOVE_FILES, "Remove");
  wxCheckBox* only_verified = new wxCheckBox(this, ID_RESULTS_ONLY_VERIFIED, "Verified");
  xml_checks->Add(save_xml, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);
  xml_checks->Add(remove_files, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);
  xml_checks->Add(only_verified, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);

  wxStaticBoxSizer *hbox1 = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML");

  hbox1->Add(xml_checks);
  hbox1->Add(xml_folder_sizer, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);
  info->Add(hbox1, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);

  ///////// POST ////////////////
  wxStaticBoxSizer* post_url_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "POST URL");
  wxTextCtrl* post_url_edit = new wxTextCtrl(this, ID_RESULTS_POST_XML_URL, wxEmptyString, wxDefaultPosition, wxSize(110, 20));
  post_url_sizer->Add(post_url_edit);

  wxStaticBoxSizer *hbox2 = new wxStaticBoxSizer(wxHORIZONTAL, this, "POST");
  wxCheckBox* post_xml = new wxCheckBox(this, ID_RESULTS_POST_XML, "Use");
  hbox2->Add(post_xml);
  hbox2->Add(post_url_sizer);
  info->Add(hbox2, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);


  ///////// SAVE IMAGES ////////////////
  wxStaticBoxSizer* save_images_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Save Folder");
  wxTextCtrl* images_folder_edit = new wxTextCtrl(this, ID_RESULTS_SAVE_IMAGES_FOLDER, wxEmptyString, wxDefaultPosition, wxSize(110, 20));
  wxButton* images_browse_button = new wxButton(this, ID_RESULTS_IMAGES_BROWSE, "Browse", wxDefaultPosition, wxSize(50, 20));
  save_images_sizer->Add(images_folder_edit);
  save_images_sizer->Add(images_browse_button);

  wxStaticBoxSizer *hbox3 = new wxStaticBoxSizer(wxHORIZONTAL, this, "Images");
  wxCheckBox* save_images = new wxCheckBox(this, ID_RESULTS_SAVE_IMAGES, "Save");
  hbox3->Add(save_images);
  hbox3->Add(save_images_sizer);
  info->Add(hbox3, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);


  //////////// XML content ////////////////
  v1->Add(CreateTagItem(content->GetStaticBox(), "Root Tag", ID_RESULTS_ROOT, ID_RESULTS_ROOT_TAG, ID_RESULTS_ROOT_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Person Name", ID_RESULTS_PERSON_NAME, ID_RESULTS_PERSON_NAME_TAG, ID_RESULTS_PERSON_NAME_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Person ID", ID_RESULTS_PERSON_ID, ID_RESULTS_PERSON_ID_TAG, ID_RESULTS_PERSON_ID_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Processed Frames", ID_RESULTS_PROCESSED_FRAMES, ID_RESULTS_PROCESSED_FRAMES_TAG, ID_RESULTS_PROCESSED_FRAMES_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Head ID", ID_RESULTS_HEAD_ID, ID_RESULTS_HEAD_ID_TAG, ID_RESULTS_HEAD_ID_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Stream Type", ID_RESULTS_STREAM_TYPE, ID_RESULTS_STREAM_TYPE_TAG, ID_RESULTS_STREAM_TYPE_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Stream Connection", ID_RESULTS_STREAM_CONNECTION, ID_RESULTS_STREAM_CONNECTION_TAG, ID_RESULTS_STREAM_CONNECTION_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Stream Index", ID_RESULTS_STREAM_INDEX, ID_RESULTS_STREAM_INDEX_TAG, ID_RESULTS_STREAM_INDEX_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Verification Thresh", ID_RESULTS_VTHRESH, ID_RESULTS_VTHRESH_TAG, ID_RESULTS_VTHRESH_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Frame Number", ID_RESULTS_FRAME_NUM, ID_RESULTS_FRAME_NUM_TAG, ID_RESULTS_FRAME_NUM_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v3->Add(CreateTagItem(content->GetStaticBox(), "UTC Time", ID_RESULTS_UTC_TIME, ID_RESULTS_UTC_TIME_TAG, ID_RESULTS_UTC_TIME_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);

  // ID_RESULTS_RANK_N (it has two edit boxes, first one is the tag last one is the amount)
  wxStaticBoxSizer* sz = new wxStaticBoxSizer(wxHORIZONTAL, content->GetStaticBox(), "Ranked Results");
  wxCheckBox* c = new wxCheckBox(sz->GetStaticBox(), ID_RESULTS_RANK_N, "");
  wxTextCtrl* f1 = new wxTextCtrl(sz->GetStaticBox(), ID_RESULTS_RANK_N_TAG, wxEmptyString, wxDefaultPosition, wxSize(50, 20));
  wxTextCtrl* f2 = new wxTextCtrl(sz->GetStaticBox(), ID_RESULTS_RANK_N_EDIT, wxEmptyString, wxDefaultPosition, wxSize(25, 20));
  wxButton* d = new wxButton(sz->GetStaticBox(), ID_RESULTS_RANK_N_DEFAULT, "Reset", wxDefaultPosition, wxSize(35, 20));
  sz->Add(c, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(f1, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(d, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(f2, 0, wxALIGN_LEFT | wxEXPAND, 1);
  v3->Add(sz, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);


  v3->Add(CreateTagItem(content->GetStaticBox(), "Matched Person Name", ID_RESULTS_MATCHED_PERSON_NAME, ID_RESULTS_MATCHED_PERSON_NAME_TAG, ID_RESULTS_MATCHED_PERSON_NAME_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v3->Add(CreateTagItem(content->GetStaticBox(), "Matched Person ID", ID_RESULTS_MATCHED_PERSON_ID, ID_RESULTS_MATCHED_PERSON_ID_TAG, ID_RESULTS_MATCHED_PERSON_ID_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v3->Add(CreateTagItem(content->GetStaticBox(), "Matched Image ID", ID_RESULTS_MATCHED_IMAGE_ID, ID_RESULTS_MATCHED_IMAGE_ID_TAG, ID_RESULTS_MATCHED_IMAGE_ID_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v4->Add(CreateTagItem(content->GetStaticBox(), "Date Time", ID_RESULTS_DATE_TIME, ID_RESULTS_DATE_TIME_TAG, ID_RESULTS_DATE_TIME_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v4->Add(CreateTagItem(content->GetStaticBox(), "Confidence", ID_RESULTS_CONFIDENCE, ID_RESULTS_CONFIDENCX_TAG, ID_RESULTS_CONFIDENCX_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v4->Add(CreateTagItem(content->GetStaticBox(), "Focus", ID_RESULTS_FOCUS, ID_RESULTS_FOCUS_TAG, ID_RESULTS_FOCUS_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v4->Add(CreateTagItem(content->GetStaticBox(), "Eye Positions", ID_RESULTS_EYE_POS, ID_RESULTS_EYE_POS_TAG, ID_RESULTS_EYE_POS_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v4->Add(CreateTagItem(content->GetStaticBox(), "Face Box", ID_RESULTS_FACX_BOX, ID_RESULTS_FACX_BOX_TAG, ID_RESULTS_FACX_BOX_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v5->Add(CreateTagItem(content->GetStaticBox(), "Match Score", ID_RESULTS_MATCH_SCORE, ID_RESULTS_MATCH_SCORE_TAG, ID_RESULTS_MATCH_SCORE_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v5->Add(CreateTagItem(content->GetStaticBox(), "Matched Status", ID_RESULTS_MATCHED_STATUS, ID_RESULTS_MATCHED_STATUS_TAG, ID_RESULTS_MATCHED_STATUS_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v5->Add(CreateTagItem(content->GetStaticBox(), "Tracked Image", ID_RESULTS_TRACKED_IM, ID_RESULTS_TRACKED_IM_TAG, ID_RESULTS_TRACKED_IM_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v5->Add(CreateTagItem(content->GetStaticBox(), "Matched Pers Thumb", ID_RESULTS_PERSON, ID_RESULTS_PERSON_TAG, ID_RESULTS_PERSON_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v5->Add(CreateTagItem(content->GetStaticBox(), "Matched Im Thumb", ID_RESULTS_IMAGE, ID_RESULTS_IMAGE_TAG, ID_RESULTS_IMAGE_DEFAULT), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);

  content->Add(v1, 0, wxALIGN_LEFT | wxLEFT, 2);
  content->Add(v2, 0, wxALIGN_LEFT | wxLEFT, 2);
  content->Add(v3, 0, wxALIGN_LEFT | wxLEFT, 2);
  content->Add(v4, 0, wxALIGN_LEFT | wxLEFT, 2);
  content->Add(v5, 0, wxALIGN_LEFT | wxLEFT, 2);



  ////////// OK CANCEL ///////////////////////////////
  wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
  wxButton* ok_button = new wxButton(this, ID_RESULTS_OK, "OK", wxPoint(15, 100));
  wxButton* cancel_button = new wxButton(this, ID_RESULTS_CANCEL, "Cancel", wxPoint(15, 100));
  hbox5->Add(ok_button);
  hbox5->Add(cancel_button);

  vbox->Add(info, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);
  vbox->Add(content, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);
  vbox->Add(hbox5, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);

  return vbox;
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
wxBoxSizer* CX_ResultsPanel::CreateLarge()
{
  // you can use either a single SetScrollbars() call or these 2 functions,
  // usually using them is better because you normally won't need to change
  // the scroll rate in the future and the sizer can be used to update the
  // virtual size automatically
  SetScrollRate(10, 10);
  SetVirtualSize(750, 1000);

  mp_mainframe = NULL;
  mp_vd_parent = NULL;

  cx_int space = 5;
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);


  //////////// XML /////////////////
  wxStaticBoxSizer *xml = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML");

  wxStaticBoxSizer* xml_folder_sizer = new wxStaticBoxSizer(wxHORIZONTAL, xml->GetStaticBox(), "XML Folder");
  wxTextCtrl* xml_folder_edit = new wxTextCtrl(xml_folder_sizer->GetStaticBox(), ID_RESULTS_SAVE_XML_FOLDER, wxEmptyString, wxDefaultPosition, wxSize(250, 20));
  wxButton* xml_browse_button = new wxButton(xml_folder_sizer->GetStaticBox(), ID_RESULTS_XML_BROWSE, "Browse", wxDefaultPosition, wxSize(50, 20));
  xml_folder_sizer->Add(xml_folder_edit, 0, wxALIGN_LEFT, 1);
  xml_folder_sizer->Add(xml_browse_button, 0, wxALIGN_RIGHT, 1);


  wxBoxSizer *xml_checks = new wxBoxSizer(wxVERTICAL);
  wxCheckBox* save_xml = new wxCheckBox(this, ID_RESULTS_SAVE_XML, "Save XML");
  wxCheckBox* remove_files = new wxCheckBox(this, ID_RESULTS_REMOVE_FILES, "Remove Files");
  wxCheckBox* only_verified = new wxCheckBox(this, ID_RESULTS_ONLY_VERIFIED, "Only Verified");
  xml_checks->Add(save_xml, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);
  xml_checks->Add(remove_files, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);
  xml_checks->Add(only_verified, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 0);

  xml->Add(xml_checks);
  xml->Add(xml_folder_sizer, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);
  vbox->Add(xml, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);


  ///////// POST ////////////////
  wxStaticBoxSizer* post_url_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "POST URL");
  wxTextCtrl* post_url_edit = new wxTextCtrl(this, ID_RESULTS_POST_XML_URL, wxEmptyString, wxDefaultPosition, wxSize(350, 20));
  post_url_sizer->Add(post_url_edit);

  wxStaticBoxSizer *post = new wxStaticBoxSizer(wxHORIZONTAL, this, "POST");
  wxCheckBox* post_xml = new wxCheckBox(this, ID_RESULTS_POST_XML, "Use");
  post->Add(post_xml);
  post->Add(post_url_sizer);
  vbox->Add(post, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);


  ///////// SAVE IMAGES ////////////////
  wxStaticBoxSizer* save_images_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Save Folder");
  wxTextCtrl* images_folder_edit = new wxTextCtrl(this, ID_RESULTS_SAVE_IMAGES_FOLDER, wxEmptyString, wxDefaultPosition, wxSize(290, 20));
  wxButton* images_browse_button = new wxButton(this, ID_RESULTS_IMAGES_BROWSE, "Browse", wxDefaultPosition, wxSize(50, 20));
  save_images_sizer->Add(images_folder_edit);
  save_images_sizer->Add(images_browse_button);

  wxStaticBoxSizer *sim = new wxStaticBoxSizer(wxHORIZONTAL, this, "Images");
  wxCheckBox* save_images = new wxCheckBox(this, ID_RESULTS_SAVE_IMAGES, "Save");
  sim->Add(save_images);
  sim->Add(save_images_sizer);
  vbox->Add(sim, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);


  //////////// XML content ////////////////

  wxStaticBoxSizer *content = new wxStaticBoxSizer(wxHORIZONTAL, this, "XML Content");
  wxBoxSizer* v1 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* v2 = new wxBoxSizer(wxVERTICAL);

  v1->Add(CreateTagItem(content->GetStaticBox(), "Root Tag", ID_RESULTS_ROOT, ID_RESULTS_ROOT_TAG, ID_RESULTS_ROOT_DEFAULT,true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Person Name", ID_RESULTS_PERSON_NAME, ID_RESULTS_PERSON_NAME_TAG, ID_RESULTS_PERSON_NAME_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Person ID", ID_RESULTS_PERSON_ID, ID_RESULTS_PERSON_ID_TAG, ID_RESULTS_PERSON_ID_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Processed Frames", ID_RESULTS_PROCESSED_FRAMES, ID_RESULTS_PROCESSED_FRAMES_TAG, ID_RESULTS_PROCESSED_FRAMES_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Head ID", ID_RESULTS_HEAD_ID, ID_RESULTS_HEAD_ID_TAG, ID_RESULTS_HEAD_ID_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Stream Type", ID_RESULTS_STREAM_TYPE, ID_RESULTS_STREAM_TYPE_TAG, ID_RESULTS_STREAM_TYPE_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Stream Connection", ID_RESULTS_STREAM_CONNECTION, ID_RESULTS_STREAM_CONNECTION_TAG, ID_RESULTS_STREAM_CONNECTION_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Stream Index", ID_RESULTS_STREAM_INDEX, ID_RESULTS_STREAM_INDEX_TAG, ID_RESULTS_STREAM_INDEX_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Verification Thresh", ID_RESULTS_VTHRESH, ID_RESULTS_VTHRESH_TAG, ID_RESULTS_VTHRESH_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "Frame Number", ID_RESULTS_FRAME_NUM, ID_RESULTS_FRAME_NUM_TAG, ID_RESULTS_FRAME_NUM_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v1->Add(CreateTagItem(content->GetStaticBox(), "UTC Time", ID_RESULTS_UTC_TIME, ID_RESULTS_UTC_TIME_TAG, ID_RESULTS_UTC_TIME_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);

  // ID_RESULTS_RANK_N (it has two edit boxes, first one is the tag last one is the amount)
  wxStaticBoxSizer* sz = new wxStaticBoxSizer(wxHORIZONTAL, content->GetStaticBox(), "Ranked Results");
  wxCheckBox* c = new wxCheckBox(sz->GetStaticBox(), ID_RESULTS_RANK_N, "");
  wxTextCtrl* f1 = new wxTextCtrl(sz->GetStaticBox(), ID_RESULTS_RANK_N_TAG, wxEmptyString, wxDefaultPosition, wxSize(105, 20));
  wxTextCtrl* f2 = new wxTextCtrl(sz->GetStaticBox(), ID_RESULTS_RANK_N_EDIT, wxEmptyString, wxDefaultPosition, wxSize(25, 20));
  wxButton* d = new wxButton(sz->GetStaticBox(), ID_RESULTS_RANK_N_DEFAULT, "Reset", wxDefaultPosition, wxSize(40, 20));
  sz->Add(c, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(f1, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(d, 0, wxALIGN_LEFT | wxEXPAND, 1);
  sz->Add(f2, 0, wxALIGN_LEFT | wxEXPAND, 1);
  v1->Add(sz, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);


  v1->Add(CreateTagItem(content->GetStaticBox(), "Matched Person Name", ID_RESULTS_MATCHED_PERSON_NAME, ID_RESULTS_MATCHED_PERSON_NAME_TAG, ID_RESULTS_MATCHED_PERSON_NAME_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Matched Person ID", ID_RESULTS_MATCHED_PERSON_ID, ID_RESULTS_MATCHED_PERSON_ID_TAG, ID_RESULTS_MATCHED_PERSON_ID_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Matched Image ID", ID_RESULTS_MATCHED_IMAGE_ID, ID_RESULTS_MATCHED_IMAGE_ID_TAG, ID_RESULTS_MATCHED_IMAGE_ID_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Date Time", ID_RESULTS_DATE_TIME, ID_RESULTS_DATE_TIME_TAG, ID_RESULTS_DATE_TIME_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Confidence", ID_RESULTS_CONFIDENCE, ID_RESULTS_CONFIDENCX_TAG, ID_RESULTS_CONFIDENCX_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Focus", ID_RESULTS_FOCUS, ID_RESULTS_FOCUS_TAG, ID_RESULTS_FOCUS_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Eye Positions", ID_RESULTS_EYE_POS, ID_RESULTS_EYE_POS_TAG, ID_RESULTS_EYE_POS_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Face Box", ID_RESULTS_FACX_BOX, ID_RESULTS_FACX_BOX_TAG, ID_RESULTS_FACX_BOX_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Match Score", ID_RESULTS_MATCH_SCORE, ID_RESULTS_MATCH_SCORE_TAG, ID_RESULTS_MATCH_SCORE_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Matched Status", ID_RESULTS_MATCHED_STATUS, ID_RESULTS_MATCHED_STATUS_TAG, ID_RESULTS_MATCHED_STATUS_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Tracked Image", ID_RESULTS_TRACKED_IM, ID_RESULTS_TRACKED_IM_TAG, ID_RESULTS_TRACKED_IM_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Matched Pers Thumb", ID_RESULTS_PERSON, ID_RESULTS_PERSON_TAG, ID_RESULTS_PERSON_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);
  v2->Add(CreateTagItem(content->GetStaticBox(), "Matched Im Thumb", ID_RESULTS_IMAGE, ID_RESULTS_IMAGE_TAG, ID_RESULTS_IMAGE_DEFAULT, true), 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 2);

  content->Add(v1, 0, wxALIGN_LEFT | wxLEFT, 2);
  content->Add(v2, 0, wxALIGN_LEFT | wxLEFT, 2);



  ////////// OK CANCEL ///////////////////////////////
  wxBoxSizer *okcan = new wxBoxSizer(wxHORIZONTAL);
  wxButton* set_all_button = new wxButton(this, ID_RESULTS_SET_ALL, "Save To All Streams", wxPoint(15, 100));
  wxButton* ok_button = new wxButton(this, ID_RESULTS_OK, "SAVE", wxPoint(15, 100));
  wxButton* cancel_button = new wxButton(this, ID_RESULTS_CANCEL, "Cancel", wxPoint(15, 100));
  okcan->Add(set_all_button);
  okcan->Add(ok_button);
  okcan->Add(cancel_button);

  vbox->Add(content, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);
  vbox->Add(okcan, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM, 1);

  return vbox;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_ResultsPanel::CX_ResultsPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxScrolled<wxPanel>(parent, id, pos, size, style, name)
{
  // creates a compact view of the options
  //wxBoxSizer* vbox = CreateCompact();

  // creates a larger, more easy to read view or the options
  wxBoxSizer* vbox = CreateLarge();

  // OK/Cancel 
  Connect(ID_RESULTS_SET_ALL, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnSetAllButton);
  Connect(ID_RESULTS_OK, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnOKButton);
  Connect(ID_RESULTS_CANCEL, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnCancelButton);

  // XML
  Connect(ID_RESULTS_SAVE_XML, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnSaveXMLcheck);
  Connect(ID_RESULTS_REMOVE_FILES, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnRemoveFilesCheck);
  Connect(ID_RESULTS_ONLY_VERIFIED, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnOnlyVerifiedCheck);
  Connect(ID_RESULTS_SAVE_XML_FOLDER, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnXMLfolderEdit);
  Connect(ID_RESULTS_XML_BROWSE, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnXMLbrowse);

  // POST
  Connect(ID_RESULTS_POST_XML, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnPOSTcheck);
  Connect(ID_RESULTS_POST_XML_URL, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnPOSTedit);

  // Images
  Connect(ID_RESULTS_SAVE_IMAGES, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnSaveImagesCheck);
  Connect(ID_RESULTS_SAVE_IMAGES_FOLDER, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnImagesFolder);
  Connect(ID_RESULTS_IMAGES_BROWSE, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnImagesBrowse);

  // XML content
  Connect(ID_RESULTS_ROOT, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnRootTagCheck);
  Connect(ID_RESULTS_ROOT_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnRootTagTag);
  Connect(ID_RESULTS_ROOT_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnRootTagDefault);

  Connect(ID_RESULTS_PERSON_NAME, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonNameCheck);
  Connect(ID_RESULTS_PERSON_NAME_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonNameTag);
  Connect(ID_RESULTS_PERSON_NAME_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonNameDefault);

  Connect(ID_RESULTS_PERSON_ID, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonIDCheck);
  Connect(ID_RESULTS_PERSON_ID_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonIDTag);
  Connect(ID_RESULTS_PERSON_ID_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonIDDefault);

  Connect(ID_RESULTS_PROCESSED_FRAMES, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnProcessedFramesCheck);
  Connect(ID_RESULTS_PROCESSED_FRAMES_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnProcessedFramesTag);
  Connect(ID_RESULTS_PROCESSED_FRAMES_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnProcessedFramesDefault);

  Connect(ID_RESULTS_HEAD_ID, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnHeadIdCheck);
  Connect(ID_RESULTS_HEAD_ID_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnHeadIdTag);
  Connect(ID_RESULTS_HEAD_ID_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnHeadIdDefault);

  Connect(ID_RESULTS_STREAM_TYPE, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamTypeCheck);
  Connect(ID_RESULTS_STREAM_TYPE_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamTypeTag);
  Connect(ID_RESULTS_STREAM_TYPE_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamTypeDefault);

  Connect(ID_RESULTS_STREAM_CONNECTION, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamConnectionCheck);
  Connect(ID_RESULTS_STREAM_CONNECTION_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamConnectionTag);
  Connect(ID_RESULTS_STREAM_CONNECTION_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamConnectionDefault);

  Connect(ID_RESULTS_STREAM_INDEX, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamIndexCheck);
  Connect(ID_RESULTS_STREAM_INDEX_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamIndexTag);
  Connect(ID_RESULTS_STREAM_INDEX_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnStreamIndexDefault);

  Connect(ID_RESULTS_VTHRESH, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnVThreshCheck);
  Connect(ID_RESULTS_VTHRESH_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnVThreshTag);
  Connect(ID_RESULTS_VTHRESH_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnVThreshDefault);

  Connect(ID_RESULTS_FRAME_NUM, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnFrameNumCheck);
  Connect(ID_RESULTS_FRAME_NUM_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnFrameNumTag);
  Connect(ID_RESULTS_FRAME_NUM_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnFrameNumDefault);

  Connect(ID_RESULTS_UTC_TIME, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnUtcTimeCheck);
  Connect(ID_RESULTS_UTC_TIME_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnUtcTimeTag);
  Connect(ID_RESULTS_UTC_TIME_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnUtcTimeDefault);

  Connect(ID_RESULTS_RANK_N, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnRankNCheck);
  Connect(ID_RESULTS_RANK_N_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnRankNTag);
  Connect(ID_RESULTS_RANK_N_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnRankNDefault);
  Connect(ID_RESULTS_RANK_N_EDIT, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnRankNedit);

  Connect(ID_RESULTS_MATCHED_PERSON_NAME, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonNameCheck);
  Connect(ID_RESULTS_MATCHED_PERSON_NAME_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonNameTag);
  Connect(ID_RESULTS_MATCHED_PERSON_NAME_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonNameDefault);

  Connect(ID_RESULTS_MATCHED_PERSON_ID, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonIDCheck);
  Connect(ID_RESULTS_MATCHED_PERSON_ID_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonIDTag);
  Connect(ID_RESULTS_MATCHED_PERSON_ID_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedPersonIDDefault);

  Connect(ID_RESULTS_MATCHED_IMAGE_ID, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageIDCheck);
  Connect(ID_RESULTS_MATCHED_IMAGE_ID_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageIDTag);
  Connect(ID_RESULTS_MATCHED_IMAGE_ID_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageIDDefault);

  Connect(ID_RESULTS_DATE_TIME, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnDateTimeCheck);
  Connect(ID_RESULTS_DATE_TIME_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnDateTimeTag);
  Connect(ID_RESULTS_DATE_TIME_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnDateTimeDefault);

  Connect(ID_RESULTS_CONFIDENCE, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnConfidenceCheck);
  Connect(ID_RESULTS_CONFIDENCX_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnConfidenceTag);
  Connect(ID_RESULTS_CONFIDENCX_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnConfidenceDefault);

  Connect(ID_RESULTS_FOCUS, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnFocusCheck);
  Connect(ID_RESULTS_FOCUS_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnFocusTag);
  Connect(ID_RESULTS_FOCUS_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnFocusDefault);

  Connect(ID_RESULTS_EYE_POS, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnEyePosCheck);
  Connect(ID_RESULTS_EYE_POS_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnEyePosTag);
  Connect(ID_RESULTS_EYE_POS_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnEyePosDefault);

  Connect(ID_RESULTS_FACX_BOX, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnFaceBoxCheck);
  Connect(ID_RESULTS_FACX_BOX_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnFaceBoxTag);
  Connect(ID_RESULTS_FACX_BOX_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnFaceBoxDefault);

  Connect(ID_RESULTS_MATCH_SCORE, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchScoreCheck);
  Connect(ID_RESULTS_MATCH_SCORE_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchScoreTag);
  Connect(ID_RESULTS_MATCH_SCORE_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchScoreDefault);

  Connect(ID_RESULTS_MATCHED_STATUS, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedStatusCheck);
  Connect(ID_RESULTS_MATCHED_STATUS_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedStatusTag);
  Connect(ID_RESULTS_MATCHED_STATUS_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedStatusDefault);

  Connect(ID_RESULTS_TRACKED_IM, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnTrackedImCheck);
  Connect(ID_RESULTS_TRACKED_IM_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnTrackedImTag);
  Connect(ID_RESULTS_TRACKED_IM_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnTrackedImDefault);

  Connect(ID_RESULTS_PERSON, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonImageCheck);
  Connect(ID_RESULTS_PERSON_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonImageTag);
  Connect(ID_RESULTS_PERSON_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnPersonImageDefault);

  Connect(ID_RESULTS_IMAGE, wxEVT_CHECKBOX, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageCheck);
  Connect(ID_RESULTS_IMAGE_TAG, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageTag);
  Connect(ID_RESULTS_IMAGE_DEFAULT, wxEVT_BUTTON, (wxObjectEventFunction)&CX_ResultsPanel::OnMatchedImageDefault);

  SetSizer(vbox);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_ResultsPanel::~CX_ResultsPanel()
{
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::SetCheckValue(wxWindowID id, bool value)
{
  wxCheckBox* c = (wxCheckBox*)this->FindWindow(id);
  if (c) c->SetValue(value);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::SetTextValue(wxWindowID id, const char* txt)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(id);
  if (c) c->SetValue(txt);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::SetValues(CX_VIDEO p_video)
{
  if (!p_video) return; 
  mp_video = p_video;
  char msg[1024];
  if (0 == CX_GetResultsParameters(p_video, &m_res, msg)) 
  {
    wxMessageBox(msg);
    return;
  }

  if (0 == CX_GetDefaultResultsParameters(p_video, &m_default, msg))
  {
    wxMessageBox(msg);
    return;
  }

  // XML
  SetCheckValue(ID_RESULTS_SAVE_XML, m_res.m_save_xml_results);
  SetTextValue(ID_RESULTS_SAVE_XML_FOLDER, m_res.m_save_xml_folder);
  SetCheckValue(ID_RESULTS_REMOVE_FILES, m_res.m_remove_files);
  SetCheckValue(ID_RESULTS_ONLY_VERIFIED, m_res.m_save_only_verified);
  // POST
  SetCheckValue(ID_RESULTS_POST_XML, m_res.m_post_xml);
  SetTextValue(ID_RESULTS_POST_XML_URL, m_res.m_post_url);
  // Images
  SetCheckValue(ID_RESULTS_SAVE_IMAGES, m_res.m_save_images);
  SetTextValue(ID_RESULTS_SAVE_IMAGES_FOLDER, m_res.m_save_images_folder);

  // XML Content
  SetCheckValue(ID_RESULTS_ROOT, true);
  SetTextValue(ID_RESULTS_ROOT_TAG, m_res.m_root_tag);
  SetCheckValue(ID_RESULTS_PERSON_NAME, m_res.m_person_name);
  SetTextValue(ID_RESULTS_PERSON_NAME_TAG, m_res.m_person_name_tag);
  SetCheckValue(ID_RESULTS_PERSON_ID, m_res.m_person_id);
  SetTextValue(ID_RESULTS_PERSON_ID_TAG, m_res.m_person_id_tag);
  SetCheckValue(ID_RESULTS_PROCESSED_FRAMES, m_res.m_processed_frames);
  SetTextValue(ID_RESULTS_PROCESSED_FRAMES_TAG, m_res.m_processed_frames_tag);
  SetCheckValue(ID_RESULTS_HEAD_ID, m_res.m_head_id);
  SetTextValue(ID_RESULTS_HEAD_ID_TAG, m_res.m_head_id_tag);
  SetCheckValue(ID_RESULTS_STREAM_TYPE, m_res.m_stream_type);
  SetTextValue(ID_RESULTS_STREAM_TYPE_TAG, m_res.m_stream_type_tag);
  SetCheckValue(ID_RESULTS_STREAM_CONNECTION, m_res.m_stream_connection_info);
  SetTextValue(ID_RESULTS_STREAM_CONNECTION_TAG, m_res.m_stream_connection_info_tag);
  SetCheckValue(ID_RESULTS_STREAM_INDEX, m_res.m_stream_index);
  SetTextValue(ID_RESULTS_STREAM_INDEX_TAG, m_res.m_stream_index_tag);
  SetCheckValue(ID_RESULTS_VTHRESH, m_res.m_verification_threshold);
  SetTextValue(ID_RESULTS_VTHRESH_TAG, m_res.m_verification_threshold_tag);
  SetCheckValue(ID_RESULTS_FRAME_NUM, m_res.m_frame_number);
  SetTextValue(ID_RESULTS_FRAME_NUM_TAG, m_res.m_frame_number_tag);
  SetCheckValue(ID_RESULTS_UTC_TIME, m_res.m_utc_time);
  SetTextValue(ID_RESULTS_UTC_TIME_TAG, m_res.m_utc_time_tag);
  SetCheckValue(ID_RESULTS_RANK_N, m_res.m_ranked_results);
  SetTextValue(ID_RESULTS_RANK_N_TAG, m_res.m_ranked_results_tag);
  SetTextValue(ID_RESULTS_RANK_N_EDIT, to_string(m_res.m_ranked_results_n).c_str());
  SetCheckValue(ID_RESULTS_MATCHED_PERSON_NAME, m_res.m_matched_person_name);
  SetTextValue(ID_RESULTS_MATCHED_PERSON_NAME_TAG, m_res.m_matched_person_name_tag);
  SetCheckValue(ID_RESULTS_MATCHED_PERSON_ID, m_res.m_matched_person_id);
  SetTextValue(ID_RESULTS_MATCHED_PERSON_ID_TAG, m_res.m_matched_person_id_tag);
  SetCheckValue(ID_RESULTS_MATCHED_IMAGE_ID, m_res.m_matched_image_id);
  SetTextValue(ID_RESULTS_MATCHED_IMAGE_ID_TAG, m_res.m_matched_image_id_tag);
  SetCheckValue(ID_RESULTS_DATE_TIME, m_res.m_date_time_stamp);
  SetTextValue(ID_RESULTS_DATE_TIME_TAG, m_res.m_date_time_stamp_tag);
  SetCheckValue(ID_RESULTS_CONFIDENCE, m_res.m_confidence_measure);
  SetTextValue(ID_RESULTS_CONFIDENCX_TAG, m_res.m_confidence_measure_tag);
  SetCheckValue(ID_RESULTS_FOCUS, m_res.m_focus_measure);
  SetTextValue(ID_RESULTS_FOCUS_TAG, m_res.m_focus_measure_tag);
  SetCheckValue(ID_RESULTS_EYE_POS, m_res.m_eye_positions);
  SetTextValue(ID_RESULTS_EYE_POS_TAG, m_res.m_eye_positions_tag);
  SetCheckValue(ID_RESULTS_FACX_BOX, m_res.m_face_box);
  SetTextValue(ID_RESULTS_FACX_BOX_TAG, m_res.m_face_box_tag);
  SetCheckValue(ID_RESULTS_MATCH_SCORE, m_res.m_match_score);
  SetTextValue(ID_RESULTS_MATCH_SCORE_TAG, m_res.m_match_score_tag);
  SetCheckValue(ID_RESULTS_MATCHED_STATUS, m_res.m_matched_status);
  SetTextValue(ID_RESULTS_MATCHED_STATUS_TAG, m_res.m_matched_status_tag);
  SetCheckValue(ID_RESULTS_TRACKED_IM, m_res.m_tracked_image);
  SetTextValue(ID_RESULTS_TRACKED_IM_TAG, m_res.m_tracked_image_tag);
  SetCheckValue(ID_RESULTS_PERSON, m_res.m_matched_person_thumbnail);
  SetTextValue(ID_RESULTS_PERSON_TAG, m_res.m_matched_person_thumbnail_tag);
  SetCheckValue(ID_RESULTS_IMAGE, m_res.m_matched_thumbnail);
  SetTextValue(ID_RESULTS_IMAGE_TAG, m_res.m_matched_thumbnail_tag);
}
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnOKButton(wxCommandEvent& event)
{
  //this->Show(false);
  if (mp_mainframe && mp_video)
  {
    char msg[1024];
    if (0 == CX_SetResultsParameters(mp_video, m_res, msg))
    {
      wxMessageBox(msg);
    }
    if (mp_vd_parent) mp_vd_parent->ShowResults(false);
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnSetAllButton(wxCommandEvent& event)
{
  if (!mp_mainframe) return;

  string txt = string("Are you sure you want set all video streams\nsave setting to these values: ");

  int answer = wxMessageBox(txt.c_str(), "Confirm", wxYES_NO | wxCANCEL);
  if (answer == wxYES)
  {
    char msg[1024];
    if (0 == CX_SetALLResultsParameters(mp_mainframe->mp_aureus, m_res, msg))
    {
      wxMessageBox(msg);
    }
    if (mp_vd_parent) mp_vd_parent->ShowResults(false);
  }
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnCancelButton(wxCommandEvent& event)
{
  //this->Show(false);
  if (mp_mainframe)
  {
    if (mp_vd_parent) mp_vd_parent->ShowResults(false);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::GetCheckValue(wxWindowID id, bool& value)
{
  wxCheckBox* c = (wxCheckBox*)this->FindWindow(id); 
  if (!c) return;
  value = c->GetValue();
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::GetTextValue(wxWindowID id, char* txt)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(id);
  if (!c) return;
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    strcpy(txt, s.c_str());
  }
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::GetTextNumValue(wxWindowID id, cx_uint& value, cx_int min_val, cx_int max_val)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(id);
  if (!c) return;
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_int val = atoi(s.c_str());
    if (val>=0 && val >= min_val && val <= max_val) value = (cx_uint)val;
  }
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::Browse(const char* title, wxWindowID id, char* txt)
{
  wxDirDialog dlg(this, title, txt, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (dlg.ShowModal() == wxID_OK)
  {
    strcpy(txt, dlg.GetPath().c_str());
    wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(id); 
    if (!c) return;
    c->SetValue(txt);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnSaveXMLcheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_SAVE_XML, m_res.m_save_xml_results);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRemoveFilesCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_REMOVE_FILES, m_res.m_remove_files);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnOnlyVerifiedCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_ONLY_VERIFIED, m_res.m_save_only_verified);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnXMLfolderEdit(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_SAVE_XML_FOLDER, m_res.m_save_xml_folder);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnXMLbrowse(wxCommandEvent& event)
{
  Browse("Choose a folder in which to save XML files", ID_RESULTS_SAVE_XML_FOLDER, m_res.m_save_xml_folder);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPOSTcheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_POST_XML, m_res.m_post_xml);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPOSTedit(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_POST_XML_URL, m_res.m_post_url);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnSaveImagesCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_SAVE_IMAGES, m_res.m_save_images);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnImagesFolder(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_SAVE_IMAGES_FOLDER, m_res.m_save_images_folder);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnImagesBrowse(wxCommandEvent& event)
{
  Browse("Choose a folder in which to save images", ID_RESULTS_SAVE_IMAGES_FOLDER, m_res.m_save_images_folder);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRootTagCheck(wxCommandEvent& event)
{
  SetCheckValue(ID_RESULTS_ROOT, true);// always on
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRootTagTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_ROOT_TAG, m_res.m_root_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRootTagDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_root_tag, m_default.m_root_tag);
  SetTextValue(ID_RESULTS_ROOT_TAG, m_res.m_root_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonNameCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_PERSON_NAME, m_res.m_person_name);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonNameTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_PERSON_NAME_TAG, m_res.m_person_name_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonNameDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_person_name_tag, m_default.m_person_name_tag);
  SetTextValue(ID_RESULTS_PERSON_NAME_TAG, m_res.m_person_name_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonIDCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_PERSON_ID, m_res.m_person_id);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonIDTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_PERSON_ID_TAG, m_res.m_person_id_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonIDDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_person_id_tag, m_default.m_person_id_tag);
  SetTextValue(ID_RESULTS_PERSON_ID_TAG, m_res.m_person_id_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnProcessedFramesCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_PROCESSED_FRAMES, m_res.m_processed_frames);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnProcessedFramesTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_PROCESSED_FRAMES_TAG, m_res.m_processed_frames_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnProcessedFramesDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_processed_frames_tag, m_default.m_processed_frames_tag);
  SetTextValue(ID_RESULTS_PROCESSED_FRAMES_TAG, m_res.m_processed_frames_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnHeadIdCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_HEAD_ID, m_res.m_head_id);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnHeadIdTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_HEAD_ID_TAG, m_res.m_head_id_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnHeadIdDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_head_id_tag, m_default.m_head_id_tag);
  SetTextValue(ID_RESULTS_HEAD_ID_TAG, m_res.m_head_id_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamTypeCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_STREAM_TYPE, m_res.m_stream_type);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamTypeTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_STREAM_TYPE_TAG, m_res.m_stream_type_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamTypeDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_stream_type_tag, m_default.m_stream_type_tag);
  SetTextValue(ID_RESULTS_STREAM_TYPE_TAG, m_res.m_stream_type_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamConnectionCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_STREAM_CONNECTION, m_res.m_stream_connection_info);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamConnectionTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_STREAM_CONNECTION_TAG, m_res.m_stream_connection_info_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamConnectionDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_stream_connection_info_tag, m_default.m_stream_connection_info_tag);
  SetTextValue(ID_RESULTS_STREAM_CONNECTION_TAG, m_res.m_stream_connection_info_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamIndexCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_STREAM_INDEX, m_res.m_stream_index);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamIndexTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_STREAM_INDEX_TAG, m_res.m_stream_index_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnStreamIndexDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_stream_index_tag, m_default.m_stream_index_tag);
  SetTextValue(ID_RESULTS_STREAM_INDEX_TAG, m_res.m_stream_index_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnVThreshCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_VTHRESH, m_res.m_verification_threshold);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnVThreshTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_VTHRESH_TAG, m_res.m_verification_threshold_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnVThreshDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_verification_threshold_tag, m_default.m_verification_threshold_tag);
  SetTextValue(ID_RESULTS_VTHRESH_TAG, m_res.m_verification_threshold_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFrameNumCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_FRAME_NUM, m_res.m_frame_number);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFrameNumTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_FRAME_NUM_TAG, m_res.m_frame_number_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFrameNumDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_frame_number_tag, m_default.m_frame_number_tag);
  SetTextValue(ID_RESULTS_FRAME_NUM_TAG, m_res.m_frame_number_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnUtcTimeCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_UTC_TIME, m_res.m_utc_time);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnUtcTimeTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_UTC_TIME_TAG, m_res.m_utc_time_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnUtcTimeDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_frame_number_tag, m_default.m_utc_time_tag);
  SetTextValue(ID_RESULTS_UTC_TIME_TAG, m_res.m_utc_time_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRankNCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_RANK_N, m_res.m_ranked_results);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRankNTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_RANK_N_TAG, m_res.m_ranked_results_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRankNDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_ranked_results_tag, m_default.m_ranked_results_tag);
  SetTextValue(ID_RESULTS_RANK_N_TAG, m_res.m_ranked_results_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnRankNedit(wxCommandEvent& event)
{
  GetTextNumValue(ID_RESULTS_RANK_N_EDIT, m_res.m_ranked_results_n, 0, 10);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonNameCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_MATCHED_PERSON_NAME, m_res.m_matched_person_name);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonNameTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_MATCHED_PERSON_NAME_TAG, m_res.m_matched_person_name_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonNameDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_person_name_tag, m_default.m_matched_person_name_tag);
  SetTextValue(ID_RESULTS_MATCHED_PERSON_NAME_TAG, m_res.m_matched_person_name_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonIDCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_MATCHED_PERSON_ID, m_res.m_matched_person_id);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonIDTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_MATCHED_PERSON_ID_TAG, m_res.m_matched_person_id_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedPersonIDDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_person_id_tag, m_default.m_matched_person_id_tag);
  SetTextValue(ID_RESULTS_MATCHED_PERSON_ID_TAG, m_res.m_matched_person_id_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageIDCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_MATCHED_IMAGE_ID, m_res.m_matched_image_id);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageIDTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_MATCHED_IMAGE_ID_TAG, m_res.m_matched_image_id_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageIDDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_image_id_tag, m_default.m_matched_image_id_tag);
  SetTextValue(ID_RESULTS_MATCHED_IMAGE_ID_TAG, m_res.m_matched_image_id_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnDateTimeCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_DATE_TIME, m_res.m_date_time_stamp);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnDateTimeTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_DATE_TIME_TAG, m_res.m_date_time_stamp_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnDateTimeDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_date_time_stamp_tag, m_default.m_date_time_stamp_tag);
  SetTextValue(ID_RESULTS_DATE_TIME_TAG, m_res.m_date_time_stamp_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnConfidenceCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_CONFIDENCE, m_res.m_confidence_measure);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnConfidenceTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_CONFIDENCX_TAG, m_res.m_confidence_measure_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnConfidenceDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_confidence_measure_tag, m_default.m_confidence_measure_tag);
  SetTextValue(ID_RESULTS_CONFIDENCX_TAG, m_res.m_confidence_measure_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFocusCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_FOCUS, m_res.m_focus_measure);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFocusTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_FOCUS_TAG, m_res.m_focus_measure_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFocusDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_focus_measure_tag, m_default.m_focus_measure_tag);
  SetTextValue(ID_RESULTS_FOCUS_TAG, m_res.m_focus_measure_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnEyePosCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_EYE_POS, m_res.m_eye_positions);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnEyePosTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_EYE_POS_TAG, m_res.m_eye_positions_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnEyePosDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_eye_positions_tag, m_default.m_eye_positions_tag);
  SetTextValue(ID_RESULTS_EYE_POS_TAG, m_res.m_eye_positions_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFaceBoxCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_FACX_BOX, m_res.m_face_box);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFaceBoxTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_FACX_BOX_TAG, m_res.m_face_box_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnFaceBoxDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_face_box_tag, m_default.m_face_box_tag);
  SetTextValue(ID_RESULTS_FACX_BOX_TAG, m_res.m_face_box_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchScoreCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_MATCH_SCORE, m_res.m_match_score);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchScoreTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_MATCH_SCORE_TAG, m_res.m_match_score_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchScoreDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_match_score_tag, m_default.m_match_score_tag);
  SetTextValue(ID_RESULTS_MATCH_SCORE_TAG, m_res.m_match_score_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedStatusCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_MATCHED_STATUS, m_res.m_matched_status);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedStatusTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_MATCHED_STATUS_TAG, m_res.m_matched_status_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedStatusDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_status_tag, m_default.m_matched_status_tag);
  SetTextValue(ID_RESULTS_MATCHED_STATUS_TAG, m_res.m_matched_status_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnTrackedImCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_TRACKED_IM, m_res.m_tracked_image);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnTrackedImTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_TRACKED_IM_TAG, m_res.m_tracked_image_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnTrackedImDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_tracked_image_tag, m_default.m_tracked_image_tag);
  SetTextValue(ID_RESULTS_TRACKED_IM_TAG, m_res.m_tracked_image_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonImageCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_PERSON, m_res.m_matched_person_thumbnail);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonImageTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_PERSON_TAG, m_res.m_matched_person_thumbnail_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnPersonImageDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_person_thumbnail_tag, m_default.m_matched_person_thumbnail_tag);
  SetTextValue(ID_RESULTS_PERSON_TAG, m_res.m_matched_person_thumbnail_tag);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageCheck(wxCommandEvent& event)
{
  GetCheckValue(ID_RESULTS_IMAGE, m_res.m_matched_thumbnail);
}
//////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageTag(wxCommandEvent& event)
{
  GetTextValue(ID_RESULTS_IMAGE_TAG, m_res.m_matched_thumbnail_tag);
}
///////////////////////////////////////////////////////////////////
void CX_ResultsPanel::OnMatchedImageDefault(wxCommandEvent& event)
{
  strcpy(m_res.m_matched_thumbnail_tag, m_default.m_matched_thumbnail_tag);
  SetTextValue(ID_RESULTS_IMAGE_TAG, m_res.m_matched_thumbnail_tag);
}
///////////////////////////////////////////////////////////////////




