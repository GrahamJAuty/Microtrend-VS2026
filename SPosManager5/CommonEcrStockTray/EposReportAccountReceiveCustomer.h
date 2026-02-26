#pragma once
/**********************************************************************/
#include "EposReportAccountReceive.h"
/**********************************************************************/

class CEposReportAccountReceiveCustomer : public CEposReportAccountReceive
{
public:
	CEposReportAccountReceiveCustomer( CEposSelectArray& SelectArray );
	bool IsDepositReport(){ return FALSE; }

public:
	void AddColumns();
	void GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	bool CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv );	
};

/**********************************************************************/

