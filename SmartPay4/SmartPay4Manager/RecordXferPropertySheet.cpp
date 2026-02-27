//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
//$$******************************************************************
#include "RecordXferPropertySheet.h"
//$$******************************************************************

CRecordXferPropertySheet::CRecordXferPropertySheet( const char* szFromCardNo, const char* szToCardNo, bool bHotlistReplace, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper, "", pParentWnd, iSelectPage)
	, m_Page1( m_RowAccountFrom, m_RecordXferData )
	, m_Page2( m_RowAccountFrom, m_RecordXferData )
{
	m_strFromCardNo = szFromCardNo;
	m_strToCardNo = szToCardNo;
	m_bHotlistReplace = bHotlistReplace;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	CSQLRepositoryAccount RepoAccount;
	m_RowAccountFrom.SetUserIDString(szFromCardNo);
	RepoAccount.SelectRow(m_RowAccountFrom,NULL);

	AddPage(&m_Page1);										// Balances
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page1.SetPointer ( szToCardNo, bHotlistReplace );

	AddPage(&m_Page2);										// Details
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page2.SetPointer (bHotlistReplace );
}

//$$******************************************************************

CRecordXferPropertySheet::~CRecordXferPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CRecordXferPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CRecordXferPropertySheet::CSSAutoShutdownPostInitDialog()
{
	CString strCaption;
	if (m_bHotlistReplace == TRUE)
	{
		strCaption.Format("Select Items to transfer to replacement UserID %s", System.FormatCardNo(m_strToCardNo));
	}
	else
	{
		strCaption.Format("Select Items to Copy / Transfer to UserID %s", System.FormatCardNo(m_strToCardNo));
	}

	SetWindowText ( strCaption );

	return TRUE;
}

//$$******************************************************************

