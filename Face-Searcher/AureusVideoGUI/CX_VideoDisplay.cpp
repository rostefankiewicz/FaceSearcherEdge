
///////////////////////////////////////////////////////////////////
// include main wxWidgets
///////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include <wx/notebook.h>
///////////////////////////////////////////////////////////////////
#include "CX_ImageGrid.h"
#include "CX_VideoDisplay.h"
#include "AureusVideoGUI.h"
#include "CX_GalleryPanel.h"
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
CX_VideoDisplay::CX_VideoDisplay(wxWindow *parent, wxWindowID id, int *gl_attrib)
  : CX_ImageWindow(parent, id, gl_attrib)
{
  mp_mainframe = NULL;
  mp_aureus_video = NULL;
  mp_head_list = NULL;
  mp_grid = NULL;
  m_is_paused = false;
  m_is_first_frame = true;
  m_is_playing = false;
  m_is_enrolling = false;

  m_usb_pin = 0;
  //m_ip_url = "http://131.111.125.248/axis-cgi/mjpg/video.cgi?fps=25.mjpeg";
  //m_ip_url = "http://166.142.23.54/mjpg/video.mjpg?.mjpeg";
  m_ip_url = "http://199.249.109.58/mjpg/video.mjpg?.mjpeg";


  m_display_rank_n = 2;
  m_display_people_thumbs = true;
  m_display_only_verified = true;
  m_display_current_head_image = true;
  m_display_pc_head_image = false;
  m_display_pc_overlays = false;
  m_display_fit_measure = false;

  m_stream_type = 0; // media file
  m_current_frame_num = 0;
  m_frame_count = 0;
  m_fps = 0.0f;
  m_pfps = 0.0f;

  m_text.push_back("Frame Num: 0 Count: 0 Size(0,0) Duration: 0.0s");
  m_text.push_back("FPS: 0.0");
  m_text.push_back("PFPS: 0.0");
  m_text.push_back("People: 0");


  m_detection_params.m_top = 0.0f;
  m_detection_params.m_left = 0.0f;
  m_detection_params.m_height = 1.0f;
  m_detection_params.m_width = 1.0f;
  m_detection_params.m_min_height_prop = 0.2f;
  m_detection_params.m_max_height_prop = 0.6f;

  // allows display of face detection/anno models etc.
  SetDisplayCallBack(DisplayCallBack, this);

  // captures left up mouse button event
  SetLeftUpCallBack(LeftUpCallBack, this);

  m_loop_media_files = false;
}
///////////////////////////////////////////////////////////////////
CX_VideoDisplay::~CX_VideoDisplay()
{
  // no need to stop the video as freein Aureus will
  // automatically stop all streams and free RAM
  //Stop();
}
///////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////
bool CX_VideoDisplay::InitializeAureusVideo(string& emsg)
{
  char msg[1024];
  mp_aureus_video = CX_CreateVideo(mp_mainframe->mp_aureus, msg);
  if (!mp_aureus_video)
  {
    emsg = msg;
    return false;
  }


  if (0 == CX_SetFrameCallBack(mp_aureus_video, FrameCallBack, this, msg))
  {
    emsg = msg;
    return false;
  }

  if (0 == CX_SetStreamTerminatedCallBack(mp_aureus_video, UnexpectedTerminationCallBack, this, msg))
  {
    emsg = msg;
    return false;
  }

  if (0 == CX_SetMediaFileFinishedCallBack(mp_aureus_video, MediaFileFinishedCallBack, this, msg))
  {
    emsg = msg;
    return false;
  }


  cx_real params[6];
  if (1 == CX_GetDetectionParameters(mp_aureus_video, params, msg))
  {
    m_detection_params.m_top = params[0];
    m_detection_params.m_left = params[1];
    m_detection_params.m_height = params[2];
    m_detection_params.m_width = params[3];
    m_detection_params.m_min_height_prop = params[4];
    m_detection_params.m_max_height_prop = params[5];
  }


  return true;
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::SetDetectionParams()
{
  if (!mp_aureus_video) return;
  char msg[1024];
  if (0 == CX_SetDetectionParameters(mp_aureus_video, m_detection_params.m_top, m_detection_params.m_left,
    m_detection_params.m_height, m_detection_params.m_width, m_detection_params.m_min_height_prop, m_detection_params.m_max_height_prop, msg))
  {
    wxMessageBox(msg);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// callback functions: just pass them on to a non static member func
// so that we can get at the data more elegantly
///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, void* p_object)
{
  if (p_object) ((CX_VideoDisplay*)p_object)->FrameCallBack(p_head_list, head_list_size, p_pixels, rows, cols, frame_num);
}
///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object)
{
  string msg("Oh heck the stream has terminated:\n");
  wxMessageBox(msg.c_str());
}
///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::DisplayCallBack(void* p_object)
{
  if (p_object) ((CX_VideoDisplay*)p_object)->Display();
}
///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::LeftUpCallBack(void* p_object, CX_Vector2D& n_mpos, cx_coord mpos)
{
  if (p_object) ((CX_VideoDisplay*)p_object)->LeftUp(n_mpos, mpos);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::MediaFileFinishedCallBack(cx_uint frame_num, void* p_object)
{
  if (p_object)
  {
    CX_VideoDisplay* p_vid = (CX_VideoDisplay*)p_object;
    if (p_vid->m_stream_type == 0 && p_vid->m_loop_media_files)
    {
      p_vid->PlayMediaFile();
    }
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// stream opening functions
///////////////////////////////////////////////////////////////////
bool CX_VideoDisplay::PlayUSBCamera(bool display_errors)
{
  if (!mp_aureus_video) return false;
  char msg[1024];

  if (m_is_paused)
  {
    (void)CX_UnPauseStream(mp_aureus_video, msg);
    m_is_paused = false;
    return true;
  }

  Stop();
  m_current_frame_num = 0;
  m_frame_count = 0;
  m_is_first_frame = true;

  string con_info; con_info+=m_usb_pin;
  if (0 == CX_ProcessStream(mp_aureus_video, 1, con_info.c_str(), msg))
  {
    if (display_errors)
    {
      string emsg = "Failed to open USB at pin " + m_usb_pin;
      emsg += string("\nSDK error: ") + msg;
      wxMessageBox(emsg.c_str());
    }
    return false;
  }
  m_start_time = clock();
  m_stream_type = 1;
  m_is_playing = true;
  return true;
}
///////////////////////////////////////////////////////////////////
bool CX_VideoDisplay::PlayIPCamera(bool display_errors)
{
  if (!mp_aureus_video) return false;
  char msg[1024];

  if (m_is_paused)
  {
    (void)CX_UnPauseStream(mp_aureus_video, msg);
    m_is_paused = false;
    return true;
  }

  Stop();
  m_current_frame_num = 0;
  m_frame_count = 0;
  m_is_first_frame = true;

  // DEV test
  //m_ip_url = "http://131.111.125.248/axis-cgi/mjpg/video.cgi?fps=25";

  // not using frame interval here as it doesn't effect the speed, merely the number of frames
  // processed, i.e. the camera runs at it's own speed

  if (0 == CX_ProcessStream(mp_aureus_video, 2, m_ip_url.c_str(), msg))
  {
    if (display_errors)
    {
      string emsg = string("Failed to open IP camera at url:\n") + string(m_ip_url) + string("\nSDK error: ") + msg;
      wxMessageBox(emsg.c_str());
    }
    return false;
  }

  m_start_time = clock();
  m_stream_type = 2;
  m_is_playing = true;
  return true;
}
///////////////////////////////////////////////////////////////////
bool CX_VideoDisplay::PlayMediaFile(bool display_errors)
{
  if (!mp_aureus_video) return false;
  char msg[1024];

  if (m_is_paused)
  {
    (void)CX_UnPauseStream(mp_aureus_video, msg);
    m_is_paused = false;
    return true;
  }

  Stop(); 
  m_current_frame_num = 0;
  m_frame_count = 0;
  m_is_first_frame = true;

  if (0 == CX_ProcessStream(mp_aureus_video, 0, m_media_fname.c_str(), msg))
  {
    if (display_errors)
    {
      string emsg = string("Failed to open media file:\n") + string(m_media_fname) + string("\nSDK error: ") + msg;
      wxMessageBox(emsg.c_str());
    }
    return false;
  }

  m_start_time = clock();
  m_stream_type = 0;
  m_is_playing = true;
  return true;
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::Pause()
{
  if (!mp_aureus_video) return;
  if (m_is_paused) return;

  char msg[1024];
  if (0 == CX_PauseStream(mp_aureus_video, msg))
  {
    wxMessageBox(msg);
  }
  m_is_paused = true;
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::Stop()
{
  if (!mp_aureus_video) return;
  char msg[1024];
  
  // save state of loop flag, then the end of media callback won't loop
  bool loop_temp = m_loop_media_files;
  m_loop_media_files = false;

  if (0 == CX_StopStream(mp_aureus_video, msg))
  {
    wxMessageBox(msg);
  }


  m_is_paused = false;
  m_is_playing = false;
  m_is_enrolling = false;

  // restore loop flag
  m_loop_media_files = loop_temp;

}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::Display()
{
  if (!mp_aureus_video || !mp_head_list) return;

  char msg[1024];
  cx_int n = CX_GetHeadListSize(mp_head_list, msg);
  if (n < 0) return;
  
  cx_uint n_points = 20;
  CX_Vector2D cen, rad;
  CX_Vector3D rgb(0.0f, 1.0f, 0.0f);

  for (cx_int i = 0; i < n; i++)
  {
    CX_HEAD p_head = CX_GetHead(mp_head_list,i,msg);

    if (p_head)
    {
      CX_HeadInfo hd;
      if (0 != CX_GetHeadInfo(p_head, &hd, msg))
      {
        if (hd.m_valid)
        //if (hd.m_frame_number == m_current_frame_num && hd.m_head_ok)
        {
          cen((hd.m_head_bl_x + hd.m_head_tr_x)*0.5f, (hd.m_head_bl_y + hd.m_head_tr_y)*0.5f);
          rad(hd.m_head_tr_x - hd.m_head_bl_x, hd.m_head_tr_y - hd.m_head_bl_y);

          CX_MESH2D* p_mesh = CX_GetMeshOverlay(p_head, msg);
          if (p_mesh)
          {
            DisplayMeshOverlay(*p_mesh);
          }

          // displaying pose corrected image overlays?
          if (m_display_pc_overlays)
          {
            // displaying current images?
            if (m_display_current_head_image)
            {
              CX_IMAGE p_im = CX_GetPCimage(p_head, msg);
              if (p_im) DisplayImageOverlay(cen, rad, p_im);
            }
            // else get best ranked PC image
            else
            {
              CX_HEAD_DATA_LIST p_hdl = CX_GetRankedHeadDataList(p_head, msg);
              if (p_hdl)
              {
                CX_HEAD_DATA p_hd = CX_GetHeadData(p_hdl, 0, msg);
                if (p_hd)
                {
                  CX_IMAGE p_im = CX_HeadDataPCimage(p_hd, msg);
                  if (p_im) DisplayImageOverlay(cen, rad, p_im);
                }
              }
            }
          }

          ////////////////////////////////////////////////
          // We could also get the 3D mesh here if required (or prefered)
          //if (m_display_current_head_image)
          //{
          //  CX_MESH3D* p_mesh3d = CX_Get3DMesh(p_head, msg); // current frame
          //}
          //else
          //{
          //  CX_HEAD_DATA_LIST p_hdl = CX_GetRankedHeadDataList(p_head, msg);
          //  if (p_hdl)
          //  {
          //    CX_HEAD_DATA p_hd = CX_GetHeadData(p_hdl, 0, msg);
          //    if (p_hd)
          //    {
          //      CX_MESH3D* p_mesh3d = CX_HeadData3DMesh(p_hd, msg); // best ranked head image 3D mesh
          //      if (p_mesh3d)
          //      {
          //        // do something with p_mesh3d
          //      }
          //    }
          //  }
          //}
          ////////////////////////////////////////////////

          // display the detected head ellipse last so it overlays all before it
          DisplayEllipse(cen, rad, rgb, n_points);
        }
      }
    }
  }
  
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::UpdateTimerInfo()
{
  cx_real seconds = (cx_real)(clock() - m_start_time) / CLOCKS_PER_SEC;
  if (seconds>0.0f)  m_fps = (cx_real)(m_current_frame_num) / seconds;

  char buf[256];

  sprintf(buf, "Frame Num: %d Count: %d Size(%d,%d) Duration: %1.3fs", m_current_frame_num, m_frame_count, m_frame_rows, m_frame_cols, seconds);
  m_text[0] = string(buf);

  sprintf(buf, "FPS: %1.3f", m_fps);
  m_text[1] = string(buf);

  sprintf(buf, "PFPS: %1.3f", m_pfps);
  m_text[2] = string(buf);

  sprintf(buf, "People: %d", m_num_people);
  m_text[3] = string(buf);
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::AddImage(CX_IMAGE p_im, vector<cx_byte*>& frame_pixels,
  vector<cx_uint>& rows_list, vector<cx_uint>& cols_list,
  vector<CX_Vector3D>& rgbs,
  vector<string>& strings, const CX_Vector3D& rgb, const string& str)
{
  if (!p_im) return;

  char msg[1024];
  cx_uint rows, cols;
  cx_byte*  p_pixels = CX_ImageData(p_im, &rows, &cols, msg);
  if (p_pixels)
  {
    frame_pixels.push_back(p_pixels);
    rows_list.push_back(rows);
    cols_list.push_back(cols);
    rgbs.push_back(rgb);
    strings.push_back(str);
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::AddRankImage(CX_RankItem& r, vector<cx_byte*>& frame_pixels,
  vector<cx_uint>& rows_list, vector<cx_uint>& cols_list,
  vector<CX_Vector3D>& rgbs,
  vector<string>& strings, vector<CX_IMAGE>& images_to_free)
{
  char msg[1024];
  cx_real vthresh = CX_GetVerificationThreshold(mp_aureus_video, msg);
  if (vthresh >= 0.0f)
  {
    if (m_display_only_verified && r.m_score < vthresh) return;
  }

  char buf[100];
  sprintf(buf, " %1.0f(%s)", 100.0f*r.m_score, r.m_person_name);
  string str(buf);

  CX_Vector3D rgb(1.0f, 1.0f, 1.0f);
  if (vthresh>=0.0f && r.m_score >= vthresh) rgb(0.0f, 1.0f, 0.0f);


  CX_IMAGE p_rim = NULL;
  // displaying the person thumbnails?
  if (m_display_people_thumbs) p_rim = CX_GetPersonThumbnail(mp_mainframe->mp_aureus, r.m_person_id, msg);
  // displaying the image thumbnails
  else p_rim = CX_GetImageThumbnail(mp_mainframe->mp_aureus, r.m_image_id, msg);


  if (p_rim)
  {
    images_to_free.push_back(p_rim);
    AddImage(p_rim, frame_pixels, rows_list, cols_list, rgbs, strings, rgb, str);
  }
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::AddGridResults(CX_HEAD_LIST p_head_list, cx_uint n_heads)
{
  
  if (!mp_grid) return; // no grid, no point
  if (!mp_mainframe) return;

  if (!mp_aureus_video || !p_head_list) return;

  char msg[1024];

  cx_uint frame_index = 0;

  CX_Vector3D white(1.0f, 1.0f, 1.0f);
  CX_Vector3D green(0.0f, 1.0f, 0.0f);
  CX_Vector3D red(1.0f, 0.0f, 0.0f);
  CX_Vector3D blue(0.0f, 0.0f, 1.0f);

  vector<CX_IMAGE> images_to_free;

  for (cx_uint i = 0; i < n_heads; i++)
  {
    CX_HEAD p_head = CX_GetHead(p_head_list,i,msg);
    if (p_head)
    {
      cx_int marked_for_deletion = CX_GetHeadMarkedForDeletion(p_head, msg);

      vector<cx_byte*> frame_pixels;
      vector<cx_uint> rows_list, cols_list;
      vector<CX_Vector3D> rgbs;
      vector<string> strings;

      int head_id = CX_GetHeadID(p_head, msg);

      // Here we get either the current head data or the top ranked head data
      // depending on the user control flag
      CX_HEAD_DATA p_hd = NULL;
      if (m_display_current_head_image) p_hd = CX_GetCurrentHeadData(p_head, msg);
      else
      {
        CX_HEAD_DATA_LIST p_hdl = CX_GetRankedHeadDataList(p_head, msg);
        if (p_hdl) p_hd = CX_GetHeadData(p_hdl, 0, msg);
        //else p_hd = CX_GetCurrentHeadData(p_head, msg);
      }

      if (p_hd)
      {
        // background colour set to red if it's marked for deletion, blue if FR has occurred
        CX_Vector3D* p_rgb = &white;

        // also display FR ranking
        CX_RankItem rank_res[10];
        cx_int n_res = CX_GetHeadFRRankedResults(p_head, rank_res, 10, msg);
        if (n_res > 0) p_rgb = &blue;
        
        // label displayed image with head ID
        string txt = to_string(head_id);


        if (m_display_fit_measure)
        {
          // add fit measure (if exists)
          cx_real fit = CX_HeadDataFit(p_hd, msg);
          if (fit >= 0.0f)
          {
            char buf[20];
            sprintf(buf, " %1.2f", fit);
            string str(buf);
            txt += str;
          }
        }

        // background colour set to red if it's marked for deletion
        //CX_Vector3D* p_rgb = &white;
        if (1 == marked_for_deletion) p_rgb = &red;
        AddImage(CX_HeadDataImage(p_hd, msg), frame_pixels, rows_list, cols_list, rgbs, strings, *p_rgb, txt);


        if (m_display_pc_head_image)
        {
          AddImage(CX_HeadDataPCimage(p_hd, msg), frame_pixels, rows_list, cols_list, rgbs, strings, *p_rgb, to_string(head_id));
        }
        
        // also display FR ranking
        //CX_RankItem rank_res[10];
        //cx_int n_res = CX_GetHeadFRRankedResults(p_head, rank_res, 10, msg);
        if (n_res > 0)
        {
          for (cx_int k = 0; k < m_display_rank_n && k<n_res; k++)
          {
            AddRankImage(rank_res[k], frame_pixels, rows_list, cols_list, rgbs, strings, images_to_free);
          }
        }
        
        // insert into the grid
        if (frame_pixels.size() > 0)
        {
          mp_grid->InsertFrameSet(frame_index++, frame_pixels, rows_list, cols_list, strings, rgbs);
        }
        
      }
    }
  }

  // free any aureus images
  for (cx_uint j = 0; j<images_to_free.size(); j++)
  {
    CX_FreeImage(images_to_free[j],msg);
  }

  // remove un-needed frames
  while (mp_grid->m_frames.size() > frame_index)
  {
    mp_grid->m_frames.pop_back();
  }
  
}
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::LeftUp(CX_Vector2D& n_mpos, cx_coord mpos)
{
  if (!m_is_enrolling) return;
  char msg[1024];

  int head_list_size = CX_GetHeadListSize(mp_head_list, msg);
  if (head_list_size < 0) return;
  
  CX_HeadInfo hinfo;

  for (cx_int i = 0; i < head_list_size; i++)
  {
    CX_HEAD p_head = CX_GetHead(mp_head_list, i, msg);
    if (p_head)
    {
      cx_int n_images = CX_GetNumHeadEnrollmentImages(p_head, msg);
      if (n_images >= 3)
      {
        if (0 != CX_GetHeadInfo(p_head, &hinfo, msg))
        {
          cx_int frame_diff = cx_abs(hinfo.m_frame_number - m_current_frame_num);
          if (frame_diff < 4 && hinfo.m_head_ok)
          {
            // determine if the mouse click the individual
            if (n_mpos.x()>hinfo.m_head_bl_x && n_mpos.y()>hinfo.m_head_bl_y &&
              n_mpos.x() < hinfo.m_head_tr_x && n_mpos.y() < hinfo.m_head_tr_y)
            {
              // everything checks out, the head can be enrolled
              //CX_Base62 b;
              const char* person_name = "AutoEnroll";
              //cx_int person_id = CX_AddNewPersonFromHead(mp_mainframe->mp_aureus, b.HundrethsSecondTimeGUID().c_str(), p_head, 3, msg);
              cx_int person_id = CX_AddNewPersonFromHead(mp_mainframe->mp_aureus, person_name, p_head, 3, msg);
              if (person_id < 0)
              {
                wxMessageBox(msg);
              }
              m_is_enrolling = false;

              // update gallery display
              (void)mp_mainframe->mp_gallery_panel->Initialize(mp_mainframe);

            }
          }
        }
      }
    }
  }
}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num)
{
  char msg[1024];

  // update frame counters
  m_current_frame_num = frame_num;
  m_frame_count++;


  // count num of active valid people being processed
  m_num_people = 0;
  cx_int np = CX_GetPeopleCount(mp_aureus_video, msg);
  if (np >= 0) m_num_people = np;
  
  mp_head_list = p_head_list; 

  // add results to display grid
  AddGridResults(p_head_list, head_list_size);
  
  /////////////////////////////////////////////////////////
  if (m_is_enrolling)
  {
    //OverlayHeads(p_pixels, rows, cols, p_head_list, head_list_size);
  }

  /////////////////////////////////////////////////////////
  cx_real ptime = CX_GetFrameProcessTime(mp_aureus_video, msg);
  if (ptime > 0.0f) m_pfps = 1.0f / ptime;

  m_frame_rows = rows;
  m_frame_cols = cols;

  UpdateTimerInfo();
#ifdef WIN32
  SetImage(p_pixels, rows, cols);
  if (m_is_first_frame)
  {
    Seek();
    Render();
    m_is_first_frame = false; 
  }
  if (mp_grid) mp_grid->Render();
#else
  //CE_List<CE_String> texts(3);
  //texts.Append( m_title + " Playing") ;
  //texts.Append(CE_String("Frame Num: ") + CE_String(m_video.m_frames_received) );
  //texts.Append(CE_String("FPS: ") + CE_String(m_video.m_fps, 2));

  if (m_is_first_frame) m_do_seek = true;
  //SetImageRefreshPaint(p_pixels, rows, cols, &texts);
  SetImageRefreshPaint(p_pixels, rows, cols);

  if (mp_grid) mp_grid->RefreshGrid();

#endif
  /////////////////////////////////////////////////////////



}
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::OverlayHeads(cx_byte* p_pixels, cx_uint rows, cx_uint cols, CX_HEAD_LIST p_head_list, cx_uint head_list_size)
{
  char msg[1024];
  cx_uint start_row = 0;
  cx_uint rows_size = cols * 4;
  for (cx_uint i = 0; i < head_list_size; i++)
  {
    CX_HEAD p_head = CX_GetHead(p_head_list, i, msg);
    if (p_head)
    {
      CX_HEAD_DATA p_head_data = CX_GetCurrentHeadData(p_head, msg);
      if (p_head_data)
      {
        CX_IMAGE p_im = CX_HeadDataImage(p_head_data, msg);
        if (p_im)
        {
          cx_uint hrows, hcols;
          cx_byte* p_head_pixels = CX_ImageData(p_im, &hrows, &hcols, msg);
          if (p_head_pixels)
          {
            cx_uint hrow_size = hcols * 4;
            // now overlay the head image on top of the display image
            for (cx_uint r = 0; r < hrows; r++)
            {
              cx_uint ri = r + start_row;
              if (ri < rows)
              {
                cx_byte* p_dst_row = &p_pixels[ri*rows_size];
                cx_byte* p_src_row = &p_head_pixels[r*hrow_size];
                memcpy(p_dst_row, p_src_row, hrow_size);
              }
            }
            start_row += hrows;
          }
        }
      }
    }
  }
}
///////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////
void CX_VideoDisplay::UpdateDisplayDev()
{
  //if (!m_display)
  {
    //SetImage(im);
    //SetImage(m_half_frame);
    //if (mp_grid) mp_grid->Render();
  }
}
///////////////////////////////////////////////////////////////////



