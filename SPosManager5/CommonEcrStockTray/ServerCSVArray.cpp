/**********************************************************************/
#include "CCSVReportLine.h"
#include "DatabaseCSVArray.h"
#include "ImportDefines.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SPOSVersions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ServerCSVArray.h"
/**********************************************************************/

CServerCSVRecord::CServerCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CServerCSVRecord::ClearRecord()
{
	m_nServerNo = 1;
	m_strTimeStamp = "";
	m_strReceiptName = "";
	m_strFullName = "";
	m_nAuthority = 1;
	m_nPassword = 1;
	m_nDrawer = 0;
	m_bFlipView = FALSE;
	m_bTrainingMode = FALSE;
	m_nLogonMacro = 0;
	m_nOpenTableMode = 1;
	m_nHomeNwkLocNo = 0;
	m_dHourlyRate = 0.0;
	m_nDailyHours = 8;
	m_bMidnightShift = FALSE;
	m_nTradingColourScheme = 0;
}

/**********************************************************************/

void CServerCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	SetServerNo(csv.GetInt(0));

	switch (csv.GetInt(1))
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;

	case 2:
		V2ConvertFromCSV(csv);
		break;

	case 3:
		V3ConvertFromCSV(csv);
		break;

	case 4:
		V4to9ConvertFromCSV(csv, 4);
		break;

	case 5:
		V4to9ConvertFromCSV(csv, 5);
		break;

	case 6:
		V4to9ConvertFromCSV(csv, 6);
		break;

	case 7:
		V4to9ConvertFromCSV(csv, 7);
		break;

	case 8:
		V4to9ConvertFromCSV(csv, 8);
		break;

	case 9:
		V4to9ConvertFromCSV(csv, 9);
		break;
	}
}

/**********************************************************************/

void CServerCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetReceiptName(csv.GetString(2));
	SetAuthority(csv.GetInt(3));
	SetPassword(csv.GetInt(4));
	SetDrawer(csv.GetInt(5));
	SetFlipViewFlag(csv.GetBool(6));
	SetTrainingModeFlag(FALSE);
	SetLogonMacro(0);
	SetOpenTableMode(1);
}

/**********************************************************************/

void CServerCSVRecord::V2ConvertFromCSV(CCSV& csv)
{
	SetReceiptName(csv.GetString(2));
	SetAuthority(csv.GetInt(3));
	SetPassword(csv.GetInt(4));
	SetDrawer(csv.GetInt(5));
	SetFlipViewFlag(csv.GetBool(6));
	SetTrainingModeFlag(csv.GetBool(7));
	SetLogonMacro(0);
	SetOpenTableMode(1);
}

/**********************************************************************/

void CServerCSVRecord::V3ConvertFromCSV(CCSV& csv)
{
	SetReceiptName(csv.GetString(2));
	SetAuthority(csv.GetInt(3));
	SetPassword(csv.GetInt(4));
	SetDrawer(csv.GetInt(5));
	SetFlipViewFlag(csv.GetBool(6));
	SetTrainingModeFlag(csv.GetBool(7));
	SetLogonMacro(csv.GetInt(8));
	SetOpenTableMode(1);
}

/**********************************************************************/

void CServerCSVRecord::V4to9ConvertFromCSV(CCSV& csv, int nVer)
{
	int nOffset = 2;

	if (nVer >= 6)
	{
		SetTimeStamp(csv.GetString(nOffset++));
	}

	SetReceiptName(csv.GetString(nOffset++));

	if (nVer >= 5)
	{
		SetFullName(csv.GetString(nOffset++));
	}

	SetAuthority(csv.GetInt(nOffset++));
	SetPassword(csv.GetInt(nOffset++));
	SetDrawer(csv.GetInt(nOffset++));
	SetFlipViewFlag(csv.GetBool(nOffset++));
	SetTrainingModeFlag(csv.GetBool(nOffset++));
	SetLogonMacro(csv.GetInt(nOffset++));
	SetOpenTableMode(csv.GetInt(nOffset++));

	if (nVer >= 7)
	{
		SetHomeNwkLocNo(csv.GetInt(nOffset++));
	}

	if (nVer >= 8)
	{
		SetHourlyRate(csv.GetDouble(nOffset++));
		SetDailyHours(csv.GetInt(nOffset++));
		SetMidnightShiftFlag(csv.GetBool(nOffset++));
	}

	if (nVer >= 9)
	{
		SetTradingColourScheme(csv.GetInt(nOffset++));
	}
}

