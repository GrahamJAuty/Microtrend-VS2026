/**********************************************************************/
#include "LocationCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "DataManagerNonDbFuncCommon.h"
/**********************************************************************/

CDataManagerNonDbFuncCommon::CDataManagerNonDbFuncCommon(void) : CDataManagerBase(),
	SystemPluFilterNames( NODE_PLUFILTER_NAME )
{
	m_nStatusCustomerAccount = DATAMANAGER_CLOSED;
	m_nStatusCustomerOneOff = DATAMANAGER_CLOSED;
	m_nStatusDepositNames = DATAMANAGER_CLOSED;
	m_nStatusEmailAddress = DATAMANAGER_CLOSED;
	m_nStatusPosTrayExportFieldNames = DATAMANAGER_CLOSED;
	m_nStatusReportSession = DATAMANAGER_CLOSED;
	m_nStatusServer = DATAMANAGER_CLOSED;
	m_nStatusServerLogon = DATAMANAGER_CLOSED;
	m_nStatusSystemDepartment = DATAMANAGER_CLOSED;
	m_nStatusSystemDepartmentSet = DATAMANAGER_CLOSED;
	m_nStatusSystemPluFilterItems = DATAMANAGER_CLOSED;
	m_nStatusSystemPluFilterNames = DATAMANAGER_CLOSED;
	m_nStatusTableGroups = DATAMANAGER_CLOSED;
	m_nStatusTableGroupSets = DATAMANAGER_CLOSED;
	m_nStatusTableNames = DATAMANAGER_CLOSED;
	m_nStatusTimeAttendReportNames = DATAMANAGER_CLOSED;
	m_nStatusTimeSliceDetail = DATAMANAGER_CLOSED;
	m_nStatusTimeSliceList = DATAMANAGER_CLOSED;

	m_nStatusEmailAddressBuffer = DATAMANAGER_CLOSED;	
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenCustomerAll( int nType, int nLocIdx, CDataManagerInfo& info )
{
	bool bResult = FALSE;
	if ( OpenCustomerAccount( nType, nLocIdx, info ) == TRUE )
	{
		if ( OpenCustomerOneOff( nType, nLocIdx, info ) == TRUE )
			bResult = TRUE;
		else
		{
			CDataManagerInfo info2;
			CloseCustomerAccount( info2 );
		}
	}
	return bResult;
}

bool CDataManagerNonDbFuncCommon::CloseCustomerAll( CDataManagerInfo& info )
{
	bool bResult = CloseCustomerAccount( info );
	
	if ( TRUE == bResult )
		bResult = CloseCustomerOneOff( info );
	else
	{
		CDataManagerInfo info2;
		CloseCustomerOneOff( info2 );
	}

	return bResult;
}

bool CDataManagerNonDbFuncCommon::WriteCustomerAll( CDataManagerInfo& info )
{
	bool bResult = FALSE;
	if ( WriteCustomerAccount( info ) == TRUE )
		bResult = WriteCustomerOneOff( info );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenCustomerAccount( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ACCOUNT );
	CString strFilename = GetFilePathCustomerAccount( nLocIdx );
	OpenCSVArray( CustomerAccount, info, strFilename, m_nStatusCustomerAccount, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseCustomerAccount( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ACCOUNT );
	return CDataManagerBase::CloseCSVArray( CustomerAccount, info, m_nStatusCustomerAccount );
}

bool CDataManagerNonDbFuncCommon::WriteCustomerAccount( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ACCOUNT );
	return CDataManagerBase::WriteCSVArray( CustomerAccount, info, m_nStatusCustomerAccount );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenCustomerOneOff( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ONEOFF );
	CString strFilename = GetFilePathCustomerOneOff( nLocIdx );
	OpenCSVArray( CustomerOneOff, info, strFilename, m_nStatusCustomerOneOff, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseCustomerOneOff( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ONEOFF );
	return CDataManagerBase::CloseCSVArray( CustomerOneOff, info, m_nStatusCustomerOneOff );
}

bool CDataManagerNonDbFuncCommon::WriteCustomerOneOff( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_ONEOFF );
	return CDataManagerBase::WriteCSVArray( CustomerOneOff, info, m_nStatusCustomerOneOff );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenDepositNames( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPOSITNAME );
	CString strFilename = GetFilePathDepositNames( nLocIdx );
	OpenCSVArray( DepositNames, info, strFilename, m_nStatusDepositNames, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

/**********************************************************************/

bool CDataManagerNonDbFuncCommon::CloseDepositNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPOSITNAME );
	return CDataManagerBase::CloseCSVArray( DepositNames, info, m_nStatusDepositNames );
}

/**********************************************************************/

bool CDataManagerNonDbFuncCommon::WriteDepositNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPOSITNAME );
	return CDataManagerBase::WriteCSVArray( DepositNames, info, m_nStatusDepositNames );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenEmailAddress( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );
	CFilenameUpdater FnUp ( SysFiles::EmailAddress );
	return OpenStringArray( EmailAddress, info, FnUp.GetFilenameToUse(), m_nStatusEmailAddress, nType, TRUE );
}

