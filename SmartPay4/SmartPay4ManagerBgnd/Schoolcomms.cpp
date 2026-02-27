//*********************************************************************
#include "Schoolcomms.hpp"
//*********************************************************************

static const char* szIPADDRESS		= "www.schoolcomms.com";
static const char* szOBJECTNAME		= "/TPGateway/Payments.svc";

static const char* szNODE_LOGIN		= "a:LoginResult";
static const char* szNODE_BATCHID	= "a:BatchID";

static const char* szLOGINSUCCESS	= "Success";

static const char* szMSG_LOGINFAIL	= "Login failure!";
static const char* szMSG_UPLOADFAIL	= "Upload failure!";
static const char* szMSG_NOLOGIN	= "No LoginResult returned!";

static const char* szPARTNERID		= "0004FF80-8B93-4663-BB34-B0FFA744A8FF";
static const char* szPURSETYPE		= "Catering";

//*********************************************************************

CSSSchoolcomms::CSSSchoolcomms ( CSchoolcommsData* pData )
{
	m_pData = pData;
	m_bMoreRecordsAvailable = FALSE;
}

/**********************************************************************
/ Get Payments
/**********************************************************************
NOTE - returned xml file DOES NOT HAVE AN XML DECLARATION of " <?xml version="1.0"?> " aat start of data
************************************************************************
- <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
-  <s:Body>
-   <RequestAccountPaymentsResponse xmlns="https://www.schoolcomms.com/tpgateway">
-    <RequestAccountPaymentsResult xmlns:a="http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
     <a:BatchID>229</a:BatchID> 
     <a:DataRequestResult>AllAvailableRecordsReturned</a:DataRequestResult> 
     <a:LoginResult>Success</a:LoginResult> 
-     <a:PaymentsList>
-      <a:AccountPaymentRecord>
        <a:AccountPaymentID>3917</a:AccountPaymentID> 
        <a:PaymentAmount>20.0000</a:PaymentAmount> 
        <a:PaymentDate>2012-07-06T17:14:05.737</a:PaymentDate> 
        <a:SIMSPersonID>7338</a:SIMSPersonID> 
       </a:AccountPaymentRecord>
      </a:PaymentsList>
     </RequestAccountPaymentsResult>
    </RequestAccountPaymentsResponse xmlns="https://www.schoolcomms.com/tpgateway">
   </s:Body>
  </s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">


 if no more payment **********

- <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
-  <s:Body>
-   <RequestAccountPaymentsResponse xmlns="https://www.schoolcomms.com/tpgateway">
-    <RequestAccountPaymentsResult xmlns:a="http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
      <a:BatchID i:nil="true" /> 
      <a:DataRequestResult>AllAvailableRecordsReturned</a:DataRequestResult> 
      <a:LoginResult>Success</a:LoginResult> 
      <a:PaymentsList i:nil="true" /> 
     </RequestAccountPaymentsResult>
    </RequestAccountPaymentsResponse>
   </s:Body>
  </s:Envelope>

 **********************************************************************/

