#pragma once
/**********************************************************************/
#define IMPORT_METHOD_ALL			0
#define IMPORT_METHOD_NEWONLY		1
#define IMPORT_METHOD_EXISTONLY		2
//**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************/

class CDatabaseImportForServer
{
public:
	CDatabaseImportForServer( const char* szFilename );
	void SetImportMethod ( int n ) { m_nImportMethod = n; }
	bool Execute();

protected:
	virtual bool IsValidFile();
	
protected:
	CString m_strFilename;

	int m_nIndexCardNo;
	int m_nAdded;
	int m_nImportMethod;

	int m_nRead;
	int m_nInvalid;
};

//**********************************************************************/
