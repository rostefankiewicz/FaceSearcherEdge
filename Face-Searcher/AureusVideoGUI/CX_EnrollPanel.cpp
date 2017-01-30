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
#include "CX_EnrollPanel.h"
#include "CX_GalleryPanel.h"
#include "FDPdlg.h"
#include "AureusVideoGUI.h"
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
static const long ID_ENROLL_TOOLBAR = wxNewId();
static const long ID_ENROLL_SETTINGS = wxNewId();
static const long ID_ENROLL_USB_CAMERA = wxNewId();
static const long ID_ENROLL_IP_CAMERA = wxNewId();
static const long ID_ENROLL_PLAY = wxNewId();
static const long ID_ENROLL_PAUSE = wxNewId();
static const long ID_ENROLL_STOP = wxNewId();
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// CX_VideoDlg
///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_EnrollPanel, wxPanel)
EVT_SIZE(CX_EnrollPanel::OnSize)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::PositionWindows()
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

  //cx_int vh = cx_round((cx_real)h * 0.8f);
  cx_int vh = cx_round((cx_real)h * 0.7f);
  if (mp_video && h>0) mp_video->SetSize(0, top, w, h);
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::ConstructToolbar()
{
  mp_toolbar = new wxToolBar(this, ID_ENROLL_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_ENROLL_TOOLBAR"));
  mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(16, 16)));

  mp_toolbar->AddTool(ID_ENROLL_SETTINGS, "Pin", cx_pin32Bitmap());
  mp_toolbar->AddSeparator();
  mp_toolbar->AddRadioTool(ID_ENROLL_USB_CAMERA, _("USB"), cx_webcam32Bitmap(), wxNullBitmap, "USB Camera");
  mp_toolbar->AddRadioTool(ID_ENROLL_IP_CAMERA, _("IP"), cx_ipcam32Bitmap(), wxNullBitmap, "IP Camera");

  mp_toolbar->AddSeparator();
  mp_toolbar->AddTool(ID_ENROLL_PLAY, "Play", cx_play32Bitmap());
  mp_toolbar->AddTool(ID_ENROLL_PAUSE, "Pause", cx_pause32Bitmap());
  mp_toolbar->AddTool(ID_ENROLL_STOP, "Stop", cx_stop32Bitmap());

  mp_toolbar->Realize();

  Connect(ID_ENROLL_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnInputSettings);
  Connect(ID_ENROLL_USB_CAMERA, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnUSBcamera);
  Connect(ID_ENROLL_IP_CAMERA, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnIPcamera);
  Connect(ID_ENROLL_PLAY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnPlay);
  Connect(ID_ENROLL_PAUSE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnPause);
  Connect(ID_ENROLL_STOP, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_EnrollPanel::OnStop);

}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_EnrollPanel::CX_EnrollPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxPanel(parent, id, pos, size, style, name)
{
  mp_mainframe = mainframe;
  mp_toolbar = NULL;
  mp_video = NULL;
  mp_enroll_video = NULL;
  m_is_first_frame = true;
  m_is_paused = false;

  m_stream_type = 1;
  m_usb_pin = 0;
  m_ip_url = "http://131.111.125.248/axis-cgi/mjpg/video.cgi?fps=25";

  ConstructToolbar();

  mp_video = new CX_ImageWindow(this);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_EnrollPanel::~CX_EnrollPanel()
{
  // no need to stop the video as freein Aureus will
  // automatically stop all streams and free RAM
  //Stop();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
bool CX_EnrollPanel::Initialize()
{
  char msg[1024];
  mp_enroll_video = CX_CreateEnrollVideo(mp_mainframe->mp_aureus, msg);
  if (!mp_enroll_video)
  {
    wxMessageBox(msg);
    return false;
  }


  if (0 == CX_SetEnrollFrameCallBack(mp_enroll_video, FrameCallBack, this, msg))
  {
    wxMessageBox(msg);
    return false;
  }

  if (0 == CX_SetEnrollStreamTerminatedCallBack(mp_enroll_video, UnexpectedTerminationCallBack, this, msg))
  {
    wxMessageBox(msg);
    return false;
  }

  //PositionWindows();
  if (mp_video) mp_video->Render();

  return true;
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// callback functions: just pass them on to a non static member func
// so that we can get at the data more elegantly
///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::FrameCallBack(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, cx_int status, const char* msg, void* p_object)
{
  if (p_object) ((CX_EnrollPanel*)p_object)->FrameCallBack(p_pixels, rows, cols, frame_num, status, msg);
}
///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object)
{
  wxMessageBox("Oh heck the stream has terminated:\n");
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::FrameCallBack(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, cx_int status, const char* msg)
{
  if (!mp_video) return;

  mp_video->SetImage(p_pixels, rows, cols);
  if (m_is_first_frame)
  {
    mp_video->Seek();
    mp_video->Render();
    m_is_first_frame = false;
  }

  // error?
  if (status < 0)
  {
    wxMessageBox(msg);
  }

  // finished?
  else if (status == 1)
  {
    // update gallery display
    mp_mainframe->mp_gallery_panel->Initialize(mp_mainframe);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// stream opening functions
///////////////////////////////////////////////////////////////////
bool CX_EnrollPanel::PlayUSBCamera()
{
  if (!mp_enroll_video) return false;
  char msg[1024];

  if (m_is_paused)
  {
    (void)CX_UnPauseEnrollmentStream(mp_enroll_video, msg);
    m_is_paused = false;
    return true;
  }

  Stop();
  m_is_first_frame = true;

  string con_info; con_info += m_usb_pin;
  if (0 == CX_StartEnrollmentStream(mp_enroll_video, 1, con_info.c_str(), msg))
  {
    string emsg = "Failed to open USB at pin " + m_usb_pin;
    emsg += string("\nSDK error: ") + msg;
    wxMessageBox(emsg.c_str());
    return false;
  }
  m_stream_type = 1;
  return true;
}
///////////////////////////////////////////////////////////////////
bool CX_EnrollPanel::PlayIPCamera()
{
  if (!mp_enroll_video) return false;
  char msg[1024];

  if (m_is_paused)
  {
    (void)CX_UnPauseEnrollmentStream(mp_enroll_video, msg);
    m_is_paused = false;
    return true;
  }

  Stop();
  m_is_first_frame = true;

  // DEV test
  //m_ip_url = "http://131.111.125.248/axis-cgi/mjpg/video.cgi?fps=25";

  if (0 == CX_StartEnrollmentStream(mp_enroll_video, 2, m_ip_url.c_str(), msg))
  {
    string emsg = "Failed to open IP camera at url:\n" + m_ip_url;
    emsg += string("\nSDK error: ") + msg;
    wxMessageBox(emsg.c_str());
    return false;
  }
  m_stream_type = 2;
  return true;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::Pause()
{
  if (!mp_enroll_video) return;
  if (m_is_paused) return;

  char msg[1024];
  if (0 == CX_PauseEnrollmentStream(mp_enroll_video, msg))
  {
    wxMessageBox(msg);
  }
  m_is_paused = true;
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::Stop()
{
  if (!mp_enroll_video) return;
  char msg[1024];
  if (0 == CX_StopEnrollmentStream(mp_enroll_video, msg))
  {
    wxMessageBox(msg);
  }
  m_is_paused = false;
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnPlay(wxCommandEvent& event)
{
  // USB camera
  if (1 == m_stream_type)
  {
    if (!PlayUSBCamera())
    {
      string msg = "Failed to play USB camera with pin: " + m_usb_pin;
      wxMessageBox(msg.c_str());
      return;
    }
    //else mp_mainframe->SetTitle(m_title.c_str());
  }

  // IP camera
  else if (2 == m_stream_type)
  {
    wxCursor(wxCURSOR_WAIT);
    if (!PlayIPCamera())
    {
      string msg = string("Failed to play IP camera with url:\n") + m_ip_url;
      wxMessageBox(msg.c_str());
      return;
    }
    //else mp_mainframe->SetTitle(m_title.c_str());
  }


  else
  {
    wxMessageBox("No input type selected");
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnPause(wxCommandEvent& event)
{
  Pause();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnStop(wxCommandEvent& event)
{
  Stop();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnIPcamera(wxCommandEvent& event)
{
  Stop();
  m_stream_type = 2;
  wxToolBarToolBase* p_tool = mp_toolbar->FindById(ID_ENROLL_SETTINGS);
  if (p_tool)
  {
    p_tool->SetNormalBitmap(cx_cable32Bitmap());
    p_tool->SetLabel("Connect");
    mp_toolbar->Realize();
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnUSBcamera(wxCommandEvent& event)
{
  Stop();
  m_stream_type = 1;
  wxToolBarToolBase* p_tool = mp_toolbar->FindById(ID_ENROLL_SETTINGS);
  if (p_tool)
  {
    p_tool->SetNormalBitmap(cx_pin32Bitmap());
    p_tool->SetLabel("Pin");
    mp_toolbar->Realize();
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_EnrollPanel::OnInputSettings(wxCommandEvent& event)
{
  // USB camera
  if (1 == m_stream_type)
  {
    long value = m_usb_pin;
    long res = wxGetNumberFromUser(wxT(""), wxT("USB Pin:"), wxT("USB Pin"), value, 0, 100, this);

    if (res != -1)
    {
      m_usb_pin = (cx_uint)res;
      //m_title = string("VideoFR USB Pin ") + m_usb_pin + " " + mp_mainframe->m_gal_name;
    }
  }

  // IP camera
  else if (2 == m_stream_type)
  {
    wxTextEntryDialog dialog(this,
      wxT("Current IP Camera url:"),
      wxT("Please enter a valid IP camera url"),
      m_ip_url.c_str(),
      wxOK | wxCANCEL);

    if (dialog.ShowModal() == wxID_OK)
    {
      m_ip_url = string(dialog.GetValue().c_str());
      //m_title = string("VideoFR IP Camera ") + mp_video->m_ip_url + " " + mp_mainframe->m_gal_name;
    }
  }


  else
  {
    wxMessageBox("No input type selected");
  }
}
///////////////////////////////////////////////////////////////////


