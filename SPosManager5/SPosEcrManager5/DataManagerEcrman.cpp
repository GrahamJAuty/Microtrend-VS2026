/**********************************************************************/
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "NodeTypes.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
/**********************************************************************/

CDataManagerEcrman::CDataManagerEcrman() : CDataManager()
{
}

/**********************************************************************/

bool CDataManagerEcrman::OpenPlu( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );

	CString strFilename = GetFilePathPlu( m_nActiveDbIdx );
	
	if ( FALSE == m_bPluPriceSetMode )
		OpenCSVArray( Plu.m_PluDatabase, info, strFilename, m_nStatusPlu, nType, TRUE );
	else
		Plu.m_PluDatabase.Read( strFilename );

	PriceSetItemManager.Reset();
	KbdListNameArray.Read( m_nActiveDbIdx );

	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerEcrman::ReopenPlu( CDataManagerInfo& info )
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

bool CDataManagerEcrman::ClosePlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLU );
	CloseCSVArray( Plu.m_PluDatabase, info, m_nStatusPlu );
	PriceSetItemManager.Reset();
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerEcrman::WritePlu( CDataManagerInfo& info )
{ 
	ResetInfo( info, NODE_PLU );
	WriteCSVArray( Plu.m_PluDatabase, info, m_nStatusPlu );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

