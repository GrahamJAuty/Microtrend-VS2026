/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetOfferGroup.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetOfferGroup, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetOfferGroup::CPropertySheetOfferGroup(WORD wSpinID, int nIndex, bool bAllowSpin, bool bWalletStampRule, CWnd* pWndParent)
	: CSpinPropertySheet(bAllowSpin, wSpinID, "", pWndParent), m_OfferGroupRecord(NODE_OFFER_GROUP)
{
	m_pListDataDlg = NULL;

	m_nIndex = nIndex;
	m_bAllowSpin = bAllowSpin;
	m_bWalletStampRule = bWalletStampRule;

	if (TRUE == m_bWalletStampRule)
	{
		InitWalletStampRule();
	}
	else
	{
		InitMixMatch();
	}
}

/**********************************************************************/

void CPropertySheetOfferGroup::InitWalletStampRule()
{
	m_Page1.SetEntryType(OFFERENTRY_PLU);
	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Plu";

	AddPage(&m_Page1);
	m_Page1.SetOfferGroupRecord(&m_OfferGroupRecord);
	m_Page1.SetWalletStampRuleFlag(TRUE);
	DataManager.OfferGroup.GetAt(m_nIndex, m_OfferGroupRecord);
	m_Page1.BuildPluArray();

	SetActivePage(0);
	UpdateTitle();
}

/**********************************************************************/

void CPropertySheetOfferGroup::InitMixMatch()
{
	m_Page1.SetEntryType(OFFERENTRY_PLU);
	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Plu";

	m_Page2.SetEntryType(OFFERENTRY_DEPT);
	m_Page2.m_psp.dwFlags |= PSP_USETITLE;
	m_Page2.m_psp.pszTitle = "Department";

	m_Page3.SetEntryType(OFFERENTRY_DEPT_SAMEPLU);
	m_Page3.m_psp.dwFlags |= PSP_USETITLE;
	m_Page3.m_psp.pszTitle = "Dept (Same PLU)";

	m_Page4.m_psp.dwFlags |= PSP_USETITLE;
	m_Page4.m_psp.pszTitle = "Group";

	m_Page5.SetEntryType(OFFERENTRY_PLU_EXCLUDE);
	m_Page5.m_psp.dwFlags |= PSP_USETITLE;
	m_Page5.m_psp.pszTitle = "Plu (Exclude)";

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);

	m_Page1.SetOfferGroupRecord(&m_OfferGroupRecord);
	m_Page2.SetOfferGroupRecord(&m_OfferGroupRecord);
	m_Page3.SetOfferGroupRecord(&m_OfferGroupRecord);
	m_Page4.SetOfferGroupRecord(&m_OfferGroupRecord);
	m_Page5.SetOfferGroupRecord(&m_OfferGroupRecord);

	DataManager.OfferGroup.GetAt(m_nIndex, m_OfferGroupRecord);

	m_Page1.BuildPluArray();
	m_Page2.BuildDeptArray();
	m_Page3.BuildDeptArray();
	m_Page4.BuildGroupArray();
	m_Page5.BuildPluArray();

	int nActivePage = 0;
	int nGroupNo = m_OfferGroupRecord.GetGroupNo();

	if (GotEntryType(nGroupNo, OFFERENTRY_PLU) == TRUE)
	{
		nActivePage = 0;
	}
	else if (GotEntryType(nGroupNo, OFFERENTRY_DEPT) == TRUE)
	{
		nActivePage = 1;
	}
	else if (GotEntryType(nGroupNo, OFFERENTRY_DEPT_SAMEPLU) == TRUE)
	{
		nActivePage = 2;
	}
	else if (GotEntryType(nGroupNo, OFFERENTRY_GROUP) == TRUE)
	{
		nActivePage = 3;
	}
	else if (GotEntryType(nGroupNo, OFFERENTRY_PLU_EXCLUDE) == TRUE)
	{
		nActivePage = 4;
	}

	SetActivePage(nActivePage);
	UpdateTitle();
}

/**********************************************************************/

