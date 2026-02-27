#pragma once
//****************************************************************************
const int REPORT_NOERROR = 0;
const int REPORT_NOSOURCE = 1;
const int REPORT_INVALIDSOURCE = 2;
const int REPORT_CREATEFAIL = 3;
const int REPORT_INVALIDCARDRANGE = 4;
const int REPORT_NOMATCH = 5;
const int REPORT_CANCEL = 6;
const int REPORT_EXIT = 7;
//****************************************************************************
const int nINTERVAL_MESSAGE = 300;					//  5 minutes - Play wait message
const int nINTERVAL_SERVER_STD = 30;				// 30 seconds
const int nINTERVAL_SERVER_ERROR = 10;				// 10 seconds
const int nINTERVAL_FLUSH = 5;						//  5 seconds - check Payink StartLevels are ok
const int nINTERVAL_SERVER_CANCEL = 10;				// 10 secnd timeout counter 
const int nINTERVAL_INPUT = 1;						// 1 second
//****************************************************************************
const int ID_TIMER_GETUSERDUAL = 1111;
const int ID_TIMER_GETUSER = 2222;
const int ID_TIMER_GETUSER2 = 3333;
const int ID_TIMER_LOADERTEST = 4444;
const int ID_TIMER_PHOTOID = 5555;
const int ID_TIMER_WAITSTATUS = 6666;
const int ID_TIMER_WAITGETUSER = 7777;
const int ID_TIMER_WAITPOSTTRAN = 8888;
//****************************************************************************
const int nDONT_START = -2;
const int nSTART_REQD = -1;
const int nSTART_FAILED = 0;
const int nSTARTED = 1;
//****************************************************************************
const int nINTERFACE_BIOMETRIC = 0;
const int nINTERFACE_MIFAREv1 = 1;
const int nINTERFACE_MIFAREv2 = 2;
const int nINTERFACE_DUAL = 3;				// combined system of Biometric & MiFarev2
const int nINTERFACE_PINPAD = 4;			// manual entry only
//****************************************************************************
const int SMART_SERVER_ERROR_TERMINALNO = 11;
const int SMART_SERVER_ERROR_COUNTRYNO = 12;
const int SMART_SERVER_ERROR_CUSTOMERNO = 13;
const int SMART_SERVER_ERROR_SITENO = 14;
const int SMART_SERVER_ERROR_CARDNO = 15;
const int SMART_SERVER_ERROR_ISSUENO = 16;
const int SMART_SERVER_ERROR_EXPIRED = 17;
const int SMART_SERVER_ERROR_INVALIDEXPIRY = 18;
const int SMART_SERVER_ERROR_OUTOFSERVICE = 19;
const int SMART_SERVER_ERROR_MISMATCH = 26;
//****************************************************************************
const int nMESSAGE_NONE = 0;
const int nMESSAGE_WAITING = 1;				// Insert card \ place finger on reader
const int nMESSAGE_FINISHED = 2;			// Remove card  / Finished
const int nMESSAGE_TOPUP_OK = 3;			// Topup added
const int nMESSAGE_TOPUP_FAILED = 4;		// Topup failed
const int nMESSAGE_WAITINGFORSERVER = 5;	// Accessing server
const int nMESSAGE_NOSERVERRESPONSE = 6;	// no response from server
const int nMESSAGE_SERVERERROR = 7;			// fatal error reporetd by server
const int nMESSAGE_EXPIRED = 8;
const int nMESSAGE_FILECREATEERROR = 9;
const int nMESSAGE_FILEREADERROR = 10;
const int nMESSAGE_FILEWRITEERROR = 11;
const int nMESSAGE_OUTOFSERVICE = 12;
//****************************************************************************
const int nGROUP_ADMIN = 99;
//****************************************************************************
const int nSTATUS_OFFLINE = 0;
const int nSTATUS_ONLINE = 1;
const int nSTATUS_DISABLED = 2;				// terminal disabled
const int nSTATUS_UNKNOWN = 3;				// status not known
//****************************************************************************
const int nWAITSTATUS_ACTION_CHECKSERVERONLINE = 0;
const int nWAITSTATUS_ACTION_SETSTATUSOFF = 1;
const int nWAITSTATUS_ACTION_SETSTATUSON = 2;
const int nWAITSTATUS_ACTION_SETSTATUSINSERVICE = 3;
const int nWAITSTATUS_ACTION_SETSTATUSOUTSERVICE = 4;
const int nWAITSTATUS_ACTION_EMPTYCASH = 5;
const int nWAITSTATUS_ACTION_SETSTATUSON_CHECKSERVERONLINE = 6;
const int nWAITSTATUS_ACTION_SETSTATUSINSERVICE_CHECKSERVERONLINE = 7;
//****************************************************************************
const int nTRANPOST_ACTION_SENDNORMAL = 0;
const int nTRANPOST_ACTION_SENDOFFLINE = 1;
//****************************************************************************
const int nPOST_ERROR = 0;
const int nPOST_SENT = 1;
const int nPOST_OFFLINE = 2;
//****************************************************************************
const int nGETUSER_MIFARE_ONLY = 0;
const int nGETUSER_BIO_ONLY = 1;
const int nGETUSER_PINPAD_ONLY = 2;
const int nGETUSER_MIFARE_PINPAD = 3;
const int nGETUSER_BIO_PINPAD = 4;
//****************************************************************************
const int nGETADMIN_CANCEL = 0;
const int nGETADMIN_WANTSETUP = 1;
const int nGETADMIN_WANTTOPUP = 2;
//****************************************************************************
#define BLACK RGB(0,0,0)
#define WHITE RGB(255,255,255)
#define RED RGB(255,0,0)
#define BLUE RGB(0,0,255)
#define GREY RGB(96,96,96)
//****************************************************************************
#define TRANSPARENT_COLOUR RGB(0,255,0)
//****************************************************************************
#define MYCOLOUR_DIALOGERROR RGB(255,0,0)
#define MYCOLOUR_STATICERROR RGB(255,255,255)
#define MYCOLOUR_DIALOG RGB(192,192,128)
//****************************************************************************
#define MYCOLOUR_BUTTON RGB(150,200,150)
#define MYCOLOUR_INHIBIT RGB(150,150,150)
//****************************************************************************
#define MYCOLOUR_GROUPBOX RGB(0,0,0), RGB(255,255,255), 10		// ColourText, ColourLine, FontSize
//****************************************************************************
#define MYCOLOUR_GREY RGB(96,96,96)
#define MYCOLOUR_WHITE RGB(255,255,255)
#define MYCOLOUR_BLACK RGB(0,0,0)
//****************************************************************************