/**********************************************************************/

void CServerCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(m_nServerNo);
	csv.Add(SERVER_VERSION);
	csv.Add(m_strTimeStamp);
	csv.Add(m_strReceiptName);
	csv.Add(m_strFullName);
	csv.Add(m_nAuthority);
	csv.Add(m_nPassword);
	csv.Add(m_nDrawer);
	csv.Add(m_bFlipView);
	csv.Add(m_bTrainingMode);
	csv.Add(m_nLogonMacro);
	csv.Add(m_nOpenTableMode);
	csv.Add(m_nHomeNwkLocNo);
	csv.Add(m_dHourlyRate,2);
	csv.Add(m_nDailyHours);
	csv.Add(m_bMidnightShift);
	csv.Add(m_nTradingColourScheme);
}

/**********************************************************************/

void CServerCSVRecord::SetServerNo( int n )
{
	Set ( m_nServerNo, n, Server::ServerNo );
}

/**********************************************************************/

void CServerCSVRecord::SetTimeStamp( const char* sz )
{
	Set ( m_strTimeStamp, sz, Server::TimeStamp );
}

/**********************************************************************/

void CServerCSVRecord::SetReceiptName ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strReceiptName, strName, Server::ReceiptName );
}

/**********************************************************************/

void CServerCSVRecord::SetFullName ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strFullName, strName, Server::FullName );
}

/**********************************************************************/

void CServerCSVRecord::SetAuthority( int n )
{
	Set ( m_nAuthority, n, Server::Authority );
}

/**********************************************************************/

void CServerCSVRecord::SetPassword( int n )
{
	Set ( m_nPassword, n, Server::Password );
}

/**********************************************************************/

void CServerCSVRecord::SetDrawer( int n )
{
	Set ( m_nDrawer, n, Server::Drawer );
}

/**********************************************************************/

void CServerCSVRecord::SetLogonMacro( int n )
{
	Set ( m_nLogonMacro, n, Server::LogonMacro );
}

/**********************************************************************/

void CServerCSVRecord::SetOpenTableMode( int n )
{
	Set ( m_nOpenTableMode, n, Server::OpenTableMode );
}

/**********************************************************************/

void CServerCSVRecord::SetHomeNwkLocNo(int n)
{
	Set(m_nHomeNwkLocNo, n, Server::HomeNwkLocNo);
}

/**********************************************************************/

void CServerCSVRecord::SetHourlyRate(double d)
{
	Set(m_dHourlyRate, d, Server::HourlyRate);
}

/**********************************************************************/

void CServerCSVRecord::SetDailyHours(int n)
{
	Set(m_nDailyHours, n, Server::DailyHours);
}

/**********************************************************************/

void CServerCSVRecord::SetTradingColourScheme(int n)
{
	Set(m_nTradingColourScheme, n, Server::TradingColourScheme);
}

/**********************************************************************/

const char* CServerCSVRecord::GetReportName()
{
	CString strTemp = m_strFullName;
	::TrimSpaces ( strTemp, FALSE );

	if (strTemp != "")
	{
		m_strReportName = strTemp;
	}
	else
	{
		m_strReportName = GetReceiptEditName();
	}

	return m_strReportName;
}

/**********************************************************************/

const char* CServerCSVRecord::GetReceiptEditName()
{
	CString strTemp = m_strReceiptName;
	::TrimSpaces ( strTemp, FALSE );
	
	if (strTemp == "")
	{
		m_strReceiptEditName.Format("Server %d", m_nServerNo);
	}
	else
	{
		m_strReceiptEditName = strTemp;
	}

	return m_strReceiptEditName;
}

