//**********************************************************************
#include "AuditReportTexts.h"
//**********************************************************************

const char* CAuditReportTexts::m_szSourceText[AUDIT_SOURCEMAX] =
{
	"serv",		// from server 
	 "pc",		// from pc 
	 "pos",		// from till 
	 "ext",		// from external system
};

//**********************************************************************

const char* CAuditReportTexts::m_szAppNoText[APPNO_MAX] =
{
	"ins",		// add record
	"del",		// delete
	"bal",		// balance
	"edit",		// edit record
	"hot",		// hotlist
	"xhot",		// unhotlist
	"p1.csh1",	// epos topup purse1 plu1
	"pts",		// add points
	"csh",		// sub cash
	"pts",		// sub points
	"+rcsh",	// bonus revaluation cash
	"+rpts",	// bonus revaluation points
	"rdp",		// redeem points
	"sale",		// sale
	"+lcsh",	// bonus loyalty cash
	"+lpts",	// bonus loyalty points
	"xout",		// transfer out
	"xin",		// transfer in
	"hxout",	// hotlist replacement transfer out
	"hxin",		// hotlist replacement transfer in
	"rfnd",		// refund sale
	"-lcsh",	// refund bonus loyalty cash
	"-lpts",	// refund bonus loyalty points
	"impi",		// import insert
	"impu",		// import update
	"bupd",		// batch update
	"bdel",		// batch delete
	"p1.csh2",	// epos topup purse1 plu2
	"hrp",		// hotlist repacement
	"rf",		// purse2 refresh
	"brdp",		// batch rdeeme
	"renew",	// renew expiry date
	"setp1",	// set purse 1
	"gift",		// cash staff gift
	"giftx",	// cash staff gift fail
	"award",	// stamp offer award
	"redm",		// stamp offer redeem
	"redm2",	// stamp offer redemm (auxillary line)
	"m.del",	// stamp offer manual delete
	"m.edt",	// stamp offer manual edit
	"m.add",	// stamp offer manual add
	"m.xfer",	// stamp offer xfer
	"m.hotl",	// stamp offer hotlist
	"a.del",	// stamp offer account delete
	"m.del.ex",	// stamp offer manual delete expire
	"m.del.ed",	// stamp offer manual delete edition
	"m.edt.ex",	// stamp offer manual delete expire
	"m.edt.ed",	// stamp offer manual delete edition
	"+lpts.d",	// dept bonus points
	"-lpts.d",	// refund dept bonus points
	"+lchs.d",	// dept bonus cash
	"-lcsh.d",	// refund dept bonus cash
	"p1.csh3",	// epos topup purse1 plu3
	"p1.csh4",	// epos topup purse1 plu4
	"m.sale",	// manual sale
	"m.rfnd",	// manual refund
	"p1.csh5",	// epos topup purse1 plu5
	"p2.csh1",	// epos topup purse2 plu1
	"p2.csh2",	// epos topup purse2 plu2
	"p2.csh3",	// epos topup purse2 plu3
	"p2.csh4",	// epos topup purse2 plu4
	"p2.csh5",	// epos topup purse1 plu5
	"p1.m.csh",	// manual topup purse1
	"p2.m.csh",	// manual topup purse2
	"p1.i.csh",	// import topup purse1
	"p2.i.csh",	// import topup purse2
};

//**********************************************************************

const char* CAuditReportTexts::m_szDefaultComment[APPNO_MAX] =
{
    "Add Account",					
    "Delete Account",				
    "View Account",					
    "Edit Account",					
    "Hotlist Account",				
    "Unhotlist Account",			
    "Purse1 Topup1",				
    "Add Points",					
    "Reduce Purse1 Value",			
    "Reduce Points",				
    "Topup Bonus Value",			
    "Topup Bonus Points",			
    "Redeem Points",				
    "EPOS Sale",					
    "Spend Bonus Value",			
    "Spend Bonus Points",			
    "Transfer Out",					
    "Transfer In",					
    "Hotlist Transfer Out",			
    "Hotlist Transfer In",			
    "Refund Sale",					
    "Refund Spend Bonus Value",		
    "Refund Spend Bonus Points",	
    "Import Insert",				
    "Import Update",				
    "Batch Update",					
    "Batch Delete",					
    "Purse1 Topup2",				
    "Hotlist Replace",				
    "Purse2 Refresh",				
    "Batch Redeem",					
    "Renew Expiry Date",			
    "Set Purse1 Value",				
    "Staff Gift",					
    "Staff Gift Fail",				
    "Award Offer Stamp",			
    "Redeem Offer Stamp",			
    "Redeem Offer Stamp",			
    "Manual Delete Offer Stamp",	
    "Manual Edit Offer Stamp",		
    "Manual Add Offer Stamp",		
    "Manual Transfer Offer Stamp",	
    "Manual Hotlist Offer Stamp",	
    "Account Delete Offer Stamp",	
    "Delete Expired Offer Stamp",	
    "Delete Previous Offer Stamp",	
    "Edit Offer Stamp Expiry",		
    "Edit Offer Stamp Edition",		
    "Dept Spend Bonus Points",			
    "Refund Dept Spend Bonus Points",	
    "Dept Spend Bonus Value",			
    "Refund Dept Spend Bonus Value",	
    "Purse1 Topup3",				
    "Purse1 Topup4",				
    "Manual Sale",						
    "Manual Refund",					
    "Purse1 Topup5",				
    "Purse2 Topup1",				
    "Purse2 Topup2",				
    "Purse2 Topup3",			
    "Purse2 Topup4",			
    "Purse2 Topup5",			
    "Purse1 Manual Topup",			
    "Purse2 Manual Topup",			
    "Purse1 Import Topup",			
    "Purse2 Import Topup",
};

