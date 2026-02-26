/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ListCustomFieldValueDlg.h"
#include "ListDataCategoryDlg.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "PriceHelpers.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PropPageStockGeneral.h"
/**********************************************************************/

CPropPageStockGeneral::CPropPageStockGeneral() : CSSPropertyPage(CPropPageStockGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockGeneral)
	//}}AFX_DATA_INIT
	m_nSubUnits = 0;
	m_dWeight = 0.0;
	m_dWastage = 0.0;
	m_nCategoryFilter = 0;

	m_IDStaticCustomField[0] = IDC_STATIC_CUSTOM_FIELD1;
	m_IDStaticCustomField[1] = IDC_STATIC_CUSTOM_FIELD2;
	m_IDStaticCustomField[2] = IDC_STATIC_CUSTOM_FIELD3;
	m_IDStaticCustomField[3] = IDC_STATIC_CUSTOM_FIELD4;
	m_IDStaticCustomField[4] = IDC_STATIC_CUSTOM_FIELD5;

	m_IDComboCustomField[0] = IDC_COMBO_CUSTOM_FIELD1;
	m_IDComboCustomField[1] = IDC_COMBO_CUSTOM_FIELD2;
	m_IDComboCustomField[2] = IDC_COMBO_CUSTOM_FIELD3;
	m_IDComboCustomField[3] = IDC_COMBO_CUSTOM_FIELD4;
	m_IDComboCustomField[4] = IDC_COMBO_CUSTOM_FIELD5;

	m_IDButtonCustomField[0] = IDC_BUTTON_CUSTOM_FIELD1;
	m_IDButtonCustomField[1] = IDC_BUTTON_CUSTOM_FIELD2;
	m_IDButtonCustomField[2] = IDC_BUTTON_CUSTOM_FIELD3;
	m_IDButtonCustomField[3] = IDC_BUTTON_CUSTOM_FIELD4;
	m_IDButtonCustomField[4] = IDC_BUTTON_CUSTOM_FIELD5;

	m_nCustomFieldCount = 0;
	m_nCustomFieldNo[0] = -1;
	m_nCustomFieldNo[1] = -1;
	m_nCustomFieldNo[2] = -1;
	m_nCustomFieldNo[3] = -1;
	m_nCustomFieldNo[4] = -1;

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
	{
		if ( DataManagerNonDb.CustomFieldNames.CheckName( nFieldNo ) == TRUE )
		{
			DataManager.SetActiveCustomFieldNo(nFieldNo);

			if ( DataManager.GetActiveCustomField() -> GetSize() > 0 )
				m_nCustomFieldNo [ m_nCustomFieldCount++ ] = nFieldNo;
		}
	}

	m_bReadOnly = FALSE;

	CDataManagerInfo info;
	DataManagerNonDb.OpenStockUnit( DB_READONLY, info );
	DataManagerNonDb.OpenSubUnit( DB_READONLY, info );
}

/**********************************************************************/

CPropPageStockGeneral::~CPropPageStockGeneral()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseStockUnit( info );
	DataManagerNonDb.CloseSubUnit( info );
}

/**********************************************************************/

void CPropPageStockGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockGeneral)
	DDX_Control(pDX, IDC_EDIT_STOCK_DESC, m_editDescription);
	DDX_Control(pDX, IDC_EDIT_IMPORTSHEET_CODE, m_editImportSheetCode);
	
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_BUTTON_CATEGORY, m_buttonCategory);

	for ( int n = 0; n < 5; n++ )
	{
		DDX_Control(pDX, m_IDStaticCustomField[n], m_staticCustomField[n] );
		DDX_Control(pDX, m_IDComboCustomField[n], m_comboCustomField[n] );
		DDX_Control(pDX, m_IDButtonCustomField[n], m_buttonCustomField[n] );
	}
	
	DDX_Control(pDX, IDC_STATIC_STOCKUNIT, m_staticStockUnit);
	DDX_Control(pDX, IDC_COMBO_STOCKUNIT, m_comboStockUnit);
	DDX_Control(pDX, IDC_BUTTON_STOCKUNIT, m_buttonStockUnit);
	DDX_Control(pDX, IDC_STATIC_STOCKMATE, m_staticStockmate);
	DDX_Control(pDX, IDC_EDIT_STOCKMATE, m_editStockmate);

	DDX_Control(pDX, IDC_STATIC_SUBUNIT, m_staticSubUnit);
	DDX_Control(pDX, IDC_STATIC_SUBUNITX, m_staticSubUnitX);
	DDX_Control(pDX, IDC_COMBO_SUBUNIT, m_comboSubUnit);
	DDX_Control(pDX, IDC_BUTTON_SUBUNIT, m_buttonSubUnit);

	DDX_Control(pDX, IDC_STATIC_WEIGHT, m_staticWeight);
	DDX_Control(pDX, IDC_STATIC_WEIGHTX, m_staticWeightX);
	DDX_Control(pDX, IDC_COMBO_WEIGHT_UNIT, m_comboUnitWeight);
	DDX_Control(pDX, IDC_STATIC_SELLBY, m_staticSellBy);
	DDX_Control(pDX, IDC_COMBO_WEIGHT_SELL, m_comboSaleWeight);

	DDX_Control(pDX, IDC_STATIC_WASTAGE, m_staticWastage);

	DDX_Control(pDX, IDC_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	
	DDX_Control(pDX, IDC_STATIC_COST, m_staticCost);
	
	DDX_Control(pDX, IDC_CHECK_PLURANGE, m_checkDisablePluRange);
	DDX_Control(pDX, IDC_CHECK_DISABLEAUTORDER, m_checkDisableAutoOrder);
	DDX_Control(pDX, IDC_CHECK_AUTOLABEL, m_checkAutoLabel);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SUBUNIT, m_nSubUnits);
	if ( m_bReadOnly == FALSE ) DDV_MinMaxInt(pDX, m_nSubUnits, Stock::SUYield.Min, Stock::SUYield.Max);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_dWeight);
	if ( m_bReadOnly == FALSE ) DDV_MinMaxDouble(pDX, m_dWeight, Stock::SUWeight.Min, Stock::SUWeight.Max);
	DDX_Text(pDX, IDC_EDIT_WASTAGE, m_dWastage);
	if ( m_bReadOnly == FALSE ) DDV_MinMaxDouble(pDX, m_dWastage, Stock::WastagePercent.Min, Stock::WastagePercent.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockGeneral)
	ON_BN_CLICKED(IDC_BUTTON_CATEGORY, OnButtonCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_FIELD1, OnSelectCustomField1)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_FIELD2, OnSelectCustomField2)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_FIELD3, OnSelectCustomField3)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_FIELD4, OnSelectCustomField4)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_FIELD5, OnSelectCustomField5)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_FIELD1, OnButtonCustomField1)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_FIELD2, OnButtonCustomField2)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_FIELD3, OnButtonCustomField3)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_FIELD4, OnButtonCustomField4)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_FIELD5, OnButtonCustomField5)
	ON_BN_CLICKED(IDC_BUTTON_STOCKUNIT, OnButtonStockUnit)
	ON_BN_CLICKED(IDC_BUTTON_SUBUNIT, OnButtonSubUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_WEIGHT_UNIT, OnSelectUnitWeight)
	ON_EN_KILLFOCUS(IDC_EDIT_WEIGHT, OnKillFocusWeight)
	ON_EN_KILLFOCUS(IDC_EDIT_SUBUNIT, OnKillFocusSubUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageStockGeneral::GetEditCost() { return GetEdit( IDC_EDIT_COST ); }
CEdit* CPropPageStockGeneral::GetEditSubUnit() { return GetEdit( IDC_EDIT_SUBUNIT ); }
CEdit* CPropPageStockGeneral::GetEditWeight() { return GetEdit( IDC_EDIT_WEIGHT ); }
CEdit* CPropPageStockGeneral::GetEditWastage() { return GetEdit( IDC_EDIT_WASTAGE ); }
/**********************************************************************/

BOOL CPropPageStockGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	SubclassEdit(IDC_EDIT_COST, SS_NUM_DP, 8, "%.5f");
	SubclassEdit(IDC_EDIT_SUBUNIT, SS_NUM, 4, "%d");
	SubclassEdit(IDC_EDIT_WEIGHT, SS_NUM_DP, 11, "%.5f");
	SubclassEdit(IDC_EDIT_WASTAGE, SS_NUM_DP, 5, "%.2f");

	m_editDescription.SetLimitText(Stock::Description.Max);
	m_editImportSheetCode.SetLimitText(Stock::ImportSheetCode.Max);
	m_comboStockUnit.LimitText(Stock::StockUnit.Max);
	m_editStockmate.LimitText(Stock::StockmateText.Max);

	FillStockUnitCombo();
	FillSubUnitCombo();
	FillUnitWeightCombo();
	GetRecordData();

	if (m_comboCategory.GetCount() < 2)
	{
		m_comboCategory.EnableWindow(FALSE);
		m_buttonCategory.EnableWindow(FALSE);
	}

	if ((Sysset.IsBarmanSystem() == FALSE) || (EcrmanOptions.GetLabelsProductAutoBatchFlag() == FALSE))
	{
		m_checkAutoLabel.ShowWindow(SW_HIDE);
	}

	for (int n = 0; n < 5; n++)
	{
		if (n < m_nCustomFieldCount)
		{
			int nFieldNo = m_nCustomFieldNo[n];
			m_staticCustomField[n].SetWindowText(DataManagerNonDb.CustomFieldNames.GetName(nFieldNo));
		}
		else
		{
			m_staticCustomField[n].ShowWindow(SW_HIDE);
			m_comboCustomField[n].ShowWindow(SW_HIDE);
			m_buttonCustomField[n].ShowWindow(SW_HIDE);
		}
	}

	int nOffset = 22 * m_nCustomFieldCount;
	MoveControl(&m_staticStockUnit, 7, 98 + nOffset);
	MoveControl(&m_comboStockUnit, 74, 98 + nOffset);
	MoveControl(&m_buttonStockUnit, 207, 98 + nOffset);
	MoveControl(&m_staticStockmate, 7, 118 + nOffset);
	MoveControl(&m_editStockmate, 74, 118 + nOffset);

	if (StockOptions.GetStockmateLinkFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_staticStockmate, FALSE);
		ShowAndEnableWindow(&m_editStockmate, FALSE);
	}
	else
	{
		nOffset += 20;
	}

	if (StockOptions.GetStockSubUnitsFlag() == TRUE)
	{
		MoveControl(&m_staticSubUnit, 7, 128 + nOffset);
		MoveControl(GetEditSubUnit(), 74, 126 + nOffset);
		MoveControl(&m_staticSubUnitX, 121, 128 + nOffset);
		MoveControl(&m_comboSubUnit, 74, 146 + nOffset);
		MoveControl(&m_buttonSubUnit, 207, 146 + nOffset);
		MoveControl(&m_staticWeight, 309, 128 + nOffset);
		MoveControl(GetEditWeight(), 376, 126 + nOffset);
		MoveControl(&m_staticWeightX, 443, 128 + nOffset);
		MoveControl(&m_comboUnitWeight, 455, 126 + nOffset);
		MoveControl(&m_staticSellBy, 341, 148 + nOffset);
		MoveControl(&m_comboSaleWeight, 376, 146 + nOffset);
		MoveControl(&m_staticWastage, 7, 178 + nOffset);
		MoveControl(GetEditWastage(), 74, 176 + nOffset);
		nOffset += 80;
	}
	else
	{
		ShowAndEnableWindow(&m_staticSubUnit, FALSE);
		ShowAndEnableWindow(GetEditSubUnit(), FALSE);
		ShowAndEnableWindow(&m_staticSubUnitX, FALSE);
		ShowAndEnableWindow(&m_comboSubUnit, FALSE);
		ShowAndEnableWindow(&m_buttonSubUnit, FALSE);
		ShowAndEnableWindow(&m_staticWeight, FALSE);
		ShowAndEnableWindow(GetEditWeight(), FALSE);
		ShowAndEnableWindow(&m_staticWeightX, FALSE);
		ShowAndEnableWindow(&m_comboUnitWeight, FALSE);
		ShowAndEnableWindow(&m_staticSellBy, FALSE);
		ShowAndEnableWindow(&m_comboSaleWeight, FALSE);
		ShowAndEnableWindow(&m_staticWastage, FALSE);
		ShowAndEnableWindow(GetEditWastage(), FALSE);
	}

	MoveControl(&m_staticSupplier, 7, 128 + nOffset);
	MoveControl(&m_editSupplier, 74, 126 + nOffset);
	MoveControl(&m_staticCost, 7, 150 + nOffset);
	MoveControl(GetEditCost(), 74, 148 + nOffset);

	SetRecordControls();
	return TRUE;
}

