#include "ParentMail.hpp"

//https://ccservice.parentmail2.co.uk/cgi-bin/CashlessCateringProxyWSDL.cgi?PMAIL
//Service :	https://ccservice.parentmail2.co.uk/CashlessProxyService

static const char* szOBJECTNAME = "CashlessProxyService";

//********************************************************************
//Demo SOAP Credentials :
//Username  : "school2027"
//Password  : "zVgVWiJUvy"
//School ID : "20"

//Username  : "SharpCCTestUn123"
//Password  : "SharpCCTestPw123"
//School ID : "105"
//***********************************************************************

CSSParentMail::CSSParentMail ( CParentMailData* pData )
	: m_pData(pData)
	, m_nTxCount(0)
	, m_nLastTransactionID(0)
{
	m_strAccountID	= "";
	m_strIPAddress	= "ccservice.parentmail2.co.uk";
}

//*******************************************************************
/*** Get Payments ***************************************************

  <?xml version="1.0" encoding="UTF-8" standalone="no" ?> 
- <SOAP-ENV:Envelope
	xmlns:SOAP-ENV	="http://schemas.xmlsoap.org/soap/envelope/"
	xmlns:impl		="https://beta.parentmail.co.uk/ParentMailCashlessCatering"
	xmlns:wsdlsoap	="http://schemas.xmlsoap.org/wsdl/soap/"
	xmlns:wsdl		="http://schemas.xmlsoap.org/wsdl/"
	xmlns:soapenc	="http://schemas.xmlsoap.org/soap/encoding/"
	xmlns:xsd		="http://www.w3.org/2001/XMLSchema"
	xmlns:tns1		="https://beta.parentmail.co.uk/ParentMailCashlessCatering"
	xmlns:xsi		="http://www.w3.org/2001/XMLSchema-instance">

-	<SOAP-ENV:Body>
-		<mns:RequestAccountPayments
		xmlns:mns="https://beta.parentmail.co.uk/ParentMailCashlessCatering"
		SOAP-ENV:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
			<LoginDetails xsi:type="tns1:LoginDetails">
				<UserName xsi:type="xsd:string">school2027</UserName> 
				<Password xsi:type="xsd:string">zVgVWiJUvy</Password> 
				<SchoolID xsi:type="xsd:string">20</SchoolID> 
			</LoginDetails>
			<LastProcessedSnapshotID xsi:type="xsd:int">0</LastProcessedSnapshotID> 
		</mns:RequestAccountPayments>
	 </SOAP-ENV:Body>
 </SOAP-ENV:Envelope>


 <?xml version="1.0" encoding="utf-8" ?> 
- <soap:Envelope
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
-   <soap:Body>
	- <tns:RequestAccountPaymentsResponse xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
	-  <RequestAccountPaymentsResult xsi:type="tns:RequestAccountPaymentsResult">
	-   <StandardResult xsi:type="tns:StandardResult">
	     <ErrorStatus xsi:type="xsd:int">0</ErrorStatus> 
	     <ErrorDescription xsi:type="xsd:string">ok</ErrorDescription> 
	    </StandardResult>
	-   <ArrayOfAccountPayment soapenc:arrayType="tns:AccountPayment[2]">
	-    <AccountPayment xsi:type="tns:AccountPayment">
	      <PaymentID xsi:type="xsd:int">104</PaymentID> 
	      <AccountID xsi:type="xsd:int">1059</AccountID> 
	      <PurseType xsi:type="xsd:int">0</PurseType> 
	      <Value xsi:type="xsd:decimal">4.00</Value> 
	      <PaymentType xsi:type="xsd:int">1</PaymentType> 
	      <PaymentDate xsi:type="xsd:dateTime">2011-08-04T11:27:17.555622</PaymentDate> 
	     </AccountPayment>
	-    <AccountPayment xsi:type="tns:AccountPayment">
	      <PaymentID xsi:type="xsd:int">105</PaymentID> 
	      <AccountID xsi:type="xsd:int">1755</AccountID> 
	      <PurseType xsi:type="xsd:int">0</PurseType> 
	      <Value xsi:type="xsd:decimal">8.00</Value> 
	      <PaymentType xsi:type="xsd:int">1</PaymentType> 
	      <PaymentDate xsi:type="xsd:dateTime">2011-08-04T11:27:57.883358</PaymentDate> 
	     </AccountPayment>
	    </ArrayOfAccountPayment>
	   </RequestAccountPaymentsResult>
	  </tns:RequestAccountPaymentsResponse>
    </soap:Body>
  </soap:Envelope>

**********************************************************************/

