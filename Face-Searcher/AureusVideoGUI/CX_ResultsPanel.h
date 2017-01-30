#pragma once
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/sizer.h"
#include "CX_ImageGrid.h"
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// wxWidgets Includes
////////////////////////////////////////////////////////////////////////////////
#include <wx/frame.h>
#include <wx/panel.h>
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Aureus includes
#include "Aureus.h"
#include "AureusVideo.h"
#include "AureusHeads.h"
/////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////
class MyFrame;
class CX_VideoDlg;
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
class CX_ResultsPanel : public wxScrolled<wxPanel>
{
  public:
    CX_ResultsPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~CX_ResultsPanel();

    MyFrame*     mp_mainframe;
    CX_VideoDlg*  mp_vd_parent;

    // these are only valid after a call to SetValues
    CX_ResultsSettings m_res;
    CX_ResultsSettings m_default;
    CX_VIDEO           mp_video;

    wxStaticBoxSizer* CreateTagItem(wxWindow* parent, const char* title, wxWindowID check_ID, wxWindowID editID, wxWindowID defaultEditID, bool large = false);

    wxBoxSizer* CreateCompact();
    wxBoxSizer* CreateLarge();

    void SetCheckValue(wxWindowID id, bool value);
    void SetTextValue(wxWindowID id, const char* txt);

    void GetCheckValue(wxWindowID id, bool& value);
    void GetTextValue(wxWindowID id, char* txt);
    void GetTextNumValue(wxWindowID id, cx_uint& value, cx_int min_val, cx_int max_val);
    void Browse(const char* title, wxWindowID id, char* txt);

    void SetValues(CX_VIDEO p_video);

    //void OnSize(wxSizeEvent& event);

    void OnSetAllButton(wxCommandEvent& event);
    void OnOKButton(wxCommandEvent& event);
    void OnCancelButton(wxCommandEvent& event);

    void OnSaveXMLcheck(wxCommandEvent& event);
    void OnRemoveFilesCheck(wxCommandEvent& event);
    void OnOnlyVerifiedCheck(wxCommandEvent& event);
    void OnXMLfolderEdit(wxCommandEvent& event);
    void OnXMLbrowse(wxCommandEvent& event);

    void OnPOSTcheck(wxCommandEvent& event);
    void OnPOSTedit(wxCommandEvent& event);

    void OnSaveImagesCheck(wxCommandEvent& event);
    void OnImagesFolder(wxCommandEvent& event);
    void OnImagesBrowse(wxCommandEvent& event);


    void OnRootTagCheck(wxCommandEvent& event);
    void OnRootTagTag(wxCommandEvent& event);
    void OnRootTagDefault(wxCommandEvent& event);

    void OnPersonNameCheck(wxCommandEvent& event);
    void OnPersonNameTag(wxCommandEvent& event);
    void OnPersonNameDefault(wxCommandEvent& event);

    void OnPersonIDCheck(wxCommandEvent& event);
    void OnPersonIDTag(wxCommandEvent& event);
    void OnPersonIDDefault(wxCommandEvent& event);

    void OnProcessedFramesCheck(wxCommandEvent& event);
    void OnProcessedFramesTag(wxCommandEvent& event);
    void OnProcessedFramesDefault(wxCommandEvent& event);

    void OnHeadIdCheck(wxCommandEvent& event);
    void OnHeadIdTag(wxCommandEvent& event);
    void OnHeadIdDefault(wxCommandEvent& event);

    void OnStreamTypeCheck(wxCommandEvent& event);
    void OnStreamTypeTag(wxCommandEvent& event);
    void OnStreamTypeDefault(wxCommandEvent& event);

    void OnStreamConnectionCheck(wxCommandEvent& event);
    void OnStreamConnectionTag(wxCommandEvent& event);
    void OnStreamConnectionDefault(wxCommandEvent& event);

    void OnStreamIndexCheck(wxCommandEvent& event);
    void OnStreamIndexTag(wxCommandEvent& event);
    void OnStreamIndexDefault(wxCommandEvent& event);

    void OnVThreshCheck(wxCommandEvent& event);
    void OnVThreshTag(wxCommandEvent& event);
    void OnVThreshDefault(wxCommandEvent& event);

    void OnFrameNumCheck(wxCommandEvent& event);
    void OnFrameNumTag(wxCommandEvent& event);
    void OnFrameNumDefault(wxCommandEvent& event);

    void OnUtcTimeCheck(wxCommandEvent& event);
    void OnUtcTimeTag(wxCommandEvent& event);
    void OnUtcTimeDefault(wxCommandEvent& event);

    void OnRankNCheck(wxCommandEvent& event);
    void OnRankNTag(wxCommandEvent& event);
    void OnRankNDefault(wxCommandEvent& event);
    void OnRankNedit(wxCommandEvent& event);

    void OnMatchedPersonNameCheck(wxCommandEvent& event);
    void OnMatchedPersonNameTag(wxCommandEvent& event);
    void OnMatchedPersonNameDefault(wxCommandEvent& event);

    void OnMatchedPersonIDCheck(wxCommandEvent& event);
    void OnMatchedPersonIDTag(wxCommandEvent& event);
    void OnMatchedPersonIDDefault(wxCommandEvent& event);

    void OnMatchedImageIDCheck(wxCommandEvent& event);
    void OnMatchedImageIDTag(wxCommandEvent& event);
    void OnMatchedImageIDDefault(wxCommandEvent& event);

    void OnDateTimeCheck(wxCommandEvent& event);
    void OnDateTimeTag(wxCommandEvent& event);
    void OnDateTimeDefault(wxCommandEvent& event);

    void OnConfidenceCheck(wxCommandEvent& event);
    void OnConfidenceTag(wxCommandEvent& event);
    void OnConfidenceDefault(wxCommandEvent& event);

    void OnFocusCheck(wxCommandEvent& event);
    void OnFocusTag(wxCommandEvent& event);
    void OnFocusDefault(wxCommandEvent& event);

    void OnEyePosCheck(wxCommandEvent& event);
    void OnEyePosTag(wxCommandEvent& event);
    void OnEyePosDefault(wxCommandEvent& event);

    void OnFaceBoxCheck(wxCommandEvent& event);
    void OnFaceBoxTag(wxCommandEvent& event);
    void OnFaceBoxDefault(wxCommandEvent& event);

    void OnMatchScoreCheck(wxCommandEvent& event);
    void OnMatchScoreTag(wxCommandEvent& event);
    void OnMatchScoreDefault(wxCommandEvent& event);

    void OnMatchedStatusCheck(wxCommandEvent& event);
    void OnMatchedStatusTag(wxCommandEvent& event);
    void OnMatchedStatusDefault(wxCommandEvent& event);

    void OnTrackedImCheck(wxCommandEvent& event);
    void OnTrackedImTag(wxCommandEvent& event);
    void OnTrackedImDefault(wxCommandEvent& event);

    void OnPersonImageCheck(wxCommandEvent& event);
    void OnPersonImageTag(wxCommandEvent& event);
    void OnPersonImageDefault(wxCommandEvent& event);

    void OnMatchedImageCheck(wxCommandEvent& event);
    void OnMatchedImageTag(wxCommandEvent& event);
    void OnMatchedImageDefault(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};
///////////////////////////////////////////////////////////////////

