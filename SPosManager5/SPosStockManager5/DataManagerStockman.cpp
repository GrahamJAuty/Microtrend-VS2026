 /**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "StockOptionsIni.h"
#include "NodeTypes.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/

CDataManagerStockman::CDataManagerStockman() : CDataManager(), ProductLabels( TRUE )
{
	m_nStatusRecipe = DATAMANAGER_CLOSED;
	m_nStatusAllowance = DATAMANAGER_CLOSED;
	m_nStatusCategory = DATAMANAGER_CLOSED;
	m_nStatusCategorySet = DATAMANAGER_CLOSED;
	m_nStatusDelUnit = DATAMANAGER_CLOSED;
	m_nStatusOrder = DATAMANAGER_CLOSED;
	m_nStatusProductLabels = DATAMANAGER_CLOSED;
	m_nStatusStock = DATAMANAGER_CLOSED;
	m_nStatusSupplier = DATAMANAGER_CLOSED;
	m_nStatusSupplierSet = DATAMANAGER_CLOSED;

	for ( int n = 0; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
	{
		m_nStatusCustomFieldValue[n] = DATAMANAGER_CLOSED;
		CustomFieldValue[n].SetFieldNo( n );

		m_nStatusCustomFieldSet[n] = DATAMANAGER_CLOSED;
		CustomFieldSet[n].SetFieldNo( n );

		m_nStatusCustomFieldValueBuffer[n] = DATAMANAGER_CLOSED;
		CustomFieldValueBuffer[n].SetFieldNo( n );

		m_nStatusCustomFieldSetBuffer[n] = DATAMANAGER_CLOSED;
		CustomFieldSetBuffer[n].SetFieldNo( n );
	}
	
	m_nStatusCategoryBuffer = DATAMANAGER_CLOSED;
	m_nStatusDelUnitBuffer = DATAMANAGER_CLOSED;
	m_nStatusSupplierBuffer = DATAMANAGER_CLOSED;

	m_bLockSuppliers = FALSE;
	m_bLockSupplierSets = FALSE;
	
	m_bOpenStockLevels = TRUE;
	m_bOpenTransferDetail = TRUE;

	m_nActiveCustomFieldNo = 1;
}

/**********************************************************************/

void CDataManagerStockman::SetActiveCustomFieldNo( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_ITEM_CUSTOM_FIELDS ) )
		m_nActiveCustomFieldNo = n;
}

/**********************************************************************/

CCustomFieldValueCSVArray* CDataManagerStockman::GetActiveCustomField()
{
	if ( ( m_nActiveCustomFieldNo >= 1 ) && ( m_nActiveCustomFieldNo <= MAX_ITEM_CUSTOM_FIELDS ) )
		return &CustomFieldValue[ m_nActiveCustomFieldNo ];
	else
		return &CustomFieldValue[ 1 ];
}

/**********************************************************************/

CCustomFieldSetCSVArray* CDataManagerStockman::GetActiveCustomFieldSet()
{
	if ( ( m_nActiveCustomFieldNo >= 1 ) && ( m_nActiveCustomFieldNo <= MAX_ITEM_CUSTOM_FIELDS ) )
		return &CustomFieldSet[ m_nActiveCustomFieldNo ];
	else
		return &CustomFieldSet[ 1 ];
}

/**********************************************************************/

void CDataManagerStockman::OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen )
{
	m_bOpenStockLevels = FALSE;
	m_bOpenTransferDetail = FALSE;
	CDataManager::OpenDatabaseQuickNoFiles( nDbIdx, bKeepIfAlreadyOpen );
}

/**********************************************************************/

bool CDataManagerStockman::OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	m_bOpenStockLevels = bOpenStockLevels;
	m_bOpenTransferDetail = bOpenTransferDetail;
	return CDataManager::OpenDatabaseReadOnly( nDbIdx, info );
}

/**********************************************************************/

bool CDataManagerStockman::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	m_bOpenStockLevels = bOpenStockLevels;
	m_bOpenTransferDetail = bOpenTransferDetail;
	return CDataManager::OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info );
}

/**********************************************************************/

bool CDataManagerStockman::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	m_bOpenStockLevels = bOpenStockLevels;
	m_bOpenTransferDetail = bOpenTransferDetail;
	return CDataManager::OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info );
}

/**********************************************************************/

