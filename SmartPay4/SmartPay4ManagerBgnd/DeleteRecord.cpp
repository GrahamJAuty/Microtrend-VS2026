/*********************************************************************/
#include "SQLTranDeleteAccount.h"
/*********************************************************************/
#include "DeleteRecord.h"
/*********************************************************************/

CDeleteRecord::CDeleteRecord(CSQLRowAccountFull& SQLRowAccount, CString strComment, CString strLeaversYear, CWnd* pParent)
{
	bool bDoneDelete = FALSE;

	CSQLTranDeleteAccount Tran;
	Tran.BeginTrans();
	Tran.DoWork(SQLRowAccount, strComment, strLeaversYear);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		Tran.AfterTransaction();
		break;

	default:
		Tran.ShowErrorMessage("Delete Account");
		break;
	}
}

//**********************************************************************
