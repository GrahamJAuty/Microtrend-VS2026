//$$******************************************************************
#pragma once
//$$******************************************************************
#define nMIFARE_DATAFORMAT_STANDARD		0
#define nMIFARE_DATAFORMAT_PAXTON		1
#define nMIFARE_DATAFORMAT_INNERRANGE	2
//$$******************************************************************

class CMyMifare : public CMifareReader
{
public:
	CMyMifare();
	void OnGotUID ( const char* szUID );
	void OnDisplayRemoveCard();
	void OnDisplayPresentCard();
	void SetDataFormat ( int nFormat ) { m_nDataFormat = nFormat; }

	void SetPresentPrompt ( const char* szText ) { m_strPresentPrompt = szText; }
	CString GetCardNo() { return m_strUID; }

public:
	bool SelectReader ( const char* szReader, const char* szPort, const char* szBaud );
	int GetUID( bool bWaitForCard = 1, bool bLogRead = FALSE );

public:
	CWnd* m_pWnd;											// used by test redaer
	bool m_bWaitForCard;

private:
	CString m_strUID;
	CString m_strPresentPrompt;
	int m_nDataFormat;
};

//$$******************************************************************
