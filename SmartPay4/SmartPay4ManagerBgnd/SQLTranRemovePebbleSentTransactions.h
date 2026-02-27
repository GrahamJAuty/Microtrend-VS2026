#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranRemovePebbleSentTransactions : public CSQLTranBase
{
public:
	CSQLTranRemovePebbleSentTransactions();

public:
	void DoWork();
	void AfterTransaction();

public:
	CString GetError() { return m_strError; }
	
private:
	CString m_strError;
};

/**********************************************************************/
