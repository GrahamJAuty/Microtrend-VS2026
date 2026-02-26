/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\ItemTabPageTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPluStock.h"
/**********************************************************************/

CPropertySheetPluStockInfo::CPropertySheetPluStockInfo()
{
	m_bEnableSpin = FALSE;
	m_bNewStock = FALSE;
	m_bApparentTab = TRUE;
	m_bStockLevelTab = TRUE;
	m_bReadOnly = FALSE;
	m_bSingleSpIdx = FALSE;
	m_nStockIdx = 0;
	m_nDefaultSuppNo = 0;
	m_nSpIdxToEdit = 0;
	m_wSpinID = IDC_SPIN;
	m_pPluFilter = NULL;
}
	
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPluStock, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetPluStock::CPropertySheetPluStock( CPropertySheetPluStockInfo& info, CWnd* pWndParent)
	 : CSpinPropertySheet( ( info.m_bEnableSpin != 0 ), info.m_wSpinID, "", pWndParent), m_config( info )
{
	m_nLastSupplierTab = -1;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	//get plu and stock records based on this index
	SetStockIdx ( m_config.m_nStockIdx );
	DataManager.Plu.GetAt ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord );
	m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
	m_PluChangesBuffer.LogStockChange ( DataManager.Stock.GetStockCode ( m_config.m_nStockIdx ) );
	DataManager.Plu.GetRecipe ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord.GetPluNoInt(), m_RecipeRecord );
	DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
	DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );
	
	m_PagePluGeneral.SetPluRecord ( &m_PluRecord );
	m_PagePluGeneral.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PagePluGeneral.SetRecipeRecord ( &m_RecipeRecord );
	m_PagePluGeneral.EditDepartments ( m_config.m_pPluFilter -> GetDepartmentFilter() == DEPARTMENTSET_ALL );
	m_PagePluGeneral.SetNewStockFlag ( m_config.m_bNewStock );
	m_PagePluGeneral.SetSimpleEditFlag ( TRUE );
	m_PagePluGeneral.SetPluChangesBuffer ( &m_PluChangesBuffer );
	m_PagePluGeneral.SetPropertySheetPluStock ( (void*) this );
	AddPage(&m_PagePluGeneral);
	m_PageTypeArray.Add ( ITEMTAB_PLU_GENERAL );

	m_bGotModifierInfo = SysInfo.IsModifiableSSPluNo( m_PluRecord.GetPluNoInt() );
	if ( TRUE == m_bGotModifierInfo )
	{
		m_PagePluPrice.SetPluRecord ( &m_PluRecord );
		m_PagePluPrice.SetReadOnlyFlag( m_config.m_bReadOnly );
		AddPage(&m_PagePluPrice);
		m_PageTypeArray.Add (ITEMTAB_PLU_PRICE );
	}
	
	m_PagePluProgram.SetPluRecord ( &m_PluRecord );
	m_PagePluProgram.SetReadOnlyFlag( m_config.m_bReadOnly );
	AddPage(&m_PagePluProgram);
	m_PageTypeArray.Add ( ITEMTAB_PLU_PROGRAM );

	if ( EcrmanOptions.GetPluTabBarcodeFlag() == TRUE )
	{
		if ( Sysset.IsBarmanSystem() )
		{
			m_PagePluUnitPrice.SetPluRecord ( &m_PluRecord );
			m_PagePluUnitPrice.SetReadOnlyFlag( m_config.m_bReadOnly );
			m_PagePluUnitPrice.SetPluChangesBuffer( &m_PluChangesBuffer );
			AddPage(&m_PagePluUnitPrice);
			m_PageTypeArray.Add( ITEMTAB_PLU_UNITPRICE );
		}
		else
		{
			m_PagePluBarcode.SetPluRecord ( &m_PluRecord );
			m_PagePluBarcode.SetReadOnlyFlag( m_config.m_bReadOnly );
			m_PagePluBarcode.SetPluChangesBuffer( &m_PluChangesBuffer );
			AddPage(&m_PagePluBarcode);
			m_PageTypeArray.Add( ITEMTAB_PLU_BARCODE );
		}
	}

	if ( EcrmanOptions.GetPluTabLinkFlag() == TRUE )
	{
		m_PagePluLink.SetPluRecord ( &m_PluRecord );
		m_PagePluLink.SetReadOnlyFlag( m_config.m_bReadOnly );
		m_PagePluLink.SetPluChangesBuffer( &m_PluChangesBuffer );
		AddPage(&m_PagePluLink);
		m_PageTypeArray.Add ( ITEMTAB_PLU_LINK );
	}

	if ( EcrmanOptions.GetPluTabInfoFlag() == TRUE )
	{
		m_PagePluInfo.SetPluRecord ( &m_PluRecord );
		m_PagePluInfo.SetReadOnlyFlag( m_config.m_bReadOnly );
		AddPage(&m_PagePluInfo);
		m_PageTypeArray.Add( ITEMTAB_PLU_INFO );
	}

	m_PageStockGeneral.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PageStockGeneral.SetCategoryFilter(0);
	AddPage( &m_PageStockGeneral );
	m_PageTypeArray.Add( ITEMTAB_STOCK_GENERAL );
	
	if ( StockOptions.GetStockMaxSuppliersPerItem() > 0 )
	{
		m_PageStockSupplier.SetReadOnlyFlag( m_config.m_bReadOnly );
		m_PageStockSupplier.SetPluRecord( &m_PluRecord );
		m_PageStockSupplier.SetSuppIdx ( 0, &m_nLastSupplierTab, m_config.m_nDefaultSuppNo );
		m_PageStockSupplier.AllowMakePreferred( TRUE );
		m_PageStockSupplier.AllowChangeSupplier( TRUE );
		AddPage( &m_PageStockSupplier );
		m_PageTypeArray.Add( ITEMTAB_STOCK_SUPPLIER );
	}
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartIdx, nEndIdx );

	m_PageStockApparent.SetAllowEditFlag( FALSE );
	if ( ( nEndIdx >= nStartIdx ) && ( m_config.m_bReadOnly == FALSE ) )
		if ( m_config.m_bStockLevelTab == TRUE )
			if ( ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC ) || ( PasswordArray.GetLocationAccessSpIdx() != -1 ) )
				m_PageStockApparent.SetAllowEditFlag( TRUE );
			
	if ( nEndIdx >= nStartIdx )
	{
		if ( m_config.m_bApparentTab == TRUE )
		{
			if ( ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC ) || ( PasswordArray.GetLocationAccessSpIdx() != -1 ) )
			{	
				AddPage ( &m_PageStockApparent );
				m_PageStockApparent.SetDefaultSpIdx( m_config.m_nSpIdxToEdit );
				m_PageStockApparent.SetSingleSpIdxFlag( m_config.m_bSingleSpIdx );
				m_PageTypeArray.Add( ITEMTAB_STOCK_APPARENT );
			}
		}
	}
	
	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );

	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluStock::~CPropertySheetPluStock()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluStock, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluStock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluStock::SavePluRecord()
{
	if ( m_config.m_bReadOnly == TRUE )
		return;

	int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();

	bool bChangeInfo, bChangePlu;
	DataManager.ResourceWrapperPlu.SaveRecordChanges( bChangeInfo, bChangePlu );
	DataManager.ListKeyImageWrapperPlu.SaveRecordChanges();
	DataManager.Plu.SavePluRecord ( nPluIdx, m_PluRecord, bChangeInfo, bChangePlu );
	DataManager.Plu.SetRecipe ( nPluIdx, m_RecipeRecord );
}

