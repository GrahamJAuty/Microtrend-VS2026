#pragma once
//**********************************************************************
#define SQLSRVR_ERRCONTEXT_NONE 0
#define SQLSRVR_ERRCONTEXT_CONNECT 1
#define SQLSRVR_ERRCONTEXT_BUILDSCHEMA 2
#define SQLSRVR_ERRCONTEXT_CHECKVERSION 3
//**********************************************************************
#define SQLSRVR_BUILDSCHEMA_ERR_NONE 0
#define SQLSRVR_BUILDSCHEMA_ERR_COUNTTABLES 1
#define SQLSRVR_BUILDSCHEMA_ERR_CREATESCHEMA 2
#define SQLSRVR_BUILDSCHEMA_ERR_VERSION 3
#define SQLSRVR_BUILDSCHEMA_ERR_MIGRATESCHEMA 4
//**********************************************************************
#define SQLSRVR_CHECKVERSION_ERR_NONE 0
#define SQLSRVR_CHECKVERSION_ERR_FAIL 1
#define SQLSRVR_CHECKVERSION_ERR_NOLINE 2
#define SQLSRVR_CHECKVERSION_ERR_BADVER 3
//**********************************************************************
#define SQLSRVR_STARTUP_ERR_NONE 0
#define SQLSRVR_STARTUP_ERR_INIT 1
#define SQLSRBR_STARTUP_ERR_OPENDB 2
//**********************************************************************
#define SQL_PREPARE_ERR_NONE 0
#define SQL_PREPARE_ERR_PREPV2 1
#define SQL_PREPARE_ERR_BIND 2
//**********************************************************************
#define SQLCRUD_ERR_NONE 0
#define SQLCRUD_ERR_RESET 1
#define SQLCRUD_ERR_BIND 2
#define SQLCRUD_ERR_NOMATCH 3
#define SQLCRUD_ERR_DUPLICATE 4
#define SQLCRUD_ERR_FAIL 5
#define SQLCRUD_ERR_PARTFAIL 6
#define SQLCRUD_ERR_EXCEPT_MISC 7
#define SQLCRUD_ERR_EXCEPT_DB 8
#define SQLCRUD_ERR_UPDATE 9
//**********************************************************************
#define SQL_BIND_ERR_NONE 0
#define SQL_BIND_ERR_RESET 1
#define SQL_BIND_ERR_BIND 2
//**********************************************************************
#define SMARTPAY_DBVERSION 19
//**********************************************************************
#define SQLCREATEFIELD_PRIMARYKEY 1
#define SQLCREATEFIELD_NOTNULL 2
#define SQLCREATEFIELD_UNIQUE 4
/**********************************************************************/
#define SQLSRVR_BIT 0
#define SQLSRVR_INT32 1
#define SQLSRVR_INT64 2
#define SQLSRVR_DECIMAL 3
#define SQLSRVR_TEXT 4
#define SQLSRVR_COMPUTED 5
#define SQLSRVR_ROWVERSION 6
#define SQLSRVR_FIXLENTEXT 7
/**********************************************************************/
#define SQLTRAN_STATE_NOTSTARTED 0
#define SQLTRAN_STATE_ACTIVE 1
#define SQLTRAN_STATE_WAITEND 2
#define SQLTRAN_STATE_COMMITOK 3
#define SQLTRAN_STATE_COMMITFAIL 4
#define SQLTRAN_STATE_ROLLBACKOK 5
#define SQLTRAN_STATE_ROLLBACKFAIL 6
/**********************************************************************/
#define PURCHASECONTROLSTAMPSET_NORMAL 0
#define PURCHASECONTROLSTAMPSET_REDEEM 1
//**********************************************************************
#define nMAXPERIODS 6
//**********************************************************************
#define szTIMERANGE_ALL		"Full 24 hrs"
#define szTIMERANGE_NOW		"Now"
//**********************************************************************
#define nMAX_INFOTEXTS 8
//**********************************************************************
