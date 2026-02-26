@echo off
echo Signing SPOS Manager release binaries...
echo.

set SIGNTOOL="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\signtool.exe"

%signtool% sign ^
  /tr http://timestamp.sectigo.com ^
  /td sha256 ^
  /fd sha256 ^
  /a ^
  "e:\microtrend\vs2022\sposmanager5\release\sposecrmanager.exe" ^
  "e:\microtrend\vs2022\sposmanager5\release\sposstockmanager.exe" ^
  "e:\microtrend\vs2022\sposmanager5\release\spostray.exe"

echo.
echo SPOS Manager signing complete.
pause