/**********************************************************************/

BOOL CPropPageStockGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageStockGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

void CPropPageStockGeneral::GetRecordData()
{
	if (m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize())
	{
		DataManager.Stock.GetAt(m_nStockIdx, m_StockRecord);

		//EDITABLE FIELDS
		m_editDescription.SetWindowText(m_StockRecord.GetDescription());
		m_editImportSheetCode.SetWindowText(m_StockRecord.GetImportSheetCode());

		FillCategoryCombo();
		m_CategorySelector.ForceSelection(m_StockRecord.GetCategory());

		for (int nPos = 0; nPos < m_nCustomFieldCount; nPos++)
		{
			int nFieldNo = m_nCustomFieldNo[nPos];
			m_nCustomFieldValueNo[nPos] = m_StockRecord.GetCustomFieldValueNo(nFieldNo);
			FillCustomFieldCombo(nPos);
		}

		m_comboStockUnit.SetWindowText(m_StockRecord.GetStockUnitName());
		m_editStockmate.SetWindowTextA(m_StockRecord.GetStockmateText());
		m_checkAutoLabel.SetCheck(m_StockRecord.GetAutoLabelFlag());
		m_checkDisableAutoOrder.SetCheck(m_StockRecord.GetDisableAutoOrderFlag());
		m_checkDisablePluRange.SetCheck(m_StockRecord.GetDisablePluRangeFlag());

		//INFORMATION ONLY FIELDS
		CString strValue;
		strValue.Format("%.*f", 5, m_StockRecord.GetUnitCost());
		GetEditCost()->SetWindowText(strValue);

		if (m_StockRecord.GetSupplierCount() == 0)
		{
			m_editSupplier.SetWindowText("No Supplier");
		}
		else
		{
			int nSuppNo = m_StockRecord.GetSuppNo(0);
			int nSuppIdx;

			if (DataManager.Supplier.FindSupplierByNumber(nSuppNo, nSuppIdx) == TRUE)
			{
				m_editSupplier.SetWindowText(DataManager.Supplier.GetName(nSuppIdx));
			}
			else
			{
				m_editSupplier.SetWindowText("Unknown");
			}
		}

		m_nSubUnits = m_StockRecord.GetSubUnits(TRUE);
		m_dWeight = m_StockRecord.GetSUWeight(TRUE);
		m_dWastage = m_StockRecord.GetWastePercent();
		UpdateData(FALSE);

		UpdateWeightCombos(m_StockRecord.GetUnitWeightType(), m_StockRecord.GetSaleWeightType());

		m_comboSubUnit.SetWindowText(m_StockRecord.GetSubUnitName());
	}
}

