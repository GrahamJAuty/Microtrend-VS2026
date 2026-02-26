#pragma once
/**********************************************************************/

class CDatabaseImportHandlerCommon  
{
public:
	CDatabaseImportHandlerCommon();	
	void SetImportMethod( int nMethod ) { m_nImportMethod = nMethod; }
	int ImportServerFile ( const char* szFilename );
	int GetImportTotalLines() { return m_nImportTotalLines; }
	int GetImportIgnoredLines() { return m_nImportIgnoredLines; }
	int GetImportRecordsAdded() { return m_nImportRecordsAdded; }
	int GetImportRecordsModified() { return m_nImportRecordsModified; }
	
private:
	void ResetCounters();

private:
	int m_nImportMethod;
	int m_nImportTotalLines;
	int m_nImportIgnoredLines;
	int m_nImportRecordsAdded;
	int m_nImportRecordsModified;
};

/**********************************************************************/


