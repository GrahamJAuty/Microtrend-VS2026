#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "BatchEmailLog.h"
#include "ProcessedOrder.h"
#include "SMTPEmailHelpers.h"
/**********************************************************************/

class CProcessedOrderSheetMaker  
{
public:
	CProcessedOrderSheetMaker();	
	void SetTopName ( const char* szName ) { m_strTopName = szName; }
	void SetTopAddress1 ( const char* szAddress ) { m_strTopAddress1 = szAddress; }
	void SetTopAddress2 ( const char* szAddress ) { m_strTopAddress2 = szAddress; }
	void SetTopAddress3 ( const char* szAddress ) { m_strTopAddress3 = szAddress; }
	void SetTopPostcode ( const char* szPostcode ) { m_strTopPostcode = szPostcode; }
	void SetTopPhone ( const char* szPhone );
	void SetTopFax ( const char* szFax );
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetAddress1 ( const char* szAddress ) { m_strAddress1 = szAddress; }
	void SetAddress2 ( const char* szAddress ) { m_strAddress2 = szAddress; }
	void SetAddress3 ( const char* szAddress ) { m_strAddress3 = szAddress; }
	void SetPostcode ( const char* szPostcode ) { m_strPostcode = szPostcode; }
	void SetDeliverAsOrderedFlag ( bool bFlag ) { m_bDeliverAsOrdered = bFlag; }
	void SetDate();
	void CreateOrderSheet ( CProcessedOrder& Order, bool bPrint, bool bEmail = FALSE );
	
	void EmailOrderSheetMAPI ( CProcessedOrder& Order, bool bSend, bool bEmailLog );
	void EmailOrderSheetSMTP ( CSMTPEmailHelpers& SMTPEmailHelpers, CProcessedOrder& Order );

	void SetInstructions1 ( const char* szInstructions ) { m_strInstructions1 = szInstructions; }
	void SetInstructions2 ( const char* szInstructions ) { m_strInstructions2 = szInstructions; }
	void SetInstructions3 ( const char* szInstructions ) { m_strInstructions3 = szInstructions; }
	void SetInstructions4 ( const char* szInstructions ) { m_strInstructions4 = szInstructions; }
	void SetInstructions5 ( const char* szInstructions ) { m_strInstructions5 = szInstructions; }
	void SetEmailLog ( CBatchEmailLog* pLog ) { m_pEmailLog = pLog; }

	bool GetRepmanWarningFlag() { return m_bRepmanWarning; }

private:
	CString m_strName;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPostcode;
	CString m_strTopName;
	CString m_strTopAddress1;
	CString m_strTopAddress2;
	CString m_strTopAddress3;
	CString m_strTopPostcode;
	CString m_strTopPhone;
	CString m_strTopFax;
	CString m_strDate;
	CString m_strInstructions1;
	CString m_strInstructions2;
	CString m_strInstructions3;
	CString m_strInstructions4;
	CString m_strInstructions5;
	bool m_bDeliverAsOrdered;
	bool m_bRepmanWarning;
	CBatchEmailLog* m_pEmailLog;
};

/**********************************************************************/
#endif
/**********************************************************************/
