// AureusVideoGUI.cpp : Defines the entry point for the console application.
//////////////////////////////////////////////////////////////////////////////////////
// Copyright: (C) 2016 CyberExtruder.com Inc.
//                     www.cyberextruder.com
//
// DISCLAIMER:
//
// THIS SOFTWARE IS PROVIDED “AS IS” AND “WITH ALL FAULTS” AND WITHOUT
// WARRANTY OF ANY KIND.
// CUSTOMER AGREES THAT THE USE OF THIS SOFTWARE IS AT CUSTOMER'S RISK
// CYBEREXTRUDER MAKES NO WARRANTY OF ANY KIND TO CUSTOMER OR ANY THIRD 
// PARTY, EXPRESS, IMPLIED OR STATUTORY, WITH RESPECT TO THE THIS 
// SOFTWARE, OPERATION OF THE SOFTWARE, OR OUTPUT OF OR RESULTS OBTAINED 
// FROM THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY 
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OPERABILITY OR 
// NON-INFRINGEMENT AND ALL SUCH WARRANTIES ARE HEREBY EXCLUDED BY 
// CYBEREXTRUDER AND WAIVED BY CUSTOMER.
//////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// include main wxWidgets
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/cmdline.h"
#include "wx/bmpcbox.h"
#include "wx/choice.h"
#include "wx/numdlg.h"
#include <wx/notebook.h>
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#include "AureusVideoGUI.h"
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#include "FDPdlg.h"
#include "CX_VideoPanel.h"
#include "CX_GalleryPanel.h"
#include "CX_EnrollPanel.h"
///////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// for Linux Fonts
#ifndef WIN32
#include <GL/glut.h>
static bool GLUT_is_initialized = false;
////////////////////////////////////////////////////////////////////////////////
void InitializeGLUT()
{
  if (!GLUT_is_initialized)
  {
    // initialize glut
    const char* myargv[1] = { "Myappname" };
    int myargc = 1;
    glutInit(&myargc, (char**)myargv);

    GLUT_is_initialized = true;
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif


/////////////////////////////////////////////////////////////////////////////
// uses the wxFileDialog to get a filename for either opening or saving
// filter example = "Image files (*.bmp;*.jpg)|*.bmp;*.jpg|Text file (*.txt)|*.txt|"
// if filter is NULL then it defaults to "All files (*.*)|*.*"
/////////////////////////////////////////////////////////////////////////////
bool GetOpenFilename(string& fname, const char* szFilter)
{
  wxFileDialog openFileDialog(NULL, _("Open file"), wxEmptyString, wxEmptyString, szFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() == wxID_CANCEL) return false; // the user changed idea...

  // proceed loading the file chosen by the user;
  fname = openFileDialog.GetPath().c_str();
  return true;
}
/////////////////////////////////////////////////////////////////////////////
bool GetSaveFilename(string& fname, const char* szFilter)
{
  wxFileDialog openFileDialog(NULL, _("Save file"), wxEmptyString, wxEmptyString, szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (openFileDialog.ShowModal() == wxID_CANCEL) return false; // the user changed idea...

  // proceed loading the file chosen by the user;
  fname = openFileDialog.GetPath().c_str();
  return true;
}
/////////////////////////////////////////////////////////////////////////////
bool GetOpenFilenameEx(string& fname, const char* szDefFileName, const char* szFilter)
{
  wxFileDialog openFileDialog(NULL,	// parent
    _("Open file"),							    // message
    wxEmptyString,							    // default dir
    wxString(szDefFileName),        // default file
    szFilter,										    // wild card
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return false; // the user changed idea...

  // proceed loading the file chosen by the user;
  fname = openFileDialog.GetPath().c_str();
  return true;
}
/////////////////////////////////////////////////////////////////////////////
bool GetSaveFilenameEx(string& fname, const char* szDefFileName, const char* szFilter) 
{
  wxFileDialog openFileDialog(NULL, // parent
    _("Save file"),                 // message
    wxEmptyString,                  // default dir
    wxString(szDefFileName),        // default file
    szFilter,                       // wild card
    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (openFileDialog.ShowModal() == wxID_CANCEL) return false; // the user changed idea...

  // proceed loading the file chosen by the user;
  fname = openFileDialog.GetPath().c_str();
  return true;
}
/////////////////////////////////////////////////////////////////////////////
bool GetOpenFilenames(vector<std::string>& fnames, const char* szFilter) 
{
  wxFileDialog openFileDialog(NULL, _("Open file"), wxEmptyString, wxEmptyString, szFilter, wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() == wxID_CANCEL) return false; // the user changed idea...

  wxArrayString filenames;
  openFileDialog.GetPaths(filenames);

  if (filenames.IsEmpty()) return false;

  for (cx_uint i = 0; i < filenames.size(); i++) 
  {
    fnames.push_back(string(filenames[i].c_str()));
  }

  return true;
}
/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// MyApp
///////////////////////////////////////////////////////////////////
IMPLEMENT_APP(MyApp)
///////////////////////////////////////////////////////////////////
bool MyApp::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  // Create the main frame window
  new MyFrame(0);

  return true;

}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  //parser.AddSwitch("", "sb", "Do not use double buffering");
  //parser.AddSwitch("", "db", "Use double buffering");
  //parser.AddSwitch("", "va", "Use vertex arrays");

  //parser.AddSwitch("", "va", "Use vertex arrays");
  //parser.AddParam();

  wxApp::OnInitCmdLine(parser);
}
///////////////////////////////////////////////////////////////////
bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  //if (parser.Found("sb")) g_doubleBuffer = GL_FALSE;
  //else if (parser.Found("db")) g_doubleBuffer = GL_TRUE;

  //if (parser.Found("va")) g_use_vertex_arrays = GL_TRUE;

  //cx_uint n_params = parser.GetParamCount();
  //string s = string("Num of params = ") + string(n_params) + "\n";
  //for (cx_uint i = 0; i < n_params; i++)
  //{
  //  s += parser.GetParam((size_t)i).c_str();
  //}
  //wxMessageBox(s.c_str());
  return wxApp::OnCmdLineParsed(parser);
}
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GUI IDs
////////////////////////////////////////////////////////////////////////////////
static const long ID_IMAGE_SET_FDP = wxNewId();
static const long ID_IMAGE_FRAME_INTERVAL = wxNewId();
static const long ID_IMAGE_REDUCX_FRAME_STEP = wxNewId();

static const long ID_FR_DETECT_HEADS = wxNewId();
static const long ID_FR_FIT_LANDMARKS = wxNewId();
static const long ID_FR_USE_IS_FACX_EVERY_FRAME = wxNewId();
static const long ID_FR_GENERATE_TEMPLATES = wxNewId();
static const long ID_FR_PERFORM_RANKING = wxNewId();
static const long ID_FR_USE_SPLIT_RANKING = wxNewId();
static const long ID_FR_SET_THREADS = wxNewId();
static const long ID_FR_SET_RANK_DISPLAY = wxNewId();

static const long ID_MAIN_NOTEBOOK = wxNewId();
static const long ID_VIDEO_PANEL = wxNewId();
static const long ID_GALLERY_PANEL = wxNewId();
static const long ID_ENROLL_PANEL = wxNewId();
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// MyFrame
///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_SIZE(MyFrame::OnSize)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNoteBookChanged)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void MyFrame::ConstructNoteBook()
{
  // set up notebook
  long wstyle = wxFULL_REPAINT_ON_RESIZE | wxTAB_TRAVERSAL;
  mp_notebook = new wxNotebook(this, ID_MAIN_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP, _T("ID_MAIN_NOTEBOOK"));
  mp_video_panel = new CX_VideoPanel(this, mp_notebook, ID_VIDEO_PANEL, wxDefaultPosition, wxDefaultSize, wstyle, _T("ID_VIDEO_PANEL"));
  mp_gallery_panel = new CX_GalleryPanel(this, mp_notebook, ID_GALLERY_PANEL, wxDefaultPosition, wxDefaultSize, wstyle, _T("ID_GALLERY_PANEL"));
  mp_enroll_panel = new CX_EnrollPanel(this, mp_notebook, ID_ENROLL_PANEL, wxDefaultPosition, wxDefaultSize, wstyle, _T("ID_ENROLL_PANEL"));
  mp_notebook->AddPage(mp_video_panel, _("Video"), false);
  mp_notebook->AddPage(mp_gallery_panel, _("Gallery"), false);
  mp_notebook->AddPage(mp_enroll_panel, _("Enroll"), false);

}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// just ensures that the tabs are painted properly
void MyFrame::OnNoteBookChanged(wxBookCtrlBaseEvent& event)
{
  int sel = mp_notebook->GetSelection();

  if (0 == sel)
  {
    if (mp_video_panel) mp_video_panel->PositionWindows();
  }
  else if (1 == sel)
  {
#ifdef WIN32
    if (mp_gallery_panel) mp_gallery_panel->PositionWindows();
#else
    // a hack, the wxWidgets on GTK doesn't display properly the first time
    // I'm sure there is a correct way but this is free (use at your own risk) example code
    if (mp_gallery_panel)
    {
      if (mp_gallery_panel->m_is_first_selection)
      {
        mp_gallery_panel->m_is_first_selection = false;
        cx_int w, h;
        // we have to force the size to change
        mp_gallery_panel->GetSize(&w, &h);
        mp_gallery_panel->SetSize(w + 5, h + 5);
        mp_gallery_panel->PositionWindows();
        // they we change it back
        mp_gallery_panel->SetSize(w, h);
        mp_gallery_panel->PositionWindows();
      }
      else mp_gallery_panel->PositionWindows();
    }
#endif
  }
  else if (2 == sel)
  {
    if (mp_enroll_panel) mp_enroll_panel->PositionWindows();
  }
}
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
bool MyFrame::LoadAureus(string& emsg)
{
  char msg[1024];

  mp_aureus = CX_CreateAureus(msg);
  if (!mp_aureus) { emsg = string("Failed to Created Aureus:\n ") + msg; return false; }

  string fname;
  (void)GetExecutablePath(m_data_dir);

  // DEV code, allows me to run directly from source x64\Release folder
  fname = m_data_dir + "/Landmarks.data";
  if (!FileExists(fname.c_str()))
  {
#ifdef WIN32
    #ifdef IS_32_BIT
        m_data_dir = "C:\\CyberExtruder\\Aureus32";
    #else
        m_data_dir = "C:\\CyberExtruder\\Aureus";
    #endif
#else
    m_data_dir = "/home/tim/CyberExtruder/Aureus";
#endif
  }

  int load_gallery = 1;
  if (0 == CX_Initialize(mp_aureus, load_gallery, m_data_dir.c_str(), msg)) { emsg += msg; return false; };

  return true;
}
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
MyFrame::MyFrame(wxFrame *parent, wxWindowID id)
{
  string emsg;
  mp_aureus = NULL;
  mp_notebook = NULL;
  mp_video_panel = NULL;
  mp_gallery_panel = NULL;
  mp_enroll_panel = NULL;

  cx_int n_video_streams = 1;
  char msg[1024];


  //////////////////////////////////////////////////
  // Load Aureus
  //////////////////////////////////////////////////
  wxCursor(wxCURSOR_WAIT);
  if (!LoadAureus(emsg))
  {
    wxMessageBox(emsg.c_str());
  }
  else
  {
    cx_int image_enabled, fr_enabled;
    if (0 == CX_GetLicenseParameters(mp_aureus, &n_video_streams, &image_enabled, &fr_enabled, msg))
    {
      wxMessageBox(msg);
    }
  }
  //////////////////////////////////////////////////

  // use this if you want to use all licensed video streams in the same GUI
  m_use_n_video_streams = n_video_streams;
  // use this if you want separate ones
  m_use_n_video_streams = 1;

  //wxFont thisFront(12,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Times New Roman"),wxFONTENCODING_DEFAULT);
  wxFont thisFront(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T("Times New Roman"), wxFONTENCODING_DEFAULT);

  //////////////////////////////////////////////////
  // calc desired position and size
  int x, y, w, h;
  wxClientDisplayRect(&x, &y, &w, &h);
  int width = cx_round((cx_real)0.375*(cx_real)w);
  int height = cx_round((cx_real)0.585*(cx_real)h);

  if (m_use_n_video_streams>1) width = cx_round((cx_real)0.975*(cx_real)w);
  if (m_use_n_video_streams>2) height = cx_round((cx_real)0.975*(cx_real)h);

  int left = (w - width) / 2;
  int top = (h - height) / 2;
  //////////////////////////////////////////////////


  //////////////////////////////////////////////////
  // Create the main frame
  //long style = wxDEFAULT_FRAME_STYLE;
  long style = wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN;
  Create(parent, id, _("VideoFR"), wxPoint(left, top), wxSize(width, height), style, _T("VideoFR"));
  //////////////////////////////////////////////////

  //////////////////////////////////////////////////
  // set main icon
  wxIcon FrameIcon;
  FrameIcon.CopyFromBitmap(cx_Texture50Bitmap());
  SetIcon(FrameIcon);
  //////////////////////////////////////////////////



  //////////////////////////////////////////////////
  // set up notebook and show the frame
  ConstructNoteBook();
  Show(true);
  Raise();
  //////////////////////////////////////////////////


  //////////////////////////////////////////////////
  // Load main Data
  //////////////////////////////////////////////////
  wxCursor cursor(wxCURSOR_WAIT);
  // load data required for video (also initializes thread pool and any data that uses)
  (void)mp_video_panel->Initialize(this);
  (void)mp_gallery_panel->Initialize(this);
  (void)mp_enroll_panel->Initialize();

  m_update_timer.mp_mainframe = this;
  m_update_timer.Start(10000);

}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
MyFrame::~MyFrame()
{
  if (mp_aureus)
  {
    char msg[1024];
    if (0 == CX_FreeAureus(mp_aureus, msg))
    {
      printf("%s\n", msg);
    }
    mp_aureus = NULL;
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Intercept menu commands
void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  // true is to force the frame to close
  Close(true);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void MyFrame::PositionWindows()
{
  cx_int cx, cy;
  GetClientSize(&cx, &cy);

  if (mp_notebook && cx>0 && cy>0) mp_notebook->SetSize(0, 0, cx, cy);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void MyFrame::OnSize(wxSizeEvent& event)
{
  if (!IsShown())
    //if (!IsShownOnScreen())
  {
    //event.Skip();
    return;
  }
  PositionWindows();
}
///////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////
void MyFrame::ConstructAndSetTitle()
{
  if (!mp_video_panel) return;

  string title("VideoFR ");
  for (cx_uint i = 0; i < mp_video_panel->m_videos.size(); i++)
  {
    // Media File
    if (0 == mp_video_panel->m_videos[i]->mp_video->m_stream_type)
    {
      title += string("File: ") + GetFileName(mp_video_panel->m_videos[i]->mp_video->m_media_fname);
    }
    // USB
    else if (1 == mp_video_panel->m_videos[i]->mp_video->m_stream_type)
    {
      title += string("USBpin: ") + to_string(mp_video_panel->m_videos[i]->mp_video->m_usb_pin);
    }
    // IP
    else if (2 == mp_video_panel->m_videos[i]->mp_video->m_stream_type)
    {
      title += string("IPurl: ") + mp_video_panel->m_videos[i]->mp_video->m_ip_url;
    }
    title += " ";
  }
  SetTitle(title.c_str());
}
///////////////////////////////////////////////////////////////////

