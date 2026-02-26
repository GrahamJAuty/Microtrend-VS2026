/**********************************************************************/
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CustomPluField.h"
#include "KbdListNameArray.h"
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "PluRangeDateDlg.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "ReportPluCustomList.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangeProgramDlg.h"
/**********************************************************************/

CPluRangeProgramDlg::CPluRangeProgramDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeProgramDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeProgramDlg)
	//}}AFX_DATA_INIT

	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter1);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter2);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter3);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter4);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter5);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter6);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter7);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter8);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter9);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter10);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter11);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter12);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter13);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter14);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter15);
	m_arrayOrderPrinterFields.Add(CustomPluList::OrderPrinter16);

	m_arrayAllergenFields.Add(CustomPluList::Allergen1);
	m_arrayAllergenFields.Add(CustomPluList::Allergen2);
	m_arrayAllergenFields.Add(CustomPluList::Allergen3);
	m_arrayAllergenFields.Add(CustomPluList::Allergen4);
	m_arrayAllergenFields.Add(CustomPluList::Allergen5);
	m_arrayAllergenFields.Add(CustomPluList::Allergen6);

#ifndef STOCKMAN_SYSTEM
	if ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
	}
#endif

	if ( EcrmanOptions.GetFeaturesAllergenFlag() == TRUE )
	{
		m_arrayAllergenTexts.AddNoChangeHeader( TRUE );
		for ( int n = 0; n < DataManagerNonDb.Allergen.GetSize(); n++ )
		{
			CGroupCSVRecordAllergen Allergen;
			DataManagerNonDb.Allergen.GetAt( n, Allergen );

			CString strListText, strCellText;
			strListText.Format( "A%2.2d,%s", Allergen.GetGroupNo(), Allergen.GetReportText() );
			strCellText.Format( "A%2.2d  %s", Allergen.GetGroupNo(), Allergen.GetReportText() );
			m_arrayAllergenTexts.AddText( strListText, strCellText );
		}
	}

#ifdef STOCKMAN_SYSTEM
	CPluRangeStockpointInfo info;
	info.m_strSpName = "As per EPOS link";
	info.m_nSpNo = 0;
	m_arrayStockpointInfo.Add( info );

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );

	for ( int n = nStartSpIdx; n <= nEndSpIdx; n++ )
	{
		CPluRangeStockpointInfo info;
		info.m_strSpName = dbStockpoint.GetName(n);
		info.m_nSpNo = dbStockpoint.GetSpNo(n);
		m_arrayStockpointInfo.Add(info);
	}
#endif
}

/**********************************************************************/

CPluRangeProgramDlg::~CPluRangeProgramDlg()
{
#ifndef STOCKMAN_SYSTEM
	if ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseEcrmanSupplier( info );
	}
#endif
}

/**********************************************************************/

void CPluRangeProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeProgramDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeProgramDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeProgramDlg)
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
	ON_MESSAGE(WM_CELL_BUTTON, OnCellButton)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeProgramDlg::GetEditPluFrom() { return GetEdit ( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeProgramDlg::GetEditPluTo() { return GetEdit ( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluRangeProgramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubclassEdit(IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s");
	SubclassEdit(IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s");

	m_editDeptNo.LimitText(MAX_LENGTH_DEPTNO);
	m_DepartmentSelector.FillDepartmentComboDatabase(m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL);

	if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
	{
		m_comboDeptName.EnableWindow(FALSE);
		m_editDeptNo.EnableWindow(FALSE);
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);

	m_listSettings.SubclassDlgItem(IDC_LIST, this);
	m_listSettings.InsertColumn(1, "Field", LVCFMT_LEFT, 180);
	m_listSettings.InsertColumn(2, "New Value", LVCFMT_LEFT, 260);
	m_listSettings.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listSettings.EnableVScroll(TRUE);

	AddList();

	return TRUE;
}

/**********************************************************************/

void CPluRangeProgramDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listSettings.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

BOOL CPluRangeProgramDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == &m_listSettings)
			{
				m_listSettings.HandleReturn(1);
				pMsg->wParam = NULL;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

bool CPluRangeProgramDlg::CheckField(const char* sz)
{
	if (PriceSetItemManager.IsPluMaster() == TRUE)
	{
		return TRUE;
	}

	if (PriceSetItemManager.CheckField(sz) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CPluRangeProgramDlg::AddList()
{
	m_listSettings.DeleteAllItems();

	//DESCRIPTION AND TYPE
	if ((EcrmanOptions.GetPluDownloadByDateFlag() == TRUE) && (PriceSetItemManager.IsPluMaster() == TRUE))
	{
		CellTypeInfo info;
		info.m_nType = CELL_TYPE_BUTTON;
		info.m_strCurrent = "";
		m_arrayCellTypes.Add(info);

		CCSV csv;
		csv.Add(CustomPluList::DownloadDatePlu.Type);
		csv.Add(CustomPluList::DownloadDatePlu.ListLabel);
		csv.Add(info.m_strCurrent);
		AddField(csv);
	}

	if (DataManager.Department.GetSize() > 0)
	{
		if (CheckField(Plu::DeptNo.Label))
		{
			AddDeptNo();
		}
	}

	if (CheckField(Plu::AnalysisCategory.Label))
	{
		AddAnalysisCategory();
	}

	if (CheckField(Plu::AllowModifier.Label))
	{
		AddYesNoField(CustomPluList::AllowModifier);
	}

	if (CheckField(Plu::MicrotrendPluType.Label))
	{
		AddPluType();
	}

	if (CheckField(Plu::ExternalProcess.Label))
	{
		AddExternalProcess();
	}

	if (CheckField(Plu::TaxBand.Label))
	{
		AddTax();
	}

	//ALLERGENS
	if (EcrmanOptions.GetFeaturesAllergenFlag() == TRUE)
	{
		for (int a = 1; a <= m_arrayAllergenFields.GetSize(); a++)
		{
			CString strLabel = "";
			CPluCSVRecord::GetAllergenLabel(a, strLabel);
			if (CheckField(strLabel)) AddAllergen(m_arrayAllergenFields[a - 1]);
		}
	}

	//LISTS
	if (CheckField(Plu::FirstMenu.Label))
	{
		AddFirstMenu();
	}

	if (CheckField(Plu::FirstQualifier.Label))
	{
		AddFirstQualifier();
	}

	if (CheckField(Plu::FirstExtra.Label))
	{
		AddFirstExtra();
	}

	//ORDER PRINTING
	if (CheckField(Plu::OrderPrinterGroup.Label))
	{
		AddOPGroup();
	}

	for (int n = 1; n <= m_arrayOrderPrinterFields.GetSize(); n++)
	{
		CString strLabel = "";
		CPluCSVRecord::GetOrderPrinterLabel(n, strLabel);
		if (CheckField(strLabel)) AddYesNoField(m_arrayOrderPrinterFields[n - 1]);
	}

	if (CheckField(Plu::Priority.Label))
	{
		AddPriority();
	}

	//MISC PROGRAMMING
	if (CheckField(Plu::PriceType.Label))
	{
		AddPriceType();
	}

	if (CheckField(Plu::AgeVerificationType.Label))
	{
		AddAgeVerificationType();
	}

	if (CheckField(Plu::KPNotAlone.Label))
	{
		AddYesNoField(CustomPluList::KPNotAlone);
	}

	if (CheckField(Plu::KPConsolidateTopItem.Label))
	{
		AddYesNoField(CustomPluList::KPConsolidateTopItem);
	}

	if (CheckField(Plu::KPImagePrint.Label))
	{
		AddYesNoField(CustomPluList::KPImagePrint);
	}

	if (CheckField(Plu::KPImageModifier.Label))
	{
		AddYesNoField(CustomPluList::KPImageModifier);
	}

	if (CheckField(Plu::AutoCoversQty.Label))
	{
		AddYesNoField(CustomPluList::AutoCoversQty);
	}

	if (CheckField(Plu::EANOnReceipt.Label))
	{
		AddYesNoField(CustomPluList::EANOnReceipt);
	}

	if (CheckField(Plu::SalesTracking.Label))
	{
		AddYesNoField(CustomPluList::SalesTracking);
	}

	if (CheckField(Plu::SoldOut.Label))
	{
		AddYesNoField(CustomPluList::SoldOut);
	}

	if (CheckField(Plu::Discountable.Label))
	{
		AddYesNoField(CustomPluList::Discountable);
	}

	if (CheckField(Plu::StaffDiscount.Label))
	{
		AddYesNoField(CustomPluList::StaffDiscount);
	}

	if (CheckField(Plu::FreeText.Label))
	{
		AddFreeText();
	}

	//LABELS
	if (CheckField(Plu::PrintShelf.Label))
	{
		AddYesNoField(CustomPluList::PrintShelf);
	}

	if (CheckField(Plu::PrintProduct.Label))
	{
		AddYesNoField(CustomPluList::PrintProduct);
	}

#ifndef STOCKMAN_SYSTEM

	//ECRMAN STOCK
	if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		if (CheckField(Plu::SupplierNo.Label))
		{
			AddEcrmanSupplier();
		}
	}

#else

	//REDIRECT TO STOCKPOINT
	{
		CString strLabel;
		CPluCSVRecord::GetStockpointPseudoLabel(strLabel);
		if (CheckField(strLabel))
		{
			AddStockpoint();
		}
	}

#endif

	//MISC
	if (CheckField(Plu::PluInfoModifier.Label))
	{
		AddYesNoField(CustomPluList::PluInfoModifier);
	}

	if (CheckField(Plu::ActiveItem.Label))
	{
		AddYesNoField(CustomPluList::ActiveItem);
	}
}

/**********************************************************************/

void CPluRangeProgramDlg::AddYesNoField(const CustomPluField& Field)
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddYesNoTexts();

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(Field.Type);
	csv.Add(Field.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddDeptNo()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int n = 0; n < DataManager.Department.GetSize(); n++)
	{
		CDepartmentCSVRecord Dept;
		DataManager.Department.GetAt(n, Dept);

		CString strListText, strCellText;
		strListText.Format("D%3.3d,%s", Dept.GetDeptNo(), Dept.GetReportText());
		strCellText.Format("D%3.3d  %s", Dept.GetDeptNo(), Dept.GetReportText());
		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::DeptNo.Type);
	csv.Add(CustomPluList::DeptNo.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddTax()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++)
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxBand, TaxRateInfo);

		CString strListText = "";
		strListText.Format("A,%s,%.4lf",
			(const char*)TaxRateInfo.m_strReportText,
			TaxRateInfo.m_dTaxRate);

		CString strCellText = "";
		strCellText.Format("A %s (%.4lf)",
			(const char*)TaxRateInfo.m_strReportText,
			TaxRateInfo.m_dTaxRate);

		strListText.SetAt(0, 'A' + nTaxBand - 1);
		strCellText.SetAt(0, 'A' + nTaxBand - 1);

		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50,150";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::TaxBand.Type);
	csv.Add(EcrmanOptions.GetTaxBandString());
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddPluType()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int n = 0; n <= PluTypeHelpers.GetMaxPluType(); n++)
	{
		int nType = PluTypeHelpers.ComboToMicrotrend(n);
		arrayTexts.AddTabbedText(PluTypeHelpers.GetText(nType));
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::PluType.Type);
	csv.Add(CustomPluList::PluType.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddExternalProcess()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("None");
	arrayTexts.AddTabbedText("1");
	arrayTexts.AddTabbedText("2");
	arrayTexts.AddTabbedText("3");
	arrayTexts.AddTabbedText("4");

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::ExternalProcess.Type);
	csv.Add(CustomPluList::ExternalProcess.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddAnalysisCategory()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("Follow On");

	DataManager.AnalysisCategory.CreateInUseArray();

	for (int i = 0; i < DataManager.AnalysisCategory.GetInUseArraySize(); i++)
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo(i);

		CString strCatName = DataManager.AnalysisCategory.GetDisplayText(nCatNo);

		CString strListText = "";
		strListText.Format("C%2.2d,%s",
			nCatNo,
			(const char*)strCatName);

		CString strCellText = "";
		strCellText.Format("C%2.2d  %s",
			nCatNo,
			(const char*)strCatName);

		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Category.Type);
	csv.Add(CustomPluList::Category.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

void CPluRangeProgramDlg::AddEcrmanSupplier()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("Unspecified");

	for (int n = 0; n < DataManagerNonDb.EcrmanSupplier.GetSize(); n++)
	{
		CEcrmanSupplierCSVRecord Supplier;
		DataManagerNonDb.EcrmanSupplier.GetAt(n, Supplier);

		CString strListText, strCellText;
		strListText.Format("S%3.3d,%s", Supplier.GetSuppNo(), Supplier.GetSuppName());
		strCellText.Format("S%3.3d  %s", Supplier.GetSuppNo(), Supplier.GetSuppName());
		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::SupplierNo.Type);
	csv.Add(CustomPluList::SupplierNo.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/
#endif
/**********************************************************************/

void CPluRangeProgramDlg::AddAllergen(const CustomPluField& Field)
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int n = 0; n < DataManagerNonDb.Allergen.GetSize(); n++)
	{
		CGroupCSVRecordAllergen Allergen;
		DataManagerNonDb.Allergen.GetAt(n, Allergen);

		CString strListText, strCellText;
		strListText.Format("A%3.3d,%s", Allergen.GetGroupNo(), Allergen.GetReportText());
		strCellText.Format("A%3.3d  %s", Allergen.GetGroupNo(), Allergen.GetReportText());
		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(m_arrayAllergenTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(Field.Type);
	csv.Add(Field.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddPriceType()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("Preset");
	arrayTexts.AddTabbedText("Non-Priced");
	arrayTexts.AddTabbedText("Price Check");

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::PriceType.Type);
	csv.Add(CustomPluList::PriceType.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddKbdListNames(CellTypeInfoTextBuffer& arrayTexts)
{
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("None");

	for (int n = 0; n < KbdListNameArray.GetSize(); n++)
	{
		arrayTexts.AddTabbedText(KbdListNameArray.GetListName(n));
	}
}

/**********************************************************************/

void CPluRangeProgramDlg::AddFirstMenu()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames(arrayTexts);

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Menu.Type);
	csv.Add(CustomPluList::Menu.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddFirstQualifier()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames(arrayTexts);

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Qualifier.Type);
	csv.Add(CustomPluList::Qualifier.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddFirstExtra()
{
	CellTypeInfoTextBuffer arrayTexts;
	AddKbdListNames(arrayTexts);

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Extra.Type);
	csv.Add(CustomPluList::Extra.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddAgeVerificationType()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("None");
	arrayTexts.AddTabbedText("16");
	arrayTexts.AddTabbedText("18");
	arrayTexts.AddTabbedText("21");
	arrayTexts.AddTabbedText("25");

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::AgeVerificationType.Type);
	csv.Add(CustomPluList::AgeVerificationType.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddFreeText()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("Text with Description");
	arrayTexts.AddTabbedText("Text Only");
	arrayTexts.AddTabbedText("None");

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::FreeText.Type);
	csv.Add(CustomPluList::FreeText.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddStockpoint()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int n = 0; n < m_arrayStockpointInfo.GetSize(); n++)
	{
		arrayTexts.AddTabbedText(m_arrayStockpointInfo[n].m_strSpName);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Stockpoint.Type);
	csv.Add(CustomPluList::Stockpoint.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddPriority()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);

	for (int n = Plu::Priority.Min; n <= Plu::Priority.Max; n++)
	{
		CString str = "";
		str.Format("%d", n);
		arrayTexts.AddTabbedText(str);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::Priority.Type);
	csv.Add(CustomPluList::Priority.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddOPGroup()
{
	CellTypeInfoTextBuffer arrayTexts;
	arrayTexts.AddNoChangeHeader(TRUE);
	arrayTexts.AddTabbedText("None");

	for (int n = 0; n < DataManagerNonDb.OPGroupSystem.GetOPGroupCount(); n++)
	{
		COPGroupMiniInfo infoMini;
		DataManagerNonDb.OPGroupSystem.GetOPGroup(n, infoMini);

		CString strListText, strCellText;
		strListText.Format("G%2.2d,%s",
			n + 1,
			(const char*)infoMini.m_strName);

		strCellText.Format("G%2.2d  %s",
			n + 1,
			(const char*)infoMini.m_strName);

		arrayTexts.AddText(strListText, strCellText);
	}

	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(arrayTexts);
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(CustomPluList::OrderPrinterGroup.Type);
	csv.Add(CustomPluList::OrderPrinterGroup.ListLabel);
	csv.Add(info.m_strCurrent);
	AddField(csv);
}

/**********************************************************************/

void CPluRangeProgramDlg::AddField(CCSV& csv)
{
	int nPos = m_listSettings.GetItemCount();
	m_listSettings.InsertItem(nPos, "");
	m_listSettings.SetItem(nPos, 0, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL);
	m_listSettings.SetItem(nPos, 1, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL);
	m_listSettings.SetItemData(nPos, csv.GetInt(0));
}

/**********************************************************************/

void CPluRangeProgramDlg::OnKillFocusDeptNumber()
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeProgramDlg::OnSelchangeComboDeptname()
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeProgramDlg::OnKillFocusPluNo()
{
	UpdateData(TRUE, FALSE);
	m_strPluFrom = BarcodePluNoTable.ConvertPlu(m_strPluFrom);
	m_strPluTo = BarcodePluNoTable.ConvertPlu(m_strPluTo);
	UpdateData(FALSE);
}

/**********************************************************************/

void CPluRangeProgramDlg::OnStaticRange()
{
	CString strStart = "1";
	::AddLeading(strStart, Super.MaxPluLen(), '0');
	GetEditPluFrom()->SetWindowText(strStart);

	CString strEnd = "9";
	::AddLeading(strEnd, Super.MaxPluLen(), '9');
	GetEditPluTo()->SetWindowText(strEnd);
}

/**********************************************************************/

LRESULT CPluRangeProgramDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
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
		if (iRowIndex >= 0 && iRowIndex < m_arrayCellTypes.GetSize())
		{
			CCSV csv(m_arrayCellTypes[iRowIndex].m_strComboListText);

			if (m_arrayCellTypes[iRowIndex].m_nType == CELL_TYPE_COMBO_TABBED)
			{
				pComboList->AddTail(m_arrayCellTypes[iRowIndex].m_strComboTabs);
			}

			for (int n = 0; n < csv.GetSize(); n++)
			{
				pComboList->AddTail(csv.GetString(n));
			}
		}
	}

	return true;
}

/**********************************************************************/

LRESULT CPluRangeProgramDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_listSettings.AllowListScroll(TRUE);
	m_listSettings.SetEditCell(-1, -1);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;

	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;

		int nRow = pDispInfo->item.iItem;
		int nCol = pDispInfo->item.iSubItem;

		if (nRow >= 0 && nRow < m_arrayCellTypes.GetSize())
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt(nRow);

			switch (info.m_nType)
			{
			case CELL_TYPE_EDIT:
				switch (info.m_nSSEditType)
				{
					case SS_NUM:
					case SS_NUM_S:
					case SS_BLANK_NUM:
					case SS_BLANK_NUM_S:
					{
						int n = (strText == "") ? -1 : atoi(strText);

						if ((n >= info.m_nMinInt) && (n <= info.m_nMaxInt))
						{
							m_arrayCellTypes[nRow].m_strCurrent = strText;
						}
						else
						{
							m_listSettings.SetItem(nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL);
						}
					}
					break;

					case SS_NUM_DP:
					case SS_NUM_SDP:
					case SS_BLANK_NUM_DP:
					case SS_BLANK_NUM_SDP:
					{
						double d = (strText == "") ? -1.00 : atof(strText);

						if ((d >= info.m_dMinDouble) && (d <= info.m_dMaxDouble))
						{
							m_arrayCellTypes[nRow].m_strCurrent = strText;
						}
						else
						{
							m_listSettings.SetItem(nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL);
						}
					}
					break;
				}
				break;

			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				{
					int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
					int nNewSel = atoi(strText) - 1;
					{
						CCSV csv(info.m_strComboCellText);
						if ((nNewSel >= 0) && (nNewSel < csv.GetSize()))
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
							m_listSettings.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nNewSel), 0, 0, 0, NULL);
						}
						else if ((nOldSel >= 0) && (nOldSel < csv.GetSize()))
						{
							m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
							m_listSettings.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nOldSel), 0, 0, 0, NULL);
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

LRESULT CPluRangeProgramDlg::OnCellButton(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nRow = pCellType -> m_nRow;
	if ( nRow >= 0 && nRow < m_arrayCellTypes.GetSize() )
	{
		CPluRangeDateDlg dlg( m_arrayCellTypes[nRow].m_strCurrent, this );

		if ( dlg.DoModal() == IDOK )
		{
			m_arrayCellTypes[nRow].m_strCurrent = dlg.m_strDate;
			m_listSettings.SetItem ( nRow, 1, LVIF_TEXT, m_arrayCellTypes[nRow].m_strCurrent, 0, 0, 0, NULL );
			m_listSettings.Invalidate();
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPluRangeProgramDlg::GetCellType(WPARAM wParam, LPARAM lParam)
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

bool CPluRangeProgramDlg::IsRelevant()
{
	if (DataManager.Department.GetSize() > 0)
	{
		if (CheckField(Plu::DeptNo.Label))
		{
			return TRUE;
		}
	}

	if (CheckField(Plu::MicrotrendPluType.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::AnalysisCategory.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::TaxBand.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::PriceType.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::Discountable.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::StaffDiscount.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::AgeVerificationType.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::FreeText.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::SalesTracking.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::SoldOut.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::EANOnReceipt.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::KPNotAlone.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::KPConsolidateTopItem.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::KPImagePrint.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::KPImageModifier.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::PluInfoModifier.Label))
	{
		return TRUE;
	}

	if (CheckField(Plu::AllowModifier.Label))
	{
		return TRUE;
	}

	if ( SysInfo.GetMaxBasePluLen() < Super.MaxPluLen() - 1 )
	{
		if (CheckField(Plu::PrintShelf.Label))
		{
			return TRUE;
		}

		if (CheckField(Plu::PrintProduct.Label))
		{
			return TRUE;
		}
	}

#ifdef STOCKMAN_SYSTEM
	{
		CString strLabel;
		CPluCSVRecord::GetStockpointPseudoLabel( strLabel );
		if ( CheckField( strLabel ) )
		{
			return TRUE;
		}
	}
#else

	if (CheckField(Plu::SupplierNo.Label))
	{
		return TRUE;
	}
#endif

	if (CheckField(Plu::OrderPrinterGroup.Label))
	{
		return TRUE;
	}

	for (int n = 1; n <= PLU_ORDER_PRINTERS_NOW; n++)
	{
		CString strLabel = "";
		CPluCSVRecord::GetOrderPrinterLabel(n, strLabel);
		if (CheckField(strLabel) == TRUE)
		{
			return TRUE;
		}
	}

	if (EcrmanOptions.GetFeaturesAllergenFlag() == TRUE)
	{
		for (int n = 1; n <= 6; n++)
		{
			CString strLabel = "";
			CPluCSVRecord::GetAllergenLabel(n, strLabel);

			if (CheckField(strLabel) == TRUE)
			{
				return TRUE;
			}
		}
	}

	if (CheckField(Plu::Priority.Label))
	{
		return TRUE;
	}

	if (EcrmanOptions.GetPluDownloadByDateFlag() == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CPluRangeProgramDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusDeptNumber();
		OnKillFocusPluNo();

		StatusProgress.Lock( TRUE, "Updating PLU programming" );
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

int CPluRangeProgramDlg::UpdateProgram()
{
	int nStartIdx = 0;
	int nRange = 0;
	__int64 nPluFrom = _atoi64(m_strPluFrom);
	__int64 nPluTo = _atoi64(m_strPluTo);
	m_FilterArray.GetArrayIdxRange(nPluFrom, nPluTo, nStartIdx, nRange);

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet(m_DepartmentSelector.GetDepartmentFilter());

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	CString strDownloadDate = "";
	if (EcrmanOptions.GetPluDownloadByDateFlag() == TRUE)
	{
		CString strDate = m_arrayCellTypes[0].m_strCurrent;

		if (strDate.GetLength() == 10)
		{
			COleDateTime date = COleDateTime(atoi(strDate.Right(4)), atoi(strDate.Mid(3, 2)), atoi(strDate.Left(2)), 0, 0, 0);

			if (date.GetStatus() == COleDateTime::valid)
			{
				strDownloadDate.Format("%4.4d%2.2d%2.2d", date.GetYear(), date.GetMonth(), date.GetDay());
			}
		}
	}

	int nCount = 0;
	for (int nOffset = 0; nOffset < nRange; nOffset++)
	{
		StatusProgress.SetPos(nOffset, nRange);

		CPluCSVRecord NewRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt(nStartIdx + nOffset);
		DataManager.Plu.GetAt(nPluIdx, NewRecord);

		__int64 nPluNo = NewRecord.GetPluNoInt();
		if ((nPluNo < nPluFrom) || (nPluNo > nPluTo))
		{
			continue;
		}

		if (DepartmentSet.GetDepartmentStatus(NewRecord.GetBaseDeptNo()) == FALSE)
		{
			continue;
		}

		if ((nCategory != 9999) && (nCategory != NewRecord.GetBaseAnalysisCategory()))
		{
			continue;
		}

		//cannot change program for database record in price set
		if (PriceSetItemManager.IsPluMaster() == FALSE)
		{
			if (PriceSetItemManager.GetPluStatus(nPluIdx) != LOCALPLU_LOCAL)
			{
				continue;
			}
		}

		if (strDownloadDate != "")
		{
			NewRecord.SetDownloadDatePlu(strDownloadDate);
		}

		bool bStockpoint = FALSE;

		for (int n = 0; n < m_listSettings.GetItemCount(); n++)
		{
			if (n >= m_arrayCellTypes.GetSize())
			{
				break;
			}

			CellTypeInfo info = m_arrayCellTypes[n];

			switch (info.m_nType)
			{
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
				break;

			default:
				continue;
			}

			if ((info.m_nComboSelect < 1) || (info.m_nComboSelect > info.m_nMaxInt))
			{
				continue;
			}

			int nCellInt = info.m_nComboSelect - 1;
			int nType = m_listSettings.GetItemData(n);

			bool bMatchedType = FALSE;

			for (int n = 0; n < m_arrayOrderPrinterFields.GetSize(); n++)
			{
				if (nType == m_arrayOrderPrinterFields[n].Type)
				{
					NewRecord.SetOrderPrinterFlag(n, (0 == nCellInt));
					bMatchedType = TRUE;
					break;
				}
			}

			if (TRUE == bMatchedType)
			{
				continue;
			}

			for (int n = 0; n < m_arrayAllergenFields.GetSize(); n++)
			{
				if (nType == m_arrayAllergenFields[n].Type)
				{
					UpdateAllergen(NewRecord, n, nCellInt);
					bMatchedType = TRUE;
					break;
				}
			}

			if (TRUE == bMatchedType)
			{
				continue;
			}

			if (nType == CustomPluList::DeptNo.Type)
			{
				int nDeptIdx = nCellInt;
				if (nDeptIdx >= 0 && nDeptIdx < DataManager.Department.GetSize())
				{
					CDepartmentCSVRecord Dept;
					DataManager.Department.GetAt(nDeptIdx, Dept);
					NewRecord.SetBaseDeptNo(Dept.GetDeptNo());
				}
			}

			else if (nType == CustomPluList::PluType.Type)
			{
				NewRecord.SetMicrotrendPluType(CPluTypeHelpers::ComboToMicrotrend(nCellInt));
			}

			else if (nType == CustomPluList::ExternalProcess.Type)
			{
				NewRecord.SetExternalProcess(nCellInt);
			}

			else if (nType == CustomPluList::Category.Type)
			{
				if (0 == nCellInt)
				{
					NewRecord.SetBaseAnalysisCategory(0);
				}
				else
				{
					NewRecord.SetBaseAnalysisCategory(DataManager.AnalysisCategory.GetInUseCatNo(nCellInt - 1));
				}
			}

#ifndef STOCKMAN_SYSTEM
			else if (nType == CustomPluList::SupplierNo.Type)
			{
				if (0 == nCellInt)
				{
					NewRecord.SetSupplierNo(0);
				}
				else
				{
					CEcrmanSupplierCSVRecord Supplier;
					DataManagerNonDb.EcrmanSupplier.GetAt(nCellInt - 1, Supplier);
					NewRecord.SetSupplierNo(Supplier.GetSuppNo());
				}
			}
#endif

			else if (nType == CustomPluList::Menu.Type)
			{
				if (0 == nCellInt)
				{
					NewRecord.SetFirstMenu(0);
				}
				else
				{
					NewRecord.SetFirstMenu(KbdListNameArray.GetListNo(nCellInt - 1));
				}
			}

			else if (nType == CustomPluList::Qualifier.Type)
			{
				if (0 == nCellInt)
				{
					NewRecord.SetFirstQualifier(0);
				}
				else
				{
					NewRecord.SetFirstQualifier(KbdListNameArray.GetListNo(nCellInt - 1));
				}
			}

			else if (nType == CustomPluList::Extra.Type)
			{
				if (0 == nCellInt)
				{
					NewRecord.SetFirstExtra(0);
				}
				else
				{
					NewRecord.SetFirstExtra(KbdListNameArray.GetListNo(nCellInt - 1));
				}
			}

			else if (nType == CustomPluList::PriceType.Type)
			{
				switch (nCellInt)
				{
				case 0:
					NewRecord.SetPriceType(2);
					break;

				case 1:
					NewRecord.SetPriceType(0);
					break;

				case 2:
					NewRecord.SetPriceType(1);
					break;
				}
			}

			else if (nType == CustomPluList::Discountable.Type)
			{
				NewRecord.SetDiscountableFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::StaffDiscount.Type)
			{
				NewRecord.SetStaffDiscountFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::AllowModifier.Type)
			{
				NewRecord.SetAllowModifierFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::AgeVerificationType.Type)
			{
				switch (nCellInt)
				{
				case 0:
					NewRecord.SetAgeVerificationType(0);
					break;

				case 1:
					NewRecord.SetAgeVerificationType(1);
					break;

				case 2:
					NewRecord.SetAgeVerificationType(2);
					break;

				case 3:
					NewRecord.SetAgeVerificationType(3);
					break;

				case 4:
					NewRecord.SetAgeVerificationType(4);
					break;
				}
			}

			else if (nType == CustomPluList::AutoCoversQty.Type)
			{
				NewRecord.SetAutoCoversQtyFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::KPNotAlone.Type)
			{
				NewRecord.SetKPNotAloneFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::KPConsolidateTopItem.Type)
			{
				NewRecord.SetKPConsolidateTopItemFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::FreeText.Type)
			{
				switch (nCellInt)
				{
				case 0:
					NewRecord.SetFreeTextFlag(TRUE);
					NewRecord.SetFreeTextDescriptionFlag(TRUE);
					break;

				case 1:
					NewRecord.SetFreeTextFlag(TRUE);
					NewRecord.SetFreeTextDescriptionFlag(FALSE);
					break;

				case 2:
					NewRecord.SetFreeTextFlag(FALSE);
					NewRecord.SetFreeTextDescriptionFlag(TRUE);
					break;
				}
			}

			else if (nType == CustomPluList::SalesTracking.Type)
			{
				NewRecord.SetSalesTrackingFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::SoldOut.Type)
			{
				NewRecord.SetSoldOutFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::EANOnReceipt.Type)
			{
				NewRecord.SetEANOnReceiptFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::KPImagePrint.Type)
			{
				NewRecord.SetKPImagePrintFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::KPImageModifier.Type)
			{
				NewRecord.SetKPImageModifierFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::PluInfoModifier.Type)
			{
				NewRecord.SetPluInfoModifierFlag(0 == nCellInt);
			}

			else if (nType == CustomPluList::PrintShelf.Type)
			{
				if (SysInfo.IsModifiableSSPluNo(NewRecord.GetPluNoInt()) == FALSE)
				{
					NewRecord.SetPrintShelfFlag(0 == nCellInt);
				}
			}

			else if (nType == CustomPluList::PrintProduct.Type)
			{
				if (BarcodePluNoTable.IsValidEan(NewRecord.GetPluNoInt()) == TRUE)
				{
					NewRecord.SetPrintProductFlag(0 == nCellInt);
				}
			}

#ifdef STOCKMAN_SYSTEM

			else if (nType == CustomPluList::Stockpoint.Type)
			{
				if ((nCellInt >= 0) && (nCellInt < m_arrayStockpointInfo.GetSize()))
				{
					int nSpNo = m_arrayStockpointInfo[nCellInt].m_nSpNo;

					CRecipeCSVRecord Recipe;
					DataManager.Plu.GetRecipe(nPluIdx, NewRecord.GetPluNoInt(), Recipe);

					if (Recipe.GetSpNo() != nSpNo)
					{
						bStockpoint = TRUE;
						Recipe.SetSpNo(nSpNo);
						DataManager.Plu.SetRecipe(nPluIdx, Recipe);
					}
				}
			}
#endif
			else if (nType == CustomPluList::OrderPrinterGroup.Type)
			{
				NewRecord.SetOrderPrinterGroup(nCellInt);
			}

			else if (nType == CustomPluList::Priority.Type)
			{
				NewRecord.SetPriority(nCellInt);
			}

			else if (nType == CustomPluList::TaxBand.Type)
			{
				NewRecord.SetBaseTaxBandInt(nCellInt + 1);
			}

			else if (nType == CustomPluList::ActiveItem.Type)
			{
				NewRecord.SetActiveItemFlag(0 == nCellInt);
			}
		}

		UpdatePluProfit(NewRecord);

		if ((DataManager.Plu.SavePluRecord(nPluIdx, NewRecord) != 0) || (TRUE == bStockpoint))
		{
			nCount++;
		}
	}

	return nCount;
}

/**********************************************************************/

void CPluRangeProgramDlg::UpdatePluProfit( CPluCSVRecord& PluRecord )
{
	switch ( PluRecord.GetGPType() )
	{
	case PLU_GPTYPE_VARIABLE:
		break;

	case PLU_GPTYPE_FIXED:
		{
			double dNewPrice = 0.0;
			double dCost = DataManager.Plu.GetPluPurchaseCost( PluRecord );
			double dProfit = PluRecord.GetProfitMargin();

			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

			dNewPrice = CPriceHelpers::GetPrice1GPFixed ( TaxRateInfo.m_dTaxRate, dCost, dProfit );

			if (dNewPrice < 0.0)
			{
				dNewPrice = 0.0;
			}

			CString strNewPrice = "";
			strNewPrice.Format ( "%.*f", SysInfo.GetDPValue(), dNewPrice );
			dNewPrice = atof ( strNewPrice );
		
			PluRecord.SetPrice ( 0, dNewPrice );
		}
		break;

	case PLU_GPTYPE_PERCENT:
		{
			double dNewPrice = 0.0;
			double dCost = DataManager.Plu.GetPluPurchaseCost( PluRecord );
			double dProfit = PluRecord.GetProfitMargin();

			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

			dNewPrice = CPriceHelpers::GetPrice1GPPercent ( TaxRateInfo.m_dTaxRate, dCost, dProfit );

			if (dNewPrice < 0.0)
			{
				dNewPrice = 0.0;
			}

			CString strNewPrice = "";
			strNewPrice.Format ( "%.*f", SysInfo.GetDPValue(), dNewPrice );
			dNewPrice = atof ( strNewPrice );
		
			PluRecord.SetPrice ( 0, dNewPrice );
		}
		break;
	}
}

/**********************************************************************/

void CPluRangeProgramDlg::UpdateAllergen( CPluCSVRecord& PluRecord, int nAllergen, int nCellInt )
{
	if ( ( nCellInt >= 0 ) && ( nCellInt < DataManagerNonDb.Allergen.GetSize() ) )
	{
		CBaseGroupInfo info;
		DataManagerNonDb.Allergen.GetGroupInfo( nCellInt, info );
		PluRecord.SetAllergenNo( nAllergen, info.m_nGroupNo );
	}
}

/**********************************************************************/
