#pragma once
/**********************************************************************/

struct CPluAutoImporterEODInfoPriceSet
{
	int m_nDbIdx;
	int m_nLocIdx;
	CString m_strFilename;
};

/**********************************************************************/

class CPluAutoImporterEODPriceSet  
{
public:
	CPluAutoImporterEODPriceSet();
	
public:
	void ImportPluIdleTime();
	void ImportPluExternal( int nLocIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextLoc();
	
private:
	void CheckLocIdx();
	
private:
	void ImportPluInternal( CPluAutoImporterEODInfoPriceSet& infoFile );
	
private:
	int m_nLocIdx;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CPluAutoImporterEODPriceSet PluAutoImporterEODPriceSet;
/**********************************************************************/