CString CSSParentMail::GetPayments ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader )
{
	CString strError = "";
	CString strLastSnapshotID =	::FormatInt64Value ( m_pData->m_nLastPaymentID );

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.Attribute ( "xmlns:xsd", "http://www.w3.org/2001/XMLSchema" );
	s.Attribute ( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );

	s.BeginSoap ( "Body" );
		s.Begin ( "mns:RequestAccountPayments" );
		s.Attribute( "xmlns:mns", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );

			s.Begin ( "LoginDetails" );
				s.Element ( "UserName", m_pData->m_strUsername, "string" );
				s.Element ( "Password", m_pData->m_strPassword, "string" );
				s.Element ( "SchoolID", m_pData->m_strSchoolID, "string" );
			s.End();
			s.Element ( "LastProcessedSnapshotID", strLastSnapshotID, "int" );
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("RequestAccountPaymentsLogin.xml"), TRUE );

	if ( ( strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( strError = ValidateReply ( &s ) ) == "" )
		{
			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "AccountPayment" ) > 0 )					// see if any payment arrived
			{
				CSSUnsortedDatabase db;
				if ( db.CreateSSDatabase( szCsvFilename, "", szHeader, 10 ) == DB_ERR_NONE )		// create new db & open in Read\Write.
				{
					for ( int n = 0 ; n < arFound.GetSize() ; n++ )					// number of records
					{
						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);
						for ( int i = 0 ; i < node->GetSize() ; i++ )				// number of items per record
						{
							if ( node->GetAt(i)->IsText() == TRUE )
								db.Set ( node->GetAt(i)->GetLocalName(), node->GetAt(i)->GetValue() ); // lose namespace part of label
						}
					}

					if ( db.GetRecordCount() == 0 || m_pData->m_bKeepXmlFiles == FALSE )	//02/06/2017 - v1.15x
						CFileRecycler::SendToRecycleBin ( szLoginReply );								// delete original xml file

					db.Close();
				}
				else
					strError = db.GetError();
			}
		}
	}
	return strError;
}

