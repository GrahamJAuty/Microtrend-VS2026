/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxdisp.h>
#include <afxinet.h>
#include <atlbase.h>

/**********************************************************************/
//
//	If the following defines are changed:
//
//	Register.lib ( CSSAccessCode::GetProgramText() ) needs to be modified
//	SysEnable ( CSysEnableDlg::OnButtonRegister() ) needs to be modified
//	SysCreate ( CSysCreateDlg::OnInitDialog() ) needs to be modified
//	NetEnable, SysEnable, VoiceModem and WebReg need to be rebuilt
//
/**********************************************************************/

#define SYSSET_ECRMAN		1		// frozen
#define SYSSET_STOCKMAN		2		// frozen
#define SYSSET_SMARTMAN		3
#define SYSSET_SMART001		4
#define SYSSET_ECRMANLITE	5
#define SYSSET_CARDLINK		6		// centralised registry
#define SYSSET_ECRMAN5000	7
#define SYSSET_STKMAN5000	8
#define SYSSET_HEADOFFICE	9
#define SYSSET_ECRMAN3		10
#define SYSSET_STKMAN3		11
#define SYSSET_GCRLINK		12		// centralised registry
#define SYSSET_SYSENABLE		13		// centralised registry
#define SYSSET_ECRMAN4		14		// centralised registry
#define SYSSET_STKMAN4		15		// centralised registry
#define SYSSET_SMARTMAN4		16		// centralised registry
#define SYSSET_ECRMANX300	17		// centralised registry
#define SYSSET_STKMANX300	18		// centralised registry
#define SYSSET_SMARTSERVER	19		// centralised registry
#define SYSSET_ECRMANX500	20		// centralised registry
#define SYSSET_STKMANX500	21		// centralised registry
#define SYSSET_CARDINIT		22		// centralised registry
#define SYSSET_LOYALTY		23		// centralised registry
#define SYSSET_SALESMAN		24		// centralised registry
#define SYSSET_BARMAN		25		// centralised registry
#define SYSSET_ECRMAN5		26		// centralised registry
#define SYSSET_STKMAN5		27		// centralised registry
#define SYSSET_SMARTMAN5		28		// centralised registry
//#define SYSSET_ECRMANX300V2	29		// centralised registry
//#define SYSSET_STKMANX300V2	30		// centralised registry
#define SYSSET_ECRMANX500V2	31		// centralised registry
#define SYSSET_STKMANX500V2	32		// centralised registry
#define SYSSET_SMARTPAY		33		// centralised registry
#define SYSSET_KIOSKBIOLINK	34		// centralised registry
#define SYSSET_KIOSKCARDLINK	35		// centralised registry
#define SYSSET_ECRMAN6		36		// centralised registry
#define SYSSET_STKMAN6		37		// centralised registry
#define SYSSET_SMARTMAN6		38		// centralised registry
#define SYSSET_ECRMANX500V3	39		// centralised registry
#define SYSSET_STKMANX500V3	40		// centralised registry
#define SYSSET_LOYALTY2		41		// centralised registry
#define SYSSET_ECRMANSPOSV4	42		// centralised registry
#define SYSSET_STKMANSPOSV4	43		// centralised registry
#define SYSSET_SMARTPAY2	44		// centralised registry
#define SYSSET_LOYALTY3		45		// centralised registry
#define SYSSET_SMARTPAY4	46		// centralised registry
#define SYSSET_LOYALTY4		47		// centralised registry

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

struct IdentityInfo
{
public:
	CString m_strUserName = "";
	CString m_strComputerName = "";
	CString m_strSessionName = "";
	CString m_strClientName = "";
};

/**********************************************************************/

class CUpdateTokenData : public CSSIniFile
{
public:
	CUpdateTokenData();
	int GetSerialNo();
	int GetDealerCode();
	CString GetVersion();
	int GetIsUpdateFlag();

	void SetSerialNo ( int n );
	void SetDealerCode ( int n );
	void SetVersion ( const char* sz );
	void SetIsUpdateFlag ( int n );
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

class CUserData : public CSSIniFile
{
public:
	CUserData();
	CString GetField ( int nIndex );				// 0 to 5
	CString GetUserID();
	CString GetExpiry();
	CString GetExpiryDate();
	int GetMaxLocations();
	int GetSmartcardCustNo();
	bool IsSchedulerSystem();
	bool IsSmartcardSystem();
	bool IsGcr500System();
	bool IsBarmanSystem();
	bool IsPhotoIDSystem();
	bool IsStockExportSystem();
	bool IsSmartcardFormat();
	bool IsMealDealSystem();
	int GetMaxDatabases();
	int GetMaxStockPoints();
	int GetMaxRemoteLocations();
	int GetSystemType();
	bool IsRetailStockSystem();
	bool IsWetStockSystem();
	int GetMaxEcrModels();
	int GetProgramNo();
	int GetSerialNo();
	int GetMaxUsers();
	int GetMaxComms();
	int GetTLogConvert();
	int GetPMS();
	int GetExternalLink();

