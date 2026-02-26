/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "Password.h"
/**********************************************************************/

void CPasswordTickNameArray::AddTickName( const CPasswordTickInfo& info, const char* szName )
{
	CSortedStringByInt item;
	item.m_nItem = info.m_nTickNo;
	item.m_strItem = szName;
	m_arrayTickNames.Consolidate( item );
}

/**********************************************************************/

const char* CPasswordTickNameArray::GetTickName( int nTickNo )
{
	CSortedStringByInt item;
	item.m_nItem = nTickNo;

	int nPos;
	if ( m_arrayTickNames.Find( item, nPos ) == TRUE )
	{
		m_arrayTickNames.GetAt( nPos, item );
		m_strTickName = item.m_strItem;
	}
	else
		m_strTickName = "Unknown";

	return m_strTickName;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPasswordCSVRecord::CPasswordCSVRecord()
{
	m_strPassword = "";
	m_strUserName = "";
	m_strPluFilter = "";
	SetAllEnables(TRUE);
}

/**********************************************************************/

void CPasswordCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(m_strUserName);
	csv.Add(m_strPassword);
	csv.Add(m_strEnable);
	csv.Add(m_strPluFilter);
}

/**********************************************************************/

void CPasswordCSVRecord::ConvertFromCSV(CCSV& csv)
{
	m_strUserName = csv.GetString(0);
	m_strPassword = csv.GetString(1);
	m_strEnable = csv.GetString(2);
	m_strPluFilter = csv.GetString(3);
}

/**********************************************************************/

void CPasswordCSVRecord::CheckEnables()
{
	CString strEnable = m_strEnable;
	CPasswordCSVArray::Code ( strEnable );
	::AddTrailing ( strEnable, PASSWORD_FLAGS, '1' );
	strEnable = strEnable.Left ( PASSWORD_FLAGS );
	CPasswordCSVArray::Code ( strEnable );
	m_strEnable = strEnable;
}

/**********************************************************************/

