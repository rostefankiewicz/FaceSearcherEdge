#include "wx/wx.h"
#include "wx/event.h"
#include "wx/cmdline.h"
#include <wx/notebook.h>
//////////////////////////////////////////////////////////
#include "CX_ImageGrid.h"
/////////////////////////////////////////////////////////////////////////////


// for Linux Fonts
#ifndef WIN32
#include <GL/glut.h>
#endif


static const long ID_REFRESH_GRID = wxNewId();


//////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_ImageGrid, wxGLCanvas)
EVT_SIZE(CX_ImageGrid::OnSize)
EVT_PAINT(CX_ImageGrid::OnPaint)
EVT_ERASE_BACKGROUND(CX_ImageGrid::OnEraseBackground)
wxEND_EVENT_TABLE()
//////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// cebase includes
////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
////////////////////////////////////////////////////////////////////////////////


using namespace std;


/////////////////////////////////////////////////////////////////////////////
CX_ImageGrid::CX_ImageGrid(wxWindow *parent, wxWindowID id, int* gl_attrib)
  : wxGLCanvas(parent, id, gl_attrib, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE |
  wxCLIP_CHILDREN | wxCLIP_SIBLINGS)
{
  m_OGLcreated = false;
  m_back_col((cx_real)0.7, (cx_real)0.7, (cx_real)0.7);

  m_zoom = (cx_real)1.0;
  m_trans(0.0f, 0.0f);
  m_is_single_row = false;
  m_max_size = 150;
  m_display_all_people = false;

  mp_DisplayCallBack = NULL;
  mp_display_callback_object = NULL;

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
  HFONT font = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, L"Ariel Black");
  wglMakeCurrent(dc.GetHDC(), m_glRC->GetGLRC());
  HFONT	oldfont = (HFONT)SelectObject(dc.GetHDC(), font);
  wglUseFontBitmaps(dc.GetHDC(), 0, 256, 1000);
  SelectObject(dc.GetHDC(), oldfont);
  DeleteObject(font);
