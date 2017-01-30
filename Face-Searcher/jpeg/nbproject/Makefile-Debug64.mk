#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cdjpeg.o \
	${OBJECTDIR}/jcapimin.o \
	${OBJECTDIR}/jcapistd.o \
	${OBJECTDIR}/jccoefct.o \
	${OBJECTDIR}/jccolor.o \
	${OBJECTDIR}/jcdctmgr.o \
	${OBJECTDIR}/jchuff.o \
	${OBJECTDIR}/jcinit.o \
	${OBJECTDIR}/jcmainct.o \
	${OBJECTDIR}/jcmarker.o \
	${OBJECTDIR}/jcmaster.o \
	${OBJECTDIR}/jcomapi.o \
	${OBJECTDIR}/jcparam.o \
	${OBJECTDIR}/jcphuff.o \
	${OBJECTDIR}/jcprepct.o \
	${OBJECTDIR}/jcsample.o \
	${OBJECTDIR}/jctrans.o \
	${OBJECTDIR}/jdapimin.o \
	${OBJECTDIR}/jdapistd.o \
	${OBJECTDIR}/jdatadst.o \
	${OBJECTDIR}/jdatasrc.o \
	${OBJECTDIR}/jdcoefct.o \
	${OBJECTDIR}/jdcolor.o \
	${OBJECTDIR}/jddctmgr.o \
	${OBJECTDIR}/jdhuff.o \
	${OBJECTDIR}/jdinput.o \
	${OBJECTDIR}/jdmainct.o \
	${OBJECTDIR}/jdmarker.o \
	${OBJECTDIR}/jdmaster.o \
	${OBJECTDIR}/jdmerge.o \
	${OBJECTDIR}/jdphuff.o \
	${OBJECTDIR}/jdpostct.o \
	${OBJECTDIR}/jdsample.o \
	${OBJECTDIR}/jdtrans.o \
	${OBJECTDIR}/jerror.o \
	${OBJECTDIR}/jfdctflt.o \
	${OBJECTDIR}/jfdctfst.o \
	${OBJECTDIR}/jfdctint.o \
	${OBJECTDIR}/jidctflt.o \
	${OBJECTDIR}/jidctfst.o \
	${OBJECTDIR}/jidctint.o \
	${OBJECTDIR}/jidctred.o \
	${OBJECTDIR}/jmemmgr.o \
	${OBJECTDIR}/jmemnobs.o \
	${OBJECTDIR}/jquant1.o \
	${OBJECTDIR}/jquant2.o \
	${OBJECTDIR}/jutils.o \
	${OBJECTDIR}/memdst.o \
	${OBJECTDIR}/memsrc.o \
	${OBJECTDIR}/rdbmp.o \
	${OBJECTDIR}/rdgif.o \
	${OBJECTDIR}/rdppm.o \
	${OBJECTDIR}/rdrle.o \
	${OBJECTDIR}/rdtarga.o \
	${OBJECTDIR}/wrbmp.o \
	${OBJECTDIR}/wrgif.o \
	${OBJECTDIR}/wrppm.o \
	${OBJECTDIR}/wrrle.o \
	${OBJECTDIR}/wrtarga.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a

${OBJECTDIR}/cdjpeg.o: cdjpeg.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cdjpeg.o cdjpeg.c

${OBJECTDIR}/jcapimin.o: jcapimin.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcapimin.o jcapimin.c

${OBJECTDIR}/jcapistd.o: jcapistd.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcapistd.o jcapistd.c

${OBJECTDIR}/jccoefct.o: jccoefct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jccoefct.o jccoefct.c

${OBJECTDIR}/jccolor.o: jccolor.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jccolor.o jccolor.c

${OBJECTDIR}/jcdctmgr.o: jcdctmgr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcdctmgr.o jcdctmgr.c

${OBJECTDIR}/jchuff.o: jchuff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jchuff.o jchuff.c

${OBJECTDIR}/jcinit.o: jcinit.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcinit.o jcinit.c

${OBJECTDIR}/jcmainct.o: jcmainct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcmainct.o jcmainct.c

${OBJECTDIR}/jcmarker.o: jcmarker.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcmarker.o jcmarker.c

${OBJECTDIR}/jcmaster.o: jcmaster.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcmaster.o jcmaster.c

${OBJECTDIR}/jcomapi.o: jcomapi.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcomapi.o jcomapi.c

${OBJECTDIR}/jcparam.o: jcparam.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcparam.o jcparam.c

${OBJECTDIR}/jcphuff.o: jcphuff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcphuff.o jcphuff.c