bool CDataManagerNonDbFuncCommon::CloseEmailAddress( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );
	return CloseStringArray( EmailAddress, info, m_nStatusEmailAddress );
}

bool CDataManagerNonDbFuncCommon::WriteEmailAddress( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );
	return WriteStringArray( EmailAddress, info, m_nStatusEmailAddress );
}

bool CDataManagerNonDbFuncCommon::OpenEmailAddressBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );

	CFilenameUpdater FnUp (SysFiles::EmailAddress );
	if ( OpenStringArray( EmailAddressBuffer, info, FnUp.GetFilenameToUse(), m_nStatusEmailAddressBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	EmailAddress.CopyFrom( EmailAddressBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncCommon::WriteEmailAddressBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );
	
	CDefTextEmailAddress temp;
	temp.CopyFrom( EmailAddressBuffer );
	EmailAddressBuffer.CopyFrom( EmailAddress);

	bool bResult = WriteStringArray( EmailAddressBuffer, info, m_nStatusEmailAddressBuffer );

	if ( bResult == FALSE )
		EmailAddressBuffer.CopyFrom( temp );

	return bResult;
}

bool CDataManagerNonDbFuncCommon::CloseEmailAddressBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_EMAIL_ADDRESS );
	EmailAddress.CopyFrom( EmailAddressBuffer );
	return CloseStringArray( EmailAddressBuffer, info, m_nStatusEmailAddressBuffer );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenPosTrayExportFieldNames( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_POSTRAY_FIELDNAMES );
	CFilenameUpdater FnUp ( SysFiles::PosTrayExportFieldNames );
	return OpenStringArray( PosTrayExportFieldNames, info, FnUp.GetFilenameToUse(), m_nStatusPosTrayExportFieldNames, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::ClosePosTrayExportFieldNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_POSTRAY_FIELDNAMES );
	return CloseStringArray( PosTrayExportFieldNames, info, m_nStatusPosTrayExportFieldNames );
}

bool CDataManagerNonDbFuncCommon::WritePosTrayExportFieldNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_POSTRAY_FIELDNAMES );
	return WriteStringArray( PosTrayExportFieldNames, info, m_nStatusPosTrayExportFieldNames );
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenReportSession( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SESSION );
	CFilenameUpdater FnUp ( SysFiles::ReportSession );
	return OpenCSVArray( ReportSession, info, FnUp.GetFilenameToUse(), m_nStatusReportSession, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseReportSession( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SESSION );
	return CloseCSVArray( ReportSession, info, m_nStatusReportSession );
}

bool CDataManagerNonDbFuncCommon::WriteReportSession( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SESSION );
	return WriteCSVArray( ReportSession, info, m_nStatusReportSession );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenSalesHistoryPlu( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );
	CFilenameUpdater FnUp ( SysFiles::PluHistory );
	return OpenCSVArray( SalesHistoryPlu, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryPlu, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseSalesHistoryPlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );
	return CloseCSVArray( SalesHistoryPlu, info, m_nStatusSalesHistoryPlu );
}

bool CDataManagerNonDbFuncCommon::WriteSalesHistoryPlu( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );
	return WriteCSVArray( SalesHistoryPlu, info, m_nStatusSalesHistoryPlu );
}