#endif

  Connect(ID_REFRESH_GRID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CX_ImageGrid::OnRefreshGrid);

}
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
CX_ImageGrid::~CX_ImageGrid()
{
  if (m_glRC)
  {
    SetCurrent(*m_glRC);
    delete m_glRC;
  }
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::SetDisplayCallBack(lpCallBackFunc p_func, void* p_object)
{
  mp_DisplayCallBack = p_func;
  mp_display_callback_object = p_object;
}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing on MSW
}
////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::OnPaint(wxPaintEvent& event)
{
  // This is a dummy, to avoid an endless succession of paint messages.
  // OnPaint handlers must always create a wxPaintDC.
  wxPaintDC dc(this);

  if (!IsShown()) return;

  Render();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::DisableUnusedOGL()
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
}
/////////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////////////
// Writes text at posn (x,y) origin bot left
void CX_ImageGrid::DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb)
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
cx_uint CX_FrameDisplay::TotalWidth()
{
  cx_uint total_width = 0;
  for (cx_uint i = 0; i<m_frames.size(); i++) total_width += m_frames[i].m_cols;
  return total_width;
}
/////////////////////////////////////////////////////////////////////////////
cx_uint CX_FrameDisplay::MaxHeight()
{
  cx_uint maxh = 0;
  for (cx_uint i = 0; i<m_frames.size(); i++) maxh = CX_MAX(maxh, m_frames[i].m_rows);
  return maxh;
}
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::Render()
{
  cx_uint i;
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);
  glColor3f(1.0f, 0.0f, 0.0f);

  
  cx_int max_size = m_max_size;
  cx_real aspect = (cx_real)m_win_size.c / (cx_real)m_win_size.r;
  cx_int cols = 1;
  cx_int rows = 1;

  cx_uint total_images = 0;
  for (i = 0; i<m_frames.size(); i++) total_images += m_frames[i].m_frames.size();

  if (total_images>0)
  {
    cols = m_frames.size(); // assuming single row (for now)
  }
  cx_int csize = cx_round((cx_real)m_win_size.c / (cx_real)(cols));
  cx_int rsize = cx_round((cx_real)m_win_size.r / (cx_real)rows);
  cx_int size = CX_MIN(csize, rsize);

  if (m_display_all_people)
  {
    if (size<max_size) max_size = size;
  }
  else
  {
    max_size = m_win_size.r;
  }

  cx_real border = (cx_real)(max_size)* (cx_real)0.025;

  cx_real x = 0.0f;
  //cx_real y = (cx_real)m_win_size.r - (cx_real)max_size;
  cx_real y = 0.0f;

  
  for (i = 0; i<m_frames.size(); i++)
  {
    CX_FrameDisplay& fdisp = m_frames[i];

    for (cx_uint k = 0; k<fdisp.m_frames.size(); k++)
    {
      CX_Image& im = fdisp.m_frames[k];
      CX_Vector3D& rgb = fdisp.m_rgbs[k];
      string& str = fdisp.m_strings[k];

      cx_real zoom = ((cx_real)max_size / (cx_real)im.m_rows);
      cx_real w = (cx_real)im.m_cols * zoom;
      
      glPixelStoref(GL_UNPACK_SKIP_PIXELS, 0.0f);
      glRasterPos2f(x, y);
      glPixelZoom(zoom, zoom);
      glDrawPixels(im.m_cols, im.m_rows, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)im.mp_pixels);

      // draw rect around image
      cx_real h = (cx_real)(max_size);
      glDisable(GL_TEXTURE_2D);
      glLineWidth(3.0f);
      glColor3f(rgb.x(), rgb.y(), rgb.z());
      //glColor3f(1.0f,0.0f,0.0f);
      glBegin(GL_LINE_LOOP);
      glVertex2f(x, y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
      glEnd();

      // Display string info, draw filled rectangle background (string will be draw on top of this
      h = (cx_real)15;
      glBegin(GL_POLYGON);
      glVertex2f(x, y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
      glEnd();
      glEnable(GL_TEXTURE_2D);

      CX_Vector3D trgb = CX_Vector3D(1, 1, 1) - rgb;
      DrawString(str, x + border, y + border, trgb);


      // move the horizontal start position
      x += w;
    }
    x += (cx_real)(max_size*0.1f);
  }
  

  glFlush();
  SwapBuffers();

#ifdef WIN32
  wglMakeCurrent(NULL, NULL);
#endif
}
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::InsertFrameSet(cx_uint index, vector<CX_Image*>& p_frames, vector<string>& strs, vector<CX_Vector3D>& rgbs)
{
  m_grid_lock.lock();
  if (index<m_frames.size())
  {
    CX_FrameDisplay& fdisp = m_frames[index];
    for (cx_uint k = 0; k<p_frames.size(); k++)
    {
      if (k<fdisp.m_frames.size())
      {
        fdisp.m_frames[k].Clone(*p_frames[k]);
        fdisp.m_rgbs[k] = rgbs[k];
        fdisp.m_strings[k] = strs[k];
      }
      else
      {
        fdisp.m_frames.push_back(CX_Image());
        fdisp.m_frames.back().Clone(*p_frames[k]);
        fdisp.m_rgbs.push_back(rgbs[k]);
        fdisp.m_strings.push_back(strs[k]);
      }
    }
    // remove any excess frames
    while (fdisp.m_frames.size() > p_frames.size())
    {
      fdisp.m_frames.pop_back();
      fdisp.m_rgbs.pop_back();
      fdisp.m_strings.pop_back();
    }
  }

  else
  {
    m_frames.push_back(CX_FrameDisplay());
    CX_FrameDisplay& fdisp = m_frames.back();
    for (cx_uint k = 0; k<p_frames.size(); k++)
    {
      fdisp.m_frames.push_back(CX_Image());
      fdisp.m_frames.back().Clone(*p_frames[k]);
      fdisp.m_rgbs.push_back(rgbs[k]);
      fdisp.m_strings.push_back(strs[k]);
    }

  }
  m_grid_lock.unlock();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::InsertFrameSet(cx_uint index, vector<cx_byte*>& p_pixels, vector<cx_uint>& rows, vector<cx_uint>& cols, vector<string>& strs, vector<CX_Vector3D>& rgbs)
{
  m_grid_lock.lock();
  cx_uint type = 1;
  cx_uint origin = 1;

  if (index<m_frames.size())
  {
    CX_FrameDisplay& fdisp = m_frames[index];
    for (cx_uint k = 0; k<p_pixels.size(); k++)
    {
      if (k<fdisp.m_frames.size())
      {
        fdisp.m_frames[k].Clone(p_pixels[k], rows[k], cols[k], type, origin);
        fdisp.m_rgbs[k] = rgbs[k];
        fdisp.m_strings[k] = strs[k];
      }
      else
      {
        fdisp.m_frames.push_back(CX_Image());
        fdisp.m_frames.back().Clone(p_pixels[k], rows[k], cols[k], type, origin);
        fdisp.m_rgbs.push_back(rgbs[k]);
        fdisp.m_strings.push_back(strs[k]);
      }
    }
    // remove any excess frames
    while (fdisp.m_frames.size() > p_pixels.size())
    {
      fdisp.m_frames.pop_back();
      fdisp.m_rgbs.pop_back();
      fdisp.m_strings.pop_back();
    }
  }

  else
  {
    m_frames.push_back(CX_FrameDisplay());
    CX_FrameDisplay& fdisp = m_frames.back();
    for (cx_uint k = 0; k<p_pixels.size(); k++)
    {
      fdisp.m_frames.push_back(CX_Image());
      fdisp.m_frames.back().Clone(p_pixels[k], rows[k], cols[k], type, origin);
      fdisp.m_rgbs.push_back(rgbs[k]);
      fdisp.m_strings.push_back(strs[k]);
    }

  }
  m_grid_lock.unlock();
}
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::Empty()
{
  m_frames.clear();
  Refresh();
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::Seek()
{
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_ImageGrid::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen()) return;

  cx_int cx = event.GetSize().GetWidth();
  cx_int cy = event.GetSize().GetHeight();
  if (0 >= cx || 0 >= cy)	return;

  m_zoom *= (cx_real)cy / (cx_real)m_win_size.r;
  m_trans *= (cx_real)cy / (cx_real)m_win_size.r;

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
void CX_ImageGrid::RefreshGrid()
{
  wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_REFRESH_GRID);
  this->AddPendingEvent(event);
  //this->ProcessEvent(event);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// this function reacts to the refresh message and checks the stomic do_seek 
// variable.
void CX_ImageGrid::OnRefreshGrid(wxCommandEvent& event)
{
  m_grid_lock.lock();
  Render();
  m_grid_lock.unlock();

}
/////////////////////////////////////////////////////////////////////////////