bool CDataManagerStockman::OpenSpecialReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info )
{
	int nAccessAllowance = nDefaultType;
	int nAccessCustomFieldValue = nDefaultType;
	int nAccessCustomFieldSet = nDefaultType;
	int nAccessCategory = nDefaultType;
	int nAccessCategorySet = nDefaultType;
	int nAccessDelUnit = nDefaultType;
	int nAccessOrder = nDefaultType;
	int nAccessProductLabels = nDefaultType;
	int nAccessStock = nDefaultType;
	int nAccessSupplier = nDefaultType;
	int nAccessSupplierSet = nDefaultType;

	for ( int n = 0; n < arrayReadWrite.GetSize(); n++ )
	{
		switch( arrayReadWrite.GetAt(n) )
		{
		case NODE_ALLOWANCE:		nAccessAllowance =	DB_READWRITE;		break;
		case NODE_CUSTOM_FIELD:		nAccessCustomFieldValue = DB_READWRITE;		break;
		case NODE_CUSTOM_FIELD_SET:	nAccessCustomFieldSet = DB_READWRITE;		break;
		case NODE_CATEGORY:			nAccessCategory = DB_READWRITE;			break;
		case NODE_CATEGORY_SET:		nAccessCategorySet = DB_READWRITE;		break;
		case NODE_DELUNIT:			nAccessDelUnit = DB_READWRITE;			break;
		case NODE_ORDER:			nAccessOrder = DB_READWRITE;			break;
		case NODE_LABELS_PRODUCT:	nAccessProductLabels = DB_READWRITE;	break;
		case NODE_STOCK:			nAccessStock = DB_READWRITE;			break;
		case NODE_SUPPLIER:			nAccessSupplier = DB_READWRITE;			break;
		case NODE_SUPPLIER_SET:		nAccessSupplierSet = DB_READWRITE;		break;
		}
	}

	for ( int n = 0; n < arrayReadOnly.GetSize(); n++ )
	{
		switch( arrayReadOnly.GetAt(n) )
		{
		case NODE_ALLOWANCE:		nAccessAllowance =	DB_READONLY;		break;
		case NODE_CUSTOM_FIELD:		nAccessCustomFieldValue = DB_READONLY;	break;
		case NODE_CUSTOM_FIELD_SET:	nAccessCustomFieldSet = DB_READONLY;	break;
		case NODE_CATEGORY:			nAccessCategory = DB_READONLY;			break;
		case NODE_CATEGORY_SET:		nAccessCategorySet = DB_READONLY;		break;
		case NODE_DELUNIT:			nAccessDelUnit = DB_READONLY;			break;
		case NODE_ORDER:			nAccessOrder = DB_READONLY;				break;
		case NODE_LABELS_PRODUCT:	nAccessProductLabels = DB_READONLY;		break;
		case NODE_STOCK:			nAccessStock = DB_READONLY;				break;
		case NODE_SUPPLIER:			nAccessSupplier = DB_READONLY;			break;
		case NODE_SUPPLIER_SET:		nAccessSupplierSet = DB_READONLY;		break;
		}
	}

	if ( nAccessAllowance != 0 )
	{
		if ( OpenAllowance( nAccessAllowance, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessCategory != 0 )
	{
		if ( OpenCategory( nAccessCategory, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessCategorySet != 0 )
	{
		if ( OpenCategorySet( nAccessCategorySet, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessCustomFieldValue != 0 )
	{
		for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
		{
			if ( OpenCustomFieldValue( n, nAccessCustomFieldValue, info ) == FALSE )
			{
				CloseDatabase();
				return FALSE;
			}
		}
	}

	if ( nAccessCustomFieldSet != 0 )
	{
		for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
		{
			if ( OpenCustomFieldSet( n, nAccessCustomFieldSet, info ) == FALSE )
			{
				CloseDatabase();
				return FALSE;
			}
		}
	}

	if ( nAccessDelUnit != 0 )
	{
		if ( OpenDelUnit( nAccessDelUnit, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessOrder != 0 )
	{
		if ( OpenOrder( nAccessOrder, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessProductLabels != 0 )
	{
		if ( OpenProductLabels( nAccessProductLabels, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessStock != 0 )
	{
		if ( OpenStock( nAccessStock, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessSupplier != 0 )
	{
		if ( m_bLockSuppliers == FALSE )
		{
			if ( OpenSupplier( nAccessSupplier, info ) == FALSE )
			{
				CloseDatabase();
				return FALSE;
			}
		}
	}

	if ( nAccessSupplierSet != 0 )
	{
		if ( m_bLockSupplierSets == FALSE )
		{
			if ( OpenSupplierSet( nAccessSupplierSet, info ) == FALSE )
			{
				CloseDatabase();
				return FALSE;
			}
		}
	}

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

bool CDataManagerStockman::OpenSpecialReadOnly( bool bSameDb, CDataManagerInfo& info )
{
	if ( ( m_nStatusAllowance != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseAllowance( info );
		OpenAllowance( DB_READONLY, info );
	}

	if ( ( m_nStatusCategory != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseCategory( info );
		OpenCategory( DB_READONLY, info );
	}

	if ( ( m_nStatusCategorySet != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseCategorySet( info );
		OpenCategorySet( DB_READONLY, info );
	}

	for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
	{
		if ( ( m_nStatusCustomFieldValue[n] != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
		{
			CloseCustomFieldValue( n, info );
			OpenCustomFieldValue( n, DB_READONLY, info );
		}
	}

	for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
	{
		if ( ( m_nStatusCustomFieldSet[n] != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
		{
			CloseCustomFieldSet( n, info );
			OpenCustomFieldSet( n, DB_READONLY, info );
		}
	}

	if ( ( m_nStatusDelUnit != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseDelUnit( info );
		OpenDelUnit( DB_READONLY, info );
	}

	if ( ( m_nStatusOrder != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseOrder( info );
		OpenOrder( DB_READONLY, info );
	}

	if ( ( m_nStatusProductLabels != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseProductLabels( info );
		OpenProductLabels( DB_READONLY, info );
	}

	if ( ( m_nStatusStock != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseStock( info );
		OpenStock( DB_READONLY, info );
	}

	if ( m_bLockSuppliers == FALSE )
	{
		if ( ( m_nStatusSupplier != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
		{
			CloseSupplier( info );
			OpenSupplier( DB_READONLY, info );
		}
	}

	if ( m_bLockSupplierSets == FALSE )
	{
		if ( ( m_nStatusSupplierSet != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
		{
			CloseSupplierSet( info );
			OpenSupplierSet( DB_READONLY, info );
		}
	}

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

void CDataManagerStockman::CloseSpecial()
{
	CDataManagerInfo info;
	CloseAllowance( info );
	CloseCategory( info );
	CloseCategorySet( info );
	CloseDelUnit( info );
	CloseOrder( info );
	CloseProductLabels( info );
	CloseStock( info );

	for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
	{
		CloseCustomFieldValue( n, info );
		CloseCustomFieldSet( n, info );
	}

	if ( m_bLockSuppliers == FALSE )	CloseSupplier( info );
	if ( m_bLockSupplierSets == FALSE )	CloseSupplierSet( info );
}

/**********************************************************************/

bool CDataManagerStockman::OpenPlu( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	CString strFilename1 = GetFilePathPlu( m_nActiveDbIdx );
	CString strFilename2 = GetFilePathRecipe( m_nActiveDbIdx );

	if ( FALSE == m_bPluPriceSetMode )
		OpenCSVArray( Plu.m_PluDatabase, info, strFilename1, m_nStatusPlu, nType, TRUE );
	else
		Plu.m_PluDatabase.Read( strFilename1 );

	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	OpenCSVArray( Plu.m_RecipeDatabase, info, strFilename2, m_nStatusRecipe, nType, TRUE );
		
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	StockPlu.Invalidate();
	PriceSetItemManager.Reset();
	KbdListNameArray.Read( m_nActiveDbIdx );

	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

bool CDataManagerStockman::ReopenPlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );
	
	bool bResult = FALSE;
	switch( m_nStatusPlu )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePlu( info );
		bResult = OpenPlu( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPlu( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CDataManagerStockman::ClosePlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	CloseCSVArray( Plu.m_PluDatabase, info, m_nStatusPlu );
	PriceSetItemManager.Reset();
		
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
	{
		CDataManagerInfo dummy;
		CloseCSVArray( Plu.m_RecipeDatabase, dummy, m_nStatusRecipe );
		return FALSE;
	}
	
	CloseCSVArray( Plu.m_RecipeDatabase, info, m_nStatusRecipe );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerStockman::WritePlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	Plu.Synchronise( TRUE );

	WriteCSVArray( Plu.m_PluDatabase, info, m_nStatusPlu );
		
	if ( info.m_nErrorType == DATAMANAGER_ERROR_NONE )
	{
		WriteCSVArray( Plu.m_RecipeDatabase, info, m_nStatusRecipe );
		WriteExportLiveApparentStockToken();
	}

	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerStockman::OpenAllowance( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLOWANCE );
	CString strFilename = GetFilePathAllowance( m_nActiveDbIdx );
	return OpenStringArray( Allowance, info, strFilename, m_nStatusAllowance, nType, FALSE );
}

bool CDataManagerStockman::CloseAllowance( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLOWANCE );
	return CloseStringArray( Allowance, info, m_nStatusAllowance );
}

bool CDataManagerStockman::WriteAllowance( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLOWANCE );
	return WriteStringArray( Allowance, info, m_nStatusAllowance );
}

/**********************************************************************/

bool CDataManagerStockman::OpenCategory( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	CString strFilename = GetFilePathCategory( m_nActiveDbIdx );
	return OpenStringArray( Category, info, strFilename, m_nStatusCategory, nType, FALSE );
}

bool CDataManagerStockman::CloseCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	return CloseStringArray( Category, info, m_nStatusCategory );
}

bool CDataManagerStockman::WriteCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	return WriteStringArray( Category, info, m_nStatusCategory );
}

bool CDataManagerStockman::OpenCategoryBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );

	CString strFilename = GetFilePathCategory( m_nActiveDbIdx );
	if ( OpenStringArray( CategoryBuffer, info, strFilename, m_nStatusCategoryBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	Category.CopyFrom( CategoryBuffer );
	return TRUE;
}

bool CDataManagerStockman::WriteCategoryBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	
	CDefTextCategory temp;
	temp.CopyFrom( CategoryBuffer );
	CategoryBuffer.CopyFrom( Category);

	bool bResult = WriteStringArray( CategoryBuffer, info, m_nStatusCategoryBuffer );

	if ( bResult == FALSE )
		CategoryBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerStockman::CloseCategoryBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	Category.CopyFrom ( CategoryBuffer );
	return CloseStringArray( CategoryBuffer, info, m_nStatusCategoryBuffer );
}

/**********************************************************************/

void CDataManagerStockman::GetSortedCategoryList( CWordArray& arrayCatNo )
{
	arrayCatNo.RemoveAll();

	CReportConsolidationArray<CSortedIntByString> arrayCategory( TRUE );
	
	for ( int n = 1; n <= Category.GetSize(); n++ )
	{
		CString strName = Category.GetText(n);
		::TrimSpaces( strName, FALSE );

		bool bDefault = TRUE;

		if ( "" != strName )
		{
			if ( strName != Category.GetDefaultText(n) )
				bDefault = FALSE;
		}
		else
			strName = Category.GetDefaultText(n);

		CString strLabel;

		if ( TRUE == bDefault )
			strLabel.Format( "1%3.3d", n );
		else
			strLabel = "0000";

		strLabel += strName;

		CSortedIntByString item;
		item.m_strItem = strLabel;
		item.m_nItem = n;
		arrayCategory.Consolidate( item );
	}

	for ( int n = 0; n < arrayCategory.GetSize(); n++ )
	{
		CSortedIntByString item;
		arrayCategory.GetAt( n, item );
		arrayCatNo.Add( item.m_nItem );
	}
}

/**********************************************************************/

bool CDataManagerStockman::OpenCategorySet( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	CString strFilename = GetFilePathCategorySet( m_nActiveDbIdx );
	return OpenCSVArray( CategorySet, info, strFilename, m_nStatusCategorySet, nType, TRUE );
}

bool CDataManagerStockman::CloseCategorySet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	return CloseCSVArray( CategorySet, info, m_nStatusCategorySet );
}

bool CDataManagerStockman::WriteCategorySet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	return WriteCSVArray( CategorySet, info, m_nStatusCategorySet );
}

/**********************************************************************/

bool CDataManagerStockman::OpenCustomFieldValue( int nFieldNo, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );
		CString strFilename = GetFilePathCustomFieldValue( m_nActiveDbIdx, nFieldNo );
		bool bResult = OpenCSVArray( CustomFieldValue[ nFieldNo ], info, strFilename, m_nStatusCustomFieldValue[ nFieldNo ], nType, TRUE );
		CustomFieldValue[ nFieldNo ].SetGlobalMode( StockOptions.GetGlobalCustomFieldFlag() );
		CustomFieldValue[ nFieldNo ].PullNextNumberFromArray();
		CustomFieldValue[ nFieldNo ].ValidateFieldValues();
		return bResult;
	}
}

bool CDataManagerStockman::CloseCustomFieldValue( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );
		return CloseCSVArray( CustomFieldValue[ nFieldNo ], info, m_nStatusCustomFieldValue[ nFieldNo ] );
	}
}

bool CDataManagerStockman::WriteCustomFieldValue( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );
		CustomFieldValue[ nFieldNo ].ValidateFieldValues();
		CustomFieldValue[ nFieldNo ].PushNextNumberToArray();
		bool bResult = WriteCSVArray( CustomFieldValue[nFieldNo], info, m_nStatusCustomFieldValue[nFieldNo] );
		CustomFieldValue[ nFieldNo ].PullNextNumberFromArray();
		return bResult;
	}
}

bool CDataManagerStockman::OpenCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );

		CString strFilename = GetFilePathCustomFieldValue( m_nActiveDbIdx, nFieldNo );

		if ( OpenCSVArray( CustomFieldValueBuffer[ nFieldNo ], info, strFilename, m_nStatusCustomFieldValueBuffer[ nFieldNo ], DB_READWRITE, FALSE ) == FALSE )
			return FALSE;
	
		CustomFieldValueBuffer[ nFieldNo ].SetGlobalMode( StockOptions.GetGlobalCustomFieldFlag() );
		CustomFieldValueBuffer[ nFieldNo ].PullNextNumberFromArray();
		
		CustomFieldValue[ nFieldNo ].CopyArray( CustomFieldValueBuffer[ nFieldNo ] );
		return TRUE;
	}
}

bool CDataManagerStockman::WriteCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );
	
		CCustomFieldValueCSVArray temp;
		temp.CopyArray( CustomFieldValueBuffer[ nFieldNo ] );
		CustomFieldValueBuffer[ nFieldNo ].CopyArray( CustomFieldValue[ nFieldNo ] );

		CustomFieldValueBuffer[ nFieldNo ].PushNextNumberToArray();
		bool bResult = WriteCSVArray( CustomFieldValueBuffer[ nFieldNo ], info, m_nStatusCustomFieldValueBuffer[ nFieldNo ] );
		CustomFieldValueBuffer[ nFieldNo ].PullNextNumberFromArray();
		
		if ( bResult == FALSE )
			CustomFieldValueBuffer[ nFieldNo ].CopyArray( temp );

		return bResult;
	}
}

bool CDataManagerStockman::CloseCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD );
		CustomFieldValue[ nFieldNo ].CopyArray( CustomFieldValueBuffer[ nFieldNo ] );
		return CloseCSVArray( CustomFieldValueBuffer[ nFieldNo ], info, m_nStatusCustomFieldValueBuffer[ nFieldNo ] );
	}
}

/**********************************************************************/

bool CDataManagerStockman::OpenCustomFieldSet( int nFieldNo, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		CString strFilename = GetFilePathCustomFieldSet( m_nActiveDbIdx, nFieldNo );
		bool bResult = OpenCSVArray( CustomFieldSet[ nFieldNo ], info, strFilename, m_nStatusCustomFieldSet[ nFieldNo ], nType, TRUE );
		return bResult;
	}
}

bool CDataManagerStockman::CloseCustomFieldSet( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		return CloseCSVArray( CustomFieldSet[ nFieldNo ], info, m_nStatusCustomFieldSet[ nFieldNo ] );
	}
}

bool CDataManagerStockman::WriteCustomFieldSet( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		bool bResult = WriteCSVArray( CustomFieldSet[nFieldNo], info, m_nStatusCustomFieldSet[nFieldNo] );
		return bResult;
	}
}

bool CDataManagerStockman::OpenCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );

		CString strFilename = GetFilePathCustomFieldSet( m_nActiveDbIdx, nFieldNo );

		if ( OpenCSVArray( CustomFieldSetBuffer[ nFieldNo ], info, strFilename, m_nStatusCustomFieldSetBuffer[ nFieldNo ], DB_READWRITE, FALSE ) == FALSE )
			return FALSE;
	
		CustomFieldSet[ nFieldNo ].CopyArray( CustomFieldSetBuffer[ nFieldNo ] );
		return TRUE;
	}
}

bool CDataManagerStockman::WriteCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
	
		CCustomFieldSetCSVArray temp;
		temp.CopyArray( CustomFieldSetBuffer[ nFieldNo ] );
		CustomFieldSetBuffer[ nFieldNo ].CopyArray( CustomFieldSet[ nFieldNo ] );

		bool bResult = WriteCSVArray( CustomFieldSetBuffer[ nFieldNo ], info, m_nStatusCustomFieldSetBuffer[ nFieldNo ] );
		
		if ( bResult == FALSE )
			CustomFieldSetBuffer[ nFieldNo ].CopyArray( temp );

		return bResult;
	}
}

bool CDataManagerStockman::CloseCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info )
{
	if ( SysInfo.IsRetailStockSystem() == FALSE )
		return TRUE;
	else if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return FALSE;
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		CustomFieldSet[ nFieldNo ].CopyArray( CustomFieldSetBuffer[ nFieldNo ] );
		return CloseCSVArray( CustomFieldSetBuffer[ nFieldNo ], info, m_nStatusCustomFieldSetBuffer[ nFieldNo ] );
	}
}

