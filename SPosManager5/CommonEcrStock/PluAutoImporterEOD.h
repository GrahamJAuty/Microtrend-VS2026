#pragma once
/**********************************************************************/

struct CPluAutoImporterEODInfo
{
	int m_nDbIdx;
	int m_nLocIdx;
	CString m_strFilenameOriginal;
	CString m_strFilenamePriceSet;
};

/**********************************************************************/

class CPluAutoImporterEOD  
{
public:
	CPluAutoImporterEOD();
	
public:
	void ImportPluIdleTime();
	void ImportPluExternal( int nDbIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextLoc();
	
private:
	void CheckLocIdx();
	
private:
	void ImportPluInternal( CPluAutoImporterEODInfo& infoFile );
	
private:
	int m_nLocIdx;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CPluAutoImporterEOD PluAutoImporterEOD;
/**********************************************************************/

