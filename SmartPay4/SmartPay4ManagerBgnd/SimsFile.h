#pragma once

//*******************************************************************

class CSimsFile
{
public:
	CSimsFile(void);
	~CSimsFile(void);

	bool Open ( const char* szFilename );
	void Close();
	bool ReadLine();
	int GetPercentPosition();
	CString GetError() { return m_strError; }

	void SetSIMSRequired ( bool bReqd ) { m_bSIMSRequired = bReqd; }
	void SetUPNRequired ( bool bReqd )  { m_bUPNRequired  = bReqd; }
	void SetPIDRequired ( bool bReqd )  { m_bPIDRequired  = bReqd; }

	bool HavePID()		{ return ( m_nIndexPID		== -1 )	? FALSE : TRUE; }
	bool HaveUPN()		{ return ( m_nIndexUPN		== -1 )	? FALSE : TRUE; }
	bool HaveSimsNo()	{ return ( m_nIndexAdno		== -1 )	? FALSE : TRUE; }
	bool HaveForename()	{ return ( m_nIndexForename	== -1 )	? FALSE : TRUE; }
	bool HaveSurname()	{ return ( m_nIndexSurname	== -1 )	? FALSE : TRUE; }
	bool HaveCardName()	{ return ( m_nIndexCardName	== -1 )	? FALSE : TRUE; }
	bool HaveDOB()		{ return ( m_nIndexDOB		== -1 )	? FALSE : TRUE; }
	bool HaveYear()		{ return ( m_nIndexYear		== -1 )	? FALSE : TRUE; }
	bool HaveReg()		{ return ( m_nIndexReg		== -1 )	? FALSE : TRUE; }
	bool HaveEmail()	{ return ( m_nIndexEmail	== -1 )	? FALSE : TRUE; }
	bool HaveGender()	{ return ( m_nIndexGender	== -1 )	? FALSE : TRUE; }
	bool HaveFSM()		{ return ( m_nIndexFreeMeal	== -1 )	? FALSE : TRUE; }

	CString GetPID()		{ return m_strPID; }
	CString GetUPN()		{ return m_strUPN; }
	CString GetAdno()		{ return m_strAdno; }
	CString GetForename()	{ return m_strForename; }
	CString GetSurname()	{ return m_strSurname; }
	CString GetCardName()	{ return m_strCardName; }
	CString GetYear()		{ return m_strYear; }
	CString GetReg()		{ return m_strReg; }
	CString GetEmail()		{ return m_strEmail; }
	CString GetDOB()		{ return m_strDOB; }
	CString GetGender()		{ return m_strGender; }
	CString GetFSM()		{ return m_strFreeMeal; }
	bool IsFSM()			{ return  ( m_strFreeMeal == "T" ) ? TRUE : FALSE; }

private:
	void GetTextFromCSVLine( CCSV& csv, int nIndex, CString& strText );

private:
	CString m_strError;

	bool m_bSIMSRequired;
	bool m_bUPNRequired;
	bool m_bPIDRequired;

	bool m_bFileOpen;
	int m_nIndexPID;
	int m_nIndexUPN;
	int m_nIndexAdno;
	int m_nIndexForename;
	int m_nIndexSurname;
	int m_nIndexCardName;
	int m_nIndexDOB;
	int m_nIndexYear;
	int m_nIndexReg;
	int m_nIndexEmail;
	int m_nIndexFreeMeal;
	int m_nIndexGender;

	CSSFile m_file;
	CString m_strPID;
	int m_nPIDTextNo;
	CString m_strUPN;
	CString m_strAdno;
	CString m_strForename;
	CString m_strSurname;
	CString m_strCardName;
	CString m_strDOB;
	CString m_strYear;
	CString m_strReg;
	CString m_strEmail;
	CString m_strFreeMeal;
	CString m_strGender;
};