/**********************************************************************/

bool CDataManagerStockman::OpenDelUnit( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );
	CFilenameUpdater FnUp ( SysFiles::DelUnit );
	return OpenStringArray( DelUnit, info, FnUp.GetFilenameToUse(), m_nStatusDelUnit, nType, TRUE );
}

bool CDataManagerStockman::CloseDelUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );
	return CloseStringArray( DelUnit, info, m_nStatusDelUnit );
}

bool CDataManagerStockman::WriteDelUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );
	return WriteStringArray( DelUnit, info, m_nStatusDelUnit );
}

bool CDataManagerStockman::OpenDelUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );

	CFilenameUpdater FnUp ( SysFiles::DelUnit );
	if ( OpenStringArray( DelUnitBuffer, info, FnUp.GetFilenameToUse(), m_nStatusDelUnitBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	DelUnit.CopyFrom( DelUnitBuffer );
	return TRUE;
}

bool CDataManagerStockman::WriteDelUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );
	
	CDefTextDelUnit temp;
	temp.CopyFrom( DelUnitBuffer );
	DelUnitBuffer.CopyFrom( DelUnit);

	bool bResult = WriteStringArray( DelUnitBuffer, info, m_nStatusDelUnitBuffer );

	if ( bResult == FALSE )
		DelUnitBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerStockman::CloseDelUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DELUNIT );
	DelUnit.CopyFrom( DelUnitBuffer );
	return CloseStringArray( DelUnitBuffer, info, m_nStatusDelUnitBuffer );
}

