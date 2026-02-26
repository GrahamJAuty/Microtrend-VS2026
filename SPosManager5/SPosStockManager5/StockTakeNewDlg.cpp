/**********************************************************************/
#include "FilterTypes.h"
#include "StatusProgress.h"
#include "StockFilterArray.h"
#include "StockImportSheetDlg.h"
#include "StockReportSupplierDlg.h"
#include "StocktakeCategorySetDlg.h"
#include "StocktakeCSVArray.h"
#include "StocktakeDlg.h"
/**********************************************************************/
#include "StocktakeNewDlg.h"
/**********************************************************************/

CStocktakeNewDlg::CStocktakeNewDlg(bool bNewStocktake, CCategorySetCSVRecord& CategorySet, CWordArray& array, CWnd* pParent)
	: CSSDialog(CStocktakeNewDlg::IDD, pParent), m_CategorySetOut(CategorySet), m_arraySuppIdxOut(array)
{
	//{{AFX_DATA_INIT(CStocktakeNewDlg)
	//}}AFX_DATA_INIT
	m_bNewStocktake = bNewStocktake;
	m_strComment = "";
	m_strCategory = "";
	m_strSupplier = "";
	m_nStocktakeType = 0;
	m_bActive = TRUE;
	m_bClosingZero = FALSE;
	
	m_CategorySetCustom.CreateCategorySet(0);
	m_CategorySetOut.CreateCategorySet(0);

	for (int n = 0; n <= DataManager.Supplier.GetSize(); n++)
	{
		m_arraySuppIdxCustom.Add(n);
		m_arraySuppIdxOut.Add(n);
	}

	m_pStocktakeArray = NULL;
	m_nSpIdx = 0;
	m_strDate = "";
	m_nExistingStocktakeNo = 0;
}

/**********************************************************************/

void CStocktakeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStocktakeNewDlg)
	DDX_Control(pDX, IDC_CHECK_CLOSING_ZERO, m_checkClosingZero);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDC_BUTTON_CATEGORY, m_buttonCategory);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_ACTIVE, m_comboActive);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_buttonImportSheet);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 40);
	DDX_Radio(pDX, IDC_RADIO_PERIOD, m_nStocktakeType);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStocktakeNewDlg, CDialog)
	//{{AFX_MSG_MAP(CStocktakeNewDlg)
	ON_BN_CLICKED(IDC_BUTTON_CATEGORY, OnButtonCategory)
	ON_BN_CLICKED(IDC_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_BN_CLICKED(IDC_RADIO_PERIOD, OnRadioPeriod)
	ON_BN_CLICKED(IDC_RADIO_YTD, OnRadioYTD)
	ON_BN_CLICKED(IDC_RADIO_APPARENT, OnRadioApparent)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStocktakeNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_checkClosingZero.SetCheck(m_bNewStocktake);

	m_comboActive.AddString( "All items matching filter" );
	m_comboActive.AddString( "Stockpoint item set only" );

	if ( TRUE == m_bNewStocktake )
	{
		SetWindowText ( "Create New Stocktake" );
		FillCategoryCombo();
		FillSupplierCombo();
		m_comboActive.SetCurSel(1);
	}
	else
	{
		SetWindowText("Edit Stocktake");

		m_comboCategory.AddString(m_strCategory);
		m_comboCategory.SetCurSel(0);
		m_comboCategory.EnableWindow(FALSE);
		m_buttonCategory.EnableWindow(FALSE);

		m_comboSupplier.AddString(m_strSupplier);
		m_comboSupplier.SetCurSel(0);
		m_comboSupplier.EnableWindow(FALSE);
		m_buttonSupplier.EnableWindow(FALSE);
		m_checkClosingZero.EnableWindow(FALSE);

		m_comboActive.SetCurSel(m_bActive ? 1 : 0);
		m_comboActive.EnableWindow(FALSE);
	}

	return TRUE;  
}

/**********************************************************************/

void CStocktakeNewDlg::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet(0);
	m_CategorySelector.IncludeAll();
	m_CategorySelector.IncludeCustom();
	m_CategorySelector.IncludeCategorySets();
	m_CategorySelector.FillCategoryCombo(&m_comboCategory);
	m_CategorySelector.ForceSelection(0);
}

/**********************************************************************/

void CStocktakeNewDlg::OnButtonCategory()
{
	CCategorySetCSVRecord setPreEdit, setPostEdit;
	UpdateCategorySet(setPreEdit);

	CCSV csv;
	setPreEdit.ConvertToCSV(csv);
	setPostEdit.ConvertFromCSV(csv);

	CStocktakeCategorySetDlg dlg(setPostEdit, this);
	if (dlg.DoModal() == FALSE)
	{
		return;
	}

	bool bCustom = FALSE;
	for (int n = 1; n <= MAX_CATEGORY; n++)
	{
		if (setPreEdit.GetCategoryStatus(n) != setPostEdit.GetCategoryStatus(n))
		{
			bCustom = TRUE;
			break;
		}
	}

	if (bCustom == TRUE)
	{
		CCSV csv;
		setPostEdit.ConvertToCSV(csv);
		m_CategorySetCustom.ConvertFromCSV(csv);
		m_CategorySelector.ForceSelection(FIRST_CATEGORYSET_FILTER - 2);
	}
}

