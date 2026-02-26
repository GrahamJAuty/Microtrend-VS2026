#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "EposReportGeneral.h"
/**********************************************************************/

class CEposReportGeneralDorbiere : public CEposReportGeneral
{
public:
	CEposReportGeneralDorbiere( CEposSelectArray& SelectArray );

public:
	virtual bool CreateReport();
	bool CreateExport();

private:
	virtual void ConsolidatePaymentBuffer( int nWeekday );

private:
	void CreateSalesSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile );
	void CreatePaymentSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, double& dDorbiereCashTotal );
	void CreatePaidIOSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, CString& strHeader, double& dPaidInTotal, double& dPaidOutTotal );
	void CreateCashSummary( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, double dCashTotal, double dPaidIn, double dPaidOut );

private:
	virtual void GetConsolidationParams( CEposReportConsolParamsStandard& Params );
	virtual const char* GetReportNameInternal( int nReportType );
};

/**********************************************************************/
#endif
/**********************************************************************/
