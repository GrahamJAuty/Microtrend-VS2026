#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define PASSWORD_FLAGS 192
#define MAX_PASSWORDS 20
/**********************************************************************/
#define LOCATION_ACCESS_NONE 0
#define LOCATION_ACCESS_SYSTEM 1
#define LOCATION_ACCESS_DB 2
#define LOCATION_ACCESS_LOC 3
/**********************************************************************/

struct CPasswordTickInfo
{
public:
	int m_nTickNo;
	int m_nIndent;
	int m_nLink1;
	int m_nLink2;
};

/**********************************************************************/

class CPasswordTickNameArray
{
public:
	void RemoveAll(){ m_arrayTickNames.RemoveAll(); }
	void AddTickName( const CPasswordTickInfo& info, const char* szName );

public:
	const char* GetTickName( int nTickNo );

private:
	CReportConsolidationArray<CSortedStringByInt> m_arrayTickNames;
	CString m_strTickName;
};

/**********************************************************************/

namespace PasswordTicks
{
	const CPasswordTickInfo FileCommsLog = { 0, 0, -1, -1 };
	const CPasswordTickInfo FileBackup = { 1, 0, -1, -1 };
	const CPasswordTickInfo FileRestore = { 2, 0, -1, -1 };
	const CPasswordTickInfo FileBackupLog = { 3, 0, -1, -1 };

	const CPasswordTickInfo DatabaseMenu = { 4, 0, -1, -1 };
	const CPasswordTickInfo DatabaseOrderProcess = { 5, 1, 4, -1 };
	//6 was used in earlier release
	const CPasswordTickInfo CommsDownloads = { 7, 0, -1, -1 };
	const CPasswordTickInfo CommsBatchSales = { 8, 0, -1, -1 };

	const CPasswordTickInfo ReportsMenu = { 9, 0, -1, -1 };
	const CPasswordTickInfo ReportsEpos = { 10, 1, 9, -1 };
	const CPasswordTickInfo ReportsStock = { 11, 1, 9, -1 };
	const CPasswordTickInfo ReportsAudit = { 12, 1, 9, -1 };
	//13 was used in earlier release
	const CPasswordTickInfo SetupMenu = { 14, 0, -1, -1 };

	const CPasswordTickInfo AdminMenu = { 15, 0, -1, -1 };
	const CPasswordTickInfo AdminEposSales = { 16, 1, 15, -1 };
	const CPasswordTickInfo AdminStockSales = { 17, 1, 15, -1 };
	const CPasswordTickInfo AdminPluStock = { 18, 1, 15, -1 };
	const CPasswordTickInfo AdminOrders = { 19, 1, 15, -1 };
	const CPasswordTickInfo AdminOrderAudits = { 20, 1, 15, -1 };
	const CPasswordTickInfo AdminStockChange = { 21, 1, 15, -1 };

	const CPasswordTickInfo EditEposData = { 22, 1, 4, -1 };
	const CPasswordTickInfo EditPluRecords = { 23, 2, 4, 22 };
	const CPasswordTickInfo EditDepartment = { 24, 2, 4, 22 };
	const CPasswordTickInfo EditGroup = { 25, 2, 4, 22 };
	const CPasswordTickInfo EditTax = { 26, 2, 4, 22 };

	const CPasswordTickInfo EditStockData = { 27, 1, 4, -1 };
	const CPasswordTickInfo EditStockRecords = { 28, 2, 4, 27 };
	const CPasswordTickInfo EditDeliveries = { 29, 2, 4, 27 };
	const CPasswordTickInfo EditTransfers = { 30, 2, 4, 27 };
	const CPasswordTickInfo EditAdjustments = { 31, 2, 4, 27 };
	const CPasswordTickInfo EditReturns = { 32, 2, 4, 27 };
	const CPasswordTickInfo EditManualSales = { 33, 2, 4, 27 };
	//34 was used in earlier release
	const CPasswordTickInfo EditOpening = { 35, 2, 4, 27 };
	const CPasswordTickInfo EditReorder = { 36, 2, 4, 27 };

