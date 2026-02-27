//**********************************************************************
#include "TucasiSOAP.hpp"
//**********************************************************************

static 	const char* szIPADDRESS_DEMO = "217.155.38.120";				//static const char* szIPADDRESS_DEMO = "demo.scoipm.co.uk";
static 	const char* szIPADDRESS_LIVE = "scopay.com";

//  http://217.155.38.120/eSCO/services/TransactionsService

//**********************************************************************

CSSTucasiSoap::CSSTucasiSoap ( CTucasiSoapData* pData )
	: m_pData(pData)
{
	m_bDemoSystem = FALSE;
}

//**********************************************************************

CString CSSTucasiSoap::GetPayments ( const char* szCsvFilename, const char* szLoginReply, const char* szHeader )
{
	CString strError = "";

	CString strLastTransaction = ::FormatInt64Value( m_pData->m_nLastPaymentID );

	CSSSoap s ( m_pData->m_strSoapVersion );

	if ( m_bDemoSystem == TRUE )
	{
		s.BeginSoap ( "Body" );
			s.Begin( "getOrderListV1a" );
			s.Attribute ( "xmlns", "http://217.155.38.120/eSCO/services/TransactionsService" );			// 	s.Attribute ( "xmlns", "http://demo.scoipm.co.uk/test/services/TransactionsService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				s.Element ( "lastTransaction", strLastTransaction );
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlLogin == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("PaymentLogin.xml"), TRUE );

		strError = s.SendHttp ( szIPADDRESS_DEMO, "/eSCO/services/TransactionsService", szLoginReply, TRUE, System.GetInternetAgentString() );	// strError = s.SendHttp ( szIPADDRESS_DEMO, "/test/services/TransactionsService", szLogInReply );
	}
	else
	{
		s.BeginSoap ( "Body" );
			s.Begin( "getOrderListV1a" );
			s.Attribute ( "xmlns", "https://www.scopay.com/eSCO/services/TransactionsService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				s.Element ( "lastTransaction", strLastTransaction );
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlLogin == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("PaymentLogin.xml"), TRUE );

		strError = s.SendHttps ( szIPADDRESS_LIVE, "/eSCO/services/TransactionsService", szLoginReply, TRUE, System.GetInternetAgentString());
	}

	if ( strError == "" )
	{
		remove ( szCsvFilename );

		CSSUnsortedDatabase db;
		if ( db.CreateSSDatabase ( szCsvFilename, "", szHeader, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
		{
			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "ns:return" ) > 0 )
			{
				for ( int n = 0 ; n < arFound.GetSize() ; n++ )			// number of records
				{
					db.AddBlankRecord();
					CXMLNodeArray* node = arFound.GetAt(n);
					for ( int i = 0 ; i < node->GetSize() ; i++ )		// number of items per record
					{
						if ( node->GetAt(i)->IsText() == TRUE )
							db.Set ( node->GetAt(i)->GetLocalName(), node->GetAt(i)->GetValue() );
					}
				}
			}

			if ( db.GetRecordCount() == 0 || m_pData->m_bKeepXmlFiles == FALSE )	//02/06/2017 - v1.15x
				CFileRecycler::SendToRecycleBin ( szLoginReply );								// delete original xml file

			db.Close();
		}
		else
			strError = db.GetError();
	}

	return strError;
}
//**********************************************************************