//**********************************************************************

CString CAuditReportTexts::GetReportSourceText(int nSourceType, int nAppNo, int nTerminalNo, bool bLongForm )
{
	CString strAppNoText = "";
	
	if ((nAppNo >= 0) && (nAppNo < APPNO_MAX))
	{
		if (FALSE == bLongForm)
		{
			strAppNoText = m_szAppNoText[nAppNo];
		}
		else
		{
			strAppNoText = m_szDefaultComment[nAppNo];
		}
	}
	else
	{
		strAppNoText = "???";
	}


	CString strText = "";

	switch (nSourceType)
	{
	case AUDIT_SERVER:
	case AUDIT_PC:
	case AUDIT_EXT:
		strText.Format("%s/%s", 
			m_szSourceText[nSourceType], 
			(const char*) strAppNoText);
		break;

	case AUDIT_POS:
		if ((nAppNo >= 0) && (nAppNo < APPNO_MAX))
		{
			strText.Format("%s%d/%s", 
				m_szSourceText[nSourceType], 
				nTerminalNo, 
				(const char*) strAppNoText);
		}
		else
		{
			strText.Format("%s%d/???",
				m_szSourceText[nSourceType], 
				nTerminalNo);
		}
		break;
	}

	return strText;
}

//*******************************************************************

int CAuditReportTexts::GetAppNoAddCash(int nPurseNo, int nTopupNo)
{
	switch (nPurseNo)
	{
	case 1:
		switch (nTopupNo)
		{
		case 1:
			return APPNO_PURSE1_TOPUP1;

		case 2:
			return APPNO_PURSE1_TOPUP2;

		case 3:
			return APPNO_PURSE1_TOPUP3;

		case 4:
			return APPNO_PURSE1_TOPUP4;

		case 5:
			return APPNO_PURSE1_TOPUP5;

		default:
			return APPNO_PURSE1_TOPUP1;
		}

	case 2:
		switch (nTopupNo)
		{
		case 1:
			return APPNO_PURSE2_TOPUP1;

		case 2:
			return APPNO_PURSE2_TOPUP2;

		case 3:
			return APPNO_PURSE2_TOPUP3;

		case 4:
			return APPNO_PURSE2_TOPUP4;

		case 5:
			return APPNO_PURSE2_TOPUP5;

		default:
			return APPNO_PURSE2_TOPUP1;
		}

	default:
		return APPNO_PURSE1_TOPUP1;
	}
}

//*******************************************************************

void CAuditReportTexts::GetAddCashIndexByAppNo(int nAppNo, int& nPurseNo, int& nTopupNo)
{
	nPurseNo = 0;
	nTopupNo = 0;

	switch (nAppNo)
	{
	case APPNO_PURSE1_TOPUP1:
		nPurseNo = 1;
		nTopupNo = 1;
		break;

	case APPNO_PURSE1_TOPUP2:
		nPurseNo = 1;
		nTopupNo = 2;
		break;

	case APPNO_PURSE1_TOPUP3:
		nPurseNo = 1;
		nTopupNo = 3;
		break;

	case APPNO_PURSE1_TOPUP4:
		nPurseNo = 1;
		nTopupNo = 4;
		break;

	case APPNO_PURSE1_TOPUP5:
		nPurseNo = 1;
		nTopupNo = 5;
		break;

	case APPNO_PURSE2_TOPUP1:
		nPurseNo = 2;
		nTopupNo = 1;
		break;

	case APPNO_PURSE2_TOPUP2:
		nPurseNo = 2;
		nTopupNo = 2;
		break;

	case APPNO_PURSE2_TOPUP3:
		nPurseNo = 2;
		nTopupNo = 3;
		break;

	case APPNO_PURSE2_TOPUP4:
		nPurseNo = 2;
		nTopupNo = 4;
		break;

	case APPNO_PURSE2_TOPUP5:
		nPurseNo = 2;
		nTopupNo = 5;
		break;
	}
}

//*******************************************************************

CString CAuditReportTexts::GetDefaultComment(int nAppNo)
{
	CString strComment = "";
	if ((nAppNo >= 0) && (nAppNo < APPNO_MAX))
	{
		strComment = m_szDefaultComment[nAppNo];
	}
	else
	{
		strComment = "???";
	}
	return strComment;
}

//*******************************************************************
