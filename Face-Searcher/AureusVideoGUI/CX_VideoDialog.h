#pragma once
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "CX_ImageGrid.h"
#include "CX_VideoDisplay.h"
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
class CX_ResultsPanel;
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
class CX_VideoDlg : public wxPanel
{
  public:
    void PositionWindows();
    void ConstructToolbar();
    void SetMenuTicks();
    void ShowResults(bool show);

    MyFrame*          mp_mainframe;

    wxToolBar*        mp_toolbar;
    wxMenu*           mp_settings_menu;
    string            m_title;
    CX_VideoDisplay*  mp_video;
    CX_ImageGrid*     mp_grid;

    bool              m_show_results_panel;
    CX_ResultsPanel*  mp_results_panel;

    CX_VideoDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    ~CX_VideoDlg();


    void Play(bool display_errors);

    void OnSize(wxSizeEvent& event);

    // Toolbar
    void OnInputSettings(wxCommandEvent& event);
    void OnMediaFile(wxCommandEvent& event);
    void OnUSBcamera(wxCommandEvent& event);
    void OnIPcamera(wxCommandEvent& event);
    void OnEnrollCode(wxCommandEvent& event);
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

    // Image menu
    void OnDisplayVideo(wxCommandEvent& event);
    void OnSetFDP(wxCommandEvent& event);
    void OnSetFrameInterval(wxCommandEvent& event);
    void OnSetReduceFrameStep(wxCommandEvent& event);
    void OnForceEveryFrame(wxCommandEvent& event);
    void OnLoopMediaFiles(wxCommandEvent& event);

    // Heads menu
    void OnDisplayPeopleOnly(wxCommandEvent& event);
    void OnDisplayOnlyVerified(wxCommandEvent& event);
    void OnDisplayCurrentHeadImages(wxCommandEvent& event);
    void OnDisplayMeshOverlay(wxCommandEvent& event);
    void OnGeneratePCImage(wxCommandEvent& event);
    void OnDisplayPCOverlayImages(wxCommandEvent& event);
    void OnDisplayPCHeadImages(wxCommandEvent& event);
    void OnDisplayFitMeasure(wxCommandEvent& event);

    // FR
    void OnFRsetVerificationThreshold(wxCommandEvent& event);
    void OnFRgenerateTemplatesToggle(wxCommandEvent& event);
    void OnFRperformRankingToggle(wxCommandEvent& event);
    void OnFRsetRankDisplay(wxCommandEvent& event);
    void OnFRDisplayAllPeople(wxCommandEvent& event);

    // Gallery
    void OnGallerySelect(wxCommandEvent& event);

    // Results Settings
    void OnResultsSettings(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
///////////////////////////////////////////////////////////////////