/**********************************************************************/

bool CDataManagerStockman::OpenOrder ( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ORDER );
	CString strFilename = GetFilePathOrder( m_nActiveDbIdx );
	return OpenCSVArray( Order, info, strFilename, m_nStatusOrder, nType, TRUE );		
}

bool CDataManagerStockman::OpenOrderSp ( int nSpIdx, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ORDER );

	CFilenameUpdater FnUp( SysFiles::SpOrders, nSpIdx );
	CString strFilename = FnUp.GetFilenameToUse();
	
	return OpenCSVArray( Order, info, strFilename, m_nStatusOrder, nType, TRUE );		
}

bool CDataManagerStockman::CloseOrder( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ORDER );
	return CloseCSVArray( Order, info, m_nStatusOrder );
}

bool CDataManagerStockman::WriteOrder( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ORDER );
	return WriteCSVArray( Order, info, m_nStatusOrder );
}

/**********************************************************************/

bool CDataManagerStockman::OpenProductLabels( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_PRODUCT );
	CString strFilename = GetFilePathProductLabels( m_nActiveDbIdx );
	OpenCSVArray( ProductLabels, info, strFilename, m_nStatusProductLabels, nType, TRUE );
	ProductLabels.MergePendingFiles();
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerStockman::CloseProductLabels( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_PRODUCT );
	ProductLabels.ClearPendingFileList();
	return CloseCSVArray( ProductLabels, info, m_nStatusProductLabels );
}

