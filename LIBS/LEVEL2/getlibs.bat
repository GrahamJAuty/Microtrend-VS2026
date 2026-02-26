@echo on
forfiles /S /M *.lib /C "cmd /c move @file e:\microtrend\vs2026\common\sslib"
forfiles /S /M *.hpp /C "cmd /c copy @file e:\microtrend\vs2026\common\ssinc"
pause