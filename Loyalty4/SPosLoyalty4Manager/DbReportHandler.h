#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//*******************************************************************
#include "DbExportHandler.h"
//*******************************************************************

#define nREPORT_GROUP		0
#define nREPORT_TOTAL		1
#define nREPORT_MAX			2

//*******************************************************************

class CDbReportHandler : public CDbExportHandler
{
public:
	CDbReportHandler();
	virtual ~CDbReportHandler() {}
	void ClearTotals( int nIndex = -1 );

	CString GetReportFilename();
	CString GetParamsFilename();
	CString GetReportKey();
	CString GetReportTitle ( const char* szText = "" );

	void AddOrderByColumnText ( CReportFile* pReport );
	void AddOrderByData( CCSV* pCsv, CSQLRowAccountFull& RowAccount);			// use when card in database
	void AddOrderByData( CCSV* pCsv, CString strCardName, __int64 nUserID );	// used when not in database

	void AddOrderByTotalText ( const char* szText, CCSV* pCsv, CCSV* pCsvTabLine, int nGroupCount );

	void AppendDbFieldsToCSVLine(CSQLRowAccountFull& RowAccount, CCSV* pCsv, bool bExists);
	void CreateReportLine (CArray<int, int>& arrayHeader, CCSV* pCsv, CSQLRowAccountFull& RowAccount, bool bRecordExists = TRUE );
	void CreateReportHeader ( CReportFile* pReport );

	bool CreateTotalLine (CArray<int, int>& arrayHeader, CCSV* pCsv, CCSV* pCsvTabLine, int nIndex );

private:
	void AddColumnCardNo ( CReportFile* pReport, bool bIindicateHotlist );
	void AddColumnCardName ( CReportFile* pReport );
	void AddCardNo ( CCSV* pCsv, CSQLRowAccountFull& RowAccount, bool bExists, bool bIndicateHotlist );
	bool AddHotlistIndicator();

private:
	CUserTexts m_Text;

	int m_nTotalPoints[nREPORT_MAX];
	int m_nTotalPointsTD[nREPORT_MAX];
	double m_dTotalPurse1[nREPORT_MAX];
	double m_dTotalPurse2[nREPORT_MAX];
	double m_dTotalCashSpendTD[nREPORT_MAX];
	double m_dTotalPurse1SpendTD[nREPORT_MAX];
	double m_dTotalPurse2SpendTD[nREPORT_MAX];
	double m_dTotalSpendTD[nREPORT_MAX];
};

//*******************************************************************
