#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "5000Client.h"
#include "JobListCSVArray.h"
#include "KeyboardMap.h"
#include "SelectMultipleEntityInfo.h"
#include "SPOSCommsConnect.h"
/**********************************************************************/

class CCommsBatchDlg : public CDialog
{
public:
	CCommsBatchDlg( CEposTaskDatabase& TaskDatabase, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>* pDownloadMasters, CString& strDate, CKeyboardMap& KeyboardMap, CWnd* pParent );
	CCommsBatchDlg( CEposTaskDatabase& TaskDatabase, CString& strDate, bool bFromMemory, CKeyboardMap& KeyboardMap, int nDbIdx, int nAutoMode, int nAutoCloseDelay, CWnd* pParent );
	int GetJobListSize(){ return m_JobArray.GetSize(); }

private:
	void DataInit( CEposTaskDatabase& TaskDatabase, CString& strDate, bool bFromMemory, CKeyboardMap& KeyboardMap, int nDbIdx, int nAutoMode, int nAutoCloseDelay );

public:
	//{{AFX_DATA(CCommsBatchDlg)
	enum { IDD = IDD_COMMS_BATCH };
	CButton	m_buttonStop;
	CButton	m_buttonCancel;
	CListBox	m_listJobs;
	CButton	m_buttonRetry;
	CStatic	m_staticInfoJobCount;
	CStatic	m_staticInfo2;
	CStatic	m_staticInfo3;
	CStatic	m_staticInfo4;
	CStatic	m_staticInfo5;
	CButton	m_buttonStart;
	CButton	m_buttonTimelog;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCommsBatchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCommsBatchDlg)
	afx_msg void OnButtonStart();
	afx_msg void OnButtonTimelog();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRetry();
	afx_msg void OnSelectJob();
	virtual void OnCancel();
	afx_msg void OnButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStaticInfo();
	//}}AFX_MSG
	virtual void OnOK();
	afx_msg long OnStartComms( WPARAM, LPARAM );
	afx_msg long OnCloseComms( WPARAM, LPARAM );
	DECLARE_MESSAGE_MAP()
	
private:
	int ExecuteDownloadPlu			( C5000Client& Client, CJobListCSVRecord& JobInfo, bool bRealTime );
	int ExecuteDownloadPluInfo		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadDept			( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadGroup		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadAllergen		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadLoyaltyScheme( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadTableArea	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadTableName	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadMixMatch		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadPromotion	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadWalletStampRule	( C5000Client& Client, CJobListCSVRecord& JobInfo);
	int ExecuteDownloadAllOffer		( C5000Client& Client, CJobListCSVRecord& JobInfo);
	int ExecuteDownloadTimePlan		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadTax			( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadBarcode		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadCategory		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadPaidInOutText( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadModifiers	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadPayment		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadServerName	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadReceipt		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadServerPreset	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadFunction		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadMacro		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteDownloadReason		(C5000Client& Client, CJobListCSVRecord& JobInfo);

private:
	int ExecuteDownloadKeyboard			( C5000Client& Client, CJobListCSVRecord& JobInfo, int nKbNo );
	int ExecuteDownloadKeyboardInternal	( C5000Client& Client, CJobListCSVRecord& JobInfo, int nKbNo );
	int ExecuteDownloadButtonImages		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	
	int ExecuteReceiveSalesBatch	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveSalesSptBook	( CJobListCSVRecord& JobInfo );
	int ExecuteReceiveSalesSmartEnt	( CJobListCSVRecord& JobInfo );
	
	int ExecuteReceiveReasons			( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveReasonsRealTime	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveReasonsBatch		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	
	int ExecuteReceiveEODPlu			( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveEODPluRealTime	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveEODPluBatch		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	
	int ExecuteReceiveEODCustomer			( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveEODCustomerRealTime	( C5000Client& Client, CJobListCSVRecord& JobInfo );
	int ExecuteReceiveEODCustomerBatch		( C5000Client& Client, CJobListCSVRecord& JobInfo );
	
	void LogDownloadExclusions		( CCommsExportBase& Export, CJobListCSVRecord& JobInfo );
	void LogDownloadExclusion		( CJobListCSVRecord& JobInfo, CString& strError );
	void LogCommsError				( CJobListCSVRecord& JobInfo, int nError );
	void LogCommsError				( CJobListCSVRecord& JobInfo, CString& strError );
	void LogSptBookCommsMessage		( CJobListCSVRecord& JobInfo, const char* szHeader, const char* szError, const char* szPreviousTime, const char* szNextTime );
	void LogSmartEntCommsMessage	( CJobListCSVRecord& JobInfo, const char* szHeader, const char* szError, const char* szPreviousTime, const char* szNextTime );
	void LogUserStop();

private:
	int ExecuteDownloadGeneral( CCommsExportBase& Export, C5000Client& Client, CJobListCSVRecord& JobInfo );
		
private:
	void BuildList();
	bool CheckRetryStatus();
	void UpdateLine( int nIndex );
	void AttemptJobs ( int nStartIdx, int nEndIdx );

	void HandleStart();
	void HandleRetry();

	void ShowStatusHeaderInfo( const char* szInfo );
	void ShowJobDetailInfo( C5000ClientPutFileInfo& infoPutFile );
	void ShowJobDetailInfo( const char* szInfo );
	void ClearJobDetailInfo();

private:
	CJobListCSVArray m_JobArray;
	bool m_bGotSales;

private:
	CString m_strTaskName;
	CArray<CJobSetupInfo, CJobSetupInfo> m_arrayJobSetupInfo;
	CString m_strJobCount;
	
private:	
	CSPOSCommsConnect m_Connect;
	
private:
	UINT m_hTimer;
	CTime m_JobReadyTime;
	bool m_bJobReady;
	bool m_bStopFlag;
	int m_nHours;
	int m_nMinutes;
	bool m_bFromMemory;
	CString m_strTempLogFilename;

private:
	int m_nAutoMode;
	int m_nAutoCloseDelay;
	bool m_bCanAutoClose;
	COleDateTime m_timeAutoClose;	
	
private:
	bool m_bCanRestart;
	int m_nStatus;

private:
	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>* m_pDownloadMasters;
	CKeyboardMap& m_KeyboardMap;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
