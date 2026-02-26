#pragma once
/**********************************************************************/
#define BKERROR_CONTEXT_NONE 0
#define BKERROR_CONTEXT_PREZIP 1
#define BKERROR_CONTEXT_ZIP_CREATE 2
#define BKERROR_CONTEXT_ZIP_EXTRACT 3
/**********************************************************************/
#define BKERROR_TYPE_BADNAME 1
#define BKERROR_TYPE_NOTEXIST 2
#define BKERROR_TYPE_NOROOT 3
#define BKERROR_TYPE_RAMDISK 4
#define BKERROR_TYPE_DRIVETYPE 5
#define BKERROR_TYPE_RENAMETEMPFOLDER 6
#define BKERROR_TYPE_MAKEFOLDER 7
#define BKERROR_TYPE_REMOVETEMPFOLDER 8
#define BKERROR_TYPE_RENAMESYSFOLDER 9
/**********************************************************************/
#define BKERROR_TYPE_ZIPCREATEARCHIVE 1
#define BKERROR_TYPE_ADDFILE 2
#define BKERROR_TYPE_ZIPWRITEARCHIVE1 3
#define BKERROR_TYPE_ZIPWRITEARCHIVE2 4
#define BKERROR_TYPE_COPYFILE 5
#define BKERROR_TYPE_BADFILEPATH 6
/**********************************************************************/
#define BKERROR_TYPE_ZIPOPENARCHIVE 1
#define BKERROR_TYPE_ZIPREADENTRY 2
#define BKERROR_TYPE_ZIPEXTRACTFILE 3
#define BKERROR_TYPE_RESTORESQL 3
#define BKERROR_TYPE_RESTORESCRIPT 4
#define BKERROR_TYPE_SCRIPTDBVERSION 4
/**********************************************************************/
#define BACKUP_RESULT_OK 0
#define BACKUP_RESULT_ERROR_CWD_FIND 1
#define BACKUP_RESULT_ERROR_CWD_CHANGE 2
#define BACKUP_RESULT_ERROR_PROCESS 3
#define BACKUP_RESULT_ERROR_UNKNOWN 4
#define BACKUP_RESULT_ERROR_LOCK_USERS 5
#define BACKUP_RESULT_ERROR_LOCK_PROGS 7
#define BACKUP_RESULT_ERROR_BACKUPSQL 8
/**********************************************************************/