bool CDataManagerNonDbFuncCommon::OpenSalesHistoryPluBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );

	CFilenameUpdater FnUp ( SysFiles::PluHistory );
	if ( OpenCSVArray( SalesHistoryPluBuffer, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryPluBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	SalesHistoryPlu.CopyFrom( SalesHistoryPluBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncCommon::CloseSalesHistoryPluBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );
	SalesHistoryPlu.CopyFrom( SalesHistoryPluBuffer );
	return CloseCSVArray( SalesHistoryPluBuffer, info, m_nStatusSalesHistoryPluBuffer );
}

bool CDataManagerNonDbFuncCommon::WriteSalesHistoryPluBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_PLU );
	
	CSalesHistoryCSVArrayPlu temp;
	temp.CopyFrom( SalesHistoryPluBuffer );
	SalesHistoryPluBuffer.CopyFrom( SalesHistoryPlu);

	bool bResult = WriteCSVArray( SalesHistoryPluBuffer, info, m_nStatusSalesHistoryPluBuffer );

	if ( bResult == FALSE )
		SalesHistoryPluBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenServer( int nType, int nDbIdx, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER );
	CString strFilename = GetFilePathServer( nDbIdx, nLocIdx );
	OpenCSVArray( Server, info, strFilename, m_nStatusServer, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseServer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER );
	return CDataManagerBase::CloseCSVArray( Server, info, m_nStatusServer );
}

bool CDataManagerNonDbFuncCommon::WriteServer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER );
	return CDataManagerBase::WriteCSVArray( Server, info, m_nStatusServer );
}

/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenServerLogon(int nType, int nDbIdx, int nLocIdx, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_SERVERLOGON);
	CString strFilename = GetFilePathServerLogon(nDbIdx, nLocIdx);
	OpenCSVArray(ServerLogon, info, strFilename, m_nStatusServerLogon, nType, TRUE);
	return (info.m_nErrorType == DATAMANAGER_ERROR_NONE);
}

bool CDataManagerNonDbFuncCommon::CloseServerLogon(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_SERVERLOGON);
	return CDataManagerBase::CloseCSVArray(ServerLogon, info, m_nStatusServerLogon);
}

bool CDataManagerNonDbFuncCommon::WriteServerLogon(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_SERVERLOGON);
	return CDataManagerBase::WriteCSVArray(ServerLogon, info, m_nStatusServerLogon);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenSystemDepartment( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	CFilenameUpdater FnUp ( SysFiles::SystemDepartment );
	return OpenCSVArray( SystemDepartment, info, FnUp.GetFilenameToUse(), m_nStatusSystemDepartment, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseSystemDepartment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	return CloseCSVArray( SystemDepartment, info, m_nStatusSystemDepartment );
}

bool CDataManagerNonDbFuncCommon::WriteSystemDepartment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	return WriteCSVArray( SystemDepartment, info, m_nStatusSystemDepartment );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenSystemDepartmentSet( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT_SET );
	CFilenameUpdater FnUp ( SysFiles::SystemDepartmentSet );
	return OpenCSVArray( SystemDepartmentSet, info, FnUp.GetFilenameToUse(), m_nStatusSystemDepartmentSet, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseSystemDepartmentSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT_SET );
	return CloseCSVArray( SystemDepartmentSet, info, m_nStatusSystemDepartmentSet );
}

bool CDataManagerNonDbFuncCommon::WriteSystemDepartmentSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT_SET );
	return WriteCSVArray( SystemDepartmentSet, info, m_nStatusSystemDepartmentSet );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenSystemPluFilterItems( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_ITEM );
	CFilenameUpdater FnUp ( SysFiles::PluFilterItems, -1 );
	return OpenCSVArray( SystemPluFilterItems, info, FnUp.GetFilenameToUse(), m_nStatusSystemPluFilterItems, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseSystemPluFilterItems( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_ITEM );
	return CloseCSVArray( SystemPluFilterItems, info, m_nStatusSystemPluFilterItems );
}

