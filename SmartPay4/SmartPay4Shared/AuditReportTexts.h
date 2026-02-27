#pragma once
//**********************************************************************
#define	AUDIT_SERVER 0			// done from server
#define	AUDIT_PC 1				// done @ pc
#define AUDIT_POS 2				// data from till
#define AUDIT_KIOSKLINK 3		// Kiosk BioLink
#define AUDIT_EODBALANCE 4		// EndODDay Balance line / APPNO_BALANCE
#define AUDIT_VENDING 5
#define AUDIT_EXT 6
//**********************************************************************
#define AUDIT_SOURCEMAX	7
//**********************************************************************
#define APPNO_INSERT 0				// record inserted into database
#define	APPNO_DELETE 1				// record deleted from database
#define APPNO_BALANCE 2
#define APPNO_EDIT 3
#define	APPNO_ADDCASH 4
//#define	APPNO_ADDCASH2 5		// was plu revaluation2 - not used plu2 used to add to Purse3 
#define APPNO_REFRESH 6
#define APPNO_ADDPOINTS 7
#define	APPNO_HOTLIST 8
#define APPNO_UNHOTLIST 9
#define	APPNO_REVALUEBONUSCASH 10
#define	APPNO_REVALUEBONUSPOINTS 11
#define	APPNO_REDEEM 12
#define	APPNO_SALE 13
#define	APPNO_REWARDBONUSCASH 14
#define	APPNO_REWARDBONUSPOINTS 15
#define APPNO_XFEROUT 16
#define APPNO_XFERIN 17
#define APPNO_HOTXFEROUT 18				
#define APPNO_HOTXFERIN 19
#define	APPNO_REFUNDSALE 20
#define	APPNO_REFUNDREWARDBONUSCASH 21
#define	APPNO_REFUNDREWARDBONUSPOINTS 22
#define APPNO_IMPORTINSERT 23
#define APPNO_IMPORTUPDATE 24
#define APPNO_BATCHUPDATE 25
#define APPNO_BATCHDELETE 26
#define APPNO_PARENTPAY 27
#define APPNO_WISEPAY 28
#define APPNO_TUCASI 29
#define APPNO_HOTLISTREPLACE 30
#define APPNO_INSERTRECORDSIMS 31
#define APPNO_INSERTRECORDPPAY 32
#define APPNO_INSERTRECORDWISEPAY 33
#define	APPNO_IMPORTHOTLIST 34			// auto hotlist on importing new account
#define	APPNO_IMPORTREPLACE	35			// auto replace due to import of new account for existing recotd
#define APPNO_IMPORTDELETE 36
#define APPNO_SCHOOLCOMMS 37
#define APPNO_IMPORTTOPUP 38
#define APPNO_TUCASIv2 39
#define APPNO_SQUID 40
#define APPNO_PARENTMAIL 41
#define APPNO_WPOPENINGBALANCE 42	  	// WebPayment opening balance (ParentMail)
#define APPNO_IMPORTCUSTOM 43			// custom csv import data
#define	APPNO_DELETELEAVER 44			// record moved to leavers database
#define	APPNO_BATCHLEAVER 45			// batch records moved to leavers database
#define	APPNO_IMPORTLEAVER 46			// imported records to move to leavers database
#define APPNO_INSERTRECORDWONDE 47		// Wonde csv import data
#define APPNO_MEAL 48
#define	APPNO_BATCHREDEEM 49
#define	APPNO_CREDIT 50
#define	APPNO_BROMCOM 51
#define APPNO_BIOREG 52
#define APPNO_BIOUNREG 53
#define APPNO_SO_AWARD 54
#define APPNO_SO_MDEL 55
#define APPNO_SO_MEDIT 56
#define APPNO_SO_MADD 57
#define APPNO_SO_XFER 58
#define APPNO_SO_HOTL 59
#define APPNO_SO_ADEL 60
#define APPNO_SO_MDEL_EXPIRE 61
#define APPNO_SO_MFIX_EXPIRE 62
#define APPNO_SO_XDEL_EXPIRE 63
#define APPNO_PEBBLE_TOPUP 64
#define APPNO_PEBBLE_TOPUP_REFUND 65
#define APPNO_PEBBLE_ORDER 66
#define APPNO_PEBBLE_ORDER_REFUND 67
//*******************************************************************
#define	APPNO_MAX 68
//*******************************************************************

class CAuditReportTexts
{
public:
	static CString GetReportSourceText(int nSourceType, int nAppNo, int nTerminalNo = 0, CString strExtraText = "");
	static CString GetDefaultComment(int nAppNo);

private:
	static const char* m_szSourceText[AUDIT_SOURCEMAX];
	static const char* m_szAppNoText[APPNO_MAX];
	static const char* m_szDefaultComment[APPNO_MAX];
};

//**********************************************************************