bool CPasswordCSVRecord::GetEnable ( CPasswordTickInfo info )
{
	CString strEnable = m_strEnable;
	CPasswordCSVArray::Code ( strEnable );

	if ( strEnable.GetAt(info.m_nTickNo) == '0' )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CPasswordCSVRecord::SetEnable ( CPasswordTickInfo info, bool bEnable )
{
	CString strEnable = m_strEnable;
	CPasswordCSVArray::Code ( strEnable );
	char c = ( bEnable == TRUE ) ? '1' : '0';
	strEnable.SetAt ( info.m_nTickNo, c );
	CPasswordCSVArray::Code ( strEnable );
	m_strEnable = strEnable;
}

/**********************************************************************/

void CPasswordCSVRecord::SetAllEnables ( bool bEnable )
{
	char c = ( bEnable == TRUE ) ? '1' : '0';
	CString strEnable = "";
	::AddLeading ( strEnable, PASSWORD_FLAGS, c );
	CPasswordCSVArray::Code ( strEnable );
	m_strEnable = strEnable;
}

/**********************************************************************/

void CPasswordCSVRecord::SetLocationAccess( int nType, int nIndex )
{
	CString strEnable = m_strEnable;
	CPasswordCSVArray::Code ( strEnable );
		
	CString strDbNo = "000";
	CString strNwkNo = "000";
	CString strLocNo = "000";

	switch( nType )
	{
	case LOCATION_ACCESS_NONE:
	case LOCATION_ACCESS_SYSTEM:
		break;

	case LOCATION_ACCESS_DB:
		if ( ( nIndex >= 0 ) && ( nIndex < dbDatabase.GetSize() ) )
			strDbNo.Format( "%3.3d", dbDatabase.GetDbNo( nIndex ) );
		else
			nType = LOCATION_ACCESS_NONE;
		break;

	case LOCATION_ACCESS_LOC:
		{
			bool bAccept = FALSE;

			if ( ( nIndex >= 0 ) && ( nIndex < dbLocation.GetSize() ) )
			{
				int nDbNo = dbLocation.GetDbNo( nIndex );
				if ( nDbNo != 0 )
				{
					strDbNo.Format( "%3.3d", nDbNo );
					strNwkNo.Format( "%3.3d", dbLocation.GetNetworkNo( nIndex ) );
					strLocNo.Format( "%3.3d", dbLocation.GetLocNo( nIndex ) );
					bAccept = TRUE;
				}
			}

			if ( FALSE == bAccept )
				nType = LOCATION_ACCESS_NONE;
		}
	}

	CString strType;
	strType.Format( "%1.1d", nType );	
	
	switch( nType )
	{
	case LOCATION_ACCESS_NONE:
	case LOCATION_ACCESS_SYSTEM:
	case LOCATION_ACCESS_DB:
	case LOCATION_ACCESS_LOC:
		strEnable.SetAt( 89, strType.GetAt(0) );
		strEnable.SetAt( 90, strDbNo.GetAt(0) );
		strEnable.SetAt( 91, strDbNo.GetAt(1) );
		strEnable.SetAt( 92, strDbNo.GetAt(2) );
		strEnable.SetAt( 93, strNwkNo.GetAt(0) );
		strEnable.SetAt( 94, strNwkNo.GetAt(1) );
		strEnable.SetAt( 95, strNwkNo.GetAt(2) );
		strEnable.SetAt( 96, strLocNo.GetAt(0) );
		strEnable.SetAt( 97, strLocNo.GetAt(1) );
		strEnable.SetAt( 98, strLocNo.GetAt(2) );

		CPasswordCSVArray::Code ( strEnable );
		m_strEnable = strEnable;
		break;
	}
}

/**********************************************************************/

void CPasswordCSVRecord::GetLocationAccess( int& nType, int& nDbIdx, int& nLocIdx, int& nSpIdx )
{
	CString strEnable = m_strEnable;
	CPasswordCSVArray::Code ( strEnable );

	nType = LOCATION_ACCESS_NONE;
	nDbIdx = 0;
	nLocIdx = 0;
	nSpIdx = -1;

	nType = atoi( strEnable.Mid(89,1) );

	switch( nType )
	{
	case LOCATION_ACCESS_NONE:
	case LOCATION_ACCESS_SYSTEM:
	case LOCATION_ACCESS_DB:
	case LOCATION_ACCESS_LOC: 
		break;

	default:
		nType = LOCATION_ACCESS_NONE;
		break;
	}

	switch( nType )
	{
	case LOCATION_ACCESS_NONE:
	case LOCATION_ACCESS_SYSTEM:
		break;

	case LOCATION_ACCESS_DB:
		{
			int nDbNo = atoi( strEnable.Mid(90,3) );
			
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			{
				nType = LOCATION_ACCESS_NONE;
				nDbIdx = 0;
			}
		}
		break;

	case LOCATION_ACCESS_LOC:
		{
			int nDbNo = atoi( strEnable.Mid(90,3) );
			int nNwkNo = atoi( strEnable.Mid(93,3) );
			int nLocNo = atoi( strEnable.Mid(96,3) );
			
			if ( dbLocation.FindLocationByNumber( nNwkNo, nLocNo, nLocIdx ) == FALSE )
			{
				nType = LOCATION_ACCESS_NONE;
				nDbIdx = 0;
				nLocIdx = 0;
			}

			if ( dbLocation.GetDbNo( nLocIdx ) != nDbNo )
			{
				nType = LOCATION_ACCESS_NONE;
				nDbIdx = 0;
				nLocIdx = 0;
			}
			
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			{
				nType = LOCATION_ACCESS_NONE;
				nDbIdx = 0;
				nLocIdx = 0;
			}
		}
		break;
	}

#ifdef STOCKMAN_SYSTEM
	if ( nType == LOCATION_ACCESS_LOC )
	{
		int nCurrentSpNo = 0;

		for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
		{
			int nSpNo = dbLocation.GetSpNo( nLocIdx, nEcrIdx );

			if ( 0 == nSpNo )
			{
				nCurrentSpNo = 0;
				break;
			}

			if ( 0 == nCurrentSpNo )
			{
				nCurrentSpNo = nSpNo;
				break;
			}
				
			if ( nCurrentSpNo != nSpNo )
			{
				nCurrentSpNo = 0;
				break;
			}
		}

		if ( nCurrentSpNo != 0 )
		{
			CArray<int,int> arrayLocIdx;
			dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

			for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
			{
				int nCheckLocIdx = arrayLocIdx.GetAt(n);

				if ( nLocIdx == nCheckLocIdx )
					continue;

				for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nCheckLocIdx ); nEcrIdx++ )
				{
					if ( dbLocation.GetSpNo( nCheckLocIdx, nEcrIdx ) == nCurrentSpNo )
					{
						nCurrentSpNo = 0;
						break;
					}
				}

				if ( 0 == nCurrentSpNo )
					break;
			}
		}

		if ( nCurrentSpNo != 0 )
		{
			int nDbNo = dbDatabase.GetDbNo( nDbIdx );
			if ( dbStockpoint.FindStockpointByNumber( nDbNo, nCurrentSpNo, nSpIdx ) == FALSE )
				nSpIdx = -1;
		}
	}
