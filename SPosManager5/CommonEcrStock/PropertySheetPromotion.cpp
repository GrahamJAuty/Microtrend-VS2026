/**********************************************************************/
#include "PropertySheetPromotion.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPromotion, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetPromotion::CPropertySheetPromotion(WORD wSpinID, bool bAllowSpin, int nPromoIdx, CWnd* pWndParent)
	: CSpinPropertySheet(bAllowSpin, wSpinID, "", pWndParent)
{
	m_pListDataDlg = NULL;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_bShowTimePlanPage = ((DataManager.TimePlan.GetSize() != 0) && (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE));

	m_Page1.SetPromotionRecord(&m_PromoRecord);
	m_Page2.SetPromotionRecord(&m_PromoRecord);
	m_Page3.SetPromotionRecord(&m_PromoRecord);
	m_Page4.SetPromotionRecord(&m_PromoRecord);
	m_Page5.SetPromotionRecord(&m_PromoRecord);
	m_PageTimePlan.SetPromotionRecord(&m_PromoRecord);
	m_Page1.SetPropertySheet(this);

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	m_PageArray.Add(1);
	m_PageArray.Add(2);

	if (TRUE == m_bShowTimePlanPage)
	{
		AddPage(&m_PageTimePlan);
		m_PageArray.Add(6);
	}

	m_nPromoIdx = nPromoIdx;
	DataManager.Promotion.GetAt(m_nPromoIdx, m_PromoRecord);
	m_PromoRecord.ValidateFlexSettings();
	m_PromoRecord.TagServiceChargeVATFixBeforeEditing();
	AddFlexTabs();
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPromotion::~CPropertySheetPromotion()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPromotion, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPromotion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPromotion::SpinNext()
{
	if (m_nPromoIdx < DataManager.Promotion.GetSize() - 1)
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
			bUpdate = m_Page3.UpdateRecord();
			break;

		case 4:
			bUpdate = m_Page4.UpdateRecord();
			break;

		case 5:
			bUpdate = m_Page5.UpdateRecord();
			break;

		case 6:
			bUpdate = m_PageTimePlan.UpdateRecord();
			break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nPromoIdx++;
		DataManager.Promotion.GetAt(m_nPromoIdx, m_PromoRecord);
		m_PromoRecord.TagServiceChargeVATFixBeforeEditing();
		UpdateTitle();

		m_Page1.SetUpdatingTabsFlag(TRUE);
		m_Page2.SetUpdatingTabsFlag(TRUE);
		m_Page3.SetUpdatingTabsFlag(TRUE);
		m_Page4.SetUpdatingTabsFlag(TRUE);
		m_Page5.SetUpdatingTabsFlag(TRUE);
		m_PageTimePlan.SetUpdatingTabsFlag(TRUE);

		AddFlexTabs();

		int nActivePageIdx = 0;
		for (int nPageIdx = 1; nPageIdx < m_PageArray.GetSize(); nPageIdx++)
		{
			int nArrayPage = m_PageArray.GetAt(nPageIdx);
			if (nArrayPage == nPage)
			{
				nActivePageIdx = nPageIdx;
				break;
			}
			else if ((3 == nPage) && (4 == nArrayPage))
			{
				nActivePageIdx = nPageIdx;
			}
			else if ((4 == nPage) && (3 == nArrayPage))
			{
				nActivePageIdx = nPageIdx;
			}
		}

		SetActivePage(nActivePageIdx);
		nPage = m_PageArray.GetAt(nActivePageIdx);

		m_Page1.SetUpdatingTabsFlag(FALSE);
		m_Page2.SetUpdatingTabsFlag(FALSE);
		m_Page3.SetUpdatingTabsFlag(FALSE);
		m_Page4.SetUpdatingTabsFlag(FALSE);
		m_Page5.SetUpdatingTabsFlag(FALSE);
		m_PageTimePlan.SetUpdatingTabsFlag(FALSE);

		switch (nPage)
		{
		case 1:
			m_Page1.Refresh();
			break;

		case 2:
			m_Page2.Refresh();
			break;

		case 3:
			m_Page3.Refresh();
			break;

		case 4:
			m_Page4.Refresh();
			break;

		case 5:
			m_Page5.Refresh();
			break;

		case 6:
			m_PageTimePlan.Refresh();
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nPromoIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetPromotion::SpinPrevious()
{
	if (m_nPromoIdx >= 1)
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
			bUpdate = m_Page3.UpdateRecord();
			break;

		case 4:	
			bUpdate = m_Page4.UpdateRecord();	
			break;

		case 5:	
			bUpdate = m_Page5.UpdateRecord();
			break;

		case 6:	
			bUpdate = m_PageTimePlan.UpdateRecord();
			break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nPromoIdx--;
		DataManager.Promotion.GetAt(m_nPromoIdx, m_PromoRecord);
		m_PromoRecord.TagServiceChargeVATFixBeforeEditing();
		UpdateTitle();

		m_Page1.SetUpdatingTabsFlag(TRUE);
		m_Page2.SetUpdatingTabsFlag(TRUE);
		m_Page3.SetUpdatingTabsFlag(TRUE);
		m_Page4.SetUpdatingTabsFlag(TRUE);
		m_Page5.SetUpdatingTabsFlag(TRUE);
		m_PageTimePlan.SetUpdatingTabsFlag(TRUE);

		AddFlexTabs();

		int nActivePageIdx = 0;
		for (int nPageIdx = 1; nPageIdx < m_PageArray.GetSize(); nPageIdx++)
		{
			int nArrayPage = m_PageArray.GetAt(nPageIdx);
			if (nArrayPage == nPage)
			{
				nActivePageIdx = nPageIdx;
				break;
			}
			else if ((3 == nPage) && (4 == nArrayPage))
			{
				nActivePageIdx = nPageIdx;
			}
			else if ((4 == nPage) && (3 == nArrayPage))
			{
				nActivePageIdx = nPageIdx;
			}
		}

		SetActivePage(nActivePageIdx);
		nPage = m_PageArray.GetAt(nActivePageIdx);

		m_Page1.SetUpdatingTabsFlag(FALSE);
		m_Page2.SetUpdatingTabsFlag(FALSE);
		m_Page3.SetUpdatingTabsFlag(FALSE);
		m_Page4.SetUpdatingTabsFlag(FALSE);
		m_Page5.SetUpdatingTabsFlag(FALSE);
		m_PageTimePlan.SetUpdatingTabsFlag(FALSE);

		switch (nPage)
		{
		case 1:
			m_Page1.Refresh();
			break;

		case 2:	
			m_Page2.Refresh();
			break;

		case 3:	
			m_Page3.Refresh();
			break;

		case 4:
			m_Page4.Refresh();
			break;

		case 5:	
			m_Page5.Refresh();
			break;

		case 6:
			m_PageTimePlan.Refresh();
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nPromoIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetPromotion::SaveRecord()
{
	m_PromoRecord.SetServiceChargeVATFixFields();
	DataManager.Promotion.SetAt ( m_nPromoIdx, m_PromoRecord );
}

/**********************************************************************/

void CPropertySheetPromotion::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "Edit Promotion (Table %d)", m_PromoRecord.GetPromoNo() );
	SetTitle ( strTitle, 0 );
}

/**********************************************************************/

void CPropertySheetPromotion::AddFlexTabs()
{
	CByteArray targetPageArray;
	targetPageArray.Add(1);
	targetPageArray.Add(2);

	if (TRUE == m_bShowTimePlanPage)
	{
		targetPageArray.Add(6);
	}

	switch (m_PromoRecord.GetFlexType())
	{
	case 1:
		targetPageArray.Add(3);
		break;

	case 2:
		targetPageArray.Add(4);
		break;
	}

	targetPageArray.Add(5);

	bool bSame = TRUE;

	if (targetPageArray.GetSize() != m_PageArray.GetSize())
	{
		bSame = FALSE;
	}
	else
	{
		for (int n = 0; n < m_PageArray.GetSize(); n++)
		{
			if (m_PageArray.GetAt(n) != targetPageArray.GetAt(n))
			{
				bSame = FALSE;
				break;
			}
		}
	}

	if (FALSE == bSame)
	{
		int nLeftPages = m_bShowTimePlanPage ? 3 : 2;

		for (int n = m_PageArray.GetSize() - 1; n >= nLeftPages; n--)
		{
			RemovePage(n);
			m_PageArray.RemoveAt(n);
		}

		switch (m_PromoRecord.GetFlexType())
		{
		case 1:
			AddPage(&m_Page3);
			m_PageArray.Add(3);
			break;

		case 2:
			AddPage(&m_Page4);
			m_PageArray.Add(4);
			break;
		}

		AddPage(&m_Page5);
		m_PageArray.Add(5);
	}
}

/**********************************************************************/
