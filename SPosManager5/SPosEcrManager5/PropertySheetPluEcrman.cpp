/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\ItemTabPageTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PluInfoResourceWrapper.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "RepCSV.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPluEcrman.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CPropertySheetPluEcrman, CSpinPropertySheet)

CPropertySheetPluEcrman::CPropertySheetPluEcrman( WORD wSpinID, CPluFilterArray* pPluFilterArray, bool bReadOnly, CWnd* pWndParent)
	 : CSpinPropertySheet( TRUE, wSpinID, "", pWndParent )
{
	m_bReadOnly = bReadOnly;
	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	//get record based on this index
	int nPluIdx = pPluFilterArray -> GetPluItemIndex();
	PriceSetItemManager.SetPluIdx ( nPluIdx );
	m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
	DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
	m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
	DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
	DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() ); 

	//GENERAL TAB
	m_PagePluGeneral.SetPluRecord ( &m_PluRecord );
	m_PagePluGeneral.SetReadOnlyFlag( m_bReadOnly );
	m_PagePluGeneral.EditDepartments ( pPluFilterArray -> GetDepartmentFilter() == DEPARTMENTSET_ALL );
	AddPage(&m_PagePluGeneral);
	m_PageTypeArray.Add( ITEMTAB_PLU_GENERAL );

	//PRICES TAB
	m_bGotModifierInfo = SysInfo.IsModifiableSSPluNo( m_PluRecord.GetPluNoInt() );
	if ( TRUE == m_bGotModifierInfo )
	{
		m_PagePluPrice.SetPluRecord ( &m_PluRecord );
		m_PagePluPrice.SetReadOnlyFlag( m_bReadOnly );
		AddPage( &m_PagePluPrice);
		m_PageTypeArray.Add( ITEMTAB_PLU_PRICE );
	}
	
	//PROGRAM TAB
	m_PagePluProgram.SetPluRecord ( &m_PluRecord );
	m_PagePluProgram.SetReadOnlyFlag( m_bReadOnly );
	AddPage(&m_PagePluProgram);
	m_PageTypeArray.Add( ITEMTAB_PLU_PROGRAM );

	//UNIT PRICE TAB
	m_PagePluUnitPrice.SetPluRecord ( &m_PluRecord );
	m_PagePluUnitPrice.SetReadOnlyFlag( m_bReadOnly );
	m_PagePluBarcode.SetPluRecord ( &m_PluRecord );
	m_PagePluBarcode.SetReadOnlyFlag( m_bReadOnly );

	if ( EcrmanOptions.GetPluTabBarcodeFlag() == TRUE )
	{
		if ( Sysset.IsBarmanSystem() )
		{
			AddPage(&m_PagePluUnitPrice);
			m_PageTypeArray.Add( ITEMTAB_PLU_UNITPRICE );
			m_PagePluUnitPrice.SetPluChangesBuffer( &m_PluChangesBuffer );
		}
		else
		{
			AddPage(&m_PagePluBarcode);
			m_PageTypeArray.Add( ITEMTAB_PLU_BARCODE );
			m_PagePluBarcode.SetPluChangesBuffer( &m_PluChangesBuffer );
		}
	}

	if ( EcrmanOptions.GetPluTabLinkFlag() == TRUE )
	{
		//PLU LINK TAB
		m_PagePluLink.SetPluRecord ( &m_PluRecord );
		m_PagePluLink.SetReadOnlyFlag( m_bReadOnly );
		AddPage(&m_PagePluLink);
		m_PageTypeArray.Add( ITEMTAB_PLU_LINK );
	}

	if ( EcrmanOptions.GetPluTabInfoFlag() == TRUE )
	{
		//PLU INFO TAB
		m_PagePluInfo.SetPluRecord ( &m_PluRecord );
		m_PagePluInfo.SetReadOnlyFlag( m_bReadOnly );
		AddPage(&m_PagePluInfo);
		m_PageTypeArray.Add( ITEMTAB_PLU_INFO );
	}

	//STOCK TAB
	if ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE && 
		PasswordArray.GetEnable( PasswordTicks::ReportsPluApparent ) == TRUE &&
		PriceSetItemManager.IsPluMaster() == TRUE )
	{
		m_PagePluStock.SetPluRecord ( &m_PluRecord );
		m_PagePluStock.SetReadOnlyFlag( m_bReadOnly );
		AddPage(&m_PagePluStock);
		m_PageTypeArray.Add( ITEMTAB_PLU_STOCK );
		m_PagePluStock.SetPluAuditMemory ( &m_arrayPluAuditMemory );

		m_PagePluRecipe.SetPluRecord ( &m_PluRecord );
		m_PagePluRecipe.SetReadOnlyFlag( m_bReadOnly );
		AddPage(&m_PagePluRecipe);
		m_PageTypeArray.Add( ITEMTAB_PLU_RECIPE );
	}

	//points to a plu filter array which is a partial index of the
	//plu database. 
	m_pPluFilterArray = pPluFilterArray;

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
		
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluEcrman::~CPropertySheetPluEcrman()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluEcrman, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluEcrman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluEcrman::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;

	int nPluIdx = m_pPluFilterArray -> GetPluItemIndex();
	
	bool bChangeInfo, bChangePlu;
	DataManager.ResourceWrapperPlu.SaveRecordChanges( bChangeInfo, bChangePlu );
	DataManager.ListKeyImageWrapperPlu.SaveRecordChanges();
	DataManager.Plu.SavePluRecord ( nPluIdx, m_PluRecord, bChangeInfo, bChangePlu );

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		if ( PriceSetItemManager.GetPluStatus() != LOCALPLU_LOCAL )
			PriceSetItemManager.RestoreMasterRecord();
	
	if ( m_arrayPluAuditMemory.GetSize() == 0 || EcrmanOptions.GetFeaturesPluAuditFlag() == FALSE )
		return;

	CSSFile fileAudit;
	int nDbIdx = DataManager.GetActiveDbIdx();
	if ( fileAudit.Open ( dbDatabase.GetFilePathDataPluStockAuditTemp ( nDbIdx ), "ab" ) == TRUE )
	{
		for ( int nIndex = 0; nIndex < m_arrayPluAuditMemory.GetSize(); nIndex++ )
			fileAudit.WriteLine ( m_arrayPluAuditMemory.GetAt(nIndex) );

		m_arrayPluAuditMemory.RemoveAll();
	}
}

