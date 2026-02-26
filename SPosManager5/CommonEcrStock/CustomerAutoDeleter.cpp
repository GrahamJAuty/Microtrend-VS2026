/**********************************************************************/
#include "LocationCSVArray.h"
#include "MessageLogger.h"
/**********************************************************************/
#include "CustomerAutoDeleter.h"
/**********************************************************************/

CCustomerAutoDeleteInfo::CCustomerAutoDeleteInfo()
{
	Reset();
}

/**********************************************************************/

void CCustomerAutoDeleteInfo::Reset()
{
	m_nNwkNo = 0;
	m_nLocNo = 0;
	m_nCheckDay = 0;
	m_FileTime = CTime::GetCurrentTime();
	m_LastCheckTime = COleDateTime::GetCurrentTime();
}

/**********************************************************************/

void CCustomerAutoDeleteInfo::Add ( CCustomerAutoDeleteInfo& source )
{
}

/**********************************************************************/

int CCustomerAutoDeleteInfo::Compare( CCustomerAutoDeleteInfo& source, int nHint )
{
	if ( m_nNwkNo != source.m_nNwkNo )
		return ( m_nNwkNo > source.m_nNwkNo ? 1 : -1 );
	
	if ( m_nLocNo != source.m_nLocNo )
		return ( m_nLocNo > source.m_nLocNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCustomerAutoDeleter::CCustomerAutoDeleter()
{
	m_nLocIdx = 0;
	m_bReadyToGo = FALSE;
	m_bUpdated = FALSE;
}

/**********************************************************************/

void CCustomerAutoDeleter::CheckLocIdx()
{
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CCustomerAutoDeleter::GotoNextLoc()
{
	m_nLocIdx++;
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CCustomerAutoDeleter::DeleteCustomerIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	CheckLocIdx();

	if ( dbLocation.GetSize() == 0 )
		return;

	if ( NeedUpdate() == FALSE )
		return;

	CDataManagerInfo info;
	if ( OpenCustomerFile( info ) == TRUE )
	{
		GetCustomerArray() -> LoadHeaderStartDateRecord();
		GetCustomerArray() -> MoveDeletionsToHelperArray();
		DeleteCustomerInternal();
		CloseCustomerFile( info );
	}
}

/**********************************************************************/

void CCustomerAutoDeleter::DeleteCustomerExternal( int nLocIdx )
{
	if ( CheckEnabled() == TRUE )
	{
		if ( m_bReadyToGo == FALSE )
			return;

		if ( ( nLocIdx < 0 ) || ( nLocIdx >= dbLocation.GetSize() ) )
			return;

		int nSavedLocIdx = m_nLocIdx;
		
		m_nLocIdx = nLocIdx;
		DeleteCustomerInternal();

		m_nLocIdx = nSavedLocIdx;
	}
}

/**********************************************************************/

void CCustomerAutoDeleter::DeleteCustomerInternal()
{	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nCurrentDay = ::GetDayNumberSince2011( timeNow );
	bool bDeletedCustomer = FALSE;

	for ( int nCustIdx = GetCustomerArray()  -> GetSize() - 1; nCustIdx >= 0; nCustIdx-- )
	{
		CCustomerCSVRecord Customer;
		GetCustomerArray() -> GetAt( nCustIdx, Customer );

		if ( Customer.GetAutoDeleteFlag() == TRUE )
		{
			if ( Customer.GetAutoDeleteDay() <= nCurrentDay )
			{
				int nCustNo = Customer.GetCustomerID();
				int nCreateDay = Customer.GetLatestCreationDay();

				CString strAge;
				if ( nCurrentDay >= nCreateDay )
					strAge.Format( "%d", nCurrentDay- nCreateDay );
				else
					strAge.Format( "%d*", nCreateDay - nCurrentDay );
				
				GetCustomerArray() -> RemoveAt(nCustIdx);

				CAutoDeleteCustLogInfo infoLog;
				infoLog.m_bOneOff = IsOneOff();
				infoLog.m_nHideLevel  = 0;
				infoLog.m_nLocIdx = m_nLocIdx;
				infoLog.m_strInfo.Format( "Auto Delete Customer (Nwk=%d Loc=%d ID=%d Days=%s)",
					dbLocation.GetNetworkNo( m_nLocIdx ),
					dbLocation.GetLocNo( m_nLocIdx ),
					nCustNo,
					(const char*) strAge );
		
				MessageLogger.LogAutoDeleteCustomer( infoLog );

				bDeletedCustomer = TRUE;
			}
		}
	}
		
	if ( TRUE == bDeletedCustomer )
	{
		CDataManagerInfo info;
		GetCustomerArray() -> SaveHeaderStartDateRecord();
		GetCustomerArray() -> MoveDeletionsToMainArray();
		WriteCustomerFile( info );
		GetCustomerArray() -> LoadHeaderStartDateRecord();
		GetCustomerArray() -> MoveDeletionsToHelperArray();
	}

	RememberUpdate();
}

/**********************************************************************/

bool CCustomerAutoDeleter::NeedUpdate()
{
	CCustomerAutoDeleteInfo info;
	info.m_nNwkNo = dbLocation.GetNetworkNo(m_nLocIdx);
	info.m_nLocNo = dbLocation.GetLocNo(m_nLocIdx);

	int nPos;
	if ( m_arrayAutoDeleteInfo.Find( info, nPos ) == FALSE )
		return TRUE;

	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nDayToday = ::GetDayNumberSince2011( timeNow );

	CFileStatus FileStatus;
	CFile::GetStatus( GetCustomerFilename(), FileStatus );

	m_arrayAutoDeleteInfo.GetAt( nPos, info );

	if ( info.m_nCheckDay != nDayToday )
		return TRUE;

	if ( info.m_FileTime != FileStatus.m_mtime )
		return TRUE;

	if ( info.m_LastCheckTime + COleDateTimeSpan ( 0, 0, 15, 0 ) < timeNow  )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

void CCustomerAutoDeleter::RememberUpdate()
{
	CCustomerAutoDeleteInfo info;
	info.m_nNwkNo = dbLocation.GetNetworkNo(m_nLocIdx);
	info.m_nLocNo = dbLocation.GetLocNo(m_nLocIdx);

	int nPos;
	bool bNewEntry = ( m_arrayAutoDeleteInfo.Find( info, nPos ) == FALSE );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nDayToday = ::GetDayNumberSince2011( timeNow );

	CFileStatus FileStatus;
	CFile::GetStatus( GetCustomerFilename(), FileStatus );

	info.m_nCheckDay = nDayToday;
	info.m_FileTime = FileStatus.m_mtime;
	info.m_LastCheckTime = timeNow;

	if ( TRUE == bNewEntry )
		m_arrayAutoDeleteInfo.InsertAt( nPos, info );
	else
		m_arrayAutoDeleteInfo.SetAt( nPos, info );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CCustomerAutoDeleterAccount::CheckEnabled()
{
	return EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag();
}

/**********************************************************************/

bool CCustomerAutoDeleterAccount::OpenCustomerFile( CDataManagerInfo& info )
{
	return DataManagerNonDb.OpenCustomerAccount( DB_READWRITE, m_nLocIdx, info );
}

/**********************************************************************/

void CCustomerAutoDeleterAccount::CloseCustomerFile( CDataManagerInfo& info )
{
	DataManagerNonDb.CloseCustomerAccount( info );
}

/**********************************************************************/

void CCustomerAutoDeleterAccount::WriteCustomerFile( CDataManagerInfo& info )
{
	DataManagerNonDb.WriteCustomerAccount( info );
}

/**********************************************************************/

CCustomerCSVArray* CCustomerAutoDeleterAccount::GetCustomerArray()
{
	return &DataManagerNonDb.CustomerAccount;
}

/**********************************************************************/

const char* CCustomerAutoDeleterAccount::GetCustomerFilename()
{
	CFilenameUpdater FnUp( SysFiles::CustomerAccount, m_nLocIdx );
	m_strCustomerFilename = FnUp.GetFilenameToUse();
	return m_strCustomerFilename;
}

/**********************************************************************/

const char* CCustomerAutoDeleterAccount::GetCustomerFileTypeName()
{
	m_strCustomerFileTypeName = "ACCOUNT";
	return m_strCustomerFileTypeName;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CCustomerAutoDeleterOneOff::CheckEnabled()
{
	return EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag();
}

/**********************************************************************/

bool CCustomerAutoDeleterOneOff::OpenCustomerFile( CDataManagerInfo& info )
{
	return DataManagerNonDb.OpenCustomerOneOff( DB_READWRITE, m_nLocIdx, info );
}

/**********************************************************************/

void CCustomerAutoDeleterOneOff::CloseCustomerFile( CDataManagerInfo& info )
{
	DataManagerNonDb.CloseCustomerOneOff( info );
}

/**********************************************************************/

void CCustomerAutoDeleterOneOff::WriteCustomerFile( CDataManagerInfo& info )
{
	DataManagerNonDb.WriteCustomerOneOff( info );
}

/**********************************************************************/

CCustomerCSVArray* CCustomerAutoDeleterOneOff::GetCustomerArray()
{
	return &DataManagerNonDb.CustomerOneOff;
}

/**********************************************************************/

const char* CCustomerAutoDeleterOneOff::GetCustomerFilename()
{
	CFilenameUpdater FnUp( SysFiles::CustomerOneOff, m_nLocIdx );
	m_strCustomerFilename = FnUp.GetFilenameToUse();
	return m_strCustomerFilename;
}

/**********************************************************************/

const char* CCustomerAutoDeleterOneOff::GetCustomerFileTypeName()
{
	m_strCustomerFileTypeName = "ONE-OFF";
	return m_strCustomerFileTypeName;
}

/**********************************************************************/
