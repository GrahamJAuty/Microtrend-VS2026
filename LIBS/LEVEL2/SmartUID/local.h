/**********************************************************************/
#pragma once
/**********************************************************************/

#include <winscard.h>

/**********************************************************************/
/*	TLP-224 Comms (7 bit)									*/
/**********************************************************************/

static const int MAX_BLOCK = 561;		// 256*2+spare

class CTLPComms : public CComms
{
public:
	CTLPComms();
	bool OpenPort ( const char* szPort, const char* szBaud );	// eg "COM1", "38400"
	void ClosePort();
	int TxCommand ( CByteArray& ar, int nRxTimeout );
	void Terminate();
	unsigned char GetRxLen() { return m_RxBuf[1]; }
	unsigned char GetRxStatus() { return m_RxBuf[2]; }
	unsigned char GetRxBuf ( int nIndex ) { return m_RxBuf[nIndex]; }

protected:
	virtual void OnCommTimer();				/* override CComms */
	virtual void OnCommRxChar ( char c );		/* override CComms */

private:
	void TxData();
	void TxNak();
	void State1();
	void Exit ( int nErrno );
	bool ValidateBlock();
	unsigned char CalcCRC ( unsigned char* buf );
	unsigned char CalcCRC ( CByteArray& ar );
	unsigned char GetLSN ( unsigned char c );
	unsigned char GetMSN ( unsigned char c );
	unsigned char AsciiToHex ( unsigned char c );
	unsigned char GetByte ( unsigned char msd, unsigned char lsd );

private:
	bool m_bExit;
	int m_nErrno;
	int m_nState;
	int m_nNakCount;
	int m_nRxTimeout;
	int m_nRetryCount;
	int m_nRxCount;
	unsigned char m_TxBuf[MAX_BLOCK];
	unsigned char m_RxBuf[MAX_BLOCK];
};

/**********************************************************************/

class CGemaltoProxP2 : public CMifare		// contactless RS232
{
public:
	CGemaltoProxP2 ( CMifareReader* pReader );
	virtual ~CGemaltoProxP2();

	bool OpenPort ( const char* szPort, const char* szBaud );		// eg "COM1", "9600"
	void ClosePort();

	int GetUID ( const char* szReader, bool bWaitForCard, bool bLogRead );		// 0 = ok, 1 = error, 2 = card not present, -1 = terminated

	virtual void OnGotUID ( const char* szUID ) { if ( m_pReader != NULL ) m_pReader->OnGotUID(szUID); }	// called when card read successfully
	virtual void OnDisplayPresentCard() { if ( m_pReader != NULL ) m_pReader->OnDisplayPresentCard(); }		// called to display "Present Card" message
	virtual void OnDisplayRemoveCard() { if ( m_pReader != NULL ) m_pReader->OnDisplayRemoveCard(); }		// called to display "Remove Card" message

	static int GetReaders ( CStringArray& arReaders );	// get array of compatible readers attached to pc, returns number of readers
	static bool IsValid ( const char* szReader );

public:
	CTLPComms* GetTLPComms() { return &m_comms; }		// DO NOT USE (for testing only)

private:
	int RFReset();						// returns 0 = ok, < 0 are comms errors, > 0 are reader status errors
	int GetFirstCard ( BYTE ucTimeout );	// ucTimeout * 100ms, returns 0 = ok, < 0 are comms errors, > 0 are reader status errors

private:
	CMifareReader* m_pReader = NULL;
	CTLPComms m_comms;
	bool m_bPortOpen = FALSE;
};

/**********************************************************************/

class CGemaltoProxDU : public CMifare		// contactless usb
{
public:
	CGemaltoProxDU ( CMifareReader* pReader );
	virtual ~CGemaltoProxDU();

	int GetUID ( const char* szReader, bool bWaitForCard, bool bLogRead );			// 0 = ok, 1 = error, 2 = card not present, -1 = terminated

	virtual void OnGotUID ( const char* szUID ) { if ( m_pReader != NULL ) m_pReader->OnGotUID(szUID); }	// called when card read successfully
	virtual void OnDisplayPresentCard() { if ( m_pReader != NULL ) m_pReader->OnDisplayPresentCard(); }		// called to display "Present Card" message
	virtual void OnDisplayRemoveCard() {if ( m_pReader != NULL ) m_pReader->OnDisplayRemoveCard(); }		// called to display "Remove Card" message

	static int GetReaders ( CStringArray& arReaders );	// get array of compatible readers attached to pc, returns number of readers
	static bool IsValid ( const char* szReader );

private:
	long GetCardStatus ( SCARDHANDLE hCard, DWORD& dwCardStatus );

private:
	CMifareReader* m_pReader = NULL;
};

/**********************************************************************/

class COmnikey5x21Contactless : public CMifare		// contactless usb
{
public:
	COmnikey5x21Contactless ( CMifareReader* pReader );
	virtual ~COmnikey5x21Contactless();

	int GetUID ( const char* szReader, bool bWaitForCard, bool bLogRead );			// 0 = ok, 1 = error, 2 = card not present, -1 = terminated

	virtual void OnGotUID ( const char* szUID ) { if ( m_pReader != NULL ) m_pReader->OnGotUID(szUID); }	// called when card read successfully
	virtual void OnDisplayPresentCard() { if ( m_pReader != NULL ) m_pReader->OnDisplayPresentCard(); }		// called to display "Present Card" message
	virtual void OnDisplayRemoveCard() {if ( m_pReader != NULL ) m_pReader->OnDisplayRemoveCard(); }		// called to display "Remove Card" message

	static int GetReaders ( CStringArray& arReaders );	// get array of compatible readers attached to pc, returns number of readers
	static bool IsValid ( const char* szReader );

private:
	long GetCardStatus ( SCARDHANDLE hCard, DWORD& dwCardStatus );

private:
	CMifareReader* m_pReader = NULL;
};

/**********************************************************************/