#endif
}

/**********************************************************************/

const char* CPasswordCSVRecord::GetPassword()
{
	m_strDisplayPassword = m_strPassword;
	CPasswordCSVArray::Code ( m_strDisplayPassword );
	return m_strDisplayPassword;
}

/**********************************************************************/

void CPasswordCSVRecord::SetPassword ( const char* szPassword )
{
	m_strPassword = szPassword;
	CPasswordCSVArray::Code ( m_strPassword );
}

/**********************************************************************/

const char* CPasswordCSVRecord::GetUserName()
{
	m_strDisplayUserName = m_strUserName;
	CPasswordCSVArray::Code ( m_strDisplayUserName );
	return m_strDisplayUserName;
}

/**********************************************************************/

void CPasswordCSVRecord::SetUserName ( const char* szUserName )
{
	m_strUserName = szUserName;
	CPasswordCSVArray::Code ( m_strUserName );
}

/**********************************************************************/

const char* CPasswordCSVRecord::GetPluFilter()
{
	m_strDisplayPluFilter = m_strPluFilter;
	CPasswordCSVArray::Code(m_strDisplayPluFilter);
	return m_strDisplayPluFilter;
}

/**********************************************************************/

void CPasswordCSVRecord::SetPluFilter(const char* szFilter)
{
	m_strPluFilter = szFilter;
	CPasswordCSVArray::Code(m_strPluFilter);
}

/**********************************************************************/

void CPasswordCSVRecord::GetPluFilterArray(CReportConsolidationArray<CSortedIntByInt>& arrayFilter)
{
	arrayFilter.RemoveAll();

	CCSV csv(GetPluFilter());

	for (int n = 0; n < csv.GetSize() - 1; n += 2 )
	{
		int nDbNo = csv.GetInt(n);
		int nPreset = csv.GetInt(n + 1);

		int nDbIdx;
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == TRUE)
		{
			if ((nPreset >= 1) && (nPreset <= 20))
			{
				CSortedIntByInt item;
				item.m_nKey = nDbNo;
				item.m_nVal = nPreset;
				arrayFilter.Consolidate(item);
			}
		}
	}
}

/**********************************************************************/

void CPasswordCSVRecord::SetPluFilterArray(CReportConsolidationArray<CSortedIntByInt>& arrayFilter)
{
	CCSV csv;

	for (int n = 0; n < arrayFilter.GetSize(); n++)
	{
		CSortedIntByInt item;
		arrayFilter.GetAt(n, item);

		int nDbNo = item.m_nKey;
		int nPreset = item.m_nVal;

		int nDbIdx;
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == TRUE)
		{
			if ((nPreset >= 1) && (nPreset <= 20))
			{
				csv.Add(nDbNo);
				csv.Add(nPreset);
			}
		}
	}

	SetPluFilter(csv.GetLine());
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPasswordCSVArray::CPasswordCSVArray() : CCSVArray()
{
	m_nPasswordIdx = -1;
	m_strPassword = "";
	m_strCurrentPassword = "";
	m_strDisplayPassword = "";

	m_nLocationAccessType = LOCATION_ACCESS_NONE;

	m_bLocationAccessLoc = FALSE;
	m_bLocationAccessSp = FALSE;

	m_nLocationAccessDbIdx = 0;
	m_nLocationAccessLocIdx = 0;

	m_bFatalReadError = FALSE;
}