	void SetField ( int nIndex, const char* szText );		// 0 to 5
	void SetUserID ( const char* szText );
	void SetExpiry ( const char* szDate );
	void SetMaxLocations ( int nMax );
	void SetSmartcardCustNo ( int nCustNo );
	void SetSchedulerSystem ( bool b );
	void SetSmartcardSystem ( bool b );
	void SetGcr500System ( bool b );
	void SetBarmanSystem ( bool b );
	void SetPhotoIDSystem ( bool b );
	void SetStockExportSystem ( bool b );
	void SetSmartcardFormat ( bool b );
	void SetMealDealSystem ( bool b );
	void SetMaxDatabases ( int nMax );
	void SetMaxStockPoints ( int nMax );
	void SetMaxRemoteLocations ( int nMax );
	void SetSystemType ( int n );
	void SetMaxEcrModels ( int nMax );
	void SetProgramNo ( int n );
	void SetSerialNo ( int n );
	void SetMaxUsers ( int nMax );
	void SetMaxComms ( int nMax );
	void SetTLogConvert ( int n );
	void SetPMS ( int n );
	void SetExternalLink ( int n );
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

// expiry date (ddmmyy) 000000=registered 999999=not registered
// expiry type (0=system, 1=dealer)

class CRegData : public CSSIniFile
{
public:
	CRegData();
	void SetSerialNo ( int nSerialNo );
	void SetSystemExpiry ( const char* szDate );		// ddmmyy
	void SetSystemExpiry ( int nDays );
	void SetDealerExpiry ( const char* szDate );		// ddmmyy
	void SetDealerExpiry ( int nDays );
	void SetDealerPhone ( const char* szPhone );

	CString GetExpiryDate();
	int GetExpiryType();
	int GetSerialNo();
	CString GetDealerPhone();

private:
	void SetExpiryDate ( const char* szDate );
	void SetExpiryType ( int nType );
	void SetExpiry ( int nDays, int nType );
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

class CLockArray : public CObArray
{
public:
	~CLockArray();
	bool LockUser ( const char* szFilename, IdentityInfo& UserIdentityInfo, const char* szPCIdHost, const char* szPCIdClient );
	void UnlockAllExcept ( const char* szFilename );
	void UnlockOtherProgram();
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

class CCommsLockArray : public CObArray
{
public:
	~CCommsLockArray();
	bool Lock ( const char* szFilename );
	void Unlock ( const char* szFilename );
};

/**********************************************************************/
/*	private structure for CSysset use only!						*/
/**********************************************************************/

class CLegacyEmbeddedSerialNo
{
public:
	CLegacyEmbeddedSerialNo();
	bool SetData ( const char* szData );
	CString GetData();
	int GetSerialNo() { return m_nSerialNo; }
	void SetSerialNo ( int nSerialNo );

public:
	static const char* GetMatchString();

private:
	int m_nSerialNo = 0;
};

/**********************************************************************/

class CSysset
{
public:
	CSysset();
	~CSysset();

	// N.B. TestRegistration() must be called before using any other functions in this class.
	//      DO NOT CONTINUE if FALSE returned.

	bool TestRegistrationMainExe ( int nProgram, CWnd* pParent, bool& bCanCreatePCId, const char* szVersion, const char* szMinTokenDate, bool bSpawned=FALSE, int nDataPathNo=0 );
	bool TestRegistrationModule (int nProgram, CWnd* pParent, bool& bCanCreatePCId, bool bSpawned = FALSE, int nDataPathNo = 0);

	bool TestOtherProgram ( int nProgram );
	CString DisplayLicenceDetails ( const char* szGroup, const char* szText, int nCodeLen, CWnd* pParent );
	void DisplayWatermark ( CDC* pDC, CWnd* pWnd );
	void DisplayUserList();
	void SaveWindowSize ( CWnd* pWnd, const char* szId="" );		// override MainFrame DestroyWindow()
	void RestoreWindowSize ( CWnd* pWnd, const char* szId="" );		// after MainFrame has been created
	CString GetVersion ( const char* szVersion );
	CString GetError() { return m_strError; }