/**********************************************************************/

void CPropertySheetPluEcrman::UndoChanges()
{
	m_PluChangesBuffer.UndoChanges(); 
	DataManager.ResourceWrapperPlu.UndoRecordChanges();
	DataManager.ListKeyImageWrapperPlu.UndoRecordChanges();

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		PriceSetItemManager.SetPluStatus( m_nPreviousPriceSetStatus );
}

/**********************************************************************/

void CPropertySheetPluEcrman::SpinNext()
{
	if ( m_pPluFilterArray -> HasNext() == FALSE )
		return;

	if ( m_PagePluRecipe.IsInContextEdit() == TRUE )
		return;

	m_pPluFilterArray -> Next();
	int nPluIdx = m_pPluFilterArray -> GetPluItemIndex();
	__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
	bool bModifiable = SysInfo.IsModifiableSSPluNo( nPluNo );
	m_pPluFilterArray -> Previous();
		
	if ( bModifiable == m_bGotModifierInfo )
	{
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:	bUpdate = m_PagePluGeneral.UpdateRecord();		break;
		case ITEMTAB_PLU_PRICE:		bUpdate = m_PagePluPrice.UpdateRecord();		break;		
		case ITEMTAB_PLU_PROGRAM:	bUpdate = m_PagePluProgram.UpdateRecord();		break;		
		case ITEMTAB_PLU_UNITPRICE:	bUpdate = m_PagePluUnitPrice.UpdateRecord();	break;
		case ITEMTAB_PLU_BARCODE:	bUpdate = m_PagePluBarcode.UpdateRecord();		break;
		case ITEMTAB_PLU_STOCK:		bUpdate = m_PagePluStock.UpdateRecord();		break;
		case ITEMTAB_PLU_RECIPE:	bUpdate = m_PagePluRecipe.UpdateRecord();		break;
		case ITEMTAB_PLU_LINK:		bUpdate = m_PagePluLink.UpdateRecord();			break;
		case ITEMTAB_PLU_INFO:		bUpdate = m_PagePluInfo.UpdateRecord();			break;	
		}

		if ( bUpdate == FALSE )
			return;

		SaveRecord();		
		m_pPluFilterArray -> Next();
		int nPluIdx = m_pPluFilterArray -> GetPluItemIndex();
		PriceSetItemManager.SetPluIdx ( nPluIdx );
		m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
		DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
	
		UpdateTitle();

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:	m_PagePluGeneral.Refresh();		break;
		case ITEMTAB_PLU_PRICE:		m_PagePluPrice.Refresh();		break;		
		case ITEMTAB_PLU_PROGRAM:	m_PagePluProgram.Refresh();		break;		
		case ITEMTAB_PLU_UNITPRICE:	m_PagePluUnitPrice.Refresh();	break;
		case ITEMTAB_PLU_BARCODE:	m_PagePluBarcode.Refresh();		break;
		case ITEMTAB_PLU_STOCK:		m_PagePluStock.Refresh();		break;
		case ITEMTAB_PLU_RECIPE:	m_PagePluRecipe.Refresh();		break;
		case ITEMTAB_PLU_LINK:		m_PagePluLink.Refresh();		break;
		case ITEMTAB_PLU_INFO:		m_PagePluInfo.Refresh();		break;	
		}
	}
}

