/**********************************************************************/
#include "DataManagerNonDbFuncStockman.h"
/**********************************************************************/

CDataManagerNonDbFuncStockman::CDataManagerNonDbFuncStockman() : CDataManagerBase(),
	AdjustText( NODE_ADJUSTTEXT ), AdjustTextBuffer( NODE_ADJUSTTEXT ),
	StockUnit ( NODE_STOCKUNIT ), StockUnitBuffer ( NODE_STOCKUNIT ),  
	SubUnit ( NODE_SUBUNIT ), SubUnitBuffer( NODE_SUBUNIT )
{
	m_nStatusAdjustText = DATAMANAGER_CLOSED;
	m_nStatusCustomFieldNames = DATAMANAGER_CLOSED;
	m_nStatusSalesHistoryStock = DATAMANAGER_CLOSED;
	m_nStatusStockUnit = DATAMANAGER_CLOSED;
	m_nStatusSubUnit = DATAMANAGER_CLOSED;
	m_nStatusSystemCategory = DATAMANAGER_CLOSED;
	m_nStatusSystemCategorySet = DATAMANAGER_CLOSED;
	
	m_nStatusAdjustTextBuffer = DATAMANAGER_CLOSED;
	m_nStatusSalesHistoryStockBuffer = DATAMANAGER_CLOSED;
	m_nStatusStockUnitBuffer = DATAMANAGER_CLOSED;
	m_nStatusSubUnitBuffer = DATAMANAGER_CLOSED;

	for ( int n = 0; n < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS; n++ )
	{
		m_nStatusStockReportSortOrder[n] = DATAMANAGER_CLOSED;
		m_nStatusStockReportSortOrderBuffer[n] = DATAMANAGER_CLOSED;
	}

	for ( int n = 0; n < MAX_ITEM_CUSTOM_FIELDS; n++ )
	{
		m_nStatusSystemCustomFieldSet[n] = DATAMANAGER_CLOSED;
	}

	m_nActiveStockReportSortOrderType = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenAdjustText( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );
	CFilenameUpdater FnUp ( SysFiles::AdjustText );
	return OpenStringArray( AdjustText, info, FnUp.GetFilenameToUse(), m_nStatusAdjustText, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseAdjustText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );
	return CloseStringArray( AdjustText, info, m_nStatusAdjustText );
}

bool CDataManagerNonDbFuncStockman::WriteAdjustText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );
	return WriteStringArray( AdjustText, info, m_nStatusAdjustText );
}

bool CDataManagerNonDbFuncStockman::OpenAdjustTextBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );

	CFilenameUpdater FnUp ( SysFiles::AdjustText );
	if ( OpenStringArray( AdjustTextBuffer, info, FnUp.GetFilenameToUse(), m_nStatusAdjustTextBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	AdjustText.CopyFrom( AdjustTextBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncStockman::WriteAdjustTextBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );
	
	CDefTextUnit temp( NODE_ADJUSTTEXT );
	temp.CopyFrom( AdjustTextBuffer );
	AdjustTextBuffer.CopyFrom( AdjustText);

	bool bResult = WriteStringArray( AdjustTextBuffer, info, m_nStatusAdjustTextBuffer );

	if ( bResult == FALSE )
		AdjustTextBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerNonDbFuncStockman::CloseAdjustTextBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ADJUSTTEXT );
	AdjustText.CopyFrom( AdjustTextBuffer );
	return CloseStringArray( AdjustTextBuffer, info, m_nStatusAdjustTextBuffer );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenCustomFieldNames( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOM_FIELD_NAME );
	CFilenameUpdater FnUp ( SysFiles::CustomFieldName );
	return OpenStringArray( CustomFieldNames, info, FnUp.GetFilenameToUse(), m_nStatusCustomFieldNames, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseCustomFieldNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOM_FIELD_NAME );
	return CloseStringArray( CustomFieldNames, info, m_nStatusCustomFieldNames );
}

bool CDataManagerNonDbFuncStockman::WriteCustomFieldNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOM_FIELD_NAME );
	return WriteStringArray( CustomFieldNames, info, m_nStatusCustomFieldNames );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenSalesHistoryStock( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );
	CFilenameUpdater FnUp ( SysFiles::StockHistory );
	return OpenCSVArray( SalesHistoryStock, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryStock, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseSalesHistoryStock( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );
	return CloseCSVArray( SalesHistoryStock, info, m_nStatusSalesHistoryStock );
}

bool CDataManagerNonDbFuncStockman::WriteSalesHistoryStock( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );
	return WriteCSVArray( SalesHistoryStock, info, m_nStatusSalesHistoryStock );
}

