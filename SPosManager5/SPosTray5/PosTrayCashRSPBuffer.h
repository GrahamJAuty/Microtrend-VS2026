#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "PosTrayCashRSPBufferThread.h"
#include "PosTrayNetworkInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define POSTRAY_CASHRSPBUFFER_INACTIVE 0
#define POSTRAY_CASHRSPBUFFER_WORKING 1
#define POSTRAY_CASHRSPBUFFER_NEWTASK 2
#define POSTRAY_CASHRSPBUFFER_WAITQUIT 3
#define POSTRAY_CASHRSPBUFFER_COMPLETE 4
/**********************************************************************/

class CPosTrayCashRSPBuffer
{
public:
	CPosTrayCashRSPBuffer();
	~CPosTrayCashRSPBuffer(void);

private:
	void Reset();
	void TerminateThread();

public:
	void SetPendingNetworkInfo( CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo>& arrayNetworkInfo);
	
public:
	void GetPendingDateTime( COleDateTime& OleDateTime ){ OleDateTime = m_OleDateTimePending; }
	bool IsDateTimeCleared( COleDateTime& OleDateTime );

public:
	bool SetPendingDateTime( COleDateTime& OleDateTime );
	const char* GetPendingWorkString();
	void Pulse();

private:
	int m_nStatus;
	bool m_bGotClearedDate;
	bool m_bGotPendingDate;
	
private:
	COleDateTime m_OleDateTimeCleared;
	COleDateTime m_OleDateTimePending;
	
private:
	CString m_strPendingWork;
	int m_nPendingNetworks;
	int m_nPendingTransactions;

private:
	CWinThread* m_pPosTrayCashRSPBufferThread;
	CPosTrayCashRSPBufferThreadInfo m_PosTrayCashRSPBufferThreadInfo;

private:
	bool m_bGotPendingNetworkInfo;
	CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo> m_arrayPendingNetworkInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
