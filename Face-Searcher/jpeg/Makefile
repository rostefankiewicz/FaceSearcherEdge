#==============================================================
TCPDIR = /home/tim
BASE = -I$(TCPDIR)/Beast/D/CyberBase/src/code2013
THIRDPARTY = -I$(TCPDIR)/Beast/D/CyberBase/src/ThirdParty
#==============================================================



#==============================================================
#============= C++ compiler ===================================
#CC = g++
CC = gcc
#==============================================================



#==============================================================
#================= X-Windows directories ======================
#==============================================================
#============ general x windows directories ===================
#XLIB = -L/usr/openwin/lib
#XINC = -I/usr/openwin/include
XLIB = 
XINC = 
#==============================================================



#==============================================================
CEBASE = $(BASE)/cebase
JPEG = $(BASE)/jpeg
PNG = $(BASE)/png
ZLIB = $(BASE)/zlib
TIFF = $(BASE)/tiff
LEVMAR = $(BASE)/levmar
CEFACE = $(BASE)/ceface
CELEARN = $(BASE)/celearn
CECASCADE = $(BASE)/cecascade
DLIB = $(THIRDPARTY)/dlib
BOOST = $(THIRDPARTY)/boost_1_57_0
MY_LIB_FOLDER = $(TCPDIR)/Beast/D/CyberBase/src/code2013/lib
TCPLIB = -L$(MY_LIB_FOLDER)
OTHER_LIBS = -ljpeg -llevmar -lpng -lzlib -lceface
#==============================================================





#==============================================================
#================= all includes ===============================
INC = $(CEBASE) $(JPEG) $(PNG) $(ZLIB) $(TIFF) \
$(LEVMAR) $(CEFACE) $(CELEARN) $(CECASCADE) $(DLIB) $(BOOST)
#==============================================================




#==============================================================
#============= compile line flags =============================
#DEBUG = -ggdb
DEBUG = 
#WARN = -Wall
WARN =
DEFS = -DLINUX -DNDEBUG
OPT = $(DEFS) -std=gnu++11 -fPIC -m64
#GUI = -Dwx_xview
GUI = -Dwx$(GUITYPE)
#==============================================================


#objects to make the ceface lib
OBJS = cdjpeg.o jcapimin.o jcapistd.o jccoefct.o jccolor.o jcdctmgr.o jchuff.o jcinit.o jcmainct.o \
jcmarker.o jcmaster.o jcomapi.o jcparam.o jcphuff.o jcprepct.o jcsample.o jctrans.o jdapimin.o \
jdapistd.o jdatadst.o jdatasrc.o jdcoefct.o jdcolor.o jddctmgr.o jdhuff.o jdinput.o jdmainct.o \
jdmarker.o jdmaster.o jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o jfdctflt.o \
jfdctfst.o jfdctint.o jidctflt.o jidctfst.o jidctint.o jidctred.o jmemmgr.o jmemnobs.o jquant1.o \
jquant2.o jutils.o memdst.o memsrc.o rdbmp.o rdgif.o rdppm.o rdrle.o rdtarga.o wrbmp.o wrgif.o \
wrppm.o wrrle.o wrtarga.o


#CPPFLAGS = $(INC) $(GUI) $(DEBUG) $(WARN) $(OPT)
CPPFLAGS = $(INC) $(DEBUG) $(WARN) $(OPT)

#LDFLAGS = $(XLIB) $(TCPLIB) $(WXLIB) $(RAD_LOC_LIB) -L. $(RAD_LIB)
LDFLAGS = $(TCPLIB) -L.

#LDLIBS = $(XGUILIBS) -lX11 -ltcp$(GUISUFFIX) -l$(wxlibrary) -lshapemod -linterp -lnumrec -lframes -lradial -llocal -lstdc++ -lm -lnsl -lsocket
LDLIBS = $(OTHER_LIBS) -lcebase -lstdc++ -lm -lnsl

.KEEPSTATE:

all: $(OBJS)
	ar -r $(MY_LIB_FOLDER)/libjpeg.a *.o


