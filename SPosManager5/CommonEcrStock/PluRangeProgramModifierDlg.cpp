/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "KbdListNameArray.h"
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "PluRangeDateDlg.h"
#include "PriceSetItemManager.h"
#include "ReportPluCustomList.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PluRangeProgramModifierDlg.h"
/**********************************************************************/

CPluRangeProgramModifierDlg::CPluRangeProgramModifierDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeProgramModifierDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeProgramModifierDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPluRangeProgramModifierDlg::~CPluRangeProgramModifierDlg()
{
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeProgramModifierDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_CHECK_MODIFIER1, m_checkModifier[1]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER2, m_checkModifier[2]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER3, m_checkModifier[3]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER4, m_checkModifier[4]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER5, m_checkModifier[5]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER6, m_checkModifier[6]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER7, m_checkModifier[7]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER8, m_checkModifier[8]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER9, m_checkModifier[9]);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeProgramModifierDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeProgramModifierDlg)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeProgramModifierDlg::GetEditPluFrom() { return GetEdit ( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeProgramModifierDlg::GetEditPluTo() { return GetEdit ( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluRangeProgramModifierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );
	
	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);

	m_listSettings.SubclassDlgItem ( IDC_LIST, this );
	m_listSettings.InsertColumn ( 1, "Field", LVCFMT_LEFT, 180 );
	m_listSettings.InsertColumn ( 2, "New Value", LVCFMT_LEFT, 260 );
	m_listSettings.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSettings.EnableVScroll( TRUE );

	for ( int nMod = 1; nMod <= 9; nMod++ )
		m_checkModifier[nMod].SetWindowText( DataManager.Modifier.GetDisplayName(nMod) );
	
	AddList();
	
	return TRUE;  
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listSettings.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

BOOL CPluRangeProgramModifierDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listSettings )
		  {
			  m_listSettings.HandleReturn(1);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckField( const char* sz )
{
	if ( PriceSetItemManager.IsPluMaster() == TRUE )
		return TRUE;

	if ( PriceSetItemManager.CheckField( sz ) == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckEnable( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelEnable( nMod, FALSE, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckEposText( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelEposTextFlag( nMod, FALSE, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckKeyText( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelKeyTextFlag( nMod, FALSE, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckDeptNo( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelDeptNo( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckTaxBand( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelTaxBand( nMod, FALSE, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckOPGroup( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelOrderPrinterGroup( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckAnalysisCategory( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelAnalysisCategory( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckPluType( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelMicrotrendPluType( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckFirstMenu( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelFirstMenu( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckFirstQualifier( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelFirstQualifier( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CheckFirstExtra( int nMod )
{
	CString strLabel = "";
	CPluCSVRecord::GetModifierLabelFirstExtra( nMod, strLabel );
	return CheckField( strLabel );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddList()
{
	m_listSettings.DeleteAllItems();

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckEnable( nMod ) == TRUE )
		{
			AddYesNoField( CustomPluList::ModifierEnable );
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckEposText( nMod ) == TRUE )
		{
			AddYesNoField( CustomPluList::ModifierEposText );
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckKeyText( nMod ) == TRUE )
		{
			AddYesNoField( CustomPluList::ModifierKeyText );
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckDeptNo( nMod ) == TRUE )
		{
			AddDeptNo();
			break;
		}
	}
	
	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckAnalysisCategory( nMod ) == TRUE )
		{
			AddAnalysisCategory();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckPluType( nMod ) == TRUE )
		{
			AddPluType();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckTaxBand( nMod ) == TRUE )
		{
			AddTax();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckFirstMenu( nMod ) == TRUE )
		{
			AddFirstMenu();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckFirstQualifier( nMod ) == TRUE )
		{
			AddFirstQualifier();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckFirstExtra( nMod ) == TRUE )
		{
			AddFirstExtra();
			break;
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{	
		if ( CheckOPGroup( nMod ) == TRUE )
		{
			AddOPGroup();
			break;
		}
	}
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddYesNoField( const CustomPluField& Field )
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddYesNoTexts();

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );
	
	CCSV csv;
	csv.Add( Field.Type );
	csv.Add( Field.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddDeptNo()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );
	
	for ( int n = 0; n < DataManager.Department.GetSize(); n++ )
	{
		CDepartmentCSVRecord Dept;
		DataManager.Department.GetAt( n, Dept );

		CString strListText, strCellText;
		strListText.Format( "D%3.3d,%s", Dept.GetDeptNo(), Dept.GetReportText() );
		strCellText.Format( "D%3.3d  %s", Dept.GetDeptNo(), Dept.GetReportText() );
		arrayTexts.AddText( strListText, strCellText );
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( CustomPluList::DeptNo.Type );
	csv.Add( CustomPluList::DeptNo.ListLabel );
	csv.Add( info.m_strCurrent );	
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddTax()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );

	for ( int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

		CString strListText;
		strListText.Format( "A,%s,%.4lf",
			(const char*) TaxRateInfo.m_strReportText,
			TaxRateInfo.m_dTaxRate );
	
		CString strCellText;
		strCellText.Format( "A %s (%.4lf)",
			(const char*) TaxRateInfo.m_strReportText,
			TaxRateInfo.m_dTaxRate );

		strListText.SetAt( 0, 'A' + nTaxBand - 1 );
		strCellText.SetAt( 0, 'A' + nTaxBand - 1 );

		arrayTexts.AddText( strListText, strCellText );
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50,150";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( CustomPluList::TaxBand.Type );
	csv.Add( EcrmanOptions.GetTaxBandString() );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddOPGroup()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );

	for ( int n = 0; n < DataManagerNonDb.OPGroupSystem.GetOPGroupCount(); n++ )
	{
		COPGroupMiniInfo infoMini;
		DataManagerNonDb.OPGroupSystem.GetOPGroup( n, infoMini );

		CString strListText, strCellText;
		strListText.Format( "G%2.2d,%s", n + 1, infoMini.m_strName );
		strCellText.Format( "G%2.2d  %s", n + 1, infoMini.m_strName );
		arrayTexts.AddText( strListText, strCellText );
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( CustomPluList::OrderPrinterGroup.Type );
	csv.Add( CustomPluList::OrderPrinterGroup.ListLabel );
	csv.Add( info.m_strCurrent );	
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddAnalysisCategory()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );
	arrayTexts.AddTabbedText( "Follow On" );

	DataManager.AnalysisCategory.CreateInUseArray();

	for ( int i = 0; i < DataManager.AnalysisCategory.GetInUseArraySize(); i++ )
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo(i);
		CString strCatName = DataManager.AnalysisCategory.GetDisplayText(nCatNo);
		
		CString strListText;
		strListText.Format( "C%2.2d,%s",
			nCatNo,
			(const char*) strCatName );

		CString strCellText;
		strCellText.Format( "C%2.2d  %s",
			nCatNo,
			(const char*) strCatName );

		arrayTexts.AddText( strListText, strCellText );
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( CustomPluList::Category.Type );
	csv.Add( CustomPluList::Category.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddPluType()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );
	arrayTexts.AddTabbedText( "General" );
	arrayTexts.AddTabbedText( "Qualifier" );

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );

	CCSV csv;
	csv.Add( CustomPluList::PluType.Type );
	csv.Add( CustomPluList::PluType.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddKbdListNames( CellTypeInfoTextBuffer& arrayTexts )
{
	arrayTexts.AddNoChangeHeader( TRUE );
	arrayTexts.AddTabbedText( "As Base Plu" );
	arrayTexts.AddTabbedText( "None" );

	for ( int n = 0; n < KbdListNameArray.GetSize(); n++ )
	{
		arrayTexts.AddText( KbdListNameArray.GetListName(n) );
	}
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddFirstMenu()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames( arrayTexts );
	
	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );
	
	CCSV csv;
	csv.Add( CustomPluList::Menu.Type );
	csv.Add( CustomPluList::Menu.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddFirstQualifier()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames( arrayTexts );
	
	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );
	
	CCSV csv;
	csv.Add( CustomPluList::Qualifier.Type );
	csv.Add( CustomPluList::Qualifier.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddFirstExtra()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames( arrayTexts );
	
	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo( arrayTexts );
	m_arrayCellTypes.Add( info );
	
	CCSV csv;
	csv.Add( CustomPluList::Extra.Type );
	csv.Add( CustomPluList::Extra.ListLabel );
	csv.Add( info.m_strCurrent );
	AddField( csv );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::AddField( CCSV& csv )
{
	int nPos = m_listSettings.GetItemCount();
	m_listSettings.InsertItem ( nPos, "" );
	m_listSettings.SetItem ( nPos, 0, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL );
	m_listSettings.SetItem ( nPos, 1, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL );
	m_listSettings.SetItemData ( nPos, csv.GetInt(0) );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

LRESULT CPluRangeProgramModifierDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);
	
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
	
	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_listSettings.GetWindowRect(&obWindowRect);
	
	int iRowIndex = wParam;
	
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if ( iRowIndex >= 0 && iRowIndex < m_arrayCellTypes.GetSize() )
		{
			CCSV csv ( m_arrayCellTypes[iRowIndex].m_strComboListText ); 
	
			if ( m_arrayCellTypes[iRowIndex].m_nType == CELL_TYPE_COMBO_TABBED )
				pComboList -> AddTail( m_arrayCellTypes[iRowIndex].m_strComboTabs );

			for ( int n = 0; n < csv.GetSize(); n++ )
				pComboList -> AddTail( csv.GetString(n) );
		}
	}
	
	return true;
}

/**********************************************************************/

LRESULT CPluRangeProgramModifierDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listSettings.AllowListScroll( TRUE );
	m_listSettings.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( nRow >= 0 && nRow < m_arrayCellTypes.GetSize() )
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt( nRow );

			switch( info.m_nType )
			{
			case CELL_TYPE_EDIT:
				switch( info.m_nSSEditType )
				{
				case SS_NUM:
				case SS_NUM_S:
				case SS_BLANK_NUM:
				case SS_BLANK_NUM_S:
					{
						int n = ( strText == "" ) ? -1 : atoi( strText );

						if ( n >= info.m_nMinInt && n <= info.m_nMaxInt )
							m_arrayCellTypes[nRow].m_strCurrent = strText;
						else
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL );
					}
					break;
				
				case SS_NUM_DP:
				case SS_NUM_SDP:
				case SS_BLANK_NUM_DP:
				case SS_BLANK_NUM_SDP:
					{
						double d = ( strText == "" ) ? -1.00 : atof( strText );

						if ( d >= info.m_dMinDouble && d <= info.m_dMaxDouble )
							m_arrayCellTypes[nRow].m_strCurrent = strText;
						else
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL );
					}
					break;
				}

				
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				{
					int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
					int nNewSel = atoi( strText ) - 1;
					{
						CCSV csv( info.m_strComboCellText );
						if ( ( nNewSel >= 0 ) && ( nNewSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, csv.GetString( nNewSel ), 0, 0, 0, NULL );
						}
						else if ( ( nOldSel >= 0 ) && ( nOldSel < csv.GetSize() ) )
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
							m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, csv.GetString( nOldSel ), 0, 0, 0, NULL );
						}
					}
				}
				break;
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPluRangeProgramModifierDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 1 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	pCellType -> m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;

	return 1;
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::IsRelevant()
{
	CString strLabel;
	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( CheckEnable( nMod ) == TRUE )				return TRUE;
		if ( CheckDeptNo( nMod ) == TRUE )				return TRUE;
		if ( CheckTaxBand( nMod ) == TRUE )				return TRUE;
		if ( CheckOPGroup( nMod ) == TRUE )				return TRUE;
		if ( CheckEposText( nMod ) == TRUE )			return TRUE;
		if ( CheckKeyText( nMod ) == TRUE )				return TRUE;
		if ( CheckAnalysisCategory( nMod ) == TRUE )	return TRUE;
		if ( CheckPluType( nMod ) == TRUE )				return TRUE;
		if ( CheckFirstMenu( nMod ) == TRUE )			return TRUE;
		if ( CheckFirstQualifier( nMod ) == TRUE )		return TRUE;
		if ( CheckFirstExtra( nMod ) == TRUE )			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPluRangeProgramModifierDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		bool bGotModifier = FALSE;
		for ( int n = 1; n <= 9; n++ )
		{
			if ( m_checkModifier[n].GetCheck() != 0 )
			{
				bGotModifier = TRUE;
				break;
			}
		}

		if ( FALSE == bGotModifier )
		{
			Prompter.Error( "You must select at least one modifier to update." );
			return;
		}

		OnKillFocusDeptNumber();
		OnKillFocusPluNo();

		StatusProgress.Lock( TRUE, "Updating Modifier settings" );
		int nCount = UpdateProgram();
		StatusProgress.Unlock();
		
		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were changed" );
			break;

		case 1:
			Prompter.Info ( "1 record was changed" );
			break;

		default:
			{
				CString strBuf;
				strBuf.Format ( "%d records were changed", nCount );
				Prompter.Info ( strBuf );
			}
			break;
		}

		MessageLogger.LogSystem( 2, 10, FALSE, nCount );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

bool CPluRangeProgramModifierDlg::CanChangeModifier( CPluCSVRecord& PluRecord, int nMod )
{
	if ( EcrmanOptions.GetPluDiscardUnusedModifierFlag() == FALSE )
		return TRUE;
	else
		return PluRecord.GetModifierEnable(nMod);
}

/**********************************************************************/

int CPluRangeProgramModifierDlg::UpdateProgram()
{
	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_DepartmentSelector.GetDepartmentFilter() );

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	int nCount = 0;
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange );  
			
		CPluCSVRecord NewRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );
		DataManager.Plu.GetAt ( nPluIdx, NewRecord );

		__int64 nPluNo = NewRecord.GetPluNoInt(); 
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		//cannot change program for database record in price set
		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			if ( PriceSetItemManager.GetPluStatus( nPluIdx ) != LOCALPLU_LOCAL )
				continue;

		for ( int n = 0; n < m_listSettings.GetItemCount(); n++ )
		{
			if ( n >= m_arrayCellTypes.GetSize() )
				break;

			CellTypeInfo info = m_arrayCellTypes[n];

			switch ( info.m_nType )
			{
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				break;

			default:
				continue;
			}

			if ( ( info.m_nComboSelect < 1 ) || ( info.m_nComboSelect > info.m_nMaxInt ) )
				continue;

			int nCellInt = info.m_nComboSelect - 1;
			int nType = m_listSettings.GetItemData( n );

			for ( int nMod = 1; nMod <= 9; nMod++ )
			{
				if ( DepartmentSet.GetDepartmentStatus ( NewRecord.GetModifierDeptNoForReport(nMod) ) == FALSE )
					continue;

				if ((nCategory != 9999) && (nCategory != NewRecord.GetModifierAnalysisCategoryForReport(nMod)))
					continue;

				if ( m_checkModifier[nMod].GetCheck() != 0 )
				{
					if ( nType == CustomPluList::ModifierEnable.Type )
					{
						if ( CheckEnable( nMod ) == TRUE )
							NewRecord.SetModifierEnable( nMod, 0 == nCellInt );
					}

					if ( CanChangeModifier( NewRecord, nMod ) == FALSE )
						continue;

					else if ( nType == CustomPluList::DeptNo.Type )
					{
						int nDeptIdx = nCellInt - 1;
						
						if ( nDeptIdx == -1 )
						{
							NewRecord.SetModifierDeptNoRaw( nMod, -1 );
						}
						if ( nDeptIdx >= 0 && nDeptIdx < DataManager.Department.GetSize() )
						{
							CDepartmentCSVRecord Dept;
							DataManager.Department.GetAt( nDeptIdx, Dept );
							NewRecord.SetModifierDeptNoRaw( nMod, Dept.GetDeptNo() );
						}
					}

					else if ( nType == CustomPluList::ModifierEposText.Type )
					{
						if ( CheckEposText( nMod ) == TRUE )
						{
							CString strText = NewRecord.GetModifierEposText(nMod);
							NewRecord.SetModifierEposText( nMod, 0 == nCellInt, strText );
						}
					}
					else if ( nType == CustomPluList::ModifierKeyText.Type )
					{
						if ( CheckKeyText( nMod ) == TRUE )
						{
							CString strText = NewRecord.GetModifierKeyText(nMod);
							NewRecord.SetModifierKeyText( nMod, 0 == nCellInt, strText );
						}
					}
					else if ( nType == CustomPluList::TaxBand.Type )
					{
						if ( CheckTaxBand( nMod ) == TRUE )
							NewRecord.SetModifierTaxBandIntRaw( nMod, nCellInt );
					}
					else if ( nType == CustomPluList::OrderPrinterGroup.Type )
					{
						if ( CheckOPGroup( nMod ) == TRUE )
							NewRecord.SetModifierOrderPrinterGroup( nMod, nCellInt );
					}
					else if ( nType == CustomPluList::Category.Type )
					{
						if ( CheckAnalysisCategory( nMod ) == TRUE )
						{
							switch( nCellInt )
							{
							case 0:
							case 1:
								NewRecord.SetModifierAnalysisCategoryRaw( nMod, nCellInt - 1 );
								break;

							default:
								NewRecord.SetModifierAnalysisCategoryRaw( nMod, DataManager.AnalysisCategory.GetInUseCatNo( nCellInt - 2 ) );
								break;
							}
						}
					}
					else if ( nType == CustomPluList::PluType.Type )
					{
						if ( CheckPluType( nMod ) == TRUE )
						{
							switch( nCellInt )
							{
							case 0:	NewRecord.SetModifierMicrotrendPluType( nMod, -1 );	break;
							case 1:	NewRecord.SetModifierMicrotrendPluType( nMod, 0 );	break;
							case 2:	NewRecord.SetModifierMicrotrendPluType( nMod, 4 );	break;
							}
						}
					}
					else if ( nType == CustomPluList::Menu.Type )
					{
						if ( CheckFirstMenu( nMod ) == TRUE )
						{
							switch( nCellInt )
							{
							case 0:
							case 1:
								NewRecord.SetModifierFirstMenu( nMod, nCellInt - 1 );
								break;

							default:
								NewRecord.SetModifierFirstMenu( nMod, KbdListNameArray.GetListNo( nCellInt - 2 ) );
								break;
							}
						}
					}
					else if ( nType == CustomPluList::Qualifier.Type )
					{
						if ( CheckFirstQualifier( nMod ) == TRUE )
						{
							switch( nCellInt )
							{
							case 0:
							case 1:
								NewRecord.SetModifierFirstQualifier( nMod, nCellInt - 1 );
								break;

							default:
								NewRecord.SetModifierFirstQualifier( nMod, KbdListNameArray.GetListNo( nCellInt - 2 ) );
								break;
							}
						}
					}
					else if ( nType == CustomPluList::Extra.Type )
					{
						if ( CheckFirstExtra( nMod ) == TRUE )
						{
							switch( nCellInt )
							{
							case 0:
							case 1:
								NewRecord.SetModifierFirstExtra( nMod, nCellInt - 1 );
								break;

							default:
								NewRecord.SetModifierFirstExtra( nMod, KbdListNameArray.GetListNo( nCellInt - 2 ) );
								break;
							}
						}
					}
				}
			}	
		}
		
		if ( DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord ) != 0 )
			nCount++;
	}
	return nCount;
}

/**********************************************************************/

