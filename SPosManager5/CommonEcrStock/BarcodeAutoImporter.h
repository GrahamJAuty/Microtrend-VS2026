#pragma once
/**********************************************************************/

class CBarcodeAutoImporter  
{
public:
	CBarcodeAutoImporter();
	
public:
	void ImportBarcodeIdleTime();
	void ImportBarcodeExternal( int nDbIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextDb();
	
private:
	void CheckDbIdx();
	
private:
	void ImportBarcodeInternal( int nDbIdx );
	
private:
	int m_nDbIdx;
	bool m_bReadyToGo;
	CString m_strImportFile;
};

/**********************************************************************/
extern CBarcodeAutoImporter BarcodeAutoImporter;
/**********************************************************************/

