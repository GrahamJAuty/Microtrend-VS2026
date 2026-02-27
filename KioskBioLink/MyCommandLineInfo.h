#pragma once
/**********************************************************************/

class CMyCommandLineInfo : public CCommandLineInfo
{
public:
	virtual void ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast );
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt ( int n ) { return m_arrayParam.GetAt(n); }

private:
	CStringArray m_arrayParam;
};

/**********************************************************************/
