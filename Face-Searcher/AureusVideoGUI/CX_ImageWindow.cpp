////////////////////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/event.h"
#include "wx/cmdline.h"
#include <wx/notebook.h>
////////////////////////////////////////////////////////////////////////////////
#include "AureusVideoGUI.h"
#include "CX_ImageWindow.h"
#include "CX_ImageGrid.h"
////////////////////////////////////////////////////////////////////////////////

// for Linux Fonts
#ifndef WIN32
#include <GL/glut.h>
#endif


static const long ID_REFRESH_FRAME = wxNewId();


//////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_ImageWindow, wxGLCanvas)
EVT_SIZE(CX_ImageWindow::OnSize)
EVT_ACTIVATE(CX_ImageWindow::OnActivate)
EVT_PAINT(CX_ImageWindow::OnPaint)
EVT_MOUSE_EVENTS(CX_ImageWindow::OnMouseEvent)
EVT_ERASE_BACKGROUND(CX_ImageWindow::OnEraseBackground)
wxEND_EVENT_TABLE()
//////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
CX_ImageWindow::CX_ImageWindow(wxWindow *parent, wxWindowID id, int* gl_attrib)
: wxGLCanvas(parent, id, gl_attrib, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE |
wxCLIP_CHILDREN | wxCLIP_SIBLINGS)
{
#ifndef WIN32
  // initialize glut
  InitializeGLUT();
#endif

  m_text_rgb(1.0f, 1.0f, 1.0f);

  m_OGLcreated = false;
  m_back_col((cx_real)0.7, (cx_real)0.7, (cx_real)0.7);

  m_mouse_op = CX_VIEWPOINT;
  m_frame_loaded = false;
  m_zoom = (cx_real)1.0;
  m_trans(0.0f, 0.0f);
  m_first_size = true;
  m_npos = CX_Vector2D(0.5f, 0.5f);

  mp_DisplayCallBack = NULL;
  mp_display_callback_object = NULL;
  mp_LeftUpCallBack = NULL;
  mp_leftup_callback_object = NULL;

  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);

  DisableUnusedOGL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  wxRect r = wxWindow::GetClientRect();
  m_win_size.c = r.GetWidth();
  m_win_size.r = r.GetHeight();
  m_win_aspect = (cx_real)m_win_size.c / (cx_real)m_win_size.r;

#ifdef WIN32
  wxClientDC dc(this);
  HFONT font = CreateFont(-18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, L"Ariel Black");
  wglMakeCurrent(dc.GetHDC(), m_glRC->GetGLRC());
  HFONT	oldfont = (HFONT)SelectObject(dc.GetHDC(), font);
  wglUseFontBitmaps(dc.GetHDC(), 0, 256, 1000);
  SelectObject(dc.GetHDC(), oldfont);	
  DeleteObject(font);	
