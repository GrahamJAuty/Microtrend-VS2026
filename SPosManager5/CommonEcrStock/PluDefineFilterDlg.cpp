/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\BarcodePluNoTable.h"
#include "..\CommonEcrStockTray\FilterTypes.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluDefineFilterDlg.h"
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

CPluDefineFilterDlg::CPluDefineFilterDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluDefineFilterDlg::IDD, pParent), 
	m_PluFilterArray ( FilterArray ), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, TRUE )
{
	//{{AFX_DATA_INIT(CPluDefineFilterDlg)
	//}}AFX_DATA_INIT
	m_strExternalTitle = "";
	m_nSearchType = m_PluFilterArray.GetSearchType();
	m_strSearchText = m_PluFilterArray.GetDescription();
	
	CDataManagerInfo info;
	DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
}

/**********************************************************************/

CPluDefineFilterDlg::~CPluDefineFilterDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseEcrmanSupplier(info);
}

/**********************************************************************/
#else
/**********************************************************************/

CPluDefineFilterDlg::CPluDefineFilterDlg(CPluFilterArray& FilterArray, CWnd* pParent)
	: CSSDialog(CPluDefineFilterDlg::IDD, pParent),
	m_PluFilterArray(FilterArray),
	m_DepartmentSelector(m_editDeptNo, m_comboDeptName)
{
	//{{AFX_DATA_INIT(CPluDefineFilterDlg)
	//}}AFX_DATA_INIT
	m_nSearchType = m_PluFilterArray.GetSearchType();
	m_strSearchText = m_PluFilterArray.GetDescription();
}

/**********************************************************************/

CPluDefineFilterDlg::~CPluDefineFilterDlg()
{
}

/**********************************************************************/
#endif
/**********************************************************************/

void CPluDefineFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDefineFilterDlg)
	
	DDX_Control(pDX, IDC_CHECK_MATCHCASE, m_checkMatchCase);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_LOC, m_comboKeyboardLoc);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_NUM, m_comboKeyboardNum);
	/*****/
	DDX_Control(pDX, IDC_STATIC_STOCK_GROUP, m_staticStockGroup);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_STATIC_STOCKCODE, m_staticStockCode);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_CHECK_NONSTOCK, m_checkNonStock);
	DDX_Control(pDX, IDC_CHECK_UNLINKED, m_checkUnlinked);
	DDX_Control(pDX, IDC_CHECK_LINKED, m_checkLinked);
	DDX_Control(pDX, IDC_CHECK_RECIPE, m_checkRecipe);
	/*****/
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	/*****/
	DDX_Radio(pDX, IDC_RADIO_SEARCH_TYPE, m_nSearchType);
	DDX_Text(pDX, IDC_EDIT_SEARCHTEXT, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT); 
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDefineFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDefineFilterDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_KEYBOARD_LOC, OnSelectKeyboardLoc)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	/*****/
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPLIER, OnKillFocusSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDefineFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_strExternalTitle != "")
	{
		SetWindowText(m_strExternalTitle);
	}
	
	SubclassEdit(IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s");
	SubclassEdit(IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s");

	//PLU TEXTS
	m_checkMatchCase.SetCheck(m_PluFilterArray.GetMatchCase());

	//ADDITIONAL FILTERS
	m_editDeptNo.LimitText(MAX_LENGTH_DEPTNO);
	m_DepartmentSelector.FillDepartmentComboDatabase(m_PluFilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL);
	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	m_comboCategory.SelectUsingItemData(m_PluFilterArray.GetAnalysisCategoryFilter());

	//ITEM FILTERS
	m_checkSpecify.SetCheck(m_PluFilterArray.GetPluRangeFlag());

	CString strStart;
	strStart.Format("%14.14I64d", m_PluFilterArray.GetPluRangeStart());
	GetEditPluFrom()->SetWindowText(strStart);

	CString strEnd;
	strEnd.Format("%14.14I64d", m_PluFilterArray.GetPluRangeEnd());
	GetEditPluTo()->SetWindowText(strEnd);

	OnTogglePluRange();

	m_KeyboardFilterSelector.FillCombos(&m_comboKeyboardLoc, &m_comboKeyboardNum);

	if (EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE)
	{
		m_comboKeyboardLoc.SetCurSel(0);
		m_comboKeyboardLoc.EnableWindow(FALSE);
		m_comboKeyboardNum.EnableWindow(FALSE);
	}
	else
	{
		CKeyboardFilterSelectorLoc Loc;
		int nKbNo;
		m_PluFilterArray.GetKeyboardFilter(Loc, nKbNo);
		m_KeyboardFilterSelector.SetSelection(Loc, nKbNo);
	}

	//FLAGS	
	m_checkActive.SetCheck( m_PluFilterArray.GetShowActiveItemsFlag() );
	m_checkInactive.SetCheck( m_PluFilterArray.GetShowInactiveItemsFlag() );
	
	InitStockFields();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPluDefineFilterDlg::GetEditPluFrom() { return GetEdit(IDC_EDIT_PLU_FROM); }
CEdit* CPluDefineFilterDlg::GetEditPluTo() { return GetEdit(IDC_EDIT_PLU_TO); }
/**********************************************************************/

void CPluDefineFilterDlg::OnSelectKeyboardLoc()
{
	m_KeyboardFilterSelector.SelectLocation();
}

/**********************************************************************/

void CPluDefineFilterDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluDefineFilterDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluDefineFilterDlg::OnKillFocusSupplier() 
{
	KillFocusSupplier();
}

/**********************************************************************/

void CPluDefineFilterDlg::OnSelectSupplier() 
{
	SelectSupplier();
}

/**********************************************************************/

void CPluDefineFilterDlg::OnTogglePluRange()
{
	bool bEnable = (m_checkSpecify.GetCheck() != 0);
	GetEditPluFrom()->EnableWindow(bEnable);
	GetEditPluTo()->EnableWindow(bEnable);
	m_buttonBrowseFrom.EnableWindow(bEnable);
	m_buttonBrowseTo.EnableWindow(bEnable);
}

/**********************************************************************/

void CPluDefineFilterDlg::OnKillFocusPluFrom()
{
	CString strPluNo = GetEditBoxText(*GetEditPluFrom());
	GetEditPluFrom()->SetWindowText(BarcodePluNoTable.ConvertPlu(strPluNo));
}

/**********************************************************************/

void CPluDefineFilterDlg::OnButtonBrowseFrom()
{
	CPluFilterArray Filter( FILTERTYPE_BROWSE_PLU);
	Filter.BuildRecordList();

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg(Filter, this, nDlgResult, nPluIdx);

	if (IDOK == nDlgResult)
	{
		if ( ( nPluIdx >= 0 ) && ( nPluIdx < DataManager.Plu.GetSize()))
		{
			GetEditPluFrom() -> SetWindowText( DataManager.Plu.GetPluNoString(nPluIdx) );
		}
	}
}

/**********************************************************************/

void CPluDefineFilterDlg::OnKillFocusPluTo()
{
	CString strPluNo = GetEditBoxText(*GetEditPluTo());
	GetEditPluTo()->SetWindowText(BarcodePluNoTable.ConvertPlu(strPluNo));
}

/**********************************************************************/

void CPluDefineFilterDlg::OnButtonBrowseTo()
{
	CPluFilterArray Filter(FILTERTYPE_BROWSE_PLU);
	Filter.BuildRecordList();

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg(Filter, this, nDlgResult, nPluIdx);

	if (IDOK == nDlgResult)
	{
		if ((nPluIdx >= 0) && (nPluIdx < DataManager.Plu.GetSize()))
		{
			GetEditPluTo()->SetWindowText(DataManager.Plu.GetPluNoString(nPluIdx));
		}
	}
}

/**********************************************************************/

void CPluDefineFilterDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		OnKillFocusSupplier();
		OnKillFocusDeptNo();

		::TrimSpaces(m_strSearchText, FALSE);
		m_PluFilterArray.SetDescriptionFilter(m_strSearchText, m_nSearchType, IsTicked(m_checkMatchCase));

		m_PluFilterArray.SetDepartmentFilter(m_DepartmentSelector.GetDepartmentFilter());
		m_PluFilterArray.SetAnalysisCategoryFilter(CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory));

		CKeyboardFilterSelectorLoc Loc;
		int nKbNo;
		m_KeyboardFilterSelector.GetSelection(Loc, nKbNo);
		m_PluFilterArray.SetKeyboardFilter(Loc, nKbNo);

		m_PluFilterArray.SetPluRangeFlag(IsTicked(m_checkSpecify));
		m_PluFilterArray.SetPluRangeStart(_atoi64(GetEditBoxText(*GetEditPluFrom())));
		m_PluFilterArray.SetPluRangeEnd(_atoi64(GetEditBoxText(*GetEditPluTo())));

		m_PluFilterArray.SetShowActiveItemsFlag(IsTicked(m_checkActive));
		m_PluFilterArray.SetShowInactiveItemsFlag(IsTicked(m_checkInactive));

		SaveStockFields();

		EndDialog(IDOK);
	}
}

/**********************************************************************/