/**********************************************************************/

const char* CPasswordCSVArray::GetPassword( int nPasswordIdx )
{
	m_strPassword = "";
	CString strTemp;

	if ( ( strTemp = GetAt ( nPasswordIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		m_strPassword = csv.GetString ( 1 );
		Code ( m_strPassword );
	}
	return m_strPassword;
}

/**********************************************************************/

const char* CPasswordCSVArray::GetUserName( int nPasswordIdx )
{
	m_strUserName = "";
	CString strTemp;

	if ( ( strTemp = GetAt ( nPasswordIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strUserName = csv.GetString ( 0 );
		Code ( m_strUserName );
	}
	return m_strUserName;
}

/**********************************************************************/

const char* CPasswordCSVArray::GetPluFilter(int nPasswordIdx)
{
	m_strPluFilter = "";
	CString strTemp;

	if ((strTemp = GetAt(nPasswordIdx)) != "")
	{
		CCSV csv(strTemp, ',', '"', 4);
		m_strPluFilter = csv.GetString(3);
		Code(m_strPluFilter);
	}
	return m_strPluFilter;
}

/**********************************************************************/

const char* CPasswordCSVArray::GetUserNo()
{
	if ( m_nPasswordIdx >= 0 )
		m_strUserNo.Format ( "%2.2d", m_nPasswordIdx );
	else
		m_strUserNo = "";

	return m_strUserNo;
}

/**********************************************************************/

bool CPasswordCSVArray::Read()
{
	CFilenameUpdater FnUp ( SysFiles::Passwords );
	
	if ( CCSVArray::Read( FnUp.GetFilenameToUse() ) == FALSE )
	{
		if ( GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}
	}

	//make sure password enable strings are the correct length
	for ( int nPasswordIdx = 0; nPasswordIdx < GetSize(); nPasswordIdx++ )
	{
		CPasswordCSVRecord PasswordRecord;
		GetAt ( nPasswordIdx, PasswordRecord );
		PasswordRecord.CheckEnables();
		SetAt ( nPasswordIdx, PasswordRecord );
	}

	//delete extra passwords
	for ( int nPasswordIdx = GetSize() - 1; nPasswordIdx > MAX_PASSWORDS; nPasswordIdx--)
		RemoveAt ( nPasswordIdx );

	//add passwords up to maximum
	while ( GetSize() < MAX_PASSWORDS + 1 )
	{
		CPasswordCSVRecord PasswordRecord;
		PasswordRecord.SetAllEnables ( TRUE );
		Add ( PasswordRecord );
	}

	return TRUE;
}

/**********************************************************************/

bool CPasswordCSVArray::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CFilenameUpdater FnUp( SysFiles::Passwords );
		bResult = CCSVArray::Write ( FnUp.GetFilenameToUse() );
		DataManager.UnlockAllUsers();
	}
	
	return bResult;
}

/**********************************************************************/

void CPasswordCSVArray::CheckPassword(const char* szPassword)
{
	m_strCurrentPassword = szPassword;
	m_strCurrentUserName = "Unknown";
	m_strCurrentPluFilter = "";

	m_nPasswordIdx = -1;

	//Check for master password ( can be blank )
	if (m_strCurrentPassword == "?TELLME?" || m_strCurrentPassword == GetPassword(0))
	{
		m_nPasswordIdx = 0;
		m_strCurrentUserName = "Master";
		m_nLocationAccessType = LOCATION_ACCESS_SYSTEM;
		m_bLocationAccessLoc = TRUE;
		m_bLocationAccessSp = TRUE;
		return;
	}

	//Other blank passwords are rejected
	if (m_strCurrentPassword == "")
		return;

	//Search user passwords for a match
	for (int nPasswordIdx = 1; nPasswordIdx < GetSize(); nPasswordIdx++)
	{
		if (m_strCurrentPassword == GetPassword(nPasswordIdx))
		{
			m_nPasswordIdx = nPasswordIdx;
			m_strCurrentUserName = GetUserName(nPasswordIdx);
			m_strCurrentPluFilter = GetPluFilter(nPasswordIdx);

			CPasswordCSVRecord PasswordRecord;
			GetAt(m_nPasswordIdx, PasswordRecord);

			int nType, nDbIdx, nLocIdx, nSpIdx;
			PasswordRecord.GetLocationAccess(nType, nDbIdx, nLocIdx, nSpIdx);

			m_nLocationAccessType = nType;
			m_nLocationAccessDbIdx = nDbIdx;
			m_nLocationAccessLocIdx = nLocIdx;
			m_nLocationAccessSpIdx = nSpIdx;

			switch (nType)
			{
			case LOCATION_ACCESS_LOC:
				m_bLocationAccessLoc = TRUE;
				m_bLocationAccessSp = (nSpIdx != -1);
				break;

			case LOCATION_ACCESS_DB:
			{
#ifdef STOCKMAN_SYSTEM
				int nStartIdx, nEndIdx;
				dbStockpoint.GetSpIdxRange(nDbIdx, nStartIdx, nEndIdx);
				m_bLocationAccessSp = (nEndIdx >= nStartIdx);
#endif

				CArray<int, int> arrayLocIdx;
				dbLocation.GetDatabaseLocIdxSet(nDbIdx, arrayLocIdx);
				m_bLocationAccessLoc = (arrayLocIdx.GetSize() != 0);
			}
			break;

			case LOCATION_ACCESS_SYSTEM:
				m_bLocationAccessLoc = TRUE;
				m_bLocationAccessSp = TRUE;
				break;

			case LOCATION_ACCESS_NONE:
			default:
				m_bLocationAccessLoc = FALSE;
				m_bLocationAccessSp = FALSE;
				break;
			}
			break;
		}
	}
}

/**********************************************************************/

bool CPasswordCSVArray::GetEnable ( CPasswordTickInfo info )
{
	switch ( m_nPasswordIdx )
	{
	case -1:		
		return FALSE;
	case 0:		
		return TRUE;
	
	default:		
		{
			CPasswordCSVRecord PasswordRecord;
			GetAt ( m_nPasswordIdx, PasswordRecord );
			return PasswordRecord.GetEnable ( info );
		}
	}
}

/**********************************************************************/

void CPasswordCSVArray::BuildTickNameList()
{
	m_arrayTickNames.RemoveAll();
	m_arrayTickNames.AddTickName(PasswordTicks::FileCommsLog, "View communication logs");
	m_arrayTickNames.AddTickName(PasswordTicks::FileBackup, "Backup system data");
	m_arrayTickNames.AddTickName(PasswordTicks::FileRestore, "Restore system data");
	m_arrayTickNames.AddTickName(PasswordTicks::FileBackupLog, "View system backup log");
	m_arrayTickNames.AddTickName(PasswordTicks::DatabaseMenu, "Enable Database menu");
	m_arrayTickNames.AddTickName(PasswordTicks::DatabaseOrderProcess, "Generate and process orders");
	m_arrayTickNames.AddTickName(PasswordTicks::CommsDownloads, "Setup download jobs");
	m_arrayTickNames.AddTickName(PasswordTicks::CommsBatchSales, "Setup batch sales jobs");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsMenu, "Enable Reports menu");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsEpos, "View Epos reports");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsStock, "View stock level reports");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsAudit, "View stock audit reports");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenu, "Enable Setup Menu");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminMenu, "Enable Admin menu");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminEposSales, "Delete Epos reports");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminStockSales, "Delete stock sales history");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminPluStock, "Delete unsold Plu or stock items");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminOrders, "Delete outstanding orders");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminOrderAudits, "Delete audit files for processed orders");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminStockChange, "Change stock codes for existing items");
	m_arrayTickNames.AddTickName(PasswordTicks::EditEposData, "Edit Epos databases");
	m_arrayTickNames.AddTickName(PasswordTicks::EditPluRecords, "Edit Plu database");
	m_arrayTickNames.AddTickName(PasswordTicks::EditDepartment, "Edit department database");
	m_arrayTickNames.AddTickName(PasswordTicks::EditGroup, "Edit department groups");
	m_arrayTickNames.AddTickName(PasswordTicks::EditTax, EcrmanOptions.GetTaxString("Edit %T rates table"));
	m_arrayTickNames.AddTickName(PasswordTicks::EditStockData, "Edit stock information");
	m_arrayTickNames.AddTickName(PasswordTicks::EditStockRecords, "Edit stock database");
	m_arrayTickNames.AddTickName(PasswordTicks::EditDeliveries, "Process adhoc deliveries");
	m_arrayTickNames.AddTickName(PasswordTicks::EditTransfers, "Process stock transfers");
	m_arrayTickNames.AddTickName(PasswordTicks::EditAdjustments, "Process stock adjustments");
	m_arrayTickNames.AddTickName(PasswordTicks::EditReturns, "Process stock returns");
	m_arrayTickNames.AddTickName(PasswordTicks::EditManualSales, "Process stock manual sales");
	m_arrayTickNames.AddTickName(PasswordTicks::EditOpening, "Process stock opening levels");
	m_arrayTickNames.AddTickName(PasswordTicks::EditReorder, "Process stock reorder levels");
	m_arrayTickNames.AddTickName(PasswordTicks::EditKeyboards, "Manage Epos keyboards");
	m_arrayTickNames.AddTickName(PasswordTicks::EditMixMatch, "Edit mix match database");
	m_arrayTickNames.AddTickName(PasswordTicks::EditPayment, "Edit payment method table");
	m_arrayTickNames.AddTickName(PasswordTicks::EditModifier, "Edit modifier table");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsPluApparent, "View Plu apparent stock reports");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsPluDelivery, "View Plu delivery audit reports");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminPluDelivery, "Delete Plu delivery log");
	m_arrayTickNames.AddTickName(PasswordTicks::HousekeepingPluApparent, "Clear Plu apparent stock levels");
	m_arrayTickNames.AddTickName(PasswordTicks::Stocktaking, "Enable Stocktaking");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingNew, "Create new stocktakes");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingEdit, "Change settings for existing stocktakes");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingLevels, "Enter closing stock levels");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingReports, "View stocktaking reports");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingFinalise, "Finalise existing stocktakes");
	m_arrayTickNames.AddTickName(PasswordTicks::StocktakingAbandon, "Abandon existing stocktakes");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminStockAudits, "Delete stock audit files");
	m_arrayTickNames.AddTickName(PasswordTicks::FileImportWizard, "Import data transfer wizard files");
	m_arrayTickNames.AddTickName(PasswordTicks::EditAnalysisCategory, "Edit analysis categories");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenu, "Enable Tasks menu");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuAdhoc, "Run adhoc tasks");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset1, "Run preset task 1");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset2, "Run preset task 2");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset3, "Run preset task 3");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset4, "Run preset task 4");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset5, "Run preset task 5");
	m_arrayTickNames.AddTickName(PasswordTicks::EditServerNames, "Edit server names");
	m_arrayTickNames.AddTickName(PasswordTicks::EditServerDetails, "Edit server details");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminSuppliers, "Delete suppliers");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminSalesXXX, "Retry sales exceptions");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsPluHistory, "View Plu sales history reports");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset6, "Run preset task 6");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset7, "Run preset task 7");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset8, "Run preset task 8");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset9, "Run preset task 9");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset10, "Run preset task 10");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset11, "Run preset task 11");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset12, "Run preset task 12");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset13, "Run preset task 13");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset14, "Run preset task 14");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset15, "Run preset task 15");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset16, "Run preset task 16");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset17, "Run preset task 17");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset18, "Run preset task 18");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset19, "Run preset task 19");
	m_arrayTickNames.AddTickName(PasswordTicks::TasksMenuPreset20, "Run preset task 20");
	m_arrayTickNames.AddTickName(PasswordTicks::EditTimePlan, "Edit time plan database");
	m_arrayTickNames.AddTickName(PasswordTicks::AdminBarcode, "Delete unlinked barcodes");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsWorking, "View time & attendance reports");
	m_arrayTickNames.AddTickName(PasswordTicks::PluChangesTask, "Run task after Plu changes");
	m_arrayTickNames.AddTickName(PasswordTicks::PluAddStock, "Add stock in Plu database");
	m_arrayTickNames.AddTickName(PasswordTicks::PluClearStock, "Clear stock in Plu database");
	m_arrayTickNames.AddTickName(PasswordTicks::PluDeliverStock, "Deliver stock in Plu database");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuSystem, "Setup system locations");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuComms, "Setup communications");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuBarcodeEntry, "Setup barcode entry table");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuLabels, "Setup label formats");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuTexts, "Setup system texts");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuOptions, "Setup system options");
	m_arrayTickNames.AddTickName(PasswordTicks::Loyalty, "Enable loyalty link");
	m_arrayTickNames.AddTickName(PasswordTicks::Smartpay, "Enable smartpay link");
	m_arrayTickNames.AddTickName(PasswordTicks::PMS, "Manage Guest Accounts");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSOptions, "Setup Guest Account options");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSManualSales, "Edit manual sales");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSManualDelete, "Delete manual sales");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSPayments, "Edit manual payments");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSInvoices, "Print invoices");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSCheckIn, "Process Check In");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSCheckOut, "Process Check Out");
	m_arrayTickNames.AddTickName(PasswordTicks::EditPaidInOutText, "Edit paid in / out texts");
	m_arrayTickNames.AddTickName(PasswordTicks::EditCustomers, "Edit customer information");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsCustomer, "View customer transaction history");
	m_arrayTickNames.AddTickName(PasswordTicks::UnitechEnable, "Link to Unitech data collector");
	m_arrayTickNames.AddTickName(PasswordTicks::UnitechSetup, "Setup Unitech data collector options");
	m_arrayTickNames.AddTickName(PasswordTicks::UnitechPluDownload, "Download Plu files to data collector");
	m_arrayTickNames.AddTickName(PasswordTicks::UnitechPluUpload, "Upload Plu files from data collector");
	m_arrayTickNames.AddTickName(PasswordTicks::UnitechStockUpload, "Upload Stock files from data collector");
	m_arrayTickNames.AddTickName(PasswordTicks::PMSChangeRoomRate, "Change room rate for bookings");
	m_arrayTickNames.AddTickName(PasswordTicks::ReportsStatement, "View customer statements");
	m_arrayTickNames.AddTickName(PasswordTicks::FileBackupStatus, "View backup results and next backup time");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuBillTexts, "Setup bill / receipt texts");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuNetworkResources, "Setup folder passwords");
	m_arrayTickNames.AddTickName(PasswordTicks::EditAllergen, "Edit allergens");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuFunction, "Setup functions");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuOPGroup, "Setup order printer groups");
	m_arrayTickNames.AddTickName(PasswordTicks::EditServerPresets, "Edit server presets");
	m_arrayTickNames.AddTickName(PasswordTicks::EditActiveStock, "Edit stockpoint item set");
	m_arrayTickNames.AddTickName(PasswordTicks::EditCustomersDelDay, "Edit customer auto delete date");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuTableNames, "Setup table names and groups");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuMacros, "Setup macros");
	m_arrayTickNames.AddTickName(PasswordTicks::SetupMenuReasons, "Setup reason texts");
	
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		m_arrayTickNames.AddTickName(PasswordTicks::EditWalletStampRule, "Edit purchase controls");
		break;

	case LOYALTY_TYPE_LOY_V4:
		m_arrayTickNames.AddTickName(PasswordTicks::EditWalletStampRule, "Edit stamp offers");
		break;
	}
}
	
