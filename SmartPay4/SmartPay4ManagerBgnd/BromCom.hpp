//**********************************************************************
#include "BromComData.h"
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRowBromComTransaction.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//**********************************************************************

class CSSBromCom
{
public:
	CSSBromCom ( CBromComData* pData );

	CString GetBromComPaymentOrders(const char* szLoginReply, const char* szCsvFilename, const char* szHeader, CWorkingDlg& WorkingDlg);
	CString GetBromComPaymentOrderItems ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader, CReportConsolidationArray<CSortedIntItem>& arrayOrderID, CWorkingDlg& WorkingDlg, bool& bNoItemsButOK);
	CString SendBromComBalance ( const char* szLoginReply, int nPersonId, double dBalance);
	CString SendBromComTransaction (const char* szLoginReply, CSQLRowBromComTransaction& RowTran );

private:
	CBromComData* m_pData;
};

//**********************************************************************