/**********************************************************************/

void CStocktakeNewDlg::UpdateCategorySet(CCategorySetCSVRecord& CategorySet)
{
	CategorySet.IncludeAllCategories(FALSE);
	int nSelection = m_CategorySelector.GetSelection();

	if (nSelection == FIRST_CATEGORYSET_FILTER - 2)
	{
		CCSV csv;
		m_CategorySetCustom.ConvertToCSV(csv);
		CategorySet.ConvertFromCSV(csv);
	}
	else
	{
		CategorySet.CreateCategorySet(nSelection);
	}
}

/**********************************************************************/

void CStocktakeNewDlg::OnButtonSupplier()
{
	CWordArray arrayPreEdit, arrayPostEdit;
	UpdateSupplierArray(arrayPreEdit);

	for (int n = 0; n < arrayPreEdit.GetSize(); n++)
	{
		arrayPostEdit.Add(arrayPreEdit.GetAt(n));
	}

	CStockReportSupplierDlg dlg(arrayPostEdit);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bCustom = FALSE;
	if (arrayPreEdit.GetSize() != arrayPostEdit.GetSize())
	{
		bCustom = TRUE;
	}
	else
	{
		for (int n = 0; n < arrayPreEdit.GetSize(); n++)
		{
			if (arrayPreEdit.GetAt(n) != arrayPostEdit.GetAt(n))
			{
				bCustom = TRUE;
				break;
			}
		}
	}

	if (bCustom == TRUE)
	{
		m_arraySuppIdxCustom.RemoveAll();
		for (int n = 0; n < arrayPostEdit.GetSize(); n++)
		{
			m_arraySuppIdxCustom.Add(arrayPostEdit.GetAt(n));
		}

		m_comboSupplier.SetCurSel(m_comboSupplier.GetCount() - 1);
	}
}

/**********************************************************************/

void CStocktakeNewDlg::OnRadioPeriod() 
{
	m_checkClosingZero.SetCheck( TRUE );
}

/**********************************************************************/

void CStocktakeNewDlg::OnRadioYTD() 
{
	m_checkClosingZero.SetCheck( TRUE );	
}

/**********************************************************************/

void CStocktakeNewDlg::OnRadioApparent() 
{
	m_checkClosingZero.SetCheck( FALSE );	
}

/**********************************************************************/

void CStocktakeNewDlg::UpdateSupplierArray( CWordArray& array )
{
	array.RemoveAll();

	int nSelection = m_comboSupplier.GetCurSel();
	if (nSelection < 0 || nSelection >= m_comboSupplier.GetCount())
	{
		return;
	}

	int nSupplierFilter = m_comboSupplier.GetItemData( nSelection );
	
	if (nSupplierFilter == 0)
	{
		for (int n = 0; n < m_arraySuppIdxCustom.GetSize(); n++)
		{
			array.Add(m_arraySuppIdxCustom.GetAt(n));
		}
	}
	else
	{
		if (nSupplierFilter < FIRST_SUPPLIERSET_FILTER)
		{
			nSupplierFilter -= 2;
		}

		DataManager.SupplierSet.CreateSuppSelectArray ( nSupplierFilter, array );	
	}
}

/**********************************************************************/

void CStocktakeNewDlg::FillSupplierCombo()
{
	//ITEM DATA 0 =          CUSTOM SUPPLIERS
	//ITEM DATA 1 =		UNSPECIFIED SUPPLIER
	//ITEM DATA 2 =		ALL SUPPLIERS
	//ITEM DATA <= 9999  =	SUPPLIER NUMBER + 2
	//ITEM DATA >= 10000 =	SUPPLIER SET + 10000

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nInsertPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nInsertPos, DataManager.Supplier.GetSuppNo ( nIndex ) + 2 );
	}

	m_comboSupplier.InsertString ( 0, "All" );
	m_comboSupplier.SetItemData ( 0, 2 );

	int nSupplierPos = 1;

	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
	{
		m_comboSupplier.InsertString ( nSupplierPos, DataManager.SupplierSet.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nSupplierPos, FIRST_SUPPLIERSET_FILTER + nIndex );
		nSupplierPos++;
	}

	m_comboSupplier.InsertString ( nSupplierPos, "Unspecified" );
	m_comboSupplier.SetItemData ( nSupplierPos, 1 );
	nSupplierPos = m_comboSupplier.InsertString ( -1, "Custom" );
	m_comboSupplier.SetItemData ( nSupplierPos, 0 );
	m_comboSupplier.SetCurSel(0);
}

/**********************************************************************/

