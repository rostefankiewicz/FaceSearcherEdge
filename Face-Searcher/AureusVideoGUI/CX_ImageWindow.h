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
#include <mutex>
#include <atomic>
/////////////////////////////////////////////////////////////////////////////


class CX_ImageGrid;

/////////////////////////////////////////////////////////////////////////////
class CX_ImageWindow : public wxGLCanvas
{
  public:
    CX_ImageWindow(wxWindow *parent, wxWindowID id = wxID_ANY, int *gl_attrib = NULL);
    virtual ~CX_ImageWindow();

    void DisableUnusedOGL();
    void Seek();
    void Render();
    CX_Vector2D GetNegTrans();

    void SetImage(CX_Image& im);
    // MUST be an RGBA image
    void SetImage(cx_byte* p_pixels, cx_uint rows, cx_uint cols);

    // utility display functions
    void DisplayNLine(CX_Vector2D& np1, CX_Vector2D& np2, const CX_Vector3D& rgb);
    void DisplayNPoint(CX_Vector2D& np, const CX_Vector3D& rgb);
    void DisplayEllipse(CX_Vector2D& center, CX_Vector2D& radii, const CX_Vector3D& rgb, cx_uint n_points = 10);
    void DisplayMeshOverlay(CX_MESH2D& mesh);
    void DisplayImageOverlay(CX_Vector2D& center, CX_Vector2D& radii, CX_IMAGE p_im);

    void DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb = CX_Vector3D(0.0f,0.0f,0.0f));

    // set a pointer to the display call back function using
    // SetDisplayCallBack() then you can call the DisplayUtility
    // functions from within you call back function
    typedef void(*lpCallBackFunc)(void* p_object);
    lpCallBackFunc mp_DisplayCallBack;
    void* mp_display_callback_object;
    void SetDisplayCallBack(lpCallBackFunc p_func, void* p_object);

    // a Left Up mouse button callback (n_mpos is the normalized location of m_pos)
    typedef void(*lpLeftUpCallBack)(void* p_object, CX_Vector2D& n_mpos, cx_coord mpos);
    lpLeftUpCallBack mp_LeftUpCallBack;
    void* mp_leftup_callback_object;
    void SetLeftUpCallBack(lpLeftUpCallBack p_func, void* p_object);

    // OpenGL
    wxGLContext*   m_glRC;
    bool           m_OGLcreated;      // flag to determine whether OGL has been initialised

    cx_coord       m_win_size;
    cx_real        m_win_aspect;
    CX_Vector3D    m_back_col;

    // mouse manipulation variables
    cx_mouse_operation m_mouse_op;
    cx_coord       m_mouse_posn;      // in client coords
    CX_Vector2D    m_npos;
    cx_real        m_zoom;
    CX_Vector2D    m_trans;
    bool           m_first_size;


    CX_Image m_frame;
    bool     m_frame_loaded;

    vector<string> m_text;
    CX_Vector3D    m_text_rgb;

    void PositionWindows();

    void OnPaint(wxPaintEvent& event);
    void OnActivate(wxActivateEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event));
    void InitGL();

    // allows frame refreshing from a different thread
    // to a GUI front end that is not thread safe (e.g. GTK)
    void SetImageRefreshPaint(CX_Image& im, vector<string>* p_texts = NULL);
    void SetImageRefreshPaint(cx_byte* p_pixels, cx_uint rows, cx_uint cols, vector<string>* p_texts = NULL);
    void RefreshPaint(vector<string>* p_texts = NULL);
    void OnRefreshFrame(wxCommandEvent& event);
    mutex        m_frame_lock;
    atomic<bool> m_do_seek;


    wxDECLARE_EVENT_TABLE();

};
/////////////////////////////////////////////////////////////////////////////