	// N.B. Spawned programs must not call LockAllUsers() and UnlockAllUsers() functions.
	//      Spawned programs share a common temp folder Pnn\Temp00

	bool LockAllUsers();
	void UnlockAllUsers();

	bool LockOtherProgram( CString strMaster );
	void UnlockOtherProgram();

	int RequestComms();				// returns handle (0=error)
	void ReleaseComms ( int nHandle );

	CString GetDataPath();				// "<DataPath>\"
	CString GetProgramPath();			// "<DataPath>\Pnn"
	CString GetPCPathHost();			// "<DataPath>\Pnn\PCnnnnnnnnnn"
	CString GetPCPathClient();			// "<DataPath>\Pnn\PCnnnnnnnnnn"
	CString GetTempPath();				// "<DataPath>\Pnn\Tempnn"
	CString GetSyssetProgramPath();		// "<DataPath>\Sysset\Pnn"

	CString GetComputerName() { return m_IdentityInfo.m_strComputerName; }
	int GetInstanceNo() { return m_nInstanceNo; }	// range 1 to max users

	int ConvertStringVersionToInt(const CString strVersion); //e.g. V4.23.C05 => 40231205
	CString ConvertIntVersionToString(const int nVersion); //e.g. 40231205 => V4.23.C05
	
	int GetPCIdHost() { return m_nPCIdHost; }
	int GetPCIdClient() { return m_nPCIdClient; }
	
	CString GetField ( int n );	// 0 to 5
	CString GetUserID();
	int GetMaxLocations();
	int GetMaxRemoteLocations();
	int GetMaxDatabases();
	int GetMaxStockPoints();
	int GetSmartcardCustNo();
	int GetMaxEcrModels();
	int GetSerialNo();
	int GetRegTokenOrLegacyEmbeddedSerialNo(int nProgramNo, CString strPath, CString strVersion);
	int GetMaxUsers();
	int GetMaxComms();
	int GetSystemType();
	int GetTLogConvert();
	int GetPMS();
	int GetExternalLink();

	bool IsSchedulerSystem();
	bool IsSmartcardSystem();
	bool IsGcr500System();
	bool IsRetailStockSystem();
	bool IsWetStockSystem();
	bool IsBarmanSystem();
	bool IsPhotoIDSystem();
	bool IsStockExportSystem();
	bool IsSmartcardFormat();
	bool IsMealDealSystem();

	bool IsNobbledDemo();
	bool GetInternalNobbledDemoFlag() { return m_bNobbledDemo; }

	static CString CodeMessage ( const char* szText );
	static CString DecodeMessage ( const char* szText );

	// This function is used by TST to create user CD

	CString GetUserData ( const char* szIPAddress, const char* szSecurityCode, const char* szSource, const char* szFields, const char* szIniFilename );	// returns error message, "" = no error

	// N.B. The following function should not normally be used, as
	//      it allows one program to access another program's data area.

	CString GetProgramFolder ( int nProgram );	// "Pnn"

	// N.B. The following functions are for CSysset use only!

	bool SetPCId( bool& bCanCreatePCId );		// used by SysCreate.exe
	bool CreateSystemFolders ( int nProgram );	// used by SysCreate.exe
	CString GetShareFilename ( int nInstNo );	// used by CSyssetDlg4::DisplayUsers()
	CString GetProgUserFilename(int nInstNo);	// used by CSyssetDlg4::DisplayUsers()
	bool RegisterOnline ( const char* szAccessCode, CString& strUserCode );

public:
	void CreateProgUserFile(CString strUserName);

private:
	CString GetPCFolder ( int nPCId );			// "PCnnnnnnnnnn"
	CString GetTempFolder ( int nInstNo );		// "Tempnn"

	CString GetSyssetPath();					// "<DataPath>\Sysset"
	CString GetSyssetComputerPath();			// "<DataPath>\Sysset\PC"
	CString GetSyssetPCPathHost();				// "<DataPath>\Sysset\PCnnnnnnnnnn"
	CString GetSyssetPCPathClient();			// "<DataPath>\Sysset\PCnnnnnnnnnn"
	CString GetSyssetWindowSizePath();			// "<DataPath>\Sysset\PCnnnnnnnnnn\ws"

