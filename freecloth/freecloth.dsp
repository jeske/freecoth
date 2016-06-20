# Microsoft Developer Studio Project File - Name="freecloth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=freecloth - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "freecloth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freecloth.mak" CFG="freecloth - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freecloth - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "freecloth - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freecloth - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /I "c:/bin/dev/boost_1_28_0" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "freecloth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "c:/bin/dev/boost_1_28_0" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "freecloth - Win32 Release"
# Name "freecloth - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\base\baMath.cpp
# End Source File
# Begin Source File

SOURCE=.\base\baStringUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\base\baTimeWindows.cpp
# End Source File
# Begin Source File

SOURCE=.\base\baTraceEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\base\baTraceStream.cpp
# SUBTRACT CPP /I "c:/bin/dev/boost_1_28_0"
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix3.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix4.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\geMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshWingedEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\gePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\geom\geVector.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcBase.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcProxyShdPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcShdPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistryFile.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistryWindows.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategyAdaptive.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategyBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\simulator\simVector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\base\baMath.h
# End Source File
# Begin Source File

SOURCE=.\base\baMath.inline.h
# End Source File
# Begin Source File

SOURCE=.\base\baStringUtil.h
# End Source File
# Begin Source File

SOURCE=.\base\baTime.h
# End Source File
# Begin Source File

SOURCE=.\base\baTime.inline.h
# End Source File
# Begin Source File

SOURCE=.\base\baTraceEntry.h
# End Source File
# Begin Source File

SOURCE=.\base\baTraceStream.h
# End Source File
# Begin Source File

SOURCE=.\base\config.h
# End Source File
# Begin Source File

SOURCE=.\base\ctype.h
# End Source File
# Begin Source File

SOURCE=.\base\debug.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix3.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix3.inline.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix4.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMatrix4.inline.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMesh.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMesh.inline.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshBuilder.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshTypes.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshWingedEdge.h
# End Source File
# Begin Source File

SOURCE=.\geom\geMeshWingedEdge.inline.h
# End Source File
# Begin Source File

SOURCE=.\geom\gePoint.h
# End Source File
# Begin Source File

SOURCE=.\geom\gePoint.inline.h
# End Source File
# Begin Source File

SOURCE=.\geom\geVector.h
# End Source File
# Begin Source File

SOURCE=.\geom\geVector.inline.h
# End Source File
# Begin Source File

SOURCE=.\base\GL_gl.h
# End Source File
# Begin Source File

SOURCE=.\base\GL_glu.h
# End Source File
# Begin Source File

SOURCE=.\base\GL_glut.h
# End Source File
# Begin Source File

SOURCE=.\base\glui.h
# End Source File
# Begin Source File

SOURCE=.\base\limits.h
# End Source File
# Begin Source File

SOURCE=.\base\math.h
# End Source File
# Begin Source File

SOURCE=.\base\package.h
# End Source File
# Begin Source File

SOURCE=.\geom\package.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\package.h
# End Source File
# Begin Source File

SOURCE=.\simulator\package.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcBase.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcProxyShdPtr.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcProxyShdPtr.inline.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcShdPtr.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\rcShdPtr.inline.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfig.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistry.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistry.imp.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistryFile.h
# End Source File
# Begin Source File

SOURCE=.\resmgt\resConfigRegistryWindows.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simMatrix.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simMatrix.inline.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simSimulator.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategy.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategyAdaptive.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simStepStrategyBasic.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simVector.h
# End Source File
# Begin Source File

SOURCE=.\simulator\simVector.inline.h
# End Source File
# Begin Source File

SOURCE=.\base\stdio.h
# End Source File
# Begin Source File

SOURCE=.\base\stdlib.h
# End Source File
# Begin Source File

SOURCE=.\base\types.h
# End Source File
# Begin Source File

SOURCE=.\base\windows.h
# End Source File
# End Group
# End Target
# End Project
