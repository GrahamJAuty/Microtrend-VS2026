//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetGroupFull.h"
//**********************************************************************

CSQLRowSetGroupFull::CSQLRowSetGroupFull()
{
}

//**********************************************************************

void CSQLRowSetGroupFull::LoadDatabaseGroups()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetGroupFull RecordSet(NULL, RSParams_Group_NormalNoParams{});

	CSQLRowGroupFull RowGroup;
	while (RecordSet.StepSelectAll(RowGroup) == TRUE)
	{
		m_arrayRows.Add(RowGroup);
	}
}

//**********************************************************************

bool CSQLRowSetGroupFull::FindGroup(int nGroupNo, int& nPos)
{
	//FIDDLE SHOULD BE BINARY SEARCH
	
	CSQLRowGroupFull RowGroup;
	for (nPos = 0; nPos < GetSize(); nPos++)
	{
		RowGroup = m_arrayRows.GetAt(nPos);
		
		if (RowGroup.GetGroupNo() >= nGroupNo)
		{
			return (RowGroup.GetGroupNo() == nGroupNo);
		}
	}

	return FALSE;
}

//**********************************************************************

void CSQLRowSetGroupFull::GetRow(int nRow, CSQLRowGroupFull& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetGroupFull::InsertRow(int n, CSQLRowGroupFull& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroupFull::UpdateRow(int n, CSQLRowGroupFull& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroupFull::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************
