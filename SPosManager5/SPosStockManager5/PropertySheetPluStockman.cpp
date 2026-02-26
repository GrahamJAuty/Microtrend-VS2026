/**********************************************************************/
#include "EditStockTypes.h"
#include "..\CommonEcrStock\ItemTabPageTypes.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPluStockman.h"
/**********************************************************************/

CPropertySheetPluStockmanInfo::CPropertySheetPluStockmanInfo()
{
	m_bEnableSpin = FALSE;
	m_bNewStock = FALSE;
	m_bReadOnly = FALSE;
	m_bApparentTab = TRUE;
	m_bStockLevelTab = TRUE;
	m_nEditMode = EDIT_STOCK_SALEQTY;
	m_wSpinID = IDC_SPIN;
	m_pPluFilter = NULL;
}

/**********************************************************************/

IMPLEMENT_DYNAMIC(CPropertySheetPluStockman, CSpinPropertySheet)

CPropertySheetPluStockman::CPropertySheetPluStockman( CPropertySheetPluStockmanInfo& info, CWnd* pWndParent)
	 : CSpinPropertySheet( ( info.m_bEnableSpin != 0 ), info.m_wSpinID, "", pWndParent), m_config( info ) 
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	//get record based on this index
	int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();
	DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
	PriceSetItemManager.SetPluIdx ( nPluIdx );
	m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
	m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
	DataManager.Plu.GetRecipe ( nPluIdx, m_PluRecord.GetPluNoInt(), m_RecipeRecord );
	DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
	DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

	if ( m_RecipeRecord.GetPluStockLinkFlag() == TRUE )
		m_PluRecord.SetPurchaseCost(0);

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
	
	m_PagePluGeneral.SetPluRecord ( &m_PluRecord );
	m_PagePluGeneral.SetRecipeRecord( &m_RecipeRecord );
	m_PagePluGeneral.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PagePluGeneral.EditDepartments ( m_config.m_pPluFilter -> GetDepartmentFilter() == DEPARTMENTSET_ALL );
	//EXPERIMENTAL 
	m_PagePluGeneral.SetNewStockFlag ( m_config.m_bNewStock );
	m_PagePluGeneral.SetPluChangesBuffer ( &m_PluChangesBuffer );
	//EXPERIMENTAL 
	AddPage(&m_PagePluGeneral);
	m_PageTypeArray.Add ( ITEMTAB_PLU_GENERAL );
		
	m_bGotModifierInfo = SysInfo.IsModifiableSSPluNo( m_PluRecord.GetPluNoInt() );
	if ( TRUE == m_bGotModifierInfo )
	{
		m_PagePluPrice.SetPluRecord ( &m_PluRecord );
		m_PagePluPrice.SetReadOnlyFlag( m_config.m_bReadOnly );
		AddPage(&m_PagePluPrice);
		m_PageTypeArray.Add( ITEMTAB_PLU_PRICE );
	}

	m_PagePluProgram.SetPluRecord ( &m_PluRecord );
	m_PagePluProgram.SetReadOnlyFlag( m_config.m_bReadOnly );
	AddPage(&m_PagePluProgram);
	m_PageTypeArray.Add( ITEMTAB_PLU_PROGRAM );

	if ( EcrmanOptions.GetPluTabBarcodeFlag() == TRUE )
	{
		if ( Sysset.IsBarmanSystem() )
		{
			m_PagePluUnitPrice.SetPluRecord ( &m_PluRecord );
			m_PagePluUnitPrice.SetReadOnlyFlag( m_config.m_bReadOnly );
			AddPage(&m_PagePluUnitPrice);
			m_PageTypeArray.Add( ITEMTAB_PLU_UNITPRICE );
			m_PagePluUnitPrice.SetPluChangesBuffer( &m_PluChangesBuffer );
		}
		else
		{
			m_PagePluBarcode.SetPluRecord ( &m_PluRecord );
			m_PagePluBarcode.SetReadOnlyFlag( m_config.m_bReadOnly );
			AddPage(&m_PagePluBarcode);
			m_PageTypeArray.Add( ITEMTAB_PLU_BARCODE );
			m_PagePluBarcode.SetPluChangesBuffer( &m_PluChangesBuffer );
		}
	}

	if ( EcrmanOptions.GetPluTabLinkFlag() == TRUE ) 
	{
		m_PagePluLink.SetPluRecord ( &m_PluRecord );
		m_PagePluLink.SetReadOnlyFlag( m_config.m_bReadOnly );
		AddPage(&m_PagePluLink);
		m_PageTypeArray.Add( ITEMTAB_PLU_LINK);
	}

	if ( ( EcrmanOptions.GetPluTabInfoFlag() == TRUE ) && ( m_config.m_nEditMode != EDIT_STOCK_SALEQTY ) )
	{
		m_PagePluInfo.SetPluRecord ( &m_PluRecord );
		m_PagePluInfo.SetReadOnlyFlag( m_config.m_bReadOnly );
		AddPage(&m_PagePluInfo);
		m_PageTypeArray.Add( ITEMTAB_PLU_INFO );
	}
	
	if ( m_config.m_nEditMode != EDIT_STOCK_NONE && PriceSetItemManager.IsPluMaster() == TRUE )
	{
		m_PagePluRecipe.SetPluRecord ( &m_PluRecord );
		m_PagePluRecipe.SetRecipeRecord( &m_RecipeRecord );
		m_PagePluRecipe.SetReadOnlyFlag( m_config.m_bReadOnly );
		m_PagePluRecipe.SetEditMode ( m_config.m_nEditMode );
		m_PagePluRecipe.SetPluChangesBuffer ( &m_PluChangesBuffer );
		m_PagePluRecipe.SetApparentTabFlag( m_config.m_bApparentTab );
		m_PagePluRecipe.SetStockLevelTabFlag( m_config.m_bStockLevelTab );
		AddPage(&m_PagePluRecipe);
		m_PageTypeArray.Add( ITEMTAB_PLU_RECIPE );
	}
	
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluStockman::~CPropertySheetPluStockman()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluStockman, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluStockman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluStockman::SaveRecord()
{
	if ( m_config.m_bReadOnly == TRUE )
		return;

	int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();

	bool bChangeInfo = FALSE; 
	bool bChangePlu = FALSE;

	if ( m_config.m_nEditMode != EDIT_STOCK_SALEQTY )
	{
		DataManager.ResourceWrapperPlu.SaveRecordChanges( bChangeInfo, bChangePlu );
		DataManager.ListKeyImageWrapperPlu.SaveRecordChanges();
	}

	DataManager.Plu.SavePluRecord ( nPluIdx, m_PluRecord, bChangeInfo, bChangePlu );
	DataManager.Plu.SetRecipe ( nPluIdx, m_RecipeRecord );
}

