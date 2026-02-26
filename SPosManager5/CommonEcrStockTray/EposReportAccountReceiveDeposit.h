#pragma once
/**********************************************************************/
#include "EposReportAccountReceive.h"
/**********************************************************************/

class CEposReportAccountReceiveDeposit : public CEposReportAccountReceive
{
public:
	CEposReportAccountReceiveDeposit( CEposSelectArray& SelectArray );
	CEposReportAccountReceiveDeposit( CEposSelectArray& SelectArray, const char* szCustomParams );
	bool IsDepositReport(){ return TRUE; }

public:
	void AddColumns();
	void GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	bool CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv );
};

/**********************************************************************/

