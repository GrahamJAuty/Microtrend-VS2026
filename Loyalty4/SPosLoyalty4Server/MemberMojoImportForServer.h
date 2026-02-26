#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************/

class CMemberMojoImportForServer
{
public:
	CMemberMojoImportForServer( CString strFilename );
	
public:
	void Execute();

private:
	void ResetFieldIdx();
	bool ReadFileLine(CSSFile& file, CString& strLine);
	bool ProcessHeaderLine(CString strHeader);
	void ProcessImportLines(CSSFile& file);
	bool GotDuplicateID(CSSFile& file);
	void LogError(CString strError, bool bForce = FALSE);

private:
	CString m_strFilename;
	
private:
	int m_nFieldIdxUserID;
	int m_nFieldIdxQRInfo;
	CArray<int, int> m_arrayHeader;
};

//**********************************************************************/
