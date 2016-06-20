# Microsoft Developer Studio Project File - Name="clothApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=clothApp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "clothApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clothApp.mak" CFG="clothApp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clothApp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "clothApp - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clothApp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I ".." /I "c:/bin/dev/boost_1_28_0" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 glui.lib glut32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib freecloth.lib /nologo /subsystem:console /machine:I386 /libpath:./release
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "clothApp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib freecloth.lib glut32.lib glui.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /LIBPATH:./debug
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "clothApp - Win32 Release"
# Name "clothApp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\clothApp\clothApp.cpp
# End Source File
# Begin Source File

SOURCE=.\clothApp\clothAppConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\colour\colColourRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGL.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindowGLUI.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindowGLUT.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImage.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageReader.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageReaderPNM.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageWriterPNM.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxWindowObserver.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\clothApp\clothApp.h
# End Source File
# Begin Source File

SOURCE=.\clothApp\clothAppConfig.h
# End Source File
# Begin Source File

SOURCE=.\colour\colColourRGB.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxConfig.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGL.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLTexture.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindow.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindowGLUI.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxGLWindowGLUT.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImage.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImage.inline.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageReader.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageReaderPNM.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageWriter.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxImageWriterPNM.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxWindow.h
# End Source File
# Begin Source File

SOURCE=.\gfx\gfxWindowObserver.h
# End Source File
# Begin Source File

SOURCE=.\clothApp\package.h
# End Source File
# Begin Source File

SOURCE=.\colour\package.h
# End Source File
# Begin Source File

SOURCE=.\gfx\package.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