CString CSSTucasiSoap::SendBalances ( const char* szCsvBalances, const char* szLoginReply )
{
	CString strError = "";

	CSSFile fileSource;
	if ( fileSource.Open ( szCsvBalances, "rb" ) == FALSE )
	{
		strError.Format ( "Cannot open source file ' %s ' ", szCsvBalances );
		return strError;
	}

	CString strLastTransaction = ::FormatInt64Value( m_pData->m_nLastPaymentID );

	if ( m_bDemoSystem == TRUE )
	{
		CCSV csvHeader;
		fileSource.Read ( csvHeader );	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "setPupilBalancesV1a" );
			s.Attribute ( "xmlns", "http://217.155.38.120/eSCO/services/BalanceService" );		// s.Attribute ( "xmlns", "http://demo.scoipm.co.uk/test/services/BalanceService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				{
					CCSV csv;
					while ( fileSource.Read ( csv ) == TRUE )
					{
						s.Begin ( "pupilBalances" );
						{
							for ( int i = 0 ; i < csv.GetSize() ; i++ )
								s.Element ( csvHeader.GetString(i), csv.GetString(i) );
						}
						s.End();
					}
				}	
				s.Element ( "productNumForBalance", "" );
				s.Element ( "lastTransaction", strLastTransaction );
				s.Element ( "schoolID", m_pData->m_strSchoolID );
			s.End();
		s.End();

		fileSource.Close();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendBalanceData.xml"), TRUE );

		strError = s.SendHttp ( szIPADDRESS_DEMO, "/eSCO/services/BalanceService", szLoginReply, TRUE, System.GetInternetAgentString());	// strError = s.SendHttp ( szIPADDRESS_DEMO, "/test/services/BalanceService", szLoginReply );
	}
	else		// live system
	{
		CCSV csvHeader;
		fileSource.Read ( csvHeader );	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "setPupilBalancesV1a" );
			s.Attribute ( "xmlns", "https://www.scopay.com/eSCO/services/BalanceService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				{
					CCSV csv;
					while ( fileSource.Read ( csv ) == TRUE )
					{
						s.Begin ( "pupilBalances" );
						{
							for ( int i = 0 ; i < csv.GetSize() ; i++ )
								s.Element ( csvHeader.GetString(i), csv.GetString(i) );
						}
						s.End();
					}
				}	
				s.Element ( "productNumForBalance", "" );
				s.Element ( "lastTransaction", strLastTransaction );
				s.Element ( "schoolID", m_pData->m_strSchoolID );
			s.End();
		s.End();

		fileSource.Close();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendBalanceData.xml"), TRUE );

		strError = s.SendHttps ( szIPADDRESS_LIVE, "/eSCO/services/BalanceService", szLoginReply, TRUE, System.GetInternetAgentString());
	}

	return strError;
}

//**********************************************************************
/*
  <?xml version="1.0" encoding="UTF-8" ?> 
- <SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:ns3="http://TransactionFormats.Cashless.SOAP.common.tucasi/xsd" xmlns:ns4="http://Cashless.SOAP.common.tucasi/TransactionsServiceSoap11Binding" xmlns:ns2="http://demo.scoipm.co.uk/test/services/TransactionsService" xmlns:ns5="http://Cashless.SOAP.common.tucasi/TransactionsServiceSoap12Binding">
- <SOAP-ENV:Body>
- <ns2:uploadTransactionListV1a>
  <ns2:username /> 
  <ns2:password /> 
- <ns2:transactions>
   <ns3:description /> 
   <ns3:paymentAmount /> 
   <ns3:paymentDate /> 
   <ns3:pupilName /> 
   <ns3:pupilNumber /> 
   <ns3:schoolID /> 
   <ns3:transactionItemNumber>0</ns3:transactionItemNumber> 
   <ns3:transactionNumber>0</ns3:transactionNumber> 
  </ns2:transactions>
- <ns2:transactions>
   <ns3:description /> 
   <ns3:paymentAmount /> 
   <ns3:paymentDate /> 
   <ns3:pupilName /> 
   <ns3:pupilNumber /> 
   <ns3:schoolID /> 
   <ns3:transactionItemNumber>0</ns3:transactionItemNumber> 
   <ns3:transactionNumber>0</ns3:transactionNumber> 
  </ns2:transactions>
  </ns2:uploadTransactionListV1a>
  </SOAP-ENV:Body>
  </SOAP-ENV:Envelope>*/