CString CSSSchoolcomms::GetPayments ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader )
{
	CString strError = "";

	CString strBatchID = ::FormatInt64Value(m_pData->m_nLastPaymentID);

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.schoolcomms.com/tpgateway/IPayments/RequestAccountPayments");
	s.BeginSoap ( "Body" );
		s.Begin ( "RequestAccountPayments" );
		s.Attribute ( "xmlns", "https://www.schoolcomms.com/tpgateway" );
			s.Begin ( "credentials" );
			s.Attribute ( "xmlns:a", "http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" );
				s.Element ( "a:LADCSFCode", m_pData->m_strSchoolID );		// <xs:element minOccurs="0" name="LADCSFCode" type="xs:long" />
				s.Element ( "a:MemorableData", m_pData->m_strMemorable );	// <xs:element minOccurs="0" name="MemorableData" nillable="true" type="xs:string" />
				s.Element ( "a:Password", m_pData->m_strPassword );			// <xs:element minOccurs="0" name="Password" nillable="true" type="xs:string" />
				s.Element ( "a:PurseTypeIdentifier", szPURSETYPE );			// xs:element minOccurs="0" name="PurseTypeIdentifier" type="tns:PurseTypeEnum"  />
				s.Element ( "a:ThirdPartyPartnerID", szPARTNERID);			// <xs:element minOccurs="0" name="ThirdPartyPartnerID" type="ser:guid" />
				s.Element ( "a:UserName", m_pData->m_strUsername );			// <xs:element minOccurs="0" name="UserName" nillable="true" type="xs:string" />
			s.End();
			s.Element ( "lastProcessedAccountPaymentBatchID", strBatchID );
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetPaymentsLogin.xml"), TRUE);
	}

	strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, FALSE, System.GetInternetAgentString() );			// no XML declaration check

	if ( ( strError = ValidateLoginReply ( &s, strError ) ) == "" )
	{
		CXMLNodeArray* p1 = s.FindNode ( szNODE_BATCHID );				// get new Batch ID
		if ( p1 != NULL )
		{
			strBatchID = p1->GetValue();								// new batch Payment ID
			if ( strBatchID != "" )										// may be empty if no data sent
			{
				CXMLNodeArray arFound;
				if ( s.FindNodes ( arFound, "a:AccountPaymentRecord" ) > 0 )
				{
					CSSUnsortedDatabase db;
					if ( db.CreateSSDatabase ( szCsvFilename, "", szHeader, 10 ) == DB_ERR_NONE )		// create new db & open in Read\Write.
					{
						for ( int n = 0 ; n < arFound.GetSize() ; n++ )	// number of records
						{
							db.AddBlankRecord();
							CXMLNodeArray* node = arFound.GetAt(n);

							db.Set ( szNODE_BATCHID, strBatchID );
							for (int i = 0; i < node->GetSize(); i++)
							{
								if (node->GetAt(i)->IsText() == TRUE)
								{
									db.Set(node->GetAt(i)->GetName(), node->GetAt(i)->GetValue());
								}
							}
						}

						p1 = s.FindNode ( "a:DataRequestResult" );		// check for more records available on web site than have been sent
						if (p1 != NULL)
						{
							m_bMoreRecordsAvailable = (p1->m_strValue == "MoreRecordsAvailableToRequest") ? TRUE : FALSE;
						}

						if (db.GetRecordCount() == 0 || m_pData->m_bKeepXmlFiles == FALSE)	//02/06/2017 - v1.15x
						{
							CFileRecycler::SendToRecycleBin(szLoginReply);								// delete original xml file
						}

						db.Close();
					}
					else
					{
						strError = db.GetError();
					}
				}
			}
		}
	}
	return strError;
}

/**********************************************************************
/ Send Transactions \ Send Balances \ Send Topups
/**********************************************************************
 <?xml version="1.0" encoding="utf-8" ?> 
- <soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/">
-  <soap11:Body>
-   <UploadTransactions xmlns="https://www.schoolcomms.com/tpgateway">
-    <credentials xmlns:a="http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService">
      <a:LADCSFCode>999331166</a:LADCSFCode> 
      <a:MemorableData>4P1Int3gr4tion</a:MemorableData> 
      <a:Password>Sh4rpUs3r238890</a:Password> 
      <a:PurseTypeIdentifier>Catering</a:PurseTypeIdentifier> 
      <*a:ThirdPartyPartnerID>0004FF80-8B93-4663-BB34-B0FFA744A8FF</a:ThirdPartyPartnerID> 
      <a:UserName>PartSharpCCM</a:UserName> 
      </credentials>
     <partnerIDsSupplied>0</partnerIDsSupplied> 

     <historyRecords>
      <PartnerTransactionID /> 
      <SIMSPersonID>7718</SIMSPersonID> 
      <DescriptionOfPurchasedItem>Aubergines, Courgettes, MANGETOUT</DescriptionOfPurchasedItem> 
      <ReceiptID>310056</ReceiptID> 
	  <FullAmount>5.12</FullAmount>
	  <ChargedAmount>5.12</ChargedAmount>
	  <FreeSchoolMeal>"0"</FreeSchoolMeal>
      <TransactionDate>2011-10-21 12:13:00</TransactionDate> 
      <CurrentMemberBalance /> 
      <TransactionType>Sale</TransactionType> 
     </historyRecords>
or
     <historyRecords>
      <PartnerTransactionID /> 
      <SIMSPersonID>7718</SIMSPersonID> 
      <DescriptionOfPurchasedItem /> 
      <ReceiptID /> 
	  <FullAmount />
	  <ChargedAmount />
	  <FreeSchoolMeal />
      <TransactionDate>2011-10-21 12:13:00</TransactionDate> 
      <CurrentMemberBalance>10.25<CurrentMemberBalance />
      <TransactionType>BalanceOnly</TransactionType> 
     </historyRecords>
or
     <historyRecords>
      <PartnerTransactionID /> 
      <SIMSPersonID>7718</SIMSPersonID> 
      <DescriptionOfPurchasedItem>"Web topup"</DescriptionOfPurchasedItem> 
      <ReceiptID>{PaymentID}</ReceiptID>
	  <FullAmount>10.00</FullAmount>
	  <ChargedAmount />
	  <FreeSchoolMeal />
      <TransactionDate>2011-10-21 12:13:00</TransactionDate> 
      <CurrentMemberBalance>22.25<CurrentMemberBalance />
      <TransactionType>Topup</TransactionType> 
     </historyRecords>

-   </UploadTransactions>
-  </soap11:Body>
- </soap11:Envelope">


reply

- <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
-  <s:Body>
-   <UploadTransactionsResponse xmlns="https://www.schoolcomms.com/tpgateway">
-    <UploadTransactionsResult xmlns:a="http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
      <a:LoginResult>Success</a:LoginResult> 
      <a:UnknownIDs xmlns:b="http://schemas.microsoft.com/2003/10/Serialization/Arrays" /> 
      <a:UploadResult>UploadSuccessful</a:UploadResult> 
     </UploadTransactionsResult>
    </UploadTransactionsResponse>
   </s:Body>
  </s:Envelope>


*****************************************************************/

