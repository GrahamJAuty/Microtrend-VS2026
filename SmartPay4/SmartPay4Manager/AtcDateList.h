#pragma once

class CAtcDateList
{
public:
	CAtcDateList ( const char* szFilename );
	int GetSize() { return m_arrayDates.GetSize(); }
	const char* GetAtDate ( int nIndex );
	void RemoveEntries ( CSSDate& pDate );

	int GetAtCount ( int nIndex );

private:
	void Flush();

private:
	CStringArray m_arrayDates;
	CString m_strLastDate;
	CString m_strDate;
	int m_nCount;
};
