/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/

CDataManagerPosTray::CDataManagerPosTray() : CDataManager()
{
	m_nStatusRecipe = DATAMANAGER_CLOSED;
	m_nStatusStock = DATAMANAGER_CLOSED;
}

/**********************************************************************/

void CDataManagerPosTray::OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen )
{
	CDataManager::OpenDatabaseQuickNoFiles( nDbIdx, bKeepIfAlreadyOpen );
}

/**********************************************************************/

bool CDataManagerPosTray::OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	return CDataManager::OpenDatabaseReadOnly( nDbIdx, info );
}

/**********************************************************************/

bool CDataManagerPosTray::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	return CDataManager::OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info );
}

/**********************************************************************/

bool CDataManagerPosTray::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bOpenStockLevels, bool bOpenTransferDetail )
{
	return CDataManager::OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info );
}

/**********************************************************************/

bool CDataManagerPosTray::OpenSpecialReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info )
{
	int nAccessStock = nDefaultType;

	for ( int n = 0; n < arrayReadWrite.GetSize(); n++ )
	{
		switch( arrayReadWrite.GetAt(n) )
		{
		case NODE_STOCK:			nAccessStock = DB_READWRITE;			break;
		}
	}

	for ( int n = 0; n < arrayReadOnly.GetSize(); n++ )
	{
		switch( arrayReadOnly.GetAt(n) )
		{
		case NODE_STOCK:			nAccessStock = DB_READONLY;				break;
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

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

bool CDataManagerPosTray::OpenSpecialReadOnly( bool bSameDb, CDataManagerInfo& info )
{
	if ( ( m_nStatusStock != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseStock( info );
		OpenStock( DB_READONLY, info );
	}

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

void CDataManagerPosTray::CloseSpecial()
{
	CDataManagerInfo info;
	CloseStock( info );
}

/**********************************************************************/

bool CDataManagerPosTray::OpenPlu( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	CString strFilename1 = GetFilePathPlu( m_nActiveDbIdx );
	CString strFilename2 = GetFilePathRecipe( m_nActiveDbIdx );

	OpenCSVArray( Plu.m_PluDatabase, info, strFilename1, m_nStatusPlu, nType, TRUE );
	
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	OpenCSVArray( Plu.m_RecipeDatabase, info, strFilename2, m_nStatusRecipe, nType, TRUE );
		
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
		
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
	return TRUE;
}

/**********************************************************************/

bool CDataManagerPosTray::ReopenPlu( CDataManagerInfo& info )
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

bool CDataManagerPosTray::ClosePlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	CloseCSVArray( Plu.m_PluDatabase, info, m_nStatusPlu );
		
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

bool CDataManagerPosTray::OpenStock( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );
	
	CString strFilename = GetFilePathStock( m_nActiveDbIdx );
	
	OpenCSVArray( Stock.m_StockDatabase, info, strFilename, m_nStatusStock, nType, TRUE );
	if ( info.m_nErrorType != DATAMANAGER_ERROR_NONE )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/

bool CDataManagerPosTray::CloseStock( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_STOCK );
	CloseCSVArray( Stock.m_StockDatabase, info, m_nStatusStock );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

const char* CDataManagerPosTray::GetFilePathRecipe ( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::RecipeDatabase, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManagerPosTray::GetFilePathStock ( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::StockDatabase, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/
