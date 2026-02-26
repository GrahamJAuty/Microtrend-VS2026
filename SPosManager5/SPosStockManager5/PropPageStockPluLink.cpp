/**********************************************************************/
#include "EditStockTypes.h"
#include "ExplodePlu.h"
#include "FilterTypes.h"
#include "BarcodePluNoTable.h"
#include "..\CommonEcrStock\PluNumberDlg.h"
#include "PropertySheetPluStockman.h"
#include "RecipeProfitSheet.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "ReportHelpers.h"
#include "StockAuditBaseReport.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageStockPluLink.h"
/**********************************************************************/

CPropPageStockPluLink::CPropPageStockPluLink() : CPropertyPage(CPropPageStockPluLink::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockPluLink)
	//}}AFX_DATA_INIT
	m_bReadOnly = FALSE;
}

/**********************************************************************/

CPropPageStockPluLink::~CPropPageStockPluLink()
{
}

/**********************************************************************/

void CPropPageStockPluLink::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockPluLink)
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_BUTTON_EDITRECIPE, m_buttonEditRecipe);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockPluLink, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockPluLink)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_EDITRECIPE, OnButtonEditRecipe)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECIPE, OnDoubleClickListRecipe)
	ON_BN_CLICKED(IDC_BUTTON_GP, OnButtonGP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageStockPluLink::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listRedirections.SubclassDlgItem ( IDC_LIST, this );
	m_listRedirections.InsertColumn ( 0, "Plu Number", LVCFMT_LEFT, 110 );
	m_listRedirections.InsertColumn ( 1, "Description", LVCFMT_LEFT, 240 );
	m_listRedirections.InsertColumn ( 2, "Department", LVCFMT_LEFT, 200 );
	m_listRedirections.InsertColumn ( 3, "SU Retail", LVCFMT_RIGHT, 90 );
	m_listRedirections.InsertColumn ( 4, "SU GP", LVCFMT_RIGHT, 90 );
	m_listRedirections.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listRecipes.SubclassDlgItem ( IDC_LIST_RECIPE, this );
	m_listRecipes.InsertColumn ( 0, "Plu Number", LVCFMT_LEFT, 110 );
	m_listRecipes.InsertColumn ( 1, "Description", LVCFMT_LEFT, 240 );
	m_listRecipes.InsertColumn ( 2, "Department", LVCFMT_LEFT, 200 );
	m_listRecipes.InsertColumn ( 3, "SU Retail", LVCFMT_RIGHT, 90 );
	m_listRecipes.InsertColumn ( 4, "SU GP", LVCFMT_RIGHT, 90 );
	m_listRecipes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AddList(0);
	AddListRecipe();
	SetRecordControls();
	return TRUE;   
}

/**********************************************************************/

void CPropPageStockPluLink::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST:
		m_listRedirections.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_RECIPE:
		m_listRecipes.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

BOOL CPropPageStockPluLink::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	
	//UPDATE PLU PRICES TO MATCH STOCK
	for ( int nIndex = 0; nIndex < m_IndexArray.GetSize(); nIndex++ )
	{
		int nPluIdx = m_IndexArray.GetAt ( nIndex );
		CPluCSVRecord NewRecord;
		DataManager.Plu.GetAt ( nPluIdx, NewRecord );
		
		DataManager.Plu.UpdatePluPrice ( NewRecord, 0.0, TRUE );
		
		m_pStockChangesBuffer -> LogPluChange ( NewRecord.GetPluNoInt() );
		DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord );
	}

	//UPDATE PLU PRICES TO MATCH STOCK
	for ( int nIndex = 0; nIndex < m_IndexArrayRecipe.GetSize(); nIndex++ )
	{
		int nPluIdx = m_IndexArrayRecipe.GetAt ( nIndex );
		CPluCSVRecord NewRecord;
		DataManager.Plu.GetAt ( nPluIdx, NewRecord );
		
		DataManager.Plu.UpdatePluPrice ( NewRecord, 0.0, TRUE );
		
		m_pStockChangesBuffer -> LogPluChange ( NewRecord.GetPluNoInt() );
		DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord );
	}
		
	AddList(0);
	AddListRecipe();
	SetRecordControls();	

	return bResult;
}