/**********************************************************************/

void CServerCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	if ( GetServerNo() >= 1000 )
	{
		version.m_nMinVer = SPOS_V4_1124;
		version.m_strReason = "Server number 1000 - 9999";
	}
}

/**********************************************************************/

CString CServerCSVRecord::GetHomeLocName()
{
	m_strHomeLocName = "";

	int nNwkLocNo = GetHomeNwkLocNo();
	if (nNwkLocNo > 0)
	{
		int nLocIdx;
		if (dbLocation.FindLocationByNumber(nNwkLocNo / 1000, nNwkLocNo % 1000, nLocIdx) == TRUE)
		{
			int nDbNo = DataManagerNonDb.Server.GetServerDbNo();
			if (( 0 == nDbNo ) || (dbLocation.GetDbNo(nLocIdx) == nDbNo))
			{
				m_strHomeLocName = dbLocation.GetName(nLocIdx);
			}
		}
	}

	return m_strHomeLocName;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CServerCSVArray::CServerCSVArray() 
{
	m_nSortMode = SERVER_SORT_NUMBER;
	m_nServerDbNo = 0;
	m_bShowHomeLoc = FALSE;
}

/**********************************************************************/

CServerCSVArray::~CServerCSVArray() 
{
}

/**********************************************************************/

int CServerCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	int nLastServerNo = 0;
	int nValidRecords = 0;
	
	CServerCSVRecord ServerRecord;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, ServerRecord );

		int nServerNo = ServerRecord.GetServerNo();
		if (nServerNo <= nLastServerNo || nServerNo > Server::ServerNo.Max)
		{
			break;
		}

		nLastServerNo = nServerNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

