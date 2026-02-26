/**********************************************************************/
#pragma once
/**********************************************************************/

/**********************************************************************/
/*	abstract base class cannot be instantiated					*/
/**********************************************************************/

class CMifare
{
public:
	CMifare() { m_bExit = FALSE; m_nUIDFormat = 0; };
	virtual ~CMifare(){};

	virtual int GetUID ( const char* szReader, bool bWaitForCard, bool bLogRead ) = 0;	// 0 = ok, 1 = error, 2 = card not present, -1 = terminated
	void Terminate() { m_bExit = TRUE; }							// break out of GetUID() function

	void SetUIDFormat ( int n ) { m_nUIDFormat = n; }		// 0 = use UID format 1(default), 1 = use UID format 2};
	const char* GetError() { return m_strError; }

	virtual bool OpenPort ( const char* szPort, const char* szBaud ) { return TRUE; }	// eg "COM1", "9600" (only used for RS232 comms)
	virtual void ClosePort() {};												// (only used for RS232 comms)

	void SetQuickScanFlag(bool bQuickScan) { m_bQuickScan = bQuickScan; }

protected:
	CString m_strError = "";
	int m_nUIDFormat = 0;
	bool m_bExit = FALSE;
	bool m_bQuickScan = FALSE;
};

/**********************************************************************/

class CMifareReader
{
public:
	CMifareReader();
	virtual ~CMifareReader();

	int GetReaders ( CStringArray& arReaders );			// get array of compatible readers, returns number of readers

	bool SelectReader ( const char* szReader, const char* szPort, const char* szBaud, bool bQuickScan = FALSE );	// eg "Gemalto Prox-P2 Contactless", "COM1", "9600" (szPort & szBaud not required if not rs232)
	void DeselectReader();

	int GetUID ( bool bWaitForCard=TRUE, bool bLogRead = FALSE );		// 0 = ok, 1 = error, 2 = card not present, -1 = terminated
	void Terminate();						// break out of GetUID() function

	void SetUIDFormat ( int n );				// 0 = use UID format 1(default), 1 = use UID format 2
	const char* GetError();

	virtual void OnGotUID ( const char* szUID ) {};		// called when card read successfully
	virtual void OnDisplayPresentCard() {};				// called to display "Present Card" message
	virtual void OnDisplayRemoveCard() {};				// called to display "Remove Card" message

private:
	CMifare* m_pReader = NULL;
	CString m_strReader = "";

protected:
	CString m_strError = "";
};

/**********************************************************************/

extern const char* szVERSION_SMARTUID;

/**********************************************************************/
