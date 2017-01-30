#include <wx/notebook.h>
#include "CX_VideoPanel.h"
#include "AureusVideoGUI.h"
#include "CX_ResultsPanel.h"
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
static const long ID_VIDEO_PANEL_TOOLBAR = wxNewId();
static const long ID_VIDEO_LICENSE_TEXT = wxNewId();
static const long ID_VIDEO_PLAY_ALL = wxNewId();
static const long ID_VIDEO_PAUSE_ALL = wxNewId();
static const long ID_VIDEO_STOP_ALL = wxNewId();
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// MyFrame
///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_VideoPanel, wxPanel)
EVT_SIZE(CX_VideoPanel::OnSize)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoPanel::ConstructToolbar()
{
  //mp_toolbar = new wxToolBar(this, ID_VIDEO_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_VIDEO_TOOLBAR"));
  mp_toolbar = new wxToolBar(this, ID_VIDEO_PANEL_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_VIDEO_TOOLBAR"));
  //mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(20, 20)));
  mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(16, 16)));

  mp_toolbar->AddTool(ID_VIDEO_PLAY_ALL, "Play All", cx_play32Bitmap());
  mp_toolbar->AddTool(ID_VIDEO_PAUSE_ALL, "Pause All", cx_pause32Bitmap());
  mp_toolbar->AddTool(ID_VIDEO_STOP_ALL, "Stop All", cx_stop32Bitmap());

  // license info
  wxStaticText* lic = new wxStaticText(mp_toolbar, ID_VIDEO_LICENSE_TEXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1));
  lic->SetLabelText("License Info");
  mp_toolbar->AddControl(lic, wxEmptyString);
  
  mp_toolbar->Realize();


  Connect(ID_VIDEO_PLAY_ALL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoPanel::OnPlayAll);
  Connect(ID_VIDEO_PAUSE_ALL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoPanel::OnPauseAll);
  Connect(ID_VIDEO_STOP_ALL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_VideoPanel::OnStopAll);

}
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
CX_VideoPanel::CX_VideoPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxPanel(parent, id, pos, size, style, name)
{
  mp_mainframe = mainframe;

  ConstructToolbar();


  cx_int num_streams = mp_mainframe->m_use_n_video_streams;
  if (num_streams == 0)
  {
    wxMessageBox("NoVideoStreams");
  }
  else
  {
    // create video stream dialogs, one per stream
    for (cx_int i = 0; i < num_streams; i++)
    {
      m_videos.push_back(new CX_VideoDlg(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_SIMPLE));
    }
  }

  //char msg[1024];
  //cx_int num_streams = 0;
  //cx_int image_enabled = 0;
  //cx_int fr_enabled = 0;
  //if (0 == CX_GetLicenseParameters(mp_mainframe->mp_aureus, &num_streams, &image_enabled, &fr_enabled, msg))
  //{
  //  wxMessageBox(msg);
  //}
  //else
  //{
  //  if (num_streams == 0)
  //  {
  //    wxMessageBox("NoVideoStreams");
  //  }
  //  else
  //  {
  //    // create video stream dialogs, one per stream
  //    for (cx_int i = 0; i < num_streams; i++)
  //    {
  //      m_videos.push_back(new CX_VideoDlg(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_SIMPLE));
  //    }
  //  }
  //}
}
////////////////////////////////////////////////////////////////////////////////
CX_VideoPanel::~CX_VideoPanel()
{
  // don't need to explicitly delete the video objects as they are wxWidget children
  // so wxWidgets will delete them
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_VideoPanel::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_VideoPanel::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;

  if (m_videos.size() == 0) return;

  cx_int t = 0;

  if (mp_toolbar)
  {
#ifdef WIN32
    t = mp_toolbar->GetSize().y;
#else
    t = 60;
#endif
    mp_toolbar->SetSize(0, 0, cx, t);
  }

  cy = cy - t;

  if (cy <= 0) return;


  // single video
  if (m_videos.size() == 1)
  {
    m_videos[0]->SetSize(0, t, cx, cy);
    return;
  }

  // 2 columns
  if (m_videos.size() <= 4)
  {
    // we split them into two halfs of the real estate
    cx_int w = cx / 2;
    cx_int h = cy / ((1 + m_videos.size()) / 2);

    bool is_odd = false;
    if (m_videos.size() % 2 != 0) is_odd = true;

    for (cx_uint i = 0; i < m_videos.size(); i++)
    {
      cx_int left = 0;
      if ((i % 2) != 0) left = w;
      cx_int top = t + ((i / 2) * h);

      // if it's the last one then set it to the remaining width
      if (i == m_videos.size() - 1)
      {
        m_videos[i]->SetSize(left, top, cx - left, h);
      }

      else m_videos[i]->SetSize(left, top, w, h);
    }
    return;
  }

  // 3 columns, 2 rows
  if (m_videos.size() <= 6)
  {
    cx_int w = cx / 3;
    cx_int h = cy / ((1 + m_videos.size()) / 3);

    for (cx_uint i = 0; i < m_videos.size(); i++)
    {
      cx_int left = (i % 3) * w;
      cx_int top = t;
      if (i >= 3) top = t + h;

      // if it's the last one then set it to the remaining width
      if (i == m_videos.size() - 1)
      {
        m_videos[i]->SetSize(left, top, cx - left, h);
      }

      else m_videos[i]->SetSize(left, top, w, h);
    }
    return;

  }

  // larger than 6 videos
  // 4 columns, n rows
  {
    cx_int w = cx / 4;
    cx_int d = cx_round(cx_ceil((cx_real)m_videos.size() / 4.0f));
    cx_int h = cy / d;

    for (cx_uint i = 0; i < m_videos.size(); i++)
    {
      cx_int left = (i % 4) * w;
      cx_int top = t + ((i / 4) * h);

      // if it's the last one then set it to the remaining width
      if (i == m_videos.size() - 1)
      {
        m_videos[i]->SetSize(left, top, cx - left, h);
      }

      else m_videos[i]->SetSize(left, top, w, h);
    }
    return;

  }

}
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoPanel::UpdateLicenseInfo()
{
  if (!mp_mainframe && mp_mainframe->mp_aureus) return;

  wxStaticText* c = (wxStaticText*)mp_toolbar->FindWindow(ID_VIDEO_LICENSE_TEXT);
  if (!c) return;

  char msg[1024];

  // Get version
  CX_GetVersion(mp_mainframe->mp_aureus, msg);
  string vers = string(msg) + ", ";

  cx_int num_streams, image_enabled, fr_enabled;
  if (0 == CX_GetLicenseParameters(mp_mainframe->mp_aureus, &num_streams, &image_enabled, &fr_enabled, msg))
  {
    c->SetLabelText(msg);
  }
  else
  {
    string txt = vers + " ";
    txt += to_string(num_streams) + " streams,";
    if (fr_enabled) txt += " FR Enabled\n";
    txt +=string(msg);

    c->SetLabelText(txt.c_str());
  }

}
///////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
bool CX_VideoPanel::Initialize(MyFrame* p_mainframe)
{
  mp_mainframe = p_mainframe;
  for (cx_uint i = 0; i < m_videos.size(); i++)
  {
    m_videos[i]->mp_mainframe = p_mainframe;
    m_videos[i]->mp_video->mp_mainframe = p_mainframe;
    m_videos[i]->mp_results_panel->mp_mainframe = p_mainframe;
    string emsg;
    if (!m_videos[i]->mp_video->InitializeAureusVideo(emsg))
    {
      wxMessageBox(emsg.c_str());
      return false;
    }
    else m_videos[i]->SetMenuTicks();

  }
  UpdateLicenseInfo();
  return true;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_VideoPanel::OnPlayAll(wxCommandEvent& event)
{
  for (cx_uint i = 0; i < m_videos.size(); i++)
  {
    m_videos[i]->Play(false);
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_VideoPanel::OnPauseAll(wxCommandEvent& event)
{
  for (cx_uint i = 0; i < m_videos.size(); i++)
  {
    m_videos[i]->OnPause(event);
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_VideoPanel::OnStopAll(wxCommandEvent& event)
{
  for (cx_uint i = 0; i < m_videos.size(); i++)
  {
    m_videos[i]->OnStop(event);
  }
}
////////////////////////////////////////////////////////////////////////////////
