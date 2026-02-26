#pragma once
/**********************************************************************/
#include "CustomerCSVArray.h"
#include "CustomerNameTable.h"
#include "DefTextEmailAddress.h"
#include "DepositNameCSVArray.h"
#include "EODDateTimeFilter.h"
#include "PosTrayExportFieldNames.h"
#include "SalesHistoryCSVArrayPlu.h"
#include "ServerCSVArray.h"
#include "ServerLogonCSVArray.h"
#include "ServerSelectionList.h"
#include "SessionCSVArray.h"
#include "SessionDateTimeFilter.h"
#include "TableGroupCSVArray.h"
#include "TableGroupSetCSVArray.h"
#include "TableNameCSVArray.h"
#include "TimeAttendReportNameCSVArray.h"
#include "TimeSliceCSVArray.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/

class CDataManagerNonDbFuncCommon : virtual public CDataManagerBase
{
public:
	CDataManagerNonDbFuncCommon();

public:
	void CloseFilesCommon( CWordArray& arrayFiles );

public:
	bool OpenCustomerAll( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenCustomerAccount( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenCustomerOneOff( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenDepositNames( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenEmailAddress( int nType, CDataManagerInfo& info );
	bool OpenPosTrayExportFieldNames( int nType, CDataManagerInfo& info );
	bool OpenReportSession( int nType, CDataManagerInfo& info );
	bool OpenSalesHistoryPlu( int nType, CDataManagerInfo& info );
	bool OpenServer( int nType, int nDbIdx, int nLocIdx, CDataManagerInfo& info );
	bool OpenServerLogon(int nType, int nDbIdx, int nLocIdx, CDataManagerInfo& info);
	bool OpenSystemDepartment( int nType, CDataManagerInfo& info );
	bool OpenSystemDepartmentSet( int nType, CDataManagerInfo& info );
	bool OpenSystemPluFilterItems( int nType, CDataManagerInfo& info );
	bool OpenSystemPluFilterNames( int nType, CDataManagerInfo& info );
	bool OpenTableGroups( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenTableGroupSets( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenTableNames( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenTimeAttendReportNames(int nType, CDataManagerInfo& info);
	bool OpenTimeSliceDetail( int nFileNo, int nType, CDataManagerInfo& info );
	bool OpenTimeSliceList( int nType, CDataManagerInfo& info );

public:
	bool OpenEmailAddressBuffer( CDataManagerInfo& info );
	bool OpenSalesHistoryPluBuffer( CDataManagerInfo& info );
	
public:
	bool CloseCustomerAll( CDataManagerInfo& info );
	bool CloseCustomerAccount( CDataManagerInfo& info );
	bool CloseCustomerOneOff( CDataManagerInfo& info );
	bool CloseDepositNames( CDataManagerInfo& info );
	bool CloseEmailAddress( CDataManagerInfo& info );
	bool ClosePosTrayExportFieldNames( CDataManagerInfo& info );
	bool CloseReportSession( CDataManagerInfo& info );
	bool CloseSalesHistoryPlu( CDataManagerInfo& info );
	bool CloseServer( CDataManagerInfo& info );
	bool CloseServerLogon(CDataManagerInfo& info);
	bool CloseSystemDepartment( CDataManagerInfo& info );
	bool CloseSystemDepartmentSet( CDataManagerInfo& info );
	bool CloseSystemPluFilterItems( CDataManagerInfo& info );
	bool CloseSystemPluFilterNames( CDataManagerInfo& info );
	bool CloseTableGroups( CDataManagerInfo& info );
	bool CloseTableGroupSets( CDataManagerInfo& info );
	bool CloseTableNames( CDataManagerInfo& info );
	bool CloseTimeAttendReportNames(CDataManagerInfo& info);
	bool CloseTimeSliceDetail( CDataManagerInfo& info );
	bool CloseTimeSliceList( CDataManagerInfo& info );

public:
	bool CloseEmailAddressBuffer( CDataManagerInfo& info );
	bool CloseSalesHistoryPluBuffer( CDataManagerInfo& info );
	
public:
	bool WriteCustomerAll( CDataManagerInfo& info );
	bool WriteCustomerAccount( CDataManagerInfo& info );
	bool WriteCustomerOneOff( CDataManagerInfo& info );
	bool WriteDepositNames( CDataManagerInfo& info );
	bool WriteEmailAddress( CDataManagerInfo& info );
	bool WritePosTrayExportFieldNames( CDataManagerInfo& info );
	bool WriteReportSession( CDataManagerInfo& info );
	bool WriteSalesHistoryPlu( CDataManagerInfo& info );
	bool WriteServer( CDataManagerInfo& info );
	bool WriteServerLogon( CDataManagerInfo& info);
	bool WriteSystemDepartment( CDataManagerInfo& info );
	bool WriteSystemDepartmentSet( CDataManagerInfo& info );
	bool WriteSystemPluFilterItems( CDataManagerInfo& info );
	bool WriteSystemPluFilterNames( CDataManagerInfo& info );
	bool WriteTableGroups( CDataManagerInfo& info );
	bool WriteTableGroupSets( CDataManagerInfo& info );
	bool WriteTableNames( CDataManagerInfo& info );
	bool WriteTimeAttendReportNames(CDataManagerInfo& info);
	bool WriteTimeSliceDetail( CDataManagerInfo& info );
	bool WriteTimeSliceList( CDataManagerInfo& info );

public:
	bool WriteEmailAddressBuffer( CDataManagerInfo& info );
	bool WriteSalesHistoryPluBuffer( CDataManagerInfo& info );
	
protected:
	const char* GetFilePathCustomerAccount( int nLocIdx );
	const char* GetFilePathCustomerOneOff( int nLocIdx );
	const char* GetFilePathDepositNames( int nLocIdx );
	const char* GetFilePathTableGroup( int nLocIdx );
	const char* GetFilePathTableGroupSet( int nLocIdx );
	const char* GetFilePathTableName( int nLocIdx );

public:
	const char* GetFilePathServer( int nDbIdx, int nLocIdx );
	const char* GetFilePathServerLogon( int nDbIdx, int nLocIdx);
	const char* GetFilePathServerCSVDef(int nDbIdx, int nLocIdx);
	const char* GetFilePathServerExport(int nDbIdx, int nLocIdx);
	const char* GetFilePathServerImport(int nDbIdx, int nLocIdx);
	const char* GetDefaultServerExportPath(int nDbIdx, int nLocIdx);

	
public:
	CCustomerCSVArray CustomerAccount;
	CCustomerCSVArray CustomerOneOff;
	CDefTextEmailAddress EmailAddress;
	CDepositNameCSVArray DepositNames;
	CPosTrayExportFieldNames PosTrayExportFieldNames;
	CSessionCSVArray ReportSession;
	CSalesHistoryCSVArrayPlu SalesHistoryPlu;
	CServerCSVArray Server;
	CServerLogonCSVArray ServerLogon;
	CDepartmentCSVArray SystemDepartment;
	CDepartmentSetCSVArray SystemDepartmentSet;
	COfferGroupEntriesCSVArray SystemPluFilterItems;
	COfferGroupsCSVArray SystemPluFilterNames;
	CTableGroupCSVArray TableGroups;
	CTableGroupSetCSVArray TableGroupSets;
	CTableNameCSVArray TableNames;
	CTimeAttendReportNameCSVArray TimeAttendReportNames;
	CTimeSliceCSVArray TimeSliceDetail;
	CTimeSliceMapCSVArray TimeSliceList;

public:
	CDefTextEmailAddress EmailAddressBuffer;
	CSalesHistoryCSVArrayPlu SalesHistoryPluBuffer;
	
public:
	CCustomerNameTable CustomerNameTable;
	CEODDateTimeFilterArray EODDateTimeFilterArray;
	CServerSelectionList ServerNameTable;
	CSessionDateTimeFilter SessionDateTimeFilter;
	
protected:
	int m_nStatusCustomerAccount;
	int m_nStatusCustomerOneOff;
	int m_nStatusDepositNames;
	int m_nStatusEmailAddress;
	int m_nStatusPosTrayExportFieldNames;
	int m_nStatusReportSession;
	int m_nStatusSalesHistoryPlu;
	int m_nStatusServer;
	int m_nStatusServerLogon;
	int m_nStatusSystemDepartment;
	int m_nStatusSystemDepartmentSet;
	int m_nStatusSystemPluFilterItems;
	int m_nStatusSystemPluFilterNames;
	int m_nStatusTableGroups;
	int m_nStatusTableGroupSets;
	int m_nStatusTableNames;
	int m_nStatusTimeAttendReportNames;
	int m_nStatusTimeSliceDetail;
	int m_nStatusTimeSliceList;

protected:
	int m_nStatusEmailAddressBuffer;
	int m_nStatusSalesHistoryPluBuffer;
	
private:
	CString m_strFilePathCommon;
};

/**********************************************************************/
