//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "BatchPropertySheet.h"
#include "BatchUpdate.h"
//*******************************************************************

CBatchPropertySheet::CBatchPropertySheet( const char* szTitle, int nUpdateType, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper,szTitle, pParentWnd, iSelectPage),
	m_Page2(BalanceMemory),
	m_Page5(DetailMemory)
{
	m_nUpdateType = nUpdateType;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);										// report on
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page1.SetPointer ( nUpdateType );

	if ( nUpdateType == nBATCHUPDATE_UPDATE )				// only required if batch update
	{
		AddPage(&m_Page2);									// updates
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	
		AddPage(&m_Page5);									// detail updates
		m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;
	}

	AddPage(&m_Page3);										// conditional
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CBatchPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//*******************************************************************

bool CBatchPropertySheet::HaveUpdates()
{
	bool bReply = TRUE;

	if (m_nUpdateType == nBATCHUPDATE_UPDATE)
	{
		if ((bReply = m_Page2.HaveUpdates()) == FALSE)		// check have some batch updates set
		{
			bReply = m_Page5.HaveUpdates();						// check have some batch updates set
		}
	}

	return bReply;
}

//*******************************************************************

bool CBatchPropertySheet::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = m_Page2.UpdateRecord(RowAccount);
		
	if (m_Page5.UpdateRecord(RowAccount) == TRUE)
	{
		bReply = TRUE;
	}

	return bReply;
}

//*******************************************************************

bool CBatchPropertySheet::IsValid(CSQLRowAccountFull& RowAccount)
{
	if (m_Page1.IsValid(RowAccount) == TRUE)
	{
		return m_Page3.CheckConditional(RowAccount);
	}

	return FALSE;
}

//*******************************************************************

void CBatchPropertySheet::AddWarnings( CStringArray* pArray )
{
	m_Page2.AddWarnings(pArray);
	m_Page5.AddWarnings(pArray);
}

//*******************************************************************


