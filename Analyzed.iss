[Setup]
AppName=Analyzed
UninstallDisplayName=Analyzed
VersionInfoVersion=0.5.1.0
AppVerName=Analyzed 0.5.1.0
AppPublisher=Marko Mihovilic
AppPublisherURL=http://free-kr.t-com.hr/mihasoftware/
AppSupportURL=http://free-kr.t-com.hr/mihasoftware/
AppUpdatesURL=http://free-kr.t-com.hr/mihasoftware/
OutputDir=.\
DefaultDirName={pf}\Windows Media Player\Visualizations\Analyzed
OutputBaseFilename=Analyzed
Compression=lzma
SolidCompression=yes
AppendDefaultDirName=no
DirExistsWarning=no
WizardImageFile=compiler:WizModernImage-New.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-New.bmp
SetupIconFile=compiler:Setup.ico
InfoBeforeFile=Changes.rtf

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl";

[Files]
Source: "Release\Analyzed.dll"; DestDir: "{app}"; Flags: regserver uninsrestartdelete;

[Tasks]
Name: setblur; Description: "Enable Blur Effect"; GroupDescription: "Options:";
Name: setvisualization; Description: "Default Visualzation"; GroupDescription: "Options:";

Name: highresolution; Description: "High"; GroupDescription: "Offscreen Buffer Size:"; Flags: exclusive unchecked
Name: normalresolution; Description: "Normal"; GroupDescription: "Offscreen Buffer Size:"; Flags: exclusive
Name: lowresolution; Description: "Low"; GroupDescription: "Offscreen Buffer Size:"; Flags: exclusive unchecked
Name: verylowresolution; Description: "Very Low"; GroupDescription: "Offscreen Buffer Size:"; Flags: exclusive unchecked

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\MediaPlayer\Preferences"; ValueType: string; ValueName: "CurrentEffectType"; ValueData: "Analyzed"; Tasks: setvisualization;
Root: HKCU; Subkey: "Software\Microsoft\MediaPlayer\Preferences"; ValueType: dword; ValueName: "CurrentEffectPreset"; ValueData: "0"; Tasks: setvisualization;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Resolution"; ValueData: "3"; Tasks: highresolution;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Resolution"; ValueData: "2"; Tasks: normalresolution;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Resolution"; ValueData: "1"; Tasks: lowresolution;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Resolution"; ValueData: "0"; Tasks: verylowresolution;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Blur"; ValueData: "1"; Tasks: setblur;
Root: HKCU; Subkey: "Software\Miha Software\Analyzed Visualization"; ValueType: dword; ValueName: "Blur"; ValueData: "0"; Tasks: not setblur;