bool CDataManagerStockman::WriteProductLabels( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_PRODUCT );
	bool bResult = WriteCSVArray( ProductLabels, info, m_nStatusProductLabels );

	if ( TRUE == bResult )
		ProductLabels.DeletePendingFiles();

	return bResult;
}

/**********************************************************************/

bool CDataManagerStockman::OpenStockOnly( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );	
	CString strFilename = GetFilePathStock( m_nActiveDbIdx );
	OpenCSVArray( Stock.m_StockDatabase, info, strFilename, m_nStatusStock, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

bool CDataManagerStockman::OpenStock( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );
	
	CString strFilename = GetFilePathStock( m_nActiveDbIdx );
	
	OpenCSVArray( Stock.m_StockDatabase, info, strFilename, m_nStatusStock, nType, TRUE );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	Stock.DeleteStockLevels();
	Stock.CreateStockLevels( nStartIdx, nEndIdx );

	if ( m_bOpenStockLevels == TRUE )
	{
		bool bError = FALSE;
		StatusProgress.Lock( TRUE, "Opening stock level files" );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			StatusProgress.SetPos( nSpIdx - nStartIdx, nEndIdx + 1 - nStartIdx );

			CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pStockLevels;

			CFilenameUpdater FnUp( SysFiles::StockLevels, nSpIdx );
			CString strFilename = FnUp.GetFilenameToUse();
			
			OpenCSVArray( *pArray, info, strFilename, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels, nType, TRUE );
			if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
			{
				bError = TRUE;
				break;
			}
			
			Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels = nType;
		}

		StatusProgress.Unlock();

		if ( bError == TRUE )
			return FALSE;
	}
	else
	{
		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels = DATAMANAGER_CLOSED;
	}

	if ( m_bOpenTransferDetail == TRUE )
	{
		bool bError = FALSE;
		StatusProgress.Lock( TRUE, "Opening transfer detail files" );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			StatusProgress.SetPos( nSpIdx - nStartIdx, nEndIdx + 1 - nStartIdx );

			CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pTransferDetail;

			CFilenameUpdater FnUp( SysFiles::TransferDetail, nSpIdx );
			CString strFilename = FnUp.GetFilenameToUse();
			
			OpenCSVArray( *pArray, info, strFilename, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail, nType, TRUE );
			if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
			{
				bError = TRUE;
				break;
			}
			
			Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail = nType;
		}

		StatusProgress.Unlock();

		if ( bError == TRUE )
			return FALSE;
	}
	else
	{
		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail = DATAMANAGER_CLOSED;
	}

	return TRUE;
}

