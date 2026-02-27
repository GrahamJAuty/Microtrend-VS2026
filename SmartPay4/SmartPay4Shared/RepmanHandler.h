#pragma once
//****************************************************************
#include "..\SmartPay4Shared\RepmanDefines.h"
//****************************************************************

//****************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//****************************************************************

class CRepmanHandler
{
public:
	CRepmanHandler( const char* szTitle = "" );
	
public:
	virtual void SetTitle ( const char* szTitle ){ m_strTitle = szTitle; }
	void SetMode ( int nMode ) { m_nMode = nMode; }

	void SetupSaveAs ( int nSaveAsType, const char* szSaveAsFilename );
	void SaveReportAs ( const char* szReport );
	CString GetRepmanHandlerError ( int nErrorNo, const char* szText = "" );

private:
	CString m_strTitle;
	CString m_strSaveAsFilename;
	int m_nMode;
	int m_nSaveAsType;
};

//****************************************************************
#else
//****************************************************************

class CRepmanHandler
{
public:
	CRepmanHandler ( const char* szTitle = "", CWnd* pParent = NULL );
	void SetParentWindow(CWnd* pParent);

public:
	int DisplayReport ( const char* szReport, const char* szParams = "", const char* szParamsKey = "", CWnd* pReportParentWnd = NULL);
	int ShowBarcode ( const char* szReport, const char* szLabel, bool bPrint = FALSE ); 

	bool DisplayError ( int nErrorNo, const char* szText = "" );
	void DisplayMessage ( const char* szMsg, int nType = MB_OK );

public:
	virtual void SetTitle ( const char* szTitle ){ m_strTitle = szTitle; }
	void SetMode ( int nMode ) { m_nMode = nMode; }

	void SetupSaveAs ( int nSaveAsType, const char* szSaveAsFilename );
	void SaveReportAs ( const char* szReport );
	CString GetRepmanHandlerError ( int nErrorNo, const char* szText = "" );

private:
	CString m_strTitle;
	CString m_strSaveAsFilename;
	int m_nMode;
	int m_nSaveAsType;
	CWnd* m_pParent;
};

//****************************************************************
#endif
//****************************************************************

//****************************************************************
extern const char* szMSG_EXCEPTIONSDETECTED;
extern const char* szMSG_CREATINGREPORT;
extern const char* szMSG_SCANNINGAUDIT;
extern const char* szMSG_SCANNINGDATABASE;
extern const char* szMSG_SCANCOMPLETE;			// "Database scan complete";
extern const char* szMSG_SCANCANCELLED;			// "Database scan terminated!"
//****************************************************************
