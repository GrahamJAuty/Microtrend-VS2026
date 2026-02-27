#pragma once
/**********************************************************************/
#include "TucasiJSONData.h"
/**********************************************************************/

class CTucasiJsonExport  
{
public:
	CTucasiJsonExport ( CTucasiJsonData* pData, CWnd* pParent );

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

	void ResendTransactions ( const char* szCsvWebFilename );

//******************************************************************
#endif
//******************************************************************

	const char* GetError() { return m_strError; }
	bool SendUpdates();
	bool SendBalances ( const char* szCsvFilename );
	
private:
	void SendTransactions ( const char* szCsvFilename );

private:
	CWnd* m_pParent; 
	CTucasiJsonData* m_pData;

	CString m_strError;
};

//******************************************************************