/**********************************************************************/

void CPropPageStockGeneral::SetRecordControls()
{
	if ( m_bReadOnly == TRUE )
	{
		m_editDescription.EnableWindow( FALSE );
		m_editImportSheetCode.EnableWindow( FALSE );
		m_comboCategory.EnableWindow( FALSE );
		m_buttonCategory.ShowWindow( SW_HIDE );
		m_comboStockUnit.EnableWindow( FALSE );
		m_buttonStockUnit.ShowWindow( SW_HIDE );
		m_editStockmate.EnableWindow(FALSE);
		m_checkAutoLabel.EnableWindow( FALSE );
		m_checkDisableAutoOrder.EnableWindow( FALSE );
		m_checkDisablePluRange.EnableWindow( FALSE );
		GetEditSubUnit() -> EnableWindow( FALSE );
		m_buttonSubUnit.ShowWindow( SW_HIDE );
		GetEditWeight() -> EnableWindow( FALSE );
		GetEditWastage() -> EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPropPageStockGeneral::OnButtonCategory() 
{
	CDataManagerInfo info;
	if ( DataManager.OpenCategoryBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	int nCategory = m_CategorySelector.GetSelection();

	CListDataCategoryDlg dlg ( nCategory, this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteCategoryBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			if ( dlg.GetListSelection() != -1 )
				nCategory = dlg.GetListSelection() + 1;
		}
	}
	
	DataManager.CloseCategoryBuffer( info );

	FillCategoryCombo();
	m_CategorySelector.ForceSelection ( nCategory );
}

/**********************************************************************/

void CPropPageStockGeneral::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet ( m_nCategoryFilter );
	m_CategorySelector.IncludeCategory ( m_StockRecord.GetCategory(), TRUE );
	m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
}

/**********************************************************************/
void CPropPageStockGeneral::OnSelectCustomField1(){ SelectCustomField(0); }
void CPropPageStockGeneral::OnSelectCustomField2(){ SelectCustomField(1); }
void CPropPageStockGeneral::OnSelectCustomField3(){ SelectCustomField(2); }
void CPropPageStockGeneral::OnSelectCustomField4(){ SelectCustomField(3); }
void CPropPageStockGeneral::OnSelectCustomField5(){ SelectCustomField(4); }
/**********************************************************************/

void CPropPageStockGeneral::SelectCustomField( int nPos )
{
	int nSel = m_comboCustomField[ nPos ].GetCurSel();
	if ( ( nSel >= 1 ) && ( nSel <= m_comboCustomField[ nPos ].GetCount() ) )
		m_nCustomFieldValueNo[ nPos ] = m_comboCustomField[ nPos ].GetItemData( nSel );
	else
		m_nCustomFieldValueNo[ nPos ] = 0;
}

/**********************************************************************/

void CPropPageStockGeneral::FillCustomFieldCombo( int nPos )
{
	int nSel = 0;
	
	int nFieldNo = m_nCustomFieldNo[ nPos ];
	DataManager.SetActiveCustomFieldNo( nFieldNo );
	
	int nCurrentNumber = m_nCustomFieldValueNo[ nPos ];

	m_comboCustomField[ nPos ].ResetContent();
	m_comboCustomField[ nPos ].AddString( "N/A" );
	m_comboCustomField[ nPos ].SetItemData( 0, 0 );

	for ( int n = 0; n < DataManager.GetActiveCustomField() -> GetSize(); n++ )
	{
		int nValueNumber = DataManager.GetActiveCustomField() -> GetNumber( n );
		CString strValueName = DataManager.GetActiveCustomField() -> GetName( n );

		m_comboCustomField[ nPos ].AddString( strValueName );
		m_comboCustomField[ nPos ].SetItemData( n + 1, nValueNumber );
		
		if ( nCurrentNumber == nValueNumber )
			nSel = n + 1;
	}

	m_comboCustomField[ nPos ].SetCurSel( nSel );
	m_nCustomFieldValueNo[ nPos ] = m_comboCustomField[ nPos ].GetItemData( nSel );
}

/**********************************************************************/
void CPropPageStockGeneral::OnButtonCustomField1(){ EditCustomFieldValues(0); }
void CPropPageStockGeneral::OnButtonCustomField2(){ EditCustomFieldValues(1); }
void CPropPageStockGeneral::OnButtonCustomField3(){ EditCustomFieldValues(2); }
void CPropPageStockGeneral::OnButtonCustomField4(){ EditCustomFieldValues(3); }
void CPropPageStockGeneral::OnButtonCustomField5(){ EditCustomFieldValues(4); }
/**********************************************************************/

void CPropPageStockGeneral::EditCustomFieldValues( int nPos ) 
{
	int nFieldNo = m_nCustomFieldNo[ nPos ];

	DataManager.SetActiveCustomFieldNo( nFieldNo );

	if ( DataManager.GetActiveCustomField() -> IsValid() == FALSE )
	{
		Prompter.CustomFieldError();
		return;
	}

	CDataManagerInfo info;
	if ( DataManager.OpenCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	//READING FROM THE FILE MAY CHANGE THE CUSTOM FIELD LIST
	FillCustomFieldCombo( nPos );
		
	CListCustomFieldValueDlg dlg( TRUE, m_nCustomFieldValueNo[ nPos ], this );

	bool bSavedCustomField = FALSE;

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteCustomFieldValueBuffer( nFieldNo, info ) == TRUE )
			bSavedCustomField = TRUE;
		else
			Prompter.WriteError( info );
	}

	DataManager.CloseCustomFieldValueBuffer( nFieldNo, info );

	if ( bSavedCustomField == TRUE )		
	{
		m_nCustomFieldValueNo[ nPos ] = dlg.GetCustomFieldValueNo();
		FillCustomFieldCombo( nPos );
	}
}

