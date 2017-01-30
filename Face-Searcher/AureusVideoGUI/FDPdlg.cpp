///////////////////////////////////////////////////////////////////
// include main wxWidgets
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/cmdline.h"
#include "wx/bmpcbox.h"
#include "wx/choice.h"
#include "wx/dialog.h"
#include "wx/button.h"
///////////////////////////////////////////////////////////////////
#include "FDPdlg.h"
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
static const long ID_FDP_TOP = wxNewId();
static const long ID_FDP_LEFT = wxNewId();
static const long ID_FDP_HEIGHT = wxNewId();
static const long ID_FDP_WIDTH = wxNewId();
static const long ID_FDP_MINP = wxNewId();
static const long ID_FDP_MAXP = wxNewId();
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
FDPdlg::FDPdlg(wxWindow*  parent, wxWindowID  id, const wxString&  title, CX_DetectionParams& fdp)
  : wxDialog(parent, id, title, wxDefaultPosition, wxSize(230,270))
{
  m_fdp = fdp;
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  wxPanel *panel = new wxPanel(this, -1);
  wxStaticBox *st = new wxStaticBox(panel, -1, wxT("Face Detection Parameters"), wxPoint(5, 5), wxSize(200, 200));

  wxStaticText* top_txt = new wxStaticText(panel, -1, "Top:", wxPoint(15, 30));
  wxStaticText* left_txt = new wxStaticText(panel, -1, "Left:", wxPoint(15, 55));
  wxStaticText* height_txt = new wxStaticText(panel, -1, "Height:", wxPoint(15, 80));
  wxStaticText* width_txt = new wxStaticText(panel, -1, "Width:", wxPoint(15, 105));
  wxStaticText* minp_txt = new wxStaticText(panel, -1, "Min Prop:", wxPoint(15, 130));
  wxStaticText* maxp_txt = new wxStaticText(panel, -1, "Max Prop:", wxPoint(15, 155));
  wxTextCtrl* top = new wxTextCtrl(panel, ID_FDP_TOP, wxString::Format(wxT("%1.3f"), m_fdp.m_top).c_str(), wxPoint(80, 30));
  wxTextCtrl* left = new wxTextCtrl(panel, ID_FDP_LEFT, wxString::Format(wxT("%1.3f"), m_fdp.m_left).c_str(), wxPoint(80, 55));
  wxTextCtrl* height = new wxTextCtrl(panel, ID_FDP_HEIGHT, wxString::Format(wxT("%1.3f"), m_fdp.m_height).c_str(), wxPoint(80, 80));
  wxTextCtrl* width = new wxTextCtrl(panel, ID_FDP_WIDTH, wxString::Format(wxT("%1.3f"), m_fdp.m_width).c_str(), wxPoint(80, 105));
  wxTextCtrl* minp = new wxTextCtrl(panel, ID_FDP_MINP, wxString::Format(wxT("%1.3f"), m_fdp.m_min_height_prop).c_str(), wxPoint(80, 130));
  wxTextCtrl* maxp = new wxTextCtrl(panel, ID_FDP_MAXP, wxString::Format(wxT("%1.3f"), m_fdp.m_max_height_prop).c_str(), wxPoint(80, 155));

  wxButton *okButton = new wxButton(this, wxID_OK, wxT("Ok"));
  wxButton *cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

  hbox->Add(okButton, 1);
  hbox->Add(cancelButton, 1, wxLEFT, 5);

  vbox->Add(panel, 1);
  vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

  Connect(ID_FDP_TOP, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnTopEdit);
  Connect(ID_FDP_LEFT, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnLeftEdit);
  Connect(ID_FDP_HEIGHT, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnHeightEdit);
  Connect(ID_FDP_WIDTH, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnWidthEdit);
  Connect(ID_FDP_MINP, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnMinpEdit);
  Connect(ID_FDP_MAXP, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&FDPdlg::OnMaxpEdit);

  SetSizer(vbox);
}
///////////////////////////////////////////////////////////////////
FDPdlg::~FDPdlg()
{
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void FDPdlg::OnTopEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_TOP);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_top = val;
  }
}
///////////////////////////////////////////////////////////////////
void FDPdlg::OnLeftEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_LEFT);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_left = val;
  }
}
///////////////////////////////////////////////////////////////////
void FDPdlg::OnHeightEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_HEIGHT);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_height = val;
  }
}
///////////////////////////////////////////////////////////////////
void FDPdlg::OnWidthEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_WIDTH);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_width = val;
  }
}
///////////////////////////////////////////////////////////////////
void FDPdlg::OnMinpEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_MINP);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_min_height_prop = val;
  }
}
///////////////////////////////////////////////////////////////////
void FDPdlg::OnMaxpEdit(wxCommandEvent& event)
{
  wxTextCtrl* c = (wxTextCtrl*)this->FindWindow(ID_FDP_MAXP);
  if (c->IsModified())
  {
    wxString s = c->GetLineText(0);
    cx_real val = (cx_real)atof(s.c_str());
    if (val >= 0.0f && val <= 1.0f) m_fdp.m_max_height_prop = val;
  }
}
///////////////////////////////////////////////////////////////////
