#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "PosTrayNetworkInfo.h"
#include "RealTimeCommsThread.h"
/**********************************************************************/

struct CPosTrayCashRSPBufferFileInfo
{
public:
	CPosTrayCashRSPBufferFileInfo()
	{
		m_nNetworkInfoIdx = 0;
	}

public:
	int m_nNetworkInfoIdx;
	CRealTimeFileBufferInfo m_infoFile;
};

/**********************************************************************/

class CPosTrayCashRSPBufferThreadInfo
{
public:
	CPosTrayCashRSPBufferThreadInfo();

public:
	void SetNetworkInfo( CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo>& arrayPendingNetworkInfo );
	int GetNetworkInfoCount(){ return m_arrayNetworkInfo.GetSize(); }
	void GetNetworkInfo( int n, CPosTrayNetworkInfo& infoNetwork );
	
public:
	void SetNetworkInfoIdxToClear();
	int GetNetworkInfoIdxToClearCount(){ return m_arrayNetworkInfoIdxToClear.GetSize(); }
	int GetNetworkInfoIdxToClear( int n );
	void RemoveNetworkInfoIdxToClear( int n );

public:
	void ResetTransactionsToClear(){ m_arrayTransactionsToClear.RemoveAll(); }
	int GetTransactionsToClearCount(){ return m_arrayTransactionsToClear.GetSize(); }
	void AddTransactionsToClear( CArray<CPosTrayCashRSPBufferFileInfo>& arrayFiles );
	void GetTransactionToClear( int n, CPosTrayCashRSPBufferFileInfo& infoTran );
	void RemoveTransactionToClear( int n );

public:
	bool GetNewTaskFlag(){ return m_bNewTask; }
	bool GetCompletionFlag(){ return m_bCompletion; }
	bool GetForceQuitFlag(){ return m_bForceQuit; }

public:
	void SetNewTaskFlag( bool b ){ m_bNewTask = b; }
	void SetCompletionFlag( bool b ){ m_bCompletion = b; }
	void SetForceQuitFlag( bool b ){ m_bForceQuit = b; }

private:
	CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo> m_arrayNetworkInfo;
	CArray<CPosTrayCashRSPBufferFileInfo> m_arrayTransactionsToClear;
	CArray<int,int> m_arrayNetworkInfoIdxToClear;

private:
	bool m_bForceQuit;
	bool m_bCompletion;
	bool m_bNewTask;
};

/**********************************************************************/

class CPosTrayCashRSPBufferThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CPosTrayCashRSPBufferThreadInfo* pInfo );

private:
	static bool GetCashRSPFileList( CPosTrayCashRSPBufferThreadInfo* pInfo, int nIdx, bool& bForceQuit, CArray<CPosTrayCashRSPBufferFileInfo>& arrayFiles );
};

/**********************************************************************/
#endif
/**********************************************************************/
