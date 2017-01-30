#pragma once
#ifndef _AUREUS_VIDEO_GUI_H_
#define _AUREUS_VIDEO_GUI_H_


///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "Aureus.h"
#include "AureusVideo.h"
#include "AureusImage.h"
#include "AureusHeads.h"
#include "AureusGallery.h"
#include "AureusEnrollVideo.h"
#include "cxutils.h"
#include "CX_UpdateTimer.h"
///////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// for Linux Fonts
#ifndef WIN32
void InitializeGLUT();
#endif
////////////////////////////////////////////////////////////////////////////////



using namespace std;

///////////////////////////////////////////////////////////////////
// some icons
wxBitmap cx_play32Bitmap();
wxBitmap cx_pause32Bitmap();
wxBitmap cx_stop32Bitmap();
wxBitmap cx_load32Bitmap();
wxBitmap cx_video32Bitmap();
wxBitmap cx_webcam32Bitmap();
wxBitmap cx_ipcam32Bitmap();
wxBitmap cx_settings32Bitmap();
wxBitmap cx_MultiPerson32Bitmap();
wxBitmap cx_pin32Bitmap();
wxBitmap cx_cable32Bitmap();
wxBitmap cx_PersonQuestionGray32Bitmap();
wxBitmap cx_updateBitmap();
wxBitmap cx_Texture50Bitmap();
///////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// uses the wxFileDialog to get a filename for either opening or saving
// filter example = "Image files (*.bmp;*.jpg)|*.bmp;*.jpg|Text file (*.txt)|*.txt|"
// if filter is NULL then it defaults to "All files (*.*)|*.*"
/////////////////////////////////////////////////////////////////////////////
bool GetOpenFilename(string& fname, const char* szFilter = NULL);
bool GetSaveFilename(string& fname, const char* szFilter = NULL);
bool GetOpenFilenameEx(string& fname, const char* szDefFileName = NULL, const char* szFilter = NULL);
bool GetSaveFilenameEx(string& fname, const char* szDefFileName = NULL, const char* szFilter = NULL);
bool GetOpenFilenames(vector<string>& fnames, const char* szFilter = NULL);
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// Define a new application type
///////////////////////////////////////////////////////////////////
class MyApp : public wxApp
{
  public:
    virtual bool OnInit();

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
};
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////
class wxNotebook;
class CX_VideoPanel;
class CX_GalleryPanel;
class CX_EnrollPanel;
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// The frame containing the GL canvas
class MyFrame : public wxFrame
{
  public:
    MyFrame(wxFrame *frame, wxWindowID id = -1);
    virtual ~MyFrame();

    void ConstructNoteBook();

    void PositionWindows();
    bool LoadAureus(string& emsg);
    void ConstructAndSetTitle();

    wxNotebook*       mp_notebook;
    CX_VideoPanel*    mp_video_panel;
    CX_GalleryPanel*  mp_gallery_panel;
    CX_EnrollPanel*   mp_enroll_panel;
    CX_UpdateTimer    m_update_timer;
    cx_int            m_use_n_video_streams;

    CX_AUREUS         mp_aureus;
    string            m_data_dir;

    string            m_title;
    string            m_video_filename;
    string            m_gal_name;

  private:
    void OnSize(wxSizeEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnNoteBookChanged(wxBookCtrlBaseEvent& event);

    wxDECLARE_EVENT_TABLE();
};
///////////////////////////////////////////////////////////////////



#endif //_AUREUS_VIDEO_GUI_H_

