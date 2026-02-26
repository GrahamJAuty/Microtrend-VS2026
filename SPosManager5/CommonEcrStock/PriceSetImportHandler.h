#pragma once
/**********************************************************************/

class CPriceSetImportHandler  
{
public:
	CPriceSetImportHandler();
	int ImportFile ( const char* szFilename, bool bCanSetStatus = FALSE );

	int GetImportTotalLines() { return m_nImportTotalLines; }
	int GetImportIgnoredLines() { return m_nImportIgnoredLines; }
	int GetImportPluModified() { return m_nImportPluModified; }

private:
	void ResetCounters();

private:
	int m_nImportTotalLines;
	int m_nImportIgnoredLines;
	int m_nImportPluModified;
};

/**********************************************************************/