bool CDataManagerNonDbFuncStockman::OpenSalesHistoryStockBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );

	CFilenameUpdater FnUp ( SysFiles::StockHistory );
	if ( OpenCSVArray( SalesHistoryStockBuffer, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryStockBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	SalesHistoryStock.CopyFrom( SalesHistoryStockBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncStockman::CloseSalesHistoryStockBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );
	SalesHistoryStock.CopyFrom( SalesHistoryStockBuffer );
	return CloseCSVArray( SalesHistoryStockBuffer, info, m_nStatusSalesHistoryStockBuffer );
}

bool CDataManagerNonDbFuncStockman::WriteSalesHistoryStockBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_STOCK );
	
	CSalesHistoryCSVArrayStock temp;
	temp.CopyFrom( SalesHistoryStockBuffer );
	SalesHistoryStockBuffer.CopyFrom( SalesHistoryStock);

	bool bResult = WriteCSVArray( SalesHistoryStockBuffer, info, m_nStatusSalesHistoryStockBuffer );

	if ( bResult == FALSE )
		SalesHistoryStockBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenStockUnit( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );
	CFilenameUpdater FnUp ( SysFiles::StockUnit );
	return OpenStringArray( StockUnit, info, FnUp.GetFilenameToUse(), m_nStatusStockUnit, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseStockUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );
	return CloseStringArray( StockUnit, info, m_nStatusStockUnit );
}

bool CDataManagerNonDbFuncStockman::WriteStockUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );
	return WriteStringArray( StockUnit, info, m_nStatusStockUnit );
}

bool CDataManagerNonDbFuncStockman::OpenStockUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );

	CFilenameUpdater FnUp ( SysFiles::StockUnit );
	if ( OpenStringArray( StockUnitBuffer, info, FnUp.GetFilenameToUse(), m_nStatusStockUnitBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	StockUnit.CopyFrom( StockUnitBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncStockman::WriteStockUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );
	
	CDefTextUnit temp( NODE_STOCKUNIT );
	temp.CopyFrom( StockUnitBuffer );
	StockUnitBuffer.CopyFrom( StockUnit);

	bool bResult = WriteStringArray( StockUnitBuffer, info, m_nStatusStockUnitBuffer );

	if ( bResult == FALSE )
		StockUnitBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerNonDbFuncStockman::CloseStockUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCKUNIT );
	StockUnit.CopyFrom( StockUnitBuffer );
	return CloseStringArray( StockUnitBuffer, info, m_nStatusStockUnitBuffer );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenSubUnit( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );
	CFilenameUpdater FnUp ( SysFiles::SubUnit );
	return OpenStringArray( SubUnit, info, FnUp.GetFilenameToUse(), m_nStatusSubUnit, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseSubUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );
	return CloseStringArray( SubUnit, info, m_nStatusSubUnit );
}

bool CDataManagerNonDbFuncStockman::WriteSubUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );
	return WriteStringArray( SubUnit, info, m_nStatusSubUnit );
}

bool CDataManagerNonDbFuncStockman::OpenSubUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );

	CFilenameUpdater FnUp ( SysFiles::SubUnit );
	if ( OpenStringArray( SubUnitBuffer, info, FnUp.GetFilenameToUse(), m_nStatusSubUnitBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	SubUnit.CopyFrom( SubUnitBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncStockman::WriteSubUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );
	
	CDefTextUnit temp( NODE_SUBUNIT );
	temp.CopyFrom( SubUnitBuffer );
	SubUnitBuffer.CopyFrom( SubUnit);

	bool bResult = WriteStringArray( SubUnitBuffer, info, m_nStatusSubUnitBuffer );

	if ( bResult == FALSE )
		SubUnitBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerNonDbFuncStockman::CloseSubUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SUBUNIT );
	SubUnit.CopyFrom( SubUnitBuffer );
	return CloseStringArray( SubUnitBuffer, info, m_nStatusSubUnitBuffer );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenSystemCategory( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	CFilenameUpdater FnUp ( SysFiles::SystemCategory ); 
	return OpenStringArray( SystemCategory, info, FnUp.GetFilenameToUse(), m_nStatusSystemCategory, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseSystemCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	return CloseStringArray( SystemCategory, info, m_nStatusSystemCategory );
}

bool CDataManagerNonDbFuncStockman::WriteSystemCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY );
	return WriteStringArray( SystemCategory, info, m_nStatusSystemCategory );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenSystemCategorySet( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	CFilenameUpdater FnUp ( SysFiles::SystemCategorySet );
	return OpenCSVArray( SystemCategorySet, info, FnUp.GetFilenameToUse(), m_nStatusSystemCategorySet, nType, FALSE );
}

bool CDataManagerNonDbFuncStockman::CloseSystemCategorySet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	return CloseCSVArray( SystemCategorySet, info, m_nStatusSystemCategorySet );
}

