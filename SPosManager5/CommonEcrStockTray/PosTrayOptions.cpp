/**********************************************************************/
#include "MaxLengths.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayOptions.h"
/**********************************************************************/

CPosTrayOptions::CPosTrayOptions()
{
	//GENRAL TAB
	m_RealTime.Create( "PosTrayRealTime", FALSE );
	m_ProcessTran.Create("PosTrayProcessTran", FALSE);
	m_NoHibernate.Create( "DisableHibernate", FALSE );
	m_CommsSpeed.Create( "RealTimeCommsSpeed", 1, 10, 1 );

	m_ReportTaskEpos.Create( "ReportTaskEpos", 0, 2, 0 );
	m_ReportTaskHistory.Create( "ReportTaskHistory", 0, 2, 0 );
	m_ReportTaskStockLevel.Create( "ReportTaskStockLevel", FALSE );
	m_ReportTaskTimeAttend.Create("ReportTaskTimeAttend", FALSE);
	m_ManageBackup.Create("ManageBackup", FALSE);
	m_ExportTaskSales.Create( "ExportTaskSales", 0, 2, 0 );
	m_ExportTaskPayment.Create( "ExportTaskPayment", 0, 2, 0 );
	m_ExportTaskVoid.Create( "ExportTaskVoid", FALSE );
	m_ExportTaskDiscount.Create( "ExportTaskDiscount", FALSE );
	m_FNBTask.Create( "FNBTask", 0, 2, 0 );
	m_ChartwellsTask.Create("ExportChartwells", FALSE);
	m_S4LabourTask.Create( "S4LabourTask", FALSE );

	m_EditFilenames.Create( "EditFilenames", FALSE );
	m_EditFieldnames.Create( "EditFieldnames", FALSE );
	m_ManualExport.Create( "ManualExport", FALSE );
	m_BusinessDate.Create( "BusinessDateMode", FALSE );
	m_SkipEmpty.Create("SkipEmptyReports", FALSE);
	m_SkipNoChange.Create("SkipUnchangedReports", FALSE);
	m_ExportDateType.Create( "ExportDateType", 1, 4, 1 );

	//FNB OPTIONS
	m_FNBDelayTime.Create( "FNBDelayTime", 1, 6, 1 );

	//CHARTWELLS EXPORT HOUR
	m_ChartwellsNextDay.Create("ChartwellsNextDay", FALSE);
	m_ChartwellsExportTime.Create("ChartwellsExportTime", 0, 1439, 0);

	//S4 LABOUR OPTIONS
	m_S4LabourExportHour.Create( "S4LabourExportHour", 0, 23, 5 );
	m_S4LabourFTPServer.Create( "S4LabourFTPServer", MAX_LENGTH_FTP_SERVER, "" );
	m_S4LabourFTPUserName.Create( "S4LabourFTPUserName", MAX_LENGTH_FTP_USERNAME, "" );
	m_S4LabourFTPPassword.Create( "S4LabourFTPPassword", MAX_LENGTH_FTP_PASSWORD, "" );

	//LIPA OPTIONS
	m_LIPAExportHour.Create( "LIPAExportHour", 0, 23, 5 );
	m_LIPAExportFolder.Create( "LIPAExportFolder", 200, "" );
	m_LIPAExportLocation.Create( "LIPAExportLocation", 0, 999999, 0 );
	
	//FTP OPTIONS
	m_FTPServer.Create( "FTPServer", MAX_LENGTH_FTP_SERVER, "" );
	m_FTPUserName.Create( "FTPUserName", MAX_LENGTH_FTP_USERNAME, "" );
	m_FTPPassword.Create( "FTPPassword", MAX_LENGTH_FTP_PASSWORD, "" );
	m_FTPUnixMode.Create( "FTPUnixMode", FALSE );
	
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPosTrayOptions::CopyFrom( CPosTrayOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPosTrayOptions::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	//GENERAL TAB
	iniFile.ReadBool(m_RealTime);
	iniFile.ReadBool(m_ProcessTran);
	iniFile.ReadBool(m_NoHibernate);
	iniFile.ReadInt(m_CommsSpeed);

	iniFile.ReadInt(m_ReportTaskEpos);
	iniFile.ReadInt(m_ReportTaskHistory);
	iniFile.ReadBool(m_ReportTaskStockLevel);
	iniFile.ReadBool(m_ReportTaskTimeAttend);
	iniFile.ReadBool(m_ManageBackup);
	iniFile.ReadInt(m_ExportTaskSales);
	iniFile.ReadInt(m_ExportTaskPayment);
	iniFile.ReadBool(m_ExportTaskVoid);
	iniFile.ReadBool(m_ExportTaskDiscount);
	iniFile.ReadInt(m_FNBTask);
	iniFile.ReadBool(m_ChartwellsTask);
	iniFile.ReadBool(m_S4LabourTask);

	if (m_ReportTaskEpos.GetValue() == 0)
	{
		CIniFileBool legacy;
		legacy.Create("EditEmailEPOS", FALSE);
		iniFile.ReadBool(legacy);
		SetReportTaskEposFlag(legacy.GetValue());
	}

	if (m_ReportTaskHistory.GetValue() == 0)
	{
		CIniFileBool legacy;
		legacy.Create("EditEmailHistory", FALSE);
		iniFile.ReadBool(legacy);
		SetReportTaskHistoryFlag(legacy.GetValue());
	}

	if (m_ExportTaskSales.GetValue() == 0)
	{
		CIniFileBool legacy;
		legacy.Create("EditEmailCSVSales", FALSE);
		iniFile.ReadBool(legacy);
		SetExportTaskSalesFlag(legacy.GetValue());
	}

	if (m_ExportTaskPayment.GetValue() == 0)
	{
		CIniFileBool legacy;
		legacy.Create("EditEmailCSVPayment", FALSE);
		iniFile.ReadBool(legacy);
		SetExportTaskPaymentFlag(legacy.GetValue());
	}

	if (m_FNBTask.GetValue() == 0)
	{
		CIniFileBool legacy;
		legacy.Create("ExportFNB", FALSE);
		iniFile.ReadBool(legacy);
		SetFNBTaskFlag(legacy.GetValue());
	}

	iniFile.ReadBool(m_EditFilenames);
	iniFile.ReadBool(m_EditFieldnames);
	iniFile.ReadBool(m_ManualExport);
	iniFile.ReadBool(m_BusinessDate);
	iniFile.ReadBool(m_SkipEmpty);
	iniFile.ReadBool(m_SkipNoChange);
	iniFile.ReadInt(m_ExportDateType);

	//FNB TAB
	iniFile.ReadInt(m_FNBDelayTime);

	//CHARTWELLS TAB
	iniFile.ReadBool(m_ChartwellsNextDay);
	iniFile.ReadInt(m_ChartwellsExportTime);

	//S4 LABOUR TAB
	iniFile.ReadInt(m_S4LabourExportHour);
	iniFile.ReadString(m_S4LabourFTPServer);
	iniFile.ReadString(m_S4LabourFTPUserName);
	iniFile.ReadString(m_S4LabourFTPPassword);

	//LIPA TAB
	iniFile.ReadInt(m_LIPAExportHour);
	iniFile.ReadString(m_LIPAExportFolder);
	iniFile.ReadInt(m_LIPAExportLocation);

	//FTP TAB
	iniFile.ReadString(m_FTPServer);
	iniFile.ReadString(m_FTPUserName);
	iniFile.ReadString(m_FTPPassword);
	iniFile.ReadBool(m_FTPUnixMode);

#ifdef STOCKMAN_SYSTEM
	m_ReportTaskStockLevel.SetValue(FALSE);
#endif
}

/**********************************************************************/

void CPosTrayOptions::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	//DISPLAY TAB 
	iniFile.WriteBool(m_RealTime);
	iniFile.WriteBool(m_ProcessTran);
	iniFile.WriteBool(m_NoHibernate);
	iniFile.WriteInt(m_CommsSpeed);

	iniFile.WriteInt(m_ReportTaskEpos);
	iniFile.WriteInt(m_ReportTaskHistory);

#ifndef STOCKMAN_SYSTEM
	iniFile.WriteBool(m_ReportTaskStockLevel);
#endif

	iniFile.WriteBool(m_ReportTaskTimeAttend);
	iniFile.WriteBool(m_ManageBackup);

	iniFile.WriteInt(m_ExportTaskSales);
	iniFile.WriteInt(m_ExportTaskPayment);
	iniFile.WriteBool(m_ExportTaskVoid);
	iniFile.WriteBool(m_ExportTaskDiscount);
	iniFile.WriteInt(m_FNBTask);
	iniFile.WriteBool(m_ChartwellsTask);
	iniFile.WriteBool(m_S4LabourTask);

	iniFile.WriteBool(m_EditFilenames);
	iniFile.WriteBool(m_EditFieldnames);
	iniFile.WriteBool(m_ManualExport);
	iniFile.WriteBool(m_BusinessDate);
	iniFile.WriteBool(m_SkipEmpty);
	iniFile.WriteBool(m_SkipNoChange);
	iniFile.WriteInt(m_ExportDateType);

	//FNB TAB
	iniFile.WriteInt(m_FNBDelayTime);

	//CHARTWELLS TAB
	iniFile.WriteBool(m_ChartwellsNextDay);
	iniFile.WriteInt(m_ChartwellsExportTime);

	//S4 LABOUR TAB
	iniFile.WriteInt(m_S4LabourExportHour);
	iniFile.WriteString(m_S4LabourFTPServer);
	iniFile.WriteString(m_S4LabourFTPUserName);
	iniFile.WriteString(m_S4LabourFTPPassword);

	//LIPA TAB
	iniFile.WriteInt(m_LIPAExportHour);
	iniFile.WriteString(m_LIPAExportFolder);
	iniFile.WriteInt(m_LIPAExportLocation);

	//FTP TAB
	iniFile.WriteString(m_FTPServer);
	iniFile.WriteString(m_FTPUserName);
	iniFile.WriteString(m_FTPPassword);
	iniFile.WriteBool(m_FTPUnixMode);
}

