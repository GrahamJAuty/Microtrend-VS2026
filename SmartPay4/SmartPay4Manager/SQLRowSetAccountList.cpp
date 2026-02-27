//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\GroupShiftValidator.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRecordSetAccountList.h"
#include "SQLRecordSetAccountSurname.h"
//**********************************************************************
#include "SQLRowSetAccountList.h"
//**********************************************************************

CSQLRowSetAccountList::CSQLRowSetAccountList()
{
	m_nUserIDPos = -1;
}

//**********************************************************************

void CSQLRowSetAccountList::SelectRows(__int64 nUserIDToFind)
{
	m_nUserIDPos = 0;
	m_arrayRows.RemoveAll();

	CSQLRecordSetAccountList RecordSet(NULL, RSParams_AccountList_NormalNoParams{});
	RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);

	while (RecordSet.IsEOF() == FALSE)
	{
		CSQLRowAccountList NewRow;
		NewRow.SetUserID(RecordSet.m_MD.m_nUserID);
		NewRow.SetUsername(RecordSet.m_MD.m_strUsername);
		NewRow.SetForename(RecordSet.m_MD.m_strForename);
		NewRow.SetSurname(RecordSet.m_MD.m_strSurname);
		NewRow.SetExpiryDate(RecordSet.m_MD.m_strExpiryDate);
		NewRow.SetGroupNo(RecordSet.m_MD.m_nGroupNo);
		NewRow.SetInactive(RecordSet.m_MD.m_bInactive);
		NewRow.SetPurse1Liability(atof(RecordSet.m_MD.m_strPurse1Liability));
		NewRow.SetPurse1Credit(atof(RecordSet.m_MD.m_strPurse1Credit));
		NewRow.SetPurse2Balance(atof(RecordSet.m_MD.m_strPurse2Balance));
		NewRow.SetPurse3Liability(atof(RecordSet.m_MD.m_strPurse3Liability));
		NewRow.SetPurse3Credit(atof(RecordSet.m_MD.m_strPurse3Credit));
		NewRow.SetPoints(RecordSet.m_MD.m_nPoints);
		NewRow.SetAlertCode(RecordSet.m_MD.m_nAlertCode);

		if (NewRow.GetUserID() == nUserIDToFind)
		{
			m_nUserIDPos = m_arrayRows.GetSize();
		}

		if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
		{
			switch (RecordSet.m_MD.m_nGroupShiftStatus)
			{
			case 1:
			case 2:
			{
				CSQLRowAccountFull RowAccountFull;
				RowAccountFull.SetGroupNo(RecordSet.m_MD.m_nGroupNo);
				RowAccountFull.SetGroupShiftStatus(RecordSet.m_MD.m_nGroupShiftStatus);
				RowAccountFull.SetGroupShiftStartDate(RecordSet.m_MD.m_strGroupShiftStartDate);
				RowAccountFull.SetGroupShiftEndDate(RecordSet.m_MD.m_strGroupShiftEndDate);
				RowAccountFull.SetGroupShiftFutureGroup(RecordSet.m_MD.m_nGroupShiftFutureGroup);

				COleDateTime dateNow = COleDateTime::GetCurrentTime();
				CGroupShiftValidator GroupShiftValidator(dateNow, RowAccountFull);

				if (GroupShiftValidator.UpdateAccount(RowAccountFull) == TRUE)
				{
					GroupShiftLogger.LogSystemChange(RowAccountFull, GroupShiftValidator);
				}

				NewRow.SetGroupNo(RowAccountFull.GetGroupNo());
			}
			break;
			}
		}

		m_arrayRows.Add(NewRow);

		RecordSet.MoveNext();
	}

	RecordSet.Close();
}

//**********************************************************************

void CSQLRowSetAccountList::SelectRowsForSurnameList(__int64 nUserIDToFind, bool bSortByGroup)
{
	m_nUserIDPos = 0;
	m_arrayRows.RemoveAll();

	CSQLRecordSetAccountSurname RecordSet(NULL);
	
	{
		CString strSort = "";
		if (TRUE == bSortByGroup)
		{
			strSort += Account::GroupNo.Label;
			strSort += ",";
		}
		strSort += Account::Surname.Label;
		
		RecordSet.m_strSort = strSort;
	}
	
	RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);

	while (RecordSet.IsEOF() == FALSE)
	{
		CSQLRowAccountList NewRow;
		NewRow.SetUserID(RecordSet.m_MD.m_nUserID);
		NewRow.SetUsername(RecordSet.m_MD.m_strUsername);
		NewRow.SetForename(RecordSet.m_MD.m_strForename);
		NewRow.SetSurname(RecordSet.m_MD.m_strSurname);
		NewRow.SetGroupNo(RecordSet.m_MD.m_nGroupNo);
		
		if (NewRow.GetUserID() == nUserIDToFind)
		{
			m_nUserIDPos = m_arrayRows.GetSize();
		}

		if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
		{
			switch (RecordSet.m_MD.m_nGroupShiftStatus)
			{
			case 1:
			case 2:
			{
				CSQLRowAccountFull RowAccountFull;
				RowAccountFull.SetGroupNo(RecordSet.m_MD.m_nGroupNo);
				RowAccountFull.SetGroupShiftStatus(RecordSet.m_MD.m_nGroupShiftStatus);
				RowAccountFull.SetGroupShiftStartDate(RecordSet.m_MD.m_strGroupShiftStartDate);
				RowAccountFull.SetGroupShiftEndDate(RecordSet.m_MD.m_strGroupShiftEndDate);
				RowAccountFull.SetGroupShiftFutureGroup(RecordSet.m_MD.m_nGroupShiftFutureGroup);

				COleDateTime dateNow = COleDateTime::GetCurrentTime();
				CGroupShiftValidator GroupShiftValidator(dateNow, RowAccountFull);

				if (GroupShiftValidator.UpdateAccount(RowAccountFull) == TRUE)
				{
					GroupShiftLogger.LogSystemChange(RowAccountFull, GroupShiftValidator);
				}

				NewRow.SetGroupNo(RowAccountFull.GetGroupNo());
			}
			break;
			}
		}

		m_arrayRows.Add(NewRow);

		RecordSet.MoveNext();
	}

	RecordSet.Close();
}

//**********************************************************************

void CSQLRowSetAccountList::GetRow(int nRow, CSQLRowAccountList& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************