bool CDataManagerNonDbFuncStockman::WriteSystemCategorySet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CATEGORY_SET );
	return WriteCSVArray( SystemCategorySet, info, m_nStatusSystemCategorySet );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenSystemCustomFieldSet( int nFieldNo, int nType, CDataManagerInfo& info )
{
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
	{
		return FALSE;
	}
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		CFilenameUpdater FnUp ( SysFiles::SystemCustomFieldSet, -1, nFieldNo ); 
		return OpenStringArray( SystemCustomFieldSet[nFieldNo], info, FnUp.GetFilenameToUse(), m_nStatusSystemCustomFieldSet[nFieldNo], nType, FALSE );
	}
}

bool CDataManagerNonDbFuncStockman::CloseSystemCustomFieldSet( int nFieldNo, CDataManagerInfo& info )
{
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
	{
		return FALSE;
	}
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		return CloseStringArray( SystemCustomFieldSet[nFieldNo], info, m_nStatusSystemCustomFieldSet[nFieldNo] );
	}
}

bool CDataManagerNonDbFuncStockman::WriteSystemCustomFieldSet( int nFieldNo, CDataManagerInfo& info )
{
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
	{
		return FALSE;
	}
	else
	{
		ResetInfo( info, NODE_CUSTOM_FIELD_SET );
		return WriteStringArray( SystemCustomFieldSet[nFieldNo], info, m_nStatusSystemCustomFieldSet[nFieldNo] );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CDataManagerNonDbFuncStockman::GetStockReportSortOrderIndexFromType( int nType )
{
	int nIndex = 0;

	if ( ( nType >= 0 ) && ( nType < MAX_STOCKREPORT_SORTORDERS ) )
		nIndex = nType;

	else if ( ( nType >= 100 ) && ( nType < 100 + MAX_STOCKTAKE_SORTORDERS ) )
		nIndex = nType + MAX_STOCKREPORT_SORTORDERS - 100;

	return nIndex;
}

/**********************************************************************/

void CDataManagerNonDbFuncStockman::SetActiveStockReportSortOrderType( int nType )
{
	if ( ( nType >= 0 ) && ( nType < MAX_STOCKREPORT_SORTORDERS ) )
		m_nActiveStockReportSortOrderType = nType;
	
	else if ( ( nType >= 100 ) && ( nType < 100 + MAX_STOCKTAKE_SORTORDERS ) )
		m_nActiveStockReportSortOrderType = nType;
}

/**********************************************************************/

CStockReportSortOrderCSVArray* CDataManagerNonDbFuncStockman::GetActiveStockReportSortOrder()
{
	int nIndex = GetStockReportSortOrderIndexFromType( m_nActiveStockReportSortOrderType );
	return &StockReportSortOrder[ nIndex ];
}

/**********************************************************************/

CStockReportSortOrderCSVArray* CDataManagerNonDbFuncStockman::GetActiveStockReportSortOrderBuffer()
{
	int nIndex = GetStockReportSortOrderIndexFromType( m_nActiveStockReportSortOrderType );
	return &StockReportSortOrderBuffer[ nIndex ];
}

/**********************************************************************/

void CDataManagerNonDbFuncStockman::OpenAllStockReportSortOrders()
{
	for ( int nType = 0; nType < MAX_STOCKREPORT_SORTORDERS; nType++ )
	{
		CDataManagerInfo info;
		SetActiveStockReportSortOrderType(nType);
		OpenActiveStockReportSortOrder( DB_READONLY, info );
	}
}

/**********************************************************************/

void CDataManagerNonDbFuncStockman::CloseAllStockReportSortOrders()
{
	for ( int nType = 0; nType < MAX_STOCKREPORT_SORTORDERS; nType++ )
	{
		CDataManagerInfo info;
		SetActiveStockReportSortOrderType(nType);
		CloseActiveStockReportSortOrder( info );
	}
}

/**********************************************************************/

void CDataManagerNonDbFuncStockman::OpenAllStockTakeSortOrders()
{
	for ( int nType = 100; nType < 100 + MAX_STOCKTAKE_SORTORDERS; nType++ )
	{
		CDataManagerInfo info;
		SetActiveStockReportSortOrderType(nType);
		OpenActiveStockReportSortOrder( DB_READONLY, info );
	}
}

/**********************************************************************/

void CDataManagerNonDbFuncStockman::CloseAllStockTakeSortOrders()
{
	for ( int nType = 100; nType < 100 + MAX_STOCKTAKE_SORTORDERS; nType++ )
	{
		CDataManagerInfo info;
		SetActiveStockReportSortOrderType(nType);
		CloseActiveStockReportSortOrder( info );
	}
}

/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenActiveStockReportSortOrder( int nType, CDataManagerInfo& info )
{
	return OpenStockReportSortOrder( nType, info, m_nActiveStockReportSortOrderType );
}

bool CDataManagerNonDbFuncStockman::CloseActiveStockReportSortOrder( CDataManagerInfo& info )
{
	return CloseStockReportSortOrder( info, m_nActiveStockReportSortOrderType );
}

bool CDataManagerNonDbFuncStockman::WriteActiveStockReportSortOrder( CDataManagerInfo& info )
{
	return WriteStockReportSortOrder( info, m_nActiveStockReportSortOrderType );
}

bool CDataManagerNonDbFuncStockman::OpenActiveStockReportSortOrderBuffer( CDataManagerInfo& info )
{
	return OpenStockReportSortOrderBuffer( info, m_nActiveStockReportSortOrderType );
}

bool CDataManagerNonDbFuncStockman::CloseActiveStockReportSortOrderBuffer( CDataManagerInfo& info )
{
	return CloseStockReportSortOrderBuffer( info, m_nActiveStockReportSortOrderType );
}

bool CDataManagerNonDbFuncStockman::WriteActiveStockReportSortOrderBuffer( CDataManagerInfo& info )
{
	return WriteStockReportSortOrderBuffer( info, m_nActiveStockReportSortOrderType );
}

/**********************************************************************/

bool CDataManagerNonDbFuncStockman::OpenStockReportSortOrder( int nType, CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );
		CFilenameUpdater FnUp ( SysFiles::StockReportSortOrder, 0, nIndex );
		bResult = OpenCSVArray( StockReportSortOrder[ nIndex ], info, FnUp.GetFilenameToUse(), m_nStatusStockReportSortOrder[ nIndex ], nType, FALSE );
	}
	
	return bResult;
}

