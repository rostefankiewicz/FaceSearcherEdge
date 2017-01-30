#pragma once
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/tbarbase.h"
#ifdef WIN32
#include "wx/msw/toolbar.h"
#else
#include "wx/gtk/toolbar.h"
#endif
#include "CX_ImageGrid.h"
#include "CX_VideoDisplay.h"
#include "CX_ImageWindow.h"
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// wxWidgets Includes
////////////////////////////////////////////////////////////////////////////////
#include <wx/frame.h>
#include <wx/panel.h>
////////////////////////////////////////////////////////////////////////////////


using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////
class MyFrame;
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
class CX_EnrollPanel : public wxPanel
{
  public:
    CX_EnrollPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~CX_EnrollPanel();

    void ConstructToolbar();
    void PositionWindows();

    // stream opening functions
    bool PlayUSBCamera();
    bool PlayIPCamera();
    void Pause();
    void Stop();

    // Frame processing direct from video interface
    static void FrameCallBack(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, cx_int status, const char* msg, void* p_object);
    static void UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object);
    void FrameCallBack(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, cx_int status, const char* msg);

    bool Initialize();

    MyFrame*          mp_mainframe;

    wxToolBar*        mp_toolbar;
    CX_ImageWindow*   mp_video;
    bool              m_is_first_frame;
    bool              m_is_paused;
    CX_ENROLL_VIDEO   mp_enroll_video;

    cx_int            m_stream_type; // -1 = not set, 1=USB cam, 2=IP cam
    cx_int            m_usb_pin;
    string            m_ip_url;


    // handlers
    void OnSize(wxSizeEvent& event);

    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

    void OnIPcamera(wxCommandEvent& event);
    void OnUSBcamera(wxCommandEvent& event);
    void OnInputSettings(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
///////////////////////////////////////////////////////////////////

