/**********************************************************************/
#include "OfferGroupHelpers.h"
/**********************************************************************/
#include "PropPageMixMatchTimeplan.h"
/**********************************************************************/

CPropPageMixMatchTimeplan::CPropPageMixMatchTimeplan() : CSSPropertyPage(CPropPageMixMatchTimeplan::IDD),
	m_ListManager(m_listSettings, this)
{
	//{{AFX_DATA_INIT(CPropPageMixMatchTimeplan)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageMixMatchTimeplan::~CPropPageMixMatchTimeplan()
{
}

/**********************************************************************/

void CPropPageMixMatchTimeplan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageMixMatchTimeplan)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageMixMatchTimeplan, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageMixMatchTimeplan)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
	ON_MESSAGE(WM_CELL_BUTTON, OnCellButton)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageMixMatchTimeplan::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_editDescription.LimitText(MixMatch::Description.Max);

	m_ListManager.InitList();

	return TRUE;  
}

/**********************************************************************/

void CPropPageMixMatchTimeplan::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listSettings.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

LRESULT CPropPageMixMatchTimeplan::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandlePopulateComboList(wParam, lParam);
	return true;
}

/**********************************************************************/

LRESULT CPropPageMixMatchTimeplan::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleEndLabelEdit(wParam, lParam);
	return 1;
}

/**********************************************************************/

LRESULT CPropPageMixMatchTimeplan::OnCellButton(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleCellButton(wParam, lParam);
	return 1;
}

/**********************************************************************/

LRESULT CPropPageMixMatchTimeplan::GetCellType(WPARAM wParam, LPARAM lParam)
{
	m_ListManager.HandleGetCellType(wParam, lParam);
	return 1;
}

/**********************************************************************/

BOOL CPropPageMixMatchTimeplan::OnKillActive()
{
	bool bResult = (CPropertyPage::OnKillActive() != 0);
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageMixMatchTimeplan::OnSetActive()
{
	bool bResult = (CPropertyPage::OnSetActive() != 0);
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageMixMatchTimeplan::SetRecordControls()
{
	m_ListManager.CreateTemplateMap(m_pMixMatchRecord);
	m_ListManager.AddList();
	m_checkEnable.SetCheck(m_pMixMatchRecord->GetEnableFlag());
	m_editDescription.SetWindowText(m_pMixMatchRecord->GetDisplayName());
	m_editDescription.SetSel(0, -1);
	m_editDescription.SetFocus();
}

/**********************************************************************/

bool CPropPageMixMatchTimeplan::UpdateRecord()
{
	SaveRecord();
	return TRUE;
}

/**********************************************************************/

void CPropPageMixMatchTimeplan::SaveRecord()
{
	m_pMixMatchRecord->SetDescription(GetEditBoxText(m_editDescription));
	m_pMixMatchRecord->SetEnableFlag(IsTicked(m_checkEnable));
	m_pMixMatchRecord->SetTimeplanList(m_ListManager.GetTimePlanTemplateList());
}

/**********************************************************************/

void CPropPageMixMatchTimeplan::Refresh()
{
	SetRecordControls();
}

/**********************************************************************/
