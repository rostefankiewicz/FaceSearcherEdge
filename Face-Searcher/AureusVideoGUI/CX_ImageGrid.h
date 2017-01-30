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
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CX_FrameDisplay
{
  public:
    vector<CX_Image>        m_frames;
    vector<CX_Vector3D>     m_rgbs;
    vector<string>          m_strings;

    cx_uint TotalWidth();
    cx_uint MaxHeight();

    CX_FrameDisplay(){};
    ~CX_FrameDisplay(){};
};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CX_ImageGrid : public wxGLCanvas
{
  public:
    CX_ImageGrid(wxWindow *parent, wxWindowID id = wxID_ANY, int *gl_attrib = NULL);
    virtual ~CX_ImageGrid();

    void DisableUnusedOGL();
    void Seek();
    void Render();

    void DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb = CX_Vector3D(0.0f, 0.0f, 0.0f));

    // set a pointer to the display call back function using
    // SetDisplayCallBack() then you can call the DisplayUtility
    // functions from within you call back function
    typedef void(*lpCallBackFunc)(void* p_object);
    lpCallBackFunc mp_DisplayCallBack;
    void* mp_display_callback_object;
    void SetDisplayCallBack(lpCallBackFunc p_func, void* p_object);

    void InsertFrameSet(cx_uint index, vector<CX_Image*>& p_frames, vector<string>& strs, vector<CX_Vector3D>& rgbs);
    void InsertFrameSet(cx_uint index, vector<cx_byte*>& p_pixels, vector<cx_uint>& rows, vector<cx_uint>& cols, vector<string>& strs, vector<CX_Vector3D>& rgbs);
    void Empty();

    // OpenGL
    wxGLContext*   m_glRC;
    bool           m_OGLcreated;      // flag to determine whether OGL has been initialised

    // general window variables 
    cx_coord       m_win_size;
    cx_real        m_win_aspect;
    CX_Vector3D    m_back_col;

    cx_real        m_zoom;
    CX_Vector2D    m_trans;

    bool           m_first_size;

    bool                     m_is_single_row;
    vector<CX_FrameDisplay>  m_frames;
    cx_int                   m_max_size;
    bool                     m_display_all_people;


    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event));
    void InitGL();

    // allows window refreshing from a different thread
    // to a GUI front end that is not thread safe (e.g. GTK)
    void OnRefreshGrid(wxCommandEvent& event);
    void RefreshGrid();
    mutex m_grid_lock;

    wxDECLARE_EVENT_TABLE();

};
/////////////////////////////////////////////////////////////////////////////