bool CPropertySheetOfferGroup::GotEntryType( int nGroupNo, int nEntryType )
{
	int nIdx1, nIdx2;
	DataManager.OfferGroupEntry.GetGroupSubRange( nGroupNo, nEntryType, nIdx1, nIdx2 );
	return ( nIdx2 > nIdx1 );
}

/**********************************************************************/

CPropertySheetOfferGroup::~CPropertySheetOfferGroup()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetOfferGroup, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetOfferGroup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetOfferGroup::SpinNext()
{
	if ( m_nIndex < DataManager.OfferGroup.GetSize() - 1 )
	{
		if (TRUE == m_bWalletStampRule)
		{
			m_Page1.SaveRecord();
		}
		else
		{
			int nPage = GetActiveIndex() + 1;

			switch (nPage)
			{
			case 1:	
				m_Page1.SaveRecord();
				break;

			case 2:	
				m_Page2.SaveRecord();
				break;

			case 3:
				m_Page3.SaveRecord();
				break;

			case 4:	
				m_Page4.SaveRecord();
				break;

			case 5:	
				m_Page5.SaveRecord();
				break;
			}
		}
		
		SaveRecord();
		m_nIndex++;
		DataManager.OfferGroup.GetAt ( m_nIndex, m_OfferGroupRecord );
		UpdateTitle();

		m_Page1.Refresh();

		//WE MUST REFRESH ALL PAGES FOR ORDINARY OFFER GROUPS
		if (FALSE == m_bWalletStampRule)
		{
			m_Page2.Refresh();
			m_Page3.Refresh();
			m_Page4.Refresh();
			m_Page5.Refresh();
		}
		
		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nIndex);
		}
	}	
}

/**********************************************************************/

void CPropertySheetOfferGroup::SpinPrevious()
{
	if ( m_nIndex >= 1 )
	{
		if (TRUE == m_bWalletStampRule)
		{
			m_Page1.SaveRecord();
		}
		else
		{
			int nPage = GetActiveIndex() + 1;

			switch (nPage)
			{
			case 1:
				m_Page1.SaveRecord();
				break;

			case 2:
				m_Page2.SaveRecord();
				break;

			case 3:
				m_Page3.SaveRecord();
				break;

			case 4:
				m_Page4.SaveRecord();
				break;

			case 5:
				m_Page5.SaveRecord();
				break;
			}
		}

		SaveRecord();
		m_nIndex--;
		DataManager.OfferGroup.GetAt ( m_nIndex, m_OfferGroupRecord );
		UpdateTitle();

		m_Page1.Refresh();

		//WE MUST REFRESH ALL PAGES FOR ORDINARY OFFER GROUPS
		if (FALSE == m_bWalletStampRule)
		{
			m_Page2.Refresh();
			m_Page3.Refresh();
			m_Page4.Refresh();
			m_Page5.Refresh();
		}
	
		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nIndex);
		}
	}
}

/**********************************************************************/

void CPropertySheetOfferGroup::SaveRecord()
{
	DataManager.OfferGroup.SetAt ( m_nIndex, m_OfferGroupRecord );

	int nOfferNo = m_OfferGroupRecord.GetGroupNo();
	DataManager.OfferGroupEntry.PurgeGroup( nOfferNo );
	
	m_Page1.SaveOfferGroupEntries();

	if (FALSE == m_bWalletStampRule)
	{
		m_Page2.SaveOfferGroupEntries();
		m_Page3.SaveOfferGroupEntries();
		m_Page4.SaveOfferGroupEntries();
		m_Page5.SaveOfferGroupEntries();
	}
}

/**********************************************************************/

void CPropertySheetOfferGroup::UpdateTitle()
{
	CString strTitle = "";

	if (TRUE == m_bWalletStampRule)
	{
		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
		{
			strTitle.Format( "Edit Purchase Control Group (Group %d)", m_OfferGroupRecord.GetGroupNo() );
		}
		else
		{
			strTitle.Format( "Edit Stamp Offer Group (Group %d)", m_OfferGroupRecord.GetGroupNo() );
		}
	}
	else
	{
		strTitle.Format( "Edit Offer Group (Group %d)", m_OfferGroupRecord.GetGroupNo() );
	}

	SetTitle( strTitle );
}

/**********************************************************************/