bool CDataManagerNonDbFuncStockman::CloseStockReportSortOrder( CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );
		bResult = CloseCSVArray( StockReportSortOrder[ nIndex ], info, m_nStatusStockReportSortOrder[ nIndex ] );
	}
	
	return bResult;
}

bool CDataManagerNonDbFuncStockman::WriteStockReportSortOrder( CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );
		return WriteCSVArray( StockReportSortOrder[ nIndex ], info, m_nStatusStockReportSortOrder[ nIndex ] );
	}

	return bResult;
}

bool CDataManagerNonDbFuncStockman::OpenStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );

		CFilenameUpdater FnUp ( SysFiles::StockReportSortOrder, 0, nIndex );
		if ( OpenCSVArray( StockReportSortOrderBuffer[ nIndex ], info, FnUp.GetFilenameToUse(), m_nStatusStockReportSortOrderBuffer[ nIndex ], DB_READWRITE, FALSE ) == TRUE )
		{
			StockReportSortOrder[ nIndex ].CopyFrom( StockReportSortOrderBuffer[ nIndex ] );
			bResult = TRUE;
		}
	}

	return bResult;
}

bool CDataManagerNonDbFuncStockman::CloseStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );
		StockReportSortOrder[ nIndex ].CopyFrom( StockReportSortOrderBuffer[ nIndex ] );
		bResult = CloseCSVArray( StockReportSortOrderBuffer[ nIndex ], info, m_nStatusStockReportSortOrderBuffer[ nIndex ] );
	}

	return bResult;
}

bool CDataManagerNonDbFuncStockman::WriteStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType )
{
	bool bResult = FALSE;

	int nIndex = GetStockReportSortOrderIndexFromType( nSOType );
	if ( ( nIndex >= 0 ) && ( nIndex < MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ) )
	{
		ResetInfo( info, NODE_STOCKREPORT_SORTORDER );
	
		CStockReportSortOrderCSVArray temp;
		temp.CopyFrom( StockReportSortOrderBuffer[ nIndex ] );
		StockReportSortOrderBuffer[ nIndex ].CopyFrom( StockReportSortOrder[ nIndex ] );

		bResult = WriteCSVArray( StockReportSortOrderBuffer[ nIndex ], info, m_nStatusStockReportSortOrderBuffer[ nIndex ] );

		if ( bResult == FALSE )
			StockReportSortOrderBuffer[ nIndex ].CopyFrom( temp );
	}

	return bResult;
}

/**********************************************************************/