/**********************************************************************/

bool CDataManagerStockman::CloseStockOnly( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );
	CloseCSVArray( Stock.m_StockDatabase, info, m_nStatusStock );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerStockman::CloseStock( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );

	CloseCSVArray( Stock.m_StockDatabase, info, m_nStatusStock );
	int nErrorType = info.m_nErrorType;

	{
		CWordArray arrayIndex;
		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusStockLevels != DATAMANAGER_CLOSED )
				arrayIndex.Add( nIndex );
		}

		int nSpToClose = arrayIndex.GetSize();
		
		if ( nSpToClose >= 2 )
			StatusProgress.Lock( TRUE, "Closing stock level files" );
		
		for ( int n = 0; n < nSpToClose; n++ )
		{
			int nIndex = arrayIndex.GetAt(n);

			if ( nSpToClose >= 2 )
				StatusProgress.SetPos( n, nSpToClose );
			
			CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

			CloseCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusStockLevels );
			
			if ( nErrorType == DATAMANAGER_ERROR_NONE )
				nErrorType = info.m_nErrorType;

			Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusStockLevels = DATAMANAGER_CLOSED;
		}	

		if ( nSpToClose >= 2 )
			StatusProgress.Unlock();
	}

	{
		CWordArray arrayIndex;
		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusTransferDetail != DATAMANAGER_CLOSED )
				arrayIndex.Add( nIndex );
		}

		int nSpToClose = arrayIndex.GetSize();
		
		if ( nSpToClose >= 2 )
			StatusProgress.Lock( TRUE, "Closing transfer detail files" );
		
		for ( int n = 0; n < nSpToClose; n++ )
		{
			int nIndex = arrayIndex.GetAt(n);

			if ( nSpToClose >= 2 )
				StatusProgress.SetPos( n, nSpToClose );
			
			CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;

			CloseCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusTransferDetail );
			
			if ( nErrorType == DATAMANAGER_ERROR_NONE )
				nErrorType = info.m_nErrorType;

			Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusTransferDetail = DATAMANAGER_CLOSED;
		}	

		if ( nSpToClose >= 2 )
			StatusProgress.Unlock();
	}

	info.m_nErrorType = nErrorType;
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerStockman::WriteStock( CDataManagerInfo& info, bool bItems, bool bLevels )
{
	ResetInfo( info, NODE_STOCK );

	if ( TRUE == bItems )
	{
		if ( m_nStatusStock != DATAMANAGER_OPEN_READWRITE )
		{
			info.m_nErrorType = DATAMANAGER_ERROR_READONLY;
			return FALSE;
		}

		WriteCSVArray( Stock.m_StockDatabase, info, m_nStatusStock );
	}

	int nErrorType = info.m_nErrorType;
	if ( DATAMANAGER_ERROR_NONE == nErrorType )
		WriteExportLiveApparentStockToken();

	if ( FALSE == bLevels )
	{
		info.m_nErrorType = nErrorType;
		return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
	}

	{
		int nProgressTarget = 0;
		int nProgressSoFar = 0;

		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusStockLevels == DATAMANAGER_OPEN_READWRITE )
			{
				CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

				if ( pArray -> IsModified() == TRUE )
					nProgressTarget++;
			}
		}

		StatusProgress.Lock( TRUE, "Writing stock level files" );
		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusStockLevels == DATAMANAGER_OPEN_READWRITE )
			{
				CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

				if ( pArray -> IsModified() == TRUE )
				{
					int nDummy = DATAMANAGER_OPEN_READWRITE;
					WriteCSVArray( *pArray, info, nDummy );
			
					if ( nErrorType == DATAMANAGER_ERROR_NONE )
						nErrorType = info.m_nErrorType;

					nProgressSoFar++;
					StatusProgress.SetPos( nProgressSoFar, nProgressTarget );
				}
			}
		}
		StatusProgress.Unlock();
	}

	{
		int nProgressTarget = 0;
		int nProgressSoFar = 0;

		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusTransferDetail == DATAMANAGER_OPEN_READWRITE )
			{
				CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;

				if ( pArray -> IsModified() == TRUE )
					nProgressTarget++;
			}
		}

		StatusProgress.Lock( TRUE, "Writing transfer detail files" );
		for ( int nIndex = 0; nIndex < Stock.m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			if ( Stock.m_arrayStockMonitorInfo[nIndex].m_nDataManagerStatusTransferDetail == DATAMANAGER_OPEN_READWRITE )
			{
				CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;

				if ( pArray -> IsModified() == TRUE )
				{
					int nDummy = DATAMANAGER_OPEN_READWRITE;
					WriteCSVArray( *pArray, info, nDummy );
			
					if ( nErrorType == DATAMANAGER_ERROR_NONE )
						nErrorType = info.m_nErrorType;

					nProgressSoFar++;
					StatusProgress.SetPos( nProgressSoFar, nProgressTarget );
				}
			}
		}
		StatusProgress.Unlock();
	}

	info.m_nErrorType = nErrorType;
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerStockman::OpenStockLevels( int nSpIdx, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKLEVEL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pStockLevels;

	CFilenameUpdater FnUp( SysFiles::StockLevels, nSpIdx );
	CString	strFilename = FnUp.GetFilenameToUse();
	
	OpenCSVArray( *pArray, info, strFilename, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels, nType, TRUE );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

bool CDataManagerStockman::CloseStockLevels( int nSpIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKLEVEL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pStockLevels;

	CFilenameUpdater FnUp( SysFiles::StockLevels, nSpIdx );
	CString strFilename = FnUp.GetFilenameToUse();
	
	CloseCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

bool CDataManagerStockman::WriteStockLevels( int nSpIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKLEVEL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	if ( Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels != DATAMANAGER_OPEN_READWRITE )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_READONLY;
		return FALSE;
	}

	CStockLevelsCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pStockLevels;
	
	WriteCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusStockLevels );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;

	WriteExportLiveApparentStockToken();
	
	return TRUE;
}

