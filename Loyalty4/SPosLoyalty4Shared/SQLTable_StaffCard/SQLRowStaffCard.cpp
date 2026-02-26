//**********************************************************************
#include "..\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "SQLRowStaffCard.h"
//**********************************************************************

CSQLRowStaffCard::CSQLRowStaffCard()
{
	Reset();
}

//**********************************************************************

void CSQLRowStaffCard::Reset()
{
	m_nFolderIndexDbNo = 0;
	m_nFolderIndexSetNo = 0;
	m_nServerNo = 1;
	m_nUserID = 0;
	/*****/
	m_strDisplayUserID = "";
	m_strDisplayAccountName = "";
}

//**********************************************************************

void CSQLRowStaffCard::SetDisplayUserInfo()
{
	SetDisplayUserID(System.FormatCardNo(m_nUserID, TRUE));

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(m_nUserID);

	CSQLRepositoryAccount repoAccount;
	if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		SetDisplayAccountName(RowAccount.GetFullname());
	}
	else
	{
		SetDisplayAccountName("Unknown");
	}
}

//**********************************************************************