//*******************************************************************
/*** send updates **************************************************

<?xml version="1.0" encoding="utf-8" ?> 
<soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/" 
xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" 
xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering" 
xmlns:types="https://beta.parentmail.co.uk/ParentMailCashlessCatering/encodedTypes" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xmlns:xsd="http://www.w3.org/2001/XMLSchema">
 <soap11:Body soap11:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
  <tns:SendTransactions>
   <LoginDetails href="#id1" /> 
   <Transactions href="#id2" /> 
  </tns:SendTransactions>
- <tns:LoginDetails id="id1" xsi:type="tns:LoginDetails">
   <UserName xsi:type="xsd:string">school2027</UserName> 
   <Password xsi:type="xsd:string">zVgVWiJUvy</Password> 
   <SchoolID xsi:type="xsd:string">20</SchoolID> 
  </tns:LoginDetails>
- <soapenc:Array id="id2" soapenc:arrayType="tns:Transaction[2]">
   <Item href="#id3" /> 
   <Item href="#id4" /> 
  </soapenc:Array>
- <tns:Transaction id="id3" xsi:type="tns:Transaction">
   <TransID xsi:type="xsd:int">33</TransID> 
   <AccountID xsi:type="xsd:int">2394</AccountID> 
   <SalesValue xsi:type="xsd:decimal">10.00</SalesValue> 
   <FreeValue xsi:type="xsd:decimal">0.00</FreeValue> 
   <TransDateTime xsi:type="xsd:dateTime">2011-10-05 14:03:00</TransDateTime> 
   <TransType xsi:type="xsd:int">3</TransType> 
   <EPosID xsi:type="xsd:string">pc</EPosID> 
   <ProductID xsi:type="xsd:int">0</ProductID> 
   <PurseType xsi:type="xsd:decimal">1</PurseType> 
   <VatRate xsi:type="xsd:decimal">0</VatRate> 
   <Balance xsi:type="xsd:decimal">182.87</Balance> 
   <ReceiptNumber xsi:type="xsd:int">33</ReceiptNumber> 
  </tns:Transaction>
- <tns:Transaction id="id4" xsi:type="tns:Transaction">
   <TransID xsi:type="xsd:int">34</TransID> 
   <AccountID xsi:type="xsd:int">2395</AccountID> 
   <SalesValue xsi:type="xsd:decimal">10.00</SalesValue> 
   <FreeValue xsi:type="xsd:decimal">0.00</FreeValue> 
   <TransDateTime xsi:type="xsd:dateTime">2011-10-05 14:03:00</TransDateTime> 
   <TransType xsi:type="xsd:int">3</TransType> 
   <EPosID xsi:type="xsd:string">pc</EPosID> 
   <ProductID xsi:type="xsd:int">0</ProductID> 
   <PurseType xsi:type="xsd:decimal">1</PurseType> 
   <VatRate xsi:type="xsd:decimal">0</VatRate> 
   <Balance xsi:type="xsd:decimal">132.30</Balance> 
   <ReceiptNumber xsi:type="xsd:int">34</ReceiptNumber> 
  </tns:Transaction>
 </soap11:Body>
</soap11:Envelope>

<?xml version="1.0" encoding="UTF-8" ?> 
<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" 
xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"
xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
 <soap:Body>
  <tns:SendTransactionsResponse xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
  <StandardResult xsi:type="tns:StandardResult">
   <ErrorStatus xsi:type="xsd:int">0</ErrorStatus> 
   <ErrorDescription xsi:type="xsd:string">ok</ErrorDescription> 
  </StandardResult>
  </tns:SendTransactionsResponse>
 </soap:Body>
</soap:Envelope>

**********************************************************************/

CString CSSParentMail::SendUpdates ( const char* szSource, const char* szLoginReply  )
{
	CString strError = "";

	int nLineCount = ::GetFileLineCount ( szSource );
	nLineCount -= 2;											// ignore the 2 header lines

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strError.Format ( "Cannot open source file ' %s ' ", szSource );
		return strError;
	}

	CCSV csvHeader1;
	CCSV csvHeader2;

	fileSource.Read ( csvHeader1 );		
	if ( fileSource.Read ( csvHeader2 )	== FALSE || nLineCount < 1 )
		return strError;

	int nMaxTxCount = m_pData->m_nMaxTransactionLines;
	m_nTxCount = ( nLineCount > nMaxTxCount ) ? nMaxTxCount : nLineCount;

	CSSSoap s ( m_pData->m_strSoapVersion );

	s.AttributeSoap ( "encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/" );
	s.Attribute ( "xmlns:soapenc", "http://schemas.xmlsoap.org/soap/encoding/" );
	s.Attribute ( "xmlns:tns",   "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );
	s.Attribute ( "xmlns:types", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering/encodedTypes" );
	s.Attribute ( "xmlns:xsi",   "http://www.w3.org/2001/XMLSchema-instance" );
	s.Attribute ( "xmlns:xsd",   "http://www.w3.org/2001/XMLSchema" );
	
	s.BeginSoap ( "Body" );
		s.Begin ( "tns:SendTransactions" );
			s.Begin ( "LoginDetails" );	s.Attribute( "href", "#id1" );	s.End();
			s.Begin ( "Transactions" );	s.Attribute( "href", "#id2" );	s.End();
		s.End();

		s.Begin ( "tns:LoginDetails" );
		s.Attribute( "id", "id1" );
		s.Attribute( "xsi:type", "tns:LoginDetails" );
			s.Element ( "UserName", m_pData->m_strUsername, "string" );
			s.Element ( "Password", m_pData->m_strPassword, "string" );
			s.Element ( "SchoolID", m_pData->m_strSchoolID, "string" );
		s.End();

		CString strMsg;
		strMsg.Format ( "tns:Transaction[%d]", m_nTxCount );

		s.Begin ( "soapenc:Array" );
		s.Attribute( "id", "id2" );
		s.Attribute( "soapenc:arrayType", strMsg );
		{
			int id = 3;
			for ( int i = 0 ; i < m_nTxCount ; i++ )
			{
				strMsg.Format ( "#id%d", id++ );
				s.Begin ( "Item" );	s.Attribute( "href", strMsg );	s.End();
			}
		}
		s.End();

		int id = 3;
		for ( int i = 0 ; i < m_nTxCount ; i++ )
		{
			CCSV csvLine;
			fileSource.Read ( csvLine );

			strMsg.Format ( "id%d", id++ );
			s.Begin ( "tns:Transaction" );
			s.Attribute( "id", strMsg );
			s.Attribute( "xsi:type", "tns:Transaction" );

			for ( int i = 0 ; i < csvLine.GetSize() ; i++ )
				s.Element ( csvHeader1.GetString(i), csvLine.GetString(i), csvHeader2.GetString(i) );

			s.End();
		}
	s.End();

	fileSource.Close();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("SendTransactionLogin.xml"), TRUE );

	strError = s.SendHttps  ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	
	if ( strError == "" )
		strError = ValidateReply ( &s );

	return strError;
}

