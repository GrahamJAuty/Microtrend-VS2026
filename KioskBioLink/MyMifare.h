#pragma once

#define nMIFARE_DATAFORMAT_STANDARD		0
#define nMIFARE_DATAFORMAT_PAXTON		1
#define nMIFARE_DATAFORMAT_INNERRANGE	2

//********************************************************

class CMyMifare : public CMifareReader
{
public:
	CMyMifare();
	void SetCardReferenceText ( const char* szText ) { m_strCardReferenceText = szText; }
	void OnGotUID ( const char* szUID );
	void OnDisplayRemoveCard();
	void OnDisplayPresentCard();
	void SetDataFormat ( int nFormat ) { m_nDataFormat = nFormat; }

	CString GetCardNo() { return m_strUID; }

public:
	CWnd* m_pWnd;											// used by test redaer
	CSSColourStatic* m_pStaticPrompt;						// use by kiosk
	bool m_bWaitForCard;

private:
	CString m_strUID;
	int m_nDataFormat;
	CString m_strCardReferenceText;
};
