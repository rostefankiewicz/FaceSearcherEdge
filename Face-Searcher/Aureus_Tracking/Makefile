#============= C++ compiler ===================================
CC = g++
#==============================================================


#=========== objects to make the test program =================
OBJS = cxutils.o Aureus_Tracking.o


#================ some base folders etc =======================
# change these to suit your system
BASE = ../


#================= includes & compiler options ================
# include path to Aureus FR SDK
AUREUS_INC = -I$(BASE)/Aureus

# local include path
AUREUS_TRACKING_INC = -I$(BASE)/Aureus_Tracking

# include path to jpeg code
JPEG = -I$(BASE)/jpeg

INC = $(AUREUS_INC) $(AUREUS_TRACKING_INC) $(JPEG) -pthread

CPPFLAGS = $(INC) -DNDEBUG -DBOOST_DISABLE_ASSERTS -std=gnu++11
#==============================================================



# we tell the linker to look in output and local folders
# this also ensures the executable can find the SDK
LDFLAGS = -L$(BASE)/lib -L. "-Wl,-rpath,./"

# libAureus.so is the Aureus Video processing and FR SDK
# we need the jpeg library as we load jpeg images in the
# example code
LDLIBS = -lAureus -ljpeg -lboost_system -lboost_filesystem -lboost_thread -ltbb

.KEEPSTATE:

# we make the target in the same folder as the SDK
all: $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(BASE)/lib/Aureus_Tracking $(OBJS) $(LDLIBS)


