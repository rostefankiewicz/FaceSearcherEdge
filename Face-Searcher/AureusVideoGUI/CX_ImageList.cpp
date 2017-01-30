#include "wx/wx.h"
#include "wx/event.h"
#include "wx/cmdline.h"
#include "wx/menu.h"
#include <wx/notebook.h>
//////////////////////////////////////////////////////////
#include "AureusVideoGUI.h"
#include "CX_ImageList.h"
/////////////////////////////////////////////////////////////////////////////

// for Linux Fonts
#ifndef WIN32
#include <GL/glut.h>
#endif


//////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_ImageList, wxGLCanvas)
EVT_SIZE(CX_ImageList::OnSize)
EVT_PAINT(CX_ImageList::OnPaint)
EVT_ERASE_BACKGROUND(CX_ImageList::OnEraseBackground)
EVT_MOUSE_EVENTS(CX_ImageList::OnMouseEvent)
wxEND_EVENT_TABLE()
//////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
CX_ImageList::CX_ImageList(wxWindow *parent)
: wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE |
wxCLIP_CHILDREN | wxCLIP_SIBLINGS)
{
#ifndef WIN32
  // initialize glut
  InitializeGLUT();
#endif

  m_OGLcreated = false;
  m_back_col((cx_real)1, (cx_real)1, (cx_real)1);

  mp_SelectionCallBack = NULL;
  mp_selection_callback_object = NULL;

  mp_SelectionPopUpCallBack = NULL;
  mp_selection_popup_callback_object = NULL;

  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);

  DisableUnusedOGL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  wxRect r = wxWindow::GetClientRect();
  m_win_size.c = r.GetWidth();
  m_win_size.r = r.GetHeight();
  m_win_aspect = (cx_real)m_win_size.c / (cx_real)m_win_size.r;

  // this is here to ensure the background is cleared properly before first showing
  SetCurrent(*m_glRC);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);
  glFlush();
  SwapBuffers();
#ifdef WIN32
  wglMakeCurrent(NULL, NULL);
#endif
  m_sel_range(-1,-1);

#ifdef WIN32
  wxClientDC dc(this);
  HFONT font = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, L"Ariel Black");
  wglMakeCurrent(dc.GetHDC(), m_glRC->GetGLRC());
  HFONT	oldfont = (HFONT)SelectObject(dc.GetHDC(), font);
  wglUseFontBitmaps(dc.GetHDC(), 0, 256, 1000);
  SelectObject(dc.GetHDC(), oldfont);
  DeleteObject(font);
