using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Threading;
using System.Drawing;
using System.Drawing.Imaging;

namespace AureusCsharpConsole
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // a structure to contain information about detected/tracked heads see CX_Aureus.h for more info
    public struct CX_HeadInfo
    {
        public int m_head_id;
        public int m_rows;
        public int m_cols;
        public bool m_head_ok;
        public float m_head_bl_x, m_head_bl_y, m_head_tr_x, m_head_tr_y;
        public bool m_face_ok;
        public float m_face_bl_x, m_face_bl_y, m_face_tr_x, m_face_tr_y;
        public float m_confidence;
        public float m_focus;
        public float m_rot_x;
        public float m_rot_y;
        public float m_rot_z;
        public bool m_has_annotation_set;
        public int m_frame_number;
        public bool m_valid;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // A structure for passing FR rank results
    [StructLayout(LayoutKind.Sequential)]
    public struct CX_RankItem
    {
      // the person ID from the gallery, this means the actual person: so in a list of CX_RankItem's
      // there might be more than one element having the same person_id because there were more
      // than one image per person; alternatively a list of CX_RankItem's might contain rank info
      // combined from many processed images, hence more than one element might have the same person_id
      public int  m_person_id;

      // this is the name of the person defined by the unique id m_person_id
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
      public byte[] m_person_name;

      // this is the image id from the gallery (same rules as person_id above wrt multiple processed images)
      public int  m_image_id;

      // this is the FR match score with range from zero (no match) to 1 (perfect match)
      public float m_score;

    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // a structure for passing in images via RAM
    [StructLayout(LayoutKind.Sequential)]
    public struct CX_RAM_Image
    {
      //public byte* mp_pixels;
      public IntPtr   mp_pixels;
      public uint     m_rows;
      public uint     m_cols;
      public uint     m_type;    // 0=RGB, 1=RGBA, 2=Gray, 3=BGR, 4=BGRA
      public uint     m_origin;  // 0=top left,  1=bottom left
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////
    // A class to create an Aureus Video object and interact with it
    class AureusVideo
    {
        const string aureus_path = "C:/CyberExtruder/Aureus/Aureus.dll";
        public StringBuilder msg = new StringBuilder(1024);

        public IntPtr mp_aureus; // pointer to the global Aureus object
        public IntPtr mp_video;  // pointer to a single Aureus CX_VIDEO object
        // a flag that can be set by Aureus denoting that Aureus has finished processing a media file
        public bool g_media_has_finished; // according the C# spec, because this is small enough it is atomic

        ///// FRAME CALLBACK, called for every processed frame, see AureusVideo.h for more details
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void FrameCallback(
            IntPtr headList, uint headListSize, IntPtr pixels, uint rows, uint columns, uint frameNumber, IntPtr objectPtr);

        [DllImport(aureus_path, EntryPoint = "CX_SetFrameCallBack", CharSet = CharSet.Ansi)]
        public static extern int CX_SetFrameCallBack(IntPtr p_video, FrameCallback callback, IntPtr objPtr, [Out] StringBuilder message);


        ///// MEDIA FILE FINISHED CALLBACK, called when processing the media file has finished, see AureusVideo.h for more details
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void MediaFinishedCallback(uint frameNumber, IntPtr objectPtr);

        [DllImport(aureus_path, EntryPoint = "CX_SetMediaFileFinishedCallBack", CharSet = CharSet.Ansi)]
        public static extern int CX_SetMediaFileFinishedCallBack(IntPtr p_video, MediaFinishedCallback callback, IntPtr objPtr, [Out] StringBuilder message);

        ///// STREAM UNEXPECTED TERMINATION CALLBACK, called when there is an unexpected
        // termination of the video stream (usually loast power/connection to a camera) see AureusVideo.h for more details
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void StreamUnexpectedTerminationCallback(int stream_type, IntPtr connection_info, IntPtr objectPtr);

        [DllImport(aureus_path, EntryPoint = "CX_SetStreamTerminatedCallBack", CharSet = CharSet.Ansi)]
        public static extern int CX_SetStreamTerminatedCallBack(IntPtr p_video, StreamUnexpectedTerminationCallback callback, IntPtr objPtr, [Out] StringBuilder message);



        [DllImport(aureus_path, EntryPoint = "CX_CreateVideo", CharSet = CharSet.Ansi)]
        public static extern IntPtr CX_CreateVideo(IntPtr p_aureus, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SetPerformGalleryFRFlag", CharSet = CharSet.Ansi)]
        public static extern int CX_SetPerformGalleryFRFlag(IntPtr p_video, int perform_gallery, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SetGenerateTemplatesFlag", CharSet = CharSet.Ansi)]
        public static extern int CX_SetGenerateTemplatesFlag(IntPtr p_video, int generate_templates, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetHeadListSize", CharSet = CharSet.Ansi)]
        public static extern int CX_GetHeadListSize(IntPtr p_head_list, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_ProcessStream", CharSet = CharSet.Ansi)]
        public static extern int CX_ProcessStream(IntPtr p_video, uint stream_type, StringBuilder connect_info, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_StopStream", CharSet = CharSet.Ansi)]
        public static extern int CX_StopStream(IntPtr p_video, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetHeadFRRankedResults", CharSet = CharSet.Ansi)]
        public static extern int CX_GetHeadFRRankedResults(IntPtr p_head, ref CX_RankItem p_rank_results, int array_size, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetHeadInfo", CharSet = CharSet.Ansi)]
        public static extern int CX_GetHeadInfo(IntPtr p_head, ref CX_HeadInfo p_info, [Out] StringBuilder message);


        [DllImport(aureus_path, EntryPoint = "CX_GetHead", CharSet = CharSet.Ansi)]
        public static extern IntPtr CX_GetHead(IntPtr p_head_list, int index, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SetFrameInterval", CharSet = CharSet.Ansi)]
        public static extern int CX_SetFrameInterval(IntPtr p_video, int frame_interval, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetFrameInterval", CharSet = CharSet.Ansi)]
        public static extern int CX_GetFrameInterval(IntPtr p_video, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SetFrameReductionStep", CharSet = CharSet.Ansi)]
        public static extern int CX_SetFrameReductionStep(IntPtr p_video, int frame_reduction_step, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetFrameReductionStep", CharSet = CharSet.Ansi)]
        public static extern int CX_GetFrameReductionStep(IntPtr p_video, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SetDetectionParameters", CharSet = CharSet.Ansi)]
        public static extern int CX_SetDetectionParameters(IntPtr p_video, float top, float left, float height, float width, float min_height_prop, float max_height_prop, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_CopyImage", CharSet = CharSet.Ansi)]
        public static extern int CX_CopyImage(ref CX_RAM_Image p_src_image, int dst_im_type, int dst_origin, IntPtr p_dst_pixels, [Out] StringBuilder message);

        // sequential image functions
        [DllImport(aureus_path, EntryPoint = "CX_DetectHeads", CharSet = CharSet.Ansi)]
        public static extern int CX_DetectHeads(IntPtr p_video, ref CX_RAM_Image p_im, int frame_number, ref CX_HeadInfo p_head_info_array, int array_size, int use_face_detector, int fail_safe, [Out] StringBuilder message);


        //////////////////////////////////////////////////////////////////////////////////////
        public void PrintHeadInfo(ref CX_HeadInfo hi)
        {
            //Console.WriteLine("*** PrintHeadInfo ***");
            Console.WriteLine("Frame num:" + hi.m_frame_number + " Head ID:" + hi.m_head_id + " size(r,c)=(" + hi.m_rows + "," + hi.m_cols + ")");
            if (!hi.m_head_ok) Console.WriteLine("head_ok=false");
            else
            {
                Console.WriteLine("head_ok=true");
                Console.Write("bl(" + hi.m_head_bl_x + "," + hi.m_head_bl_y + ")");
                Console.Write("tr(" + hi.m_head_tr_x + "," + hi.m_head_tr_y + ")");
            }
            Console.Write("\n");
            if (!hi.m_face_ok) Console.WriteLine("m_face_ok=false");
            else
            {
                Console.WriteLine("m_face_ok=true");
                Console.Write("bl(" + hi.m_face_bl_x + "," + hi.m_face_bl_y + ")");
                Console.Write("tr(" + hi.m_face_tr_x + "," + hi.m_face_tr_y + ")");
            }
            Console.Write("\n");
            if (hi.m_has_annotation_set)
            {
                Console.WriteLine("conf: "+hi.m_confidence+", focus: "+hi.m_focus+", has anns=true");
            }
            else
            {
                Console.WriteLine("has anna = false");
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////////////////
        public void PrintHeadFR(IntPtr p_head)
        {
          //Console.WriteLine("*** PrintHeadFR ***");
          CX_RankItem rank = new CX_RankItem();
          int n = CX_GetHeadFRRankedResults(p_head, ref rank, 1, msg);
          if (n > 0)
          {
              string s = System.Text.Encoding.UTF8.GetString(rank.m_person_name).TrimEnd('\0');
            Console.WriteLine("Name: " + s + " score " + rank.m_score + "\n");
          }
          else Console.WriteLine("No FR \n");
        }
        //////////////////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////////////////
        public void PrintHeadData(uint frame_number, int head_index, IntPtr p_head)
        {
          //Console.WriteLine("*** PrintHeadData ***");
          CX_HeadInfo hi = new CX_HeadInfo(); ;
          if (0 ==CX_GetHeadInfo(p_head, ref hi, msg))
          {
            Console.WriteLine("Frame "+frame_number+": Failed to get head "+head_index+" info: " + msg);
          }
          else
          {
            PrintHeadInfo(ref hi);
            PrintHeadFR(p_head);
          }
        }
        //////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////
        public void PrintHeadListData(uint frame_number, IntPtr p_head_list, bool details)
        {
          //Console.WriteLine("*** PrintHeadListData ***");
          int n_heads = CX_GetHeadListSize(p_head_list, msg);
          if (n_heads < 0) Console.WriteLine("Frame " + frame_number+" Error: "+ msg);
          else
          {
            if (details)
            {
              Console.WriteLine("FRAME "+frame_number+", Tracking "+n_heads+" HEADS");
              for (int i = 0; i < n_heads; i++)
              {
                IntPtr p_head = CX_GetHead(p_head_list, i, msg);
                if (p_head == IntPtr.Zero) Console.WriteLine("Failed to get head "+i+" of "+n_heads+" heads: " + msg);
                else
                {
                  PrintHeadData(frame_number, i, p_head);
                }
              }
            }
            else
            {
              Console.Write("FRAME "+frame_number+", Tracking "+n_heads+" HEADS ");
              for (int i = 0; i < n_heads; i++)
              {
                IntPtr p_head = CX_GetHead(p_head_list, i, msg);
                if (p_head == IntPtr.Zero) Console.Write("Failed to get head "+i+" of "+n_heads+" heads:"+ msg);
                else
                {
                  CX_HeadInfo hi = new CX_HeadInfo();
                  if (1 == CX_GetHeadInfo(p_head, ref hi, msg))
                  {
                      Console.Write("Head ID:"+hi.m_head_id);
                  }
                }
              }
              Console.Write("\n");

            }
          }

        }
        //////////////////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////////////////
        // Simple unexpected stream termination call back
        void UnexpectedTerminationCallBack(int stream_type, IntPtr connection_info, IntPtr p_object)
        {
            string s = Marshal.PtrToStringAnsi(connection_info);
            Console.WriteLine("Unexpected stream termination, stream type = " + stream_type + ", connection info = " + s);
        }
        //////////////////////////////////////////////////////////////////////////////////////


        ////////////////////////////////////////////////////////////////////
        // simple end of media file stream callback
        private void EndOfMediaFileCallBack(uint frame_num, IntPtr p_object)
        {
            Console.WriteLine("\nEND OF MEDIA FILE Callback, frame_num = " + frame_num);
            g_media_has_finished = true;
        }
        ////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////////////////
        // Simple frame call back
        private void VideoFrameCallBack(IntPtr p_head_list, uint head_list_size, IntPtr p_pixels, uint rows, uint cols, uint frame_num, IntPtr p_object)
        {
          StringBuilder vmsg = new StringBuilder(1024);

          // just print num of heads being tracked
          int n_heads = CX_GetHeadListSize(p_head_list, vmsg);
          if (n_heads < 0) Console.WriteLine("Frame " + frame_num + " Error: " + vmsg);
          else
          {
              //Console.WriteLine("FRAME " + frame_num + ", Tracking " + n_heads + " HEADS rows = " + rows + " cols = " + cols);
              // or print more detailed info (including FR if switched on)
              PrintHeadListData(frame_num, p_head_list, true);
          }

          //////////////////////////////////////////////////////////
          // Example of how to save the frame image (or any Aureus Image)
          // The Frame is always returned as a 32bppRGBA with bottom left origin, however 
          // when using a C# Bitmap, the Bitmap object assumes that it is stored in 
          // memory as BGRA or BGR, with the origin top left, hence we use CX_CopyImage
          // to provide a fast copy to the format we want (fastest will be BGRA as
          // CX_CopyImage can copy whole rows at a time
          //
          // Note: This will dramatically slow down the video processing since we are 
          // constructing and saving an image to disk
          //
          bool save_images = false; // change this to save the frames
          if (save_images)
          {
              int stride = (int)((cols * 4) + ((cols * 4) % 4));
              Bitmap im = new Bitmap((int)cols, (int)rows, PixelFormat.Format32bppRgb); // 4 bytes per pixel
              //Bitmap im = new Bitmap((int)cols, (int)rows, PixelFormat.Format24bppRgb); // 3 bytes per pixel

              // lock the bits as we are going to allow CX_CopyImage to fill the bitmap
              Rectangle rect = new Rectangle(0, 0, im.Width, im.Height);
              BitmapData bmpData = im.LockBits(rect, ImageLockMode.ReadWrite, im.PixelFormat);

              // Get the pointer to the first line, hence first pixel
              IntPtr ptr = bmpData.Scan0;

              // construct a CX_RAM_Image struct
              CX_RAM_Image rim = new CX_RAM_Image();
              rim.mp_pixels = p_pixels; // frame pixels
              rim.m_rows = rows;
              rim.m_cols = cols;
              rim.m_origin = 1; // frames always have bottom left origin
              rim.m_type = 1; // frames are always RGBA

              // select desired output type
              int dst_type = 4; // BGRA
              //int dst_type = 3; // BGR

              int dst_origin = 0; // we want origin at top left

              if (0 == CX_CopyImage(ref rim, dst_type, dst_origin, ptr, msg))
              {
                  im.UnlockBits(bmpData);
                  Console.WriteLine(msg);
              }
              else
              {
                  im.UnlockBits(bmpData);
                  string odir = "C:/temp/Test"; // obviously change this to something suitable on your machine
                  System.IO.Directory.CreateDirectory(odir);
                  string oname = odir + "/Frame_";
                  oname += frame_num;
                  oname += ".jpg";
                  im.Save(oname);
              }
              //////////////////////////////////////////////////////////
          }
        }
        //////////////////////////////////////////////////////////////////////////////////////



        //////////////////////////////////////////////////////////////
        public void TestMediaFileStream()
        {
            StringBuilder video_file = new StringBuilder("C:/CyberExtruder/Aureus/Videos/Doorway4.wmv");
            Console.WriteLine("Processing media file stream: \n" + video_file);

            // set a frame callback so we can capture the results
            if (0 == CX_SetFrameCallBack(mp_video, VideoFrameCallBack, IntPtr.Zero, msg))
            {
                Console.WriteLine(msg);
                return;
            }

            // set a media file finished callback so we know when to stop
            if (0 == CX_SetMediaFileFinishedCallBack(mp_video, EndOfMediaFileCallBack, IntPtr.Zero, msg))
            {
                Console.WriteLine( msg);
                return;
            };

            // set an unexpected termination callback (just to demonstrate, since it's a 
            // media file it's very unlikely to terminate unexpectedly)
            if (0 == CX_SetStreamTerminatedCallBack(mp_video, UnexpectedTerminationCallBack, IntPtr.Zero, msg))
            {
                Console.WriteLine(msg);
                return;
            }

            int frame_interval = 1;
            if (0 == CX_SetFrameInterval(mp_video, frame_interval, msg))
            {
                Console.WriteLine(msg);
                return;
            }
            frame_interval = CX_GetFrameInterval(mp_video, msg);
            Console.WriteLine("Frame Interval = " + frame_interval);

            int frame_reduction_step = 2;
            if (0 == CX_SetFrameReductionStep(mp_video, frame_reduction_step, msg))
            {
                Console.WriteLine(msg);
                return;
            }
            frame_reduction_step = CX_GetFrameReductionStep(mp_video, msg);
            Console.WriteLine("Frame Reduction step = " + frame_reduction_step);

            float top = 0.0f;
            float left = 0.0f;
            float height = 1.0f;
            float width = 1.0f;
            float minh = 0.2f;
            float maxh = 1.0f;
            if (0==CX_SetDetectionParameters(mp_video, top,left,height,width,minh,maxh,msg))
            {
                Console.WriteLine(msg);
                return;
            }

            // Process the stream
            g_media_has_finished = false; // initialize finished flag
            uint stream_type = 0; // Media file
            if (0 == CX_ProcessStream(mp_video, stream_type, video_file, msg))
            {
                Console.WriteLine(msg);
                return;
            }

            // spin whilst waiting for the media file
            // to finish processing
            while (true)
            {
                if (g_media_has_finished) break;
                else
                {
                    Thread.Sleep(500); // sleep for a while before checking
                }
            }
            Console.WriteLine("FINISHED SPINNING, detected end of media file processing");


            // this is not necessary, but it demonstrates that
            // there are no more frames being processed
            // if you want to prove this, uncomment this sleep call
            //Thread.Sleep(2000);


            // Stop processing
            if (0 == CX_StopStream(mp_video, msg))
            {
                Console.WriteLine(msg);
                return;
            }

        }
        //////////////////////////////////////////////////////////////



        //////////////////////////////////////////////////////////////
        public void TestDetectHeads()
        {
            // as an example we are going to load an image from the example sequence
            //string image_file = "C:/CyberExtruder/Aureus/SequentialFrames/Doorway4/M001.jpg";
            //string image_file = "D:/CyberBase/data/Gallery/Tygart/Roby/bounding_box/A.jpg";
            string image_file = "D:/CyberBase/data/Gallery/Tygart/Roby/bounding_box/B.jpg";
            //string image_file = "D:/CyberBase/data/Gallery/Tygart/Roby/bounding_box/C.bmp";
            Console.WriteLine("Detecting heads in image: \n" + image_file);

            // set detection parameters
            float top = 0.0f;
            float left = 0.0f;
            float height = 1.0f;
            float width = 1.0f;
            float minh = 0.2f;
            float maxh = 1.0f;
            if (0 == CX_SetDetectionParameters(mp_video, top, left, height, width, minh, maxh, msg))
            {
                Console.WriteLine(msg);
                return;
            }

            // set frame reduction step
            int frame_reduction_step = 1;
            if (0 == CX_SetFrameReductionStep(mp_video, frame_reduction_step, msg))
            {
                Console.WriteLine(msg);
                return;
            }
            frame_reduction_step = CX_GetFrameReductionStep(mp_video, msg);
            Console.WriteLine("Frame Reduction step = " + frame_reduction_step);

            // data to store detected head
            CX_HeadInfo hi = new CX_HeadInfo();

            // load the image
            Bitmap im = new Bitmap(image_file);

            // lock the bits as we are going to pass it into Aureus
            Rectangle rect = new Rectangle(0, 0, im.Width, im.Height);
            BitmapData bmpData = im.LockBits(rect, ImageLockMode.ReadWrite, im.PixelFormat);

            // construct a CX_RAM_Image struct
            CX_RAM_Image rim = new CX_RAM_Image();
            rim.mp_pixels = bmpData.Scan0; // the pointer to the first line, hence first pixel
            rim.m_rows = (uint)im.Height;
            rim.m_cols = (uint)im.Width;
            rim.m_origin = 0; // C# loaded Bitmaps usually have a top left origin


            // set the RAM image pixel format (we only use 24 & 32 bit here)
            bool format_ok = true;
            if (im.PixelFormat.Equals(PixelFormat.Format24bppRgb)) rim.m_type = 3; // BGR
            else if (im.PixelFormat.Equals(PixelFormat.Format32bppArgb)) rim.m_type = 4; // BGRA
            else if (im.PixelFormat.Equals(PixelFormat.Format32bppPArgb)) rim.m_type = 4; // BGRA
            else if (im.PixelFormat.Equals(PixelFormat.Format32bppRgb)) rim.m_type = 4; // BGRA
            //if (im.PixelFormat.Equals(PixelFormat.Format24bppRgb)) rim.m_type = 0; // RGB
            //else if (im.PixelFormat.Equals(PixelFormat.Format32bppArgb)) rim.m_type = 1; // RGBA
            //else if (im.PixelFormat.Equals(PixelFormat.Format32bppPArgb)) rim.m_type = 1; // RGBA
            //else if (im.PixelFormat.Equals(PixelFormat.Format32bppRgb)) rim.m_type = 1; // RGBA
            else format_ok = false;

            if (!format_ok)
            {
                Console.WriteLine("ERROR! can't use this format");
                return;
            }

            // detect the heads
            int use_face_detector = 1;
            int use_fail_safe = 1;
            int n = CX_DetectHeads(mp_video, ref rim, 1, ref hi, 1, use_face_detector, use_fail_safe, msg);


            string txt = "Detected " + n.ToString() + " Heads";
            Console.WriteLine(txt);



            if (n>0)
            {
                // here we convert to pixels with origin top left to easily
                // enable us to see where on the image the detection occurs
                if (hi.m_face_ok)
                {
                    // swap origin
                    hi.m_face_bl_y = 1.0f - hi.m_face_bl_y;
                    hi.m_face_tr_y = 1.0f - hi.m_face_tr_y;
                    // convert to pixels
                    hi.m_face_bl_x *= im.Width;
                    hi.m_face_bl_y *= im.Height;
                    hi.m_face_tr_x *= im.Width;
                    hi.m_face_tr_y *= im.Height;
                }
                if (hi.m_head_ok)
                {
                    // swap origin
                    hi.m_head_bl_y = 1.0f - hi.m_head_bl_y;
                    hi.m_head_tr_y = 1.0f - hi.m_head_tr_y;
                    // convert to pixels
                    hi.m_head_bl_x *= im.Width;
                    hi.m_head_bl_y *= im.Height;
                    hi.m_head_tr_x *= im.Width;
                    hi.m_head_tr_y *= im.Height;
                }
                PrintHeadInfo(ref hi);
            }

        }
        //////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////
        public void SetUpVideoFR(bool use_fr)
        {
            int perform_gallery = 0;
            int generate_templates = 0;
            if (use_fr)
            {
                perform_gallery = 1;
                generate_templates = 1;
            }
            // set the gallery matching flag
            if (0 == CX_SetPerformGalleryFRFlag(mp_video, perform_gallery, msg))
            {
                Console.WriteLine(msg);
            }
            // also set the template generation flag
            else if (0 == CX_SetGenerateTemplatesFlag(mp_video, generate_templates, msg))
            {
                Console.WriteLine(msg);
            }
            else Console.WriteLine("Set Perform gallery and generate templates flags OK");
        }
        //////////////////////////////////////////////////////////////




        //////////////////////////////////////////////////////////////
        // constructor, keeps a copy of the Aureus pointer and creates 
        // the video object ready for using
        public AureusVideo(IntPtr p_aureus)
        {
            mp_aureus = p_aureus;

            Console.WriteLine("Creating video object");
            mp_video = CX_CreateVideo(mp_aureus, msg);
            if (mp_video == IntPtr.Zero)
            {
                Console.WriteLine("Failed to create video object: " + msg);
            }
            else
            {
                Console.WriteLine("Created Video OK");
            }
        }
        //////////////////////////////////////////////////////////////
    }
}