/**********************************************************************/

void CPropertySheetPluStock::SpinNext()
{
	if ( m_config.m_pPluFilter -> HasNext() == FALSE )
		return;

	m_config.m_pPluFilter -> Next();
	int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();
	__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
	bool bModifiable = SysInfo.IsModifiableSSPluNo( nPluNo );
	m_config.m_pPluFilter -> Previous();

	if ( bModifiable == m_bGotModifierInfo )
	{
		//EXPERIMENTAL
		m_PagePluGeneral.SetNewStockFlag ( FALSE );
		//EXPERIMENTAL
		
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:		bUpdate = m_PagePluGeneral.UpdateRecord();		break;
		case ITEMTAB_PLU_PRICE:			bUpdate = m_PagePluPrice.UpdateRecord();		break;		
		case ITEMTAB_PLU_PROGRAM:		bUpdate = m_PagePluProgram.UpdateRecord();		break;		
		case ITEMTAB_PLU_UNITPRICE:		bUpdate = m_PagePluUnitPrice.UpdateRecord();	break;
		case ITEMTAB_PLU_BARCODE:		bUpdate = m_PagePluBarcode.UpdateRecord();		break;
		case ITEMTAB_PLU_LINK:			bUpdate = TRUE;									break;
		case ITEMTAB_PLU_INFO:			bUpdate = m_PagePluInfo.UpdateRecord();			break;
		case ITEMTAB_STOCK_GENERAL:		bUpdate = m_PageStockGeneral.UpdateRecord();	break;
		case ITEMTAB_STOCK_SUPPLIER:	bUpdate = m_PageStockSupplier.UpdateRecord();	break;
		case ITEMTAB_STOCK_APPARENT:	bUpdate = TRUE;									break;
		}

		if ( bUpdate == FALSE )
			return;

		m_config.m_pPluFilter -> Next();
		CPluCSVRecord TempPluRecord;
		DataManager.Plu.GetAt ( m_config.m_pPluFilter -> GetPluItemIndex(), TempPluRecord );
		
		bUpdate = FALSE;

		if ( DataManager.Plu.IsRecipe ( TempPluRecord.GetPluNoInt() ) == FALSE )
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( TempPluRecord.GetPluNoInt(), strStockCode, FALSE );
			
			if ( strStockCode != "" )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
				{
					m_config.m_nStockIdx = nStockIdx;
					bUpdate = TRUE;
				}
			}
		}

		m_config.m_pPluFilter -> Previous();
			
		if ( bUpdate == FALSE )
			return;
		
		SavePluRecord();
		m_config.m_pPluFilter -> Next();		
		DataManager.Plu.GetAt ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord );
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		m_PluChangesBuffer.LogStockChange ( DataManager.Stock.GetStockCode ( m_config.m_nStockIdx ) );
		DataManager.Plu.GetRecipe ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord.GetPluNoInt(), m_RecipeRecord );
		SetStockIdx ( m_config.m_nStockIdx );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
	
		UpdateTitle();

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:		m_PagePluGeneral.Refresh();		break;
		case ITEMTAB_PLU_PRICE:			m_PagePluPrice.Refresh();		break;		
		case ITEMTAB_PLU_PROGRAM:		m_PagePluProgram.Refresh();		break;		
		case ITEMTAB_PLU_UNITPRICE:		m_PagePluUnitPrice.Refresh();	break;
		case ITEMTAB_PLU_BARCODE:		m_PagePluBarcode.Refresh();		break;
		case ITEMTAB_PLU_LINK:			m_PagePluLink.Refresh();		break;
		case ITEMTAB_PLU_INFO:			m_PagePluInfo.Refresh();		break;
		case ITEMTAB_STOCK_GENERAL:		m_PageStockGeneral.Refresh();	break;
		case ITEMTAB_STOCK_SUPPLIER:	m_PageStockSupplier.Refresh();	break;
		case ITEMTAB_STOCK_APPARENT:	m_PageStockApparent.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPluStock::SpinPrevious()
{
	if ( m_config.m_pPluFilter -> HasPrevious() == FALSE )
		return;

	m_config.m_pPluFilter -> Previous();
	int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();
	__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
	bool bModifiable = SysInfo.IsModifiableSSPluNo( nPluNo );
	m_config.m_pPluFilter -> Next();

	if ( bModifiable == m_bGotModifierInfo )
	{
		//EXPERIMENTAL
		m_PagePluGeneral.SetNewStockFlag ( FALSE );
		//EXPERIMENTAL
		
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:		bUpdate = m_PagePluGeneral.UpdateRecord();		break;
		case ITEMTAB_PLU_PRICE:			bUpdate = m_PagePluPrice.UpdateRecord();		break;		
		case ITEMTAB_PLU_PROGRAM:		bUpdate = m_PagePluProgram.UpdateRecord();		break;		
		case ITEMTAB_PLU_UNITPRICE:		bUpdate = m_PagePluUnitPrice.UpdateRecord();	break;
		case ITEMTAB_PLU_BARCODE:		bUpdate = m_PagePluBarcode.UpdateRecord();		break;
		case ITEMTAB_PLU_LINK:			bUpdate = TRUE;									break;
		case ITEMTAB_PLU_INFO:			bUpdate = m_PagePluInfo.UpdateRecord();			break;
		case ITEMTAB_STOCK_GENERAL:		bUpdate = m_PageStockGeneral.UpdateRecord();	break;
		case ITEMTAB_STOCK_SUPPLIER:	bUpdate = m_PageStockSupplier.UpdateRecord();	break;
		case ITEMTAB_STOCK_APPARENT:	bUpdate = TRUE;									break;
		}

		if ( bUpdate == FALSE )
			return;

		m_config.m_pPluFilter -> Previous();
		CPluCSVRecord TempPluRecord;
		DataManager.Plu.GetAt ( m_config.m_pPluFilter -> GetPluItemIndex(), TempPluRecord );
		
		bUpdate = FALSE;

		if ( DataManager.Plu.IsRecipe ( TempPluRecord.GetPluNoInt() ) == FALSE )
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( TempPluRecord.GetPluNoInt(), strStockCode, FALSE );

			if ( strStockCode != "" )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
				{
					m_config.m_nStockIdx = nStockIdx;
					bUpdate = TRUE;
				}
			}
		}

		m_config.m_pPluFilter -> Next();
			
		if ( bUpdate == FALSE )
			return;
		
		SavePluRecord();
		m_config.m_pPluFilter -> Previous();		
		DataManager.Plu.GetAt ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord );
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		m_PluChangesBuffer.LogStockChange ( DataManager.Stock.GetStockCode ( m_config.m_nStockIdx ) );
		DataManager.Plu.GetRecipe ( m_config.m_pPluFilter -> GetPluItemIndex(), m_PluRecord.GetPluNoInt(), m_RecipeRecord );
		SetStockIdx ( m_config.m_nStockIdx );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
	
		UpdateTitle();
			
		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:		m_PagePluGeneral.Refresh();		break;
		case ITEMTAB_PLU_PRICE:			m_PagePluPrice.Refresh();		break;		
		case ITEMTAB_PLU_PROGRAM:		m_PagePluProgram.Refresh();		break;		
		case ITEMTAB_PLU_UNITPRICE:		m_PagePluUnitPrice.Refresh();	break;
		case ITEMTAB_PLU_BARCODE:		m_PagePluBarcode.Refresh();		break;
		case ITEMTAB_PLU_LINK:			m_PagePluLink.Refresh();		break;
		case ITEMTAB_PLU_INFO:			m_PagePluInfo.Refresh();		break;
		case ITEMTAB_STOCK_GENERAL:		m_PageStockGeneral.Refresh();	break;
		case ITEMTAB_STOCK_SUPPLIER:	m_PageStockSupplier.Refresh();	break;
		case ITEMTAB_STOCK_APPARENT:	m_PageStockApparent.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPluStock::UpdateTitle()
{
	CString strAction = ( m_config.m_bReadOnly ) ? "View" : "Edit";

	CString strText; 
	strText.Format ( "%s Record %d, Plu %s, %s",
		(const char*) strAction,
		m_config.m_pPluFilter -> GetPluItemIndex() + 1,
		m_PluRecord.GetPluNoString(),
		m_PluRecord.GetReportText() );
		
	SetTitle ( strText, 0 );
}

/**********************************************************************/

void CPropertySheetPluStock::SetStockIdx( int nStockIdx )
{
	m_config.m_nStockIdx = nStockIdx;
	m_PageStockGeneral.SetStockIdx ( m_config.m_nStockIdx );
	m_PageStockSupplier.SetStockIdx ( m_config.m_nStockIdx );
	m_PageStockApparent.SetStockIdx ( m_config.m_nStockIdx );
}

/**********************************************************************/
