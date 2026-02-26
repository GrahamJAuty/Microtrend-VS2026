#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CExportDate 
{
public:
	CExportDate();
	
	void Read();
	void Write();
	const char* GetCSVLine();
	const char* GetDate() { return m_strDate; }
	const char* GetLastPluFile() { return m_strLastPluFile; }
	const char* GetLastStockFile() { return m_strLastStockFile; }

	void SetDate ( const char* szDate ) { m_strDate = szDate; }
	void SetDate ( CTime& time );
	void SetLastPluFile ( const char* szFile ) { m_strLastPluFile = szFile; }
	void SetLastStockFile ( const char* szFile ) { m_strLastStockFile = szFile; }

private:
	CString m_strDate;
	CString m_strLastPluFile;
	CString m_strLastStockFile;
	CString m_strCSVLine;
};

/**********************************************************************/

class CCSVExportDate : public CCSV
{
public:
	CCSVExportDate ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetDate() { return GetString(0); }
	const char* GetLastPluFile() { return GetString(1); }
	const char* GetLastStockFile() { return GetString(2); }
};

/**********************************************************************/
extern CExportDate ExportDate;		/* global */
/**********************************************************************/
