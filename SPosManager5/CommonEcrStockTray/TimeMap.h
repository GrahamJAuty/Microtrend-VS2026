/**********************************************************************/
#pragma once
/**********************************************************************/

class CTimeMap
{
public:
	CTimeMap(void);
	void BuildMap( int nTimeSliceFileNo );
	const char* GetSortOrder( int nHour, int nMinute );
	const char* GetSortOrderName( const char* szSortOrder );

public:
	~CTimeMap(void);

private:
	CStringArray m_arraySortOrder;
	CStringArray m_arraySharedNames;
	CStringArray m_arrayUniqueNames;

private:
	CString m_strSortOrderOut;
	CString m_strSortOrderName;
};

/**********************************************************************/