bool CServerCSVArray::FindServerByNumber( int nServerNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CServerCSVRecord Server;
		GetAt ( nIndex, Server );
		int nArrayNo = Server.GetServerNo();
			
		if (nServerNo == nArrayNo)
		{
			return TRUE;
		}

		if (nServerNo < nArrayNo)
		{
			nEnd = nIndex - 1;
		}
		else
		{
			nStart = nIndex + 1;
		}
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CServerCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CServerCSVRecord Server;
	for ( int n = 0; n < GetSize(); n++ )
	{
		GetAt( n, Server );
		if ( Server.GetServerNo() > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if (nResult < Server::ServerNo.Min || nResult > Server::ServerNo.Max)
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

const char* CServerCSVArray::GetReportName(int nServerNo)
{
	if (nServerNo == 0)
	{
		m_strReportName = "None";
	}
	else
	{
		m_strReportName.Format("Server %4.4d", nServerNo);

		if (EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE)
		{
			int nServerIdx = 0;
			if (FindServerByNumber(nServerNo, nServerIdx) == TRUE)
			{
				CServerCSVRecord Server;
				GetAt(nServerIdx, Server);
				m_strReportName = Server.GetReportName();
			}
		}
	}

	return m_strReportName;
}

/**********************************************************************/

bool CServerCSVArray::IsTrainingServer( int nServerNo )
{
	int nServerIdx;
	if (FindServerByNumber(nServerNo, nServerIdx) == FALSE)
	{
		return FALSE;
	}

	CServerCSVRecord Server;
	GetAt( nServerIdx, Server );
	return Server.GetTrainingModeFlag();
}

/**********************************************************************/

int CServerCSVArray::GetServerNo( int nIndex )
{
	int nServerNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nServerNo = csv.GetInt(0);
	}

	return nServerNo;
}

/**********************************************************************/

void CServerCSVArray::CreateServerReport(int nDbIdx, int nLocIdx, int nTerminalLocIdx, bool bSortOrder, CReportFile& ReportFile)
{
#ifndef POSTRAY_UTILITY
	CString strExtra = "";

	if (nLocIdx >= 0)
	{
		strExtra.Format(" (%s)", dbLocation.GetName(nLocIdx));
	}
	else if (nDbIdx >= 0)
	{
		strExtra.Format(" (%s)", dbDatabase.GetName(nDbIdx));
	}
	else
	{
		strExtra = " (Global)";
	}

	CString str = "Server Details";
	str += strExtra;

	std::map<int, int64_t> MapLogonNoToServerNo;
	if ((PasswordArray.GetEnable(PasswordTicks::EditServerDetails) == TRUE) && (SysInfo.IsEcrLiteSystem() == FALSE))
	{
		if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
		{
			for (int n = 0; n < DataManagerNonDb.ServerLogon.GetSize(); n++)
			{
				CServerLogonCSVRecord ServerLogon;
				DataManagerNonDb.ServerLogon.GetAt(n, ServerLogon);
				__int64 nLogonNo = ServerLogon.GetLogonNo();
				int nServerNo = ServerLogon.GetServerNo();
				
				if (MapLogonNoToServerNo.find(nServerNo) == MapLogonNoToServerNo.end())
				{
					MapLogonNoToServerNo[nServerNo] = nLogonNo;
				}
			}
		}
	}

	ReportFile.SetStyle1(str);
	ReportFile.AddColumn("No", TA_LEFT, 100);
	ReportFile.AddColumn("Receipt Name", TA_LEFT, 400);
	ReportFile.AddColumn("Full Name", TA_LEFT, 400);

	if ((PasswordArray.GetEnable(PasswordTicks::EditServerDetails) == TRUE) && (SysInfo.IsEcrLiteSystem() == FALSE))
	{
		if (DataManagerNonDb.Server.GetShowHomeLocFlag() == TRUE)
		{
			ReportFile.AddColumn("Default Location", TA_LEFT, 300);
		}
		ReportFile.AddColumn("Password", TA_LEFT, 200);
		ReportFile.AddColumn("Authority", TA_LEFT, 200);
		ReportFile.AddColumn("Drawer", TA_LEFT, 200);
		ReportFile.AddColumn("Left/Right", TA_LEFT, 200);
		ReportFile.AddColumn("Training", TA_LEFT, 200);
		ReportFile.AddColumn("Open Table", TA_LEFT, 350);
		ReportFile.AddColumn("Hourly Rate", TA_LEFT, 350);
		ReportFile.AddColumn("Daily Hours", TA_LEFT, 350);
		ReportFile.AddColumn("Midnight Shift", TA_LEFT, 350);
		ReportFile.AddColumn("Colour Scheme", TA_LEFT, 350);

		if ( EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE )
		{
			ReportFile.AddColumn("MiFare Logon", TA_LEFT, 400);
		}

		ReportFile.AddColumn("Logon Macro", TA_LEFT, 700);

		if ( nTerminalLocIdx >= 0 )
		{
			ReportFile.AddColumn("Terminals", TA_LEFT, 700);
		}
	}

	if (FALSE == bSortOrder)
	{
		for (int nServerIdx = 0; nServerIdx < GetSize(); nServerIdx++)
		{
			AppendRecordToReport(ReportFile, nServerIdx, nTerminalLocIdx, MapLogonNoToServerNo);
		}
	}
	else
	{
		for (int nSortIdx = 0; nSortIdx < GetSortArraySize(); nSortIdx++)
		{
			AppendRecordToReport(ReportFile, GetArrayIdxFromSortIdx(nSortIdx), nTerminalLocIdx, MapLogonNoToServerNo);
		}
	}
#endif
}

/**********************************************************************/

void CServerCSVArray::AppendRecordToReport(CReportFile& ReportFile, int nServerIdx, int nTerminalLocIdx, std::map<int, int64_t>& MapLogonNoToServerNo)
{
#ifndef POSTRAY_UTILITY
	CServerCSVRecord ServerRecord;
	GetAt(nServerIdx, ServerRecord);

	CCSVReportLine csvOut;
	csvOut.AppendInt(ServerRecord.GetServerNo());
	csvOut.AppendString(ServerRecord.GetReceiptName());
	csvOut.AppendString(ServerRecord.GetFullName());

	if ((PasswordArray.GetEnable(PasswordTicks::EditServerDetails) == TRUE) && (SysInfo.IsEcrLiteSystem() == FALSE))
	{
		if (DataManagerNonDb.Server.GetShowHomeLocFlag() == TRUE)
		{
			csvOut.AppendString(ServerRecord.GetHomeLocName());
		}

		csvOut.AppendInt(ServerRecord.GetPassword());
		csvOut.AppendInt(ServerRecord.GetAuthority());

		switch (ServerRecord.GetDrawer())
		{
		case 1:		
			csvOut.AppendString("One");
			break;

		case 2:		
			csvOut.AppendString("Two");	
			break;

		case 0:
		default:	
			csvOut.AppendString("None");	
			break;
		}

		if (ServerRecord.GetFlipViewFlag() == TRUE)
		{
			csvOut.AppendString("Left");
		}
		else
		{
			csvOut.AppendString("Right");
		}

		csvOut.AppendYesOrBlank(ServerRecord.GetTrainingModeFlag());

		switch (ServerRecord.GetOpenTableMode())
		{
		case 1:
			csvOut.AppendString("Touch");
			break;

		case 2:
			csvOut.AppendString("Select and Open");
			break;

		default:
			csvOut.AppendString("Unknown");
			break;
		}

		csvOut.Add(ServerRecord.GetHourlyRate(),2);
		csvOut.Add(ServerRecord.GetDailyHours());
		csvOut.AppendYesOrBlank(ServerRecord.GetMidnightShiftFlag());
		csvOut.Add(ServerRecord.GetTradingColourScheme());

		if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
		{
			int nServerNo = ServerRecord.GetServerNo();
			auto it = MapLogonNoToServerNo.find(nServerNo);
			if (it != MapLogonNoToServerNo.end())
			{
				csvOut.Add(it->second);
			}
			else
			{
				csvOut.AppendString("None");
			}
		}

		int nMacroNo = ServerRecord.GetLogonMacro();
		if (nMacroNo != 0)
		{
			CString str;
			str.Format("%5.5d, %s",
				nMacroNo,
				DataManagerNonDb.CCMacroActive.GetMacroNameFromNumber(nMacroNo));

			csvOut.AppendString(str);
		}
		else			
		{
			csvOut.AppendString("");
		}

		if (nTerminalLocIdx >= 0)
		{
			CServerRestrictionCSVRecord RestrictionRecord;
			DataManagerNonDb.ServerRestriction.GetRestrictions(ServerRecord, RestrictionRecord);

			CReportConsolidationArray<CSortedIntItem> arrayRestricted;

			CCSV csvRestriction(RestrictionRecord.GetRestrictions(), ';');
			for (int n = 0; n < csvRestriction.GetSize(); n++)
			{
				CSortedIntItem item;
				item.m_nItem = csvRestriction.GetInt(n);
				arrayRestricted.Consolidate(item);
			}

			CCSV csvAllowed;

			int nLocIdx = DataManagerNonDb.ServerRestriction.GetCurrentLocIdx();
			for (int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount(nLocIdx); nTermIdx++)
			{
				CSortedIntItem item;
				item.m_nItem = dbLocation.GetTNo(nLocIdx, nTermIdx);

				int nPos = 0;
				if (arrayRestricted.Find(item, nPos) == FALSE)
				{
					csvAllowed.Add(item.m_nItem);
				}
			}
			
			csvOut.AppendString(csvAllowed.GetLine());
		}
	}

	ReportFile.WriteLine(csvOut.GetLine());
#endif
}

/**********************************************************************/

void CServerCSVArray::SetSortMode(int nMode)
{
	switch (nMode)
	{
	case SERVER_SORT_NUMBER:
	case SERVER_SORT_NAME_UP:
	case SERVER_SORT_NAME_DOWN:
	case SERVER_SORT_HOMELOC_UP:
	case SERVER_SORT_HOMELOC_DOWN:
		m_arraySort.RemoveAll();
		m_nSortMode = nMode;
		break;

	default:
		return;
	}

	switch (m_nSortMode)
	{
	case SERVER_SORT_NAME_UP:
	case SERVER_SORT_NAME_DOWN:
	case SERVER_SORT_HOMELOC_UP:
	case SERVER_SORT_HOMELOC_DOWN:
	{
		CReportConsolidationArray<CSortedStringAndIntItem> arrayTemp(TRUE);

		for (int n = 0; n < GetSize(); n++)
		{
			CServerCSVRecord Server;
			GetAt(n, Server);

			CSortedStringAndIntItem item;

			switch (m_nSortMode)
			{
			case SERVER_SORT_NAME_UP:
			case SERVER_SORT_NAME_DOWN:
				item.m_strItem = Server.GetReportName();
				break;

			case SERVER_SORT_HOMELOC_UP:
			case SERVER_SORT_HOMELOC_DOWN:
				item.m_strItem = Server.GetHomeLocName();
				break;
			}

			item.m_strItem.MakeUpper();
			
			switch (m_nSortMode)
			{
			case SERVER_SORT_NAME_UP:
			case SERVER_SORT_HOMELOC_UP:
				item.m_nItem = n;
				break;

			case SERVER_SORT_NAME_DOWN:
			case SERVER_SORT_HOMELOC_DOWN:
				item.m_nItem = n * (-1);
				break;
			}
			
			arrayTemp.Consolidate(item);
		}

		for (int n = 0; n < arrayTemp.GetSize(); n++)
		{
			CSortedStringAndIntItem item;
			arrayTemp.GetAt(n, item);

			switch (m_nSortMode)
			{
			case SERVER_SORT_NAME_DOWN:
			case SERVER_SORT_HOMELOC_DOWN:
				m_arraySort.InsertAt(0, item.m_nItem * (-1));
				break;

			case SERVER_SORT_NAME_UP:
			case SERVER_SORT_HOMELOC_UP:
				m_arraySort.Add(item.m_nItem);
				break;
			}
		}
	}
	break;
	}
}

/**********************************************************************/

int CServerCSVArray::GetSortArraySize()
{
	switch (m_nSortMode)
	{
	case SERVER_SORT_NAME_UP:
	case SERVER_SORT_NAME_DOWN:
	case SERVER_SORT_HOMELOC_UP:
	case SERVER_SORT_HOMELOC_DOWN:
		return m_arraySort.GetSize();

	case SERVER_SORT_NUMBER:
	default:
		return GetSize();
	}
}

/**********************************************************************/

int CServerCSVArray::GetArrayIdxFromSortIdx(int n)
{
	int nResult = 0;

	switch (m_nSortMode)
	{
	case SERVER_SORT_NAME_UP:
	case SERVER_SORT_NAME_DOWN:
	case SERVER_SORT_HOMELOC_UP:
	case SERVER_SORT_HOMELOC_DOWN:
		if ((n >= 0) && (n < m_arraySort.GetSize()))
		{
			nResult = m_arraySort.GetAt(n);
		}
		else
		{
			nResult = 0;
		}
		break;

	default:
		nResult = n;
		break;
	}

	return nResult;
}

/**********************************************************************/

int CServerCSVArray::GetSortIdxFromArrayIdx(int n)
{
	int nResult = 0;

	switch (m_nSortMode)
	{
	case SERVER_SORT_NAME_UP:
	case SERVER_SORT_NAME_DOWN:
	case SERVER_SORT_HOMELOC_UP:
	case SERVER_SORT_HOMELOC_DOWN:
	{
		for (int x = 0; x < m_arraySort.GetSize(); x++)
		{
			if (m_arraySort.GetAt(x) == n)
			{
				nResult = x;
				break;
			}
		}
	}
	break;

	default:
		nResult = n;
		break;
	}

	return nResult;
}

/**********************************************************************/

int CServerCSVArray::ImportLine(int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine)
{
	CCSV csvImport(szImportLine);
	int nServerNo = csvImport.GetInt(0);

	if ( ( nServerNo >= Server::ServerNo.Min ) && ( nServerNo <= Server::ServerNo.Max) )
	{
		if (FindServerByNumber(nServerNo, nIndex) == TRUE)	// server already exists
		{
			if (nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_EXIST || nImportMethod == IMPORT_METHOD_PRICESET)
			{
				CServerCSVRecord NewRecord;
				GetAt(nIndex, NewRecord);
				NewRecord.ImportLine(import, csvImport);
				SetAt(nIndex, NewRecord);
				return IMPORT_LINE_MODIFIED;
			}
		}
		else			// server not found
		{
			if (nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_NEW)
			{
				CServerCSVRecord NewRecord;
				NewRecord.ImportLine(import, csvImport);

				CString strTimeStamp;
				::GetSimpleHexTimeStamp(strTimeStamp);
				NewRecord.SetTimeStamp(strTimeStamp);

				InsertAt(nIndex, NewRecord);
				return IMPORT_LINE_ADDED;
			}
		}
	}

	nIndex = -1;

	return IMPORT_LINE_IGNORED;
}

/**********************************************************************/

void CServerCSVRecord::ImportLine(CSSImportFile& import, CCSV& csv)
{
	int n = 0;

	//DESCRIPTION AND TYPE
	ImportInt(csv, import, n, m_nServerNo, Server::ServerNo);
	n++;		// protected "VersionNo"
	n++;		// protected "TimeStamp"
	ImportString(csv, import, n, m_strReceiptName, Server::ReceiptName);
	ImportString(csv, import, n, m_strFullName, Server::FullName);
	ImportInt(csv, import, n, m_nAuthority, Server::Authority);
	ImportInt(csv, import, n, m_nPassword, Server::Password);
	ImportInt(csv, import, n, m_nDrawer, Server::Drawer);
	ImportBool(csv, import, n, m_bFlipView);
	ImportBool(csv, import, n, m_bTrainingMode);
	ImportInt(csv, import, n, m_nLogonMacro, Server::LogonMacro);
	ImportInt(csv, import, n, m_nOpenTableMode, Server::OpenTableMode);
	
	if (DealerFlags.GetServerHomeLocationFlag() == TRUE)
	{
		ImportInt(csv, import, n, m_nHomeNwkLocNo, Server::HomeNwkLocNo);
	}
	else
	{
		n++;
	}
	
	ImportDouble(csv, import, n, m_dHourlyRate, Server::HourlyRate);
	ImportInt(csv, import, n, m_nDailyHours, Server::DailyHours);
	ImportBool(csv, import, n, m_bMidnightShift);
	ImportInt(csv, import, n, m_nTradingColourScheme, Server::TradingColourScheme);
}

/********************************************************************/

void CServerCSVRecord::ImportString(CCSV& csv, CSSImportFile& import, int& n, CString& str, const recString& rec, bool bFilter)
{
	if (import.IsRequired(n) == TRUE)
	{
		CString strIn;
		if (FALSE == bFilter)
		{
			strIn = csv.GetString(n);
		}
		else
		{
			strIn = FilterEposText(csv.GetString(n));
		}

		Import(strIn, str, rec);
	}

	n++;
}

/**********************************************************************/

void CServerCSVRecord::ImportDouble(CCSV& csv, CSSImportFile& import, int& n, double& d, const recDouble& rec)
{
	if (import.IsRequired(n) == TRUE)
	{
		Import(csv.GetString(n), d, rec);
	}

	n++;
}

/**********************************************************************/

void CServerCSVRecord::ImportInt(CCSV& csv, CSSImportFile& import, int& n, int& i, const recInt& rec)
{
	if (import.IsRequired(n) == TRUE)
	{
		Import(csv.GetString(n), i, rec);
	}

	n++;
}

/**********************************************************************/

void CServerCSVRecord::ImportInt64(CCSV& csv, CSSImportFile& import, int& n, __int64& i, const recInt64& rec)
{
	if (import.IsRequired(n) == TRUE)
	{
		Import(csv.GetString(n), i, rec);
	}

	n++;
}

/**********************************************************************/

void CServerCSVRecord::ImportBool(CCSV& csv, CSSImportFile& import, int& n, bool& b)
{
	if (import.IsRequired(n) == TRUE)
	{
		Import(csv.GetString(n), b);
	}

	n++;
}

/**********************************************************************/
