//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "BatchUpdate.h"
//$$******************************************************************
#include "BatchPropertySheet.h"
//$$******************************************************************

CBatchPropertySheet::CBatchPropertySheet( const char* szTitle, int nUpdateType, CSQLRowAccountFull& RowAccount, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CWnd* pParentWnd, UINT iSelectPage)
	:CSSPropertySheet(szTitle, pParentWnd, iSelectPage), m_Page2( BalanceMemory), m_Page5( RowAccount, DetailMemory)
{
	m_nUpdateType = nUpdateType;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);										// report on
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page1.SetPointer ( &m_UpdateHelpers, szTitle, nUpdateType );

	if ( nUpdateType == nBATCHUPDATE_UPDATE )				// only required if batch update
	{
		AddPage(&m_Page2);									// balance updates
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
		
		AddPage(&m_Page5);									// detail updates
		m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page5.RememberGroupNo();
	}

	AddPage(&m_Page3);										// conditional
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
}

//$$******************************************************************

CBatchPropertySheet::~CBatchPropertySheet()
{
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertySheet, CPropertySheet)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;
}

//$$******************************************************************

bool CBatchPropertySheet::HaveUpdates()
{
	bool bReply = TRUE;

	if ( m_nUpdateType == nBATCHUPDATE_UPDATE )
	{
		bReply = (m_Page2.HaveUpdates() || m_Page5.HaveUpdates());
	}

	return bReply;
}

//$$******************************************************************
