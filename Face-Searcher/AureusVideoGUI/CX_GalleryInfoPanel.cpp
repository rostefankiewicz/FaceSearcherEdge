#include "wx/wx.h"
#include "wx/stattext.h"
#include <wx/notebook.h>
#include "CX_GalleryPanel.h"
#include "AureusVideoGUI.h"
#include "CX_GalleryInfoPanel.h"
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
static const long ID_INFO_STATIC = wxNewId();
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_GalleryInfoPanel, wxPanel)
//EVT_SIZE(CX_GalleryInfoPanel::OnSize)
wxEND_EVENT_TABLE()
///////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
CX_GalleryInfoPanel::CX_GalleryInfoPanel(MyFrame* mainframe, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  :wxPanel(parent, id, pos, size, style, name)
{
  mp_mainframe = mainframe;

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  wxStaticText* txt = new wxStaticText(this, ID_INFO_STATIC, wxEmptyString);

  vbox->Add(txt, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxEXPAND, 1);

  SetSizer(vbox);

}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
CX_GalleryInfoPanel::~CX_GalleryInfoPanel()
{
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_GalleryInfoPanel::SetInfo()
{
  if (!mp_mainframe) return;
  wxStaticText* c = (wxStaticText*)this->FindWindow(ID_INFO_STATIC);
  if (!c) return;

  char msg[1024];
  string txt = mp_mainframe->m_gal_name + "\n";

  txt += "Num People = ";
  cx_int n_people = CX_GetNumGalleryPeople(mp_mainframe->mp_aureus, msg);
  if (n_people < 0) txt += "Unknown\n";
  else txt += to_string(n_people) + "\n";

  txt += "Num Images = ";
  cx_int n_images = CX_GetNumGalleryImages(mp_mainframe->mp_aureus, msg);
  if (n_images < 0) txt += "Unknown\n";
  else txt += to_string(n_images) + "\n";


  cx_int n_engines = CX_GetNumFRengines(mp_mainframe->mp_aureus, msg);
  if (n_engines > 0)
  {
    txt += string("Num FR Engines = ") + to_string(n_engines);
    txt += "\n";
    for (cx_int i = 0; i < n_engines; i++)
    {
      const char* name = CX_GetFRname(mp_mainframe->mp_aureus, i, msg);
      if (name)
      {
        cx_int n_templates = CX_GetFRnumTemplates(mp_mainframe->mp_aureus, i, msg);
        if (n_templates >= 0)
        {
          txt += string(name) + ": ";
          txt += to_string(n_templates) + " templates\n";
        }
      }
    }
  }

  c->SetLabelText(txt.c_str());
}
/////////////////////////////////////////////////////////////////////////////
