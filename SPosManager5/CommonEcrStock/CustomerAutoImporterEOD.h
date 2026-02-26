#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct CCustomerAutoImporterEODInfo
{
	int m_nLocIdx;
	CString m_strFilenameOriginal;
};

/**********************************************************************/

class CCustomerAutoImporterEOD  
{
public:
	CCustomerAutoImporterEOD();
	
public:
	void ImportCustomerIdleTime();
	void ImportCustomerExternal( int nLocIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextLoc();
	
private:
	void CheckLocIdx();
	
private:
	void ImportCustomerInternal( CCustomerAutoImporterEODInfo& infoFile );
	
private:
	int m_nLocIdx;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CCustomerAutoImporterEOD CustomerAutoImporterEOD;
/**********************************************************************/
#endif
/**********************************************************************/
