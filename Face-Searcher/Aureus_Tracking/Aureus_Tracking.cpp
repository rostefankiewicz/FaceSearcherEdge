//////////////////////////////////////////////////////////////////////////////////////
// Aureus_Tracking.cpp : Defines the entry point for the console application.
//////////////////////////////////////////////////////////////////////////////////////
// Copyright: (C) 2016 CyberExtruder.com Inc.
//                     www.cyberextruder.com
//
// DISCLAIMER:
//
// THIS SOFTWARE IS PROVIDED “AS IS” AND “WITH ALL FAULTS” AND WITHOUT
// WARRANTY OF ANY KIND.
// CUSTOMER AGREES THAT THE USE OF THIS SOFTWARE IS AT CUSTOMER'S RISK
// CYBEREXTRUDER MAKES NO WARRANTY OF ANY KIND TO CUSTOMER OR ANY THIRD 
// PARTY, EXPRESS, IMPLIED OR STATUTORY, WITH RESPECT TO THE THIS 
// SOFTWARE, OPERATION OF THE SOFTWARE, OR OUTPUT OF OR RESULTS OBTAINED 
// FROM THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY 
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OPERABILITY OR 
// NON-INFRINGEMENT AND ALL SUCH WARRANTIES ARE HEREBY EXCLUDED BY 
// CYBEREXTRUDER AND WAIVED BY CUSTOMER.
//////////////////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <stdio.h>
#include <stdint.h>
#include "CX_Aureus.h"
#include "Aureus.h"
#include "AureusVideo.h"
#include "AureusHeads.h"
#include "AureusGallery.h"
#include "AureusImage.h"
#include "cxutils.h"
#include <atomic>
//////////////////////////////////////////////////////////////////////////////////////


