#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportConsolLoyaltyRecon.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

class CEposReportLoyaltyRecon : public CEposReport
{
public:
	CEposReportLoyaltyRecon(CEposSelectArray& SelectArray, const char* szCustomSettings);
	~CEposReportLoyaltyRecon();

	virtual void Consolidate();
	virtual bool CreateReport();

public:
	bool SpecialPrepare();

private:
	void CreateSalesSection(CEposReportLoyaltyReconBlock& Block);

private:
	void WriteBlockTotals(CEposReportLoyaltyReconBlock& Block);

private:
	void InitialiseConsolidationBlocks();

private:
	void ConsolidateLogData(CString strRequestFile);
	void PurgeOldRequestAndResponseFiles(COleDateTime timeNow);

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportLoyaltyReconBlock, CEposReportLoyaltyReconBlock> m_arrayReportBlocks;
	CArray<int, int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolLoyaltyReconLog> m_ConsolLog;

private:
	CEposReportCustomSettingsLoyaltyRecon m_ReportSettings;
	CBusinessDateInfo m_BusinessDateInfo;

private:
	int m_nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> m_arrayFileInfo;
	
private:
	CString m_strLogError;
	CString m_strLoyaltyorSmartPay;
	CString m_strEcrmanOrStockman;
};

/**********************************************************************/
#endif
/**********************************************************************/
