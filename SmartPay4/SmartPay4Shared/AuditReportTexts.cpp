//**********************************************************************
#include "AuditReportTexts.h"
//**********************************************************************

const char* CAuditReportTexts::m_szSourceText[AUDIT_SOURCEMAX] =
{
	"serv",		// from server 
	"pc",		// from pc
	"pos",		// from till 
	"ksk",		// Kiosk BioLink 
	"eod",		// Internal EndOfDat audit line 
	"vnd",		// from vending
	"ext",		// from external system
};

//**********************************************************************

const char* CAuditReportTexts::m_szAppNoText[APPNO_MAX] =
{
	"ins",		// add record
	"del",		// delete
	"bal",		// balance
	"edit",		// edit record
	"csh",		// add cash by 1st Plu revaluation
	"csh2",		// ** not used add cash by 2nd Plu revaluation
	"rfsh",		// purse2 refresh 
	"pts",		// add points
	"hot",		// hotlist 
	"xhot",		// unhotlist
	"+rcsh",	// bonus revaluation cash
	"+rpts",	// bonus revaluation points
	"rdp",		// redeem points
	"sale",		// sale
	"+lcsh",	// bonus SmartPay cash
	"+lpts",	// bonus SmartPay points
	"xout",		// transfer out
	"xin",		// transfer in
	"hxout",	// hotlist replacement transfer out
	"hxin",		// hotlist replacement transfer in
	"rfnd",		// refund sale
	"-lcsh",	// refund bonus SmartPay cash
	"-lpts",	// refund bonus SmartPay points
	"impi",		// import insert
	"impu",		// import update
	"bupd",		// batch update
	"bdel",		// batch delete
	"pp",		// ParentPay Import
	"wp",		// WisePay Import
	"tu",		// Tucasi(Soap) Import
	"hrp",		// hotlist replacment	
	"sims",		// SIMS import	
	"ppi",		// ParentPay import	
	"wpi",		// WisePay import	
	"imph",		// import hotlist on importing new account
	"impr",		// import replacement due to import of new account for existing recotd
	"impd",		// import deleted record
	"sci",		// Schoolcomms import	
	"impt",		// Import topup
	"tu",		// Tucasi(Json) Import
	"sqd",		// sQuid Import
	"pm",		// ParentMail
	"wob",		// WebPayment Opening Balance
	"impc",		// import custom csv update
	"left",		// single record moved to leavers database
	"left",		// batch move to leavers database
	"left",		// import move to leavers database
	"gci",		// Wonde Import
	"meal",		// pc meal
	"brdp",		// batch redeem points
	"cred",		// non liability credit
	"bcm",		// brom com import
	"bio_reg",	// bio register
	"bio_del",	// bio unregister
	"award",	// purchase control award
	"m.del",	// purchase control manual delete
	"m.edt",	// purchase control manual edit
	"m.add",	// purchase control manual add
	"m.xfer",	// purchase control xfer
	"m.hotl",	// purchase control hotlist
	"ac.del",	// purchase control account delete
	"m.del.ex",	// purchase control manual delete expire
	"m.edt.ex",	// purchase control manual edit expire
	"a.del.ex",	// purchase control auto delete expire
	"pbl.topup",// pebble topup
	"pbl.topup.rfnd",// pebble topup refund
	"pbl.order",// pebble topup
	"pbl.order.rfnd",// pebble topup refund
};

//**********************************************************************