bool CDataManagerNonDbFuncCommon::WriteSystemPluFilterItems( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_ITEM );
	return WriteCSVArray( SystemPluFilterItems, info, m_nStatusSystemPluFilterItems );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenSystemPluFilterNames( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_NAME );
	CFilenameUpdater FnUp ( SysFiles::PluFilterNames, -1 );
	return OpenCSVArray( SystemPluFilterNames, info, FnUp.GetFilenameToUse(), m_nStatusSystemPluFilterNames, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseSystemPluFilterNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_NAME );
	return CloseCSVArray( SystemPluFilterNames, info, m_nStatusSystemPluFilterNames );
}

bool CDataManagerNonDbFuncCommon::WriteSystemPluFilterNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_NAME );
	return WriteCSVArray( SystemPluFilterNames, info, m_nStatusSystemPluFilterNames );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTableGroups( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUP );
	CString strFilename = GetFilePathTableGroup( nLocIdx );
	OpenCSVArray( TableGroups, info, strFilename, m_nStatusTableGroups, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseTableGroups( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUP );
	return CDataManagerBase::CloseCSVArray( TableGroups, info, m_nStatusTableGroups );
}

bool CDataManagerNonDbFuncCommon::WriteTableGroups( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUP );
	return CDataManagerBase::WriteCSVArray( TableGroups, info, m_nStatusTableGroups );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTableGroupSets( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUPSET );
	CString strFilename = GetFilePathTableGroupSet( nLocIdx );
	OpenCSVArray( TableGroupSets, info, strFilename, m_nStatusTableGroupSets, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseTableGroupSets( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUPSET );
	return CDataManagerBase::CloseCSVArray( TableGroupSets, info, m_nStatusTableGroupSets );
}

bool CDataManagerNonDbFuncCommon::WriteTableGroupSets( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_GROUPSET );
	return CDataManagerBase::WriteCSVArray( TableGroupSets, info, m_nStatusTableGroupSets );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTableNames( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_NAME );
	CString strFilename = GetFilePathTableName( nLocIdx );
	OpenCSVArray( TableNames, info, strFilename, m_nStatusTableNames, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncCommon::CloseTableNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_NAME );
	return CDataManagerBase::CloseCSVArray( TableNames, info, m_nStatusTableNames );
}

bool CDataManagerNonDbFuncCommon::WriteTableNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_NAME );
	return CDataManagerBase::WriteCSVArray( TableNames, info, m_nStatusTableNames );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTimeSliceDetail( int nFileNo, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_DETAIL );
	CFilenameUpdater FnUp ( SysFiles::TimeSliceDetail, -1, nFileNo );
	return OpenCSVArray( TimeSliceDetail, info, FnUp.GetFilenameToUse(), m_nStatusTimeSliceDetail, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseTimeSliceDetail( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_DETAIL );
	return CloseCSVArray( TimeSliceDetail, info, m_nStatusTimeSliceDetail );
}

bool CDataManagerNonDbFuncCommon::WriteTimeSliceDetail( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_DETAIL );
	return WriteCSVArray( TimeSliceDetail, info, m_nStatusTimeSliceDetail );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTimeSliceList( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_LIST );
	CFilenameUpdater FnUp ( SysFiles::TimeSliceList );
	return OpenCSVArray( TimeSliceList, info, FnUp.GetFilenameToUse(), m_nStatusTimeSliceList, nType, FALSE );
}

bool CDataManagerNonDbFuncCommon::CloseTimeSliceList( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_LIST );
	return CloseCSVArray( TimeSliceList, info, m_nStatusTimeSliceList );
}

