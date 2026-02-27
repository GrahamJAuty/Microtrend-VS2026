//#include "GlobalFunctions.h"
#include "Squid.hpp"

//*********************************************************************
// https://services.squidcard.com/SchoolService/School?wsdl
// https://services.squidcard.com/SchoolService/School?xsd=1
//
// live site = https://services.squidcard.com/SchoolService/School
//
// demo site = http://83.231.196.115:8080/SchoolService/School
// demo IssuerID	= "10312"	<- first one - now not used
// Demo Password	= sQuid
// demo IssuerID	= "10039"	<- use this one
// Demo Password	= squid	<- use this one
// Demo MerchantID  = 91000132
//
//*********************************************************************

static const char* szLIVE_IPADDRESS		= "services.squidcard.com";
static const char* szDEMO_IPADDRESS		= "83.231.196.115:8080";
static const char* szOBJECTNAME			= "/SchoolService/School";

//*********************************************************************

#define nTX_TRANSACTIONS	0
#define nTX_PAYMENTACKS		1

static const char* szTRUE = "true";

//*********************************************************************

CSSSquid::CSSSquid ( CSquidData* pData )
	: m_pData(pData)
	, m_bUseHttps(TRUE)
	, m_strSquidAccountNo(_T(""))
	, m_strSquidCardNo(_T(""))
	, m_strSquidCVV(_T(""))
	, m_bMoreRecordsAvailable(FALSE)
{
	m_strIPAddress	= pData->m_strIPAddress;
	m_strIssuerID	= pData->m_strIssuerID;
	m_strPassword	= pData->m_strPassword;
	m_strMerchantID	= pData->m_strMerchantID;

	if ( m_strIssuerID == "D3M0ID" )			// DEMOID - note 0=zero not letter O
	{
		m_strIPAddress	= szDEMO_IPADDRESS;
		m_strIssuerID	= "10039";				
		m_bUseHttps		= FALSE;
	}
}

/**********************************************************************
// Retrieve Card Details

<?xml version="1.0" encoding="UTF-8"?>
-<soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/">
  -<soap11:Body xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:tns="http://endpoint.webservice.host.squidcard.com/">
   -<tns:retrieveCardDetails>
   -<credentials>
    <issuerId>10039</issuerId>
    <password>squid</password>
   </credentials>
   <cardHolderReferenceNumber>N823432113104</cardHolderReferenceNumber>
  </tns:retrieveCardDetails>
 </soap11:Body>
</soap11:Envelope>


<?xml version="1.0"?>
-<S:Envelope xmlns:S="http://schemas.xmlsoap.org/soap/envelope/">
 -<S:Body>
  -<ns2:retrieveCardDetailsResponse xmlns:ns2="http://endpoint.webservice.host.squidcard.com/">
   -<return>
   <reasonCode>RSN001</reasonCode>
   <responseCode>RSN001</responseCode>
   <success>true</success>
    -<cardDetails>
    <cardNumber>6337991003900065</cardNumber>
    <cvv>798</cvv>
    -<purseDetails>
     <purseAccountNumber>633799003800028810</purseAccountNumber>
     <schemeId>38</schemeId>
    </purseDetails>
    -<purseDetails>
     <purseAccountNumber>633799005000003329</purseAccountNumber>
     <schemeId>50</schemeId>
    </purseDetails>
   </cardDetails>
  </return>
  </ns2:retrieveCardDetailsResponse>
 </S:Body>
</S:Envelope>

*******************************************************************/

