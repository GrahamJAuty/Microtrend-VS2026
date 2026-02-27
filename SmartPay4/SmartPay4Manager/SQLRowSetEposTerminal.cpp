//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRecordSetEposTerminal.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetEposTerminal.h"
//**********************************************************************

CSQLRowSetEposTerminal::CSQLRowSetEposTerminal()
{
}

//**********************************************************************

void CSQLRowSetEposTerminal::LoadDatabaseEposTerminals()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetEposTerminal RecordSet(NULL, RSParams_EposTerminal_NormalNoParams{});

	CSQLRowEposTerminal RowEP;
	while (RecordSet.StepSelectAll(RowEP) == TRUE)
	{
		m_arrayRows.Add(RowEP);
	}
}

//**********************************************************************

bool CSQLRowSetEposTerminal::FindTerminal(int nTerminalNo, int& nPos)
{
	//FIDDLE SHOULD BE BINARY SEARCH
	
	CSQLRowEposTerminal RowEP;
	for (nPos = 0; nPos < GetSize(); nPos++)
	{
		RowEP = m_arrayRows.GetAt(nPos);
		
		if (RowEP.GetTerminalNo() >= nTerminalNo)
		{
			return (RowEP.GetTerminalNo() == nTerminalNo);
		}
	}

	return FALSE;
}

//**********************************************************************

void CSQLRowSetEposTerminal::GetRow(int nRow, CSQLRowEposTerminal& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetEposTerminal::InsertRow(int n, CSQLRowEposTerminal& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetEposTerminal::UpdateRow(int n, CSQLRowEposTerminal& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetEposTerminal::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************