//*******************************************************************
/*** gets Parentmail AccountID for given card details **************

 <?xml version="1.0" encoding="utf-8" ?> 
- <soap11:Envelope
   xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/"
   xmlns:xsd="http://www.w3.org/2001/XMLSchema"
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

-  <soap11:Body xmlns:NS2="https://services.parentmail.co.uk/ParentMailCashlessCatering">
-	<NS1:LinkNewAccount xmlns:NS1="https://services.parentmail.co.uk/ParentMailCashlessCatering">
	 <LoginDetails href="#1" /> 
	 <Forename xsi:type="xsd:string">George</Forename> 
	 <Surname xsi:type="xsd:string">Garlic</Surname> 
	 <FormName xsi:type="xsd:string">13A</FormName> 
	 <ExternalID xsi:type="xsd:string" /> 
	 <AccountID xsi:type="xsd:int">0</AccountID> 
	 <DateOfBirth xsi:type="xsd:dateTime" /> 
	 <ProviderID xsi:type="xsd:string">000007</ProviderID> 
	 <AdditionalID xsi:type="xsd:string" /> 
	 <AccountType xsi:type="xsd:string">P</AccountType> 
	</NS1:LinkNewAccount>
-	<NS2:LoginDetails id="1" xsi:type="NS2:LoginDetails">
	<UserName xsi:type="xsd:string">school2027</UserName> 
	<Password xsi:type="xsd:string">zVgVWiJUvy</Password> 
	<SchoolID xsi:type="xsd:string">20</SchoolID> 
	</NS2:LoginDetails>
   </soap11:Body>
  </soap11:Envelope>

  <?xml version="1.0" encoding="UTF-8" ?> 
- <soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
-  <soap:Body>
-   <tns:LinkNewAccountResponse xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
-    <IntegerResult xsi:type="tns:IntegerResult">
-     <StandardResult xsi:type="tns:StandardResult">
       <ErrorStatus xsi:type="xsd:int">0</ErrorStatus> 
       <ErrorDescription xsi:type="xsd:string">ok</ErrorDescription> 
      </StandardResult>
      <Result xsi:type="xsd:int">2401</Result> 
     </IntegerResult>
    </tns:LinkNewAccountResponse>
   </soap:Body>
  </soap:Envelope>

*******************************************************************/