/**********************************************************************/

void CPropPageStockGeneral::OnButtonStockUnit() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenStockUnitBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CString strText;
	m_comboStockUnit.GetWindowText( strText );

	CListDataUnitDlg dlg ( DataManagerNonDb.StockUnit, strText, this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteStockUnitBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			if ( dlg.GetSelectedText() != "" )
				strText = dlg.GetSelectedText();
		}
	}

	DataManagerNonDb.CloseStockUnitBuffer( info );
	
	FillStockUnitCombo();
	m_comboStockUnit.SetWindowText( strText );
}

/**********************************************************************/

void CPropPageStockGeneral::OnButtonSubUnit() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSubUnitBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CString strText;
	m_comboSubUnit.GetWindowText( strText );

	CListDataUnitDlg dlg ( DataManagerNonDb.SubUnit, strText, this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteSubUnitBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			if ( dlg.GetSelectedText() != "" )
				strText = dlg.GetSelectedText();
		}
	}

	DataManagerNonDb.CloseSubUnitBuffer( info );
	
	FillSubUnitCombo();
	m_comboSubUnit.SetWindowText( strText );
}

/**********************************************************************/

void CPropPageStockGeneral::OnKillFocusWeight() 
{
	UpdateControls();	
}

/**********************************************************************/

void CPropPageStockGeneral::OnKillFocusSubUnit() 
{
	UpdateControls();
}

