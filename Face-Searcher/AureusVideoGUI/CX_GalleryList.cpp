#include "wx/wx.h"
#include "wx/msgdlg.h"
#include "wx/generic/textdlgg.h"
#include <wx/notebook.h>
#include "AureusVideoGUI.h"
#include "CX_GalleryList.h"
#include "CX_GalleryPanel.h"
///////////////////////////////////////////////////////////////////


static const long ID_SCROLL = wxNewId();
static const long ID_SCROLL_MIN = wxNewId();
static const long ID_SCROLL_MAX = wxNewId();
static const long ID_SCROLL_LINE_UP = wxNewId();
static const long ID_SCROLL_LINE_DOWN = wxNewId();
static const long ID_SCROLL_PAGE_UP = wxNewId();
static const long ID_SCROLL_PAGE_DOWN = wxNewId();


///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_GalleryList, wxWindow)
EVT_SIZE(CX_GalleryList::OnSize)
EVT_SCROLL(CX_GalleryList::OnScroll)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
CX_GalleryList::CX_GalleryList(wxWindow *parent)
: wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC | wxFULL_REPAINT_ON_RESIZE, "People")
{
  mp_mainframe = NULL;
  mp_image_list = NULL;

  mp_image_list = new CX_ImageList(this);
  mp_scrollbar = new wxScrollBar(this, wxID_ANY);

  m_num_people = 0;
  m_start_index = 0;
  m_n = 10;
  m_fin_index = 0;

  mp_image_list->SetSelectionCallBack(SelectionCallBack, this);

  mp_image_list->m_popup_menu_strings.push_back("Add Currently Loaded Image To Person");
  mp_image_list->m_popup_menu_strings.push_back("Delete Selected People");
  mp_image_list->m_popup_menu_strings.push_back("Change Person Name");

  mp_image_list->SetSelectionPopUpCallBack(SelectionPopUpCallBack, this);
  m_sel = -1;
}
///////////////////////////////////////////////////////////////////
CX_GalleryList::~CX_GalleryList()
{
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryList::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryList::SelectPerson(cx_uint person_id)
{
  char msg[1024];
  cx_int index = CX_GetPersonIndex(mp_mainframe->mp_aureus, person_id, msg);
  if (index >= 0)
  {
    cx_uint new_pos = (cx_uint)index;

    if ((new_pos + m_n) >= (cx_uint)m_num_people)
    {
      new_pos = (cx_uint)m_num_people - m_n; // ensure we don't go past the end
    }

    // if we're already at the same posn no point in updating
    if (new_pos != m_start_index) PopulateList(new_pos);

  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryList::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;

  
  cx_int scroll_h = 0;
  if (mp_scrollbar)
  {
#ifdef WIN32
    scroll_h = mp_scrollbar->GetSize().y;
#else
    scroll_h = 25;
#endif
  }

  cx_int list_h = cy - scroll_h;
  cx_int w = cx;

  if (list_h < 0) return;

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
void CX_GalleryList::SelectionCallBack(void* p_object, cx_int sel, cx_coord multi_sel)
{
  if (p_object) ((CX_GalleryList*)p_object)->SelectionCallBack(sel, multi_sel);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_GalleryList::SelectionCallBack(cx_int sel, cx_coord multi_sel)
{
  if (!mp_mainframe) return;

  if (sel < 0) m_sel = sel; // indicates that nothing is singular selected, but we might be in the process of a multiple selection without having lifted the left mouse button yet.
  else m_sel = m_start_index + sel;
  m_multi_sel.r = m_start_index + multi_sel.r;
  m_multi_sel.c = m_start_index + multi_sel.c;
  PopulateList(m_start_index);

  // populate the person image list
  if (m_sel >= 0 && m_sel < (cx_int)m_num_people)
  {
    CX_PersonInfo p;
    char msg[1024];
    if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, m_sel, &p, msg))
    {
      mp_mainframe->mp_gallery_panel->SetPersonList(p);
    }
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_GalleryList::SelectionPopUpCallBack(void* p_object, cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index)
{
  if (p_object) ((CX_GalleryList*)p_object)->SelectionPopUpCallBack(sel, multi_sel, pop_up_string_index);
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_GalleryList::SelectionPopUpCallBack(cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index)
{
  if (!mp_mainframe) return;

  // first trreat this like any other selection so that we can SEE the selection
  m_sel = m_start_index + sel;
  m_multi_sel.r = m_start_index + multi_sel.r;
  m_multi_sel.c = m_start_index + multi_sel.c;
  PopulateList(m_start_index);

  // populate the person image list
  if (m_sel >= 0 && m_sel < (cx_int)m_num_people)
  {
    CX_PersonInfo p;
    char msg[1024];
    if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, m_sel, &p, msg))
    {
      mp_mainframe->mp_gallery_panel->SetPersonList(p);
    }
  }

  // now deal with the menu selection

  // Add image to selected person
  if (0 == pop_up_string_index)
  {
    CX_Image& im = mp_mainframe->mp_gallery_panel->m_selected_image;
    if (im.m_rows>0) // must have an image present
    {
      if (0 == m_num_people) return;

      if (m_sel >= 0 && m_sel < (cx_int)m_num_people)
      {
        CX_PersonInfo p;
        char msg[1024];
        if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, m_sel, &p, msg))
        {
          string txt = string("Are you sure you want to add the image to\n Person: ") + string(p.m_person_name);
          txt += string("\nPerson id ");
          txt += p.m_person_id;

          int answer = wxMessageBox(txt.c_str(), "Confirm", wxYES_NO | wxCANCEL);
          if (answer == wxYES)
          {
            CX_RAM_Image rim;
            rim.mp_pixels = (cx_byte*)im.mp_pixels;
            rim.m_cols = im.m_cols;
            rim.m_rows = im.m_rows;
            rim.m_origin = im.m_origin;
            rim.m_type = im.m_type;

            if (0 == CX_AddImageToPerson(mp_mainframe->mp_aureus, p.m_person_id, &rim, msg))
            {
              wxMessageBox(msg);
            }
            else
            {
              // since we've added a new image get them from the gallery
              mp_mainframe->mp_gallery_panel->RefreshPersonList();
              //mp_mainframe->mp_gallery_panel->Initialize(mp_mainframe);
            }
          }
        }
      }
    }
  }


  // Delete people
  else if (1 == pop_up_string_index)
  {
    if (0 == m_num_people) return;

    if (m_multi_sel.r >= 0 && m_multi_sel.c >= 0 && m_multi_sel.c < (cx_int)m_num_people)
    {
      cx_uint num_people_to_delete = m_multi_sel.c - m_multi_sel.r + 1;
      if (num_people_to_delete > 0)
      {
        string txt = string("Are you sure you want to delete ") + to_string(num_people_to_delete) + " people?";

        int answer = wxMessageBox(txt.c_str(), "Confirm", wxYES_NO | wxCANCEL);
        if (answer == wxYES)
        {
          cx_int i;
          vector<CX_PersonInfo> people;
          char msg[1024];
          for (i = m_multi_sel.r; i <= m_multi_sel.c; i++)
          {
            CX_PersonInfo p;
            if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, i, &p, msg)) people.push_back(p);
          }
          for (i = 0; i < (cx_int)people.size(); i++)
          {
            if (0 == CX_DeletePerson(mp_mainframe->mp_aureus, people[i].m_person_id, msg))
            {
              wxMessageBox(msg);
            }
          }
          // reset gallery
          mp_mainframe->mp_gallery_panel->EmptyPersonList();
          mp_mainframe->mp_gallery_panel->Initialize(mp_mainframe);
        }
      }
    }
  }


  // Change Person Name
  else if (2 == pop_up_string_index)
  {
    if (0 == m_num_people) return;

    if (m_sel >= 0 && m_sel < (cx_int)m_num_people)
    {
      CX_PersonInfo p;
      char msg[1024];
      if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, m_sel, &p, msg))
      {
        wxString res = wxGetTextFromUser("Enter the new person name:", "Enter the new person name", p.m_person_name, this);
        if (res != wxEmptyString)
        {
          if (0 == CX_ChangePersonName(mp_mainframe->mp_aureus, p.m_person_id, res.c_str(), msg))
          {
            wxMessageBox(msg);
          }
          else
          {
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
void CX_GalleryList::Initialize(MyFrame* p_mainframe)
{
  mp_mainframe = p_mainframe;
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  m_num_people = 0;

  char msg[1024];
  cx_int n = CX_GetNumGalleryPeople(mp_mainframe->mp_aureus, msg);
  if (n < 0)
  {
    wxMessageBox(msg);
  }
  m_num_people = n;

  cx_int range = m_num_people;
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(0, thumb_size, range, page_size);

  m_sel = -1;
  m_multi_sel(-1, -1);
  PopulateList(0);
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_GalleryList::OnScroll(wxScrollEvent& event)
{
  if (!mp_mainframe) return;
  if (!mp_scrollbar) return;

  cx_uint new_pos = mp_scrollbar->GetThumbPosition();

  if ((new_pos + m_n) >= (cx_uint)m_num_people)
  {
    new_pos = (cx_uint)m_num_people - m_n; // ensure we don't go past the end
  }

  // if we're already at the same posn no point in updating
  if (new_pos != m_start_index) PopulateList(new_pos);

  event.Skip();
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryList::PopulateList(cx_uint start_index)
{
  char msg[1024];
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  if (m_num_people == 0)
  {
    cx_int n = CX_GetNumGalleryPeople(mp_mainframe->mp_aureus, msg);
    if (n > 0) m_num_people = n;
    else return;
  }

  mp_image_list->Empty();

  cx_uint fin_index = start_index + m_n;
  if (fin_index > (cx_uint)m_num_people) fin_index = (cx_uint)m_num_people;

  m_start_index = start_index;
  m_fin_index = fin_index;

  char buf[100];

  cx_uint rows, cols;
  for (cx_uint i = m_start_index; i < m_fin_index; i++)
  {
    if (i < (cx_uint)m_num_people)
    {
      CX_PersonInfo p;
      if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, i , &p, msg))
      {
        CX_IMAGE p_im = CX_GetPersonThumbnail(mp_mainframe->mp_aureus, p.m_person_id, msg);
        if (p_im)
        {
          cx_byte* p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
          if (p_pixels)
          {
            mp_image_list->m_frames.push_back(CX_ImageItem());
            CX_ImageItem& im = mp_image_list->m_frames.back();
            im.m_frame.Clone(p_pixels, rows, cols,1,1);
            im.m_strings.push_back(string(p.m_person_name));
            sprintf(buf, "P: %d",p.m_person_id);
            im.m_strings.push_back(string(buf));
            im.m_rgb(1.0f, 1.0f, 1.0f);
            if (m_sel == (cx_int)i) im.m_is_selected = true;

            // multi_selection
            if (m_multi_sel.r>=0 && m_multi_sel.r <= (cx_int)i  && m_multi_sel.c >= (cx_int)i) im.m_is_selected = true;
          }
          CX_FreeImage(p_im, msg);
        }
      }
    }
  }
  mp_image_list->Refresh();

  cx_int range = m_num_people;
  cx_int thumb_size = m_n; // n images at a time
  cx_int page_size = m_n - 1;
  mp_scrollbar->SetScrollbar(m_start_index, thumb_size, range, page_size);
}
///////////////////////////////////////////////////////////////////



