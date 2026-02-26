#pragma once
/**********************************************************************/
#include "PMSDefines.h"
/**********************************************************************/
#define SYSTEMFILE_VERSION 1
/**********************************************************************/
#define LOYALTY_TYPE_NONE 0
#define LOYALTY_TYPE_UNKNOWN 1
#define LOYALTY_TYPE_LOY_V2 2
#define LOYALTY_TYPE_LOY_V3 3
#define LOYALTY_TYPE_SMP_V1 4
#define LOYALTY_TYPE_SMP_V2 5
#define LOYALTY_TYPE_LOY_V4 6
#define LOYALTY_TYPE_SMP_V4 7
/**********************************************************************/
#define ECRLITE_TYPE_NONE 0
#define ECRLITE_TYPE_NEW 1
/**********************************************************************/

class CSysInfo 
{
public:
	CSysInfo();
	bool Read();

public:
	int GetEcrLiteSystemType();
	bool IsEcrLiteSystem();
	CString GetEcrLiteVersionDetail();
	
public:
	bool IsStockSystem();
	bool IsIDraughtExportSystem();
	bool IsSageExportSystem();
	bool IsRetailStockSystem();
	bool IsPosTraySystem();
	bool IsPosTrayNoTaskSystem();
	bool IsPosTrayTimeAndAttendanceSystem();
	bool IsSPOSSMTPServerSystem();
	bool IsPosTraySocketSystem();
	bool IsSportsBookerSystem();
	bool IsSmartEntertainmentSystem();
	
	void SetPosTrayStockSystemFlag( bool b ){ m_bIsPosTrayStockSystem = b; }

	bool IsPMSSystem();
	int GetMaxPMSRooms();

	int GetMaxPresetTasks();

	bool Write();

	void RememberSettings();
	void RestoreSettings();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void SetDefaults();

	int GetDPQty() { return m_nDPQty; }
	int GetDPValue() { return m_nDPValue; }
	int GetCommsThreadSpeed(){ return m_nCommsThreadSpeed; }
	bool GetProcessCashRSPBufferFlag(){ return m_bProcessCashRSPBuffer; }
	
	void SetDPQty ( int nDP );
	void SetDPValue ( int nDP );
	void SetCommsThreadSpeed( int n );
	void SetProcessCashRSPBufferFlag( bool bFlag ){ m_bProcessCashRSPBuffer = bFlag; }
		
	//NOT PART OF THE CSV LINE
	int GetLoyaltyOrSmartPayType();
	void SetLoyaltyOrSmartPayType( int n );
	bool GotLoyaltyLink();
	bool GotSmartPayLink();
	bool GotLoyaltyOrSmartPayLink(){ return ( GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_NONE ); }

	int GetCustomTaxFilter() { return m_nCustomTaxFilter; }
	const char* GetCustomTaxFilterString() { return m_strCustomTaxFilter; }
	void SetCustomTaxFilter( int n ); 
	
	int GetCustomPriceLevelFilter() { return m_nCustomPriceLevelFilter; }
	const char* GetCustomPriceLevelFilterString() { return m_strCustomPriceLevelFilter; }
	void SetCustomPriceLevelFilter( int n );

	int GetCustomModifierFilter() { return m_nCustomModifierFilter; }
	const char* GetCustomModifierFilterString() { return m_strCustomModifierFilter; }
	void SetCustomModifierFilter( int n ); 

	int GetDataPathNo(){ return m_nDataPathNo; }
	void SetDataPathNo( int n ){ m_nDataPathNo = n; }

	int GetMaxBasePluLen( bool bForceRefresh = FALSE );
	bool GetAltFilenamesIfNoModifiersFlag() { return m_bAltFilenamesIfNoModifiers; }
	bool IsModifiableSSPluNo( __int64 nPluNo );
	bool IsValidSSPluNo( __int64 nPluNo );
	bool IsLiteralEposPluNo( __int64 nPluNo );

	bool GetNewBarcodePluFlag(){ return m_bNewBarcodePlu; }
	void SetNewBarcodePluFlag( bool b ){ m_bNewBarcodePlu = b; }

	int GetMaxDatabases();
	int GetMaxLocations();
	int GetMaxRemoteLocations();

	void SetStackBase( DWORD dw ){ m_dwStackBase = dw; }
	void EstimateStackUsage();

	bool GetScriptModeFlag(){ return m_bScriptMode; }
	void SetScriptModeFlag( bool b ){ m_bScriptMode = b; }

	bool GetTaskModeFlag(){ return m_bTaskMode; }
	const char* GetTaskModeString(){ return m_strTaskMode; }
	void SetTaskModeString( const char* sz );
	
	void GetBetaTypeForBackup( CString& str ){ str = "0"; }

private:
	int GetExternalLink();

private:
	int m_nDPQty;
	int m_nDPValue;
	int m_nCommsThreadSpeed;
	bool m_bProcessCashRSPBuffer;
	int m_nDataPathNo;
	bool m_bScriptMode;
	bool m_bTaskMode;
	CString m_strTaskMode;
	int m_nPluMode;
	int m_nMaxBasePluLen;
	bool m_bAltFilenamesIfNoModifiers;
	bool m_bNewBarcodePlu;
	CString m_strCSVLine;

private:
	int m_nStoredDPQty;
	int m_nStoredDPValue;
	int m_nStoredCommsThreadSpeed;
	bool m_bStoredProcessCashRSPBuffer;

private:
	CString m_strCustomTaxFilter;
	CString m_strCustomPriceLevelFilter;
	CString m_strCustomModifierFilter;
	int m_nCustomTaxFilter;
	int m_nCustomPriceLevelFilter;
	int m_nCustomModifierFilter;
	int m_nLoyaltyOrSmartPayType;
	
private:
	bool m_bFatalReadError;
	bool m_bIsPosTrayStockSystem;
	DWORD m_dwStackBase;
};

/**********************************************************************/

class CCSVSysInfo : public CCSV
{
public:
	CCSVSysInfo ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	int GetDPQty() { return GetInt(0); }
	int GetDPValue() { return GetInt(1); }
	//int GetTimeout() { return GetInt(2); }
	//bool GetRemoteSalesFlag() { return GetBool(3); }
	int GetCommsThreadSpeed(){ return GetInt(4); }
	bool GetProcessCashRSPBufferFlag(){ return GetBool(5); }
};

/**********************************************************************/
extern CSysInfo SysInfo;		/* global */
/**********************************************************************/

