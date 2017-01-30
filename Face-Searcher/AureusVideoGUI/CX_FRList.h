#pragma once
///////////////////////////////////////////////////////////////////
#include "CX_ImageWindow.h"
#include "CX_ImageList.h"
///////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Aureus includes
#include "Aureus.h"
#include "AureusHeads.h"
#include "AureusVideo.h"
#include "AureusGallery.h"
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// local includes
/////////////////////////////////////////////////////////////////////////////


class MyFrame;
class wxScrollBar;

///////////////////////////////////////////////////////////////////
class CX_FRList : public wxWindow
{
  public:
    CX_FRList(wxWindow *parent);
    virtual ~CX_FRList();

    void Initialize(MyFrame* p_mainframe);
    void PositionWindows();

    void SetData(CX_RankItem* p_rank, cx_int n);

    void PopulateList(cx_uint start_index);

    static void SelectionCallBack(void* p_object, cx_int sel, cx_coord multi_sel);
    void SelectionCallBack(cx_int sel, cx_coord multi_sel);


    MyFrame*      mp_mainframe;
    CX_ImageList* mp_image_list;
    wxScrollBar*  mp_scrollbar;

    vector<CX_RankItem> m_rank;

    cx_uint       m_start_index;
    cx_uint       m_fin_index;
    cx_uint       m_n;
    cx_int        m_sel;


    void OnSize(wxSizeEvent& event);
    void OnScroll(wxScrollEvent& event);

    wxDECLARE_EVENT_TABLE();
};
///////////////////////////////////////////////////////////////////

