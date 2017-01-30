#pragma once
////////////////////////////////////////////////////////////////////////////////
#include <wx/frame.h>
#include <wx/panel.h>
#include "CX_VideoDialog.h"
////////////////////////////////////////////////////////////////////////////////


using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////


class MyFrame;


////////////////////////////////////////////////////////////////////////////////
class CX_VideoPanel : public wxPanel
{
  public:
    CX_VideoPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~CX_VideoPanel();

    bool Initialize(MyFrame* p_mainframe);
    void UpdateLicenseInfo();
    void ConstructToolbar();
    void PositionWindows();

    MyFrame*              mp_mainframe;
    wxToolBar*            mp_toolbar;
    vector<CX_VideoDlg*>  m_videos;


  private:
    void OnSize(wxSizeEvent& event);

    void OnPlayAll(wxCommandEvent& event);
    void OnPauseAll(wxCommandEvent& event);
    void OnStopAll(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
////////////////////////////////////////////////////////////////////////////////

