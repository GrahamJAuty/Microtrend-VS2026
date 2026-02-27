#pragma once
/**********************************************************************/
#include "TucasiSOAPData.h"
/**********************************************************************/

class CTucasiSoapExport  
{
public:
	CTucasiSoapExport();
	bool SendTransactions ( const char* szWebCsvFilename, const char* szLoginReply );
	bool SendBalances ( const char* szWebCsvFilename, const char* szLoginReply );
	CString  GetError() { return m_strError; }

private:
	CTucasiSoapData m_data;
	CString m_strError;
};

/**********************************************************************/
