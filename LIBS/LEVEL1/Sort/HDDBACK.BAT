xcopy *.sln 		..\BACKUP
xcopy *.vcxproj 	..\BACKUP
xcopy *.vcxproj.* 	..\BACKUP
xcopy *.rc 		..\BACKUP
xcopy *.cpp /s		..\BACKUP
xcopy *.c /s		..\BACKUP
xcopy *.bat 		..\BACKUP
xcopy *.hpp /s		..\BACKUP
xcopy *.h /s		..\BACKUP
xcopy *.rc2 /s 		..\BACKUP
xcopy *.ico /s 		..\BACKUP
xcopy *.bmp /s 		..\BACKUP
xcopy *.def /s 		..\BACKUP
xcopy *.txt /s 		..\BACKUP
e:\7za a ..\BACKUP.7z ..\BACKUP\* -pTT691JM608##
DEL ..\BACKUP\*.* /s /q
RMDIR ..\BACKUP /s /q