/**********************************************************************/

void CPropertySheetPluEcrman::SpinPrevious()
{
	if ( m_pPluFilterArray -> HasPrevious() == FALSE )
		return;

	if ( m_PagePluRecipe.IsInContextEdit() == TRUE )
		return;

	m_pPluFilterArray -> Previous();
	int nPluIdx = m_pPluFilterArray -> GetPluItemIndex();
	__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
	bool bModifiable = SysInfo.IsModifiableSSPluNo( nPluNo );
	m_pPluFilterArray -> Next();
	
	if ( bModifiable == m_bGotModifierInfo )
	{
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:	bUpdate = m_PagePluGeneral.UpdateRecord();		break;
		case ITEMTAB_PLU_PRICE:		bUpdate = m_PagePluPrice.UpdateRecord();		break;		
		case ITEMTAB_PLU_PROGRAM:	bUpdate = m_PagePluProgram.UpdateRecord();		break;		
		case ITEMTAB_PLU_UNITPRICE:	bUpdate = m_PagePluUnitPrice.UpdateRecord();	break;
		case ITEMTAB_PLU_BARCODE:	bUpdate = m_PagePluBarcode.UpdateRecord();		break;
		case ITEMTAB_PLU_STOCK:		bUpdate = m_PagePluStock.UpdateRecord();		break;
		case ITEMTAB_PLU_RECIPE:	bUpdate = m_PagePluRecipe.UpdateRecord();		break;
		case ITEMTAB_PLU_LINK:		bUpdate = m_PagePluLink.UpdateRecord();			break;
		case ITEMTAB_PLU_INFO:		bUpdate = m_PagePluInfo.UpdateRecord();			break;	
		}

		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_pPluFilterArray -> Previous();
		int nPluIdx = m_pPluFilterArray -> GetPluItemIndex();
		PriceSetItemManager.SetPluIdx ( nPluIdx );
		m_nPreviousPriceSetStatus = PriceSetItemManager.GetPluStatus();
		DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
		m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
		DataManager.ResourceWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.SetCurrentPluNo( PLUINFO_MODE_EDIT, m_PluRecord.GetPluNoInt(), m_PluRecord.GetImageFilename() );

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			m_PluRecord.SetGPType( PLU_GPTYPE_VARIABLE );
	
		UpdateTitle();
	
		switch ( nPage )
		{
		case ITEMTAB_PLU_GENERAL:	m_PagePluGeneral.Refresh();		break;
		case ITEMTAB_PLU_PRICE:		m_PagePluPrice.Refresh();		break;		
		case ITEMTAB_PLU_PROGRAM:	m_PagePluProgram.Refresh();		break;		
		case ITEMTAB_PLU_UNITPRICE:	m_PagePluUnitPrice.Refresh();	break;
		case ITEMTAB_PLU_BARCODE:	m_PagePluBarcode.Refresh();		break;
		case ITEMTAB_PLU_STOCK:		m_PagePluStock.Refresh();		break;
		case ITEMTAB_PLU_RECIPE:	m_PagePluRecipe.Refresh();		break;
		case ITEMTAB_PLU_LINK:		m_PagePluLink.Refresh();		break;
		case ITEMTAB_PLU_INFO:		m_PagePluInfo.Refresh();		break;	
		}
	}
}

/**********************************************************************/

void CPropertySheetPluEcrman::UpdateTitle()
{
	CString strAction = ( m_bReadOnly ) ? "View" : "Edit";

	CString strText; 
	strText.Format ( "%s Record %d, Plu %s, %s",
		(const char*) strAction,
		m_pPluFilterArray -> GetPluItemIndex() + 1,
		m_PluRecord.GetPluNoString(),
		m_PluRecord.GetReportText() );
	
	SetTitle ( strText, 0 );
}

/**********************************************************************/

