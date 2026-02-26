#pragma once
/**********************************************************************/
#include "AuditRecord.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "RecordTransferDlg.h"
#include "SQLRowSetGroup.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranXFerAccount : public CSQLTranBase
{
public:
	void DoWork(CRecordTransferInfo& RecordXferData, bool bNeedCreate, bool bHotlistReplace, bool bDoHotlist, CString strHotlistComment,__int64 nUserIDFrom, __int64 nUserIDTo);
	void AfterTransaction();

private:
	void Transfer(CRecordTransferInfo& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut);
	void TransferValues(CRecordTransferInfo& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut);
	bool TransferStamps(CSQLRowAccountFull& RowAccountFrom, CSQLRowAccountFull& RowAccountTo);
	
private:
	__int64 m_nUserIDFrom;
	__int64 m_nUserIDTo;
	bool m_bHotlistReplace;
	bool m_bCopyPurchaseHistory;
};

/**********************************************************************/
