#pragma once
/**********************************************************************/
 
class CEposReportFile
{
public:
	CEposReportFile(void);
	~CEposReportFile();

public:
	void SetCreateMD5HashFlag(bool b) 
	{ 
		m_bCreateMD5Hash = b; 
	}

	CString GetMD5Hash()
	{
		return m_MD5Stream.GetResult();
	}

	bool GotDataLine()
	{
		return m_bGotDataLine;
	}

public:
	bool Open( const char* szFilename)							
	{ 
		return m_ReportFile.Open( szFilename ); 
	}

	void SetStyle1( const char* szStyle )
	{ 
		m_ReportFile.SetStyle1( szStyle );
	}

	void AddColumn( const char* szName, UINT wAlign, int nWidth )	
	{ 
		m_ReportFile.AddColumn( szName, wAlign, nWidth ); 
	}

	void Close()											
	{ 
		m_ReportFile.Close(); 
	}

public:	
	void WriteReportDataLine(const char* szLine) 
	{
		WriteReportLine(szLine, TRUE);
	}
	
	void WriteReportMiscLine(const char* szLine)
	{ 
		WriteReportLine(szLine, FALSE); 
	}

	void WriteHiddenLine(const char* sz)
	{
		m_ReportFile.WriteLine(sz);
	}

	void AddSubColumn(int nIndex, CString strDescription, UINT wTabAlign, double dPercentWidth)
	{
		m_ReportFile.AddSubColumn( nIndex, strDescription, wTabAlign, dPercentWidth );
	}

	void HandleNewPage();
	void LockBlankLines( int nCount );
	void RequestBlankLines( int nCount );
	void PreventEmptyFile();

private:
	void WriteReportLine(const char* szLine, bool bData);

private:
	CReportFile m_ReportFile;

private:
	CSSMD5Stream m_MD5Stream;
	bool m_bCreateMD5Hash;

private:
	bool m_bFirstLineOnPage;
	bool m_bLockedBlankLines;
	bool m_bGotReportLine;
	bool m_bGotDataLine;
	int m_nBlankLineCount;
};

/**********************************************************************/
