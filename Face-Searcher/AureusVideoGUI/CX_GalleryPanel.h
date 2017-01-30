#pragma once
////////////////////////////////////////////////////////////////////////////////
#include <wx/frame.h>
#include <wx/panel.h>
#ifdef WIN32
#include <wx/msw/gauge.h>
#else
#include <wx/gtk/gauge.h>
#endif
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Aureus includes
#include "Aureus.h"
#include "AureusHeads.h"
#include "AureusVideo.h"
#include "AureusGallery.h"
/////////////////////////////////////////////////////////////////////////////



using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////


class MyFrame;
class CX_GalleryList;
class CX_PersonImageList;
class CX_FRList;
class CX_ImageWindow;
class CX_GalleryInfoPanel;
class CX_3Dwin;

////////////////////////////////////////////////////////////////////////////////
class CX_GalleryPanel : public wxPanel
{
  public:
    CX_GalleryPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~CX_GalleryPanel();

    void ConstructToolbar();
    void PositionWindows();
    bool Initialize(MyFrame* p_mainframe);
    void SetPersonList(CX_PersonInfo& person);
    void EmptyPersonList();
    void SetPersonImage(cx_int image_id);
    void RefreshPersonList();
    void Update3D();

    void SelectPersonInPeopleList(cx_uint person_id);

    static void ProgressCallBack(const char* msg, cx_int total, cx_int current, void* p_object);
    void ProgressCallBack(const char* msg, cx_int total, cx_int current);

    static void ImageDisplayCallBack(void* p_object);
    void ImageDisplayCallBack();

    void GetFRnames();

    MyFrame*             mp_mainframe;
    wxToolBar*           mp_toolbar;
    wxGauge*             mp_progress;
    CX_GalleryList*      mp_people_list; // displays people thumnails etc.
    CX_PersonImageList*  mp_person_image_list; // displays images associated with person
    CX_ImageWindow*      mp_im_display;
    CX_FRList*           mp_fr_list;
    CX_GalleryInfoPanel* mp_info_panel;
    CX_3Dwin*            mp_3dwin;
    CX_ImageWindow*      mp_pcim_display;

    // full size selected image from mp_person_image_list selection
    // or loaded from a file
    CX_Image             m_selected_image;
    CX_MESH3D            m_mesh;
    CX_Image             m_pc_image;

    vector<string>       m_FRnames;
    cx_int               m_FRselection;
    bool                 m_is_first_selection;

private:
    void OnSize(wxSizeEvent& event);
    void OnActivate(wxActivateEvent& event);

    // Toolbar
    void OnGalleryLoadImage(wxCommandEvent& event);
    void OnAddImageToGallery(wxCommandEvent& event);
    void OnApplyFR(wxCommandEvent& event);
    void OnFRChoice(wxCommandEvent& event);
    void OnChangeGallery(wxCommandEvent& event);
    void OnUpdateFRs(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};
////////////////////////////////////////////////////////////////////////////////

