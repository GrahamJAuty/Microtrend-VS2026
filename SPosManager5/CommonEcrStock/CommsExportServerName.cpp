/**********************************************************************/
#include "CommsErrors.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ServerCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportServerName.h"
/**********************************************************************/

CCommsExportServerName::CCommsExportServerName( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportServerName::PrepareFile()
{
	if (m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize())
	{
		return COMMS_ERROR_BADNETIDX;
	}

	switch (dbNetwork.GetConnectionType(m_nNetworkIdx))
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	m_strSourceFolder = dbNetwork.GetFolderPathData(m_nNetworkIdx);
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder(m_nLocIdx, m_nTerminalNo, m_bImmediate);

	int nResult = COMMS_ERROR_NODATA;

	CDataManagerInfo info;

	if (FALSE == m_bFromMemory)
	{
		int nServerDbIdx = -1;
		int nServerLocIdx = -1;

		switch (EcrmanOptions.GetFeaturesServerSettingsType())
		{
		case SERVER_DB:
		case SERVER_LOC:
		{
			int nDbNo = dbLocation.GetDbNo(m_nLocIdx);
			if (dbDatabase.FindDatabaseByNumber(nDbNo, nServerDbIdx) == FALSE)
			{
				return COMMS_ERROR_BADDBLINK;
			}
		}
		break;
		}

		switch (EcrmanOptions.GetFeaturesServerSettingsType())
		{
		case SERVER_LOC:
			nServerLocIdx = m_nLocIdx;
			break;
		}

		CDataManagerInfo info;
		DataManagerNonDb.OpenServer(DB_READONLY, nServerDbIdx, nServerLocIdx, info);
		
		if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
		{
			DataManagerNonDb.OpenServerLogon(DB_READONLY, nServerDbIdx, nServerLocIdx, info);
		}

		DataManagerNonDb.OpenServerRestriction(DB_READONLY, m_nLocIdx, info);
	}

	if (DataManagerNonDb.Server.GetSize() != 0)
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion(m_nNetworkIdx);

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			nResult = PrepareSPOSFile(nSPOSVersion);
		}
		else
		{
			nResult = COMMS_ERROR_CREATEFILE;
		}
	}

	if (FALSE == m_bFromMemory)
	{
		DataManagerNonDb.CloseServer(info);
		
		if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
		{
			DataManagerNonDb.CloseServerLogon(info);
		}
	
		DataManagerNonDb.CloseServerRestriction(info);
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportServerName::PrepareSPOSFile(int nSPOSVersion)
{
	CSSFile fileServer;
	m_strSourceFilename = "Servers.sql";

	if (fileServer.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CCSV csvHeader;
	csvHeader.Add("[SrvNo]");
	csvHeader.Add("[Name]");
	csvHeader.Add("[Auth]");
	csvHeader.Add("[Password]");
	csvHeader.Add("[Drawer]");
	csvHeader.Add("[FlipView]");
	csvHeader.Add("[TrainingMode]");
	csvHeader.Add("[LogonMacro]");
	csvHeader.Add("[OpenTableMode]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1622) == TRUE)
	{
		csvHeader.Add("[TradingColourScheme]");
	}

	CSQLOut SQLOut(fileServer, "Servers", csvHeader.GetLine());
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	
	int nSize = DataManagerNonDb.Server.GetSize();
	for (int nSrvrIdx = 0; nSrvrIdx < nSize; nSrvrIdx++)
	{
		StatusProgress.SetPos(nSrvrIdx, nSize);

		CServerCSVRecord Server;
		DataManagerNonDb.Server.GetAt(nSrvrIdx, Server);

		CMinimumSPOSVersion Version;
		Server.CheckMinimumSPOSVersion(Version);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_SERVER, nErrorType);
			continue;
		}

		SQLOut.Add(Server.GetServerNo());
		SQLOut.Add(Server.GetReceiptName());
		SQLOut.Add(Server.GetAuthority());
		SQLOut.Add(Server.GetPassword());
		SQLOut.Add(Server.GetDrawer());
		SQLOut.Add(Server.GetFlipViewFlag());
		SQLOut.Add(Server.GetTrainingModeFlag());
		SQLOut.Add(Server.GetLogonMacro());
		SQLOut.Add(Server.GetOpenTableMode());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1622) == TRUE)
		{
			SQLOut.Add(Server.GetTradingColourScheme());
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
	{
		csvHeader.RemoveAll();
		csvHeader.Add("[KeyValue]");
		csvHeader.Add("[SrvNo]");
		
		CSQLOut SQLOut(fileServer, "CLCKeys", csvHeader.GetLine());
		SQLOut.WriteDelete();

		int nSize = DataManagerNonDb.ServerLogon.GetSize();
		for (int nLogonIdx = 0; nLogonIdx < nSize; nLogonIdx++)
		{
			StatusProgress.SetPos(nLogonIdx, nSize);

			CServerLogonCSVRecord Logon;
			DataManagerNonDb.ServerLogon.GetAt(nLogonIdx, Logon);

			CString strKeyValue = "";
			strKeyValue.Format("0x%I64X", Logon.GetLogonNo());
			
			if ((strKeyValue.GetLength() % 2) == 1)
			{
				strKeyValue.Insert(2, "0");
			}

			SQLOut.AddHexString(strKeyValue);
			SQLOut.Add(Logon.GetServerNo());
			SQLOut.WriteInsert();
			TallyDownloadAccept();
		}
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2067) == TRUE)
	{
		if (EcrmanOptions.GetFeaturesServerRestrictionFlag() == TRUE)
		{
			for (int nSrvrIdx = 0; nSrvrIdx < nSize; nSrvrIdx++)
			{
				StatusProgress.SetPos(nSrvrIdx, nSize);

				CServerCSVRecord ServerRecord;
				DataManagerNonDb.Server.GetAt(nSrvrIdx, ServerRecord);

				CString strDelete;
				strDelete.Format("DELETE FROM SysValues WHERE FieldName='ServerTills%5.5d'", ServerRecord.GetServerNo());
				fileServer.WriteLine(strDelete);
				fileServer.WriteLine("--->GO");

				CCSV csvHeader2;
				csvHeader2.Add("[FieldName]");
				csvHeader2.Add("[FieldIndex]");
				csvHeader2.Add("[iValue]");
				csvHeader2.Add("[fValue]");
				csvHeader2.Add("[sValue]");

				CServerRestrictionCSVRecord TerminalRecord;
				DataManagerNonDb.ServerRestriction.GetRestrictions(ServerRecord, TerminalRecord);

				CCSV csvExclude(TerminalRecord.GetRestrictions(), ';');

				if (csvExclude.GetSize() != 0)
				{
					for (int n = 0; n < csvExclude.GetSize(); n++)
					{
						CString strFieldName;
						strFieldName.Format("ServerTills%5.5d", ServerRecord.GetServerNo());

						CSQLOut SQLOut2(fileServer, "SysValues", csvHeader2.GetLine());
						SQLOut2.Add(strFieldName);
						SQLOut2.Add(csvExclude.GetInt(n));
						SQLOut2.Add(0);
						SQLOut2.Add(0.0, 1);
						SQLOut2.Add("");
						SQLOut2.WriteInsert();
						TallyDownloadAccept();
					}
				}
			}
		}
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileServer.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
