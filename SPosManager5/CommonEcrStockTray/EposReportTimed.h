#pragma once
/**********************************************************************/
#include "EposReportConsolTimed.h"
#include "EposReportConsolTran.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

class CEposReportTimed : public CEposReport
{
public:
	CEposReportTimed( CEposSelectArray& SelectArray );
	~CEposReportTimed();

	virtual void Consolidate();
	virtual bool CreateReport();
	virtual bool CreateGraph();
	virtual bool CheckGraph();

protected:
	virtual bool GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel ) = 0;
	virtual const char* GetDateOrTimeReport( CString& strLabel ) = 0;

protected:
	void CreateSalesSection( CEposReportTimedBlock& Block );
	
protected:	
	void WriteBlockTotals( CEposReportTimedBlock& Block );

protected:
	void InitialiseConsolidationBlocks();
	void PostConsolidateSalesTax( CEposReportTimedBlock& Block, CArray<CEposReportConsolTimedTax,CEposReportConsolTimedTax>& arraySales );

protected:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportTimedBlock,CEposReportTimedBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranDateOrTime> m_arrayTranDateOrTime;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;

protected:
	CString m_strDateOrTimeColumn;
	CString m_strDateOrTimeReport;
};

/**********************************************************************/