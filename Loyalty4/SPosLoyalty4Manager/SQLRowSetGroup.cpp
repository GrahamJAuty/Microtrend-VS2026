//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetGroup.h"
//**********************************************************************

CSQLRowSetGroup::CSQLRowSetGroup()
{
}

//**********************************************************************

void CSQLRowSetGroup::LoadDatabaseGroups()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetGroup RecordSet(NULL, RSParams_Group_NormalNoParams{});

	CSQLRowGroup RowGroup;
	while (RecordSet.StepSelectAll(RowGroup) == TRUE)
	{
		m_arrayRows.Consolidate(RowGroup);
	}
}

//**********************************************************************

bool CSQLRowSetGroup::FindGroup(int nGroupNo, int& nPos)
{
	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(nGroupNo);
	return m_arrayRows.Find(RowGroup, nPos);
}

//**********************************************************************

void CSQLRowSetGroup::GetRow(int nRow, CSQLRowGroup& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		m_arrayRows.GetAt(nRow, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroup::InsertRow(int n, CSQLRowGroup& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroup::UpdateRow(int n, CSQLRowGroup& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetGroup::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************
