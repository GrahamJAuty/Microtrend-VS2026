#pragma once
//**********************************************************************
#define	AUDIT_SERVER				0				// done from server
#define	AUDIT_PC					1				// done @ pc
#define AUDIT_POS					2				// data from till
#define AUDIT_EXT					3				// data from external source
//**********************************************************************
#define AUDIT_SOURCEMAX				4
//**********************************************************************
#define APPNO_INSERT				0				// record inserted into daabase
#define	APPNO_DELETE				1				// record deleted from database
#define APPNO_BALANCE				2
#define APPNO_EDIT					3
#define APPNO_HOTLIST				4
#define APPNO_UNHOTLIST				5
#define	APPNO_PURSE1_TOPUP1			6
#define APPNO_ADDPOINTS				7
#define	APPNO_SUBCASH				8
#define APPNO_SUBPOINTS				9
#define	APPNO_REVALUEBONUSCASH		10
#define	APPNO_REVALUEBONUSPOINTS	11
#define	APPNO_REDEEM				12
#define	APPNO_SALE					13
#define	APPNO_REWARDBONUSCASH		14
#define	APPNO_REWARDBONUSPOINTS		15
#define APPNO_XFEROUT				16				// transfer out
#define APPNO_XFERIN				17
#define APPNO_HOTXFEROUT			18				// hotlist replacment transfer
#define APPNO_HOTXFERIN				19
#define	APPNO_REFUNDSALE				20
#define	APPNO_REFUNDREWARDBONUSCASH	21
#define	APPNO_REFUNDREWARDBONUSPOINTS	22
#define APPNO_IMPORTINSERT			23
#define APPNO_IMPORTUPDATE			24
#define APPNO_BATCHUPDATE			25
#define APPNO_BATCHDELETE			26
#define	APPNO_PURSE1_TOPUP2			27				// Added by 2nd Plu Revaluation 
#define APPNO_INSERTREPLACE			28				// hotlist replacement record inserted into database
#define APPNO_REFRESH				29				// purse2 refresh
#define	APPNO_BATCHREDEEM			30
#define APPNO_EXTENDEXPIRE			31
#define APPNO_SETPURSE1				32
#define APPNO_ADDGIFT				33
#define APPNO_ADDGIFTX				34
#define APPNO_SO_AWARD				35
#define APPNO_SO_REDEEM				36
#define APPNO_SO_REDEEM2			37
#define APPNO_SO_MDEL				38
#define APPNO_SO_MEDIT				39
#define APPNO_SO_MADD				40
#define APPNO_SO_XFER				41
#define APPNO_SO_HOTL				42
#define APPNO_SO_ADEL				43
#define APPNO_SO_MDEL_EXPIRE		44
#define APPNO_SO_MDEL_EDITION		45
#define APPNO_SO_MFIX_EXPIRE		46
#define APPNO_SO_MFIX_EDITION		47
#define	APPNO_DEPTBONUSPOINTS		48
#define	APPNO_REFUNDDEPTBONUSPOINTS	49
#define	APPNO_DEPTBONUSCASH			50
#define	APPNO_REFUNDDEPTBONUSCASH	51
#define	APPNO_PURSE1_TOPUP3			52
#define	APPNO_PURSE1_TOPUP4			53
#define	APPNO_MANUAL_SPEND			54
#define	APPNO_MANUAL_REFUND			55
#define	APPNO_PURSE1_TOPUP5			56
#define	APPNO_PURSE2_TOPUP1			57
#define	APPNO_PURSE2_TOPUP2			58
#define	APPNO_PURSE2_TOPUP3			59
#define	APPNO_PURSE2_TOPUP4			60
#define	APPNO_PURSE2_TOPUP5			61
#define APPNO_PURSE1_MANTOPUP		62
#define APPNO_PURSE2_MANTOPUP		63
#define APPNO_PURSE1_IMPTOPUP		64
#define APPNO_PURSE2_IMPTOPUP		65
//**********************************************************************
#define	APPNO_MAX					66
//**********************************************************************

class CAuditReportTexts
{
public:
	static CString GetReportSourceText(int nSourceType, int nAppNo, int nTerminalNo = 0, bool bLongForm = FALSE);
	static int GetAppNoAddCash(int nPurseNo, int nTopupNo);
	static void GetAddCashIndexByAppNo(int nAppNo, int& nPurseNo, int& nTopupNo);
	static CString GetDefaultComment(int nAppNo);

private:
	static const char* m_szSourceText[AUDIT_SOURCEMAX];
	static const char* m_szAppNoText[APPNO_MAX];
	static const char* m_szDefaultComment[APPNO_MAX];
};

//**********************************************************************