////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "wx/event.h"
#include "wx/cmdline.h"
#include <wx/notebook.h>
////////////////////////////////////////////////////////////////////////////////
#include "AureusVideoGUI.h"
#include "CX_3Dwin.h"
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
wxBEGIN_EVENT_TABLE(CX_3Dwin, wxGLCanvas)
EVT_SIZE(CX_3Dwin::OnSize)
//EVT_ACTIVATE(CX_3Dwin::OnActivate)
EVT_PAINT(CX_3Dwin::OnPaint)
EVT_MOUSE_EVENTS(CX_3Dwin::OnMouseEvent)
EVT_ERASE_BACKGROUND(CX_3Dwin::OnEraseBackground)
wxEND_EVENT_TABLE()
//////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
CX_3Dwin::CX_3Dwin(wxWindow *parent, wxWindowID id, int *gl_attrib)
: wxGLCanvas(parent, id, gl_attrib, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE |
wxCLIP_CHILDREN | wxCLIP_SIBLINGS)
{
  m_OGLcreated = false;
  m_FOV = (cx_real)1;
  m_back_col((cx_real)0.9, (cx_real)0.9, (cx_real)0.9);
  m_wire_colour((cx_real)0.0, (cx_real)0.0, (cx_real)0.5);
  m_light_posn(0, 0, 1000);

  //m_mouse_op = CE_VIEWPOINT;
  m_texture_selected = false;
  m_copy_texture = false;
  m_display_wire = true;

  m_mesh_loaded = false;
  m_image_loaded = false;
  m_allow_rotation = true;

  m_rot(0, 0, 0);

  mp_mesh = NULL;

  m_use_polygon_offset = false;

  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);


  // viewpoint variables
  m_trans(0, 0, -300); // viewpoint default transformation values
  m_rot_cen(0, 0, 0);
  m_rot(0, 0, 0);
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)(&(m_rot_mat[0])));
  glPopMatrix();

  InitGL();
}
////////////////////////////////////////////////////////////////////////////////
CX_3Dwin::~CX_3Dwin()
{
  if (m_glRC)
  {
    SetCurrent(*m_glRC);
    delete m_glRC;
  }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::DisableUnusedOGL()
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
void CX_3Dwin::InitGL()
{
  // Make the new context current (activate it for use) with this canvas.
  SetCurrent(*m_glRC);

  m_FOV = (cx_real)1;

  wxRect r = wxWindow::GetClientRect();
  m_win_size.c = r.GetWidth();
  m_win_size.r = r.GetHeight();
  m_mouse_posn.c = m_win_size.c / 2;
  m_mouse_posn.r = m_win_size.r / 2;

  DisableUnusedOGL();
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);
  glClearDepth(1.0f);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  // viewpoint variables
  m_trans(0, 0, -300); // viewpoint default transformation values
  m_rot_cen(0, 0, 0);
  m_rot(0, 0, 0);
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)(&(m_rot_mat[0])));
  glPopMatrix();

}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::SetFrustum(cx_uint unW, cx_uint unH, cx_real rNear, cx_real rFar)
{
  SetCurrent(*m_glRC);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLdouble aspect_ratio = (GLdouble)unW / (GLdouble)unH;

  cx_real rTheta = CX_PI*(m_FOV / (cx_real)2) / (cx_real)180;

  GLdouble Ytop = (GLdouble)(rNear * tan(rTheta));
  GLdouble Ybottom = (GLdouble)(-Ytop);
  GLdouble Xright = aspect_ratio * Ytop;
  GLdouble Xleft = aspect_ratio * Ybottom;

  glFrustum(Xleft, Xright, Ybottom, Ytop, rNear, rFar);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::SetPerspective()
{
  if (!m_mesh_loaded || NULL == mp_mesh)
  {
    SetFrustum((cx_uint)m_win_size.c, (cx_uint)m_win_size.r, 1.0f, 2000.0f);
  }
  else
  {
    //m_mesh.SetFeatureExtents();
    CX_Vector3D extent(mp_mesh->m_extent_verts.x, mp_mesh->m_extent_verts.y, mp_mesh->m_extent_verts.z);
    CX_Vector3D minv(mp_mesh->m_min_verts.x, mp_mesh->m_min_verts.y, mp_mesh->m_min_verts.z);
    CX_Vector3D maxv(mp_mesh->m_max_verts.x, mp_mesh->m_max_verts.y, mp_mesh->m_max_verts.z);
    CX_Vector3D centre = (maxv + minv);
    centre /= (cx_real)2;
    cx_real range = extent.Mag();


    if (range <= 10) range = (cx_real)100;   // ensure a reasonable range if it's too small
    else  range *= (cx_real)2.5;             // increase range distance

    // since the OGL viewpoint is applied as camera_translation then rotation then negative rotation_centre_translation
    // this results in the camera to mesh distance always being equal to the magnitude of the camera_translation, obviously
    // this is a slight simplification with significant x & y translation, but it's a very good approximation
    cx_real val = m_trans.Mag();

    cx_real near_val = val - (range / 2.0f);
    if (near_val <= 0) near_val = (cx_real)1;
    cx_real far_val = near_val + range;
    SetFrustum((cx_uint)m_win_size.c, (cx_uint)m_win_size.r, near_val, far_val);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen()) return;

  m_win_size.c = event.GetSize().x;
  m_win_size.r = event.GetSize().y;
  m_mouse_posn.c = m_win_size.c / 2;
  m_mouse_posn.r = m_win_size.r / 2;

  m_win_aspect = (cx_real)m_win_size.c / (cx_real)m_win_size.r;

  if (0 >= m_win_size.c || 0 >= m_win_size.r)	return;

  SetCurrent(*m_glRC);
  glViewport(0, 0, m_win_size.c, m_win_size.r);

  // select the projection matrix and clear it
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  SetPerspective();

  //Render();
}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::Seek()
{
  if (!(mp_mesh && m_mesh_loaded)) return;

  // ensure a valid FOV
  if (m_FOV <= 0) m_FOV = (cx_real)1;

  // position camera at world centre
  m_trans(0, 0, 0);

  // reset rotation matrix to identity
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)(&(m_rot_mat[0])));
  glPopMatrix();

  // increase the Y extent slightly (reduces size in window slightly)
  cx_real ey = mp_mesh->m_extent_verts.y * 1.1f;

  // calculate the z distance from the camera required to centre the mesh in the viewpoint
  cx_real rExtraZ = ey / (2.0f * tan(cx_deg2rad(m_FOV / 2.0f)));
  m_trans.z() = -rExtraZ;

  // set the frustum 
  SetPerspective();
  //SetFrustum((cx_uint)m_win_size.c, (cx_uint)m_win_size.r, 1.0f, 2000.0f);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// applies the current 3D viewpoint