	CString GetCommsFilename ( int nHandle );	// "<DataPath>\Sysset\Pnn\Cnn"
	CString GetRegFilename();				// "<DataPath>\Sysset\Pnn\R01"
	CString GetUserFilename();				// "<DataPath>\Sysset\Pnn\R02"
	CString GetUpdateTokenFilename();		// "<DataPath>\Sysset\Pnn\R03"

	bool HasSoftwareExpired();
	bool RegisterSoftware ( CWnd* pParent );
	bool SetDealerExpiry ( CWnd* pParent );
	bool TestMaxUsers();
	bool CreateTempFolder();
	bool LoadUpdateFile ( int nSerialNo, const char* szUserFilename );
	CString UpdateOnline ( const char* szSecurityCode, const char* szFilename );

	int GetPCIdFromComputerName(CString strComputerName, bool bLegacy);
	bool WritePCIdFile(bool bClient);

private:
	int GetDealerCode();
	void CheckDemoToken(bool& bWantDemoToken, bool& bGotDemoToken);
	void MakeInternalUpdateToken(CString strVersion, CString strUpdateTokenFilename);
	int CheckDealerUpdateToken(int nDealerCode, const char* szMinTokenDate);
	bool ValidateUpdateToken(CString strVersion, CString strMinTokenDate, bool bExistUpdateToken, CString strUpdateTokenFilename);
	bool TestRegistrationInternal(int nProgram, CWnd* pParent, bool& bCanCreatePCId, const char* szVersion, const char* szMinTokenDate, bool bSpawned, int nDataPathNo, bool bWaiveUpdateToken, bool bAllowRegToken);
	
private:
	bool MayHaveLegacyEmbeddedSerialNo();
	void CheckRegToken(int& nEmbeddedSerialNo, CString strPath, CString strVersion, CString& strRegTokenError);
	void PrepareRegTokenGeneralError(CString& strError, CString strTokenFilename, const CString strReason);
	void PrepareRegTokenInvalidDataError(CString& strError, CString strTokenFilename, int nErrorNum);
	bool TestNumeric(const CString str);

private:
	CUserData m_UserData;
	CRegData m_RegData;
	CString m_strError;
	CString m_strDataPath;
	IdentityInfo m_IdentityInfo;
	CLockArray m_LockArray;
	CCommsLockArray m_CommsLockArray;
	bool m_bUsersLocked;
	bool m_bSpawned;
	int m_nVersionType;
	int m_nProgram;
	int m_nInstanceNo;
	int m_nPCIdHost = 0;
	int m_nPCIdClient = 0;

private:
	int m_nDemoTokenLocCount;
	int m_nDemoTokenDbCount;
	int m_nDemoTokenUserCount;
	bool m_bNobbledDemo;

private:
	bool ConvertOldToNew();
};

/**********************************************************************/

class CLegacyEmbeddedSerialNoFind : public CLegacyEmbeddedSerialNo
{
public:
	CLegacyEmbeddedSerialNoFind ( const char* szExeFilename );
	bool LocateDetails();
	bool WriteDetails();

private:
	CString m_strExeFilename;
	long m_lOffset;
};

/**********************************************************************/

class CMessageTexts
{
public:
	static CString SoftwareNotRegistered() { return "Your software has not been registered"; }
	static CString CannotWriteRegFile() { return "Cannot write registration file"; }
	static CString CannotWritePCId() { return "Cannot write PCId"; }
	static CString DataPathNotExist() { return "Data path does not exist"; }
	static CString DataPathInvalid() { return "Invalid Data path"; }
	static CString UnregisteredSoftware() { return "Unregistered Software"; }
	static CString UserFileCorrupted() { return "User file corrupted"; }
	static CString RegistrationFileCorrupted() { return "Registration file corrupted"; }
	static CString ContactDealerAccessCode() { return "Contact your Dealer quoting the access code"; }
	static CString InternetWait() { return "Connecting to internet - Please wait"; }
};

/**********************************************************************/

class CFilenameTexts
{
public:
	static CString RegFilename() { return "R01"; }
	static CString UserFilename() { return "R02"; }
	static CString UpdateTokenFilename() { return "R03"; }
	static CString OldRegFilename() { return "SysData"; }
	static CString Sysset() { return "Sysset"; }
	static CString UpdateFilename() { return "u%6.6d.dat"; }
	static CString DatapathFilename() { return "datapath.dat"; }
};

/**********************************************************************/

class PasswordTexts
{
public:
	static CString Update() { return "update"; }
	static CString Dealer() { return "redsky6"; }
};

/**********************************************************************/

extern const char* szVERSION_SYSSET3;

/**********************************************************************/