CString CSSSchoolcomms::SendTransactions ( const char* szSource, const char* szLoginReply, int nMaxTxCount )
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
	s.SetSoapAction ("https://www.schoolcomms.com/tpgateway/IPayments/UploadTransactions");
	s.BeginSoap ( "Body" );
		s.Begin ( "UploadTransactions" );
		s.Attribute ( "xmlns", "https://www.schoolcomms.com/tpgateway" );
			s.Begin ( "credentials" );
			s.Attribute ( "xmlns:a", "http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" );
				s.Element ( "a:LADCSFCode", m_pData->m_strSchoolID );		// <xs:element minOccurs="0" name="LADCSFCode" type="xs:long" />
				s.Element ( "a:MemorableData", m_pData->m_strMemorable );	// <xs:element minOccurs="0" name="MemorableData" nillable="true" type="xs:string" />
				s.Element ( "a:Password", m_pData->m_strPassword );			// <xs:element minOccurs="0" name="Password" nillable="true" type="xs:string" />
				s.Element ( "a:PurseTypeIdentifier", szPURSETYPE );			// <xs:element minOccurs="0" name="PurseTypeIdentifier" type="tns:PurseTypeEnum"  />
				s.Element ( "a:ThirdPartyPartnerID", szPARTNERID );			// <xs:element minOccurs="0" name="ThirdPartyPartnerID" type="ser:guid" />
				s.Element ( "a:UserName", m_pData->m_strUsername );			// <xs:element minOccurs="0" name="UserName" nillable="true" type="xs:string" />
			s.End();
			s.Element ( "partnerIDsSupplied", "false" );

			s.Begin ( "historyRecords" );
			s.Attribute ( "xmlns:a", "http://schemas.datacontract.org/2004/07/SCThirdPartyGatewayService" );
			{
				CCSV csv;
				for ( int i = 0 ; i < nMaxTxCount ; i++ )
				{
					fileSource.Read ( csv );
					s.Begin ( "a:PartnerTransactionRecord" );
					{
						for (int i = 0; i < csv.GetSize(); i++)
						{
							s.Element(csvHeader.GetString(i), csv.GetString(i));
						}
					}
					s.End();
				}
			}
			s.End();	

			fileSource.Close();
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlFiles == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("SendTransactionData.xml"), TRUE);
	}

	strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, FALSE, System.GetInternetAgentString() );			// no XML declaration check

	if ((strError = ValidateLoginReply(&s, strError)) == "")
	{
		CXMLNodeArray* p1 = s.FindNode("a:UploadResult");	// check for upload result node
		if (p1 != NULL)
		{
			CString strResult = p1->m_strValue;								// strValue = "UploadSuccessful"
																			// strValue = "UploadSuccessfulWithUnknownMISIDs"
			if (strResult.Left(16) != "UploadSuccessful")				// strValue = "FailedOneOrMoreRecordsOlderThanLastProcessedRecord"
			{
				strError.Format("%s - %s", szMSG_UPLOADFAIL, strResult);
			}
		}
		else
		{
			strError.Format("%s - No UploadResult returned", szMSG_UPLOADFAIL);
		}
	}
	return strError;
}

//**********************************************************************

CString CSSSchoolcomms::ValidateLoginReply ( CSSSoap* s, const char* szError )
{
	CString strError = szError;

	if ( strError == "" )										// check first for TCP comms errors
	{
		CXMLNodeArray* p1 = s->FindNode ( szNODE_LOGIN );				// check for login node
		if (p1 != NULL)
		{
			if (p1->m_strValue != szLOGINSUCCESS)								// check login ok
			{
				strError.Format("%s - %s", szMSG_LOGINFAIL, p1->m_strValue);	// LogInResult text
			}
		}
		else
		{
			strError.Format("%s - %s", szMSG_LOGINFAIL, szMSG_NOLOGIN);		// LogInResult text
		}
	}

	return strError;
}

//**********************************************************************
