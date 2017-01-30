#include "wx/wx.h"
#include "wx/window.h"
#include "wx/scrolbar.h"
#include <wx/notebook.h>
#include "AureusVideoGUI.h"
#include "CX_FRList.h"
#include "CX_GalleryPanel.h"
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_FRList, wxWindow)
EVT_SIZE(CX_FRList::OnSize)
EVT_SCROLL(CX_FRList::OnScroll)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_FRList::CX_FRList(wxWindow *parent)
: wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC, "FR Results")
{
  mp_mainframe = NULL;
  mp_image_list = NULL;

  mp_image_list = new CX_ImageList(this);
  mp_scrollbar = new wxScrollBar(this, wxID_ANY);

  m_start_index = 0;
  m_n = 10;
  m_fin_index = 0;

  mp_image_list->SetSelectionCallBack(SelectionCallBack, this);

  m_sel = -1;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_FRList::~CX_FRList()
{
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_FRList::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_FRList::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;


  cx_int scroll_h = 0;
  if (mp_scrollbar) scroll_h = mp_scrollbar->GetSize().y;

  cx_int list_h = cy - scroll_h;
  cx_int w = cx;

  if (list_h <= 0) return;

  if (mp_scrollbar) mp_scrollbar->SetSize(0, list_h, cx, scroll_h);
  if (mp_image_list) mp_image_list->SetSize(0, 0, cx, list_h);

  cx_real item_width = (cx_real)list_h * 0.7f;
  if (item_width > 0.0f)
  {
    m_n = (cx_uint)cx_round(cx_ceil((cx_real)cx / item_width));
    if (m_n <= 0) m_n = 1;
    PopulateList(m_start_index);
  }
  if (mp_image_list) mp_image_list->Render();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_FRList::Initialize(MyFrame* p_mainframe)
{
  mp_mainframe = p_mainframe;
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  cx_int range = 40;
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(0, thumb_size, range, page_size);

  m_sel = -1;
  PopulateList(0);
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_FRList::OnScroll(wxScrollEvent& event)
{
  if (!mp_mainframe) return;
  if (!mp_scrollbar) return;

  cx_uint new_pos = mp_scrollbar->GetThumbPosition();

  if ((new_pos + m_n) >= (cx_uint)m_rank.size())
  {
    new_pos = (cx_uint)m_rank.size() - m_n; // ensure we don't go past the end
  }

  // if we're already at the same posn no point in updating
  if (new_pos != m_start_index) PopulateList(new_pos);

  event.Skip();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_FRList::SelectionCallBack(void* p_object, cx_int sel, cx_coord multi_sel)
{
  if (p_object) ((CX_FRList*)p_object)->SelectionCallBack(sel, multi_sel);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_FRList::SelectionCallBack(cx_int sel, cx_coord multi_sel)
{
  if (!mp_mainframe) return;

  m_sel = m_start_index + sel;
  PopulateList(m_start_index);

  // populate the person image list
  if (m_sel >= 0 && m_sel < (cx_int)m_rank.size())
  {
    CX_RankItem& r = m_rank[m_sel];
    mp_mainframe->mp_gallery_panel->SelectPersonInPeopleList(r.m_person_id);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_FRList::SetData(CX_RankItem* p_rank, cx_int n)
{
  m_sel = -1;
  m_rank.clear();
  for (cx_int i = 0; i < n; i++)
  {
    m_rank.push_back(p_rank[i]);
  }
  PopulateList(0);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_FRList::PopulateList(cx_uint start_index)
{
  char msg[1024];
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;
  if (m_rank.size() == 0) return;

  mp_image_list->Empty();

  cx_uint fin_index = start_index + m_n;
  if (fin_index > (cx_uint)m_rank.size()) fin_index = (cx_uint)m_rank.size();

  m_start_index = start_index;
  m_fin_index = fin_index;

  cx_uint type = 1;
  cx_uint origin = 1;
  
  //CX_Array<cx_byte> thumb;
  cx_uint rows, cols;
  for (cx_uint i = m_start_index; i < m_fin_index; i++)
  {
    if (i < (cx_uint)m_rank.size())
    {
      CX_RankItem& r = m_rank[i];
      CX_IMAGE p_im = CX_GetImageThumbnail(mp_mainframe->mp_aureus, r.m_image_id, msg);
      if (p_im)
      {
        cx_byte* p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
        if (p_pixels)
        {
          mp_image_list->m_frames.push_back(CX_ImageItem());
          CX_ImageItem& im = mp_image_list->m_frames.back();
          im.m_frame.Clone(p_pixels, rows, cols, type, origin);
          im.m_strings.push_back(to_string(r.m_person_id));
          im.m_strings.push_back(to_string(r.m_score));
          im.m_rgb(1.0f, 1.0f, 1.0f);
          if (m_sel == (cx_int)i) im.m_is_selected = true;
        }
        CX_FreeImage(p_im, msg);
      }
    }
  }
  mp_image_list->Refresh();

  cx_int range = m_rank.size();
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(m_start_index, thumb_size, range, page_size);
}
///////////////////////////////////////////////////////////////////



