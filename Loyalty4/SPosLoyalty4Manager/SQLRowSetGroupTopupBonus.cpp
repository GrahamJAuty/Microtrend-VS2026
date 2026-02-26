//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetGroupTopupBonus.h"
//**********************************************************************

CSQLRowSetGroupTopupBonus::CSQLRowSetGroupTopupBonus()
{
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::LoadDatabaseGroups()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetGroupTopupBonus RecordSet(NULL, RSParams_GroupTopupBonus_NormalNoParams{});

	CSQLRowGroupTopupBonus RowGroup;
	while (RecordSet.StepSelectAll(RowGroup) == TRUE)
	{
		m_arrayRows.Consolidate(RowGroup);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::GetRow(int nRow, CSQLRowGroupTopupBonus& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		m_arrayRows.GetAt(nRow, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::InsertRow(int n, CSQLRowGroupTopupBonus& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::UpdateRow(int n, CSQLRowGroupTopupBonus& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::RemoveGroupTopups(int nGroupNo)
{
	CSQLRowGroupTopupBonus RowGroup;
	RowGroup.SetGroupNo(nGroupNo);
	RowGroup.SetTopupNo(0);
	RowGroup.SetPurseNo(0);

	int nPos = 0;
	m_arrayRows.Find(RowGroup, nPos);
	
	while ((nPos >= 0) && (nPos < GetSize()))
	{
		m_arrayRows.GetAt(nPos, RowGroup);
		if (RowGroup.GetGroupNo() != nGroupNo)
		{
			break;
		}

		m_arrayRows.RemoveAt(nPos);
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::GetGroupTopups(int nGroupNo, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopups)
{
	arrayGroupTopups.RemoveAll();

	CSQLRowGroupTopupBonus RowGroup;
	RowGroup.SetGroupNo(nGroupNo);
	RowGroup.SetTopupNo(0);
	RowGroup.SetPurseNo(0);

	int nPos = 0;
	m_arrayRows.Find(RowGroup, nPos);

	int nLastTopupNo = 0;
	int nLastPurseNo = 0;

	while ((nPos >= 0) && (nPos < GetSize()))
	{
		m_arrayRows.GetAt(nPos, RowGroup);

		if (RowGroup.GetGroupNo() != nGroupNo)
		{
			break;
		}

		if (RowGroup.GetTopupNo() < nLastTopupNo)
		{
			break;
		}

		if ((RowGroup.GetTopupNo() == nLastTopupNo) && (RowGroup.GetPurseNo() <= nLastPurseNo))
		{
			break;
		}

		nLastTopupNo = RowGroup.GetTopupNo();
		nLastPurseNo = RowGroup.GetPurseNo();

		arrayGroupTopups.DirectAdd(RowGroup);
		nPos++;
	}
}

//**********************************************************************

void CSQLRowSetGroupTopupBonus::SetGroupTopups(int nGroupNo, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopups)
{
	RemoveGroupTopups(nGroupNo);
	
	CSQLRowGroupTopupBonus RowGroup;
	RowGroup.SetGroupNo(nGroupNo);
	RowGroup.SetTopupNo(0);
	RowGroup.SetPurseNo(0);

	int nPos = 0;
	m_arrayRows.Find(RowGroup, nPos);

	int nLastTopupNo = 0;
	int nLastPurseNo = 0;

	for (int i = 0; i < arrayGroupTopups.GetSize(); i++)
	{
		CSQLRowGroupTopupBonus RowGroup;
		arrayGroupTopups.GetAt(i, RowGroup);

		if (RowGroup.GetGroupNo() != nGroupNo)
		{
			break;
		}

		if (RowGroup.GetTopupNo() < nLastTopupNo)
		{
			break;
		}

		if ((RowGroup.GetTopupNo() == nLastTopupNo) && (RowGroup.GetPurseNo() <= nLastPurseNo))
		{
			break;
		}

		nLastTopupNo = RowGroup.GetTopupNo();
		nLastPurseNo = RowGroup.GetPurseNo();

		m_arrayRows.InsertAt( nPos++, RowGroup);
	}
}

//**********************************************************************
