#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

class CZDeliveryImporterEcrman  
{
public:
	CZDeliveryImporterEcrman();
	void LookForDeliveries( int nLocIdx, int nEcrIdx );
	
private:
	void ProcessPendingFiles();
	bool ProcessPendingFile( CString& strPath, CString& strFilename );
	
private:
	int m_nLocIdx;
	int m_nEcrIdx;
	int m_nDbIdx;
	int m_nNetworkIdx;
	CStringArray m_arrayFilesToImport;
};

/**********************************************************************/
#endif
/**********************************************************************/
