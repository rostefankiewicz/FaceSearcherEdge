///////////////////////////////////////////////////////////////////
// include main wxWidgets
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/cmdline.h"
#include "wx/bmpcbox.h"
#include "wx/choice.h"
#include "wx/numdlg.h"
#include "wx/dialog.h"
#include <wx/notebook.h>
///////////////////////////////////////////////////////////////////
#include "CX_VideoDialog.h"
#include "FDPdlg.h"
#include "AureusVideoGUI.h"
#include "CX_GalleryPanel.h"
#include "CX_ResultsPanel.h"
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
static const long ID_VIDEO_TOOLBAR = wxNewId();
static const long ID_VIDEO_DISPLAY = wxNewId();
static const long ID_VIDEO_MEDIA_FILE = wxNewId();
static const long ID_VIDEO_USB_CAMERA = wxNewId();
static const long ID_VIDEO_IP_CAMERA = wxNewId();
static const long ID_VIDEO_SETTINGS = wxNewId();
static const long ID_VIDEO_PLAY = wxNewId();
static const long ID_VIDEO_PAUSE = wxNewId();
static const long ID_VIDEO_STOP = wxNewId();
static const long ID_VIDEO_ENROLL_CODE = wxNewId();

static const long ID_IMAGE_SETTINGS = wxNewId();

static const long ID_IMAGE_MENU = wxNewId();
static const long ID_IMAGE_SET_FDP = wxNewId();
static const long ID_IMAGE_FRAME_INTERVAL = wxNewId();
static const long ID_IMAGE_REDUCX_FRAME_STEP = wxNewId();
static const long ID_IMAGE_FORCE_EVERY_FRAME = wxNewId();
static const long ID_IMAGE_LOOP_MEDIA_FILES = wxNewId();


static const long ID_HEAD_DISPLAY_PERSON_ONLY = wxNewId();
static const long ID_HEAD_DISPLAY_ONLY_VERIFIED = wxNewId();
static const long ID_HEAD_DISPLAY_CURRENT_IMAGE = wxNewId();
static const long ID_HEAD_GENERATE_MESH_OVERLAY = wxNewId();
static const long ID_HEAD_GENERATE_PCIMAGE = wxNewId();
static const long ID_HEAD_DISPLAY_PC_IMAGE_OVERLAY = wxNewId();
static const long ID_HEAD_DISPLAY_PC_IMAGE = wxNewId();
static const long ID_HEAD_DISPLAY_FIT_MEASURE = wxNewId();


static const long ID_HEAD_MENU = wxNewId();


static const long ID_FR_MENU = wxNewId();
static const long ID_FR_SET_VERIFICATION_THRESH = wxNewId();
static const long ID_FR_GENERATE_TEMPLATES = wxNewId();
static const long ID_FR_PERFORM_FR = wxNewId();
static const long ID_FR_SET_RANK_DISPLAY = wxNewId();
static const long ID_FR_DISPLAY_ALL_PEOPLE = wxNewId();

static const long ID_GALLERY_MENU = wxNewId();
static const long ID_SELECT_GALLERY = wxNewId();

static const long ID_RESULTS_SETTINGS = wxNewId();

