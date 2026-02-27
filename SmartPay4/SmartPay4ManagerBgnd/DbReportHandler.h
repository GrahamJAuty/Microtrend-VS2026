#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "DbExportHandler.h"
//$$******************************************************************
#define nREPORT_SORTON		0				// SortOn level
#define nREPORT_GROUP		1				// Sortby Group
#define nREPORT_TOTAL		2				// report Total 
#define nREPORT_MAX			3
//$$******************************************************************

class CDbReportHandler : public CDbExportHandler
{
public:
	CDbReportHandler();
	virtual ~CDbReportHandler() {}

public:
	void ClearTotals();
	void ClearTotals( int nIndex );

public:
	CString GetReportFilename();
	CString GetParamsFilename();
	CString GetReportKey();
	CString GetReportTitle ( const char* szText = "" );

	void AddOrderByColumnText();
	void AddSpecialColumn(CString strColumn);
	
	void AppendDbFieldsToCSVLine (CSQLRowAccountFull& AccountRow, CCSV* pCsv, bool bRecordExists = TRUE, __int64 nUnknownCardNo = 0, CString strUnknownCardName = "", int nFixGroupNo = -1 );
	void CreateReportHeader ( CReportFileWithEndText* pReport );

	void CreateTotalLine ( int nTextPos, CString strText, CCSV* pCsv, CCSV* pCsvTabLine, int nIndex, int nGroupCount );

public:
	void ResetSpecialFields();
	void AddSpecialField(CString str);
	void AddSpecialField(double d, int nDPs);
	void AddSpecialField(int n);
	CString GetSpecialField(int n);

private:
	void AddCardNo ( CCSV* pCsv, __int64 nCardNo, bool bExists = TRUE );

	const char* GetBoolText ( bool bFlag );
	CString GetAllergyText(CSQLRowAccountFull& AccountRow);
	CString GetAlertText(CSQLRowAccountFull& AccountRow);

private:
	int m_nTotalPoints[nREPORT_MAX];
	int m_nTotalPointsTD[nREPORT_MAX];
	double m_dTotalPurse1Liability[nREPORT_MAX];
	double m_dTotalPurse1Credit[nREPORT_MAX];
	double m_dTotalPurse2[nREPORT_MAX];
	double m_dTotalPurse3Liability[nREPORT_MAX];
	double m_dTotalPurse3Credit[nREPORT_MAX];
	double m_dTotalCashSpendTD[nREPORT_MAX];
	double m_dTotalPurse1Spend[nREPORT_MAX];
	double m_dTotalPurse2Spend[nREPORT_MAX];
	double m_dTotalPurse3Spend[nREPORT_MAX];
	double m_dTotalPurse1SpendTD[nREPORT_MAX];
	double m_dTotalPurse2SpendTD[nREPORT_MAX];
	double m_dTotalPurse3SpendTD[nREPORT_MAX];
	double m_dTotalCardSpendTD[nREPORT_MAX];
	double m_dTotalSpendTD[nREPORT_MAX];
	CStringArray m_arraySpecialFields;
};

//$$******************************************************************
