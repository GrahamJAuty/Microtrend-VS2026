#pragma once
/**********************************************************************/
 
struct CBaseGroupInfo
{
public:
	CBaseGroupInfo();

public:
	int m_nGroupNo;
	CString m_strReportText;
	CString m_strDefText;
};

/**********************************************************************/

class CGroupCSVRecordBase : public CCSVRecord
{
public:
	CGroupCSVRecordBase();
	virtual ~CGroupCSVRecordBase() {}

public:
	int GetGroupNo() { return m_nGroupNo; }
	virtual const char* GetEposText() = 0;
	virtual const char* GetRepText() = 0;

public:
	void SetGroupNo ( int nGroupNo );
	virtual void SetEposText ( const char* sz ) = 0;
	virtual void SetRepText ( const char* sz ) = 0;

public:
	virtual const recInt GetGroupNoRange() = 0;
	virtual void SetDefaultGroup( int nGroupNo ) = 0;
	
public:
	CString StripSpaces ( const char* szText );
	virtual const char* GetReportText();
	virtual const char* GetDefaultText();
			
protected:
	int m_nGroupNo;
	
protected:
	CString m_strReportText;
	CString m_strDefaultText;
};

/**********************************************************************/

class CGroupCSVArrayBase : public CSharedCSVArray
{
public:
	CGroupCSVArrayBase();

public:
	bool FindGroupByNumber ( int nGroupNo, int& nIndex );
	const char* GetGroupDisplayText( int nGroupNo );
	const char* GetGroupReportText( int nGroupNo );

public:
	virtual void GetGroupInfo( int nIndex, CBaseGroupInfo& info ) = 0;
	virtual bool GetListDataLine( int nIndex, CString& strLine ) = 0;
	virtual const recInt GetGroupNoRange() = 0;
	virtual void RememberSettings() = 0;
	virtual void RestoreSettings() = 0;

public:
	bool FindNextFreeGroupNumber( int& nInsertPos, int& nGroupNo );
	virtual bool CanEditGroupZero(){ return FALSE; }

protected:
	CString m_strPrefix;
	CString m_strDisplayText;
	CString m_strReportText;
	CString m_strListDataLine;

protected:
	CStringArray m_arraySaved;
};

/**********************************************************************/