////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// CX_VideoDlg
///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_VideoDlg, wxWindow)
EVT_SIZE(CX_VideoDlg::OnSize)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::ConstructToolbar()
{
  mp_toolbar = new wxToolBar(this, ID_VIDEO_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_VIDEO_TOOLBAR"));
  mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(16, 16)));

  mp_toolbar->AddTool(ID_VIDEO_SETTINGS, "Load", cx_load32Bitmap());
  //mp_toolbar->AddSeparator();
  mp_toolbar->AddRadioTool(ID_VIDEO_MEDIA_FILE, _("File"), cx_video32Bitmap(), wxNullBitmap, "Media File");
  mp_toolbar->AddRadioTool(ID_VIDEO_USB_CAMERA, _("USB"), cx_webcam32Bitmap(), wxNullBitmap, "USB Camera");
  mp_toolbar->AddRadioTool(ID_VIDEO_IP_CAMERA, _("IP"), cx_ipcam32Bitmap(), wxNullBitmap, "IP Camera");
  
  //mp_toolbar->AddSeparator();
  mp_toolbar->AddTool(ID_IMAGE_SETTINGS, "Settings", cx_settings32Bitmap(), wxEmptyString, wxITEM_DROPDOWN);
  
  wxMenu* image_menu = new wxMenu();
  image_menu->Append(ID_IMAGE_SET_FDP, "Set face detection parameters");
  image_menu->Append(ID_IMAGE_FRAME_INTERVAL, "Set frame interval");
  image_menu->Append(ID_IMAGE_REDUCX_FRAME_STEP, "Set reduce frame step");
  image_menu->Append(ID_IMAGE_FORCE_EVERY_FRAME, "Force Every Frame", wxEmptyString, wxITEM_CHECK);
  image_menu->Append(ID_IMAGE_LOOP_MEDIA_FILES, "Loop Media Files", wxEmptyString, wxITEM_CHECK);

  wxMenu* head_menu = new wxMenu();
  head_menu->Append(ID_HEAD_DISPLAY_PERSON_ONLY, "Display Main Person Image", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_DISPLAY_ONLY_VERIFIED, "Display Only Verified", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_DISPLAY_CURRENT_IMAGE, "Display Current Head Images", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_DISPLAY_FIT_MEASURE, "Display Head Fit Measure", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_GENERATE_MESH_OVERLAY, "Display Mesh Overlay", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_GENERATE_PCIMAGE, "Generate PC Image", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_DISPLAY_PC_IMAGE_OVERLAY, "Display PC Image Overlays", wxEmptyString, wxITEM_CHECK);
  head_menu->Append(ID_HEAD_DISPLAY_PC_IMAGE, "Display PC Head Images", wxEmptyString, wxITEM_CHECK);

  

  wxMenu* fr_menu = new wxMenu();
  fr_menu->Append(ID_FR_SET_VERIFICATION_THRESH, "Set Verification Threshold", wxEmptyString, wxITEM_NORMAL);
  fr_menu->Append(ID_FR_GENERATE_TEMPLATES, "Generate Templates", wxEmptyString, wxITEM_CHECK);
  fr_menu->Append(ID_FR_PERFORM_FR, "Perform Gallery FR", wxEmptyString, wxITEM_CHECK);
  fr_menu->Append(ID_FR_SET_RANK_DISPLAY, "Set Rank Display", wxEmptyString, wxITEM_NORMAL);
  fr_menu->Append(ID_FR_DISPLAY_ALL_PEOPLE, "Display All people", wxEmptyString, wxITEM_CHECK);

  
  mp_settings_menu = new wxMenu();
  mp_settings_menu->Append(ID_IMAGE_MENU, "Image", image_menu);
  mp_settings_menu->Append(ID_HEAD_MENU, "Heads", head_menu);
  mp_settings_menu->Append(ID_FR_MENU, "FR", fr_menu);
  //mp_settings_menu->Append(ID_GALLERY_MENU, "Gallery", gal_menu);
  //mp_settings_menu->Append(ID_SELECT_GALLERY, "Select Gallery");
  mp_settings_menu->Append(ID_RESULTS_SETTINGS, "Results Settings");
  mp_toolbar->SetDropdownMenu(ID_IMAGE_SETTINGS, mp_settings_menu);
  
  //mp_toolbar->AddSeparator();
  mp_toolbar->AddTool(ID_VIDEO_ENROLL_CODE, "Enroll", cx_MultiPerson32Bitmap());
  //mp_toolbar->AddSeparator();
  mp_toolbar->AddTool(ID_VIDEO_PLAY, "Play", cx_play32Bitmap());
  mp_toolbar->AddTool(ID_VIDEO_PAUSE, "Pause", cx_pause32Bitmap());
  mp_toolbar->AddTool(ID_VIDEO_STOP, "Stop", cx_stop32Bitmap());


  mp_toolbar->Realize();

  Connect(ID_VIDEO_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnInputSettings);
  Connect(ID_VIDEO_MEDIA_FILE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnMediaFile);
  Connect(ID_VIDEO_USB_CAMERA, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnUSBcamera);
  Connect(ID_VIDEO_IP_CAMERA, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnIPcamera);
  Connect(ID_VIDEO_PLAY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnPlay);
  Connect(ID_VIDEO_PAUSE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnPause);
  Connect(ID_VIDEO_STOP, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnStop);
  Connect(ID_VIDEO_ENROLL_CODE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnEnrollCode);

  
  //////////////////////////////////////////////
  // connect the menu handlers 
  // Image
  Connect(ID_IMAGE_SET_FDP, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnSetFDP);
  Connect(ID_IMAGE_FRAME_INTERVAL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnSetFrameInterval);
  Connect(ID_IMAGE_REDUCX_FRAME_STEP, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnSetReduceFrameStep);
  Connect(ID_IMAGE_FORCE_EVERY_FRAME, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnForceEveryFrame);
  Connect(ID_IMAGE_LOOP_MEDIA_FILES, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnLoopMediaFiles);
  
  // Heads
  Connect(ID_HEAD_DISPLAY_PERSON_ONLY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayPeopleOnly);
  Connect(ID_HEAD_DISPLAY_ONLY_VERIFIED, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayOnlyVerified);
  Connect(ID_HEAD_DISPLAY_CURRENT_IMAGE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayCurrentHeadImages);
  Connect(ID_HEAD_GENERATE_MESH_OVERLAY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayMeshOverlay);
  Connect(ID_HEAD_GENERATE_PCIMAGE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnGeneratePCImage);
  Connect(ID_HEAD_DISPLAY_PC_IMAGE_OVERLAY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayPCOverlayImages);
  Connect(ID_HEAD_DISPLAY_PC_IMAGE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayPCHeadImages);
  Connect(ID_HEAD_DISPLAY_FIT_MEASURE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnDisplayFitMeasure);

  
  // FR
  Connect(ID_FR_SET_VERIFICATION_THRESH, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnFRsetVerificationThreshold);
  Connect(ID_FR_GENERATE_TEMPLATES, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnFRgenerateTemplatesToggle);
  Connect(ID_FR_PERFORM_FR, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnFRperformRankingToggle);
  Connect(ID_FR_SET_RANK_DISPLAY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnFRsetRankDisplay);
  Connect(ID_FR_DISPLAY_ALL_PEOPLE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnFRDisplayAllPeople);

  
  // Gallery
  Connect(ID_SELECT_GALLERY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnGallerySelect);
  
  // Results settings
  Connect(ID_RESULTS_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoDlg::OnResultsSettings);
  
  //////////////////////////////////////////////
  
}
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
///////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
CX_VideoDlg::CX_VideoDlg(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxPanel(parent, id, pos, size, style, name)
{
  mp_mainframe = NULL;
  mp_toolbar = NULL;
  mp_video = NULL;
  mp_grid = NULL;
  mp_results_panel = NULL;
  m_show_results_panel = false;

  ConstructToolbar();

  //////////////////////////////////////////////////
#ifdef __WXMSW__
  int *gl_attrib = NULL;
#else
  int gl_attrib[20] =
  { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
  WX_GL_DOUBLEBUFFER,
#  if defined(__WXMAC__) || defined(__WXCOCOA__)
  GL_NONE };
#  else
  None
};
#  endif
#endif


  //////////////////////////////////////////////////
  mp_video = new CX_VideoDisplay(this, wxID_ANY, gl_attrib);
  //mp_video->mp_mainframe = mp_mainframe;
  //////////////////////////////////////////////////

  mp_grid = new CX_ImageGrid(this);
  mp_video->mp_grid = mp_grid;

  //////////////////////////////////////////////////
  mp_results_panel = new CX_ResultsPanel(this);
  mp_results_panel->mp_vd_parent = this;
  mp_results_panel->Show(false);
  //////////////////////////////////////////////////

  mp_toolbar->Show(true);
  mp_video->Show(true);

  SetMenuTicks();


}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
CX_VideoDlg::~CX_VideoDlg()
{
  // don't need to explicitly delete the video objects as they are wxWidget children
  // so wxWidgets will delete them
}
/////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
void CX_VideoDlg::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;

  cx_int top = 0;

  if (mp_toolbar)
  {
#ifdef WIN32
    top = mp_toolbar->GetSize().y;
#else
    top = 60;
#endif
    mp_toolbar->SetSize(0, 0, cx, top);
  }
  
  cx_int h = cy - top;
  cx_int w = cx;

  if (h <= 0) return;

  if (m_show_results_panel)
  {
    mp_results_panel->SetSize(0, top, w, h);
  }
  

  cx_int vh = cx_round((cx_real)h * 0.7f);
  if (mp_video && vh>0) mp_video->SetSize(0, top, w, vh);

  cx_int gh = h - vh;
  if (mp_grid && gh>0) mp_grid->SetSize(0, top + vh, w, gh);


}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnInputSettings(wxCommandEvent& event)
{
  if (!mp_video) return;

  // media file
  if (0 == mp_video->m_stream_type)
  {
    string fname;
    string filter("Files (*.wmv;*.avi;*.mpg;*.mpeg;*.mov;*.flv;*.mp4;*.mkv)|*.wmv;*.avi;*.mpg;*.mpeg;*.mov;*.flv;*.mp4;*.mkv;*.WMV;*.AVI;*.MPG;*.MPEG;*.MOV;*.FLV;*.MP4;*.MKV||");
    if (!GetOpenFilenameEx(fname, NULL, filter.c_str())) return;

    wxBusyCursor wait;
    //mp_video->Stop();

    mp_video->m_media_fname = fname;
    m_title = string("File: ") + GetFileName(fname) + "." + GetExtension(fname);
  }

  // USB camera
  else if (1 == mp_video->m_stream_type)
  {
    long value = mp_video->m_usb_pin;
    long res = wxGetNumberFromUser(wxT(""), wxT("USB Pin:"), wxT("USB Pin"), value, 0, 100, this);

    if (res != -1)
    {
      mp_video->m_usb_pin = (cx_uint)res;
      m_title = "USB Pin " + mp_video->m_usb_pin;
    }
  }

  // IP camera
  else if (2 == mp_video->m_stream_type)
  {
    wxTextEntryDialog dialog(this,
      wxT("Current IP Camera url:"),
      wxT("Please enter a valid IP camera url"),
      mp_video->m_ip_url.c_str(),
      wxOK | wxCANCEL);

    if (dialog.ShowModal() == wxID_OK)
    {
      mp_video->m_ip_url = string(dialog.GetValue().c_str());
      m_title = string("IP url ") + mp_video->m_ip_url;
    }
  }


  else
  {
    wxMessageBox("No input type selected");
  }
  mp_mainframe->ConstructAndSetTitle();

}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnMediaFile(wxCommandEvent& event)
{
  if (mp_video)
  {
    mp_video->Stop();
    mp_video->m_stream_type = 0;
    wxToolBarToolBase* p_tool = mp_toolbar->FindById(ID_VIDEO_SETTINGS);
    if (p_tool)
    {
      p_tool->SetNormalBitmap(cx_load32Bitmap());
      p_tool->SetLabel("Load");
      mp_toolbar->Realize();
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnUSBcamera(wxCommandEvent& event)
{
  if (mp_video)
  {
    mp_video->Stop();
    mp_video->m_stream_type = 1;
    wxToolBarToolBase* p_tool = mp_toolbar->FindById(ID_VIDEO_SETTINGS);
    if (p_tool)
    {
      p_tool->SetNormalBitmap(cx_pin32Bitmap());
      p_tool->SetLabel("Pin");
      mp_toolbar->Realize();
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnIPcamera(wxCommandEvent& event)
{
  if (mp_video)
  {
    mp_video->Stop();
    mp_video->m_stream_type = 2;
    wxToolBarToolBase* p_tool = mp_toolbar->FindById(ID_VIDEO_SETTINGS);
    if (p_tool)
    {
      p_tool->SetNormalBitmap(cx_cable32Bitmap());
      p_tool->SetLabel("Connect");
      mp_toolbar->Realize();
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::Play(bool display_errors)
{
  if (!mp_video) return;

  // media file
  if (0 == mp_video->m_stream_type)
  {
    if (!mp_video->PlayMediaFile(display_errors))
    {
      if (display_errors)
      {
        string msg = string("Failed to open:\n") + mp_video->m_media_fname;
        wxMessageBox(msg.c_str());
      }
      return;
    }
  }

  // USB camera
  else if (1 == mp_video->m_stream_type)
  {
    if (!mp_video->PlayUSBCamera(display_errors))
    {
      if (display_errors)
      {
        string msg = string("Failed to play USB camera with pin: ") + to_string(mp_video->m_usb_pin);
        wxMessageBox(msg.c_str());
      }
      return;
    }
  }

  // IP camera
  else if (2 == mp_video->m_stream_type)
  {
    wxCursor(wxCURSOR_WAIT);
    if (!mp_video->PlayIPCamera(display_errors))
    {
      if (display_errors)
      {
        string msg = string("Failed to play IP camera with url:\n") + mp_video->m_ip_url;
        wxMessageBox(msg.c_str());
      }
      return;
    }
  }


  else
  {
    if (display_errors)
    {
      wxMessageBox("No input type selected");
    }
  }

  mp_mainframe->ConstructAndSetTitle();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnPlay(wxCommandEvent& event)
{
  Play(true);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnPause(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->Pause();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnStop(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->Stop();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayVideo(wxCommandEvent& event)
{
  if (!mp_video) return;
  //mp_video->m_display = !mp_video->m_display;
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayPeopleOnly(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_people_thumbs = !mp_video->m_display_people_thumbs;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayOnlyVerified(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_only_verified = !mp_video->m_display_only_verified;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayCurrentHeadImages(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_current_head_image = !mp_video->m_display_current_head_image;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayPCOverlayImages(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_pc_overlays = !mp_video->m_display_pc_overlays;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayPCHeadImages(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_pc_head_image = !mp_video->m_display_pc_head_image;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayFitMeasure(wxCommandEvent& event)
{
  if (!mp_video) return;
  mp_video->m_display_fit_measure = !mp_video->m_display_fit_measure;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnSetFDP(wxCommandEvent& event)
{
  if (!mp_video) return;
  FDPdlg dlg(this, wxID_ANY, "Face Detection Parameters", mp_video->m_detection_params);
  if (dlg.ShowModal() == wxID_OK)
  {
    mp_video->m_detection_params = dlg.m_fdp;
    mp_video->SetDetectionParams();
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnSetFrameInterval(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  long value = (long)CX_GetFrameInterval(mp_video->mp_aureus_video, msg);
  if (value < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // get new value from user
  long res = wxGetNumberFromUser(wxT(""), wxT("Frame Interval:"), wxT("Frame Interval"), value, 1, 10, this);

  // test cancel was not pressed
  if (res != -1)
  {
    // set value
    if (0 == CX_SetFrameInterval(mp_video->mp_aureus_video, res, msg))
    {
      wxMessageBox(msg);
      return;
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnSetReduceFrameStep(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  int val = CX_GetFrameReductionStep(mp_video->mp_aureus_video, msg);
  if (val == -10)
  {
    wxMessageBox(msg);
    return;
  }
  // get new value from user
  long value = val;
  long res = wxGetNumberFromUser(wxT(""), wxT("Frame Step:"), wxT("Frame Step"), value, 1, 10, this);

  // test cancel was not pressed
  if (res != -1)
  {
    // set value
    val = res;
    if (0 == CX_SetFrameReductionStep(mp_video->mp_aureus_video, val, msg))
    {
      wxMessageBox(msg);
      return;
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnFRsetRankDisplay(wxCommandEvent& event)
{
  if (!mp_video) return;
  long value = mp_video->m_display_rank_n;
  long res = wxGetNumberFromUser(wxT(""), wxT("Rank Display num:"), wxT("Rank Display num"), value, 1, 100, this);

  if (res != -1)
  {
    wxCursor(wxCURSOR_WAIT);
    mp_video->m_display_rank_n = (cx_uint)res;
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnFRDisplayAllPeople(wxCommandEvent& event)
{
  if (!mp_video) return;

  if (!mp_video) return;
  mp_video->mp_grid->m_display_all_people = !mp_video->mp_grid->m_display_all_people;

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnFRsetVerificationThreshold(wxCommandEvent& event)
{
  if (!mp_video) return;

  char msg[1024];
  cx_real val = CX_GetVerificationThreshold(mp_video->mp_aureus_video, msg);
  if (val < 0.0f)
  {
    wxMessageBox(msg);
    return;
  }

  wxTextEntryDialog dialog(this, wxT("Verification Threshold:"), wxT("Verification Threshold:"), to_string(val), wxOK | wxCANCEL);
  if (dialog.ShowModal() == wxID_OK)
  {
    val = atof(dialog.GetValue().c_str());
    wxCursor(wxCURSOR_WAIT);

    if (0 == CX_SetVerificationThreshold(mp_video->mp_aureus_video, val, msg))
    {
      wxMessageBox(msg);
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::SetMenuTicks()
{
  if (!mp_mainframe || !mp_mainframe->mp_aureus) return;
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  
  // Only use this when the menu bar has been associated with a frame; otherwise, use the wxMenu equivalent call
  //mp_settings_menu->Check(ID_VIDEO_DISPLAY, mp_video->m_display);
 
  mp_settings_menu->Check(ID_HEAD_DISPLAY_PERSON_ONLY, mp_video->m_display_people_thumbs);
  mp_settings_menu->Check(ID_HEAD_DISPLAY_ONLY_VERIFIED, mp_video->m_display_only_verified);
  mp_settings_menu->Check(ID_HEAD_DISPLAY_CURRENT_IMAGE, mp_video->m_display_current_head_image);
  mp_settings_menu->Check(ID_HEAD_DISPLAY_PC_IMAGE, mp_video->m_display_pc_head_image);
  mp_settings_menu->Check(ID_HEAD_DISPLAY_PC_IMAGE_OVERLAY, mp_video->m_display_pc_overlays);
  mp_settings_menu->Check(ID_HEAD_DISPLAY_FIT_MEASURE, mp_video->m_display_fit_measure);
  mp_settings_menu->Check(ID_FR_DISPLAY_ALL_PEOPLE, mp_video->mp_grid->m_display_all_people);


  
  
  // get CX_VIDEO settings
  int val;
  val = CX_GetGenerateTemplatesFlag(mp_video->mp_aureus_video, msg);
  if (val >= 0)
  {
    bool setting = false;
    if (val != 0) setting = true;
    else setting = false;
    mp_settings_menu->Check(ID_FR_GENERATE_TEMPLATES, setting);
  }

  val = CX_GetPerformGalleryFRFlag(mp_video->mp_aureus_video, msg);
  if (val >= 0)
  {
    bool setting = false;
    if (val != 0) setting = true;
    else setting = false;
    mp_settings_menu->Check(ID_FR_PERFORM_FR, setting);
  }
 
  val = CX_GetForceEveryFrame(mp_video->mp_aureus_video, msg);
  if (val >= 0)
  {
    bool setting = false;
    if (val != 0) setting = true;
    else setting = false;
    mp_settings_menu->Check(ID_IMAGE_FORCE_EVERY_FRAME, setting);
  }

  val = CX_GetGenerateMeshOverlayFlag(mp_video->mp_aureus_video, msg);
  if (val >= 0)
  {
    bool setting = false;
    if (val != 0) setting = true;
    else setting = false;
    mp_settings_menu->Check(ID_HEAD_GENERATE_MESH_OVERLAY, setting);
  }


  val = CX_GetGeneratePoseCorrectionFlag(mp_video->mp_aureus_video, msg);
  if (val >= 0)
  {
    bool setting = false;
    if (val != 0) setting = true;
    else setting = false;
    mp_settings_menu->Check(ID_HEAD_GENERATE_PCIMAGE, setting);
  }
 

  mp_settings_menu->Check(ID_IMAGE_LOOP_MEDIA_FILES, mp_video->m_loop_media_files);
  
  
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnForceEveryFrame(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  int val = CX_GetForceEveryFrame(mp_video->mp_aureus_video, msg);
  if (val < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // toggle
  if (val == 0) val = 1;
  else val = 0;

  // set
  if (0 == CX_SetForceEveryFrame(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnDisplayMeshOverlay(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  int val = CX_GetGenerateMeshOverlayFlag(mp_video->mp_aureus_video, msg);
  if (val < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // toggle
  if (val == 0) val = 1;
  else val = 0;

  // set
  if (0 == CX_SetGenerateMeshOverlayFlag(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnGeneratePCImage(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  int val = CX_GetGeneratePoseCorrectionFlag(mp_video->mp_aureus_video, msg);
  if (val < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // toggle
  if (val == 0) val = 1;
  else val = 0;

  // set
  if (0 == CX_SetGeneratePoseCorrectionFlag(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  // if we are generating a PC Image we also need to generate a 3D mesh
  if (0 == CX_SetGenerate3DMeshFlag(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnLoopMediaFiles(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  // toggle
  if (mp_video->m_is_playing)
  {
    // pause if playing
    mp_video->Pause();
    mp_video->m_loop_media_files = !mp_video->m_loop_media_files;
    Play(true); // re-start
  }
  else
  {
    mp_video->m_loop_media_files = !mp_video->m_loop_media_files;
  }

  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnFRgenerateTemplatesToggle(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;

  char msg[1024];
  // get current value
  int val = CX_GetGenerateTemplatesFlag(mp_video->mp_aureus_video, msg);
  if (val < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // toggle
  if (val == 0) val = 1;
  else val = 0;

  // set
  if (0 == CX_SetGenerateTemplatesFlag(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnFRperformRankingToggle(wxCommandEvent& event)
{
  if (!mp_video || !mp_video->mp_aureus_video) return;
  
  char msg[1024];
  // get current value
  int val = CX_GetPerformGalleryFRFlag(mp_video->mp_aureus_video, msg);
  if (val < 0)
  {
    wxMessageBox(msg);
    return;
  }
  // toggle
  if (val == 0) val = 1;
  else val = 0;

  // set
  if (0 == CX_SetPerformGalleryFRFlag(mp_video->mp_aureus_video, val, msg))
  {
    wxMessageBox(msg);
    return;
  }
  SetMenuTicks();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnGallerySelect(wxCommandEvent& event)
{
  if (!mp_mainframe || !mp_mainframe->mp_aureus) return;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnEnrollCode(wxCommandEvent& event)
{
  if (!mp_video) return;

  // check that we're not already enrolling
  if (mp_video->m_is_enrolling) return;

  // ensure that the video is playing
  if (mp_video->m_is_paused || !mp_video->m_is_playing)
  {
    OnPlay(event);
    // check that worked 
    if (mp_video->m_is_paused || !mp_video->m_is_playing) return; // failed
  }

  // set enrolling flag
  mp_video->m_is_enrolling = true;

}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::ShowResults(bool show)
{
  m_show_results_panel = show;
  if (m_show_results_panel)
  {
    mp_results_panel->Show(true);
    mp_video->Show(false);
    mp_grid->Show(false);
  }
  else
  {
    mp_results_panel->Show(false);
    mp_video->Show(true);
    mp_grid->Show(true);
  }
  PositionWindows();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDlg::OnResultsSettings(wxCommandEvent& event)
{
  if (!mp_video) return;
  if (!mp_video->mp_aureus_video) return;

  mp_results_panel->SetValues(mp_video->mp_aureus_video);

  ShowResults(true);
}
///////////////////////////////////////////////////////////////////
