#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRowBromComPendingOrder.h"
//*******************************************************************

class CBromComPaymentExceptionReport  
{
public:
	CBromComPaymentExceptionReport ( const char* szLabel, CWnd* pParent );
	void Show();

private:
	int Create ( const char* szReportFile );
	void AddColumnText();
	void ShowTotalValue();
	void SaveLine(CSQLRowBromComPendingOrder& RowOrder, CConsolidatedDoubleByInt& item);

private:
	CString m_strReportLabel;
	CWnd* m_pParent;

	CReportFileWithEndText m_Report;
	CString m_strReportTitle;

	double m_dTotalValue;
};

//*******************************************************************