bool CDataManagerStockman::OpenTransferDetail( int nSpIdx, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TRANSFER_DETAIL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pTransferDetail;

	CFilenameUpdater FnUp( SysFiles::TransferDetail, nSpIdx );
	CString	strFilename = FnUp.GetFilenameToUse();
	
	OpenCSVArray( *pArray, info, strFilename, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail, nType, TRUE );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

bool CDataManagerStockman::CloseTransferDetail( int nSpIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TRANSFER_DETAIL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pTransferDetail;

	CFilenameUpdater FnUp( SysFiles::TransferDetail, nSpIdx );
	CString strFilename = FnUp.GetFilenameToUse();
	
	CloseCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

bool CDataManagerStockman::WriteTransferDetail( int nSpIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TRANSFER_DETAIL );
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange ( m_nActiveDbIdx, nStartIdx, nEndIdx );

	if ( nSpIdx < nStartIdx || nSpIdx > nEndIdx )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_SPIDX;
		return FALSE;
	}

	if ( Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail != DATAMANAGER_OPEN_READWRITE )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_READONLY;
		return FALSE;
	}

	CTransferDetailCSVArray* pArray = Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_pTransferDetail;
	
	WriteCSVArray( *pArray, info, Stock.m_arrayStockMonitorInfo[ nSpIdx - nStartIdx ].m_nDataManagerStatusTransferDetail );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/

bool CDataManagerStockman::OpenSupplier( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	
	CFilenameUpdater FnUp( SysFiles::Supplier );
	bool bResult = OpenCSVArray( Supplier, info, FnUp.GetFilenameToUse(), m_nStatusSupplier, nType, TRUE );
	Supplier.ValidateSuppliers();
	return bResult;
}

bool CDataManagerStockman::CloseSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	return CloseCSVArray( Supplier, info, m_nStatusSupplier );
}