/**********************************************************************/

void CPropertySheetPluStockman::UndoChanges()
{ 
	m_PluChangesBuffer.UndoChanges(); 
	DataManager.ResourceWrapperPlu.UndoRecordChanges();
	DataManager.ListKeyImageWrapperPlu.UndoRecordChanges();

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		PriceSetItemManager.SetPluStatus( m_nPreviousPriceSetStatus );
}

/**********************************************************************/

void CPropertySheetPluStockman::SpinNext()
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
		case ITEMTAB_PLU_LINK:			bUpdate = m_PagePluLink.UpdateRecord();			break;
		case ITEMTAB_PLU_INFO:			bUpdate = m_PagePluInfo.UpdateRecord();			break;
		case ITEMTAB_PLU_RECIPE:		bUpdate = m_PagePluRecipe.UpdateRecord();		break;
		}

		if ( bUpdate == FALSE )
			return;

		SaveRecord();

		m_config.m_pPluFilter -> Next();
		int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();
		DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
		PriceSetItemManager.SetPluIdx ( nPluIdx );
		m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		DataManager.Plu.GetRecipe ( nPluIdx, m_PluRecord.GetPluNoInt(), m_RecipeRecord );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );

		if ( m_RecipeRecord.GetPluStockLinkFlag() == TRUE )
			m_PluRecord.SetPurchaseCost(0);

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
		case ITEMTAB_PLU_RECIPE:		m_PagePluRecipe.Refresh();		break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPluStockman::SpinPrevious()
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
		case ITEMTAB_PLU_LINK:			bUpdate = m_PagePluLink.UpdateRecord();			break;
		case ITEMTAB_PLU_INFO:			bUpdate = m_PagePluInfo.UpdateRecord();			break;
		case ITEMTAB_PLU_RECIPE:		bUpdate = m_PagePluRecipe.UpdateRecord();		break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_config.m_pPluFilter -> Previous();
		int nPluIdx = m_config.m_pPluFilter -> GetPluItemIndex();
		DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
		PriceSetItemManager.SetPluIdx ( nPluIdx );
		m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		DataManager.Plu.GetRecipe ( nPluIdx, m_PluRecord.GetPluNoInt(), m_RecipeRecord );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );

		if ( m_RecipeRecord.GetPluStockLinkFlag() == TRUE )
			m_PluRecord.SetPurchaseCost(0);

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
		case ITEMTAB_PLU_RECIPE:		m_PagePluRecipe.Refresh();		break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPluStockman::UpdateTitle()
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
