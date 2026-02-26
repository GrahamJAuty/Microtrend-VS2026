//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
//**********************************************************************
#include "SQLStaffAccountBuffer.h"
//**********************************************************************

void CSQLStaffAccountBufferItem::ValidateStaffGift()
{
	m_RowStaffGiftException.SetErrorCode(0);
	m_RowStaffGiftException.SetServerID(0);

	if (ValidateStaffGiftInternal() == TRUE)
	{
		m_RowAccountStaff.SetUserID(m_RowStaffGiftException.GetServerID());

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(m_RowAccountStaff, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			m_nStaffGiftMode = STAFFGIFTMODE_OK;
		}
		else
		{
			m_nStaffGiftMode = STAFFGIFTMODE_EXCEPTION;
			m_RowStaffGiftException.SetErrorCode(5);
		}
	}
	else
	{
		m_nStaffGiftMode = STAFFGIFTMODE_EXCEPTION;
	}
}

//**********************************************************************

bool CSQLStaffAccountBufferItem::ValidateStaffGiftInternal()
{
	if (m_RowStaffGiftException.GetServerNo() <= 0)
	{
		m_RowStaffGiftException.SetErrorCode(1);
		return FALSE;
	}

	CSQLRowStaffCard RowSC;
	RowSC.SetServerNo(m_RowStaffGiftException.GetServerNo());
	RowSC.SetFolderIndexDbNo(m_RowStaffGiftException.GetFolderIndexDbNo());
	RowSC.SetFolderIndexSetNo(m_RowStaffGiftException.GetFolderIndexSetNo());

	CSQLRepositoryStaffCard repoStaff;
	if (repoStaff.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		RowSC.SetFolderIndexSetNo(0);

		if (repoStaff.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			RowSC.SetFolderIndexDbNo(0);

			if (repoStaff.SelectRow(RowSC, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				m_RowStaffGiftException.SetErrorCode(2);
				return FALSE;
			}
		}
	}

	if (RowSC.GetUserID() <= 0)
	{
		m_RowStaffGiftException.SetErrorCode(3);
		return FALSE;
	}

	m_RowStaffGiftException.SetServerID(RowSC.GetUserID());

	if (m_RowStaffGiftException.GetServerID() == m_RowStaffGiftException.GetCustomerID())
	{
		m_RowStaffGiftException.SetErrorCode(4);
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************

bool CSQLStaffAccountBufferItem::ApplyStaffGift(CDatabase* pDatabase)
{
	bool bResult = FALSE;

	if (m_nStaffGiftMode == STAFFGIFTMODE_OK)
	{
		CSQLRowAccountFull RowCheck;
		RowCheck.SetUserID(m_RowAccountStaff.GetUserID());

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(RowCheck, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (m_RowAccountStaff.CompareRowVersion(RowCheck) == TRUE)
			{
				bResult = (repoAccount.UpdateRow(m_RowAccountStaff, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
			}
		}
	}
	else
	{
		CSQLRepositoryStaffGiftException repoStaff;
		bResult = (repoStaff.InsertRow(m_RowStaffGiftException, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
	}

	return bResult;
}

//**********************************************************************

bool CSQLStaffAccountBufferItem::PrepareAuditStaffGift(CAuditRecord_base& atc, int nAuditType)
{
	bool bResult = FALSE;

	if (STAFFGIFTMODE_OK == m_nStaffGiftMode)
	{
		double dGiftValue = m_RowStaffGiftException.GetValue();
		atc.SetSQL(m_RowAccountStaff, AUDIT_PC, TRUE);				
		
		m_RowAccountStaff.AddToPurse1(dGiftValue);

		atc.SetPurse1Transaction(dGiftValue);
		atc.SetPurse2Transaction(0);
		atc.SetCashTransaction(0);
		atc.SetSourceType(nAuditType);
		atc.SetApplicationNo(APPNO_ADDGIFT);
		/****/
		atc.SetPoints(m_RowAccountStaff.GetPoints());
		atc.SetPointsTD(m_RowAccountStaff.GetPointsToDate());
		atc.SetPurse1(m_RowAccountStaff.GetPurse1Balance());
		atc.SetPurse2(m_RowAccountStaff.GetPurse2Balance());
		atc.SetPurse1SpendTD(m_RowAccountStaff.GetPurse1SpendToDate());
		atc.SetPurse2SpendTD(m_RowAccountStaff.GetPurse2SpendToDate());
		atc.SetCashSpendTD(m_RowAccountStaff.GetCashSpendToDate());

		bResult = TRUE;
	}

	return bResult;
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CSQLStaffAccountBuffer::CSQLStaffAccountBuffer()
{
	Reset();
}

//**********************************************************************

void CSQLStaffAccountBuffer::Reset()
{
	m_arrayItems.RemoveAll();
}

//**********************************************************************

void CSQLStaffAccountBuffer::BufferStaffGift(CSQLRowStaffGiftException& RowException)
{
	bool bGotGift = FALSE;
	for (int n = 0; n < m_arrayItems.GetSize(); n++)
	{
		if (RowException.GetServerNo() == m_arrayItems[n].m_RowStaffGiftException.GetServerNo())
		{
			m_arrayItems[n].m_RowStaffGiftException.AddToValue(RowException.GetValue());
			bGotGift = TRUE;
			break;
		}
	}

	if (FALSE == bGotGift)
	{
		CSQLStaffAccountBufferItem StaffGift;
		StaffGift.m_nStaffGiftMode = STAFFGIFTMODE_NONE;
		StaffGift.m_RowStaffGiftException = RowException;
		m_arrayItems.Add(StaffGift);
	}
}

//**********************************************************************

void CSQLStaffAccountBuffer::ValidateStaffGifts()
{
	for (int n = 0; n < m_arrayItems.GetSize(); n++)
	{
		m_arrayItems[n].ValidateStaffGift();
	}
}

//**********************************************************************

bool CSQLStaffAccountBuffer::ApplyStaffGift(int n, CDatabase* pDatabase)
{
	bool bResult = FALSE;

	if ((n >= 0) && (n < m_arrayItems.GetSize()))
	{
		bResult = m_arrayItems[n].ApplyStaffGift(pDatabase);
	}

	return bResult;
}

//**********************************************************************

bool CSQLStaffAccountBuffer::PrepareAuditStaffGift(int n, CAuditRecord_base& atc, int nAuditType)
{
	bool bResult = FALSE;

	if ((n >= 0) && (n < m_arrayItems.GetSize()))
	{
		bResult = m_arrayItems[n].PrepareAuditStaffGift(atc, nAuditType);
	}

	return bResult;
}

//**********************************************************************