${OBJECTDIR}/jcprepct.o: jcprepct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcprepct.o jcprepct.c

${OBJECTDIR}/jcsample.o: jcsample.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jcsample.o jcsample.c

${OBJECTDIR}/jctrans.o: jctrans.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jctrans.o jctrans.c

${OBJECTDIR}/jdapimin.o: jdapimin.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdapimin.o jdapimin.c

${OBJECTDIR}/jdapistd.o: jdapistd.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdapistd.o jdapistd.c

${OBJECTDIR}/jdatadst.o: jdatadst.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdatadst.o jdatadst.c

${OBJECTDIR}/jdatasrc.o: jdatasrc.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdatasrc.o jdatasrc.c

${OBJECTDIR}/jdcoefct.o: jdcoefct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdcoefct.o jdcoefct.c

${OBJECTDIR}/jdcolor.o: jdcolor.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdcolor.o jdcolor.c

${OBJECTDIR}/jddctmgr.o: jddctmgr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jddctmgr.o jddctmgr.c

${OBJECTDIR}/jdhuff.o: jdhuff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdhuff.o jdhuff.c

${OBJECTDIR}/jdinput.o: jdinput.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdinput.o jdinput.c

${OBJECTDIR}/jdmainct.o: jdmainct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdmainct.o jdmainct.c

${OBJECTDIR}/jdmarker.o: jdmarker.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdmarker.o jdmarker.c

${OBJECTDIR}/jdmaster.o: jdmaster.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdmaster.o jdmaster.c

${OBJECTDIR}/jdmerge.o: jdmerge.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdmerge.o jdmerge.c

${OBJECTDIR}/jdphuff.o: jdphuff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdphuff.o jdphuff.c

${OBJECTDIR}/jdpostct.o: jdpostct.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdpostct.o jdpostct.c

${OBJECTDIR}/jdsample.o: jdsample.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdsample.o jdsample.c

${OBJECTDIR}/jdtrans.o: jdtrans.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jdtrans.o jdtrans.c

${OBJECTDIR}/jerror.o: jerror.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jerror.o jerror.c

${OBJECTDIR}/jfdctflt.o: jfdctflt.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jfdctflt.o jfdctflt.c

${OBJECTDIR}/jfdctfst.o: jfdctfst.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jfdctfst.o jfdctfst.c

${OBJECTDIR}/jfdctint.o: jfdctint.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jfdctint.o jfdctint.c

${OBJECTDIR}/jidctflt.o: jidctflt.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jidctflt.o jidctflt.c

${OBJECTDIR}/jidctfst.o: jidctfst.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jidctfst.o jidctfst.c

${OBJECTDIR}/jidctint.o: jidctint.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jidctint.o jidctint.c

${OBJECTDIR}/jidctred.o: jidctred.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jidctred.o jidctred.c

${OBJECTDIR}/jmemmgr.o: jmemmgr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jmemmgr.o jmemmgr.c

${OBJECTDIR}/jmemnobs.o: jmemnobs.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jmemnobs.o jmemnobs.c

${OBJECTDIR}/jquant1.o: jquant1.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jquant1.o jquant1.c

${OBJECTDIR}/jquant2.o: jquant2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jquant2.o jquant2.c

${OBJECTDIR}/jutils.o: jutils.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jutils.o jutils.c

${OBJECTDIR}/memdst.o: memdst.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/memdst.o memdst.c

${OBJECTDIR}/memsrc.o: memsrc.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/memsrc.o memsrc.c

${OBJECTDIR}/rdbmp.o: rdbmp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rdbmp.o rdbmp.c

${OBJECTDIR}/rdgif.o: rdgif.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rdgif.o rdgif.c

${OBJECTDIR}/rdppm.o: rdppm.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rdppm.o rdppm.c

${OBJECTDIR}/rdrle.o: rdrle.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rdrle.o rdrle.c

${OBJECTDIR}/rdtarga.o: rdtarga.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rdtarga.o rdtarga.c

${OBJECTDIR}/wrbmp.o: wrbmp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wrbmp.o wrbmp.c

${OBJECTDIR}/wrgif.o: wrgif.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wrgif.o wrgif.c

${OBJECTDIR}/wrppm.o: wrppm.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wrppm.o wrppm.c

${OBJECTDIR}/wrrle.o: wrrle.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wrrle.o wrrle.c

${OBJECTDIR}/wrtarga.o: wrtarga.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DLINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wrtarga.o wrtarga.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjpeg.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
