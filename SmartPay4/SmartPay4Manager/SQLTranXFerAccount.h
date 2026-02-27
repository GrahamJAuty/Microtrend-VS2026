#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "SQLRowSetGroupFull.h"
#include "RecordXferPropertySheet.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranXFerAccount : public CSQLTranBase
{
public:
	void DoWork(CRecordXferData& RecordXferData, bool bNeedCreate, bool bHotlistReplace, bool bDoHotlist, int nHotlistType, CString strHotlistComment,__int64 nUserIDFrom, __int64 nUserIDTo);
	void AfterTransaction();

private:
	void Transfer(CRecordXferData& RecordXferData, CSQLRowAccountFull& RowAccount, CString strOtherID, bool bXferOut);
	void TransferValues(CRecordXferData& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut);
	bool TransferStamps(CSQLRowAccountFull& RowAccountFrom, CSQLRowAccountFull& RowAccountTo);

private:
	__int64 m_nUserIDFrom;
	__int64 m_nUserIDTo;
	bool m_bHotlistReplace;
	bool m_bCopyPurchaseHistory;
};

/**********************************************************************/