#ifndef WIN32
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////////////////////
void DoSleep(int millisec)
{
#ifdef WIN32
  Sleep(millisec);
#else
  usleep(millisec * 1000);
#endif
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
void SetDetectionParameters(CX_VIDEO p_video)
{
  char msg[1204];
  // set head detection params
  float top = 0.00f;
  float left = 0.00f;
  float width = 1.0f;
  float height = 1.0f;

  // these are the most important detection settings to control depending on the content of
  // your image sequence
  float min_height_prop = 0.15f; // this works well for the "Doorway4" sequence
  //float min_height_prop = 0.2f;  // this works well for many sequences and is faster
  float max_height_prop = 0.6f; // this assumes that all heads are less than 0.6 image height

  //int frame_reduction_step = 2; // force half size frames, faster but might miss some heads
  int frame_reduction_step = 1; // use full size frames

  if (0 == CX_SetFrameReductionStep(p_video, frame_reduction_step, msg))
  {
    printf("FAILED to set frame reductions step:\n%s\n\n", msg);
  }

  int frame_interval = 1; // process every frame frame
  //int frame_interval = 2; // skip every other frame
  if (0 == CX_SetFrameInterval(p_video, frame_interval, msg))
  {
    printf("FAILED to set frame interval:\n%s\n\n", msg);
  }

  if (!CX_SetDetectionParameters(p_video, top, left, height, width, min_height_prop, max_height_prop, msg))
  {
    printf("FAILED to set detection parameters:\n%s\n\n", msg);
  }


  cx_real fdp[6];
  CX_GetDetectionParameters(p_video, fdp, msg);
  // 0 = top, 1 = left, 2 = height, 3 = width, 4 = min_height_prop, 5 = max_height_prop
  printf("Detection params:\n");
  printf("top = %f,  left = %f, height = %f, width = %f\n", fdp[0], fdp[1], fdp[2], fdp[3]);
  printf("min prop = %f,  max prop = %f\n", fdp[4], fdp[5]);

}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
void ProcessFramesSeparateDetection(CX_VIDEO p_video, const char* dir, bool print_details = true)
{
  int i;
  char msg[1204];
  std::vector<CX_RAM_Image> frames;
  LoadFrames(dir, frames, print_details);

  printf("Processing sequential images frame from folder:\n%s\n", dir);

  printf("Detecting heads in %d images\n", (int)frames.size());

  SetDetectionParameters(p_video);


  std::vector<CX_HeadInfo*> heads;
  std::vector<int> num_heads;
  int max_heads = 20;
  CX_HeadInfo* tmp_head = NULL;

  //////////// Detection ///////////////////////////////////
  // step through all frames detecting heads
  for (i = 0; i < (int)frames.size(); i++)
  {
    // allocate mem for head detections
    heads.push_back(tmp_head);
    heads[i] = new CX_HeadInfo[max_heads];
    int use_face_detector = 1;
    int use_fail_safe = 0;

    int rval = CX_DetectHeads(p_video, &frames[i], i, heads[i], max_heads, use_face_detector, use_fail_safe, msg);
    // catch any errors
    if (rval < 0)
    {
      printf("Image %d error: %s\n", i, msg);
      num_heads.push_back(0); // save zero heads because there was an error
    }
    else
    {
      if (print_details) printf("Image %d detected %d heads\n", i, rval);
      num_heads.push_back(rval); // save number of detected heads
    }
  }
  //////////////////////////////////////////////////////////////

  ////////////////// Tracking ////////////////////////////////
  // now step through them applying tracking
  printf("Tracking heads in %d images\n", (int)frames.size());
  for (i = 0; i < (int)frames.size(); i++)
  {
    CX_HEAD_LIST p_head_list = CX_ProcessFrameFromDetectedHeads(p_video, &frames[i], i, heads[i], num_heads[i], msg);

    if (!p_head_list) printf("Error at frame num %d : %s\n", i, msg);
    else PrintHeadListData(i, p_head_list, print_details);

  }
  //////////////////////////////////////////////////////////////


  // delete allocated head info data
  for (i = 0; i < (int)frames.size(); i++)
  {
    if (heads[i]) delete[] heads[i];
    heads[i] = NULL;
  }

  // delete the images
  DeleteFrames(frames);
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void TestDetectHeads(CX_VIDEO p_video)
{
  string image_file = "D:/CyberBase/data/Gallery/Tygart/Roby/bounding_box/A.jpg";
  CX_RAM_Image im;
  if (!LoadImageFromDisk(image_file.c_str(), im)) return;

  printf("Detecting heads in %s\n", image_file.c_str());

  char msg[1204];

  SetDetectionParameters(p_video);

  int max_heads = 20;
  CX_HeadInfo p_heads[20];

  int use_face_detector = 1;
  int use_fail_safe = 0;

  int rval = CX_DetectHeads(p_video, &im, 1, p_heads, max_heads, use_face_detector, use_fail_safe, msg);
  // catch any errors
  if (rval < 0)
  {
    printf("Detect Heads error: %s\n", msg);
  }
  else
  {
    printf("Detected %d heads\n", rval);

    for (int i = 0; i < rval; i++)
    {
      CX_HeadInfo hi = p_heads[i];
      // here we convert to pixels with origin top left to easily
      // enable us to see where on the image the detection occurs
      if (hi.m_face_ok)
      {
        // swap origin
        hi.m_face_bl_y = 1.0f - hi.m_face_bl_y;
        hi.m_face_tr_y = 1.0f - hi.m_face_tr_y;
        // convert to pixels
        hi.m_face_bl_x *= im.m_cols;
        hi.m_face_bl_y *= im.m_rows;
        hi.m_face_tr_x *= im.m_cols;
        hi.m_face_tr_y *= im.m_rows;
      }
      if (hi.m_head_ok)
      {
        // swap origin
        hi.m_head_bl_y = 1.0f - hi.m_head_bl_y;
        hi.m_head_tr_y = 1.0f - hi.m_head_tr_y;
        // convert to pixels
        hi.m_head_bl_x *= im.m_cols;
        hi.m_head_bl_y *= im.m_rows;
        hi.m_head_tr_x *= im.m_cols;
        hi.m_head_tr_y *= im.m_rows;
      }

      PrintHeadInfo(hi);
    }
  }


  // free mem allocate for image pixels
  DeleteImagePixels(im);
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
void GenerateTemplate(const char* image_file, CX_AUREUS p_aureus, CX_VIDEO p_video, int frame_num, vector<cx_byte*>& templates)
{
  char msg[1204];
  SetDetectionParameters(p_video);


  CX_RAM_Image im;
  if (!LoadImageFromDisk(image_file, im)) return;

  int max_heads = 20;
  CX_HeadInfo p_heads[20];

  int use_face_detector = 1;
  int use_fail_safe = 1;

  printf("Detecting heads\n");
  int n_heads = CX_DetectHeads(p_video, &im, 1, p_heads, max_heads, use_face_detector, use_fail_safe, msg);
  // catch any errors
  if (n_heads < 0)
  {
    printf("CX_DetectHeads error: %s\n", msg);
    return;
  }

  printf("Detected %d heads, processing from detections\n", n_heads);
  CX_HEAD_LIST p_head_list = CX_ProcessFrameFromDetectedHeads(p_video, &im, frame_num, p_heads, n_heads, msg);
  if (NULL == p_head_list)
  {
    printf("CX_ProcessFrameFromDetectedHeads error: %s\n", msg);
    return;
  }

  int n_tracked_heads = CX_GetHeadListSize(p_head_list, msg);
  if (n_tracked_heads < 0)
  {
    printf("CX_GetHeadListSize error: %s\n", msg);
    return;
  }

  int annos_size = CX_AnnotationsByteArraySize(p_aureus, msg);
  if (annos_size < 0)
  {
    printf("CX_AnnotationsByteArraySize error: %s\n", msg);
    return;
  }
  cx_byte* p_array = new cx_byte[annos_size];


  printf("Tracking %d heads, extracting data\n", n_tracked_heads);
  for (int i = 0; i < n_tracked_heads; i++)
  {
    printf("---- Head num %d ----\n", i);
    CX_HEAD p_head = CX_GetHead(p_head_list, i, msg);
    if (p_head < 0)
    {
      printf("CX_GetHead (i=%d) error: %s\n", i, msg);
    }
    else
    {
      CX_HEAD_DATA p_head_data = CX_GetCurrentHeadData(p_head, msg);
      if (p_head_data == NULL)
      {
        printf("CX_GetCurrentHeadData (i=%d) error: %s\n", i, msg);
      }
      else
      {
        CX_IMAGE p_im = CX_HeadDataImage(p_head_data, msg);
        if (!p_im)
        {
          printf("CX_HeadDataImage (i=%d) error: %s\n", i, msg);
        }
        else
        {
          // get extracted image so we can use it further down
          CX_RAM_Image him;
          him.mp_pixels = CX_ImageData(p_im, &him.m_rows, &him.m_cols, msg);
          him.m_type = 1; // always RGBA
          him.m_origin = 1; //always bot left

          if (1 != CX_HeadDataAnnotationsAsByteArray(p_head_data, p_array, msg))
          {
            printf("CX_HeadDataAnnotationsAsByteArray error: %s\n", msg);
          }
          else
          {
            int t_size = CX_GetTemplateSize(p_aureus, msg);
            if (t_size < 0)
            {
              printf("CX_GetTemplateSize error: %s\n", msg);
            }
            else
            {
              // NOTE! using the returned extracted image not the original
              // as the extracted image corresponds to the annotations
              cx_byte* p_template = new cx_byte[t_size];
              if (1 != CX_GenerateTemplateFromPackedAnnotations(p_aureus, &him, p_array, p_template, msg))
              {
                printf("CX_GenerateTemplateFromPackedAnnotations error: %s\n", msg);
                delete[] p_template;
              }
              else
              {
                templates.push_back(p_template);
              }
            }
          }
        }
      }
    }
    printf("-----------\n");
  }
  delete [] p_array;
  DeleteImagePixels(im);
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void ImageFR(CX_AUREUS p_aureus, CX_VIDEO p_video)
{
  char msg[1204];

  int t_size = CX_GetTemplateSize(p_aureus, msg);
  if (t_size < 0)
  {
    printf("CX_GetTemplateSize error: %s\n", msg);
    return;
  }


  string image_file1 = "D:/CyberBase/data/Gallery/Aimetis/Mike2.jpg";
  vector<cx_byte*> templates1;
  GenerateTemplate(image_file1.c_str(), p_aureus, p_video, 1, templates1);
  printf("Got %d templates from image: %s\n", (int)templates1.size(), image_file1.c_str());

  string image_file2 = "D:/CyberBase/data/Gallery/Aimetis/Mike3.jpg";
  vector<cx_byte*> templates2;
  GenerateTemplate(image_file2.c_str(), p_aureus, p_video, 2, templates2);
  printf("Got %d templates from image: %s\n", (int)templates2.size(), image_file2.c_str());


  for (int t1 = 0; t1 < (int)templates1.size(); t1++)
  {
    for (int t2 = 0; t2 < (int)templates2.size(); t2++)
    {
      cx_real score = CX_MatchFRtemplates(p_aureus, templates1[t1], t_size, templates2[t2], t_size, msg);
      if (score < 0.0f)
      {
        printf("CX_MatchFRtemplates (%d, %d) error: %s\n", t1, t2, msg);
      }
      else
      {
        printf("Matching index1 %d against index2 %d, score = %f\n", t1, t2, score);
      }
    }
  }

  // clean up allocated templates
  int i;
  for (i = 0; i < (int)templates1.size(); i++)
  {
    if (templates1[i]) delete[] templates1[i];
    templates1[i] = NULL;
  }
  for (i = 0; i < (int)templates2.size(); i++)
  {
    if (templates2[i]) delete[] templates2[i];
    templates2[i] = NULL;
  }
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void ImageFRwithFocus(CX_AUREUS p_aureus)
{
  char msg[1204];

  int t_size = CX_GetTemplateSize(p_aureus, msg);
  if (t_size < 0)
  {
    printf("CX_GetTemplateSize error: %s\n", msg);
    return;
  }
  printf("Template size = %d\n", t_size);

  CX_DetectionParams fdp;
  fdp.m_left = 0.0f; fdp.m_top = 0.0f; fdp.m_width = 1.0f; fdp.m_height = 1.0f;
  fdp.m_min_height_prop = 0.2f; fdp.m_max_height_prop = 1.0f;

  CX_RAM_Image im1, im2;
  // these are actually the same image (originally), however the second is 
  // half the size of the first and smoothed, this will show up in the focus 
  // and confidence measures, however the resulting FR match will still be
  // extremely good (in fact identical, score = 1.0) size the matcher is
  // robust to low res images
  string image_file1 = "D:/CyberBase/data/Gallery/Aimetis/Mike3.jpg";
  string image_file2 = "D:/CyberBase/data/Gallery/Aimetis/Mike4.jpg"; 

  if (!LoadImageFromDisk(image_file1.c_str(), im1)) return;
  if (!LoadImageFromDisk(image_file2.c_str(), im2)) return;

  cx_byte* template1 = new cx_byte[t_size];
  cx_byte* template2 = new cx_byte[t_size];
  bool t1_ok = false;
  bool t2_ok = false;

  cx_real face_bl_x, face_bl_y, face_tr_x, face_tr_y;
  cx_real conf, focus;
  if (0 == CX_GenerateTemplateAndFaceMeasures(p_aureus, &im1, fdp, template1, &face_bl_x, &face_bl_y, &face_tr_x, &face_tr_y, &conf, &focus, msg))
  {
    printf("%s\n", msg);
  }
  else
  {
    t1_ok = true;
    printf("Got template from image 1, confidence = %f, focus = %f\n", conf, focus);
    printf("Face BL(%f,%f) TR(%f,%f)\n", face_bl_x, face_bl_y, face_tr_x, face_tr_y);
  }

  if (0 == CX_GenerateTemplateAndFaceMeasures(p_aureus, &im2, fdp, template2, &face_bl_x, &face_bl_y, &face_tr_x, &face_tr_y, &conf, &focus, msg))
  {
    printf("%s\n", msg);
  }
  else
  {
    t2_ok = true;
    printf("Got template from image 2, confidence = %f, focus = %f\n", conf, focus);
    printf("Face BL(%f,%f) TR(%f,%f)\n", face_bl_x, face_bl_y, face_tr_x, face_tr_y);
  }


  if (t1_ok && t2_ok)
  {
    cx_real score = CX_MatchFRtemplates(p_aureus, template1, t_size, template2, t_size, msg);
    if (score < 0.0f) printf("%s\n", msg);
    else printf("Match score = %f\n", score);
  }

  delete[] template1;
  delete[] template2;

  DeleteImagePixels(im1);
  DeleteImagePixels(im2);
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void ProcessSequentialFrames(CX_VIDEO p_video, const char* dir, bool print_details = true)
{
  printf("FInding images in %s\n", dir);

  char msg[1204];
  std::vector<std::string> fnames;
  CE_FindFiles(fnames, dir, "*.jpg");

  printf("Processing %d images from %s\n", (int)fnames.size(), dir);

  SetDetectionParameters(p_video);


  for (int i = 0; i < (int)fnames.size(); i++)
  {
    string& fname = fnames[i];
    CX_RAM_Image im;
    if (LoadImageFromDisk(fname.c_str(), im))
    {
      string im_name = GetFileName(fname);
      printf("Loaded frame %d %s\n", i, im_name.c_str());

      int use_face_detector = 1;
      int use_fail_safe = 0;
      CX_HEAD_LIST p_head_list = CX_ProcessFrame(p_video, &im, i, use_face_detector, use_fail_safe, msg);

      if (!p_head_list) printf("Error at frame num %d : %s\n", i, msg);
      else PrintHeadListData(i, p_head_list, print_details);

      // free mem allocate for image pixels
      DeleteImagePixels(im);
    }
    else
    {
      printf("FAILED TO LOAD FRAME %d\n", i);
    }
  }
  printf("Finished processing %d sequential frames\n", (int)fnames.size());
}
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
void TestSequentialFrames(CX_AUREUS p_aureus, CX_VIDEO p_video, bool separate_detection, bool print_details = true)
{
  // this is a simple set of sequential images used to show Aureus is working properly
  // it contains one person walking through a doorway
#ifdef WIN32
  const char* image_dir = "C:/CyberExtruder/Aureus/SequentialFrames/Doorway4";
#else
  const char* image_dir = "/home/tim/CyberExtruder/Aureus/SequentialFrames/Doorway4";
#endif

  // this example detects and tracks all at once
  if (!separate_detection)  ProcessSequentialFrames(p_video, image_dir, print_details);

  // this example loads all frames, detects head in each frame, then tracks afterwards
  // a demonstration of how to separate the detection from the tracking
  else ProcessFramesSeparateDetection(p_video, image_dir, print_details);

  // if you were going to process more than one frame set, (or stream or camera etc) with
  // the same video object then remember to reset the processing between calls so that we 
  // tell Aureus it is a new set of sequential images thus:
  //if (0 == CX_ResetProcessing(p_video, msg))
  //{
  //  printf("%s\n", msg);
  //  return;
  //}

}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void TestSequentialFramesTwice(CX_AUREUS p_aureus, CX_VIDEO p_video, bool separate_detection)
{
  char msg[1024];

  // this is a simple set of sequential images used to show Aureus is working properly
  // it contains one person walking through a doorway
#ifdef WIN32
  const char* image_dir = "C:/CyberExtruder/Aureus/SequentialFrames/Doorway4";
#else
  const char* image_dir = "/home/tim/CyberExtruder/Aureus/SequentialFrames/Doorway4";
#endif

  // this example detects and tracks all at once
  if (!separate_detection) ProcessSequentialFrames(p_video, image_dir, false);

  // this example loads all frames, detects head in each frame, then tracks afterwards
  // a demonstration of how to separate the detection from the tracking
  else ProcessFramesSeparateDetection(p_video, image_dir, false);


  // reset the processing again so that we tell Aureus it is a new set of sequential images
  printf("Reseting  Video Processing\n");
  if (0 == CX_ResetProcessing(p_video, msg))
  {
    printf("%s\n", msg);
    return;
  }


  // this example detects and tracks all at once
  if (!separate_detection) ProcessSequentialFrames(p_video, image_dir, false);

  // this example loads all frames, detects head in each frame, then tracks afterwards
  // a demonstration of how to separate the detection from the tracking
  else ProcessFramesSeparateDetection(p_video, image_dir, false);

}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// Simple frame call back
void FrameCallBack(CX_HEAD_LIST p_head_list, cx_uint head_list_size, cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint frame_num, void* p_object)
{
  // prints all data
  //PrintHeadListData(frame_num, p_head_list);

  // just print num of heads being tracked
  char msg[1204];
  int n_heads = CX_GetHeadListSize(p_head_list, msg);
  if (n_heads < 0) printf("Frame %d Error: %s\n", frame_num, msg);
  else
  {
    //printf("FRAME %d, Tracking %d HEADS rows=%d cols=%d\n", frame_num, n_heads,rows,cols);

    // or print more detailed info (including FR if switched on)
    PrintHeadListData(frame_num, p_head_list, true);
  }

}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// Simple unexpected stream termination call back
void UnexpectedTerminationCallBack(cx_int stream_type, const char* connection_info, void* p_object)
{
  fprintf(stderr, "Unexpected stream termination, stream type = %d, connection_info = %s\n", stream_type, connection_info);
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// This is a simple example of how to use the callback that occurs
// when a media file has finished processing.
// This callback function will be called at that moment. We simply
// print that the file has finished processing and set an atomic
// flag that allows the main thread to spin waiting for the processing
// to finish whiclts checking for it in a thread safe way.
//////////////////////////////////////////////////////////////////////////////////////
std::atomic<bool> g_media_has_finished;
////////////////////////////////////////////////////////////////////
void EndOfMediaFileCallBack(cx_uint frame_num, void* p_object)
{
  printf("\nEND OF MEDIA FILE Callback, frame_num = %d\n",frame_num);
  g_media_has_finished = true;
}
////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
void TestMediaFileStream(CX_AUREUS p_aureus, CX_VIDEO p_video)
{
  char msg[1024];


  // this is a media file used to show Aureus is working properly from a stream
  // it contains one person walking through a doorway
#ifdef WIN32
  const char* video_file = "C:/CyberExtruder/Aureus/Videos/Doorway4.wmv";
#else
  const char* video_file = "/home/tim/CyberExtruder/Aureus/Videos/Doorway4.wmv";
  //const char* video_file = "/home/tim/CyberExtruder/Aureus/Videos/Doorway4_Full.wmv";
#endif


  printf("Processing media file stream:\n%s\n", video_file);

  // set the detection parameters for the stream
  SetDetectionParameters(p_video);

  // set a frame callback so we can capture the results
  if (0 == CX_SetFrameCallBack(p_video, FrameCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // set a media file finished callback
  if (0 == CX_SetMediaFileFinishedCallBack(p_video, EndOfMediaFileCallBack, NULL, msg)) 
  { 
    printf("%s\n", msg);
    return;
  };


  // set an unexpected termination callback (just to demonstrate, since it's a 
  // media file it's very unlikely to terminate unexpectedly)
  if (0 == CX_SetStreamTerminatedCallBack(p_video, UnexpectedTerminationCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  int frame_interval = 1;
  if (0 == CX_SetFrameInterval(p_video, frame_interval, msg))
  {
    printf("%s\n", msg);
    return;
  }
  frame_interval = CX_GetFrameInterval(p_video, msg);
  printf("Frame Interval = %d\n", frame_interval);

  int frame_reduction_step = 1;
  if (0 == CX_SetFrameReductionStep(p_video, frame_reduction_step, msg))
  {
    printf("%s\n", msg);
    return;
  }
  frame_reduction_step = CX_GetFrameReductionStep(p_video, msg);
  printf("Frame Reduction step = %d\n", frame_reduction_step);


  int force_every_frame = 0;
  //int force_every_frame = 1;
  if (0 == CX_SetForceEveryFrame(p_video, force_every_frame, msg))
  {
    printf("%s\n", msg);
    return;
  }
  force_every_frame = CX_GetForceEveryFrame(p_video, msg);
  printf("Force every frame flag = %d\n", force_every_frame);


  // Process the stream
  cx_uint stream_type = 0; // Media file
  if (0 == CX_ProcessStream(p_video, stream_type, video_file, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // spin whilst waiting for the media file
  // to finish processing
  while (true)
  {
    if (g_media_has_finished) break;
    else
    {
      DoSleep(500); // sleep for a while before checking
    }
  }
  printf("FINISHED SPINNING, detected end of media file processing\n");

  // this is not necessary, but it demonstrates that
  // there are no more frames being processed
  // if you want to prove this, uncomment this sleep call
  //DoSleep(2000);


  // Stop processing
  if (0 == CX_StopStream(p_video, msg))
  {
    printf("%s\n", msg);
    return;
  }
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void TestUSBStream(CX_AUREUS p_aureus, CX_VIDEO p_video)
{
  char msg[1024];


  // here we're going to show Aureus is working properly from a USB stream
  // if you havn't got a USB camera connected then this won't work
  // also, you may have to adjust the pin number, usually the first USB camera
  // is allocated pin number 0, then 1 etc.
  const char* usb_pin = "0";
  printf("Processing USB at pin: %s\n", usb_pin);

  // set the detection parameters for the stream
  SetDetectionParameters(p_video);

  // set a frame callback so we can capture the results
  if (0 == CX_SetFrameCallBack(p_video, FrameCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // set an unexpected termination callback (just to demonstrate, since it's a 
  // media file it's very unlikely to terminate unexpectedly)
  if (0 == CX_SetStreamTerminatedCallBack(p_video, UnexpectedTerminationCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // Process the stream
  cx_uint stream_type = 1; // USB camera
  if (0 == CX_ProcessStream(p_video, stream_type, usb_pin, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // wait a while
  DoSleep(10000);


  // Stop processing
  if (0 == CX_StopStream(p_video, msg))
  {
    printf("%s\n", msg);
    return;
  }
}
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
void TestIPStream(CX_AUREUS p_aureus, CX_VIDEO p_video)
{
  char msg[1024];



  // here we're going to show Aureus is working properly from an IP stream
  // Please note you will need to change this IP camera url to one
  // you have, otherwise it will not work.
  // Fir RTSP connections the string is as follows:
  //
  // "rtsp://username:password@IPaddress:portnum/manufacturers_string"
  //
  // these urls may or may not work, depends if the cmaera is connected.
  const char* url = "http://199.249.109.58/mjpg/video.mjpg?.mjpeg";
  //const char* url = "http://166.142.23.54/mjpg/video.mjpg?.mjpeg";
  printf("Processing IP Camera at url:\n%s\n", url);

  // set the detection parameters for the stream
  SetDetectionParameters(p_video);

  // set a frame callback so we can capture the results
  if (0 == CX_SetFrameCallBack(p_video, FrameCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // set an unexpected termination callback (just to demonstrate, since it's a 
  // media file it's very unlikely to terminate unexpectedly)
  if (0 == CX_SetStreamTerminatedCallBack(p_video, UnexpectedTerminationCallBack, NULL, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // Process the stream
  cx_uint stream_type = 2; // IP camera
  if (0 == CX_ProcessStream(p_video, stream_type, url, msg))
  {
    printf("%s\n", msg);
    return;
  }

  // wait a while
  DoSleep(10000);


  // Stop processing
  if (0 == CX_StopStream(p_video, msg))
  {
    printf("%s\n", msg);
    return;
  }
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
void FRupdateCallback(const char* msg, cx_int total, cx_int current, void* p_object)
{
  if (total>0) printf("Updating FR %s %d/%d   \r", msg, current, total);
}
//////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  char msg[1024];

  /////////////////////////////////////////////////////////////
  int load_gallery = 0; // instruct Aureus NOT to load the FR gallery
  // some FR flags, you can use FR with ot without a gallery
  // or not at all, if you intend to perform FR agaianst
  // a gallery then you need to instruct Aureus to load a gallery
  //

  // we need to load the FR engines (might be mroe than one) if
  // we want to perform any FR
  //bool load_FR_engines = false;
  bool load_FR_engines = true;

  // For this example program fr is switch off since we are just cetecting 
  // and tracking, however you can switch it back on if required
  bool use_fr = true;
  //bool use_fr = false;
  if (use_fr)
  {
    load_gallery = 1; // instruct Aureus to load the FR gallery
    load_FR_engines = true; // must also load the engines
  }
  /////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////
  // just a bit of feedback to std out
  printf("----------------\n");
  if (load_FR_engines) printf("Loading FR engines\n");
  else printf("Not loading FR engines\n");
  if (use_fr) printf("Using FR\n");
  else printf("Not using FR\n");
  if (load_gallery) printf("Loading Gallery\n");
  else printf("Not loading gallery\n");
  printf("----------------\n");
  /////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////
  // first thing we do is demonstrate getting the machine ID
  if (!CX_GetMachineID(msg)) printf("Failed to get machine ID:\n%s\n", msg);
  else printf("Machine ID = %s\n", msg);
  /////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////
  // create Aureus instance
  CX_AUREUS p_aureus = CX_CreateAureus(msg);
  if (!p_aureus)
  {
    printf("Failed to create Aureus Object\n%s\n",msg);
    return 0;
  }
  printf("Created Aureus\n");
  /////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////
  int use_verbose = 1; // can be useful if you have (somehow) lost required files
  (void)CX_SetVerbose(p_aureus, use_verbose, msg);
  /////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////
  // print out the Aureus version
  if (!CX_GetVersion(p_aureus, msg)) printf("Failed to get Aureus Version:\n%s\n", msg);
  else printf("Aureus Version = %s\n", msg);
  /////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////
  // initialize and process video frames
  // 
  // this is the default installation path for Aureus, obviously this will need
  // to be altered if you install anywhere else
#ifdef WIN32
  #ifdef IS_32_BIT
    const char* install_dir = "C:/CyberExtruder/Aureus32"; // this is the default folder for 32 bit Aureus
  #else
    const char* install_dir = "C:/CyberExtruder/Aureus";
  #endif
#else
  #ifdef IS_32_BIT
    const char* install_dir = "/home/tim/CyberExtruder/Aureus32"; // this is the default folder for 32 bit Aureus
  #else
    const char* install_dir = "/home/tim/CyberExtruder/Aureus";
  #endif
#endif
  
  printf("Initializing Aureus from %s\n", install_dir);
  if (CX_Initialize(p_aureus, load_gallery, install_dir, msg))
  {
    printf("Successful initialization\n");

    // print license info
    printf("------- License Info --------\n");
    (void)CX_GetLicenseInfo(p_aureus, msg);
    printf("License Info: %s\n", msg);
    int num_streams, image_enabled, fr_enabled;
    (void)CX_GetLicenseParameters(p_aureus, &num_streams, &image_enabled, &fr_enabled, msg);
    printf("Number of licensed video streams = %d\n", num_streams);
    printf("Image enabled = %d\n", image_enabled);
    printf("FR Enabled = %d\n", fr_enabled);
    printf("-----------------------------\n");

    //////////////////////////////////////////
    // if we're loading FR engines select the engine we want
    if (load_FR_engines)
    {
      int n_engines = CX_GetNumFRengines(p_aureus, msg);
      if (n_engines<0) 
      {
        printf("%s\n", msg);
        CX_FreeAureus(p_aureus, msg);
        return 0;
      }
      else
      {
        printf("There are %d FR engines\n", n_engines);
        // print out the engine names
        for (int i = 0; i < n_engines; i++)
        {
          printf("Engine number %d = %s\n", i, CX_GetFRname(p_aureus, i, msg));
        }
        if (n_engines == 0)
        {
          printf("No FR Engines FR will not be performed!\n");
        }
        else
        {
          // we'll select the last engine as this tends to be the best
          int engine_num = n_engines - 1;
          printf("Selecting FR engine: %s\n", CX_GetFRname(p_aureus, engine_num, msg));
          if (0 == CX_SelectFRengine(p_aureus, engine_num, msg))
          {
            printf("Failed to select engine %d, error = %s\n", engine_num, msg);
            CX_FreeAureus(p_aureus, msg);
            return 0;
          }
          else printf("Sucessfully selected engine %d\n", engine_num);
        }
      }

      // if we are using FR with a gallery make sure it's up to date
      // only reason it would be out of date (apart from corruption) is
      // if we have added a new engine
      if (use_fr)
      {
        // register an FR update callback (will print info during update)
        CX_SetUpdateFRCallBack(p_aureus, FRupdateCallback, NULL, msg);
        // update the FR (this just makes sure all templates are present)
        CX_UpdateFR(p_aureus, msg);
        printf("\n"); // end line to std out
      }
    }
    //////////////////////////////////////////


    ///////////////////////
    // create a video object
    printf("Creating CX_VIDEO object\n");
    CX_VIDEO p_video = CX_CreateVideo(p_aureus, msg);
    if (!p_video)
    {
      printf("%s\n", msg);
      printf("Freeing Aureus\n");
      if (!CX_FreeAureus(p_aureus, msg)) printf("Failed to free Aureus:\n%s\n", msg);
      else printf("Successfully freed Aureus\n");

      return 0;
    }
    printf("Successfully created CX_VIDEO\n");
    //////////////////////


    //////////////////////
    int perform_gallery = 0;
    int generate_templates = 0;
    if (use_fr)
    {
      perform_gallery = 1;
      generate_templates = 1;
    }
    // set the gallery matching flag
    if (0 == CX_SetPerformGalleryFRFlag(p_video, perform_gallery, msg))
    {
      printf("%s\n", msg);
    }
    // also set the template generation flag
    else if (0 == CX_SetGenerateTemplatesFlag(p_video, generate_templates, msg))
    {
      printf("%s\n", msg);
    }
    //////////////////////


    /////////////////////////////////////////////////////////////
    // A flag that simply controls the sequential images test function in this file.
    // If true, then detection and processing (tracking with/without FR) will
    // be performed separately, otherwise they will be performed in one step
    bool separate_detection = false;
    /////////////////////////////////////////////////////////////


    // Simple head detection example in a single image
    //TestDetectHeads(p_video);

    // test sequential frame processing
    //separate_detection = true;
    //TestSequentialFrames(p_aureus, p_video, separate_detection);

    // test sequential frame processing twice, this shows the effect of reseting the processing
    // uncomment to test
    //separate_detection = true; // separate the detection from the processing
    //TestSequentialFramesTwice(p_aureus, p_video, separate_detection);

    // test processing a video stream from a media file
    TestMediaFileStream(p_aureus, p_video);

    // test processing a video stream from a USB camera
    //TestUSBStream(p_aureus, p_video);

    // test processing a video stream from a IP camera
    //TestIPStream(p_aureus, p_video);

    // very simple example of matching two images
    //ImageFR(p_aureus, p_video);

    // very simple example of matching two images providing face locations, confidence and focus measures
    //ImageFRwithFocus(p_aureus);
  }
  /////////////////////////////////////////////////////
  // Initialization failed
  else
  {
    printf("Failed to Initialize Aureus:\n%s\n", msg);
  }
  /////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////
  // Free Aureus
  printf("Freeing Aureus\n");

  if (!CX_FreeAureus(p_aureus, msg)) printf("Failed to free Aureus:\n%s\n", msg);
  else printf("Successfully freed Aureus\n");
  /////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////
  // uncomment to test that the video stream licenses are renewed
  // after a call to CX_FreeAureus
  /*
  printf("------ Testing Video Stream License Renewal ------\n");
  p_aureus = CX_CreateAureus(msg);
  if (CX_Initialize(p_aureus, load_gallery, install_dir, msg))
  {
    printf("Successful initialization\n");
  }
  CX_VIDEO p_video2 = CX_CreateVideo(p_aureus, msg);
  if (!p_video2)
  {
    printf("%s\n", msg);
    return 0;
  }
  printf("Created CX_VIDEO\n");
  if (!CX_FreeAureus(p_aureus, msg)) printf("Failed to free Aureus:\n%s\n", msg);
  else printf("Successfully freed Aureus\n");
  */
  return 0;
}
//////////////////////////////////////////////////////////////////////////////////////