	const CPasswordTickInfo EditKeyboards = { 37, 1, 4, -1 };

	const CPasswordTickInfo EditMixMatch = { 38, 2, 4, 22 };
	const CPasswordTickInfo EditPayment = { 39, 2, 4, 22 };
	const CPasswordTickInfo EditModifier = { 40, 2, 4, 22 };

	const CPasswordTickInfo ReportsPluApparent = { 41, 1, 9, -1 };
	const CPasswordTickInfo ReportsPluDelivery = { 42, 1, 9, -1 };

	const CPasswordTickInfo AdminPluDelivery = { 43, 1, 15, -1 };

	const CPasswordTickInfo HousekeepingPluApparent = { 44, 2, 9, 41 };

	const CPasswordTickInfo Stocktaking = { 45, 0, -1, -1 };
	const CPasswordTickInfo StocktakingNew = { 46, 1, 45, -1 };
	const CPasswordTickInfo StocktakingEdit = { 47, 1, 45, -1 };
	const CPasswordTickInfo StocktakingLevels = { 48, 1, 45, -1 };
	const CPasswordTickInfo StocktakingReports = { 49, 1, 45, -1 };
	const CPasswordTickInfo StocktakingFinalise = { 50, 1, 45, -1 };
	const CPasswordTickInfo StocktakingAbandon = { 51, 1, 45, -1 };

	const CPasswordTickInfo AdminStockAudits = { 52, 1, 15, -1 };

	const CPasswordTickInfo FileImportWizard = { 53, 0, -1, -1 };

	const CPasswordTickInfo EditAnalysisCategory = { 54, 2, 4, 22 };