/**********************************************************************/

BOOL CPropPageStockPluLink::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

void CPropPageStockPluLink::OnButtonAdd() 
{
	CString strStockCode = m_StockRecord.GetStockCode();
	
	CPluNumberDlg dlg ( "Add Plu", TRUE, this );
	
	if ( dlg.DoModal() != IDOK )
		return;

	CString strPluNo = BarcodePluNoTable.ConvertPlu( dlg.m_strPluNumber );
	__int64 nPluNo = _atoi64( strPluNo );
	
	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
	{
		CRecipeCSVRecord Recipe;
		DataManager.Plu.GetRecipe ( nPluIdx, nPluNo, Recipe );

		if ( Recipe.IsRecipe() )
		{
			Prompter.Error ( "The selected Plu is a recipe and\ncannot be used for redirection" );
			return;
		}

		if ( Recipe.GetSize() == 1 )
		{
			//FIND OUT WHICH STOCK CODE THIS PLU IS LINKED TO
			CString strPluStockCode = Recipe.GetStockCode(0);

			//PLU THINKS IT IS ALREADY LINKED TO THE CURRENT
			//STOCK CODE SO SEE IF THE STOCK CODE AGREES
			if ( strPluStockCode == strStockCode )
			{
				for ( int nPos = 0; nPos < m_IndexArray.GetSize(); nPos++ )
				{
					if ( m_IndexArray.GetAt ( nPos ) == ( WORD ) nPluIdx )
					{
						Prompter.Error ( "The selected Plu is already\nlinked to this stock item" );
						return;
					}
				}
			
				DataManager.StockPlu.AddLink ( strStockCode, nPluNo );
			
				GetRecordData();
				AddList( nPluNo );
				return;
			}

			//PLU IS LINKED TO AN EXISTING STOCK CODE
			CString strMessage;
			strMessage.Format ( "Plu %s is already\nredirected to stock code %s.\n\nWould you like to redirect it to\nstock code %s instead.",
				( const char* ) strPluNo,
				( const char* ) strPluStockCode,
				( const char* ) strStockCode );

			if ( Prompter.YesNo ( strMessage ) == IDNO )
				return;	
			
			//REMOVE LINK TO PLU FROM PREVIOUS STOCK RECORD
			m_pStockChangesBuffer -> LogStockPluChange ( strPluStockCode );
			DataManager.StockPlu.RemoveLink ( strPluStockCode, nPluNo );
		}

		//ADD LINK TO PLU FROM THIS STOCK RECORD
		DataManager.StockPlu.AddLink ( strStockCode, nPluNo );

		//ADD LINK TO THIS STOCK RECORD FROM PLU
		m_pStockChangesBuffer -> LogPluChange ( nPluNo ); 
		CRecipeCSVRecord RecipeRecord;
		RecipeRecord.SetRecipePluNo ( strPluNo );
		RecipeRecord.AddStockCode ( strStockCode, 1.0 );
		DataManager.Plu.SetRecipe ( nPluIdx, RecipeRecord );
				
		//ADD PLU LINK TO ONSCREEN LIST
		GetRecordData();
		AddList( nPluNo );		
	}
	else
	{
		int nMaxBaseLen = SysInfo.GetMaxBasePluLen();
		if ( nMaxBaseLen != 0 )
		{
			CString strTestPluNo;
			strTestPluNo.Format( "%I64d", nPluNo );

			if ( strTestPluNo.GetLength() == nMaxBaseLen + 1 )
			{
				CString strMessage;
				strMessage.Format( "%s is not configured to allow %d digit PLU items.", 
					AfxGetAppName(),
					nMaxBaseLen + 1 );
					
				Prompter.ContactDealer( strMessage );
				return;
			}
		}

		CString strMsg = "There is no item with the plu number\n";
		strMsg += strPluNo;
		strMsg += " in the plu database.\n\n";
		strMsg += "Would you like to create this item now?";

		if ( Prompter.YesNo ( strMsg ) == IDYES )
		{
			CPluCSVRecord NewPluRecord;
			NewPluRecord.SetPluNo ( nPluNo );
			
			//ADD LINK TO THIS STOCK RECORD FROM PLU
			m_pStockChangesBuffer -> LogPluChange ( nPluNo );
				
			NewPluRecord.SetDefaultText();	
			NewPluRecord.SetModifiedDateTimePlu();
				
			CString strDescription = m_StockRecord.GetDescription();

			if ( strDescription != "" )
			{
				NewPluRecord.SetEposText ( strDescription );
				NewPluRecord.SetRepText ( strDescription );
			}
			
			//ADD NEW PLU TO DATABASE
			DataManager.Plu.InsertRecord ( NewPluRecord, strStockCode, TRUE );
				
			//add new plu to redirection list
			DataManager.StockPlu.AddLink ( strStockCode, nPluNo );
			
			GetRecordData();
			AddList( NewPluRecord.GetPluNoInt() );
			
			//edit new plu
			OnButtonEdit();
		}
	}
}

