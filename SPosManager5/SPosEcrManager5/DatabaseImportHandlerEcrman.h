#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

class CDatabaseImportHandlerEcrman  
{
public:
	CDatabaseImportHandlerEcrman();	
	void SetPluImportMethod( int nMethod ) { m_nImportMethodPlu = nMethod; }
	void SetBarcodeImportMethod( int nMethod ) { m_nImportMethodBarcode = nMethod; }
	int ImportPluFile ( const char* szFilename );
	int ImportPluBarcodeFile ( const char* szFilename );
	int ImportBarcodeFile ( const char* szFilename );
	int GetImportTotalLines() { return m_nImportTotalLines; }
	int GetImportIgnoredLines() { return m_nImportIgnoredLines; }
	int GetImportPluAdded() { return m_nImportPluAdded; }
	int GetImportPluModified() { return m_nImportPluModified; }
	int GetImportBarcodeAdded() { return m_nImportBarcodeAdded; }
	int GetImportBarcodeModified() { return m_nImportBarcodeModified; }

private:
	void ResetCounters();

private:
	int m_nImportMethodPlu;
	int m_nImportMethodBarcode;
	int m_nImportTotalLines;
	int m_nImportIgnoredLines;
	int m_nImportPluAdded;
	int m_nImportPluModified;
	int m_nImportBarcodeAdded;
	int m_nImportBarcodeModified;
};

/**********************************************************************/
#endif
/**********************************************************************/


