xcopy *.sln /s		..\BACKUP
xcopy *.vcproj /s 	..\BACKUP
xcopy *.rc /s		..\BACKUP
xcopy *.cpp /s		..\BACKUP
xcopy *.c /s		..\BACKUP
xcopy *.bat /s 		..\BACKUP
xcopy *.hpp /s		..\BACKUP
xcopy *.h /s		..\BACKUP
xcopy *.rc2 /s 		..\BACKUP
xcopy *.ico /s 		..\BACKUP
xcopy *.bmp /s 		..\BACKUP
xcopy *.def /s 		..\BACKUP
xcopy *.txt /s 		..\BACKUP
xcopy *.lib /s		..\BACKUP
e:\7za a ..\BACKUP.7z ..\BACKUP\* -pTT691JM608##
DEL ..\BACKUP\*.* /s /q
RMDIR ..\BACKUP /s /q
