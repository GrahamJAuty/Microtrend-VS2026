/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "Password.h"
#include "RepCSV.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ProcessedOrderGenerateDlgHelpers.h"
/**********************************************************************/

CProcessedOrderGenerateDlgHelpers::CProcessedOrderGenerateDlgHelpers(void)
{
	m_pListDatabase = NULL;
	m_pComboSupplier = NULL;
	m_pListSupplier = NULL;
	m_pComboCategory = NULL;
	m_pSuppSelectArray = NULL;
	m_pCategorySelector = NULL;
	m_pLocationSelector = NULL;
	m_pComboConLevel = NULL;
	m_nDbIdx = 0;
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::HandleInitDialog( CWnd* pWnd, const char* szTitle )
{
	m_pListSupplier -> SubclassDlgItem ( IDC_LIST_SUPPLIER, pWnd );
	m_pListSupplier -> SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_pListSupplier -> InsertColumn ( 0, "Suppliers", LVCFMT_LEFT, 220 );

	pWnd -> SetWindowText( szTitle );
	
	CByteArray Dummy;
	AddSupplierList ( Dummy );
	m_pListSupplier -> SetCurSel(-1);
	FillSupplierCombo();

	m_pListDatabase -> SubclassDlgItem( IDC_LIST_LOCATION, pWnd );
	m_pListDatabase -> SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_pListDatabase -> InsertColumn ( 0, "Database", LVCFMT_LEFT, 220 );

	for ( int n = 0; n < m_pArrayEntityIdx -> GetSize(); n++ )
	{
		int nDbIdx = m_pArrayEntityIdx -> GetAt(n);

		int nListPos = m_pArrayLocation -> GetSize();
		m_pListDatabase -> InsertItem ( nListPos, "" );
		m_pListDatabase -> SetItem ( nListPos, 0, LVIF_TEXT, dbDatabase.GetName( nDbIdx ), 0, 0, 0, NULL );
	
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetDatabaseNode( nDbIdx );
		m_pArrayLocation -> Add( LocSelEntity );
	}

	m_pListDatabase -> SetCurSel(0);
	m_pListDatabase -> SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_pListDatabase -> SetSelectionMark(0);	

	m_nDbIdx = m_pArrayEntityIdx -> GetAt(0);
	OpenDatabaseReadOnly();
	UpdateDatabaseControls();
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::HandleSelectDatabase()
{
	int nSel = m_pListDatabase -> GetCurSel();
	if ( ( nSel >=0 ) && ( nSel < m_pArrayEntityIdx -> GetSize() ) )
	{
		m_nDbIdx = m_pArrayEntityIdx -> GetAt( nSel );
		
		if ( m_nDbIdx != DataManager.GetActiveDbIdx() )
		{
			OpenDatabaseReadOnly();
			UpdateDatabaseControls();
		}
	}
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::OpenDatabaseReadOnly()
{
	if ( m_nDbIdx != DataManager.GetActiveDbIdx() )
	{
		DataManager.LockSuppliers();
		DataManager.LockSupplierSets();
		DataManager.CloseDatabase();

		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseReadOnly( m_nDbIdx, infoDM, TRUE, FALSE );

		DataManager.UnlockSuppliers();
		DataManager.UnlockSupplierSets();
	}
}

/**********************************************************************/

bool CProcessedOrderGenerateDlgHelpers::OpenDatabaseReadWrite( CDataManagerInfo& infoDM )
{
	bool bResult = FALSE;

	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	DataManager.CloseDatabase();

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );
	arrayReadWrite.Add( NODE_PLU );

	if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_ORDER )
		arrayReadWrite.Add( NODE_ORDER );

	if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, infoDM ) == TRUE )
	{
		DataManager.Order.SortOrders();
		bResult = TRUE;
	}

	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();

	return bResult;
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::OpenStockLevels()
{
	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartSpIdx, nEndSpIdx );

	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
	{
		CDataManagerInfo info;
		DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ); 
	}
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::UpdateDatabaseControls()
{
	FillCategoryCombo();	
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_pLocationSelector -> SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_pLocationSelector -> SetSingleDbIdx( m_nDbIdx );
	m_pLocationSelector -> SetDatabaseMode ( NODE_STOCKPOINT );	
	m_pLocationSelector -> BuildList( arrayTypes );
	m_pLocationSelector -> FillLocationCombo ( m_pComboConLevel );
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::FillCategoryCombo()
{
	int nOldSelect = 0;
	if ( m_pCategorySelector -> GotCombo() == TRUE )
		nOldSelect = m_pCategorySelector -> GetSelection();

	m_pCategorySelector -> SelectCategorySet(0);
	m_pCategorySelector -> IncludeAll();
	m_pCategorySelector -> IncludeCategorySets();
	m_pCategorySelector -> FillCategoryCombo ( m_pComboCategory );
	m_pCategorySelector -> ForceSelection( nOldSelect );
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::FillSupplierCombo()
{
	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
		m_pComboSupplier -> AddString ( DataManager.SupplierSet.GetName ( nIndex ) );

	if ( m_pComboSupplier -> GetCount() < 1 )
	{
		m_pComboSupplier -> InsertString ( 0, "No sets available" );
		m_pComboSupplier -> EnableWindow ( FALSE );
		m_pComboSupplier -> ShowWindow ( SW_HIDE );
	}
	else
	{
		m_pComboSupplier -> InsertString ( 0, "Custom Selection" );
		m_pComboSupplier -> EnableWindow ( TRUE );
	}

	m_pComboSupplier -> SetCurSel(0);
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::BuildSupplierArray()
{
	m_pSuppSelectArray -> RemoveAll();

	for ( int nIndex = 0; nIndex < m_pListSupplier -> GetItemCount(); nIndex++ )
		if ( m_pListSupplier -> GetItemState( nIndex, LVIS_SELECTED ) != 0 )
			m_pSuppSelectArray -> Add ( ( WORD ) m_pListSupplier -> GetItemData ( nIndex ) );
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::SelectSupplierSet() 
{
	int nSelection = m_pComboSupplier -> GetCurSel();

	if ( nSelection < 1 || nSelection > DataManager.SupplierSet.GetSize() )
		return;

	CSupplierSetCSVRecord SupplierSet;
	CByteArray SupplierFlags;
	DataManager.SupplierSet.GetAt ( nSelection - 1, SupplierSet );
	SupplierSet.GetSupplierFlags ( SupplierFlags );
	AddSupplierList ( SupplierFlags );
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::AddSupplierList( CByteArray& SupplierFlags )
{
	int nCurSel = m_pListSupplier -> GetCurSel();
	m_pListSupplier -> DeleteAllItems();

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nPos = m_pListSupplier -> AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_pListSupplier -> SetItemData ( nPos, nIndex + 1 );

		if ( nIndex < SupplierFlags.GetSize() - 1 )
		{
			if ( SupplierFlags.GetAt ( nIndex + 1 ) == 1 )
			{
				m_pListSupplier -> SetItemState(nPos, LVIS_SELECTED, LVIS_SELECTED);
				m_pListSupplier -> SetSelectionMark(nPos);	
			}
		}
	}

	m_pListSupplier -> InsertString ( 0, "Unspecified" );
	m_pListSupplier -> SetItemData ( 0, 0 );

	if ( SupplierFlags.GetSize() > 0 )
	{
		if ( SupplierFlags.GetAt(0) == 1 )
		{
			m_pListSupplier -> SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			m_pListSupplier -> SetSelectionMark(0);	
		}
	}

	m_pListSupplier -> SetCurSel ( nCurSel );
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::SelectSuppliersAll() 
{
	for ( int nIndex = 0; nIndex < m_pListSupplier -> GetItemCount(); nIndex++ )
	{
		m_pListSupplier -> SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
		m_pListSupplier -> SetSelectionMark(nIndex);	
	}
}


/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::SelectSuppliersNone() 
{
	for ( int nIndex = 0; nIndex < m_pListSupplier -> GetItemCount(); nIndex++ )
		m_pListSupplier -> SetItemState(nIndex, 0, LVIS_SELECTED);
}

/**********************************************************************/

void CProcessedOrderGenerateDlgHelpers::GenerateOrder( int nMethod, CProcessedOrderPreviewCSVArray& OrderArray )
{
	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet ( m_pCategorySelector -> GetSelection() );
	
	int nSize = DataManager.Stock.GetSize();
	int nCount = DataManager.Stock.GetSize();
		
	//LOOP THROUGH ALL STOCK ITEMS IN DATABASE
	for ( int nStockIdx = 0; nStockIdx < nCount; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nCount );
		bool bSupplierMatch = FALSE;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		//NOTHING TO DO IF THIS RECORD IS DISABLED FROM AUTOMATIC ORDERING
		if ( StockRecord.GetDisableAutoOrderFlag() == TRUE )
			continue;

		//NOTHING TO DO IF WE FAIL TO MATCH CATEGORY
		if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
			continue;

		//SPECIAL CASE UNSPECIFIED SUPPLIER
		if ( StockRecord.GetSupplierCount() == 0 )
			bSupplierMatch = ( m_pSuppSelectArray -> GetAt ( 0 ) == 0 );
		else
		{
			int nSuppIdx;
			int nSuppNo = StockRecord.GetSuppNo(0);	
			if ( DataManager.Supplier.FindSupplierByNumber ( nSuppNo, nSuppIdx ) == TRUE )
			{
				for ( int nCheck = 0; nCheck < m_pSuppSelectArray -> GetSize(); nCheck++ )
				{
					if ( nSuppIdx == m_pSuppSelectArray -> GetAt ( nCheck ) - 1 )
					{
						bSupplierMatch = TRUE;
						break;
					}
				}
			}
		}
				
		//NOTHING TO DO IF WE FAIL TO MATCH SUPPLIER
		if ( bSupplierMatch == FALSE )
			continue;

		int nStockUnitsReq = 0;

		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector -> GetSelectedEntity( LocSelEntity );

		//WORK OUT IF WE NEED TO MAKE AN ORDER
		CStringArray arrayReorderInfo;
		DataManager.Stock.GetReorderInfo( DataManager.GetActiveDbIdx(), nStockIdx, LocSelEntity, arrayReorderInfo, FALSE, ( 2 != nMethod ), ( 0 == nMethod ) );

		//NO NEED TO MAKE AN ORDER IF WE ONLY GET CONSOLIDATED LINE BACK
		if ( arrayReorderInfo.GetSize() < 2 )
			continue;

		//CREATE ORDER PREVIEW RECORD
		CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
		OrderPreviewRecord.SetStockIdx ( nStockIdx );

		for ( int nReorderIdx = 0; nReorderIdx < arrayReorderInfo.GetSize() - 1; nReorderIdx++ )
		{
			CCSVInternalReorderInfo csvIn ( arrayReorderInfo.GetAt ( nReorderIdx ) );
			OrderPreviewRecord.AddSURequired ( csvIn.GetSpIdx(), csvIn.GetSUShortQty() );	
		}

		if ( ( 0 != nMethod ) || ( OrderPreviewRecord.HasOrder( LocSelEntity ) == TRUE ) )
		{
			OrderPreviewRecord.UpdateSupplierCosts( LocSelEntity );
			OrderArray.Add ( OrderPreviewRecord );
		}
	}
}

/**********************************************************************/
