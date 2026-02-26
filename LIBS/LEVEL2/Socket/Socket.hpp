/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxsock.h>
#include <afxmt.h>			// CCriticalSection

/**********************************************************************/
/*	Blocking Socket class									*/
/**********************************************************************/

class CSSSocket
{
public:
	CSSSocket();
	CSSSocket ( DWORD dwConnectTimeout, DWORD dwRxTimeout, DWORD dwTxTimeout );
	virtual ~CSSSocket();

	BOOL Accept ( CSSSocket& sock );
	BOOL Bind ( UINT uiPort, const char* szIPAddress = NULL );
	BOOL Connect ( const char* szIPAddress, UINT uiPort );
	void Close();
	BOOL Create ( UINT uiPort = 0, int nSocketType = SOCK_STREAM );
	BOOL Listen ( int nBacklog = 5 );
	int Receive ( void* lpBuf, int nBufLen );
	int Send ( void* lpBuf, int nBufLen );
	int Shutdown ( int nHow );

	SOCKET Detach();
	void Attach ( SOCKET hSocket );

	void SetRxTimeout ( DWORD dwRxTimeout );	// milliseconds
	void SetTxTimeout ( DWORD dwTxTimeout );	// milliseconds

	bool TxData ( unsigned char* pBuf, int nLength );
	bool RxData ( unsigned char* pBuf, int nLength );

	CString GetError() { return m_strError; }
	SOCKET GetHandle() { return m_hSocket; }

public:
	bool WriteString ( const char* szBuf );		// dont append crlf
	bool WriteLine ( const char* szBuf );		// append crlf

	// the following functions use m_strRxBuf
	// do not use with other (unbuffered) receive functions

	bool ReadString ( CString& strBuf );		// discard crlf
	bool ReadLine ( CString& strBuf );			// keep crlf
	bool ReadData ( CString& strBuf, int nLength );

private:
	bool RxText();

protected:
	CString m_strError;

private:
	SOCKET m_hSocket;
	TIMEVAL m_tvRx;
	TIMEVAL m_tvTx;
	CString m_strRxBuf;
};

/**********************************************************************/
/*	Multi-threaded Server class								*/
/**********************************************************************/

class CSSServer
{
public:
	CSSServer ( int nMaxThreads, DWORD dwExitTimeout );
	bool StartListening ( UINT uiPort );
	bool StopListening();
	CString GetError() { return m_strError; }

protected:
	virtual void OnConnect ( CSSSocket& sockConnect, int nThreadNo, bool& bTerminate ) = 0;
	bool RemoteStopListening();	// must only be called from OnConnect() routine

private:
	void Init();

	void IncThreadCount() { m_cs.Lock(); m_nThreadCount++; m_cs.Unlock(); }
	void DecThreadCount() { m_cs.Lock(); m_nThreadCount--; m_cs.Unlock(); }

private:
	static UINT ThreadProc ( LPVOID pParam );

private:
	static CSSSocket m_sockListen;
	static int m_nMaxThreads;
	static int m_nThreadCount;
	static bool m_bTerminate;

protected:
	CString m_strError;

private:
	bool m_bListening;
	DWORD m_dwExitTimeout;
	CCriticalSection m_cs;
};

/**********************************************************************/

extern const char* szVERSION_SOCKET;

/**********************************************************************/
