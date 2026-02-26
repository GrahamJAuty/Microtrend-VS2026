#pragma once
//*******************************************************************
#include "..\SPosLoyalty4Shared\RepmanDefines.h"
//*******************************************************************

class CRepmanHandler
{
public:
	CRepmanHandler ( const char* szTitle, CWnd* pParent );
	void SetMode ( int nMode ) { m_nMode = nMode; }
	void SetTitle ( const char* szTitle ) { m_strTitle = szTitle; }

	int DisplayReport ( const char* szReport, const char* szParams, const char* szParamsKey = "", bool bPrintReqd = FALSE );
	int ShowBarcode ( const char* szReport, const char* szLabel, bool bPrint = FALSE ); 

	void SetupSaveAs ( int nSaveAsType, const char* szSaveAsFilename );
	bool DisplayError ( int nErrorNo );
	void DisplayMessage ( const char* szMsg, int nType = MB_OK );

	CString GetTitle() { return m_strTitle; }

private:
	void SaveReportAs ( const char* szReport );

private:
	CString m_strTitle;
	CWnd* m_pParent;

	int m_nMode;
	int m_nSaveAsType;
	CString m_strSaveAsFilename;
};

//*******************************************************************