/**********************************************************************/

void CPropPageStockPluLink::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageStockPluLink::OnButtonEdit() 
{
	int nSelection = m_listRedirections.GetCurSel();
	
	if ( nSelection < 0 || nSelection >= m_IndexArray.GetSize() )
		return;
	
	int nPluIdx = m_IndexArray.GetAt ( nSelection );

	CPluCSVRecord Record;
	DataManager.Plu.GetAt ( nPluIdx, Record );
	m_pStockChangesBuffer -> LogPluChange ( Record.GetPluNoInt() ); 

	CPluFilterArray TempFilter ( FILTERTYPE_DATABASE );

	TempFilter.SetArrayIdx ( nPluIdx );

	CPropertySheetPluStockmanInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pPluFilter = &TempFilter;
	info.m_nEditMode = EDIT_STOCK_SALEQTY;
	info.m_bNewStock = FALSE;
	info.m_bReadOnly = m_bReadOnly;
	
	CPropertySheetPluStockman propSheet ( info, this );
	
	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();
		
	AddList( Record.GetPluNoInt() );
}

/**********************************************************************/

void CPropPageStockPluLink::OnDoubleClickListRecipe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditRecipe();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageStockPluLink::OnButtonEditRecipe() 
{
	int nSelection = m_listRecipes.GetCurSel();
	
	if ( nSelection < 0 || nSelection >= m_IndexArrayRecipe.GetSize() )
		return;
	
	int nPluIdx = m_IndexArrayRecipe.GetAt ( nSelection );

	CPluCSVRecord Record;
	DataManager.Plu.GetAt ( nPluIdx, Record );
	m_pStockChangesBuffer -> LogPluChange ( Record.GetPluNoInt() ); 
	
	CPluFilterArray TempFilter ( FILTERTYPE_DATABASE );

	TempFilter.SetArrayIdx ( nPluIdx );

	CPropertySheetPluStockmanInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pPluFilter = &TempFilter;
	info.m_nEditMode = EDIT_STOCK_SALEQTY;
	info.m_bNewStock = FALSE;
	info.m_bReadOnly = m_bReadOnly;
	
	CPropertySheetPluStockman propSheet ( info, this );
	
	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();
	
	AddListRecipe();
	m_listRecipes.SetCurSel ( nSelection );
}

/**********************************************************************/

void CPropPageStockPluLink::OnButtonGP() 
{
	CRecipeProfitSheet ProfitSheet;
	int nResult = ProfitSheet.CreateProfitSheet( m_nStockIdx );

	switch ( nResult )
	{
	case 0:
		{
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), "", "", this );
		}
		break;

	case 1:
		Prompter.Error( "You have not set up any Plu items\nto sell this stock item." );
		break;

	default:
		{
			CString strMsg;
			strMsg.Format ( "Unable to create report\n(error code %d)", nResult );
			Prompter.Error( strMsg );
		}
		break;
	}
}

/**********************************************************************/

