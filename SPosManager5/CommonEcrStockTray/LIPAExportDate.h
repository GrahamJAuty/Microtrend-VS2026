#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CLIPAExportDate 
{
public:
	CLIPAExportDate();

public:
	const char* GetLastExportDate( bool bTemp );
	void SetLastExportDate( COleDateTime& date, bool bTemp );
	void SetLastExportDate( const char* sz, bool bTemp );
	bool GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate );

private:
	const char* GetDateFilename( bool bTemp );
	
private:
	CString m_strLastExportDate;
	CString m_strFilename;
	CString m_strDateFilename;
};

/**********************************************************************/