#endif

}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
CX_ImageList::~CX_ImageList()
{
  if (m_glRC)
  {
    SetCurrent(*m_glRC);
    delete m_glRC;
  }
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::SetSelectionCallBack(lpCallBackFunc p_func, void* p_object)
{
  mp_SelectionCallBack = p_func;
  mp_selection_callback_object = p_object;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::SetSelectionPopUpCallBack(lpPopUpCallBackFunc p_func, void* p_object)
{
  mp_SelectionPopUpCallBack = p_func;
  mp_selection_popup_callback_object = p_object;
}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_ImageList::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing on MSW
}
////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::OnPaint(wxPaintEvent& event)
{
  // This is a dummy, to avoid an endless succession of paint messages.
  // OnPaint handlers must always create a wxPaintDC.
  wxPaintDC dc(this);

  if (!IsShownOnScreen()) return;

  Render();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::DisableUnusedOGL()
{
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);

  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
  glDisable(GL_LOGIC_OP);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
  glPixelTransferi(GL_RED_SCALE, 1);
  glPixelTransferi(GL_RED_BIAS, 0);
  glPixelTransferi(GL_GREEN_SCALE, 1);
  glPixelTransferi(GL_GREEN_BIAS, 0);
  glPixelTransferi(GL_BLUE_SCALE, 1);
  glPixelTransferi(GL_BLUE_BIAS, 0);
  glPixelTransferi(GL_ALPHA_SCALE, 1);
  glPixelTransferi(GL_ALPHA_BIAS, 0);

  /*
  // Disable extensions that could slow down glDrawPixels.
  const GLubyte* extString = glGetString(GL_EXTENSIONS);

  if (extString != NULL) {
  if (strstr((char*) extString, "GL_EXT_convolution") != NULL) {
  glDisable(GL_CONVOLUTION_1D_EXT);
  glDisable(GL_CONVOLUTION_2D_EXT);
  glDisable(GL_SEPARABLE_2D_EXT);
  }
  if (strstr((char*) extString, "GL_EXT_histogram") != NULL) {
  glDisable(GL_HISTOGRAM_EXT);
  glDisable(GL_MINMAX_EXT);
  }
  if (strstr((char*) extString, "GL_EXT_texture3D") != NULL) {
  glDisable(GL_TEXTURE_3D_EXT);
  }
  }
  */
}
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
void CX_ImageList::DrawTextBackGround(vector<string>& text, cx_real x, cx_real y, const CX_Vector3D& rgb)
{
  /*
  if (text.size() == 0) return;
  cx_real maxd = 0.0f;
  glPushMatrix();
  glTranslatef(x, y, 0.0);
  glScalef(0.1f, 0.1f, 0.1f);
  for (cx_uint i = 0; i < text.size(); i++)
  {
    cx_real d = glutStrokeLengthf(GLUT_STROKE_ROMAN, (const cx_byte*)text[i].c_str());
    maxd = CX_MAX(maxd, d);
  }

  // draw rectangle background
  cx_real w = maxd;
  //cx_real w = (cx_real)s.Length() * 80.0f;
  CX_Vector3D trgb = CX_Vector3D(1, 1, 1) - rgb;
  glColor3f(trgb.x(), trgb.y(), trgb.z());
  cx_real h = (cx_real)80 * (cx_real)text.size();
  cx_real wo = 15.0f;
  glBegin(GL_POLYGON);
  glVertex2f(x - wo, y - h);
  glVertex2f(x + w, y - h);
  glVertex2f(x + w, y + h);
  glVertex2f(x - wo, y + h);
  glEnd();

  glPopMatrix();

  */
}
////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Writes text at posn (x,y) origin bot left
void CX_ImageList::DrawStringWithBackground(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb)
{
  /*
  glPushMatrix();
  glTranslatef(x, y, 0.0);

  glScalef(0.1f, 0.1f, 0.1f);

  // draw rectangle background
  cx_real w = glutStrokeLengthf(GLUT_STROKE_ROMAN, (const cx_byte*)s.c_str());
  //cx_real w = (cx_real)s.Length() * 80.0f;
  CX_Vector3D trgb = CX_Vector3D(1, 1, 1) - rgb;
  glColor3f(trgb.x(), trgb.y(), trgb.z());
  cx_real h = (cx_real)120;
  cx_real wo = 15.0f;
  glBegin(GL_POLYGON);
  glVertex2f(-wo, -wo);
  glVertex2f(w + wo, -wo);
  glVertex2f(w + wo, h + wo);
  glVertex2f(-wo, h + wo);
  glEnd();

  glColor3f(rgb.x(), rgb.y(), rgb.z());
  for (cx_uint i = 0; i < s.size(); i++)
  {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, s.c_str()[i]);
  }
  glPopMatrix();
  */
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Writes text at posn (x,y) origin bot left
void CX_ImageList::DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb)
{
#ifdef WIN32
  glColor3f(rgb.x(), rgb.y(), rgb.z());
  glRasterPos2f(x, y);
  glListBase(1000);
  glCallLists(strlen(s.c_str()), GL_UNSIGNED_BYTE, s.c_str());
#else
  glPushMatrix();
  glTranslatef(x, y, 0.0);

  glScalef(0.1f, 0.1f, 0.1f);

  // draw rectangle background
  cx_real w = glutStrokeLength(GLUT_STROKE_ROMAN, (const cx_byte*)s.c_str());
  CX_Vector3D trgb = CX_Vector3D(1, 1, 1) - rgb;
  glColor3f(trgb.x(), trgb.y(), trgb.z());
  cx_real h = (cx_real)120;
  cx_real wo = 15.0f;
  glBegin(GL_POLYGON);
  glVertex2f(-wo, -wo);
  glVertex2f(w + wo, -wo);
  glVertex2f(w + wo, h + wo);
  glVertex2f(-wo, h + wo);
  glEnd();

  glColor3f(rgb.x(), rgb.y(), rgb.z());
  for (cx_uint i = 0; i < (cx_uint)s.size(); i++)
  {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, s.c_str()[i]);
  }
  glPopMatrix();
#endif
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::Render()
{
  cx_uint i;
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);
  //glColor3f(1.0f, 0.0f, 0.0f);

  cx_real border = (cx_real)(m_win_size.r)* (cx_real)0.025;

  cx_real trans_x = 0.0f;

  for (i = 0; i<m_frames.size(); i++)
  {
    CX_ImageItem& fdisp = m_frames[i];
    CX_Image& im = fdisp.m_frame;
    CX_Vector3D& rgb = fdisp.m_rgb;

    // calc zoom based on image height
    //cx_real zoom = ((cx_real)m_win_size.r / (cx_real)im.m_rows) * (cx_real)0.95;
    cx_real image_zoom = ((cx_real)m_win_size.r / (cx_real)im.m_rows) * (cx_real)0.70;
    cx_real cell_zoom = ((cx_real)m_win_size.r / (cx_real)im.m_rows);
    
    cx_real image_width = ((cx_real)im.m_cols*image_zoom); // width of displayed image
    cx_real cell_width = ((cx_real)im.m_cols*cell_zoom);
    cx_real cell_border = (cell_width - image_width) * 0.5f; 
    cx_real image_height = ((cx_real)im.m_rows*image_zoom);
    cx_real height_border = ((cx_real)m_win_size.r - image_height) * 0.9f;

    // display image
    glPixelStoref(GL_UNPACK_SKIP_PIXELS, 0.0f);
    glRasterPos2f(trans_x + cell_border, height_border);
    glPixelZoom(image_zoom, image_zoom);
    glDrawPixels(im.m_cols, im.m_rows, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)im.mp_pixels);

    fdisp.m_x_left = trans_x;
    fdisp.m_x_right = fdisp.m_x_left + cell_width;

    // draw rect around image
    glDisable(GL_TEXTURE_2D);
    glLineWidth(3.0f);
    // display different colour if selected
    if (fdisp.m_is_selected) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(rgb.x(), rgb.y(), rgb.z());
    cx_real x = trans_x + (border*0.5f);
    cx_real y = (border*0.5f);
    cx_real w = cell_width - border;
    cx_real h = (cx_real)(m_win_size.r) - border;
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    // draw filled rectangle background (string will be drawn on top of this)
    glColor3f(rgb.x(), rgb.y(), rgb.z());
    x = trans_x + border;
    y = border;
    w = image_width;
    h = (cx_real)15;
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Display string info
    CX_Vector3D trgb = CX_Vector3D(1, 1, 1) - rgb;
    
    for (cx_uint k = 0; k < fdisp.m_strings.size(); k++)
    {
      string& str = fdisp.m_strings[k];
      x = trans_x + border + border;
      y = border + border + (cx_real)(fdisp.m_strings.size() - k - 1)*15.0f;
      DrawString(str, x, y, trgb);
    }
    
    // move trans_x on to start position of next image
    trans_x += cell_width;
  }


  glFlush();
  SwapBuffers();