CString CSSSquid::RetrieveCardDetails ( const char* szLoginReply, const char* szLinkReference1, const char* szLinkReference2 )
{
	CString strError = "";

	CString strReference	= szLinkReference1;					// UPN
	CString strLabel		= "cardHolderReferenceNumber";

	if ( strReference == "" )
	{
		strReference	= szLinkReference2;						//MIS_ID
		strLabel		= "externalReferenceNumber";
	}


	m_strSquidCardNo	= "";
	m_strSquidAccountNo = "";
	m_strSquidCVV		= "";		

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:tns", "http://endpoint.webservice.host.squidcard.com/" );
	s.Attribute ( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );

		s.Begin ( "tns:retrieveCardDetails" );
			AddLoginCredentials( &s );									// loginCredentials
			s.Element ( strLabel, strReference );
		s.End();

//*********** test vesrion - gets al card data
//		s.Begin ( "tns:retrieveAllCardDetails" );
//			AddLoginCredentials( &s );							// loginCredentials
//			s.Element ( "batchNumber", "1" );
//		s.End();
//***********

	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("RetrieveCardDetailsLogin.xml"), TRUE );			// for use when debugging

	if ( m_bUseHttps == FALSE )	strError = s.SendHttp  ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString() );
	else						strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());

	if ( ( strError = ValidateLoginReply ( &s, strError ) ) == "" )
	{
		CXMLNodeArray* p1 = s.FindNode ( "cardNumber" );
		if ( p1 != NULL )
		{
			m_strSquidCardNo = p1->m_strValue;							// sQuid card number

			p1 = s.FindNode ( "cvv" );
			if ( p1 != NULL )	m_strSquidCVV = p1->m_strValue;			// card ccv
			else				strError = "No account CVV supplied!";

			strError = ExtractPurseAccountNumber ( &s );				// extract purseAccountNumber
		}
		else
			strError = "No cardNumber supplied!";
	}

	return strError;
}

//**********************************************************************

