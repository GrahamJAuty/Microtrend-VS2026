#pragma once
/**********************************************************************/

class CRecordSorter
{
public:
	CRecordSorter();
	void Add ( const char* szLabel, int nRecNo );
	void Update ( const char* szLabel, long nRecNo, bool bModifyIfFound );
	void Consolidate ( const char* szLabel, long lValue );
	void Remove ( const char* szLabel );

	int GetSize() { return m_arrayData.GetSize(); }
	int GetRecordNo ( int nIndex );
	const char* GetLabel ( int nIndex );
	void RemoveAll() { m_arrayData.RemoveAll(); }

	int GetPercentPosition ( int nIndex );

private:
	CStringArray m_arrayData;
	CString m_strLabel;
};

/**********************************************************************/
