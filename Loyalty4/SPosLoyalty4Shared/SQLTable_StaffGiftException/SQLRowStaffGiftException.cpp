//**********************************************************************
#include "..\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "SQLRowStaffGiftException.h"
//**********************************************************************

CSQLRowStaffGiftException::CSQLRowStaffGiftException()
{
	Reset();
}

//**********************************************************************

void CSQLRowStaffGiftException::Reset()
{
	m_nLineID = 0;
	m_nCustomerID = 0;
	m_nServerNo = 1;
	m_nFolderIndexDbNo = 0;
	m_nFolderIndexSetNo = 0;
	m_nServerID = 0;
	m_strDate = "";
	m_strTime = "";
	m_dValue = 0.0;
	m_nErrorCode = 0;
}

//**********************************************************************

void CSQLRowStaffGiftException::SetDisplayUserInfo()
{
	CSQLRepositoryAccount repoAccount;

	SetDisplayCustomerID(System.FormatCardNo(m_nCustomerID, TRUE));

	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(m_nCustomerID);

		if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			SetDisplayCustomerName(RowAccount.GetFullname());
		}
		else
		{
			SetDisplayCustomerName("Unknown");
		}
	}

	m_strDisplayServerID.Format("%2.2d \\ %2.2d \\ %3.3d \\ %s",
		m_nFolderIndexDbNo,
		m_nFolderIndexSetNo,
		m_nServerNo,
		(const char*) System.FormatCardNo(m_nServerID, TRUE));

	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(m_nServerID);
		if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			SetDisplayServerName(RowAccount.GetFullname());
		}
		else
		{
			SetDisplayServerName("Unknown");
		}
	}
}

//**********************************************************************