/**********************************************************************/

void CPosTrayOptions::Reset()
{
	//GENERAL TAB
	m_RealTime.Reset();
	m_ProcessTran.Reset();
	m_NoHibernate.Reset();
	m_CommsSpeed.Reset();
	m_ReportTaskEpos.Reset();
	m_ReportTaskHistory.Reset();
	m_ReportTaskStockLevel.Reset();
	m_ReportTaskTimeAttend.Reset();
	m_ManageBackup.Reset();
	m_ExportTaskSales.Reset();
	m_ExportTaskPayment.Reset();
	m_ExportTaskVoid.Reset();
	m_ExportTaskDiscount.Reset();
	m_FNBTask.Reset();
	m_ChartwellsTask.Reset();
	m_S4LabourTask.Reset();
	m_EditFilenames.Reset();
	m_EditFieldnames.Reset();
	m_ManualExport.Reset();
	m_BusinessDate.Reset();
	m_SkipEmpty.Reset();
	m_SkipNoChange.Reset();
	m_ExportDateType.Reset();

	//FNB TAB
	m_FNBDelayTime.Reset();

	//CHARTWELLS TAB
	m_ChartwellsNextDay.Reset();
	m_ChartwellsExportTime.Reset();

	//FNB TAB
	m_S4LabourExportHour.Reset();
	m_S4LabourFTPServer.Reset();
	m_S4LabourFTPUserName.Reset();
	m_S4LabourFTPPassword.Reset();

	//LIPA TAB
	m_LIPAExportHour.Reset();
	m_LIPAExportFolder.Reset();

	//FTP TAB
	m_FTPServer.Reset();
	m_FTPUserName.Reset();
	m_FTPPassword.Reset();
	m_FTPUnixMode.Reset();

	//TEMPORARY FLAGS, NOT SAVED TO FILE
	m_bTemporaryStartup = FALSE;
}

