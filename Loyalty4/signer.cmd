@echo off
echo Signing Loyalty release binaries...
echo.

set SIGNTOOL="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\signtool.exe"

%signtool% sign ^
  /tr http://timestamp.sectigo.com ^
  /td sha256 ^
  /fd sha256 ^
  /a ^
  "e:\microtrend\vs2026\loyalty4\release\sposloyalty4manager.exe" ^
  "e:\microtrend\vs2026\loyalty4\release\sposloyalty4server.exe" ^
  "e:\microtrend\vs2026\loyalty4\release\sposloyalty4servericon.exe"

echo.
echo Loyalty signing complete.
pause