CString CSSParentMail::LinkNewAccount ( const char* szXMLReplyFile, const char* szForename, const char* szSurname, const char* szXMLDoB, const char* szCardNo, const char* szAdditionalID, const char* szFormName, const char* szAccountType )
{
	CString strError = "";

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.Attribute ( "xmlns:xsd", "http://www.w3.org/2001/XMLSchema" );			// <label xsi:type="xsd:string">value</label>
	s.Attribute ( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );		

	s.BeginSoap ( "Body" );
	s.Attribute ( "xmlns:NS2", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );
		s.Begin ( "NS1:LinkNewAccount" );
		s.Attribute( "xmlns:NS1", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );

			s.Begin ( "LoginDetails" );
				s.Attribute( "href", "#1" );
			s.End();

			s.Element ( "Forename", szForename, "string" );
			s.Element ( "Surname", szSurname, "string" );
			s.Element ( "FormName", szFormName, "string" );
			s.Element ( "ExternalID", "", "string" );
			s.Element ( "AccountID", "0", "int" );
			s.Element ( "DateOfBirth", szXMLDoB, "dateTime" );
			s.Element ( "ProviderID", szCardNo, "string" );
			s.Element ( "AdditionalID", szAdditionalID, "string" );
			s.Element ( "AccountType", szAccountType, "string" );
		s.End();

		s.Begin ( "NS2:LoginDetails" );
		s.Attribute( "id", "1" );
		s.Attribute( "xsi:type", "NS2:LoginDetails" );
			s.Element ( "UserName", m_pData->m_strUsername, "string" );
			s.Element ( "Password", m_pData->m_strPassword, "string" );
			s.Element ( "SchoolID", m_pData->m_strSchoolID, "string" );
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("NewLinkAccountLogin.xml"), TRUE );

	m_strAccountID = "";
	if ( ( strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szXMLReplyFile, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( strError = ValidateReply ( &s ) ) == "" )
		{
			CXMLNodeArray* p2 = s.FindNode ( "Result" );	// look for result label
			if ( p2 != NULL )
				m_strAccountID = p2->m_strValue;
		}
		strError = "";										// clear any - only interested if have\have an accountID
	}

	return strError;
}

//*******************************************************************
/* Request list of manually linked accounts

  <?xml version="1.0" encoding="utf-8" ?> 
- <soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
-  <soap11:Body>
-   <mns:RequestLinkedAccounts xmlns:mns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
-   <LoginDetails>
     <UserName xsi:type="xsd:string">school2027</UserName> 
     <Password xsi:type="xsd:string">zVgVWiJUvy</Password> 
     <SchoolID xsi:type="xsd:string">20</SchoolID> 
    </LoginDetails>
    </mns:RequestLinkedAccounts>
   </soap11:Body>
  </soap11:Envelope>


  <?xml version="1.0" encoding="UTF-8" ?> 
- <soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
-  <soap:Body>
-  <tns:RequestLinkedAccountsResponse xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
-   <RequestLinkedAccountsResult xsi:type="tns:RequestLinkedAccountsResult">
-    <StandardResult xsi:type="tns:StandardResult">
      <ErrorStatus xsi:type="xsd:int">0</ErrorStatus> 
      <ErrorDescription xsi:type="xsd:string">ok</ErrorDescription> 
     </StandardResult>
-    <ArrayOfLinkedAccount soapenc:arrayType="tns:LinkedAccount[1]">
-     <LinkedAccount xsi:type="tns:LinkedAccount">
       <ProviderID xsi:type="xsd:string">000014</ProviderID> 
       <AccountID xsi:type="xsd:int">2407</AccountID> 
      </LinkedAccount>
     </ArrayOfLinkedAccount>
    </RequestLinkedAccountsResult>
    </tns:RequestLinkedAccountsResponse>
   </soap:Body>
  </soap:Envelope>
********************************************************************/

