#pragma once

//**********************************************************************

class CRecordSorter
{
public:
	CRecordSorter();
	void Add ( const char* szLabel, int nRecNo );
	void Update ( const char* szLabel, int nRecNo, bool bModifyIfFound );
	void Consolidate ( const char* szLabel, int nValue );

	int GetSize() { return m_arrayData.GetSize(); }
	int GetRecordNo ( int nIndex );
	CString GetLabel ( int nIndex );
	void RemoveAll() { m_arrayData.RemoveAll(); }

	int GetPercentPosition ( int nIndex );

private:
	CStringArray m_arrayData;
};