void CPropPageStockPluLink::OnButtonRemove() 
{
	int nSelection = m_listRedirections.GetCurSel();
	CString strStockCode = m_StockRecord.GetStockCode();

	if ( nSelection != -1 )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to remove this link" ) == IDYES )
		{
			//delete link from list
			int nPluIdx = m_IndexArray.GetAt ( nSelection );
			m_IndexArray.RemoveAt ( nSelection );
			
			//delete link from plu record
			CString strPluNumber = DataManager.Plu.GetPluNoString( nPluIdx );
			__int64 nPluNumber = DataManager.Plu.GetPluNoInt( nPluIdx );

			m_pStockChangesBuffer -> LogPluChange ( nPluNumber ); 
			
			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo( strPluNumber );
			DataManager.Plu.SetRecipe( nPluIdx, RecipeRecord );
			
			DataManager.StockPlu.RemoveLink ( strStockCode, nPluNumber );
			
			if ( nSelection == m_IndexArray.GetSize() )
				nSelection--;

			AddList( 0 );
			m_listRedirections.SetCurSel ( nSelection );
		}
	}
}

/**********************************************************************/

void CPropPageStockPluLink::GetRecordData()
{
	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
	{
		DataManager.Stock.GetAt ( m_nStockIdx, m_StockRecord );

		CString strStockCode = m_StockRecord.GetStockCode();
		m_IndexArray.RemoveAll();
		m_IndexArrayRecipe.RemoveAll();
	
		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
		{
			for ( int nIndex = 0; nIndex < DataManager.StockPlu.GetPluNoCount( nStockPluIdx ); nIndex++ )
			{
				__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nIndex );
			
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
					m_IndexArray.Add ( nPluIdx );	
			}

			for ( int nIndex = 0; nIndex < DataManager.StockPlu.GetRecipePluNoCount( nStockPluIdx ); nIndex++ )
			{
				__int64 nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, nIndex );
			
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
					m_IndexArrayRecipe.Add ( nPluIdx );	
			}
		}
	}
}

/**********************************************************************/

void CPropPageStockPluLink::SetRecordControls()
{
	if ( m_bReadOnly == TRUE )
	{
		m_buttonAdd.ShowWindow( SW_HIDE );
		m_buttonRemove.ShowWindow( SW_HIDE );
		m_buttonEdit.SetWindowText( "View" );
		m_buttonEditRecipe.SetWindowText( "View" );
	}
}

/**********************************************************************/

void CPropPageStockPluLink::SaveRecord()
{
}

/**********************************************************************/

bool CPropPageStockPluLink::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageStockPluLink::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	AddList(0);
	AddListRecipe();
	SetRecordControls();
}

/**********************************************************************/

void CPropPageStockPluLink::AddList( __int64 nPluNo )
{
	m_listRedirections.DeleteAllItems();
	CString strStockCode = m_StockRecord.GetStockCode();

	int nSel = -1;

	for ( int nIndex = 0; nIndex < m_IndexArray.GetSize(); nIndex++ )
	{
		int nPluIdx = m_IndexArray.GetAt ( nIndex );
		
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
		csv.Add ( PluRecord.GetPluNoString() );
		csv.Add ( ReportHelpers.GetReportText( PluRecord ) );
		csv.Add ( DataManager.Department.GetReportTextByDeptNo( PluRecord.GetBaseDeptNo() ) );

		bool bGotProfit = FALSE;
		CExplodePluDatabase ExplodePlu;
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( PluRecord.GetPluNoInt(), StockDetails, AUDIT_SALES, 1.00, PluRecord.GetPrice(0) ) == PLU_EXCEPTION_NONE )
		{
			int nRecipePos = -1;

			for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
			{
				CCSVExplodedPlu csv ( StockDetails.GetAt ( nIndex ) );
				
				if ( strStockCode == csv.GetStockCode() )
				{
					nRecipePos = nIndex;
					break;
				}
			}
	
			if ( nRecipePos != -1 )
			{
				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

				CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nRecipePos ) );
				double dStockUnitCost = m_StockRecord.GetUnitCost ( 0 ) + DataManager.Stock.GetUnitWastageCost( m_StockRecord, 0 );
				double dStockQty = csvSale.GetStockQty();
				double dPluCost = dStockQty * dStockUnitCost;
				double dPluRetail = csvSale.GetStockValue(); 
				double dNonTax = ReportHelpers.CalcNonTax ( dPluRetail, TaxRateInfo.m_dTaxRate );
				double dPluTaxContent = dPluRetail - dNonTax;
				double dPluGP = dPluRetail - ( dPluCost + dPluTaxContent );
					
				if ( dStockQty != 0.0 )
				{
					csv.Add ( dPluRetail / dStockQty, SysInfo.GetDPValue() );
					csv.Add ( dPluGP / dStockQty, SysInfo.GetDPValue() );
					bGotProfit = TRUE;
				}
			}
		}

		if ( bGotProfit == FALSE )
		{
			csv.Add ( 0.0, SysInfo.GetDPValue() );
			csv.Add ( 0.0, SysInfo.GetDPValue() );
		}

		if ( PluRecord.GetPluNoInt() == nPluNo )
			nSel = m_listRedirections.GetItemCount();

		m_listRedirections.AddString ( csv.GetLine() );
	}
	
	if ( nSel != -1 )
		m_listRedirections.SetCurSel( nSel );
	else if ( m_listRedirections.GetItemCount() > 0 )
		m_listRedirections.SetCurSel(0);
}

