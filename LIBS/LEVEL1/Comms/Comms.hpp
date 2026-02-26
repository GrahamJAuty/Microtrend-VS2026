/**********************************************************************/
#ifndef __COMMS_HPP__
#define __COMMS_HPP__
/**********************************************************************/

static const DWORD EVENT_MASK = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;

/**********************************************************************/

class CComms
{
public:
	CComms();
	CComms ( HANDLE hComm );
	~CComms();

	bool Open ( const char* szMode,			/* eg "COM1:9600,o,7,1" */
			UINT cbInQueue = 1024,
			UINT cbOutQueue = 1024 );
	bool Close();

public:
	HANDLE GetHandle() { return m_hComm; }

	void RaiseDtr() { ::EscapeCommFunction ( m_hComm, SETDTR ); }
	void RaiseRts() { ::EscapeCommFunction ( m_hComm, SETRTS ); }
	void DropDtr()  { ::EscapeCommFunction ( m_hComm, CLRDTR ); }
	void DropRts()  { ::EscapeCommFunction ( m_hComm, CLRRTS ); }
	bool CommGetCTS();
	bool CommGetDCD();
	bool CommGetRI();
	void CommRxPurge() { ::PurgeComm ( m_hComm, PURGE_RXCLEAR ); }
	bool GetCommState ( DCB* lpDCB ) { return ::GetCommState ( m_hComm, lpDCB ) != 0; }
	bool SetCommState ( DCB* lpDCB ) { return ::SetCommState ( m_hComm, lpDCB ) != 0; }

public:
	bool CommTxChar ( const char c );
	bool CommTxString ( const char* szBuf );
	bool CommTxBlock ( const char* pBuf, DWORD dwCount );
	bool CommEnableNotification ( DWORD dwEventMask = EVENT_MASK );
	void CommDisableNotification();
	bool CommStartTimer ( DWORD dwTimeMSecs );
	void CommStopTimer();

protected:
	virtual void OnCommTimer() {};
	virtual void OnCommRxChar ( char c ) {};
	virtual void OnCommEventMask ( DWORD dwEvent ) {};

private:
	static UINT ReadThread ( LPVOID pParam );
	static volatile bool m_bReadThreadActive;

private:
	void BeginTimerThread();
	void EndTimerThread();

	static UINT TimerThread ( LPVOID pParam );
	static volatile bool m_bTimerThreadActive;
	static volatile DWORD m_dwTimeMSecs;
	static HANDLE m_hTimerEvent;
	HANDLE m_hTimerThread;

private:
	HANDLE m_hComm;
};

/**********************************************************************/

extern const char* szVERSION_COMMS;

/**********************************************************************/
#endif			/* __COMMS_HPP__ */
/**********************************************************************/
