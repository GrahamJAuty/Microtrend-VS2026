/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "DepartmentCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportDept.h"
/**********************************************************************/

CCommsExportDept::CCommsExportDept( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportDept::PrepareFile()
{
	if ( m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize() )
		return COMMS_ERROR_BADNETIDX;
		
	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return COMMS_ERROR_BADLOCIDX;

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenDepartment( info );
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( DataManager.Department.GetSize() != 0 )
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );

		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile( nSPOSVersion );
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportDept::PrepareSPOSFile(int nSPOSVersion)
{
	CSSFile fileDept;
	m_strSourceFilename = "Depts.sql";

	if (fileDept.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CCSV csvHeader;
	csvHeader.Add("[DptNo]");
	csvHeader.Add("[GrpNo]");
	csvHeader.Add("[Name]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1734) == TRUE)
	{
		csvHeader.Add("[TicketsMode]");
	}

	csvHeader.Add("[Tickets]");
	csvHeader.Add("[QtyDP]");
	csvHeader.Add("[ValNeg]");
	csvHeader.Add("[NonTurnover]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_827) == TRUE)
	{
		csvHeader.Add("[SpecimenLearnPLU]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_714) == TRUE)
	{
		csvHeader.Add("[ImageFilename]");
	}

	CSQLOut SQLOut(fileDept, "Depts", csvHeader.GetLine());
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.Department.GetSize();
	for (int nDeptIdx = 0; nDeptIdx < nSize; nDeptIdx++)
	{
		StatusProgress.SetPos(nDeptIdx, nSize);

		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt(nDeptIdx, DeptRecord);

		SQLOut.Add(DeptRecord.GetDeptNo());
		SQLOut.Add(DeptRecord.GetEposGroup());
		SQLOut.Add(DeptRecord.GetEposText());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1734) == TRUE)
		{
			SQLOut.Add(DeptRecord.GetTicketsMode());
		}

		SQLOut.Add(DeptRecord.GetTickets());
		SQLOut.Add(DeptRecord.GetDPQtyFlag());
		SQLOut.Add(DeptRecord.GetNegativeFlag());
		SQLOut.Add(DeptRecord.GetNonAddFlag());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_827) == TRUE)
		{
			SQLOut.Add(DeptRecord.GetSpecimenLearnPlu());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_714) == TRUE)
		{
			SQLOut.Add(DeptRecord.GetImageFilename());
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileDept.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