void CStocktakeNewDlg::OnButtonImportSheet()
{
	if (TRUE == m_bNewStocktake)
	{
		CreateImportSheetNewStocktake();
	}
	else
	{
		CreateImportSheetEditStocktake();
	}
}

/**********************************************************************/

void CStocktakeNewDlg::CreateImportSheetNewStocktake()
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	UpdateSettings();

	int nStocktakeIdx = 0;
	int nStocktakeNo = 0;
	m_pStocktakeArray->AddStocktake(nStocktakeIdx, nStocktakeNo);

	if (nStocktakeIdx < 0)
	{
		AfxMessageBox("Unable to create temporary stocktake");
		return;
	}

	CStocktakeCSVRecord Stocktake;
	m_pStocktakeArray->GetAt(nStocktakeIdx, Stocktake);
	Stocktake.SetItemCount(CStocktakeDlg::BeginStocktake(nStocktakeNo, m_CategorySetOut, m_arraySuppIdxOut, m_bActive, m_bClosingZero, FALSE, m_nSpIdx));
	Stocktake.SetStocktakeType(m_nStocktakeType);
	Stocktake.SetClosingStockZeroFlag(m_bClosingZero);
	Stocktake.SetCategory(m_strCategory);
	Stocktake.SetSupplier(m_strSupplier);
	Stocktake.SetActiveOnlyFlag(m_bActive);
	Stocktake.SetComment(m_strComment);
	Stocktake.SetDate(m_strDate);
	m_pStocktakeArray->SetAt(nStocktakeIdx, Stocktake);

	CStockImportSheetDlg dlgImportSheet(m_nSpIdx, nStocktakeNo, this);
	
	if (dlgImportSheet.DoModal() == IDOK)
	{
		CStockFilterArray FilterArray(FILTERTYPE_STOCKTAKE);
		FilterArray.SetSpIdx(m_nSpIdx, 0);
		FilterArray.SetStocktakeFilter(Stocktake.GetStocktakeNo());
		FilterArray.BuildRecordList();

		CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx, Stocktake.GetStocktakeNo());
		CString strFields = dlgImportSheet.GetSavedSettingsLine();
		ImportSheetHelpers.CreateSheet(&FilterArray, strFields, dlgImportSheet.m_nCopies);
	}

	int nSize = DataManager.Stock.GetSize();
	int nSpNo = dbStockpoint.GetSpNo(m_nSpIdx);

	StatusProgress.Lock(TRUE, "Removing Temporary Stocktake");
	for (int nStockIdx = 0; nStockIdx < nSize; nStockIdx++)
	{
		StatusProgress.SetPos(nStockIdx, nSize);

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

		if (StockLevels.GetStocktakeNo() == nStocktakeNo)
		{
			StockLevels.AbandonStocktake(nStocktakeNo);
			DataManager.Stock.SetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);
		}
	}
	StatusProgress.Unlock();

	m_pStocktakeArray -> RemoveStocktake(nStocktakeIdx);
}

/**********************************************************************/

void CStocktakeNewDlg::CreateImportSheetEditStocktake()
{
	//UpdateSettings();

	CStockImportSheetDlg dlgImportSheet(m_nSpIdx, m_nExistingStocktakeNo, this);

	if (dlgImportSheet.DoModal() == IDOK)
	{
		CStockFilterArray FilterArray(FILTERTYPE_STOCKTAKE);
		FilterArray.SetSpIdx(m_nSpIdx, 0);
		FilterArray.SetStocktakeFilter(m_nExistingStocktakeNo);
		FilterArray.BuildRecordList();

		CStockImportSheetHelpers ImportSheetHelpers(m_nSpIdx, m_nExistingStocktakeNo);
		CString strFields = dlgImportSheet.GetSavedSettingsLine();
		ImportSheetHelpers.CreateSheet(&FilterArray, strFields, dlgImportSheet.m_nCopies);
	}
}

/**********************************************************************/

void CStocktakeNewDlg::OnOK() 
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	UpdateSettings();
	EndDialog(IDOK);
}

/**********************************************************************/

void CStocktakeNewDlg::UpdateSettings()
{
	m_bClosingZero = IsTicked(m_checkClosingZero);

	if (TRUE == m_bNewStocktake)
	{
		UpdateCategorySet(m_CategorySetOut);
		UpdateSupplierArray(m_arraySuppIdxOut);
		m_comboCategory.GetWindowText(m_strCategory);
		m_comboSupplier.GetWindowText(m_strSupplier);
		m_bActive = (m_comboActive.GetCurSel() == 1);
	}

	::TrimSpaces(m_strComment, FALSE);
	if (m_strComment == "")
	{
		CString strText1 = m_strCategory;
		if (strText1 == "All")
		{
			strText1 = "All Categories";
		}

		CString strText2 = m_strSupplier;
		if (strText2 == "All")
		{
			strText2 = "All Suppliers";
		}

		strText1 += "/";
		strText1 += strText2;

		m_strComment = strText1;
	}
}

/**********************************************************************/



