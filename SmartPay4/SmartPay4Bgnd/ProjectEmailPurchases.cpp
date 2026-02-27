/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDB.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\EmailPurchases.h"
/**********************************************************************/

CEmailPurchases::CEmailPurchases()
{
	Init();
}

/**********************************************************************/

void CEmailPurchases::Execute()
{
	int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

	if ( nDayNo == m_data.m_nPurchaseDay )
	{
		ScanDatabase();
	}
}

//*******************************************************************

void CEmailPurchases::ScanDatabase()
{
	CEmailLogDatabase db;
	if (db.Open(Filenames.GetEmailLogDatabasePathname(2), DB_READWRITE) == DB_ERR_NONE)
	{
		CSQLRecordSetAccountFull RecordSet(NULL, 
			RSParams_AccountFull_NoParams{}, FALSE);

		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			bool bDummy = FALSE;
			CString strError = "";
			CreateEmail(RowAccount, db, bDummy, strError);
		}
		db.Close();
	}
}

//*******************************************************************
