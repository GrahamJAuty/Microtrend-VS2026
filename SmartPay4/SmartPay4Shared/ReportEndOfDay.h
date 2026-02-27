#pragma once
//$$******************************************************************
#include "EODCashlessConsolidator.h"
#include "EODDefines.h"
#include "EODTextData.h"
#include "ReportFileWithEndText.h"
//$$******************************************************************
#define nEOD_FALSE 0				// EOD not done
#define nEOD_PERFORMED 1			// EOD done
#define nEOD_EXISTS	2				// EOD already done
#define nEOD_INPROGRESS	3			// EOD already done
//$$******************************************************************

struct CEODListInfo
{
public:
	CString m_strKeyDate = "";
	CString m_strDisplayDate = "";
	CString m_strDisplayTime = "";
	double m_dPurse1 = 0.0;
	double m_dPurse3 = 0.0;
	bool m_bArchived = FALSE;
};

//$$******************************************************************

class CReportEndOfDay  
{
public:
	CReportEndOfDay ( CWnd* pParent = NULL );

//$$******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//$$******************************************************************

public:
	int Background();
	CString GetBackgroundError() { return m_strBackgroundError; }

private:
	CString m_strBackgroundError;

//$$******************************************************************
#else
//$$******************************************************************

public:
	void Show();

private:
	int RegenerateReport ( CString strDate, CString strReportFile, bool bSingle );
	int ConsolidateReport ( CString strKeyDateFrom, CString strKeyDateTo, CString strReportFile );
	int DisplayNoDate ( CString strDateFrom, CString strDateTo );
	CString CreateDisplayDateFromSortDate(CString strSortDate);

//$$******************************************************************
#endif
//$$******************************************************************

public:
	static void GetEODDateInfo(CArray<CEODListInfo, CEODListInfo>& arrayEODListInfo, int& nTotalRows, int nOffset = 0, int nMaxCount = -1);

private:
	void Init();
	void Reset();

#ifndef SYSTEMTYPE_SERVER
	int EndOfDayReport ( const char* szReportFile, bool bPerformEOD );
	void OtherEODFunctions(CSQLAuditRecord* pAtcRecord);
#endif

	int AddTerminalGrouping ( CEODCashlessConsolidator* pEODCashlessConsolidator );
	CString GetTerminalSetName ( int nSetNo );

	void AddColumnText();
	void CreateReport (CEODCashlessConsolidator* pEODCashlessConsolidator, bool bHaveDatabaseBalance);

	void ShowLines ( int nColumns, bool bExtraReqd = FALSE );
	void ShowColumnHeadings ( const char* szText1, const char* szText2, bool bNoPurse2Text, bool bNoCashText );

	bool CreateLine1Value(const char* szText2, const char* szText3, double dPurse1Value, bool bIsTopUp = FALSE);
	bool CreateLine3Value(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse2Value, double dPurse3Value, bool bForceValue3 );
	bool CreateLine4Value(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse2Value, double dPurse3Value, double dCashValue );
	bool CreateLinePurse1And3(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse3Value);
	bool CreateLinePosting(const char* szText1, const char* szText2, const char* szText3, CEODCashlessConsolidator* pEODCashlessConsolidator, int nActionType, bool bCredit, bool bNegative, bool bShowColumn3 = TRUE, bool bShowColumn4 = FALSE);
	bool CreateLineInternal(const char* szText1, const char* szText2, const char* szText3, double dValue1, double dValue2, double dValue3, double dValue4, bool bZeroSkip, bool bNoPurse2, bool bNoPurse3, bool bUseValue2, bool bUseValue3, bool bUseValue4 );

	void AddAdditionalInfo();
	void AddPeriodSales ( const char* szAuditFile, bool bUseFullFile );
	void AddEposPayments ( const char* szAuditFile);
	bool ShowTaxLine(double dSalesValue);

	CString CheckEODAuditFile();
	void MakeEODAuditFile();

	CString SetEODInProgress();
	void ClearEODInProgress();

private:
	void CreateLiabilityAdditionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport);
	void CreateCreditAdditionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport);
	void CreateLiabilityDeductionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport);
	void CreateCreditDeductionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport);
	void CreateClosingBalanceSection(bool bHaveDatabaseBalance);
	void CreateCashSalesSection(CEODCashlessConsolidator* pEODCashlessConsolidator);
	void CreateSalesBreakdownSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport);

private:
	CWnd* m_pParent;
	CString m_strReportLabel;
	CString m_strTitle;
	CString m_strEODSaveFilename;
	
	int m_nReportTNoSetNo;

	bool m_bSingleEODRow;
	int m_nEODReportType;
	int m_nDayCounter;
	bool m_bShowPurse3Column;
	bool m_bZeroSkipDetail;
	bool m_bZeroSkipVAT;
	bool m_bDefaultZeroSkipDetail;
	bool m_bDefaultZeroSkipVAT;

	CEODTextData m_texts;
	CReportFileWithEndText m_Report;
	CSQLAuditRecord m_atc;

	CString m_strOpeningDate;
	CString m_strOpeningTime;
	double m_dOpeningPurse1Liability;
	double m_dOpeningPurse2Balance;
	double m_dOpeningPurse3Liability;
	double m_dOpeningPurse1Credit;
	double m_dOpeningPurse3Credit;

	CString m_strClosingDate;
	CString m_strClosingTime;
	double m_dClosingPurse1Liability;
	double m_dClosingPurse2Balance;
	double m_dClosingPurse3Liability;
	double m_dClosingPurse1Credit;
	double m_dClosingPurse3Credit;

	double m_dDatabasePurse1Liability;
	double m_dDatabasePurse3Liability;
	double m_dDatabasePurse1Credit;
	double m_dDatabasePurse3Credit;

	bool m_bPerformingEOD;
	CSSFile m_fileEODInUse;
	CString m_strEODInUseFilename;

	CString m_strEODAuditFilename;
	CString m_strEODPaymentAuditFilename;
};

//$$******************************************************************