#endif

  m_do_seek = true;
  Connect(ID_REFRESH_FRAME, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_ImageWindow::OnRefreshFrame);

}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
CX_ImageWindow::~CX_ImageWindow()
{
  if (m_glRC)
  {
    SetCurrent(*m_glRC);
    delete m_glRC;
  }
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::SetDisplayCallBack(lpCallBackFunc p_func, void* p_object)
{
  mp_DisplayCallBack = p_func;
  mp_display_callback_object = p_object;
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::SetLeftUpCallBack(lpLeftUpCallBack p_func, void* p_object)
{
  mp_LeftUpCallBack = p_func;
  mp_leftup_callback_object = p_object;
}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing on MSW
}
////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::Seek()
{
  if (m_frame_loaded)
  {
    if (m_win_size.c < m_win_size.r)
    {
      m_zoom = (cx_real)m_win_size.c / (cx_real)m_frame.m_cols;
      m_trans.x() = 0.0f;
      m_trans.y() = ((cx_real)m_win_size.r - (m_zoom*(cx_real)m_frame.m_rows)) / 2.0f;
    }
    else
    {
      m_zoom = (cx_real)m_win_size.r / (cx_real)m_frame.m_rows;
      m_trans.x() = ((cx_real)m_win_size.c - (m_zoom*(cx_real)m_frame.m_cols)) / 2.0f;
      m_trans.y() = 0.0f;
    }
    Render();
  }
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::OnActivate(wxActivateEvent& event)
{
  if (event.GetActive()) PositionWindows();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::PositionWindows()
{
  if (!IsShownOnScreen()) return;
  cx_int cx, cy;
  GetClientSize(&cx, &cy);
  if (cx <= 0 || cy <= 0) return;

  if (cx < cy)
  {
    m_zoom *= (cx_real)cx / (cx_real)m_win_size.c;
    m_trans *= (cx_real)cx / (cx_real)m_win_size.c;
  }
  else
  {
    m_zoom *= (cx_real)cy / (cx_real)m_win_size.r;
    m_trans *= (cx_real)cy / (cx_real)m_win_size.r;
  }

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

  Render();
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::OnSize(wxSizeEvent& event)
{
  PositionWindows();
  return;

  if (!IsShownOnScreen()) return;

  cx_int cx = event.GetSize().GetWidth();
  cx_int cy = event.GetSize().GetHeight();

  if (cx < cy)
  {
    m_zoom *= (cx_real)cx / (cx_real)m_win_size.c;
    m_trans *= (cx_real)cx / (cx_real)m_win_size.c;
  }
  else
  {
    m_zoom *= (cx_real)cy / (cx_real)m_win_size.r;
    m_trans *= (cx_real)cy / (cx_real)m_win_size.r;
  }

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
void CX_ImageWindow::OnPaint(wxPaintEvent& event)
{
  // This is a dummy, to avoid an endless succession of paint messages.
  // OnPaint handlers must always create a wxPaintDC.
  wxPaintDC dc(this);

  if (!IsShownOnScreen()) return;

  Render();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisableUnusedOGL()
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
CX_Vector2D CX_ImageWindow::GetNegTrans()
{
  CX_Vector2D trans = m_trans;
  if (cx_fabs(m_zoom)>0.0f)
  {
    if (trans.x()>0.0f) trans.x() /= m_zoom;
    if (trans.y()>0.0f) trans.y() /= m_zoom;
  }
  return trans;
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::OnMouseEvent(wxMouseEvent& event)
{
  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  bool update_display = false;

  cx_coord point(event.GetY(), event.GetX());

  bool moving = event.Moving(); // true if moving and no button is down
  bool dragging = event.Dragging(); // true if moving with a button down

  bool bLeftPressed = event.LeftIsDown();
  bool bRightPressed = event.RightIsDown();
  bool bBothPressed = bLeftPressed && bRightPressed;
  bool bControlPressed = event.ControlDown();
  bool bShiftPressed = event.ShiftDown();



  // mouse & rel movement in anno coords
  CX_Vector2D p((cx_real)point.c, (cx_real)(m_win_size.r - point.r));
  if (cx_fabs(m_zoom)>0.0f) p /= m_zoom;
  p -= GetNegTrans();
  if (m_frame_loaded) p.DivideElements(m_frame.RealSize());
  CX_Vector2D prel = p - m_npos;
  m_npos = p;

  cx_coord rel = point - m_mouse_posn;
  m_mouse_posn = point;

  if (event.LeftDClick()) Seek();


  if (event.LeftUp())
  {
    if (mp_LeftUpCallBack) (mp_LeftUpCallBack)(mp_leftup_callback_object, m_npos, m_mouse_posn);
  }


  //--------- VIEWPOINT (or annotating but nothing selected) -----------
  if ((m_mouse_op == CX_VIEWPOINT) || (bControlPressed && (m_mouse_op == CX_ANNOTATING)))
  {
    if (bBothPressed)  // ZOOM
    {
      m_zoom += (cx_real)0.01 * (cx_real)rel.r;
      update_display = true;
    }
    else if (bRightPressed) // TRANSLATION
    {
      cx_real s = 1.0f;
      if (m_zoom>0.0f) s /= m_zoom;
      m_trans.x() += s * (cx_real)rel.c * 2.0f;
      m_trans.y() -= s * (cx_real)rel.r * 2.0f;
      update_display = true;
    }
  }

  //----------- EYE POSITIONING---------------------------------
  else if (CX_EYE_POSITION == m_mouse_op && m_frame_loaded)
  {
    update_display = true;
  }

  if (update_display) Render();

}
////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Writes text at posn (x,y) origin bot left
void CX_ImageWindow::DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb)
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
void CX_ImageWindow::Render()
{
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);
  glColor3f(1.0f, 0.0f, 0.0f);

  CX_Vector2D trans = m_trans;

  if (m_frame_loaded)
  {
    CX_Image& im = m_frame;
    GLvoid* pix = (GLvoid*)im.mp_pixels;
    cx_int h = im.m_rows;

    // for negative translation we need to change the draw start position
    if (m_trans.x()<0.0f)
    {
      glPixelStoref(GL_UNPACK_SKIP_PIXELS, CX_ABS(m_trans.x()));
      trans.x() = 0.0f;
    }
    else glPixelStoref(GL_UNPACK_SKIP_PIXELS, 0.0f);


    if (m_trans.y()<0.0f)
    {
      trans.y() = 0.0f;
      cx_uint r = (cx_uint)cx_round(cx_fabs(m_trans.y()));
      h = im.m_rows - r - 1;
      cx_uint row_size = im.m_cols * 4; // NB! assumes RGBA
      cx_byte* row_pointer = &im.mp_pixels[r*row_size];
      pix = (GLvoid*)row_pointer;
    }

    glRasterPos2f(trans.x(), trans.y());
    glPixelZoom(m_zoom, m_zoom);


    glDrawPixels(im.m_cols, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

        
    for (cx_uint i = 0; i < m_text.size(); i++)
    {
      cx_real x = 20.0f;
      cx_real y = (cx_real)m_win_size.r - 20.0f - ((cx_real)i*15.0f);
      DrawString(m_text[i], x, y, m_text_rgb);
    }
    
  }


  //DisplayFaces();
  if (mp_DisplayCallBack) (mp_DisplayCallBack)(mp_display_callback_object);


  glFlush();
  SwapBuffers();

#ifdef WIN32
  wglMakeCurrent(NULL, NULL);
#endif
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisplayEllipse(CX_Vector2D& center, CX_Vector2D& radii, const CX_Vector3D& rgb, cx_uint n_points)
{
  glDisable(GL_TEXTURE_2D);
  glColor3f(rgb.x(), rgb.y(), rgb.z());
  glLineWidth((GLfloat)3);
  CX_Vector2D size = m_frame.RealSize();

  CX_Vector2D nt = GetNegTrans();

  CX_Vector2D cen = center;
  CX_Vector2D rad = radii;
  cen.MultiplyElements(size);
  rad.MultiplyElements(size);
  rad *= 0.5f;

  CX_Vector2D p;
  cx_uint step = 360 / n_points;

  glBegin(GL_LINE_LOOP);

  for (cx_uint deg = 0; deg < 360; deg += step)
  {
    p.x() = rad.x() * cx_cos(cx_deg2rad((cx_real)deg));
    p.y() = rad.y() * cx_sin(cx_deg2rad((cx_real)deg));
    p += cen;
    p += nt;
    p *= m_zoom;
    glVertex2f(p.x(), p.y());
  }
  glEnd();

  glLineWidth((GLfloat)1);
  glEnable(GL_TEXTURE_2D);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisplayMeshOverlay(CX_MESH2D& mesh)
{
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.0f,0.0f,1.0f);
  glLineWidth((GLfloat)1);
  CX_Vector2D size = m_frame.RealSize();

  CX_Vector2D nt = GetNegTrans();

  for (int i = 0; i < mesh.m_num_polys; i++)
  {
    CX_POLY2D& p = mesh.m_polys[i];
    if (p.m_is_front)
    {
      glBegin(GL_LINE_LOOP);
      for (int k = 0; k < p.m_elems; k++)
      {
        CX_VECTOR2D& v = mesh.m_verts[p.m_indices[k]];
        CX_Vector2D p(v.x, v.y);
        p += nt;
        p *= m_zoom;
        glVertex2f(p.x(), p.y());
      }
      glEnd();
    }
  }

  glLineWidth((GLfloat)1);
  glEnable(GL_TEXTURE_2D);

}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisplayImageOverlay(CX_Vector2D& center, CX_Vector2D& radii, CX_IMAGE p_im)
{
  char msg[1024];
  cx_uint rows, cols;
  cx_byte* p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
  if (!(p_pixels && rows > 0 && cols > 0))
  {
    DisplayEllipse(center, radii, CX_Vector3D(1.0f, 1.0f, 1.0f));
    return;
  }

  GLvoid* pix = (GLvoid*)p_pixels;
  cx_int h = (cx_int)rows;

  CX_Vector2D size = m_frame.RealSize();
  CX_Vector2D nt = GetNegTrans();
  CX_Vector2D cen = center;
  CX_Vector2D rad = radii;
  cen.MultiplyElements(size);
  rad.MultiplyElements(size);
  rad *= 0.5f;

  CX_Vector2D trans = cen - rad;
  trans += nt;
  trans *= m_zoom;

  cx_real zoom = m_zoom * (2.0f * (cx_real)rad.x() / (cx_real)cols);

  glRasterPos2f(trans.x(), trans.y());
  glPixelZoom(zoom, zoom);

  glDrawPixels(cols, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisplayNPoint(CX_Vector2D& np, const CX_Vector3D& rgb)
{
  glDisable(GL_TEXTURE_2D);
  glColor3f(rgb.x(), rgb.y(), rgb.z());
  CX_Vector2D size = m_frame.RealSize();
  cx_real ps = (cx_real)5 * m_zoom;
  if (ps<2.0f) ps = 2.0f;
  glPointSize((GLfloat)ps);


  CX_Vector2D p = np;
  p.MultiplyElements(size);
  p += GetNegTrans();
  p *= m_zoom;

  glBegin(GL_POINTS);
  glVertex2f(p.x(), p.y());
  glEnd();

  glEnable(GL_TEXTURE_2D);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::DisplayNLine(CX_Vector2D& np1, CX_Vector2D& np2, const CX_Vector3D& rgb)
{
  glDisable(GL_TEXTURE_2D);
  glColor3f(rgb.x(), rgb.y(), rgb.z());
  CX_Vector2D size = m_frame.RealSize();
  glLineWidth((GLfloat)2);


  CX_Vector2D nt = GetNegTrans();

  CX_Vector2D p1 = np1;
  p1.MultiplyElements(size);
  p1 += nt;
  p1 *= m_zoom;

  CX_Vector2D p2 = np2;
  p2.MultiplyElements(size);
  p2 += nt;
  p2 *= m_zoom;

  glBegin(GL_LINES);
  glVertex2f(p1.x(), p1.y());
  glVertex2f(p2.x(), p2.y());
  glEnd();

  glEnable(GL_TEXTURE_2D);
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::SetImage(CX_Image& im)
{
  m_frame.Clone(im);
  m_frame_loaded = true;
  Render();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// MUST be an RGBA image to work properly
/////////////////////////////////////////////////////////////////////////////
void CX_ImageWindow::SetImage(cx_byte* p_pixels, cx_uint rows, cx_uint cols)
{
  m_frame.Clone(p_pixels, rows, cols,1,1);
  m_frame_loaded = true;
  Render();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// this function allows the image window to refresh from a different thread
// even when the GUI framework is not thread safe (e.g. GTK). It uses a lock
// to safely copy the frame, then sends a message to the GUI thread to refresh
// the display.
void CX_ImageWindow::SetImageRefreshPaint(cx_byte* p_pixels, cx_uint rows, cx_uint cols, vector<string>* p_texts)
{
  m_frame_lock.lock();
  m_frame.Clone(p_pixels, rows, cols, 1, 1);
  if (p_texts)
  {
    cx_uint n = (cx_uint)p_texts->size();
    if (n > m_text.size()) n = m_text.size();
    for (cx_uint i = 0; i < n; i++)
    {
      m_text[i] = (*p_texts)[i];
    }
  }
  m_frame_loaded = true;
  m_frame_lock.unlock();

  wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_REFRESH_FRAME);
  this->AddPendingEvent(event);
  //this->ProcessEvent(event);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// this function allows the image window to refresh from a different thread
// even when the GUI framework is not thread safe (e.g. GTK). It uses a lock
// to safely copy the frame, then sends a message to the GUI thread to refresh
// the display.
void CX_ImageWindow::SetImageRefreshPaint(CX_Image& im, vector<string>* p_texts)
{
  m_frame_lock.lock();
  m_frame.Clone(im);
  if (p_texts)
  {
    cx_uint n = (cx_uint)p_texts->size();
    if (n > m_text.size()) n = m_text.size();
    for (cx_uint i = 0; i < n; i++)
    {
      m_text[i] = (*p_texts)[i];
    }
  }
  m_frame_loaded = true;
  m_frame_lock.unlock();

  wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_REFRESH_FRAME);
  this->AddPendingEvent(event);
  //this->ProcessEvent(event);
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// this function allows the image window to re-refresh from a different thread
// even when the GUI framework is not thread safe (e.g. GTK). It uses a lock
// to safely mark a loaded frame (assumes one is loaded), then sends a message 
// to the GUI thread to refresh the display.
void CX_ImageWindow::RefreshPaint(vector<string>* p_texts)
{
  m_frame_lock.lock();
  //m_frame_lock.WaitThenLock();
  m_frame_loaded = true;
  cx_uint n = p_texts->size();
  if (n > m_text.size()) n = m_text.size();
  for (cx_uint i = 0; i < n; i++)
  {
    m_text[i] = (*p_texts)[i];
  }
  m_frame_lock.unlock();

  wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_REFRESH_FRAME);
  this->AddPendingEvent(event);
  //this->ProcessEvent(event);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// this function reacts to the refresh message and checks the stomic do_seek 
// variable.
void CX_ImageWindow::OnRefreshFrame(wxCommandEvent& event)
{
  m_frame_lock.lock();
  Render();
  if (m_do_seek)
  {
    m_do_seek = false;
    Seek();
  }
  m_frame_lock.unlock();

}
/////////////////////////////////////////////////////////////////////////////
