#pragma once
/**********************************************************************/
#include "PMSDefines.h"
#include "PMSReportFileFinder.h"
#include "TermFileListInfo.h"
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CEposReportPMSModes
{
public:
	CEposReportPMSModes(void);

public:
	bool GetEposFlag(){ return m_bEpos; }
	bool GetWantDepositsFlag(){ return m_bWantDeposits; }
	bool GetWantCompleteFlag(){ return m_bWantComplete; }
	bool GetWantPendingFlag(){ return m_bWantPending; }
	bool GetWantActiveFlag(){ return m_bWantActive; }
	bool GetWantCancelledFlag(){ return m_bWantCancelled; }
	int GetAllowMode(){ return m_nAllowMode; }
	int GetDateMode(){ return m_nDateMode; }

public:
	void SetEposFlag( bool b ){ m_bEpos = b; }
	void SetWantDepositsFlag( bool b ){ m_bWantDeposits = b; }
	void SetWantCompleteFlag( bool b ){ m_bWantComplete = b; }
	void SetWantPendingFlag( bool b ){ m_bWantPending = b; }
	void SetWantActiveFlag( bool b ){ m_bWantActive = b; }
	void SetWantCancelledFlag( bool b ){ m_bWantCancelled = b; }
	void SetAllowMode( int n ){ m_nAllowMode = n; }
	void SetDateMode( int n );

public:
	int GetPMSLocationStatus( CTermFileListInfo& infoFile );
	void GetInitialSaleDateTime( bool bIsPMRLocation, CTermFileListInfo& infoFile, bool bSaleTime, CString& strDate, CString& strTime );
	void GetPMSTransactionDate( bool bIsPMSLocation, CTermFileListInfo& infoFile, CString& strDate, CString& strTime );
	bool CheckPMSTransactionType( bool bIsPMSLocation, int& nCashRSPLineType );
	bool CheckPMSTimeLine( bool bIsPMSLoction, CString& strBuffer, CString& strThisSaleDate, CString& strThisSaleTime, COleDateTime& oleDatePMS, bool& bPMSInclude );

private:
	bool m_bEpos;
	bool m_bWantDeposits;
	bool m_bWantComplete;
	bool m_bWantPending;
	bool m_bWantActive;
	bool m_bWantCancelled;

private:
	int m_nAllowMode;
	int m_nDateMode;
};

/**********************************************************************/
#else
/**********************************************************************/

class CEposReportPMSModes
{
public:
	CEposReportPMSModes(void){};
	
public:
	bool GetEposFlag(){ return TRUE; }
	bool GetWantDepositsFlag(){ return FALSE; }
	bool GetWantCompleteFlag(){ return FALSE; }
	bool GetWantPendingFlag(){ return FALSE; }
	bool GetWantActiveFlag(){ return FALSE; }
	bool GetWantCancelledFlag(){ return FALSE; }
	int GetAllowMode(){ return PMS_ALLOWMODE_ALL; }
	int GetDateMode(){ return PMS_DATEMODE_ACTUAL; }

public:
	void SetEposFlag( bool b ){}
	void SetWantDepositsFlag( bool b ){}
	void SetWantCompleteFlag( bool b ){}
	void SetWantPendingFlag( bool b ){}
	void SetWantActiveFlag( bool b ){}
	void SetWantCancelledFlag( bool b ){}
	void SetAllowMode( int n ){}
	void SetDateMode( int n ){}

public:
	int GetPMSLocationStatus( CTermFileListInfo& infoFile ){ return 2; }
	void GetInitialSaleDateTime( bool bIsPMSLocation, CTermFileListInfo& infoFile, bool bSaleTime, CString& strDate, CString& strTime );
	void GetPMSTransactionDate( bool bIsPMSLocation, CTermFileListInfo& infoFile, CString& strDate, CString& strTime ){}
	bool CheckPMSTransactionType( bool bIsPMSLocation, int& nCashRSPLineType ){ return TRUE; }
	bool CheckPMSTimeLine( bool bIsPMSLoction, CString& strBuffer, CString& strThisSaleDate, CString& strThisSaleTime, COleDateTime& oleDatePMS, bool& bPMSInclude ){ return TRUE; }
};

/**********************************************************************/
#endif
/**********************************************************************/
