; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "VeriAktar"
#define MyAppVersion "1 alpha"
#define MyAppPublisher "M�min G�ler"
#define MyAppURL "http://veriaktar.blogspot.com.tr/"
#define MyAppExeName "veriaktar.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{CC54DC48-C744-4DA4-AB8C-BC3F39E9FB31}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=c:\veriaktar
DefaultGroupName={#MyAppName}
OutputDir=C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar
OutputBaseFilename=veriaktar_setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\Release\veriaktar.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\SEMBOLLER.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\versiyon.txt"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent