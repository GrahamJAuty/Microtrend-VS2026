//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SystemData.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountUserID.h"
//**********************************************************************
#include "SQLRecordSetAccountList.h"
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

	if (System.GetAccountPreloadFlag() == FALSE)
	{
		CSQLRecordSetAccountUserID RecordSet(NULL, RSParams_AccountUserId_NormalNoParams{});
		RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);

		while (RecordSet.IsEOF() == FALSE)
		{
			CSQLRowAccountList NewRow;
			NewRow.SetACListUserID(RecordSet.m_MD.m_nUserID);
			NewRow.SetACListDataLoadedFlag(FALSE);

			if (0 == m_nUserIDPos)
			{
				if (NewRow.GetACListUserID() == nUserIDToFind)
				{
					m_nUserIDPos = m_arrayRows.GetSize();
				}
			}

			m_arrayRows.Add(NewRow);

			RecordSet.MoveNext();
		}

		RecordSet.Close();
	}
	else
	{
		CSQLRecordSetAccountList RecordSet(NULL, RSParams_AccountList_NormalNoParams{});
		RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);

		while (RecordSet.IsEOF() == FALSE)
		{
			CSQLRowAccountList NewRow;
			NewRow.SetACListUserID(RecordSet.m_MD.m_nUserID);
			NewRow.SetACListDataLoadedFlag(TRUE);
			NewRow.SetACListFullname(RecordSet.m_MD.m_strFullname);
			NewRow.SetACListGroupNo(RecordSet.m_MD.m_nGroupNo);
			NewRow.SetACListExpiryDate(RecordSet.m_MD.m_strExpiryDate);
			NewRow.SetACListInfo1(RecordSet.m_MD.m_strInfo1);
			NewRow.SetACListInfo2(RecordSet.m_MD.m_strInfo2);
			NewRow.SetACListInfo3(RecordSet.m_MD.m_strInfo3);
			NewRow.SetACListInfo4(RecordSet.m_MD.m_strInfo4);
			NewRow.SetACListAddress1(RecordSet.m_MD.m_strAddress1);
			NewRow.SetACListAddress5(RecordSet.m_MD.m_strAddress5);
			NewRow.SetACListInactive(RecordSet.m_MD.m_bInactive);
			NewRow.SetACListPurse1Balance(atof(RecordSet.m_MD.m_strPurse1Balance));
			NewRow.SetACListPurse1SpendToDate(atof(RecordSet.m_MD.m_strPurse1SpendToDate));
			NewRow.SetACListPurse2Balance(atof(RecordSet.m_MD.m_strPurse2Balance));
			NewRow.SetACListPurse2SpendToDate(atof(RecordSet.m_MD.m_strPurse2SpendToDate));
			NewRow.SetACListCashSpendToDate(atof(RecordSet.m_MD.m_strCashSpendToDate));
			NewRow.SetACListPoints(RecordSet.m_MD.m_nPoints);
			NewRow.SetACListPointsToDate(RecordSet.m_MD.m_nPointsToDate);
			NewRow.SetACListHotlistCode(RecordSet.m_MD.m_nHotlistCode);
			NewRow.SetACListAlertCodes(RecordSet.m_MD.m_strAlertCodes);

			if (NewRow.GetACListUserID() == nUserIDToFind)
			{
				m_nUserIDPos = m_arrayRows.GetSize();
			}

			m_arrayRows.Add(NewRow);

			RecordSet.MoveNext();
		}

		RecordSet.Close();
	}
}

//**********************************************************************

void CSQLRowSetAccountList::FindUserID(__int64 nUserID)
{
	m_nUserIDPos = 0;

	for (int n = 0; n < m_arrayRows.GetSize(); n++)
	{
		if (m_arrayRows[n].GetACListUserID() == nUserID)
		{
			m_nUserIDPos = n;
			break;
		}
	}
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

void CSQLRowSetAccountList::SetRow(int nRow, CSQLRowAccountList& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		m_arrayRows.SetAt(nRow, Row);
	}
}

//**********************************************************************
