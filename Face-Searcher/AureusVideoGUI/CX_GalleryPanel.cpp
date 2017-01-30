///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/textdlg.h"
#ifdef WIN32
#include "wx/msw/choice.h"
#else
#include "wx/gtk/choice.h"
#endif
#include <wx/notebook.h>
#include "CX_ImageWindow.h"
#include "CX_3Dwin.h"
#include "CX_GalleryPanel.h"
#include "AureusVideoGUI.h"
#include "CX_GalleryList.h"
#include "CX_PersonImageList.h"
#include "CX_FRList.h"
#include "CX_GalleryInfoPanel.h"
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
static const long ID_GALLERY_TOOLBAR = wxNewId();
static const long ID_GALLERY_LOAD_IMAGE = wxNewId();
static const long ID_GALLERY_ADD_TO_GALLERY = wxNewId();
static const long ID_GALLERY_APPLY_FR = wxNewId();
static const long ID_GALLERY_FR_NAMES = wxNewId();
static const long ID_GALLERY_CHANGE_GALLERY = wxNewId();
static const long ID_GALLERY_UPDATE_FR = wxNewId();
static const long ID_GALLERY_PROGRESS = wxNewId();
static const long GALLERY_INFO_PANEL = wxNewId();
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_GalleryPanel, wxPanel)
EVT_SIZE(CX_GalleryPanel::OnSize)
EVT_ACTIVATE(CX_GalleryPanel::OnActivate)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryPanel::GetFRnames()
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  char msg[1024];
  m_FRnames.clear();
  int n = CX_GetNumFRengines(mp_mainframe->mp_aureus, msg);
  if (n > 0)
  {
    for (cx_int i = 0; i < n; i++)
    {
      const char* p = CX_GetFRname(mp_mainframe->mp_aureus, i, msg);
      if (p)
      {
        m_FRnames.push_back(p);
      }
    }
  }
  m_FRselection = -1;
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_GalleryPanel::ConstructToolbar()
{
  //mp_toolbar = new wxToolBar(this, ID_VIDEO_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_VIDEO_TOOLBAR"));
  mp_toolbar = new wxToolBar(this, ID_GALLERY_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_TEXT | wxSUNKEN_BORDER | wxNO_BORDER, _T("ID_VIDEO_TOOLBAR"));
  //mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(20, 20)));
  mp_toolbar->SetToolBitmapSize(wxDLG_UNIT(this, wxSize(16, 16)));

  //mp_toolbar->AddSeparator();
  mp_toolbar->AddTool(ID_GALLERY_LOAD_IMAGE, "Load Image", cx_load32Bitmap());
  mp_toolbar->AddTool(ID_GALLERY_ADD_TO_GALLERY, "Enroll", cx_MultiPerson32Bitmap());
  mp_toolbar->AddTool(ID_GALLERY_APPLY_FR, "Apply FR", cx_PersonQuestionGray32Bitmap());
  
  mp_toolbar->AddSeparator();
  
  // FR Engine selection combo box
  wxComboBox* combo = new wxComboBox(mp_toolbar, ID_GALLERY_FR_NAMES, wxEmptyString, wxDefaultPosition, wxSize(120, -1), 0, 0, wxCB_READONLY);
  GetFRnames(); // first get them from the SDK
  for (cx_uint i = 0; i < m_FRnames.size(); i++) combo->Append(m_FRnames[i].c_str());
  mp_toolbar->AddControl(combo, wxT("FR Engines"));


  mp_toolbar->AddTool(ID_GALLERY_CHANGE_GALLERY, "Change Gallery", cx_load32Bitmap());
  mp_toolbar->AddTool(ID_GALLERY_UPDATE_FR, "UpdateFR", cx_updateBitmap());

  mp_progress = new wxGauge(mp_toolbar, ID_GALLERY_PROGRESS, 100, wxDefaultPosition, wxSize(100,-1));
  mp_progress->Show(false);
  mp_toolbar->AddControl(mp_progress, wxT("Progress"));

  mp_toolbar->Realize();

  Connect(ID_GALLERY_LOAD_IMAGE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnGalleryLoadImage);
  Connect(ID_GALLERY_ADD_TO_GALLERY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnAddImageToGallery);
  Connect(ID_GALLERY_APPLY_FR, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnApplyFR);
  Connect(ID_GALLERY_FR_NAMES, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnFRChoice);
  Connect(ID_GALLERY_CHANGE_GALLERY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnChangeGallery);
  Connect(ID_GALLERY_UPDATE_FR, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_GalleryPanel::OnUpdateFRs);

  combo->SetSelection(0);

  char msg[1024];
  CX_SetUpdateFRCallBack(mp_mainframe->mp_aureus, ProgressCallBack, this, msg);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnFRChoice(wxCommandEvent& event)
{
  wxComboBox* cbox = (wxComboBox*)mp_toolbar->FindControl(ID_GALLERY_FR_NAMES);
  if (cbox)
  {
    cx_int sel = cbox->GetSelection();
    if (sel != wxNOT_FOUND)
    {
      m_FRselection = sel;
      char msg[1024];
      wxCursor cursor(wxCURSOR_WAIT);
      if (0 == CX_SelectFRengine(mp_mainframe->mp_aureus, m_FRselection, msg))
      {
        wxMessageBox(msg);
      }
    }
  }
}
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
CX_GalleryPanel::CX_GalleryPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxPanel(parent, id, pos, size, style, name)
{
  mp_mainframe = mainframe;
  mp_toolbar = NULL;
  mp_people_list = NULL;
  mp_person_image_list = NULL;
  mp_im_display = NULL;
  mp_progress = NULL;
  mp_info_panel = NULL;
  m_is_first_selection = true;

  ConstructToolbar();

  mp_info_panel = new CX_GalleryInfoPanel(mainframe, this, GALLERY_INFO_PANEL);

  mp_people_list = new CX_GalleryList(this);
  mp_person_image_list = new CX_PersonImageList(this);
  mp_fr_list = new CX_FRList(this);


  mp_im_display = new CX_ImageWindow(this);
  mp_im_display->SetDisplayCallBack(ImageDisplayCallBack, this);

  mp_3dwin = new CX_3Dwin(this);
  mp_pcim_display = new CX_ImageWindow(this);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
CX_GalleryPanel::~CX_GalleryPanel()
{
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::ProgressCallBack(const char* msg, cx_int total, cx_int current, void* p_object)
{
  if (p_object)
  {
    ((CX_GalleryPanel*)p_object)->ProgressCallBack(msg, total, current);
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::ProgressCallBack(const char* msg, cx_int total, cx_int current)
{
  if (mp_progress)
  {
    if (current < 0)
    {
      mp_progress->Show(false);
    }
    else
    {
      cx_int val = cx_round(100.0f * (cx_real)current / (cx_real)total);
      mp_progress->Show(true);
      mp_progress->SetValue(val);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::PositionWindows()
{
  if (!IsShownOnScreen()) return;

  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 100 || cy <= 100) return;


  cx_int top = 0;
  if (mp_toolbar)
  {
#ifdef WIN32
    top = mp_toolbar->GetSize().y;
#else
    top = 60;
#endif
    mp_toolbar->SetSize(0, 0, cx, top);
  }
  cx_int border = 10;

  cx_int list_left = cx_round((cx_real)cx*0.35f);
  cx_int list_h = 150;
  cx_int list_w = cx - list_left;

  cx_int info_h = 100;
  cx_int info_w = list_left;

  cx_int im_w = info_w-border;
  cx_int im_h = cx_round(cy - (top + border + info_h))/2;

  cx_int mesh_h = cx_round((cx_real)im_h * 0.75f);
  cx_int mesh_w = im_w/2;

  if (mp_info_panel && info_h>0) mp_info_panel->SetSize(0, top + border, info_w, info_h);
  if (mp_people_list) mp_people_list->SetSize(list_left, top + border, list_w - 2 * border, list_h);
  if (mp_person_image_list) mp_person_image_list->SetSize(list_left, top + border + list_h + border, list_w - 2 * border, list_h);
  if (mp_fr_list) mp_fr_list->SetSize(list_left, top + border + list_h + border + list_h + border, list_w - 2 * border, list_h);

  if (mp_im_display) mp_im_display->SetSize(0, top + border + info_h, im_w, im_h);
  if (mp_3dwin) mp_3dwin->SetSize(0, top + border + info_h + im_h + border, mesh_w, mesh_h);
  if (mp_pcim_display) mp_pcim_display->SetSize(mesh_w, top + border + info_h + im_h + border, mesh_w, mesh_h);
  
  
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnSize(wxSizeEvent& event)
{
  PositionWindows();
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnActivate(wxActivateEvent& event)
{
  if (event.GetActive())
  {
    Show(true);
    PositionWindows();
  }
  PositionWindows();
}
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
bool CX_GalleryPanel::Initialize(MyFrame* p_mainframe)
{
  mp_mainframe = p_mainframe;
  if (mp_people_list) mp_people_list->Initialize(p_mainframe);
  if (mp_person_image_list) mp_person_image_list->Initialize(p_mainframe);
  if (mp_fr_list) mp_fr_list->Initialize(p_mainframe);
  mp_info_panel->SetInfo();
  return true;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::EmptyPersonList()
{
  if (mp_person_image_list) mp_person_image_list->EmptyPerson();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::SetPersonList(CX_PersonInfo& person)
{
  if (mp_person_image_list) mp_person_image_list->SetPerson(person);
  else
  {
    if (person.m_num_images > 0)
    {
      SetPersonImage(person.m_image_ids[0]);
    }
  }
  
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::SetPersonImage(cx_int image_id)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;
  if (!mp_im_display) return;

  char msg[1024];
  CX_IMAGE p_im = CX_GetGalleryImage(mp_mainframe->mp_aureus, image_id, msg);
  if (!p_im)
  {
    wxMessageBox(msg);
    return;
  }

  cx_uint rows, cols;
  cx_byte* p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
  if (p_pixels)
  {
    m_selected_image.Clone(p_pixels, rows, cols, 1, 1);
    mp_im_display->SetImage(m_selected_image);
    mp_im_display->Seek();
    mp_im_display->Refresh();
    Update3D();
  }

  
  
  CX_FreeImage(p_im, msg);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::Update3D()
{
  // Generate and display 3D mesh
  CX_DetectionParams fdp;
  fdp.m_left = 0.0f;  fdp.m_top = 0.0f;  fdp.m_height = 1.0f; fdp.m_width = 1.0f;
  fdp.m_min_height_prop = 0.3f; fdp.m_max_height_prop = 1.0f;

  CX_RAM_Image rim;
  rim.mp_pixels = m_selected_image.mp_pixels; rim.m_rows = m_selected_image.m_rows; rim.m_cols = m_selected_image.m_cols;
  rim.m_origin = 1; rim.m_type = 1;
  char msg[1024];

  if (0 == CX_Generate3DMesh(mp_mainframe->mp_aureus, &rim, fdp, &m_mesh, msg))
  {
    wxMessageBox(msg);
  }
  else
  {
    mp_3dwin->SetMesh(&m_mesh);
    mp_3dwin->Seek();
    mp_3dwin->Refresh3D();
  }

  // Generate & display pose corrected image
  CX_IMAGE p_pcim = CX_GeneratePoseCorrectedImage(mp_mainframe->mp_aureus, &rim, fdp, 480, msg);
  if (!p_pcim)
  {
    wxMessageBox(msg);
  }
  else
  {
    cx_uint rows, cols;
    cx_byte* p_pixels = CX_ImageData(p_pcim, &rows, &cols, msg);
    if (p_pixels)
    {
      m_pc_image.Clone(p_pixels, rows, cols, 1, 1);
      mp_pcim_display->SetImage(m_pc_image);
      mp_pcim_display->Seek();
      mp_pcim_display->Refresh();
    }
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::ImageDisplayCallBack(void* p_object)
{
  if (p_object)
  {
    ((CX_GalleryPanel*)p_object)->ImageDisplayCallBack();
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::ImageDisplayCallBack()
{
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnGalleryLoadImage(wxCommandEvent& event)
{
  string fname;
  string filter("Files (*.jpg;*.bmp)|*.jpg;*.bmp||");
  if (!GetOpenFilenameEx(fname, NULL, filter.c_str())) return;

  wxBusyCursor wait;
  CX_Image im;
  if (!im.Load(fname.c_str()))
  {
    string msg = string("Failed to load:\n") + fname;
    wxMessageBox(msg.c_str());
    return;
  }

  m_selected_image.Clone(im);
  mp_im_display->SetImage(m_selected_image);
  mp_im_display->Seek();

  Update3D();
}
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnApplyFR(wxCommandEvent& event)
{
  if (m_selected_image.m_rows == 0) return;

  CX_RankItem rank[40];
  char msg[1024];

  CX_RAM_Image rim;
  rim.mp_pixels = m_selected_image.mp_pixels;
  rim.m_rows = m_selected_image.m_rows;
  rim.m_cols = m_selected_image.m_cols;
  rim.m_type = m_selected_image.m_type;
  rim.m_origin = m_selected_image.m_origin;

  cx_int n = CX_ApplyFR(mp_mainframe->mp_aureus, &rim, rank, 40, msg);
  if (n < 0)
  {
    wxMessageBox(msg);
    return;
  }

  if (mp_fr_list) mp_fr_list->SetData(rank, n);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnAddImageToGallery(wxCommandEvent& event)
{
  if (m_selected_image.m_rows == 0) return;


  wxString res = wxGetTextFromUser(wxT("Enter Person Name"), wxT("Person Name:"), "P", this);

  if (res == wxEmptyString) return;

  string name(res.c_str());

  CX_RAM_Image rim;
  rim.mp_pixels = m_selected_image.mp_pixels;
  rim.m_rows = m_selected_image.m_rows;
  rim.m_cols = m_selected_image.m_cols;
  rim.m_type = m_selected_image.m_type;
  rim.m_origin = m_selected_image.m_origin;


  char msg[1024];
  cx_int person_id = CX_AddNewPerson(mp_mainframe->mp_aureus, name.c_str(), &rim, msg);

  if (person_id < 0)
  {
    wxMessageBox(msg);
    return;
  }

  // update gallery display
  (void)Initialize(mp_mainframe);

}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::RefreshPersonList()
{
  cx_int sel = mp_people_list->m_sel;
  if (sel >= 0)
  {
    CX_PersonInfo p;
    char msg[1024];
    if (0 != CX_GetPersonInfo(mp_mainframe->mp_aureus, sel, &p, msg))
    {
      SetPersonList(p);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::SelectPersonInPeopleList(cx_uint person_id)
{
  if (mp_people_list) mp_people_list->SelectPerson(person_id);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnChangeGallery(wxCommandEvent& event)
{
  string fname;
  string filter("Files (*.gal)|*.gal||");
  if (!GetOpenFilenameEx(fname, NULL, filter.c_str())) return;

  wxBusyCursor wait;
  char msg[1024];
  if (0 == CX_ChangeGallery(mp_mainframe->mp_aureus, fname.c_str(), msg))
  {
    wxMessageBox(msg);
    return;
  }

  // update gallery display
  (void)Initialize(mp_mainframe);
}
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
void CX_GalleryPanel::OnUpdateFRs(wxCommandEvent& event)
{
  if (!mp_mainframe) return;
  if (!mp_mainframe->mp_aureus) return;

  char msg[1024 * 100];
  cx_int ret_val = CX_UpdateFR(mp_mainframe->mp_aureus, msg);

  if (ret_val != 1)
  {
    wxMessageBox(msg);
  }
  else
  {
    wxMessageBox("Update FRs Successfull");
  }
}
////////////////////////////////////////////////////////////////////////////////
