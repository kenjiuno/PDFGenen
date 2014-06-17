; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

!define APP "PDFGenen"
!define COM "HIRAOKA HYPERS TOOLS, Inc."

!define APV "0_1"
!define VER "0.1"

; The name of the installer
Name "${APP} ${VER}"

; The file to write
OutFile "Setup_${APP}_${APV}.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\${APP}"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${COM}\${APP}" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

!include "LogicLib.nsh"

;--------------------------------

; Pages

Page license
Page components
Page directory
Page instfiles

LicenseData "LICENSE"

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "右クリックで変換 x86版" x86
  SetOutPath "$INSTDIR\x86"
  File "Release\PDFGenen.dll"
  File "Release\ToPDF.exe"
  File "tiff2pdf.exe"
  File "tiffcp.exe"

  ExecWait 'regsvr32.exe /s "$OUTDIR\PDFGenen.dll"' $0
  DetailPrint "結果: $0"

SectionEnd

Section "右クリックで変換 x64版" x64
  SetOutPath "$INSTDIR\x64"
  File "x64\Release\PDFGenen.dll"
  File "x64\Release\ToPDF.exe"
  File "tiff2pdf.exe"
  File "tiffcp.exe"

  ExecWait 'regsvr32.exe /s "$OUTDIR\PDFGenen.dll"' $0
  DetailPrint "結果: $0"

SectionEnd

Section
  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\${COM}\${APP}" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "DisplayName" "${APP}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section "スタートメニューへ: TIFやTIFFをPDFに変換(ToPDF)"

  StrCpy $1 "$SMPROGRAMS\${COM}\${APP}"
  CreateDirectory "$1"
  CreateShortCut "$1\TIFやTIFFをPDFに変換(ToPDF).lnk" "$INSTDIR\x86\ToPDF.exe" "" "$INSTDIR\x86\ToPDF.exe" 0
  CreateShortCut "$1\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd

;--------------------------------

Function .onInit
  ${If} "$PROGRAMFILES32" == "$PROGRAMFILES64"
    ; x86
    SectionSetFlags ${x64} 0
  ${Else}
    ; x64
  ${EndIf}
FunctionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Remove files and uninstaller

  ; x86
  ExecWait 'regsvr32.exe /s /u "$INSTDIR\x86\PDFGenen.dll"' $0
  DetailPrint "結果: $0"
  Delete /rebootok "$INSTDIR\x86\PDFGenen.dll"
  Delete /rebootok "$INSTDIR\x86\ToPDF.exe"
  Delete /rebootok "$INSTDIR\x86\tiff2pdf.exe"
  Delete /rebootok "$INSTDIR\x86\tiffcp.exe"

  ; x64
  ExecWait 'regsvr32.exe /s /u "$INSTDIR\x64\PDFGenen.dll"' $0
  DetailPrint "結果: $0"
  Delete /rebootok "$INSTDIR\x64\PDFGenen.dll"
  Delete /rebootok "$INSTDIR\x64\ToPDF.exe"
  Delete /rebootok "$INSTDIR\x64\tiff2pdf.exe"
  Delete /rebootok "$INSTDIR\x64\tiffcp.exe"

  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${COM}\${APP}\*.lnk"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}"
  DeleteRegKey HKLM "Software\${COM}\${APP}"

  ; Remove directories used
  RMDir "$SMPROGRAMS\${COM}\${APP}"
  RMDir "$SMPROGRAMS\${COM}"
  RMDir "$INSTDIR"
  
  IfRebootFlag 0 noreboot
    MessageBox MB_YESNO "再起動が必要です。今すぐ再起動?" IDNO noreboot
      Reboot
  noreboot:

SectionEnd
