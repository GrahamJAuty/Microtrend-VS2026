/**********************************************************************/
#include "FileRemover.h"
#include "MessageLogger.h"
/**********************************************************************/
#include "DataManagerBase.h"
/**********************************************************************/

CDataManagerBase::CDataManagerBase() 
{
	m_bNoLogWrite = FALSE;
}	
/**********************************************************************/

CDataManagerBase::~CDataManagerBase()
{
}

/**********************************************************************/

bool CDataManagerBase::OpenStringArray( CSharedStringArray& array, CDataManagerInfo& info, const char* szFilename, int& nCurrentState, int nNewState, bool bNoFileIsOK )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	if ( nCurrentState != DATAMANAGER_CLOSED )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_ALREADYOPEN;
	}
	else
	{
		info.m_nErrorType = array.Open( szFilename, nNewState );

		if ( bNoFileIsOK == TRUE )
			if ( info.m_nErrorType != DB_ERR_SHARE )
				if ( nNewState == DB_READONLY )
					info.m_nErrorType = DATAMANAGER_ERROR_NONE;
		
		if ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ) 
			nCurrentState = nNewState;
	}
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerBase::CloseStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	array.SetModifiedFlag( FALSE );
		
	switch ( nCurrentState )
	{
	case DATAMANAGER_OPEN_READONLY:
	case DATAMANAGER_OPEN_READWRITE:
		array.Close();
		info.m_nErrorType = DATAMANAGER_ERROR_NONE;
		break;

	case DATAMANAGER_CLOSED:
	default:
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		break;
	}
	
	nCurrentState = DATAMANAGER_CLOSED;
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerBase::WriteStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	switch ( nCurrentState )
	{
	case DATAMANAGER_OPEN_READWRITE:
		if ( array.ForceWrite() == FALSE )
			info.m_nErrorType = DATAMANAGER_ERROR_WRITE;
		else
			info.m_nErrorType = DATAMANAGER_ERROR_NONE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		info.m_nErrorType = DATAMANAGER_ERROR_READONLY;
		break;
			
	case DATAMANAGER_CLOSED:
	default:
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		break;
	}

	if ( ( DealerFlags.GetLogFileWritesFlag() == TRUE ) && ( FALSE == m_bNoLogWrite ) )
		MessageLogger.LogFileWrite( array.GetFilename(), info.m_nErrorType, GetLastError() );
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerBase::OpenCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, const char* szFilename, int& nCurrentState, int nNewState, bool bNoFileIsOK )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	if ( nCurrentState != DATAMANAGER_CLOSED )
		info.m_nErrorType = DATAMANAGER_ERROR_ALREADYOPEN;
	else
	{
		info.m_nErrorType = array.Open( szFilename, nNewState );

		if ( bNoFileIsOK == TRUE )
			if ( info.m_nErrorType != DB_ERR_SHARE )
				if ( nNewState == DB_READONLY )
					info.m_nErrorType = DATAMANAGER_ERROR_NONE;

		if ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ) 
			nCurrentState = nNewState;
	}
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerBase::CloseCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	array.SetModifiedFlag( FALSE );
		
	switch ( nCurrentState )
	{
	case DATAMANAGER_OPEN_READONLY:
	case DATAMANAGER_OPEN_READWRITE:
		array.Close();
		info.m_nErrorType = DATAMANAGER_ERROR_NONE;
		break;

	case DATAMANAGER_CLOSED:
	default:
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		break;
	}

	nCurrentState = DATAMANAGER_CLOSED;
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

bool CDataManagerBase::WriteCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	switch ( nCurrentState )
	{
	case DATAMANAGER_OPEN_READWRITE:
		if ( array.ForceWrite() == FALSE )
			info.m_nErrorType = DATAMANAGER_ERROR_WRITE;
		else
			info.m_nErrorType = DATAMANAGER_ERROR_NONE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		info.m_nErrorType = DATAMANAGER_ERROR_READONLY;
		break;

	case DATAMANAGER_CLOSED:
	default:
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		break;
	}

	if ( ( DealerFlags.GetLogFileWritesFlag() == TRUE ) && ( FALSE == m_bNoLogWrite ) )
		MessageLogger.LogFileWrite( array.GetFilename(), info.m_nErrorType, GetLastError() );
	
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE );
}

/**********************************************************************/

void CDataManagerBase::ResetInfo( CDataManagerInfo& info, int nNodeType )
{
	info.m_nNodeType = nNodeType;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;
}

/**********************************************************************/
