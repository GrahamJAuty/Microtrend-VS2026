#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPluAutoImporterCSV  
{
public:
	CPluAutoImporterCSV();
	
public:
	void ImportPluIdleTime();
	void ImportPluExternal( int nDbIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextDb();
	
private:
	void CheckDbIdx();
	
private:
	void GetImportFileList( int nDbIdx, CReportConsolidationArray<CSortedStringItem>& arrayFilenames );
	void ImportPluInternal( int nDbIdx, const char* szFilename );
	
private:
	int m_nDbIdx;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CPluAutoImporterCSV PluAutoImporterCSV;
/**********************************************************************/