CString CSSTucasiSoap::SendTransactions ( const char* szSource, const char* szLoginReply )
{
	CString strError = "";

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strError.Format ( "Cannot open source file ' %s ' ", szSource );
		return strError;
	}

	if ( m_bDemoSystem == TRUE )
	{
		CCSV csvHeader;
		fileSource.Read ( csvHeader );	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "uploadTransactionListV1a" );
			s.Attribute ( "xmlns", "http://demo.scoipm.co.uk/test/services/TransactionsService" );		// s.Attribute ( "xmlns", "http://217.155.38.120/eSCO/services/TransactionsService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				{
					CCSV csv;
					while ( fileSource.Read ( csv ) == TRUE )
					{
						s.Begin ( "transactions" );
						{
							s.Element ( "schoolID", m_pData->m_strSchoolID );
							for ( int i = 0 ; i < csv.GetSize() ; i++ )
								s.Element ( csvHeader.GetString(i), csv.GetString(i) );
						}
						s.End();
					}
					fileSource.Close();
				}
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendTransactionData.xml"), TRUE );

		strError = s.SendHttp ( szIPADDRESS_DEMO, "/eSCO/services/TransactionsService", szLoginReply, TRUE, System.GetInternetAgentString());	// strError = s.SendHttp ( szIPADDRESS_DEMO, "/test/services/TransactionsService", szLoginReply );
	}
	else
	{
		CCSV csvHeader;
		fileSource.Read ( csvHeader );	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "uploadTransactionListV1a" );
			s.Attribute ( "xmlns", "https://www.scopay.com/eSCO/services/TransactionsService" );
				s.Element ( "username", m_pData->m_strUserName );
				s.Element ( "password", m_pData->m_strPassword );
				{
					CCSV csv;
					while ( fileSource.Read ( csv ) == TRUE )
					{
						s.Begin ( "transactions" );
						{
							s.Element ( "schoolID", m_pData->m_strSchoolID );
							for ( int i = 0 ; i < csv.GetSize() ; i++ )
								s.Element ( csvHeader.GetString(i), csv.GetString(i) );
						}
						s.End();
					}
					fileSource.Close();
				}
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendTransactionData.xml"), TRUE );

		strError = s.SendHttps ( szIPADDRESS_LIVE, "/eSCO/services/TransactionsService", szLoginReply, TRUE, System.GetInternetAgentString());
	}

	return strError;
}

/********************************************************************/
/*	Balance Service Functions - NOT USED							*/
/********************************************************************

CString CSSTucasi::GetPupilBalanceV1a ( const char* szPupilID, const char* szFilename )
{
	CSSSoap s ( m_strSoapVersion );
	s.Begin ( "getPupilBalanceV1a" );
	s.Attribute ( "xmlns", "http://demo.scoipm.co.uk/test/services/BalanceService" );
		s.Element ( "username", m_strUsername );
		s.Element ( "password", m_strPassword );
		s.Element ( "pupilNumForBalance", szPupilID );
		s.Element ( "productNumForBalance", "" );
		s.Element ( "pupilName", "" );
		s.Element ( "schoolID", m_strSchoolID );
	s.End();

	const char* szObjectName = "/test/services/BalanceService";
	CString strError = s.Send ( szIPADDRESS_DEMO, szObjectName, szFilename );
	return strError;

/**********************************************************************

CString CSSTucasi::GetPupilBalancesV1a ( const char* szFilename )
{
	CSSSoap s ( m_strSoapVersion );
	s.Begin ( "getPupilBalancesV1a" );
	s.Attribute ( "xmlns", "http://demo.scoipm.co.uk/test/services/BalanceService" );
		s.Element ( "username", m_strUsername );
		s.Element ( "password", m_strPassword );
		s.Element ( "productNumForBalance", "" );
		s.Element ( "schoolID", m_strSchoolID );
	s.End();

	const char* szObjectName = "/test/services/BalanceService";

	CString strError = s.Send ( szIPADDRESS, szObjectName, szFilename );
	return strError;
}

********************************************************************/