/**********************************************************************/

void CPropPageStockPluLink::AddListRecipe()
{
	m_listRecipes.DeleteAllItems();
	CString strStockCode = m_StockRecord.GetStockCode();

	for ( int nIndex = 0; nIndex < m_IndexArrayRecipe.GetSize(); nIndex++ )
	{
		int nPluIdx = m_IndexArrayRecipe.GetAt ( nIndex );
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
		csv.Add ( PluRecord.GetPluNoString() );
		csv.Add ( ReportHelpers.GetReportText( PluRecord ) );
		csv.Add ( DataManager.Department.GetReportTextByDeptNo( PluRecord.GetBaseDeptNo() ) );

		bool bGotProfit = FALSE;
		
		CExplodePluDatabase ExplodePlu;
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( PluRecord.GetPluNoInt(), StockDetails, AUDIT_SALES, 1.00, PluRecord.GetPrice(0) ) == PLU_EXCEPTION_NONE )
		{
			int nRecipePos = -1;
	
			for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
			{
				CCSVExplodedPlu csv ( StockDetails.GetAt ( nIndex ) );
					
				if ( strStockCode == csv.GetStockCode() )
				{
					nRecipePos = nIndex;
					break;
				}
			}

			if ( nRecipePos != -1 )
			{
				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

				CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nRecipePos ) );
				double dStockUnitCost = m_StockRecord.GetUnitCost ( 0 );
				double dStockQty = csvSale.GetStockQty();
				double dPluCost = dStockQty * dStockUnitCost;
				double dPluRetail = csvSale.GetStockValue(); 
				double dNonTax = ReportHelpers.CalcNonTax ( dPluRetail, TaxRateInfo.m_dTaxRate );
				double dPluTaxContent = dPluRetail - dNonTax;
				double dPluGP = dPluRetail - ( dPluCost + dPluTaxContent );

				if ( dStockQty != 0.0 )
				{
					csv.Add ( dPluRetail / dStockQty, SysInfo.GetDPValue() );
					csv.Add ( dPluGP / dStockQty, SysInfo.GetDPValue() );
					bGotProfit = TRUE;
				}
			}
		}

		if ( bGotProfit == FALSE )
		{
			csv.Add ( 0.0, SysInfo.GetDPValue() );
			csv.Add ( 0.0, SysInfo.GetDPValue() );
		}

		m_listRecipes.AddString ( csv.GetLine() );
	}
	if ( m_listRecipes.GetItemCount() > 0 )
		m_listRecipes.SetCurSel(0);
}

/**********************************************************************/

void CPropPageStockPluLink::SetStockIdx ( int nStockIdx )
{
	if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
		m_nStockIdx = nStockIdx;
	else
		m_nStockIdx = -1;
}

/**********************************************************************/



