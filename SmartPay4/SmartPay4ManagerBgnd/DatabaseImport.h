#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "SQLTranImportHelpers.h"
/**********************************************************************/
#include "CSVImportData.h"
/**********************************************************************/

class CImportDatabase
{
public:
	CImportDatabase ( CWnd* pParent = NULL );

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

private:
	void DisplayMessage( const char* szMsg ){}
	void LogMessage( CString strMag );

public:
	void ShowStats(CSQLImportCounters& Counters){}

//******************************************************************
#else
//******************************************************************

public:
	bool UserImport(CSQLImportCounters& Counters);	
	bool CustomImport(CSQLImportCounters& Counters);
	bool ImportTopups(CSQLImportCounters& Counters);

private:
	bool CheckCache(CString strImportFilename, CString& strCacheFilename, int nType);

private:
	void DisplayMessage( const char* szMsg );
	void LogMessage( CString strMag ){}

public:
	void ShowStats(CSQLImportCounters& Counters);

public:
	bool IsValidFileForUpgrade(CString strFilename);
	bool DirectImport(CString strFilename);
	
//******************************************************************
#endif
//******************************************************************

private:
	void Init();

public:
	~CImportDatabase();

public:
	bool AutoImportDatabase ( int nDataFormat, const char* szFilename, int nImportMethod, CSQLImportCounters& Counters );

private:
	bool ValidateFile ( CString strImportFilename, CString strMandatoryHeader );
	
	void CopyPhotoFile ( const char* szFromCardNo, const char* szToCardNo );
	void CopyPurchaseFiles ( const char* szFromCardNo, const char* szToCardNo );
	void CopyRefreshDatabase ( const char* szOldCardNo, const char* szNewCardNo );

	void CreateNumericHeader(CCSV& csvImportHeader);
	bool CompareLabel(CString strLabel1, CString strLabel2);

private:
	void SaveStandardNextNewUserID();
	void SaveCustomNextNewUserID();
	void SaveTopupNextNewUserID();

private:
	CWnd* m_pParent;

	CCsvImportData m_data;
	int m_nLocateOn;
	CString m_strLocateOnLabel;
	CString m_strNextNewUserID;

	CArray<int, int> m_arrayImportHeader;

	CString m_strExceptionPrefix;

	int m_nImportAppNo;
	int m_nImportMethod;
	int m_nIndexCardNo;
	int	m_nIndexLocateOn;
	
	bool m_bMiFare2Updated;

	CStringArray m_arrayDeleteList;

	CString m_strError;
	CString m_strTitle;
};

/**********************************************************************/
