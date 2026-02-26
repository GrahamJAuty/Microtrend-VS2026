/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
/**********************************************************************/
#include "CommsExportReason.h"
/**********************************************************************/

CCommsExportReason::CCommsExportReason( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
	m_nParam1 = JobInfo.GetParam1();
}

/**********************************************************************/

int CCommsExportReason::PrepareFile()
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

	int nResult = COMMS_ERROR_NODATA;

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
		nResult = PrepareSPOSFile( nDbIdx );
	else
		nResult = COMMS_ERROR_CREATEFILE;
	
	return nResult;
}

/**********************************************************************/

int CCommsExportReason::PrepareSPOSFile(int nDbIdx)
{
	CSSFile fileReason;
	m_strSourceFilename = "Reason.sql";
	if (fileReason.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	bool bWithClear = ((m_nParam1 & 0x10000) != 0);
	bool bReasonTypesInUse[REASONTEXT_TYPES];

	for (int n = 0; n < REASONTEXT_TYPES; n++)
	{
		bReasonTypesInUse[n] = FALSE;
	}

	CSQLOut SQLOutBeginEnd(fileReason, "", "");
	SQLOutBeginEnd.WriteFileBegin();

	{
		CCSV csvHeader;
		csvHeader.Add("[FieldName]");
		csvHeader.Add("[FieldIndex]");
		csvHeader.Add("[iValue]");
		csvHeader.Add("[fValue]");
		csvHeader.Add("[sValue]");

		CSQLOut SQLOut(fileReason, "SysValues", csvHeader.GetLine());

		for (int nReasonType = 0; nReasonType < REASONTEXT_TYPES; nReasonType++)
		{
			if ((m_nParam1 & (1 << nReasonType)) == 0)
			{
				continue;
			}

			CDataManagerInfo infoDM;
			DataManagerNonDb.OpenCCReasonLocation(m_nLocIdx, DB_READONLY, nReasonType, infoDM);
			DataManagerNonDb.CCReasonLocation.LoadParentReasonsDatabase(nDbIdx);
			DataManagerNonDb.CCReasonLocation.LoadParentReasonsSystem();

			CReportConsolidationArray<CSortedIntItem> arrayReasonTypesInUse;

			CString strReasonType = "";
			strReasonType += "REASON.";
			strReasonType += CCascadingReasonCSVArray::GetSQLLabelByReasonType(nReasonType);

			if (TRUE == bWithClear)
			{
				CString strDelete = "";
				strDelete.Format("DELETE FROM SysValues WHERE FieldName='%s'",
					(const char*)strReasonType);

				fileReason.WriteLine(strDelete);
				fileReason.WriteLine("--->GO");
			}

			for (int nReasonIdx = 0; nReasonIdx < DataManagerNonDb.CCReasonLocation.GetSize(); nReasonIdx++)
			{
				CCascadingReasonCSVRecord Reason;
				DataManagerNonDb.CCReasonLocation.GetAt(nReasonIdx, Reason);

				CString strText = "";
				bool bManualEntry = FALSE;
				if (Reason.GetLocalReasonFlag() == FALSE)
				{
					strText = Reason.GetParentReasonText();
					bManualEntry = Reason.GetParentManualEntryFlag();
				}
				else
				{
					strText = Reason.GetLocalReasonText();
					bManualEntry = Reason.GetLocalManualEntryFlag();
				}

				if (FALSE == bWithClear)
				{
					CString strDelete = "";
					strDelete.Format("DELETE FROM SysValues WHERE FieldName='%s' AND FieldIndex=%d",
						(const char*)strReasonType,
						Reason.GetReasonNo());

					fileReason.WriteLine(strDelete);
					fileReason.WriteLine("--->GO");
				}

				SQLOut.Add(strReasonType);
				SQLOut.Add(Reason.GetReasonNo());
				SQLOut.Add(bManualEntry ? -1 : 0);
				SQLOut.Add(0.0, 3);
				SQLOut.Add(strText);
				SQLOut.WriteInsert();
				TallyDownloadAccept();

				bReasonTypesInUse[nReasonType] = TRUE;
			}

			DataManagerNonDb.CloseCCReasonLocation(infoDM);
		}
	}

	if ((dbLocation.GetTerminalCount(m_nLocIdx) > 0) && (DealerFlags.GetReasonTicksNoDownloadFlag() == FALSE))
	{
		CArray<int, int> arrayReasonType;

		for (int nReasonType = 0; nReasonType < REASONTEXT_TYPES; nReasonType++)
		{
			if ((m_nParam1 & (1 << nReasonType)) != 0)
			{
				if ((TRUE == bWithClear) || (TRUE == bReasonTypesInUse[nReasonType]))
				{
					arrayReasonType.Add(nReasonType);
				}
			}
		}

		if (arrayReasonType.GetSize() > 0)
		{
			CCSV csvHeader;
			csvHeader.Add("[TillNo]");

			for (int n = 0; n < arrayReasonType.GetSize(); n++)
			{
				int nReasonType = arrayReasonType.GetAt(n);

				csvHeader.Add(CCascadingReasonCSVArray::GetCfgTillLabelByReasonType(nReasonType, FALSE));

				if (REASONTEXT_TYPE_ITEMVOID == nReasonType)
				{
					csvHeader.Add(CCascadingReasonCSVArray::GetCfgTillLabelByReasonType(nReasonType, TRUE));
				}
			}

			for (int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount(m_nLocIdx); nTermIdx++)
			{
				CSQLOut SQLOut(fileReason, "CfgTills", csvHeader.GetLine());
				SQLOut.AddCondition(dbLocation.GetTNo(m_nLocIdx, nTermIdx));

				for (int n = 0; n < arrayReasonType.GetSize(); n++)
				{
					int nReasonType = arrayReasonType.GetAt(n);
					
					SQLOut.Add(bReasonTypesInUse[nReasonType]);

					if (REASONTEXT_TYPE_ITEMVOID == nReasonType)
					{
						SQLOut.Add(FALSE);
					}
				}

				SQLOut.WriteModify();
			}
		}
	}

	SQLOutBeginEnd.WriteFileEnd();

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

