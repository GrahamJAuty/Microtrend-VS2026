//*******************************************************************
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLDefines.h"
//*******************************************************************
#include "ServerWrapper.h"
//*******************************************************************

//*******************************************************************
// Add new record to database & create csv reply line ( not if MiFare-2 interface )
// ID=nnn already in csvReply line 

bool CServerWrapper::AddNewRecord( const char* szAccountID, CCSV* pCsvLine, CCSV* pCsvReply)
{
	bool bReply = TRUE;

	CSQLRowAccountFull AccountRow;
	AccountRow.SetUserID(_atoi64(szAccountID));
	AccountRow.SetGroupNo(Server.GetUnknownGroup());

	if (pCsvLine != NULL)
	{
		ExtractNewRecordDetails(AccountRow, pCsvLine);		// extract details from epos line
	}

	CSQLRepositoryAccount RepoAccount;
	if ( RepoAccount.InsertRow( AccountRow,NULL ).GetSQLError() != SQLCRUD_ERR_NONE)
	{ 
		pCsvReply->Add(StringErrorCodeAddRecord());
		pCsvReply->Add(StringErrorTextAddRecord());
		bReply = FALSE;
	}
	else
	{
		AuditNewRecord(AccountRow);								// audit new record details
		SaveRequestDetails(AccountRow, pCsvReply, FALSE);		// create epos reply 
	}

	return bReply;
}

//**********************************************************************
// extract new database record details - ID= first field in csv line

void CServerWrapper::ExtractNewRecordDetails(CSQLRowAccountFull& AccountRow, CCSV* pCsv)
{
	for (int i = 1; i < pCsv->GetSize(); i++)
	{
		CString strField = pCsv->GetString(i);

		strField.MakeUpper();
		strField.TrimLeft(' ');
		strField.TrimRight(' ');

		if (strField.Left(3) == "N1=")				// Surname
		{
			CString strName = strField.Mid(3);
			AccountRow.SetSurname(strName);

			if (AccountRow.GetUsername() == "")				// see if have a user name
			{
				AccountRow.SetUsername(strName);			// no - set to surname
			}
		}
		else if (strField.Left(3) == "N2=")			// Forename
		{
			AccountRow.SetForename(strField.Mid(3));
		}
		else if (strField.Left(3) == "I1=")			// Info1
		{
			AccountRow.SetInfo1(strField.Mid(3));
		}
		else if (strField.Left(3) == "GR=")			// Group
		{
			AccountRow.SetGroupNo(atoi(strField.Mid(3)));
		}
	}
}

//*******************************************************************

void CServerWrapper::AuditNewRecord(CSQLRowAccountFull& RowAccount)
{
	CSQLAuditRecord_base atc;
	atc.Set(&RowAccount, AUDIT_SERVER);
	atc.SetApplicationNo(APPNO_INSERT);
	atc.SaveNewLine();
}

//*******************************************************************

bool CServerWrapper::CanAddNewRecord(const char* szUserID)
{
	CString strUserID = szUserID;

	if (strUserID == "")
	{
		return FALSE;
	}

	if (strUserID.GetLength() > System.GetUserIDLength())
	{
		return FALSE;
	}

	if (System.GetInterfaceType() == nINTERFACE_MIFAREv2)
	{
		return FALSE;
	}

	if (strUserID == "0")														// cash account
	{
		return TRUE;
	}

	if (FALSE == m_bIsEposRequest)
	{
		return FALSE;
	}

	return Server.GetAddUnknownsFlag();
}

//*******************************************************************