/**********************************************************************/

void CPropPageStockGeneral::UpdateControls()
{
	CString strText;
	
	GetEditSubUnit() -> GetWindowText( strText );
	bool bSubUnits = ( atoi ( strText ) > 1 );
	m_comboSubUnit.EnableWindow( bSubUnits );
	m_buttonSubUnit.EnableWindow( bSubUnits );

	GetEditWeight() -> GetWindowText( strText );
	double dWeight =( atof ( strText ) );
	bool bWeight = ( CPriceHelpers::CompareDoubles( dWeight, 0.0, 5 ) != 0 );
	m_comboUnitWeight.EnableWindow( bWeight );
	m_comboSaleWeight.EnableWindow( bWeight && ( m_comboSaleWeight.GetCount() > 1 ) );
}

/**********************************************************************/

bool CPropPageStockGeneral::UpdateRecord()
{
	SaveRecord();
	return TRUE;
}

/**********************************************************************/

void CPropPageStockGeneral::Refresh()
{
	GetRecordData();
	SetRecordControls();
}

/**********************************************************************/

void CPropPageStockGeneral::FillStockUnitCombo()
{
	m_comboStockUnit.ResetContent();
	for ( int nIndex = 0; nIndex < DataManagerNonDb.StockUnit.GetSize(); nIndex++ )
		m_comboStockUnit.AddString ( DataManagerNonDb.StockUnit.GetText ( nIndex ) );
}