/**********************************************************************/

const char* CPasswordCSVArray::GetTickName ( CPasswordTickInfo info )
{
	m_strTickName = "";
	for (int n = 0; n <= info.m_nIndent; n++)
	{
		m_strTickName += "   ";
	}

	m_strTickName += m_arrayTickNames.GetTickName( info.m_nTickNo );

	return m_strTickName;
}

/**********************************************************************/

const char* CPasswordCSVArray::GetMasterPassword()
{
	m_strDisplayPassword = "";

	if (GetSize() > 0)
	{
		m_strDisplayPassword = GetPassword(0);
	}

	return m_strDisplayPassword;
}

/**********************************************************************/

void CPasswordCSVArray::SetMasterPassword( const char* szPassword )
{
	if ( GetSize() > 0 )
	{
		CPasswordCSVRecord PasswordRecord;
		GetAt ( 0, PasswordRecord );
		PasswordRecord.SetPassword ( szPassword );
		SetAt ( 0, PasswordRecord );
	}
}

/**********************************************************************/

void CPasswordCSVArray::Code(CString& strBuf)
{
	for (int i = 0; i < strBuf.GetLength(); i++)
	{
		strBuf.SetAt(i, (strBuf.GetAt(i) ^ ((i % 10) + 1)));
	}
}

