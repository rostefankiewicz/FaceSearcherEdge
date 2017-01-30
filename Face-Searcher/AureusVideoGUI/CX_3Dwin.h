////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// wxWidgets & OpenGL stuff
// we need OpenGL headers for GLfloat/GLint types used below
/////////////////////////////////////////////////////////////////////////////
#include "CX_OGLdefs.h"
#include "wx/wx.h"
#include "wx/glcanvas.h"
/////////////////////////////////////////////////////////////////////////////



using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
class CX_3Dwin : public wxGLCanvas
{
  public:
    CX_3Dwin(wxWindow *parent, wxWindowID id = wxID_ANY, int *gl_attrib = NULL);
    virtual ~CX_3Dwin();

    void DisableUnusedOGL();
    void SetFrustum(cx_uint unW, cx_uint unH, cx_real rNear, cx_real rFar);
    void SetPerspective();
    void ApplyOGLViewPointTransformation();
    void GetAppliedMatrixRotation(const CX_Vector3D& kRotationAxis, const cx_real rRotationAngle, cx_real* p_rot_mat);
    void RotateViewPoint(const CX_Vector3D& kRotationAxis, cx_real rRotationAngle);
    void CalcRay(const cx_coord& kPoint2D);
    void MapToHemiSphere(cx_coord& point_2d, CX_Vector3D& point_3d);
    void SetOpenGLTextureValues();
    bool SelectOGLTextureImage();
    void SetMaterial(const CX_Vector3D& kColour, cx_real rAmbient);
    void DisplayPerspectiveGrid();
    void DisplayTextureMesh(CX_MESH3D* p_mesh);
    void DisplayWireframe(CX_MESH3D* p_mesh);
    void DisplayMesh(CX_MESH3D* p_mesh);
    void Seek();
    void Render();
    void Refresh3D();
    void SetMesh(CX_MESH3D* p_mesh);


    // OpenGL
    wxGLContext*   m_glRC;
    bool           m_OGLcreated;      // flag to determine whether OGL has been initialised

    cx_coord       m_win_size;
    cx_real        m_win_aspect;
    CX_Vector3D    m_back_col;
    CX_Vector3D    m_light_posn;
    cx_coord       m_mouse_posn;

    // 3D viewpoint variables
    CX_Vector3D m_trans;              // viewpoint position
    CX_Vector3D m_rot_cen;            // centre of rotation
    cx_real     m_rot_mat[16];        // viewpoint rotation matrix (4 x 4
    CX_Vector3D m_rot;                // additional rotation
    cx_real     m_FOV;
    cx_real     m_ray[6];             // the line (ray) from mouse pos through Z=0 plane

    // display control
    CX_Vector3D m_wire_colour;
    bool m_copy_texture;              // allows faster texture copying (must be correct size)
    bool m_texture_selected;          // flags whether to update the OGL texture
    bool m_display_wire;              // flags whether to display wire frame
    bool m_use_polygon_offset;

    CX_MESH3D* mp_mesh;
    bool m_mesh_loaded;
    bool m_image_loaded;
    bool m_allow_rotation;


    void OnPaint(wxPaintEvent& event);
    //void OnActivate(wxActivateEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event));
    void InitGL();


    wxDECLARE_EVENT_TABLE();

};
////////////////////////////////////////////////////////////////////////////////