const char* CAuditReportTexts::m_szDefaultComment[APPNO_MAX] =
{
	"Add Record",			// add record
	"Delete Record",		// delete
	"Balance",				// balance
	"Edit Record",			// edit record
	"Add Cash Plu1",		// add cash by 1st Plu revaluation
	"Add Cash Plu2",		// ** not used add cash by 2nd Plu revaluation
	"Purse2 Refresh",		// purse2 refresh 
	"Add Points",			// add points
	"Hotlist",				// hotlist 
	"Unhotlist",			// unhotlist
	"Bonus Reval Cash",		// bonus revaluation cash
	"Bonus Reval Point",	// bonus revaluation points
	"Redeem Points",		// redeem points
	"Sale",					// sale
	"Bonus Cash",			// bonus SmartPay cash
	"Bonus Points",			// bonus SmartPay points
	"Transfer Out",			// transfer out
	"Transfer In",			// transfer in
	"Hotlist Transfer Out",	// hotlist replacement transfer out
	"Hotlist Transfer In",	// hotlist replacement transfer in
	"Refund Sale",			// refund sale
	"Refund Bonus Cash",	// refund bonus SmartPay cash
	"Refund Bonus Points",	// refund bonus SmartPay points
	"Import Insert",		// import insert
	"Import Update",		// import update
	"Batch Update",			// batch update
	"Batch Delete",			// batch delete
	"ParentPay Import",		// ParentPay Import
	"WisePay Import",		// WisePay Import
	"Tucasi (SOAP) Import",	// Tucasi(Soap) Import
	"Hotlist Replace",		// hotlist replacment	
	"SIMS Import",			// SIMS import	
	"Parent Pay Import",	// ParentPay import	
	"WisePay Import",		// WisePay import	
	"Import Hotlist",		// import hotlist on importing new account
	"Import Replace",		// import replacement due to import of new account for existing recotd
	"Import Delete",		// import deleted record
	"SchoolComms Import",	// Schoolcomms import	
	"Import Topup",			// Import topup
	"Tucasi (JSON) Import",	// Tucasi(Json) Import
	"Squid Import",			// sQuid Import
	"ParentMail",			// ParentMail
	"WebPayment Opening Bal",// WebPayment Opening Balance
	"Import Custom CSV",	// import custom csv update
	"Move Leavers Single",	// single record moved to leavers database
	"Move Leavers Batch",	// batch move to leavers database
	"Move Leavers Import",	// import move to leavers database
	"Wonde Import",			// Wonde Import
	"PC Meal",				// pc meal
	"Batch Redeem Points",	// batch redeem points
	"Non Liability Credit",	// non liability credit
	"BromCom Import",		// brom com import
	"Bio Register",			// bio register
	"Bio Unregister",		// bio unregister
	"PCtrl Sale",			// purchase control award
	"PCtrl Manual Delete",	// purchase control manual delete
	"PCtrl Edit",			// purchase control manual edit
	"PCtrl Add",			// purchase control manual add
	"PCtrl Transfer",		// purchase control xfer
	"PCtrl Hotlist",		// purchase control hotlist
	"PCtrl Account Delete",	// purchase control account delete
	"PCtrl Man Del Expire",	// purchase control manual delete expire
	"PCtrl Edit Expire",	// purchase control manual edit expire
	"PCtrl Auto Del Expire",// purchase control auto delete expire
	"Pebble Topup",
	"Refund Pebble Topup",
	"Pebble Order",
	"Refund Pebble Order",
};

//**********************************************************************

CString CAuditReportTexts::GetReportSourceText(int nSourceType, int nAppNo, int nTerminalNo, CString strExtraText)
{
	CString strText = "";

	switch (nSourceType)
	{
	case AUDIT_SERVER:
	case AUDIT_PC:
	case AUDIT_EODBALANCE:
		strText.Format("%s/%s", 
			m_szSourceText[nSourceType], 
			m_szAppNoText[nAppNo]);
		break;

	case AUDIT_EXT:
		strText.Format("%s/%s %s", 
			m_szSourceText[nSourceType], 
			m_szAppNoText[nAppNo], 
			(const char*)strExtraText);
		break;

	case AUDIT_VENDING:
	case AUDIT_POS:
		strText.Format("%s%d/%s %s", 
			m_szSourceText[nSourceType], 
			nTerminalNo, 
			m_szAppNoText[nAppNo], 
			(const char*)strExtraText);
		break;

	case AUDIT_KIOSKLINK:
		strText.Format("%s%d/%s", 
			m_szSourceText[nSourceType], 
			nTerminalNo, 
			m_szAppNoText[nAppNo]);		// (v1.02g fix)
		break;
	}

	switch (nAppNo)
	{
	case APPNO_PARENTPAY:
	case APPNO_WISEPAY:
	case APPNO_TUCASI:
	case APPNO_TUCASIv2:
	case APPNO_SQUID:
	case APPNO_PARENTMAIL:
	case APPNO_BROMCOM:
	case APPNO_SCHOOLCOMMS:	
	case APPNO_PEBBLE_TOPUP:
	case APPNO_PEBBLE_TOPUP_REFUND:
	case APPNO_PEBBLE_ORDER:
	case APPNO_PEBBLE_ORDER_REFUND:
		strText += " ";
		strText += strExtraText;	
		break;
	
	case APPNO_HOTLIST:
	case APPNO_UNHOTLIST:
	case APPNO_IMPORTHOTLIST:	
		// add Alert code
		strText += strExtraText;			
		break;
	}

	return strText;
}

//**********************************************************************

CString CAuditReportTexts::GetDefaultComment(int nAppNo)
{
	CString strComment = "";

	if ((nAppNo >= 0) && (nAppNo < APPNO_MAX))
	{
		strComment = m_szDefaultComment[nAppNo];
	}

	return strComment;
}

//**********************************************************************

