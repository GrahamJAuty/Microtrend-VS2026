#pragma once
//*******************************************************************

class CStatus
{
public:
	CStatus(int nTerminalNo, const char* szImportFilename = "" );
	
public:
	int GetTerminalNo() { return m_nTerminalNo; }
	int GetStatusErrorNo() { return m_nStatusErrorNo; }
	CString GetImportFile() { return m_strImportFile; }
	CString GetStatusReply() { return m_strStatusReply; }

public:
	void SetStatusReply(const CString& strReply) { m_strStatusReply = strReply; }
	void SetStatusErrorNo(const int nErrorNo) { m_nStatusErrorNo = nErrorNo; }	
		
private:
	int m_nTerminalNo = 0;
	CString m_strImportFile = "";
	CString m_strStatusReply = "";
	int m_nStatusErrorNo = 0;
};

//*******************************************************************