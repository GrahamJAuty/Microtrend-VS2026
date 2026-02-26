#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolDiscount.h"
#include "EposReportCustomSettings.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

class CEposReportDiscountBase : public CEposReport
{
public:
	CEposReportDiscountBase( CEposSelectArray& SelectArray );
	~CEposReportDiscountBase();

	virtual void Consolidate();
	virtual bool CreateReport(){ return FALSE; }

protected:
	void CreateDiscountSection( CEposReportDiscountBlock& Block, bool bBlockTotals );
	void ClearGrandTotals();
	
protected:
	virtual void InitialiseConsolidationBlocks(){}
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo ){}

protected:
	void InitialiseConsolidationBlocksStageOne();
	void InitialiseConsolidationBlocksStageTwo();

protected:
	void WriteUnderline();
	void WriteGrandTotals();
	void WriteTotals( bool bGrandTotal, int nEntityType, double dDiscount, double dMarkdown, double dMixMatch, double Premium, double dPMS );

protected:
	int m_nServerBlockIdxStart;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportDiscountBlock,CEposReportDiscountBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CEposReportCustomSettingsDiscount m_ReportSettings;
	CReportPluInfoFinder m_PluInfoFinder;

protected:
	int m_nAuditLevel;
	double m_dGrandTotalDiscount;
	double m_dGrandTotalMarkdown;
	double m_dGrandTotalPremium;
	double m_dGrandTotalMixMatch;
	double m_dGrandTotalPMS;
	
private:
	int m_nLineNo;
};

/**********************************************************************/
