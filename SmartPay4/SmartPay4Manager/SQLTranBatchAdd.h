#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranBatchAdd : public CSQLTranBase
{
public:
	void DoWork(__int64 nFirstNewUserID,__int64 nLastNewUserID, __int64 nRecordsToAdd, CArray<__int64, __int64>& arrayInUse, CString strComment);

public:
	__int64 GetUserIDToSelect() { return m_nUserIDToSelect; }
	
private:
	__int64 m_nUserIDToSelect;
};

/**********************************************************************/
