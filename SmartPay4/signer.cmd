@echo off
echo Signing SmartPay release binaries...
echo.

set SIGNTOOL="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\signtool.exe"

%signtool% sign ^
  /tr http://timestamp.sectigo.com ^
  /td sha256 ^
  /fd sha256 ^
  /a ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4manager.exe" ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4server.exe" ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4servericon.exe" ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4bgnd.exe" ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4bgndicon.exe" ^
  "e:\microtrend\vs2022\smartpay4\release\spossmartpay4emailbatchsend.exe"

echo.
echo SmartPay signing complete.
pause