/**********************************************************************/

bool CPosTrayOptions::Read()
{
	CFilenameUpdater FnUp( SysFiles::PosTrayOptions );
	CString strFilename = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CPosTrayOptions::Write()
{
	bool bResult = FALSE;

	CEnhancedIniFile iniFile;
	PrepareIniFile(iniFile);

	CFilenameUpdater FnUp(SysFiles::PosTrayOptions);
	bResult = iniFile.Write(FnUp.GetFilenameToUse());

	DataManager.UnlockAllUsers();

	return bResult;
}

/**********************************************************************/

bool CPosTrayOptions::AreExportsEnabled()
{
	return ( GetExportTaskSalesFlag() || GetExportTaskPaymentFlag() || GetExportTaskVoidFlag() || GetExportTaskDiscountFlag() );
}

/**********************************************************************/

bool CPosTrayOptions::AreTasksEnabled()
{
	bool bResult = (GetReportTaskEposFlag() || GetReportTaskHistoryFlag() || GetReportTaskStockLevelFlag() || GetPosTrayProcessTranFlag() || AreExportsEnabled());

	if (TRUE == SysInfo.IsPosTrayTimeAndAttendanceSystem())
	{
		bResult |= GetReportTaskTimeAttendFlag();
	}

	return bResult;
}

/**********************************************************************/

bool CPosTrayOptions::GotFTPSettings()
{
	{
		CString strServer = GetFTPServer();
		::TrimSpaces( strServer, FALSE );
		if ( strServer == "" )
			return FALSE;
	}

	{
		CString strUserName = GetFTPUserName();
		::TrimSpaces( strUserName, FALSE );
		if ( strUserName == "" )
			return FALSE;
	}

	{
		CString strPassword = GetFTPPassword();
		::TrimSpaces( strPassword, FALSE );
		if ( strPassword == "" )
			return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CPosTrayOptions::ResetRunNowList()
{
	m_arrayRunNow.RemoveAll();
}

/**********************************************************************/

void CPosTrayOptions::AddToRunNowList(int nTaskType, int nTaskNo)
{
	CSortedIntAndInt item;
	item.m_nInt1 = nTaskType;
	item.m_nInt2 = nTaskNo;
	m_arrayRunNow.Consolidate(item);
}

/**********************************************************************/

void CPosTrayOptions::ProcessRunNowList()
{
	int nSize = m_arrayRunNow.GetSize();

	if (nSize > 0)
	{
		CString strFolder = "PosTray";
		::GetSyssetProgramPath(strFolder);
		MakeSubdirectory(strFolder);
		strFolder += "\\RunTaskNow";
		MakeSubdirectory(strFolder);

		for (int n = 0; n < nSize ; n++)
		{
			CSortedIntAndInt item;
			m_arrayRunNow.GetAt(n, item);

			CString strFilename = "";
			strFilename.Format("\\T%3.3dN%3.3d.dat",
				item.m_nInt1,
				item.m_nInt2);

			CSSFile fileToken;
			fileToken.Open(strFolder + strFilename, "wb");
			fileToken.WriteLine("SPOS");
			fileToken.Close();
		}
	}

	ResetRunNowList();
}

/**********************************************************************/
