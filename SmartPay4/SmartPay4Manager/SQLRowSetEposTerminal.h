#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRowEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLFieldValidatorEposTerminal.h"
//**********************************************************************

class CSQLRowSetEposTerminal
{
public:
	CSQLRowSetEposTerminal();

public:
	void LoadDatabaseEposTerminals();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowEposTerminal& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowEposTerminal& Row);
	void UpdateRow(int nPos, CSQLRowEposTerminal& Row);
	bool FindTerminal(int nTerminalNo, int& nPos);
	
private:
	CArray<CSQLRowEposTerminal, CSQLRowEposTerminal> m_arrayRows;
};

//**********************************************************************
