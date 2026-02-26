/**********************************************************************/
#include "PropertySheetMixMatch.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetMixMatch, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetMixMatch::CPropertySheetMixMatch(WORD wSpinID, bool bAllowSpin, int nMixMatchIdx, CWnd* pWndParent)
	: CSpinPropertySheet(bAllowSpin, wSpinID, "", pWndParent)
{
	m_pListDataDlg = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetMixMatchRecord(&m_MixMatchRecord);
	AddPage(&m_Page1);
	m_PageArray.Add(1);

	m_Page2.SetMixMatchRecord(&m_MixMatchRecord);
	AddPage(&m_Page2);
	m_PageArray.Add(2);

	m_PageTimeplan.SetMixMatchRecord(&m_MixMatchRecord);

	if ((DataManager.TimePlan.GetSize() != 0) && (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE))
	{
		AddPage(&m_PageTimeplan);
		m_PageArray.Add(3);
	}

	m_nMixMatchIdx = nMixMatchIdx;
	DataManager.MixMatch.GetAt(m_nMixMatchIdx, m_MixMatchRecord);
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetMixMatch::~CPropertySheetMixMatch()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetMixMatch, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetMixMatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetMixMatch::SpinNext()
{
	if (m_nMixMatchIdx < DataManager.MixMatch.GetSize() - 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:
			bUpdate = m_Page1.UpdateRecord();
			break;

		case 2:	
			bUpdate = m_Page2.UpdateRecord();
			break;

		case 3:	
			bUpdate = m_PageTimeplan.UpdateRecord();
			break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nMixMatchIdx++;
		DataManager.MixMatch.GetAt(m_nMixMatchIdx, m_MixMatchRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	
			m_Page1.Refresh();
			break;

		case 2:
			m_Page2.Refresh();
			break;

		case 3:	
			m_PageTimeplan.Refresh();	
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nMixMatchIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetMixMatch::SpinPrevious()
{
	if (m_nMixMatchIdx >= 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:	
			bUpdate = m_Page1.UpdateRecord();
			break;

		case 2:
			bUpdate = m_Page2.UpdateRecord();
			break;

		case 3:	
			bUpdate = m_PageTimeplan.UpdateRecord();
			break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nMixMatchIdx--;
		DataManager.MixMatch.GetAt(m_nMixMatchIdx, m_MixMatchRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	
			m_Page1.Refresh();
			break;

		case 2:	
			m_Page2.Refresh();
			break;

		case 3:	
			m_PageTimeplan.Refresh();
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nMixMatchIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetMixMatch::SaveRecord()
{
	DataManager.MixMatch.SetAt ( m_nMixMatchIdx, m_MixMatchRecord );
}

/**********************************************************************/

void CPropertySheetMixMatch::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "Edit Mix Match (Table %d)", m_MixMatchRecord.GetMixMatchNo() );
	SetTitle( strTitle, 0 );

}

/**********************************************************************/