CString CSSParentMail::RequestLinkedAccounts ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader )
{
	CString strError = "";

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.Attribute ( "xmlns:xsd", "http://www.w3.org/2001/XMLSchema" );
	s.Attribute ( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );

	s.BeginSoap ( "Body" );
		s.Begin ( "mns:RequestLinkedAccounts" );
		s.Attribute( "xmlns:mns", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );
			s.Begin ( "LoginDetails" );
				s.Element ( "UserName", m_pData->m_strUsername, "string" );
				s.Element ( "Password", m_pData->m_strPassword, "string" );
				s.Element ( "SchoolID", m_pData->m_strSchoolID, "string" );
			s.End();
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("RequestLinkedAccountLogin.xml"), TRUE );

	if ( ( strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( strError = ValidateReply ( &s ) ) == "" )
		{
			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "LinkedAccount" ) > 0 )
			{
				CSSUnsortedDatabase db;
				if ( db.CreateSSDatabase ( szCsvFilename, "", szHeader, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
				{
					for ( int n = 0 ; n < arFound.GetSize() ; n++ )				// number of records
					{
						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);
						for ( int i = 0 ; i < node->GetSize() ; i++ )			// number of items per record
						{
							if ( node->GetAt(i)->IsText() == TRUE )
								db.Set ( node->GetAt(i)->GetLocalName(), node->GetAt(i)->GetValue() ); // lose namespace part of label
						}
					}
					db.Close();
				}
				else
					strError = db.GetError();
			}
		}
	}

	return strError;
}

//*******************************************************************
/*
  <?xml version="1.0" encoding="utf-8" ?> 
- <soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
-  <soap11:Body>
-  <mns:GetTransactionSnapshotID xmlns:mns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
-   <LoginDetails>
     <UserName xsi:type="xsd:string">school2027</UserName> 
     <Password xsi:type="xsd:string">zVgVWiJUvy</Password> 
     <SchoolID xsi:type="xsd:string">20</SchoolID> 
    </LoginDetails>
   </mns:GetTransactionSnapshotID>
   </soap11:Body>
  </soap11:Envelope

  <?xml version="1.0" encoding="UTF-8" ?> 
- <soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
-  <soap:Body>
-   <tns:GetTransactionSnapshotIDResponse xmlns:tns="https://beta.parentmail.co.uk/ParentMailCashlessCatering">
-    <IntegerResult xsi:type="tns:IntegerResult">
-     <StandardResult xsi:type="tns:StandardResult">
       <ErrorStatus xsi:type="xsd:int">0</ErrorStatus> 
       <ErrorDescription xsi:type="xsd:string">ok</ErrorDescription> 
      </StandardResult>
      <Result xsi:type="xsd:int">0</Result> 
     </IntegerResult>
    </tns:GetTransactionSnapshotIDResponse>
   </soap:Body>
  </soap:Envelope>
  
*********************************************************************/

CString CSSParentMail::GetLastProcessedSnapShotID ( const char* szLoginReply )
{
	CString strError = "";

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.Attribute ( "xmlns:xsd", "http://www.w3.org/2001/XMLSchema" );
	s.Attribute ( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );

	s.BeginSoap ( "Body" );
		s.Begin ( "mns:GetTransactionSnapshotID" );
		s.Attribute( "xmlns:mns", "https://ccservice.parentmail2.co.uk/ParentMailCashlessCatering" );
			s.Begin ( "LoginDetails" );
				s.Element ( "UserName", m_pData->m_strUsername, "string" );
				s.Element ( "Password", m_pData->m_strPassword, "string" );
				s.Element ( "SchoolID", m_pData->m_strSchoolID, "string" );
			s.End();
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("GetTransactionSnapsnotIDLogin.xml"), TRUE );

	if ( ( strError = s.SendHttps ( m_strIPAddress, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( strError = ValidateReply ( &s ) ) == "" )
		{
			CXMLNodeArray* p2 = s.FindNode ( "Result" );				// look for result label
			if ( p2 != NULL )
				m_nLastTransactionID = _atoi64( p2->m_strValue );
		}
	}

	return strError;
}

//*******************************************************************

CString CSSParentMail::ValidateReply ( CSSSoap* pSoap )
{
	CString strError = "";

	CXMLNodeArray* p1 = pSoap->FindNode ( "ErrorStatus" );				// look for status label
	if ( p1 != NULL )
	{
		if ( p1->m_strValue != "0" )
		{
			CXMLNodeArray* p2 = pSoap->FindNode ( "ErrorDescription" );	// look for description label
			if ( p2 != NULL )		strError = p2->m_strValue;
			else					strError = "No ErrorDescription node returned";
		}
	}
	else
		strError = "No ErrorStatus node returned";

	return strError;
}

//*******************************************************************
