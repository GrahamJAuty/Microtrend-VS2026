#pragma once
/**********************************************************************/
#include "EposReportAccountReceive.h"
/**********************************************************************/

class CEposReportAccountBooking : public CEposReportAccountReceive
{
public:
	CEposReportAccountBooking( CEposSelectArray& SelectArray );
	bool IsDepositReport(){ return TRUE; }

public:
	void AddColumns();
	bool CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv );
		
public:
	bool ValidateEventTime( int nLocIdx, int nAccountID, CString& strEventDate, CString& strEventTime );
	void SetConsolidationLocation( CTermFileListInfo& infoFile, CEposReportAccountReceiveInfo& infoReceive, bool& bGotDepositNames );
};

/**********************************************************************/

