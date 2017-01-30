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




/////////////////////////////////////////////////////////////////////////////
class CX_ImageItem
{
  public:
    CX_Image                 m_frame;
    vector<string>           m_strings;
    CX_Vector3D              m_rgb;
    cx_real                  m_x_left, m_x_right;
    bool                     m_is_selected;

    CX_ImageItem(){ m_x_left = 0.0f; m_x_right = 0.0f; m_is_selected = false; };
    ~CX_ImageItem(){};
};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CX_ImageList : public wxGLCanvas
{
  public:
    CX_ImageList(wxWindow *parent);
    virtual ~CX_ImageList();


    void DisableUnusedOGL();
    void Seek();
    void Render();
    void Empty();

    void DrawTextBackGround(vector<string>& text, cx_real x, cx_real y, const CX_Vector3D& rgb);
    void DrawStringWithBackground(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb = CX_Vector3D(0.0f, 0.0f, 0.0f));
    void DrawString(const string& s, cx_real x, cx_real y, const CX_Vector3D& rgb = CX_Vector3D(0.0f, 0.0f, 0.0f));


    typedef void(*lpCallBackFunc)(void* p_object, cx_int sel, cx_coord multi_sel);
    lpCallBackFunc mp_SelectionCallBack;
    void* mp_selection_callback_object;
    void SetSelectionCallBack(lpCallBackFunc p_func, void* p_object);


    typedef void(*lpPopUpCallBackFunc)(void* p_object, cx_int sel, cx_coord multi_sel, cx_uint pop_up_string_index);
    lpPopUpCallBackFunc mp_SelectionPopUpCallBack;
    void* mp_selection_popup_callback_object;
    void SetSelectionPopUpCallBack(lpPopUpCallBackFunc p_func, void* p_object);

    void ShowContextMenu(const cx_coord& pos, cx_int sel, cx_coord multi_sel);

    // OpenGL
    wxGLContext*   m_glRC;
    bool           m_OGLcreated;      // flag to determine whether OGL has been initialised

    // general window variables 
    cx_coord       m_win_size;
    cx_real        m_win_aspect;
    CX_Vector3D    m_back_col;

    cx_int                 m_sel1, m_sel2; // for multi-selection
    cx_coord               m_sel_range;
    vector<CX_ImageItem>   m_frames;

    vector<string>   m_popup_menu_strings;

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event));
    void OnMouseEvent(wxMouseEvent& event);
    void InitGL();

    wxDECLARE_EVENT_TABLE();

};
/////////////////////////////////////////////////////////////////////////////

