#pragma once
/**********************************************************************/

class CSuper
{
public:
	CSuper();

public:
	void SetPosTraySocketFlag( bool b ){ m_bPosTraySocket = b; }
	bool GetPosTraySocketFlag(){ return m_bPosTraySocket; }

public:
	void Initialise();

public:
	void SetSpecialReportFilename( const char* sz ){ m_strSpecialReportFilename = sz; }
	
public:
	const char* SchedLogOld()			{ return m_strSchedLogOld; }
	const char* SchedLogAll()			{ return m_strSchedLogAll; }
	const char* EmailLogAll()			{ return m_strEmailLogAll; }
	const char* EmailLogOld()			{ return m_strEmailLogOld; }
	const char* AutoDeleteCustLogAll()	{ return m_strAutoDeleteCustLogAll; }
	const char* AutoDeleteCustLogOld()	{ return m_strAutoDeleteCustLogOld; }
	const char* IDraughtLogOld()		{ return m_strIDraughtLogOld; }
	const char* IDraughtLogAll()		{ return m_strIDraughtLogAll; }
	const char* FNBLogOld()				{ return m_strFNBLogOld; }
	const char* FNBLogAll()				{ return m_strFNBLogAll; }
	const char* ChartwellsLogOld()		{ return m_strChartwellsLogOld; }
	const char* ChartwellsLogAll()		{ return m_strChartwellsLogAll; }
	const char* S4LabourLogOld()		{ return m_strS4LabourLogOld; }
	const char* S4LabourLogAll()		{ return m_strS4LabourLogAll; }
	const char* FTPLogOld()				{ return m_strFTPLogOld; }
	const char* FTPLogAll()				{ return m_strFTPLogAll; }
	const char* SageLogOld()			{ return m_strSageLogOld; }
	const char* SageLogAll()			{ return m_strSageLogAll; }
	const char* PosTrayLogActionOld()	{ return m_strPosTrayLogActionOld; }
	const char* PosTrayLogActionAll()	{ return m_strPosTrayLogActionAll; }
	const char* PosTrayLogExportOld()	{ return m_strPosTrayLogExportOld; }
	const char* PosTrayLogExportAll()	{ return m_strPosTrayLogExportAll; }
	const char* ReportFilename();	
	const char* HTMLFilename()			{ return m_strHTMLFilename; }
	const char* UserLog()				{ return m_strUserLog; }
	const char* UserLogOld()			{ return m_strUserLogOld; }
	const char* EmailLog()				{ return m_strEmailLog; }
	const char* OrderLog()				{ return m_strOrderLog; }
	const char* BackupLogToRead()		{ return m_strBackupLogToRead; }
	const char* BackupLogToWrite()		{ return m_strBackupLogToWrite; }
	const char* BackupFolderToWrite()	{ return m_strBackupFolderToWrite; }
	const char* BackupLogParams()		{ return m_strBackupLogParams; }
	const char* FileWriteLogNew()		{ return m_strFileWriteLogNew; }
	const char* FileWriteLogOld()		{ return m_strFileWriteLogOld; }
	const char* WebAPICallLogNew()		{ return m_strWebAPICallLogNew; }
	const char* WebAPICallLogOld()		{ return m_strWebAPICallLogOld; }

	const char* PluStart()				{ return m_strPluStart; }
	const char* PluEnd()				{ return m_strPluEnd; }
	const char* BarcodeEnd()			{ return m_strBarcodeEnd; }
	const char* StockStart()			{ return m_strStockStart; }
	const char* StockEnd()				{ return m_strStockEnd; }
	
public:
	int MaxEcrs()						{ return 16; }
	int MaxGroupLen()					{ return 2; }
	int MaxKPLen()						{ return 1; }
	int MaxCostLen()					{ return 9; }
	int MaxPriceLen()					{ return 7; }
	int MaxPointsLen()					{ return 5; }
	int MaxStockLen()					{ return 16; }
	int MaxPasswordLen()				{ return 32; }
	int MaxUsernameLen()				{ return 32; }
	int MaxPluLen()						{ return 14; }
	int MaxBarcodeLen()					{ return 15; }
	long MaxLogLocFilesize();
	long MaxLogSysFilesize();
	__int64 MaxPluInt()					{ return 99999999999999; }
	__int64 MaxBarcodeInt()				{ return 999999999999999; }
	
private:
	CString m_strSchedLogOld;
	CString m_strSchedLogAll;
	CString m_strEmailLogOld;
	CString m_strEmailLogAll;
	CString m_strAutoDeleteCustLogOld;
	CString m_strAutoDeleteCustLogAll;
	CString m_strExportLogOld;
	CString m_strExportLogAll;
	CString m_strIDraughtLogOld;
	CString m_strIDraughtLogAll;
	CString m_strFNBLogOld;
	CString m_strFNBLogAll;
	CString m_strChartwellsLogOld;
	CString m_strChartwellsLogAll;
	CString m_strS4LabourLogOld;
	CString m_strS4LabourLogAll;
	CString m_strFTPLogOld;
	CString m_strFTPLogAll;
	CString m_strSageLogOld;
	CString m_strSageLogAll;
	CString m_strPosTrayLogActionOld;
	CString m_strPosTrayLogActionAll;
	CString m_strPosTrayLogExportOld;
	CString m_strPosTrayLogExportAll;
	CString m_strReportFilename;
	CString m_strSpecialReportFilename;
	CString m_strHTMLFilename;
	CString m_strUserLog;
	CString m_strUserLogOld;
	CString m_strEmailLog;
	CString m_strOrderLog;
	CString m_strBackupLogToRead;
	CString m_strBackupLogToWrite;
	CString m_strBackupFolderToWrite;
	CString m_strBackupLogParams;
	CString m_strFileWriteLogNew;
	CString m_strFileWriteLogOld;
	CString m_strWebAPICallLogNew;
	CString m_strWebAPICallLogOld;

private:
	CString m_strPluStart;
	CString m_strPluEnd;
	CString m_strBarcodeEnd;
	CString m_strStockStart;
	CString m_strStockEnd;

private:
	bool m_bPosTraySocket;
};

/**********************************************************************/
extern CSuper Super;
/**********************************************************************/

          