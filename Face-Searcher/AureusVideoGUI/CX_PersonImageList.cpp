///////////////////////////////////////////////////////////////////
#include <wx/notebook.h>
#include "AureusVideoGUI.h"
#include "CX_PersonImageList.h"
#include "CX_GalleryPanel.h"
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_PersonImageList, wxWindow)
EVT_SIZE(CX_PersonImageList::OnSize)
EVT_SCROLL(CX_PersonImageList::OnScroll)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_PersonImageList::CX_PersonImageList(wxWindow *parent)
: wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC, "Person")
{
  m_person.m_person_id = -1;
  m_person.m_num_images = 0;

  mp_mainframe = NULL;
  mp_image_list = NULL;

  mp_image_list = new CX_ImageList(this);
  mp_scrollbar = new wxScrollBar(this, wxID_ANY);

  m_start_index = 0;
  m_n = 10;
  m_fin_index = 0;
  m_sel = -1;

  mp_image_list->SetSelectionCallBack(SelectionCallBack, this);

  mp_image_list->m_popup_menu_strings.push_back("Delete Image");

  mp_image_list->SetSelectionPopUpCallBack(SelectionPopUpCallBack, this);

}
///////////////////////////////////////////////////////////////////
CX_PersonImageList::~CX_PersonImageList()
{
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_PersonImageList::EmptyPerson()
{
  m_person.m_person_id = -1;
  m_person.m_num_images = 0;
  m_sel = -1;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_PersonImageList::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_PersonImageList::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;


  cx_int scroll_h = 0;
  if (mp_scrollbar) scroll_h = mp_scrollbar->GetSize().y;

  cx_int list_h = cy - scroll_h;
  cx_int w = cx;

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
void CX_PersonImageList::SelectionCallBack(void* p_object, cx_int sel, cx_coord multi_sel)
{
  if (p_object) ((CX_PersonImageList*)p_object)->SelectionCallBack(sel, multi_sel);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_PersonImageList::SelectionCallBack(cx_int sel, cx_coord multi_sel)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  
  m_sel = m_start_index + sel;
  PopulateList(m_start_index);

  if (m_sel >= 0 && m_sel < (cx_int)m_person.m_num_images)
  {
    mp_mainframe->mp_gallery_panel->SetPersonImage(m_person.m_image_ids[m_sel]);
  }
  
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_PersonImageList::SelectionPopUpCallBack(void* p_object, cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index)
{
  if (p_object) ((CX_PersonImageList*)p_object)->SelectionPopUpCallBack(sel, multi_sel, pop_up_string_index);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_PersonImageList::SelectionPopUpCallBack(cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;


  if (0 == pop_up_string_index)
  {
    m_sel = m_start_index + sel;
    PopulateList(m_start_index);

    if (m_sel >= 0 && m_sel < (cx_int)m_person.m_num_images)
    {
      cx_int image_id = m_person.m_image_ids[m_sel];
      string txt = string("Are you sure you want delete image ") + to_string(image_id);
      txt += string("\nFrom Person : ") + string(m_person.m_person_name);
      txt += string("\nPerson id ") + to_string(m_person.m_person_id);

      bool one_image_in_person = false;
      if (m_person.m_num_images == 1)
      {
        txt += string("\n\nNOTE: proceeding will also delete the person since\nit has a single image");
        one_image_in_person = true;
      }

      char msg[1024];

      int answer = wxMessageBox(txt.c_str(), "Confirm", wxYES_NO | wxCANCEL);
      if (answer == wxYES)
      {
        if (0 == CX_DeleteImage(mp_mainframe->mp_aureus, image_id, msg))
        {
          wxMessageBox(msg);
          return;
        }
        else
        {
          // if it's a simple image delete then just referesh the person image list
          if (!one_image_in_person) mp_mainframe->mp_gallery_panel->RefreshPersonList();
          // otherwise the person has been removed so update the people list
          else
          {
            EmptyPerson();
            mp_mainframe->mp_gallery_panel->Initialize(mp_mainframe);
          }
        }
      }
    }
  }

  else
  {
    wxMessageBox("Unknown pop up");
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_PersonImageList::Initialize(MyFrame* p_mainframe)
{
  mp_mainframe = p_mainframe;
  if (!mp_mainframe) return;

  cx_int range = m_person.m_num_images;
  if (range == 0) range = 2;
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(0, thumb_size, range, page_size);

  m_sel = -1;
  PopulateList(0);
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_PersonImageList::OnScroll(wxScrollEvent& event)
{
  if (!mp_mainframe) return;
  if (!mp_scrollbar) return;

  
  if (m_person.m_num_images > 0)
  {
    cx_uint new_pos = mp_scrollbar->GetThumbPosition();

    if ((new_pos + m_n) >= (cx_uint)m_person.m_num_images)
    {
      new_pos = (cx_uint)m_person.m_num_images - m_n; // ensure we don't go past the end
    }

    // if we're already at the same posn no point in updating
    if (new_pos != m_start_index) PopulateList(new_pos);
  }
  
  event.Skip();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_PersonImageList::SetPerson(CX_PersonInfo& person)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  m_person = person;
  PopulateList(0);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_PersonImageList::PopulateList(cx_uint start_index)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

 
  if (m_person.m_num_images == 0) return;

  mp_image_list->Empty();

  cx_uint fin_index = start_index + m_n;
  if (fin_index > (cx_uint)m_person.m_num_images) fin_index = (cx_uint)m_person.m_num_images;

  m_start_index = start_index;
  m_fin_index = fin_index;
  char msg[1024];
  char buf[100];

  cx_uint rows, cols;
  for (cx_uint i = m_start_index; i < m_fin_index; i++)
  {
    if (i < (cx_uint)m_person.m_num_images)
    {
      cx_int image_id = m_person.m_image_ids[i];
      CX_IMAGE p_im = CX_GetImageThumbnail(mp_mainframe->mp_aureus, image_id, msg);
      if (p_im)
      {
        cx_byte* p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
        if (p_pixels)
        {
          mp_image_list->m_frames.push_back(CX_ImageItem());
          CX_ImageItem& im = mp_image_list->m_frames.back();
          im.m_frame.Clone(p_pixels, rows, cols, 1, 1);
          string s = string(m_person.m_person_name) + string(" ") + to_string(image_id);
          im.m_strings.push_back(s);
          sprintf(buf, "P: %d", m_person.m_person_id);
          im.m_strings.push_back(string(buf));
          im.m_rgb(1.0f, 1.0f, 1.0f);
          if (m_sel == (cx_int)i) im.m_is_selected = true;
        }
        CX_FreeImage(p_im, msg);
      }
    }
  }
  mp_image_list->Refresh();

  cx_int range = m_person.m_num_images;
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(m_start_index, thumb_size, range, page_size);
  
}
///////////////////////////////////////////////////////////////////