	const CPasswordTickInfo TasksMenu = { 55, 0, -1, -1 };
	const CPasswordTickInfo TasksMenuAdhoc = { 56, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset1 = { 57, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset2 = { 58, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset3 = { 59, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset4 = { 60, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset5 = { 61, 1, 55, -1 };

	const CPasswordTickInfo EditServerNames = { 62, 2, 4, 22 };
	const CPasswordTickInfo EditServerDetails = { 63, 2, 4, 22 };

	const CPasswordTickInfo AdminSuppliers = { 64, 1, 15, -1 };
	const CPasswordTickInfo AdminSalesXXX = { 65, 1, 15, -1 };

	const CPasswordTickInfo ReportsPluHistory = { 66, 1, 9, -1 };

	const CPasswordTickInfo TasksMenuPreset6 = { 67, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset7 = { 68, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset8 = { 69, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset9 = { 70, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset10 = { 71, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset11 = { 72, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset12 = { 73, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset13 = { 74, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset14 = { 75, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset15 = { 76, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset16 = { 77, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset17 = { 78, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset18 = { 79, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset19 = { 80, 1, 55, -1 };
	const CPasswordTickInfo TasksMenuPreset20 = { 81, 1, 55, -1 };

	const CPasswordTickInfo EditTimePlan = { 82, 2, 4, 22 };

	const CPasswordTickInfo AdminBarcode = { 83, 1, 15, -1 };
	const CPasswordTickInfo ReportsWorking = { 84, 1, 9, -1 };

	const CPasswordTickInfo PluChangesTask = { 85, 1, 55, -1 };
	const CPasswordTickInfo PluAddStock = { 86, 2, 4, 22 };
	const CPasswordTickInfo PluClearStock = { 87, 2, 4, 22 };
	const CPasswordTickInfo PluDeliverStock = { 88, 2, 4, 22 };
	//89 reserved for location access type
	//90 to 92 reserved for database number
	//93 to 95 reserved for network number
	//96 to 98 reserved for location number
	const CPasswordTickInfo SetupMenuSystem = { 99, 1, 14, -1 };
	const CPasswordTickInfo SetupMenuComms = { 100, 1, 14, -1 };
	const CPasswordTickInfo SetupMenuBarcodeEntry = { 101, 1, 14, -1 };
	const CPasswordTickInfo SetupMenuLabels = { 102, 1, 14, -1 };
	const CPasswordTickInfo SetupMenuTexts = { 103, 1, 14, -1 };
	const CPasswordTickInfo SetupMenuOptions = { 104, 1, 14, -1 };
	const CPasswordTickInfo Loyalty = { 105, 0, -1, -1 };
	const CPasswordTickInfo Smartpay = { 106, 0, -1, -1 };

	const CPasswordTickInfo PMS = { 107, 0, -1, -1 };
	const CPasswordTickInfo PMSOptions = { 108, 1, 107, -1 };
	const CPasswordTickInfo PMSManualSales = { 109, 1, 107, -1 };
	const CPasswordTickInfo PMSManualDelete = { 110, 2, 107, 109 };
	const CPasswordTickInfo PMSPayments = { 111, 1, 107, -1 };
	const CPasswordTickInfo PMSInvoices = { 112, 1, 107, -1 };
	const CPasswordTickInfo PMSCheckIn = { 113, 1, 107, -1 };
	const CPasswordTickInfo PMSCheckOut = { 114, 1, 107, -1 };

	const CPasswordTickInfo EditPaidInOutText = { 115, 2, 4, 22 };
	const CPasswordTickInfo EditCustomers = { 116, 2, 4, 22 };

	const CPasswordTickInfo ReportsCustomer = { 117, 1, 9, -1 };

	const CPasswordTickInfo UnitechSetup = { 118, 1, 14, -1 };
	const CPasswordTickInfo UnitechEnable = { 119, 1, 4, -1 };
	const CPasswordTickInfo UnitechPluDownload = { 120, 2, 4, 119 };
	const CPasswordTickInfo UnitechPluUpload = { 121, 2, 4, 119 };
	const CPasswordTickInfo UnitechStockUpload = { 122, 2, 4, 119 };

	const CPasswordTickInfo PMSChangeRoomRate = { 123, 1, 107, -1 };

	const CPasswordTickInfo ReportsStatement = { 124, 1, 9, -1 };

	const CPasswordTickInfo FileBackupStatus = { 125, 0, -1, -1 };

	const CPasswordTickInfo SetupMenuBillTexts = { 126, 2, 14, 103 };
	const CPasswordTickInfo SetupMenuNetworkResources = { 127, 1, 14, -1 };

	const CPasswordTickInfo EditAllergen = { 128, 2, 4, 22 };

	const CPasswordTickInfo SetupMenuFunction = { 129, 2, 14, 103 };
	const CPasswordTickInfo SetupMenuOPGroup = { 130, 2, 14, 103 };
	const CPasswordTickInfo EditServerPresets = { 131, 2, 4, 22 };
	const CPasswordTickInfo EditActiveStock = { 132, 2, 4, 27 };

	const CPasswordTickInfo EditCustomersDelDay = { 133, 2, 4, 22 };

	const CPasswordTickInfo SetupMenuTableNames = { 134, 2, 14, 103 };
	const CPasswordTickInfo SetupMenuMacros = { 135, 2, 14, 103 };
	const CPasswordTickInfo SetupMenuReasons = { 136, 2, 14, 103 };

	const CPasswordTickInfo EditWalletStampRule = { 137, 2, 4, 22 };
};

/**********************************************************************/

class CPasswordCSVRecord : public CCSVRecord 
{
public:
	CPasswordCSVRecord();

	void ConvertToCSV ( CCSV& csv );
	void ConvertFromCSV ( CCSV& csv );

	const char* GetUserName();
	const char* GetPassword();
	const char* GetPluFilter();
	bool GetEnable ( CPasswordTickInfo info );

	void SetLocationAccess( int nType, int nIndex );
	void GetLocationAccess( int& nType, int& nDbIdx, int& nLocIdx, int& nSpIdx );
	
	void SetUserName(const char* szUserName);
	void SetPassword ( const char* szPassword );
	void SetPluFilter(const char* szFilter);
	void SetEnable ( CPasswordTickInfo info, bool bEnable );

	void GetPluFilterArray(CReportConsolidationArray<CSortedIntByInt>& arrayFilter);
	void SetPluFilterArray(CReportConsolidationArray<CSortedIntByInt>& arrayFilter);
	
	//Enable or disable all flags
	void SetAllEnables ( bool bEnable );

	//Make sure we have the right number of flags
	void CheckEnables();
		
private:
	CString m_strDisplayUserName;
	CString m_strDisplayPassword;
	CString m_strDisplayPluFilter;
	CString m_strUserName;
	CString m_strPassword;
	CString m_strPluFilter;
	CString m_strEnable;
	CString m_strCSVLine;
};

/**********************************************************************/

class CPasswordCSVArray : public CCSVArray
{
public:
	CPasswordCSVArray();
	bool Read();
	bool Write();

	void RememberSettings();
	void RestoreSettings();

	const char* GetMasterPassword();
	void SetMasterPassword( const char* szPassword );
	bool IsMasterPassword(){ return m_nPasswordIdx == 0; }
	int GetPasswordIdx(){ return m_nPasswordIdx; }

	void CheckPassword ( const char* szPassword );

	bool GetEnable ( CPasswordTickInfo info );
	
	void BuildTickNameList();
	const char* GetTickName( CPasswordTickInfo info );

	const char* GetUserNo();
	const char* GetPassword ( int nPasswordIdx );
	const char* GetPassword() { return m_strCurrentPassword; }
	const char* GetUserName ( int nPasswordIdx );
	const char* GetUserName() { return m_strCurrentUserName; }
	const char* GetPluFilter(int nPasswordIdx);
	const char* GetPluFilter() { return m_strCurrentPluFilter; }
	int GetPluFilterPresetNo(int nDbIdx);

	int GetLocationAccessType(){ return m_nLocationAccessType; }
	
	bool CheckLocationAccessSystem();
	bool CheckLocationAccessLoc(){ return m_bLocationAccessLoc; }
	bool CheckLocationAccessSp(){ return m_bLocationAccessSp; }
	
	bool CheckLocationAccessDbIdx( int nDbIdx );
	bool CheckLocationAccessLocIdx( int nLocIdx );
	bool CheckLocationAccessSpIdx( int nSpIdx );
	
	int GetLocationAccessDbIdx(){ return m_nLocationAccessDbIdx; }
	int GetLocationAccessLocIdx(){ return m_nLocationAccessLocIdx; }
	int GetLocationAccessSpIdx(){ return m_nLocationAccessSpIdx; }

	bool GetUnitechPluEnable();
	bool GetUnitechStockEnable();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }

public:
	static void Code ( CString& strBuf );
	
private:
	int m_nPasswordIdx;
	CString m_strCurrentPassword;
	CString m_strPassword;
	CString m_strDisplayPassword;
	CString m_strUserName;
	CString m_strCurrentUserName;
	CString m_strPluFilter;
	CString m_strCurrentPluFilter;
	CString m_strUserNo;

private:
	bool m_bLocationAccessLoc;
	bool m_bLocationAccessSp;
	int m_nLocationAccessType;
	int m_nLocationAccessDbIdx;
	int m_nLocationAccessLocIdx;
	int m_nLocationAccessSpIdx;

private:
	bool m_bFatalReadError;
	
private:
	CPasswordTickNameArray m_arrayTickNames;
	CString m_strTickName;	

private:
	CStringArray m_arrayMemory;
};

/**********************************************************************/
extern CPasswordCSVArray PasswordArray;
/**********************************************************************/