/**********************************************************************/

void CPasswordCSVArray::RememberSettings()
{
	m_arrayMemory.RemoveAll();

	for (int n = 0; n < GetSize(); n++)
	{
		m_arrayMemory.Add(GetAt(n));
	}
}

/**********************************************************************/

void CPasswordCSVArray::RestoreSettings()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < m_arrayMemory.GetSize(); n++ )
	{
		CPasswordCSVRecord Password;
		CCSV csv( m_arrayMemory.GetAt(n) );
		Password.ConvertFromCSV( csv );
		Add( Password );
	}
}

/**********************************************************************/

bool CPasswordCSVArray::CheckLocationAccessSystem()
{
	return ( m_nLocationAccessType == LOCATION_ACCESS_SYSTEM );
}

/**********************************************************************/

bool CPasswordCSVArray::CheckLocationAccessDbIdx( int nDbIdx )
{
	switch( m_nLocationAccessType )
	{
	case LOCATION_ACCESS_SYSTEM:
		return TRUE;

	case LOCATION_ACCESS_DB:
	case LOCATION_ACCESS_LOC:
		return ( nDbIdx == m_nLocationAccessDbIdx );
		break;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPasswordCSVArray::CheckLocationAccessLocIdx( int nLocIdx )
{
	switch( m_nLocationAccessType )
	{
	case LOCATION_ACCESS_SYSTEM:
		return TRUE;

	case LOCATION_ACCESS_DB:
		return ( dbLocation.GetDbNo( nLocIdx ) == dbDatabase.GetDbNo( m_nLocationAccessDbIdx ) );

	case LOCATION_ACCESS_LOC:
		return ( nLocIdx == m_nLocationAccessLocIdx );

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPasswordCSVArray::CheckLocationAccessSpIdx( int nSpIdx )
{
#ifndef STOCKMAN_SYSTEM
	return FALSE;
#else
	switch( m_nLocationAccessType )
	{
	case LOCATION_ACCESS_SYSTEM:
		return TRUE;

	case LOCATION_ACCESS_DB:
		return ( dbStockpoint.GetDbNo( nSpIdx ) == dbDatabase.GetDbNo( m_nLocationAccessDbIdx ) );

	case LOCATION_ACCESS_LOC:
		return ( nSpIdx == m_nLocationAccessSpIdx );

	default:
		return FALSE;
	}
#endif
}

/**********************************************************************/

bool CPasswordCSVArray::GetUnitechPluEnable()
{
	if ( GetEnable( PasswordTicks::DatabaseMenu) == FALSE )
		return FALSE;

	if ( GetEnable( PasswordTicks::UnitechEnable) == FALSE )
		return FALSE;

	if ( GetEnable( PasswordTicks::UnitechPluDownload) == TRUE )
		return TRUE;

	//if ( GetEnable( PasswordTicks::UnitechPluUpload) == TRUE )
	//	return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CPasswordCSVArray::GetUnitechStockEnable()
{
	if ( GetEnable( PasswordTicks::DatabaseMenu) == FALSE )
		return FALSE;

	if ( GetEnable( PasswordTicks::UnitechEnable) == FALSE )
		return FALSE;

	if ( GetEnable( PasswordTicks::UnitechStockUpload) == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

int CPasswordCSVArray::GetPluFilterPresetNo(int nDbIdx)
{
	int nPresetNo = 0;
	int nDbNo = dbDatabase.GetDbNo(nDbIdx);

	CCSV csv(GetPluFilter());

	for (int n = 0; n < csv.GetSize() - 1; n += 2)
	{
		if (csv.GetInt(n) == nDbNo)
		{
			nPresetNo = csv.GetInt(n + 1);
			break;
		}
	}

	return nPresetNo;
}

/**********************************************************************/
