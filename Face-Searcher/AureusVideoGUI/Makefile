#==============================================================
#============= my home directory ==============================
TCPDIR = /home/tim
BASE = $(TCPDIR)/Beast/D/CyberBase/src/code2013
THIRDPARTY = $(TCPDIR)/Beast/D/CyberBase/src/ThirdParty
#==============================================================



#============= C++ compiler ===================================
CC = g++
#==============================================================


#=========== objects to make the example program =================
OBJS = AureusVideoGUI.o \
cxutils.o \
CX_EnrollPanel.o \
CX_FRList.o \
CX_GalleryInfoPanel.o \
CX_GalleryList.o \
CX_GalleryPanel.o \
CX_Icons.o \
CX_ImageGrid.o \
CX_ImageList.o \
CX_ImageWindow.o \
CX_PersonImageList.o \
CX_ResultsPanel.o \
CX_UpdateTimer.o \
CX_VideoDialog.o \
CX_VideoDisplay.o \
CX_VideoPanel.o \
FDPdlg.o
#==============================================================


#================ some base folders etc =======================
# change these to suit your system
BASE = ../
#==============================================================


#================= includes & compiler options ================
# include path to Aureus FR SDK
AUREUS_INC = -I$(BASE)/Aureus

# local include path
AUREUS_VIDEOGUI_INC = -I$(BASE)/AureusVideoGUI

# include path to jpeg code
JPEG = -I$(BASE)/jpeg

# include path to wxWidgets GUI code
WX_INC_1 = -I$(BASE)/wxWidgets/include/gtk2
WX_INC_2 = -I$(BASE)/wxWidgets/include

INC = $(AUREUS_INC) $(AUREUS_VIDEOGUI_INC) $(JPEG) $(WX_INC_1) $(WX_INC_2) -pthread

#CPPFLAGS = $(INC) -DNDEBUG -DBOOST_DISABLE_ASSERTS -DOS_LINUX -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_UNICODE -DwxUSE_GLCANVAS -std=gnu++11 -fPIC -m64
#CPPFLAGS = -DNDEBUG -DBOOST_DISABLE_ASSERTS -DOS_LINUX -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_UNICODE -DwxUSE_GLCANVAS -std=gnu++11 -fPIC -m64 -fopenmp $(INC) 
CPPFLAGS = -DNDEBUG -DBOOST_DISABLE_ASSERTS -DOS_LINUX -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_UNICODE -DwxUSE_GLCANVAS -std=gnu++11 -fPIC -m64 $(INC) 
#==============================================================


#==============================================================
# we tell the linker to look in output and local folders
# this also ensures the executable can find the SDK
LDFLAGS = -L$(BASE)/lib -L$(BASE)/wxWidgets/libs/gtk2 -L. "-Wl,-rpath,./"
#==============================================================


#==============================================================
# libAureus.so is the Aureus Video processing and FR SDK
# we need the jpeg library as we load jpeg images in the
# example code, we also need the wxWidgets GUI libs and OpenGL

EXTRALIBS_GUI =  -lwx_gtk2u_xrc-3.0 -lwx_gtk2u_qa-3.0 -lwx_baseu_net-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu-3.0 -lgthread-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 -lfreetype -lpng -lexpat -lwxregexu-3.0 -lwxtiff-3.0 -lwxjpeg-3.0 -lwx_gtk2u_gl-3.0 -lz -ldl -lm

LDLIBS = -lAureus -ljpeg -lboost_system -lboost_filesystem -lboost_thread -ltbb $(EXTRALIBS_GUI) -lglut -lGLU -lGL
#==============================================================


.KEEPSTATE:

# we make the target in the same folder as the SDK
all: $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(BASE)/lib/AureusVideoGUI $(OBJS) $(LDLIBS)