bool CDataManagerStockman::ForceCloseSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	return CDataManagerBase::CloseCSVArray( Supplier, info, m_nStatusSupplier );
}

bool CDataManagerStockman::WriteSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	Supplier.ValidateSuppliers();	
	return WriteCSVArray( Supplier, info, m_nStatusSupplier );
}

bool CDataManagerStockman::ForceWriteSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	Supplier.ValidateSuppliers();	
	return CDataManagerBase::WriteCSVArray( Supplier, info, m_nStatusSupplier );
}

bool CDataManagerStockman::OpenSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );

	CFilenameUpdater FnUp( SysFiles::Supplier );
	if ( OpenCSVArray( SupplierBuffer, info, FnUp.GetFilenameToUse(), m_nStatusSupplierBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;
	
	Supplier.CopyArray( SupplierBuffer );
	return TRUE;
}

bool CDataManagerStockman::WriteSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	
	CSupplierCSVArray temp;
	temp.CopyArray( SupplierBuffer );
	SupplierBuffer.CopyArray( Supplier);

	bool bResult = WriteCSVArray( SupplierBuffer, info, m_nStatusSupplierBuffer );

	if ( bResult == FALSE )
		SupplierBuffer.CopyArray( temp );

	return bResult;
}

bool CDataManagerStockman::CloseSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER );
	Supplier.CopyArray( SupplierBuffer );
	return CloseCSVArray( SupplierBuffer, info, m_nStatusSupplierBuffer );
}

/**********************************************************************/

bool CDataManagerStockman::OpenSupplierSet( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER_SET );
	
	CFilenameUpdater FnUp( SysFiles::SupplierSet );
	return OpenCSVArray( SupplierSet, info, FnUp.GetFilenameToUse(), m_nStatusSupplierSet, nType, TRUE );
}

bool CDataManagerStockman::CloseSupplierSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER_SET );
	return CloseCSVArray( SupplierSet, info, m_nStatusSupplierSet );
}

bool CDataManagerStockman::ForceCloseSupplierSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER_SET );
	return CDataManagerBase::CloseCSVArray( SupplierSet, info, m_nStatusSupplierSet );
}

bool CDataManagerStockman::WriteSupplierSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER_SET );
	return WriteCSVArray( SupplierSet, info, m_nStatusSupplierSet );
}

bool CDataManagerStockman::ForceWriteSupplierSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUPPLIER_SET );
	return CDataManagerBase::WriteCSVArray( SupplierSet, info, m_nStatusSupplierSet );
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathAllowance ( int nDbIdx )
{
	if ( StockOptions.GetGlobalAllowanceFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::Allowance, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Allowance, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathCategory ( int nDbIdx )
{
	if ( StockOptions.GetGlobalCategoryFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::Category, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Category, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathCategorySet ( int nDbIdx )
{
	if ( StockOptions.GetGlobalCategoryFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::CategorySet, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::CategorySet, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathCustomFieldValue ( int nDbIdx, int nFieldNo )
{
	if ( StockOptions.GetGlobalCustomFieldFlag() == FALSE )
	{	
		CFilenameUpdater FnUp( SysFiles::CustomFieldValue, nDbIdx, nFieldNo );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::CustomFieldValue, -1, nFieldNo );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathCustomFieldSet ( int nDbIdx, int nFieldNo )
{
	if ( StockOptions.GetGlobalCustomFieldFlag() == FALSE )
	{	
		CFilenameUpdater FnUp( SysFiles::CustomFieldSet, nDbIdx, nFieldNo );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::CustomFieldSet, -1, nFieldNo );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathOrder( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::Orders, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}
/**********************************************************************/

const char* CDataManagerStockman::GetFilePathProductLabels( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::DbProductLabels, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathRecipe ( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::RecipeDatabase, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathStock ( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::StockDatabase, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerStockman::GetFilePathSupplier()
{
	CFilenameUpdater FnUp( SysFiles::Supplier );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

void CDataManagerStockman::WriteExportLiveApparentStockToken()
{
	if ( ( DealerFlags.GetExportLiveApparentStockFlag() == TRUE ) && ( DealerFlags.GetBlockExportLiveApparentStockFlag() == FALSE ) )
	{
		CFilenameUpdater FnUp( SysFiles::ExportLiveApptStockToken, m_nActiveDbIdx );
		CString strFilename = FnUp.GetFilenameToUse();
		
		CSSFile fileToken;
		if ( fileToken.Open( strFilename, "wb" ) == TRUE )
			fileToken.WriteLine( "Microtrend" );
	}
}

/**********************************************************************/

void CDataManagerStockman::GetCustomFieldPtrArray( CPtrArray& CustomFieldPtrArray )
{
	CustomFieldPtrArray.RemoveAll();
	CustomFieldPtrArray.Add( &CustomFieldValue[1] );
	CustomFieldPtrArray.Add( &CustomFieldValue[2] );
	CustomFieldPtrArray.Add( &CustomFieldValue[3] );
	CustomFieldPtrArray.Add( &CustomFieldValue[4] );
	CustomFieldPtrArray.Add( &CustomFieldValue[5] );
}

/**********************************************************************/