#ifdef WIN32
  wglMakeCurrent(NULL, NULL);
#endif
}
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::Empty()
{
  m_frames.clear();
  Refresh();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::Seek()
{
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen()) return;

  cx_int cx = event.GetSize().GetWidth();
  cx_int cy = event.GetSize().GetHeight();
  if (0 >= cx || 0 >= cy)	return;

  m_win_size.c = cx;
  m_win_size.r = cy;
  m_win_aspect = (cx_real)m_win_size.c / (cx_real)m_win_size.r;

  if (0 >= cx || 0 >= cy)	return;

  SetCurrent(*m_glRC);
  glViewport(0, 0, cx, cy);

  // select the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(0,cx,0,cy,-1.0,1.0);
  gluOrtho2D(0.0, (GLfloat)cx, 0.0, (GLfloat)cy);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageList::ShowContextMenu(const cx_coord& pos, cx_int sel, cx_coord multi_sel)
{
  if (m_popup_menu_strings.size() == 0) return;

  wxMenu menu(wxPOPUP_WINDOW);

  wxPoint pnt;
  pnt.x = pos.c;
  pnt.y = pos.r;

  menu.SetTitle("Options:");
  int start_index = 200;

  vector<cx_int> IDs;
  for (cx_uint i = 0; i < m_popup_menu_strings.size(); i++)
  {
    IDs.push_back(start_index + i);
    menu.Append(IDs.back(), m_popup_menu_strings[i].c_str(), " ", false);
  }

  const int rc = GetPopupMenuSelectionFromUser(menu, pnt);
  if (rc == wxID_NONE) return;
  else
  {
    for (cx_uint k = 0; k < IDs.size(); k++)
    {
      if (rc == IDs[k])
      {
        if (mp_SelectionPopUpCallBack) (mp_SelectionPopUpCallBack)(mp_selection_popup_callback_object, sel, multi_sel, IDs[k] - start_index);
        break;
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
void CX_ImageList::OnMouseEvent(wxMouseEvent& event)
{
  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  bool update_display = false;

  cx_real x = (cx_real)event.GetX();
  cx_real y = (cx_real)event.GetY();

  cx_int sel = -1;
  for (cx_uint i = 0; i < m_frames.size(); i++)
  {
    if (x>m_frames[i].m_x_left && x < m_frames[i].m_x_right)
    {
      sel = i;
      break;
    }
  }

  // multiple selection
  if (event.LeftDown() && sel>=0)
  {
    m_sel1 = sel;
    m_sel2 = sel;
  }
  else if (event.LeftIsDown())
  {
    bool changed = m_sel2 != sel;
    m_sel2 = sel;

    if (changed)
    {
      // ensure correct order
      cx_coord sel_range(m_sel1, m_sel2);
      if (m_sel1 > m_sel2 && m_sel2 != -1) sel_range(m_sel2, m_sel1);

      if (mp_SelectionCallBack) (mp_SelectionCallBack)(mp_selection_callback_object, -1, sel_range);
    }
  }


  if (event.LeftUp())
  {
    // ensure correct order
    cx_coord sel_range(m_sel1, m_sel2);
    if (m_sel1 > m_sel2 && m_sel2 != -1) sel_range(m_sel2, m_sel1);

    if (mp_SelectionCallBack) (mp_SelectionCallBack)(mp_selection_callback_object, sel, sel_range);
  }

  else if (event.RightUp())
  {
    // ensure correct order
    cx_coord sel_range(m_sel1, m_sel2);
    if (m_sel1 > m_sel2 && m_sel2 != -1) sel_range(m_sel2, m_sel1);

    cx_coord p(y, x);
    ShowContextMenu(p, sel, sel_range);
  }
}
////////////////////////////////////////////////////////////////////////////////
