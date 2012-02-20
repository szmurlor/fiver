; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Fiver
AppPublisher=IETiSIP - Warsaw University of Technology
AppVersion=2.0.03
DefaultDirName={pf}\Fiver
DefaultGroupName=Fiver
UninstallDisplayIcon={app}\fivermain.exe
Compression=lzma2
SolidCompression=yes
OutputDir=.

[Files]
Source: "win\fivermain.exe"; DestDir: "{app}"
Source: "win\boxmgr.dll"; DestDir: "{app}"
Source: "win\coremesh2.dll"; DestDir: "{app}"
Source: "win\fvattr.dll"; DestDir: "{app}"
Source: "win\libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "win\meshreader2.dll"; DestDir: "{app}"
Source: "win\mingwm10.dll"; DestDir: "{app}"
Source: "win\QGLViewer2.dll"; DestDir: "{app}"
Source: "win\QtCore4.dll"; DestDir: "{app}"
Source: "win\QtGui4.dll"; DestDir: "{app}"
Source: "win\QtOpenGL4.dll"; DestDir: "{app}"
Source: "win\QtXml4.dll"; DestDir: "{app}"
Source: "win\trivlog.dll"; DestDir: "{app}"
;Source: "examples\.kond_opt.field"; DestDir: "{app}\examples"
Source: "examples\kond_opt.grid"; DestDir: "{app}\examples"
;Source: "examples\kond_opt.dp"; DestDir: "{app}\examples"
Source: "examples\szmurlor2.grid"; DestDir: "{app}\examples"

[Icons]
Name: "{group}\Fiver 2.0.03"; Filename: "{app}\fivermain.exe"

[Registry]
Root: HKCU; Subkey: "Software\ZETiIS"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\ZETiIS\RSoft"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\ZETiIS\RSoft\FViewer"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\ZETiIS\RSoft\FViewer"; ValueType: string; ValueName: "RecentDir"; ValueData: "{app}\examples"
Root: HKCU; Subkey: "Software\ZETiIS\RSoft\FViewer"; ValueType: string; ValueName: "LastFilter"; ValueData: "1 - Diffpack grid file (*.grid)"

