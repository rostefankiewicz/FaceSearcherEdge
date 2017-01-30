#pragma once
///////////////////////////////////////////////////////////////////
#include "CX_ImageWindow.h"
#include "CX_ImageList.h"
///////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Aureus includes
#include "Aureus.h"
#include "AureusVideo.h"
#include "AureusHeads.h"
#include "AureusGallery.h"
/////////////////////////////////////////////////////////////////////////////



using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// local includes
/////////////////////////////////////////////////////////////////////////////


class MyFrame;
class wxScrollBar;

///////////////////////////////////////////////////////////////////
class CX_PersonImageList : public wxWindow
{
  public:
    CX_PersonImageList(wxWindow *parent);
    virtual ~CX_PersonImageList();

    void SetPerson(CX_PersonInfo& person);
    void Initialize(MyFrame* p_mainframe);
    void PositionWindows();
    void EmptyPerson();

    void PopulateList(cx_uint start_index);

    static void SelectionCallBack(void* p_object, cx_int sel, cx_coord multi_sel);
    void SelectionCallBack(cx_int sel, cx_coord multi_sel);

    static void SelectionPopUpCallBack(void* p_object, cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index);
    void SelectionPopUpCallBack(cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index);


    MyFrame*      mp_mainframe;
    CX_ImageList* mp_image_list;
    wxScrollBar*  mp_scrollbar;

    CX_PersonInfo m_person;

    cx_uint       m_start_index;
    cx_uint       m_fin_index;
    cx_uint       m_n;
    cx_int        m_sel;

    void OnSize(wxSizeEvent& event);
    void OnScroll(wxScrollEvent& event);

    wxDECLARE_EVENT_TABLE();

};
///////////////////////////////////////////////////////////////////

