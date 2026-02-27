//*******************************************************************
#include "TucasiSOAP.hpp"
//*******************************************************************
#include "TucasiSoapExport.h"
//*******************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************

CTucasiSoapExport::CTucasiSoapExport()
	: m_strError(_T(""))
{
	m_data.Read();												// Read System data
}

//**********************************************************************
// csvfile in Tucasi balamnce file format ( with header line )

bool CTucasiSoapExport::SendBalances ( const char* szWebCsvFilename, const char* szLoginReply )
{
	CSSTucasiSoap tu ( &m_data );
	m_strError = tu.SendBalances ( szWebCsvFilename, szLoginReply );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//**********************************************************************
// csvfile in Tucasi transaction file format ( with header line )

bool CTucasiSoapExport::SendTransactions ( const char* szWebCsvFilename, const char* szLoginReply )
{
	CSSTucasiSoap tu ( &m_data );
	m_strError = tu.SendTransactions ( szWebCsvFilename, szLoginReply );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//*******************************************************************