/**********************************************************************/

void CPropPageStockGeneral::FillSubUnitCombo()
{
	m_comboSubUnit.ResetContent();
	for ( int nIndex = 0; nIndex < DataManagerNonDb.SubUnit.GetSize(); nIndex++ )
		m_comboSubUnit.AddString ( DataManagerNonDb.SubUnit.GetText ( nIndex ) );
}

/**********************************************************************/

void CPropPageStockGeneral::FillUnitWeightCombo()
{
	m_comboUnitWeight.AddString( "Gallon" );
	m_comboUnitWeight.AddString( "Pint" );
	m_comboUnitWeight.AddString( "Fluid Ounce" );
	m_comboUnitWeight.AddString( "Litre" );
	m_comboUnitWeight.AddString( "Centilitre" );
	m_comboUnitWeight.AddString( "Millilitre" );
	m_comboUnitWeight.AddString( "Kilogram" );
	m_comboUnitWeight.AddString( "Gram" );
	m_comboUnitWeight.SetItemData( 0, WEIGHTTYPE_GALLON );
	m_comboUnitWeight.SetItemData( 1, WEIGHTTYPE_PINT );
	m_comboUnitWeight.SetItemData( 2, WEIGHTTYPE_FLOUNCE );
	m_comboUnitWeight.SetItemData( 3, WEIGHTTYPE_LITRE );
	m_comboUnitWeight.SetItemData( 4, WEIGHTTYPE_CLITRE );
	m_comboUnitWeight.SetItemData( 5, WEIGHTTYPE_MLITRE );
	m_comboUnitWeight.SetItemData( 6, WEIGHTTYPE_KILO );
	m_comboUnitWeight.SetItemData( 7, WEIGHTTYPE_GRAM );
}

/**********************************************************************/

void CPropPageStockGeneral::GetWeightTypes( int& nUnitWeightType, int& nSaleWeightType ) 
{
	int nSel = m_comboUnitWeight.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboUnitWeight.GetCount() )
		nUnitWeightType = m_comboUnitWeight.GetItemData( nSel );
	else
		nUnitWeightType = WEIGHTTYPE_GALLON;

	nSel = m_comboSaleWeight.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboSaleWeight.GetCount() )
		nSaleWeightType = m_comboSaleWeight.GetItemData( nSel );
	else
		nSaleWeightType = WEIGHTTYPE_GALLON;
}

/**********************************************************************/

void CPropPageStockGeneral::OnSelectUnitWeight()
{
	int nUnitWeightType, nSaleWeightType;
	GetWeightTypes( nUnitWeightType, nSaleWeightType );
	UpdateWeightCombos( nUnitWeightType, nSaleWeightType );
}

/**********************************************************************/

