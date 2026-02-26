//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
//**********************************************************************
#include "SQLRowSetStaffCard.h"
//**********************************************************************

CSQLRowSetStaffCard::CSQLRowSetStaffCard()
{
}

//**********************************************************************

void CSQLRowSetStaffCard::LoadDatabaseCards()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetStaffCard RecordSet(NULL, RSParams_StaffCard_NormalNoParams{});

	CSQLRowStaffCard RowSC;
	while (RecordSet.StepSelectAll(RowSC) == TRUE)
	{
		RowSC.SetDisplayUserInfo();
		AddOrInsertRow(RowSC);
	}
}

//**********************************************************************

bool CSQLRowSetStaffCard::FindCard(int nServerNo, int nFolderIndexDbNo, int nFolderIndexSetNo, int& nPos)
{
	//FIDDLE SHOULD BE BINARY SEARCH
	for (nPos = 0; nPos < GetSize(); nPos++)
	{
		CSQLRowStaffCard RowSC;
		RowSC = m_arrayRows.GetAt(nPos);
		
		if ( (RowSC.GetServerNo() == nServerNo) && ( RowSC.GetFolderIndexDbNo() == nFolderIndexDbNo ) && (RowSC.GetFolderIndexSetNo() == nFolderIndexSetNo))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//**********************************************************************

void CSQLRowSetStaffCard::GetRow(int nRow, CSQLRowStaffCard& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetStaffCard::InsertRow(int n, CSQLRowStaffCard& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetStaffCard::UpdateRow(int n, CSQLRowStaffCard& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetStaffCard::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************

int CSQLRowSetStaffCard::AddOrInsertRow(CSQLRowStaffCard& RowNew)
{
	int nPos = -1;

	if (GetSize() == 0)
	{
		m_arrayRows.Add(RowNew);
		nPos = 0;
	}
	else
	{
		CSQLRowStaffCard RowEnd;
		GetRow(GetSize() - 1, RowEnd);

		if (RowEnd.GetConsolidationPos() < RowNew.GetConsolidationPos())
		{
			m_arrayRows.Add(RowNew);
			nPos = GetSize() - 1;
		}
		else
		{
			bool bSavedRow = FALSE;

			for (nPos = 0; nPos < GetSize(); nPos++)
			{
				CSQLRowStaffCard RowExisting;
				GetRow(nPos, RowExisting);

				if ( RowExisting.GetConsolidationPos() == RowNew.GetConsolidationPos() )
				{
					UpdateRow(nPos, RowNew);
					bSavedRow = TRUE;
					break;
				}
				else if (RowExisting.GetConsolidationPos() > RowNew.GetConsolidationPos())
				{
					InsertRow(nPos, RowNew);
					bSavedRow = TRUE;
					break;
				}
			}

			if (FALSE == bSavedRow)
			{
				m_arrayRows.Add(RowNew);
				nPos = GetSize() - 1;
			}
		}
	}

	return nPos;
}

//**********************************************************************