////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::ApplyOGLViewPointTransformation()
{
  glTranslatef((float)m_trans.x(), (float)m_trans.y(), (float)m_trans.z());

  glMultMatrixf((GLfloat *)m_rot_mat);

  glTranslatef((float)-m_rot_cen.x(), (float)-m_rot_cen.y(), (float)-m_rot_cen.z());
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::CalcRay(const cx_coord& kPoint2D)
{
  // get the current model view matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  ApplyOGLViewPointTransformation();  // apply current rotation & translation
  GLdouble kModel[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, kModel);     // get matrix
  glPopMatrix();

  GLdouble kProjection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, kProjection); // get current projection matrix

  GLint kViewport[4];
  glGetIntegerv(GL_VIEWPORT, kViewport);         // get current viewport

  GLdouble p1[3];
  GLdouble p2[3];

  // un-project a point with coords (kPoint2D.x,kPoint2D.y,0)
  gluUnProject((GLdouble)kPoint2D.c, (GLdouble)(kViewport[3] - kPoint2D.r), (GLdouble)0,
    kModel, kProjection, kViewport, &p1[0], &p1[1], &p1[2]);

  // un-project a point with coords (kPoint2D.x,kPoint2D.y,1)
  gluUnProject((GLdouble)kPoint2D.c, (GLdouble)(kViewport[3] - kPoint2D.r), (GLdouble)1,
    kModel, kProjection, kViewport, &p2[0], &p2[1], &p2[2]);


  // calc normal
  cx_real norm[3];
  norm[0] = (cx_real)(p2[0] - p1[0]);
  norm[1] = (cx_real)(p2[1] - p1[1]);
  norm[2] = (cx_real)(p2[2] - p1[2]);

  // force it to be a unit vector
  cx_real mag = sqrt((norm[0] * norm[0]) + (norm[1] * norm[1]) + (norm[2] * norm[2]));
  if (mag != 0.0f)
  {
    norm[0] /= mag;     norm[1] /= mag;     norm[2] /= mag;
  }


  m_ray[0] = (cx_real)p1[0]; m_ray[1] = (cx_real)p1[1]; m_ray[2] = (cx_real)p1[2];     // line point
  m_ray[3] = norm[0];        m_ray[4] = norm[1];        m_ray[5] = norm[2];   // line direction
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// projects the 2D point onto a hemi-sphere centered within the window
////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::MapToHemiSphere(cx_coord& point_2d, CX_Vector3D& point_3d)
{
  point_3d.x() = (cx_real)(2 * point_2d.c - m_win_size.c) / (cx_real)m_win_size.c;
  point_3d.y() = (cx_real)(m_win_size.r - 2 * point_2d.r) / (cx_real)m_win_size.r;
  point_3d.z() = 0.0f;
  cx_real m = point_3d.Mag();
  if (m > 1.0f) m = 1.0f;
  point_3d.z() = cos((CX_PI/2)*m);

  // force it to be a unit vector
  m = point_3d.Mag();
  if (m != 0)
  {
    point_3d /= m;
  }
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::SetOpenGLTextureValues()
{
  //GLint  m_TextureFunction = GL_DECAL;
  //GLint  m_TextureFunction = GL_BLEND;
  //GLint  m_TextureFunction = GL_REPLACE;
  GLint  m_TextureFunction = GL_MODULATE;

  ::glEnable(GL_TEXTURE_2D);
  ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_TextureFunction);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
bool CX_3Dwin::SelectOGLTextureImage()
{
  if (!(mp_mesh && m_mesh_loaded)) return false;

  // ensure we have a legitamate texture map
  if (mp_mesh->m_cols == 0 || mp_mesh->m_rows == 0) return false;

  // set up the texture mapping properties
  GLint  TextureFunction = GL_DECAL;
  //GLint  TextureFunction = GL_BLEND;
  //GLint  TextureFunction = GL_REPLACE;
  //GLint  TextureFunction = GL_MODULATE;
  GLfloat bcol[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  // enable texture mapping and control the wrapping and filtering properties
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bcol);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, TextureFunction);

  // Generate a series of texture maps of decreasing size
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, mp_mesh->m_cols, mp_mesh->m_rows, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(mp_mesh->m_pixels));

  return true;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::SetMaterial(const CX_Vector3D& kColour, cx_real rAmbient)
{
  cx_real shiny = (cx_real)0.15;
  cx_real spec = (cx_real)0.0;

  CX_Vector3D kAmb = kColour; kAmb *= rAmbient;
  CX_Vector3D kDiff = kColour; kDiff *= rAmbient;
  GLfloat Ambient[] = { kAmb.x(), kAmb.y(), kAmb.z(), 1.0f };
  GLfloat Diffuse[] = { kDiff.x(), kDiff.y(), kDiff.z(), 1.0f };
  GLfloat Shiny[] = { shiny*1.0f };
  GLfloat Specular[] = { spec, spec, spec, 1.0f };

  GLfloat LightModel[] = { 1.0 }; // 
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, LightModel);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shiny);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::DisplayWireframe(CX_MESH3D* p_mesh)
{
  CX_Vector3D n(0, 0, 1);


  glLineWidth((GLfloat)1.0);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glLineWidth((GLfloat)1);
  glColor3f(m_wire_colour.x(), m_wire_colour.y(), m_wire_colour.z());

  for (cx_int i = 0; i<p_mesh->m_num_polys; i++)
  {
    CX_POLY3D& poly = p_mesh->m_polys[i];
    glBegin(GL_LINE_LOOP);
    for (cx_int k = 0; k<poly.m_elems; k++)
    {
      CX_POLYVERTEX& pv = poly.m_indices[k];
      CX_VECTOR3D& v = p_mesh->m_verts[pv.v];
      glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::DisplayPerspectiveGrid()
{
  glPushMatrix();
  ApplyOGLViewPointTransformation();
  cx_real len = 1000;
  cx_real rStep = 20;
  SetMaterial(CX_Vector3D((cx_real)1, (cx_real)1, (cx_real)1), (cx_real)0.25);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor4d(0, 0, 0, 1.0);
  for (cx_real rZ = -len; rZ <= len; rZ += rStep)
  {
    glVertex3d(-len, 0.0, rZ);
    glVertex3d(len, 0.0, rZ);
  }
  for (cx_real rX = -len; rX <= len; rX += rStep)
  {
    glVertex3d(rX, 0.0, -len);
    glVertex3d(rX, 0.0, len);
  }
  glEnd();
  glEnable(GL_LIGHTING);
  glPopMatrix();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::DisplayTextureMesh(CX_MESH3D* p_mesh)
{
  if (!p_mesh) return;

  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glDisable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);


  // select the texture into OGL if required
  if (!m_texture_selected)
  {
     m_texture_selected = SelectOGLTextureImage();
  }

  for (cx_int i = 0; i<p_mesh->m_num_polys; i++)
  {
    CX_POLY3D& poly = p_mesh->m_polys[i];
    glBegin(GL_POLYGON);
    for (cx_int k = 0; k<poly.m_elems; k++)
    {
      CX_POLYVERTEX& pv = poly.m_indices[k];
      CX_VECTOR3D& v = p_mesh->m_verts[pv.v];
      CX_VECTOR3D& n = p_mesh->m_norms[pv.n];
      CX_VECTOR2D& t = p_mesh->m_tex[pv.t];

      glNormal3f(n.x, n.y, n.z);
      glTexCoord2f(t.x, t.y);
      glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
  }

}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::DisplayMesh(CX_MESH3D* p_mesh)
{
  glDisable(GL_CULL_FACE);

  // display wireframe first (as it includes a polygon offset)
  if (m_display_wire) DisplayWireframe(p_mesh);

  // display the textured mesh
  DisplayTextureMesh(p_mesh);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::Render()
{
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);
  glClearColor(m_back_col.x(), m_back_col.y(), m_back_col.z(), 0.0f);

  GLfloat LightPosition[] = { m_light_posn.x(), m_light_posn.y(), m_light_posn.z(), 0.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_display_wire)
  {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset((GLfloat)5.0, (GLfloat)5.0);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //DisplayPerspectiveGrid();

  if (NULL != mp_mesh)
  {
    glPushMatrix();
    ApplyOGLViewPointTransformation();

    if (m_mesh_loaded)
    {
      SetMaterial(CX_Vector3D(1, 1, 1), (cx_real)0.75);
      DisplayMesh(mp_mesh);
    }
    glPopMatrix();
  }

  if (m_display_wire) glDisable(GL_POLYGON_OFFSET_FILL);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::Refresh3D()
{
  if (!IsShownOnScreen()) return;

  Render();

  glFlush();
  SwapBuffers();
  Update();
}
////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::OnPaint(wxPaintEvent& event)
{
  // This is a dummy, to avoid an endless succession of paint messages.
  // OnPaint handlers must always create a wxPaintDC.
  wxPaintDC dc(this);

  Refresh3D();
}
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::GetAppliedMatrixRotation(const CX_Vector3D& kRotationAxis, const cx_real rRotationAngle, cx_real* p_rot_mat)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Rotates by the angles passed
  glRotatef(rRotationAngle, kRotationAxis.x(), kRotationAxis.y(), kRotationAxis.z());

  // Then applies the current rotation
  glMultMatrixf((GLfloat *)(&(m_rot_mat[0])));

  // Passes back the new rotation matrix
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)(&p_rot_mat[0]));
  glPopMatrix();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::RotateViewPoint(const CX_Vector3D& kRotationAxis, cx_real rRotationAngle)
{
  // Get the rotation matrix after applying to the current scene
  cx_real  rot_mat[16];
  GetAppliedMatrixRotation(kRotationAxis, rRotationAngle, rot_mat);

  // Set the scene rotation matrix
  memcpy(&m_rot_mat[0], &rot_mat[0], 16 * sizeof(cx_real));
}
////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::OnMouseEvent(wxMouseEvent& event)
{
  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  cx_coord point(event.GetY(), event.GetX());

  // 3D axis & angle (using the window as a hemisphere)
  CX_Vector3D last_posn, this_posn;
  MapToHemiSphere(m_mouse_posn, last_posn);
  MapToHemiSphere(point, this_posn);
  CX_Vector3D axis = Cross3D(last_posn, this_posn);
  cx_real angle = (cx_real)90 * Distance(this_posn, last_posn);

  // 2D client coords
  cx_real xrel = (cx_real)(m_mouse_posn.c - point.c);
  cx_real yrel = (cx_real)(m_mouse_posn.r - point.r);
  m_mouse_posn = point;

  bool bLeftPressed = event.LeftIsDown();
  bool bRightPressed = event.RightIsDown();
  bool bBothPressed = bLeftPressed && bRightPressed;
  bool bControlPressed = event.ControlDown();
  bool bShiftPressed = event.ShiftDown();

  bool update_display = false;

  if (event.LeftDClick()) Seek();
  if (event.RightDClick())
  {
    m_display_wire = !m_display_wire;
    update_display = true;
  }


  //---------- move viewpoint mouse operation----------------------------
  if (bBothPressed == true) // viewpoint zoom
  {
    cx_real rVal = (cx_real)0.0025 * Distance(m_rot_cen, m_trans);
    m_trans.z() -= yrel*rVal;
    SetPerspective(); // since the frustum might need changing
    update_display = true;
  }
  else if (bRightPressed == true) // viewpoint translation
  {
    cx_real rVal = (cx_real)0.00005 * Distance(m_rot_cen, m_trans);
    m_trans.x() += -xrel*rVal;
    m_trans.y() += yrel*rVal;
    update_display = true;
  }
  else if (bLeftPressed == true) // viewpoint rotation
  {
    RotateViewPoint(axis, angle);
    update_display = true;
  }

  if (update_display) Refresh3D();

}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CX_3Dwin::SetMesh(CX_MESH3D* p_mesh)
{
  mp_mesh = p_mesh;
  m_mesh_loaded = true;
  m_texture_selected = false;
  m_image_loaded = false;
  m_allow_rotation = true;

  Seek();
  Refresh3D();
}
////////////////////////////////////////////////////////////////////////////////
