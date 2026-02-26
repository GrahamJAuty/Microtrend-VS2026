#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CDatabaseImportHandlerStockman  
{
public:
	CDatabaseImportHandlerStockman();
	void SetCreateStockFlag( bool bFlag ) { m_bCreateStock = bFlag; }
	void SetPluImportMethod( int nMethod ) { m_nImportMethodPlu = nMethod; }
	void SetStockImportMethod( int nMethod ) { m_nImportMethodStock = nMethod; }
	void SetBarcodeImportMethod( int nMethod ) { m_nImportMethodBarcode = nMethod; }
	
	int ImportPluFile ( const char* szFilename );
	int ImportStockFile ( const char* szFilenane );
	int ImportPluStockFile ( const char* szFilename );
	int ImportPluBarcodeFile ( const char* szFilename );
	int ImportBarcodeFile ( const char* szFilename );

	int GetImportTotalLines() { return m_nImportTotalLines; }
	int GetImportIgnoredLines() { return m_nImportIgnoredLines; }
	int GetImportPluAdded() { return m_nImportPluAdded; }
	int GetImportPluModified() { return m_nImportPluModified; }
	int GetImportBarcodeAdded() { return m_nImportBarcodeAdded; }
	int GetImportBarcodeModified() { return m_nImportBarcodeModified; }
	int GetImportStockAdded() { return m_nImportStockAdded; }
	int GetImportStockModified() { return m_nImportStockModified; }
	int GetImportSupplierAdded() { return m_nImportSupplierAdded; }
	int GetImportSupplierModified() { return m_nImportSupplierModified; }

private:
	void ResetCounters();
	int ImportPluFileInternal ( const char* szFilename );
	int ImportStockFileInternal ( const char* szFilename );
	
private:
	bool m_bCreateStock;
	int m_nImportMethodPlu;
	int m_nImportMethodStock;
	int m_nImportMethodBarcode;
	int m_nImportTotalLines;
	int m_nImportIgnoredLines;
	int m_nImportPluAdded;
	int m_nImportPluModified;
	int m_nImportBarcodeAdded;
	int m_nImportBarcodeModified;
	int m_nImportStockAdded;
	int m_nImportStockModified;
	int m_nImportSupplierAdded;
	int m_nImportSupplierModified;
};

/**********************************************************************/
#endif
/**********************************************************************/
