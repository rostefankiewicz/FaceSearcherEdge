using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace AureusCsharpConsole
{

    class Program
    {
        //////////////////////////////////////////////////////////////
        static Aureus m_ar;
        static bool use_fr = true; // set to false if nor using FR
        //////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////
        public static bool SetUpFR(bool use_fr)
        {
            Console.WriteLine("Setting up FR...");
            if (use_fr)
            {
                int n_engines = m_ar.GetNumFRengines();
                if (n_engines < 0) { m_ar.FreeAureus(); return false; }

                Console.WriteLine("There are " + n_engines + " FR Engines available");
                for (int i=0; i<n_engines; i++)
                {
                    string s = m_ar.GetFRname(i);
                    Console.WriteLine("Engine number " + i + " = " + s);
                }
                if (n_engines==0)
                {
                    Console.WriteLine("No FR Engines, FR will not be performed!");
                }
                else
                {
                    // we'll select the last engine as it tends to be the best
                    int engine_num = n_engines - 1;
                    if (!m_ar.SelectFRengine(engine_num))
                    {
                        Console.WriteLine("Failed to select engine number " + engine_num);
                        m_ar.FreeAureus();
                        return false;
                    }

                    m_ar.SetFRUpdateCallBack();
                }
            }
            return true;
        }
        //////////////////////////////////////////////////////////////




        //////////////////////////////////////////////////////////////
        static void Main(string[] args)
        {
            StringBuilder msg = new StringBuilder(1024);

            m_ar = new Aureus();
            if (!m_ar.LaunchAureus()) Console.WriteLine("Launch Failed");

            // Successful launch
            else
            {
                Console.WriteLine("Launch Successfull");
                if (0 == m_ar.GetVersion(msg)) Console.WriteLine(msg);
                else Console.WriteLine("Version = " + msg );

                m_ar.PrintLicenseParams();

                Console.WriteLine("------------------------");
                if (!SetUpFR(use_fr)) { m_ar.FreeAureus(); return;  };

                Console.WriteLine("------------------------");
                AureusVideo vid = new AureusVideo(m_ar.mp_aureus);

                ////////////////////////////////////////
                // detecting heads
                vid.TestDetectHeads();
                ////////////////////////////////////////

                ////////////////////////////////////////
                // processing a media file stream
                // set up FR flags
                //vid.SetUpVideoFR(use_fr);
                // test processing a video stream from a media file
                //vid.TestMediaFileStream();
                ////////////////////////////////////////


            }

            Console.WriteLine("------------------------");
            m_ar.FreeAureus();
        }
        //////////////////////////////////////////////////////////////
    }
}