void CPropPageStockGeneral::UpdateWeightCombos( int nUnitWeightType, int nSaleWeightType )
{
	CWordArray arrayTypes;
	CStockCSVRecord::GetValidSaleWeights( nUnitWeightType, arrayTypes );

	for ( int i = 0; i < m_comboUnitWeight.GetCount(); i++ )
	{
		if ( nUnitWeightType == (int) m_comboUnitWeight.GetItemData(i) )
		{
			m_comboUnitWeight.SetCurSel(i);
			break;
		}
	}

	m_comboSaleWeight.ResetContent();
	for ( int i = 0; i < arrayTypes.GetSize(); i++ )
	{
		bool bAdd = TRUE;
		int nType = arrayTypes.GetAt(i);
		switch ( nType )
		{
		case WEIGHTTYPE_GALLON:	m_comboSaleWeight.AddString ( "Gallon" );		break;
		case WEIGHTTYPE_PINT:	m_comboSaleWeight.AddString ( "Pint" );			break;
		case WEIGHTTYPE_FLOUNCE:	m_comboSaleWeight.AddString ( "Fluid Ounce" );	break;
		case WEIGHTTYPE_LITRE:	m_comboSaleWeight.AddString ( "Litre" );		break;
		case WEIGHTTYPE_CLITRE:	m_comboSaleWeight.AddString ( "Centilitre" );	break;
		case WEIGHTTYPE_MLITRE:	m_comboSaleWeight.AddString ( "Millilitre" );	break;
		case WEIGHTTYPE_KILO:	m_comboSaleWeight.AddString ( "Kilogram" );		break;
		case WEIGHTTYPE_GRAM:	m_comboSaleWeight.AddString ( "Gram" );			break;
		default:				bAdd = FALSE;									break;
		}

		if ( bAdd == TRUE )
			m_comboSaleWeight.SetItemData( m_comboSaleWeight.GetCount() - 1, nType );
	}

	m_comboSaleWeight.SetCurSel(0);
	for ( int i = 0; i < m_comboUnitWeight.GetCount(); i++ )
	{
		if ( nSaleWeightType == (int) m_comboSaleWeight.GetItemData(i) )
		{
			m_comboSaleWeight.SetCurSel(i);
			break;
		}
	}

	UpdateControls();
}

/**********************************************************************/

void CPropPageStockGeneral::SetStockIdx ( int nStockIdx )
{
	if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
		m_nStockIdx = nStockIdx;
	else
		m_nStockIdx = -1;
}

/**********************************************************************/

void CPropPageStockGeneral::SaveRecord()
{
	if( m_bReadOnly == TRUE )
		return;

	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
	{
		int nCategory = m_CategorySelector.GetSelection();
		m_StockRecord.SetCategory ( nCategory );

		for ( int nPos = 0; nPos < m_nCustomFieldCount; nPos++ )
		{
			int nFieldNo = m_nCustomFieldNo[ nPos ];
			
			SelectCustomField( nPos );
			m_StockRecord.SetCustomFieldValueNo( nFieldNo, m_nCustomFieldValueNo[ nPos ] );
		}
		
		CString strText;
		m_editDescription.GetWindowText( strText );
		m_StockRecord.SetDescription ( strText );

		m_StockRecord.SetImportSheetCode(GetEditBoxText(m_editImportSheetCode));
		
		m_comboStockUnit.GetWindowText( strText );
		m_StockRecord.SetStockUnitName ( strText );

		m_StockRecord.SetStockmateText(GetEditBoxText(m_editStockmate));

		m_comboSubUnit.GetWindowText( strText );
		m_StockRecord.SetSubUnitName ( strText );

		m_StockRecord.SetSubUnits( m_nSubUnits );
		m_StockRecord.SetSUWeight( m_dWeight );
		m_StockRecord.SetWastePercent( m_dWastage );

		int nUnitWeight, nSaleWeight;
		GetWeightTypes( nUnitWeight, nSaleWeight );
		m_StockRecord.SetSUWeightType( nUnitWeight, nSaleWeight );
		
		m_StockRecord.SetAutoLabelFlag ( m_checkAutoLabel.GetCheck() != 0 );
		m_StockRecord.SetDisableAutoOrderFlag ( m_checkDisableAutoOrder.GetCheck() != 0 );
		m_StockRecord.SetDisablePluRangeFlag ( m_checkDisablePluRange.GetCheck() != 0 );
		
		DataManager.Stock.SetAt ( m_nStockIdx, m_StockRecord );
	}
}

/**********************************************************************/