bool CDataManagerNonDbFuncCommon::WriteTimeSliceList( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMESLICE_LIST );
	return WriteCSVArray( TimeSliceList, info, m_nStatusTimeSliceList );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathCustomerAccount( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::CustomerAccount, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathCustomerOneOff( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::CustomerOneOff, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathDepositNames( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::DepositNames, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathServer( int nDbIdx, int nLocIdx )
{
	if ( nDbIdx == -1 )
	{
		CFilenameUpdater FnUp( SysFiles::Servers, -1, -1, FNUP_FAMILY_SYSTEM );
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else if ( nLocIdx == -1 )
	{
		CFilenameUpdater FnUp( SysFiles::Servers, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Servers, nLocIdx, -1, FNUP_FAMILY_LOCATION );
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathServerLogon(int nDbIdx, int nLocIdx)
{
	if (nDbIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerLogons, -1, -1, FNUP_FAMILY_SYSTEM);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else if (nLocIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerLogons, nDbIdx, -1, FNUP_FAMILY_DATABASE);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ServerLogons, nLocIdx, -1, FNUP_FAMILY_LOCATION);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}

	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathServerCSVDef(int nDbIdx, int nLocIdx)
{
	if (nDbIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerCSVDef, -1, -1, FNUP_FAMILY_SYSTEM);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else if (nLocIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerCSVDef, nDbIdx, -1, FNUP_FAMILY_DATABASE);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ServerCSVDef, nLocIdx, -1, FNUP_FAMILY_LOCATION);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}

	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathServerImport(int nDbIdx, int nLocIdx)
{
	if (nDbIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerImport, -1, -1, FNUP_FAMILY_SYSTEM);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else if (nLocIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerImport, nDbIdx, -1, FNUP_FAMILY_DATABASE);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ServerImport, nLocIdx, -1, FNUP_FAMILY_LOCATION);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}

	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathServerExport(int nDbIdx, int nLocIdx)
{
	if (nDbIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerExport, -1, -1, FNUP_FAMILY_SYSTEM);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else if (nLocIdx == -1)
	{
		CFilenameUpdater FnUp(SysFiles::ServerExport, nDbIdx, -1, FNUP_FAMILY_DATABASE);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ServerExport, nLocIdx, -1, FNUP_FAMILY_LOCATION);
		m_strFilePathCommon = FnUp.GetFilenameToUse();
	}

	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetDefaultServerExportPath(int nDbIdx, int nLocIdx)
{
	if ( nDbIdx == -1)
	{
		m_strFilePathCommon = "SysFiles";
		GetDataProgramPath(m_strFilePathCommon);
	}
	else if (nLocIdx == -1)
	{
		m_strFilePathCommon = dbDatabase.GetFolderPathData(nDbIdx);
	}
	else
	{
		m_strFilePathCommon = dbLocation.GetFolderPathData(nLocIdx);
	}

	m_strFilePathCommon += "\\servers.csv";

	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathTableGroup( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::TableGroups, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathTableGroupSet( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::TableGroupSets, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncCommon::GetFilePathTableName( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::TableNames, nLocIdx );
	m_strFilePathCommon = FnUp.GetFilenameToUse();
	return m_strFilePathCommon;
}

/**********************************************************************/

bool CDataManagerNonDbFuncCommon::OpenTimeAttendReportNames(int nType, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_TIMEATTEND_NAMES);
	CFilenameUpdater FnUp(SysFiles::TimeAttendReportNames);
	return OpenCSVArray(TimeAttendReportNames, info, FnUp.GetFilenameToUse(), m_nStatusTimeAttendReportNames, nType, FALSE);
}

bool CDataManagerNonDbFuncCommon::CloseTimeAttendReportNames(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_TIMEATTEND_NAMES);
	return CloseCSVArray(TimeAttendReportNames, info, m_nStatusTimeAttendReportNames);
}

bool CDataManagerNonDbFuncCommon::WriteTimeAttendReportNames(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_TIMEATTEND_NAMES);
	return WriteCSVArray(TimeAttendReportNames, info, m_nStatusTimeAttendReportNames);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CDataManagerNonDbFuncCommon::CloseFilesCommon( CWordArray& arrayFiles )
{
	CDataManagerInfo info;
	for ( int n = 0; n < arrayFiles.GetSize(); n++ )
	{
		switch( arrayFiles.GetAt(n) )
		{
		case NODE_TABLE_NAME:			CloseTableNames( info );			break;
		case NODE_SERVER:				CloseServer( info );				break;
		case NODE_SERVERLOGON:			CloseServerLogon(info);				break;
		case NODE_TIMEATTEND_NAMES:		CloseTimeAttendReportNames(info);	break;
		}
	}
}

/**********************************************************************/
