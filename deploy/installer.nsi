
;--------------------------------

; The name of the installer
!include "FileFunc.nsh"
;!insertmacro MUI_LANGUAGE "Russian"

;!getdllversion "ValeraTamasha.exe" Expv_
;!define VERSION ${Expv_1}.${Expv_2}.${Expv_3}.${Expv_4}

SetCompressor /SOLID lzma

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Build Unicode installer
Unicode True

; The default installation directory
InstallDir $PROGRAMFILES\ValeraTamasha

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\JupiterSoft\ValeraTamasha" "Install_Dir"


; The file to write
Name "ValeraTamasha"

OutFile "ValeraTamasha.exe"



;--------------------------------

; Pages

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

LoadLanguageFile "${NSISDIR}\Contrib\Language files\Russian.nlf"
LangString Name ${LANG_RUSSIAN} "Russian"

;--------------------------------

; The stuff to install
Section "ValeraTamasha (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ;File /r "c:\Qt\Deploy\taxiplus\pub\*.*"
  File /r "*.*"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\JupiterSoft\ValeraTamasha" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\taxiplus" "DisplayName" "taxiplus"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\taxiplus" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\taxiplus" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\taxiplus" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\taxiplus"
  CreateShortcut "$SMPROGRAMS\taxiplus\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\taxiplus\ТаксиПлюс.lnk" "$INSTDIR\taxiplus-desktop.exe" "" "$INSTDIR\taxiplus-desktop.exe" 0
  CreateShortcut "$DESKTOP\ТаксиПлюс.lnk" "$INSTDIR\taxiplus-desktop.exe" "" "$INSTDIR\taxiplus-desktop.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\taxiplus"
  DeleteRegKey HKLM "Software\JupiterSoft\Taxi+"

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\taxiplus\*.*"
  Delete "$DESKTOP\ТаксиПлюс.lnk"

  ; Remove directories used
  RMDir /r "$SMPROGRAMS\taxiplus"
  RMDir /r "$INSTDIR"

SectionEnd
