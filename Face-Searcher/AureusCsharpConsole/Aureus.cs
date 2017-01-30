using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;



namespace AureusCsharpConsole
{
    //////////////////////////////////////////////////////////////
    // a class to create and initialize Aureus
    class Aureus
    {
        const string aureus_path = "C:/CyberExtruder/Aureus/Aureus.dll"; // the path to Aureus (change to your install path)
        public IntPtr mp_aureus; // the Aureus object pointer
        public StringBuilder msg = new StringBuilder(1024); // a message holder (Aureus will fill this with infomation)

        //////////////////////////////////////////////////////////////
        // Some of the Aureus interface functions (see the Aureus header files for more info)
        [DllImport(aureus_path, EntryPoint = "CX_GetMachineID", CharSet = CharSet.Ansi)]
        public static extern int CX_GetMachineID([Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_CreateAureus", CharSet = CharSet.Ansi)]
        public static extern IntPtr CX_CreateAureus([Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_FreeAureus", CharSet = CharSet.Ansi)]
        public static extern int CX_FreeAureus(IntPtr p_aureus, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_Initialize", CharSet = CharSet.Ansi)]
        public static extern int CX_Initialize(IntPtr p_aureus, int load_gallery, StringBuilder dir, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetLicenseInfo", CharSet = CharSet.Ansi)]
        public static extern int CX_GetLicenseInfo(IntPtr p_aureus, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetVersion", CharSet = CharSet.Ansi)]
        public static extern int CX_GetVersion(IntPtr p_aureus, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetLicenseParameters", CharSet = CharSet.Ansi)]
        public static extern int CX_GetLicenseParameters(IntPtr p_aureus, ref int num_video_streams, ref int image_enabled, ref int fr_enabled, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetNumFRengines", CharSet = CharSet.Ansi)]
        public static extern int CX_GetNumFRengines(IntPtr p_aureus, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_SelectFRengine", CharSet = CharSet.Ansi)]
        public static extern int CX_SelectFRengine(IntPtr p_aureus, int index, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_GetFRname", CharSet = CharSet.Ansi)]
        public static extern IntPtr CX_GetFRname(IntPtr p_aureus, int index, [Out] StringBuilder message);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void UpdateFRCallback(IntPtr fr_msg, int total, int current, IntPtr objectPtr);

        [DllImport(aureus_path, EntryPoint = "CX_SetUpdateFRCallBack", CharSet = CharSet.Ansi)]
        public static extern int CX_SetUpdateFRCallBack(IntPtr p_aureus, UpdateFRCallback callback, IntPtr objPtr, [Out] StringBuilder message);

        [DllImport(aureus_path, EntryPoint = "CX_UpdateFR", CharSet = CharSet.Ansi)]
        public static extern int CX_UpdateFR(IntPtr p_aureus, [Out] StringBuilder message);

        //////////////////////////////////////////////////////////////
        // this call back simply prints info to std out about the state of updating the
        // gallery, total and current will be -1 after all updates have finished. This
        // is provided by Aureus so that you can provide visual feedback of the status
        // of the gallery update process
        private void FRcallback(IntPtr fr_msg, int total, int current, IntPtr objectPtr)
        {
            string s = Marshal.PtrToStringAnsi(fr_msg);
            Console.WriteLine("FRcallback: " + s + " " + current + "/" + total);
        }
        //////////////////////////////////////////////////////////////
        // this tells sets the Aureus FR update callback to the function above
        public void SetFRUpdateCallBack()
        {
            if (0 == CX_SetUpdateFRCallBack(mp_aureus, FRcallback, IntPtr.Zero, msg))
            {
                Console.WriteLine(msg);
            }
            else
            {
                Console.WriteLine("Set FR Update Callback OK");
                int ret = CX_UpdateFR(mp_aureus, msg);
                if (ret < 0) Console.WriteLine(msg);
                else Console.WriteLine("Update FR returned " + ret);
            }
        }
        //////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////
        // some function wrappers that provide FR info and selection
        public int GetNumFRengines()
        {
            int n = CX_GetNumFRengines(mp_aureus, msg);
            if (n < 0) Console.WriteLine(msg);
            return n;
        }
        public bool SelectFRengine(int index)
        {
            if (0==CX_SelectFRengine(mp_aureus,index,msg))
            {
                Console.WriteLine(msg);
                return false;
            }
            return true;
        }
        public string GetFRname(int index)
        {
            string s = "Unknown";
            IntPtr p = CX_GetFRname(mp_aureus,index,msg);
            if (p == IntPtr.Zero)
            {
                Console.WriteLine(msg);
                return s;
            }
            s = Marshal.PtrToStringAnsi(p);
            return s;
        }
        //////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////
        public int GetVersion([Out] StringBuilder message)
        {
            return CX_GetVersion(mp_aureus, message);
        }
        //////////////////////////////////////////////////////////////
        public void PrintLicenseParams()
        {
            int num_video_streams = 0;
            int image_enabled = 0;
            int fr_enabled = 0;
            if (0 == CX_GetLicenseParameters(mp_aureus, ref num_video_streams, ref image_enabled, ref fr_enabled, msg))
            {
                Console.WriteLine("Failed to get license params: " + msg);
            }
            else
            {
                Console.WriteLine("Got license params");
                Console.WriteLine("num_video_streams = " + num_video_streams);
                Console.WriteLine("image_enabled = " + image_enabled);
                Console.WriteLine("fr_enabled = " + fr_enabled);
            }
        }
        //////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////
        // creates Aureus and initializes it.
        public bool LaunchAureus()
        {
            //StringBuilder msg = new StringBuilder(1024);
            int val = CX_GetMachineID(msg);
            Console.WriteLine("C# MachineID: " + msg);

            //IntPtr p_aureus = CX_CreateAureus(msg);
            mp_aureus = CX_CreateAureus(msg);
            if (mp_aureus == IntPtr.Zero) { Console.WriteLine(msg); return false; }
            Console.WriteLine("Created Aureus");

            StringBuilder install_dir = new StringBuilder("C:/CyberExtruder/Aureus");
            int load_gallery = 1;
            if (1 == CX_Initialize(mp_aureus, load_gallery, install_dir, msg))
            {
                Console.WriteLine("Initialized Aureus");
                if (0 == CX_GetLicenseInfo(mp_aureus, msg)) Console.WriteLine(msg);
                else Console.WriteLine(msg);
            }
            else
            {
                Console.WriteLine("Initialization Failed: " + msg);
                return false;
            }
            return true;
        }
        //////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////
        // frees Aureus when we are done with it
        public void FreeAureus()
        {
            if (mp_aureus != IntPtr.Zero)
            {
                if (0 == CX_FreeAureus(mp_aureus, msg)) { Console.WriteLine(msg); return; }
                Console.WriteLine("Freed Aureus");
            }
        }
        //////////////////////////////////////////////////////////////
    }
}
