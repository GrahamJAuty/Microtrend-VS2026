#pragma once
/**********************************************************************/

struct CSmartPayGroupShiftLogLine
{
public:
	CSmartPayGroupShiftLogLine() { Reset(); }
	bool Read(CString str);
	
	void Reset()
	{
		m_strActionDate = "";
		m_strActionTime = "";
		m_nActionSource = 0;
		m_nUserID = 0;
		m_strUsername = "";
		m_nCurrentGroupNo = 0;
		m_nOriginalStatus = 0;
		m_strOriginalDateFrom = "";
		m_strOriginalDateTo = "";
		m_nOriginalGroupNo = 0;
		m_nEditedStatus = 0;
		m_strEditedDateFrom = "";
		m_strEditedDateTo = "";
		m_nEditedGroupNo = 0;
	}

public:
	CString GetActionSourceString();
	CString GetOriginalStatusString() { return GetStatusString(m_nOriginalStatus); }
	CString GetEditedStatusString() { return GetStatusString(m_nEditedStatus); }

public:
	CString GetActionDate() { return m_strActionDate; }
	CString GetActionTime() { return m_strActionTime; }
	int GetActionSource() { return m_nActionSource; }
	__int64 GetUserID() { return m_nUserID; }
	CString GetUsername() { return m_strUsername; }
	int GetCurrentGroupNo() { return m_nCurrentGroupNo; }
	int GetOriginalStatus() { return m_nOriginalStatus; }
	CString GetOriginalDateFrom() { return m_strOriginalDateFrom; }
	CString GetOriginalDateTo() { return m_strOriginalDateTo; }
	int GetOriginalGroupNo() { return m_nOriginalGroupNo; }
	int GetEditedStatus() { return m_nEditedStatus; }
	CString GetEditedDateFrom() { return m_strEditedDateFrom; }
	CString GetEditedDateTo() { return m_strEditedDateTo; }
	int GetEditedGroupNo() { return m_nEditedGroupNo; }

public:
	void SetActionDate( CString str ) { m_strActionDate = str; }
	void SetActionTime( CString str ) { m_strActionTime = str; }
	void SetActionSource( int n ) { m_nActionSource = n; }
	void SetUserID( __int64 n ) { m_nUserID = n; }
	void SetUsername(CString str) { m_strUsername = str; }
	void SetCurrentGroupNo(int n) { m_nCurrentGroupNo = n; }
	void SetOriginalStatus( int n ) { m_nOriginalStatus = n; }
	void SetOriginalDateFrom( CString str ) { m_strOriginalDateFrom = str; }
	void SetOriginalDateTo( CString str ) { m_strOriginalDateTo = str; }
	void SetOriginalGroupNo( int n ) { m_nOriginalGroupNo = n; }
	void SetEditedStatus( int n ) { m_nEditedStatus = n; }
	void SetEditedDateFrom( CString str ) { m_strEditedDateFrom = str; }
	void SetEditedDateTo( CString str ) { m_strEditedDateTo = str; }
	void SetEditedGroupNo( int n ) { m_nEditedGroupNo = n; }

private:
	CString GetStatusString(int nStatus);

private:
	CString m_strActionDate;
	CString m_strActionTime;
	int m_nActionSource;
	__int64 m_nUserID;
	CString m_strUsername;
	int m_nCurrentGroupNo;
	int m_nOriginalStatus;
	CString m_strOriginalDateFrom;
	CString m_strOriginalDateTo;
	int m_nOriginalGroupNo;
	int m_nEditedStatus;
	CString m_strEditedDateFrom;
	CString m_strEditedDateTo;
	int m_nEditedGroupNo;
};

/**********************************************************************/

class CSmartPayGroupShiftFile
{
public:
	CSmartPayGroupShiftFile( const char* szFilename = "" );				
	~CSmartPayGroupShiftFile();

	bool Open();
	void Close();
	
	bool ReadLine ( CSmartPayGroupShiftLogLine& line );
	void JumpIn ( const char* szDate );
	
	int GetFileSize();
	int GetFilePos();
	void SetFilePos( int nPos );
	int GetPercentPosition();

private:
	CString m_strFilename;
	CSSFile m_fileIn1;
	bool m_bLogFileOpen;
};

/**********************************************************************/
