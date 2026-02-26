/**********************************************************************/
#include "DefEditTimePlanDlg.h"
#include "NewRecordNumberDlg.h"
/**********************************************************************/
#include "PropPagePromotionTimePlan.h"
/**********************************************************************/

CPropPagePromotionTimePlan::CPropPagePromotionTimePlan() : CSSPropertyPage(CPropPagePromotionTimePlan::IDD),
	m_ListManager(m_listSettings, this)
{
	//{{AFX_DATA_INIT(CPropPagePromotionTimePlan)
	//}}AFX_DATA_INIT
	m_bFiltered = FALSE;
	m_bUpdatingTabs = FALSE;
}

/**********************************************************************/

CPropPagePromotionTimePlan::~CPropPagePromotionTimePlan()
{
}

/**********************************************************************/

void CPropPagePromotionTimePlan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePromotionTimePlan)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePromotionTimePlan, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePromotionTimePlan)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
	ON_MESSAGE(WM_CELL_BUTTON, OnCellButton)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePromotionTimePlan::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_editDescription.LimitText( Promotions::Description.Max );

	m_ListManager.InitList();

	return TRUE;  
}

/**********************************************************************/

void CPropPagePromotionTimePlan::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listSettings.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

LRESULT CPropPagePromotionTimePlan::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandlePopulateComboList(wParam, lParam);
	return true;
}

/**********************************************************************/

LRESULT CPropPagePromotionTimePlan::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleEndLabelEdit(wParam, lParam);
	return 1;
}

/**********************************************************************/

LRESULT CPropPagePromotionTimePlan::OnCellButton(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleCellButton(wParam, lParam);
	return 1;
}

/**********************************************************************/

LRESULT CPropPagePromotionTimePlan::GetCellType(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleGetCellType(wParam, lParam);
	return 1;
}

/**********************************************************************/

BOOL CPropPagePromotionTimePlan::OnKillActive()
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePromotionTimePlan::OnSetActive()
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePromotionTimePlan::SetRecordControls()
{
	m_ListManager.CreateTemplateMap(m_pPromoRecord);
	m_ListManager.AddList();
	m_editDescription.SetWindowText(m_pPromoRecord->GetDisplayName());
	m_editDescription.SetSel(0, -1);
	m_editDescription.SetFocus();
}

/**********************************************************************/

bool CPropPagePromotionTimePlan::UpdateRecord()
{
	SaveRecord();
	return TRUE;
}

/**********************************************************************/

void CPropPagePromotionTimePlan::SaveRecord()
{
	if (FALSE == m_bUpdatingTabs)
	{
		m_pPromoRecord->SetDescription(GetEditBoxText(m_editDescription));
		m_pPromoRecord->SetTimeplanList(m_ListManager.GetTimePlanTemplateList());
	}
}

/**********************************************************************/

void CPropPagePromotionTimePlan::Refresh()
{
	SetRecordControls();
}

/**********************************************************************/