CString CSSSquid::DisassociateCard ( const char* szLoginReply, const char* szSquidCardNo )
{
	CString strError = "";

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:tns", "http://endpoint.webservice.host.squidcard.com/" );
	s.Attribute ( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );

		s.Begin ( "tns:disassociateCard" );
			AddLoginCredentials( &s );							// loginCredentials
			s.Element ( "cardNumber", szSquidCardNo );			// existing sQuid Card number
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetDisassocateCardLogin.xml"), TRUE);			// for use when debugging
	}

	if (m_bUseHttps == FALSE)
	{
		strError = s.SendHttp(m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	}
	else
	{
		strError = s.SendHttps(m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	}

	strError = ValidateLoginReply ( &s, strError );

	return strError;
}

/**********************************************************************
// Get Payments

  <?xml version="1.0" ?> 
- <S:Envelope xmlns:S="http://schemas.xmlsoap.org/soap/envelope/">
-  <S:Body>
-   <ns2:collectRpusWithAckResponse xmlns:ns2="http://endpoint.webservice.host.squidcard.com/">
-    <return>
      <reasonCode>RSN2000</reasonCode> 
      <responseCode>RSN2000</responseCode> 
      <success>true</success> 
      <moreRpusToFollow>false</moreRpusToFollow> 
-     <rpus>
       <operation>CREDIT</operation> 
       <purseAccountNumber>633799005000000028</purseAccountNumber> 
       <rpuID>52649</rpuID> 
       <sequenceNumber>0</sequenceNumber> 
       <value>2500</value> 
      </rpus>
-     <rpus>
       <operation>CREDIT</operation> 
       <purseAccountNumber>633799005000000036</purseAccountNumber> 
       <rpuID>52669</rpuID> 
       <sequenceNumber>0</sequenceNumber> 
       <value>5000</value> 
      </rpus>
     </return>
    </ns2:collectRpusWithAckResponse>
   </S:Body>
  </S:Envelope>

************************************************************************/

CString CSSSquid::GetPayments ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader )
{
	CString strError = "";

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:tns", "http://endpoint.webservice.host.squidcard.com/" );
	s.Attribute ( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );

		s.Begin ( "tns:collectRpusWithAck" );
			AddLoginCredentials( &s );									// loginCredentials
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("GetPaymentsLogin.xml"), TRUE );			// for use when debugging

	if ( m_bUseHttps == FALSE )	strError = s.SendHttp  ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	else						strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());

	if ( ( strError = ValidateLoginReply ( &s, strError ) ) == "" )
	{
		CXMLNodeArray* p1 = s.FindNode ( "moreRpusToFollow" );			// see if more than 1000 waiting to be collected
		if ( p1->m_strValue == szTRUE )
			m_bMoreRecordsAvailable = TRUE;

		CSSUnsortedDatabase db;
		if ( db.CreateSSDatabase ( szCsvFilename, "", szHeader, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
		{
			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "rpus" ) > 0 )
			{
				for ( int n = 0 ; n < arFound.GetSize() ; n++ )		// number of records
				{
					db.AddBlankRecord();
					CXMLNodeArray* node = arFound.GetAt(n);
					for ( int i = 0 ; i < node->GetSize() ; i++ )	// number of items per record
					{
						if ( node->GetAt(i)->IsText() == TRUE )
							db.Set ( node->GetAt(i)->GetName(), node->GetAt(i)->GetValue() );
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
//*** Send data to sQuid ***********************************************
//**********************************************************************

CString CSSSquid::UploadTransactions ( const char* szSource, const char* szLoginReply )	{ return UploadData ( nTX_TRANSACTIONS, szSource, szLoginReply ); }
CString CSSSquid::UploadPaymentAcks  ( const char* szSource, const char* szLoginReply )	{ return UploadData ( nTX_PAYMENTACKS, szSource, szLoginReply ); }

CString CSSSquid::UploadData ( int nType, const char* szSource, const char* szLoginReply )
{
	CString strError = "";

	int nMaxTxCount = m_pData->m_nMaxTransactionLines;

	while ( TRUE )
	{
		int nLineCount = ::GetFileLineCount ( szSource );
		nLineCount -= 1;											// ignore header line

		int nTxCount = ( nLineCount > nMaxTxCount ) ? nMaxTxCount : nLineCount;
		if ( nTxCount > 0 )
		{
			if ( nType == nTX_TRANSACTIONS )	strError = SendTransactions ( szSource, szLoginReply, nTxCount );
			else								strError = SendPaymentAcks ( szSource, szLoginReply, nTxCount );
				
			if ( strError != "" )
				break;

			::RemoveFileLines ( szSource, nTxCount, TRUE );
		}
		else
			break;
	}
	return strError;
}

/**********************************************************************
// Send Transactions \ Send Balances \ Send Topups

  <?xml version="1.0" encoding="utf-8" ?> 
- <soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/">
-  <soap11:Body xmlns:tns="http://endpoint.webservice.host.squidcard.com/" xmlns:xs="http://www.w3.org/2001/XMLSchema">
-   <tns:sendTransaction>
-    <credentials>
      <issuerId>10312</issuerId> 
      <password>sQuid</password> 
     </credentials>
-    <transaction>
      <amount /> 
      <closingBalance>5000</closingBalance> 
      <dateAndTime>2012-09-17 14:15:00</dateAndTime> 
      <description /> 
      <merchantId>91000132</merchantId> 
      <purseAccountNumber>633799005000000028</purseAccountNumber> 
      <rpu>true</rpu> 
      <rpuSequence>0</rpuSequence> 
      <transactionNumber>0</transactionNumber> 
      <transactionType /> 
     </transaction>
-    <transaction>
      <amount>805</amount> 
      <closingBalance>5695</closingBalance> 
      <dateAndTime>2012-09-18 13:42:00</dateAndTime> 
      <description>ecr sale</description> 
      <merchantId>91000132</merchantId> 
      <purseAccountNumber>633799005000000028</purseAccountNumber> 
      <rpu>false</rpu> 
      <rpuSequence /> 
      <transactionNumber>0</transactionNumber> 
      <transactionType>PURCHASE</transactionType> 
     </transaction>
    </tns:sendTransaction>
   </soap11:Body>
  </soap11:Envelope>

reply
  <?xml version="1.0" ?> 
- <S:Envelope xmlns:S="http://schemas.xmlsoap.org/soap/envelope/">
-  <S:Body>
-   <ns2:sendTransactionResponse xmlns:ns2="http://endpoint.webservice.host.squidcard.com/">
-   <return>
     <reasonCode>RSN2000</reasonCode> 
     <responseCode>RSN2000</responseCode> 
     <success>true</success> 
    </return>
    </ns2:sendTransactionResponse>
   </S:Body>
  </S:Envelope>

*****************************************************************/

CString CSSSquid::SendTransactions ( const char* szSource, const char* szLoginReply, int nTxCount )
{
	CString strError = "";

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strError.Format ( "Cannot open source file ' %s ' ", szSource );
		return strError;
	}

	CCSV csvHeader;
	fileSource.Read ( csvHeader );	

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:tns", "http://endpoint.webservice.host.squidcard.com/" );
	s.Attribute ( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );

		s.Begin ( "tns:sendTransaction" );
			AddLoginCredentials( &s );							// loginCredentials

			CCSV csv;
			for ( int i = 0 ; i < nTxCount ; i++ )
			{
				fileSource.Read ( csv );
				s.Begin ( "transactions" );
				{
					for ( int i = 0 ; i < csv.GetSize() ; i++ )
						s.Element ( csvHeader.GetString(i), csv.GetString(i) );
				}
				s.End();
			}

		s.End();
	s.End();

	fileSource.Close();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("SendTransactionsLogin.xml"), TRUE );			// for use when debugging

	if ( m_bUseHttps == FALSE )	strError = s.SendHttp  ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	else						strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	
	strError = ValidateLoginReply ( &s, strError );

	return strError;
}

/**********************************************************************
// Send paymnet acknowledgements 

<?xml version="1.0" encoding="UTF-8"?>
-<soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/">
- <soap11:Body xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:tns="http://endpoint.webservice.host.squidcard.com/">
-  <tns:acknowledgeRpus>
-   <credentials>
     <issuerId>10039</issuerId>
     <password>squid</password>
    </credentials>
    <rpuIds>94153</rpuIds>
    <rpuIds>94157</rpuIds>
    <rpuIds>94154</rpuIds>
    <rpuIds>94158</rpuIds>
   </tns:acknowledgeRpus>
  </soap11:Body>
</soap11:Envelope>

*******************************************************************************/

CString CSSSquid::SendPaymentAcks ( const char* szSource, const char* szLoginReply, int nTxCount )
{
	CString strError = "";

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strError.Format ( "Cannot open source file ' %s ' ", szSource );
		return strError;
	}

	CCSV csvHeader;
	fileSource.Read ( csvHeader );								// header only = "rpuIds"

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:tns", "http://endpoint.webservice.host.squidcard.com/" );
	s.Attribute ( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );

		s.Begin ( "tns:acknowledgeRpus" );
			AddLoginCredentials( &s );								// loginCredentials

			CString strBuf;
			for ( int i = 0 ; i < nTxCount ; i++ )
			{
				if ( fileSource.ReadString ( strBuf ) == TRUE )
					s.Element ( csvHeader.GetString(0), strBuf );					
			}

		s.End();
	s.End();

	fileSource.Close();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("SendPaymentAcksLogin.xml"), TRUE );			// for use when debugging

	if ( m_bUseHttps == FALSE )	strError = s.SendHttp  ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	else						strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	
	strError = ValidateLoginReply ( &s, strError );

	return strError;
}

//**********************************************************************
// Add login credentials

void CSSSquid::AddLoginCredentials ( CSSSoap* pSoap )
{
	pSoap->Begin ( "credentials" );
		pSoap->Element ( "issuerId", m_strIssuerID );			// card isuer ID
		pSoap->Element ( "password", m_strPassword );			// card issuer password
	pSoap->End();
}

//**********************************************************************

CString CSSSquid::ValidateLoginReply ( CSSSoap* s, const char* szError )
{
	CString strError = szError;

	if ( strError == "" )										// check first for no TCP comms errors
	{
		CXMLNodeArray* p1 = s->FindNode ( "success" );			// look for result label
		if ( p1 != NULL )
		{
			if ( p1->m_strValue != szTRUE )						// check returned error
			{
				p1 = s->FindNode ( "reasonCode" );				// look for reason for failure
				if ( p1 != NULL )	strError.Format ( "Reason Code Error : %s", p1->m_strValue );
				else				strError = "No reasonCode node found in login reply!";
			}
		}
		else
			strError = "No success node found in login reply!";
	}

	return strError;
}

//**********************************************************************
// First ten digits of purse account number must be either
// pre prod prefix is 6337990050
// in prod a sharp catering purse would be 6337990082 the last eight digits are unique to the sQuid registration number.

CString CSSSquid::ExtractPurseAccountNumber ( CSSSoap* pSoap )
{
	CString strError = "No purseAccountNumber nodes supplied!";		// assume error

	CXMLNodeArray arFound;
	if ( pSoap->FindNodes ( arFound, "purseAccountNumber" ) > 0 )
	{
		strError = "No valid purseAccountNumber supplied!";			// assume different error

		for ( int n = 0 ; n < arFound.GetSize() ; n++ )				// number of records
		{
			CXMLNodeArray* node = arFound.GetAt(n);
			m_strSquidAccountNo = node->m_strValue;					// purse account number

			CString strPrefix = m_strSquidAccountNo.Left(10);		// account number

			if ( strPrefix == "6337990050" || strPrefix == "6337990082" )
			{
				strError = "";										// clear error message
				break;
			}
		}
	}

	return strError;
}

//**********************************************************************
