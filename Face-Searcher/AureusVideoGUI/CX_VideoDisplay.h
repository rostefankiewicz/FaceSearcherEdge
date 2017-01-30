#pragma once
///////////////////////////////////////////////////////////////////
#include "CX_ImageWindow.h"
///////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Aureus includes
#include "Aureus.h"
#include "AureusVideo.h"
#include "AureusHeads.h"
/////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////
#include "cxutils.h"
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
#include <atomic>
/////////////////////////////////////////////////////////////////////////////


class MyFrame;
class CX_ImageGrid;

///////////////////////////////////////////////////////////////////
class CX_VideoDisplay : public CX_ImageWindow
{
  public:
    CX_VideoDisplay(wxWindow *parent, wxWindowID id = wxID_ANY, int *gl_attrib = NULL);
    ~CX_VideoDisplay();

    // stream opening functions
    bool PlayUSBCamera(bool display_errors=true);
    bool PlayIPCamera(bool display_errors = true);
    bool PlayMediaFile(bool display_errors = true);
    void Pause();
    void Stop();

    void UpdateTimerInfo(); // calcs FPS etc
    void AddGridResults(CX_HEAD_LIST p_head_list, cx_uint n_heads);
    void SetDetectionParams();
    void AddImage(CX_IMAGE p_im, vector<cx_byte*>& frame_pixels, vector<cx_uint>& rows_list, vector<cx_uint>& cols_list, vector<CX_Vector3D>& rgbs, vector<string>& strings, const CX_Vector3D& rgb, const string& str);
    void AddRankImage(CX_RankItem& r, vector<cx_byte*>& frame_pixels,
      vector<cx_uint>& rows_list, vector<cx_uint>& cols_list,
      vector<CX_Vector3D>& rgbs,
      vector<string>& strings, vector<CX_IMAGE>& images_to_free);


    void OverlayHeads(cx_byte* p_pixels, cx_uint rows, cx_uint cols, CX_HEAD_LIST p_head_list, cx_uint head_list_size);

    // DEV func for displaying...without displaying..
    void UpdateDisplayDev();

    // Frame processing direct from video interface
    static void FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, void* p_object);
    static void UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object);
    static void MediaFileFinishedCallBack(cx_uint frame_num, void* p_object);
    static void DisplayCallBack(void* p_object);
    static void LeftUpCallBack(void* p_object, CX_Vector2D& n_mpos, cx_coord mpos);
    void FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num);
    void Display();
    void LeftUp(CX_Vector2D& n_mpos, cx_coord mpos);

    bool InitializeAureusVideo(string& emsg);

    MyFrame*          mp_mainframe;
    CX_VIDEO          mp_aureus_video;
    CX_HEAD_LIST      mp_head_list;

    CX_DetectionParams m_detection_params;

    bool              m_is_paused;
    bool              m_is_playing;
    std::atomic<bool>  m_is_enrolling;

    cx_int            m_current_frame_num;
    cx_uint           m_frame_count;
    cx_real           m_fps;
    cx_real           m_pfps;
    cx_uint           m_frame_rows, m_frame_cols;
    clock_t           m_start_time;


    cx_int            m_stream_type; // -1 = not set, 0=media file, 1=USB cam, 2=IP cam
    string            m_media_fname;
    cx_int            m_usb_pin;
    string            m_ip_url;

    CX_ImageGrid*     mp_grid;

    cx_uint           m_num_people;

    bool              m_is_first_frame;

    bool              m_display_only_verified;
    cx_int            m_display_rank_n;
    bool              m_display_people_thumbs;
    bool              m_display_current_head_image;
    bool              m_display_pc_head_image;
    bool              m_display_pc_overlays;

    bool              m_display_fit_measure;
    bool              m_loop_media_files;


};
///////////////////////////////////////////////////////////////////

