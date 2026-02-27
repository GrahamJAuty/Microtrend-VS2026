//**********************************************************************
#include "ParentPay.hpp"
//**********************************************************************

extern const char* szVERSION_SMARTPAY_ABOUT;

//**********************************************************************

static const char* szIPADDRESS	= "dc3api.parentpay.com";
static const char* szOBJECTNAME	= "/POSWS3/POSapi3.asmx";

const char* szPAYMENTSHEADER	= "PaymentID,accountID,rollNumber,UPN,MISID,txnAmount,PayerInfo,PaidDate,ReceiptCode,paymentTypeCode,ConsumerName";
const char* szDINERDATAHEADER	= "accountID,forename,surname,dateOfBirth,yearGroup,regGroup,rollNumber,UPN,misID,gender,dinerStatus";

const char* szERROR_BATCHSIZE	= "Insufficient upload batch size available!";

//**********************************************************************
//**********************************************************************
//**********************************************************************
// Demo service IDs
// Service1ID = 282565
// Service2ID = 287704

CSSParentPay::CSSParentPay ( CParentPayData* pData )
{
	m_pData = pData;
	m_strSendError = "";
	m_strValidateError = "";
	m_strUploadToken = "";
	m_nUploadBatchSize = 0;
	m_nQueueLength = 0;
}

/**********************************************************************************
<?xml version="1.0" encoding="utf-8"?>
<soap11:Envelope xmlns:soap11="http://schemas.xmlsoap.org/soap/envelope/">
 <soap11:Body>
  <GetPaymentReport3 xmlns="https://www.parentpay.com/POSWS3/POSapi3">
   <loginCredentials>
    <username>wsp0064057</username>
    <password>ccm8P80TJVS</password>
    <schoolID>school-1-0064057</schoolID>
    <connectingPOSID>my-pos</connectingPOSID>
   </loginCredentials>
   <serviceID>282565</serviceID>
   <mealSessionID>1</mealSessionID>
   <minPaymentID>0</minPaymentID>
   <maxPaymentID>-1</maxPaymentID>
  </GetPaymentReport3>
 </soap11:Body>
</soap11:Envelope>

Get Payments response *************************************************
<?xml version="1.0" encoding="UTF-8"?>
<soap:Envelope xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
 <soap:Body>
  <GetPaymentReport3Response xmlns="https://www.parentpay.com/POSWS3/POSapi3">
   <GetPaymentReport3Result>
    <SuccessState>0</SuccessState>
    <ErrorDescription>OK</ErrorDescription>
    <NumRecords>2</NumRecords>
    <TotalAmount>15</TotalAmount>
    -<PaymentArray>
     -<PaymentRecord3>
      <PaymentID>126476510</PaymentID>
      <accountID>6212813</accountID>
      <rollNumber>0011034</rollNumber>
      <UPN>U820200109032</UPN>
      <MISID>2464</MISID>
      <txnAmount>5</txnAmount>
      <PayerInfo/>
      <PaidDate>2014-02-04T13:31:40.0830000-00:00</PaidDate>
      <ReceiptCode>MANL:92058388</ReceiptCode>
      <paymentTypeCode>5</paymentTypeCode>
      <Amount>500</Amount>								// legacy data
      <PaymentType>ManualOther</PaymentType>			// legacy data
      <ConsumerName>Liz Aaron</ConsumerName>			// legacy data
      <Identifier>0011034</Identifier>					// legacy data
      <Quantity>0</Quantity> 							// legacy data
     </PaymentRecord3>
     <PaymentRecord3>
      <PaymentID>126476873</PaymentID>
      <accountID>6212820</accountID>
      <rollNumber>000981</rollNumber>
      <UPN>M820200106060</UPN>
      <MISID>2087</MISID>
      <txnAmount>10</txnAmount>
      <PayerInfo/>
      <PaidDate>2014-02-04T13:32:43.6500000-00:00</PaidDate>
      <ReceiptCode>MANL:92058729</ReceiptCode>
      <paymentTypeCode>5</paymentTypeCode>
      <Amount>1000</Amount>
      <PaymentType>ManualOther</PaymentType>
      <ConsumerName>Jordan Acton</ConsumerName> 
      <Identifier>000981</Identifier>
      <Quantity>0</Quantity>
     </PaymentRecord3>
    </PaymentArray>
   </GetPaymentReport3Result>
  </GetPaymentReport3Response>
 </soap:Body>
</soap

**********************************************************************/

CString CSSParentPay::GetPayments ( int nServiceNo, const char* szLoginReply, const char* szCsvFilename )
{
	m_strSendError		= "";
	m_strValidateError	= "";

	CString strTitle	= "GetPayment";

	__int64 nServiceID		 = ( nServiceNo == 1 ) ? m_pData->m_nAccountID1	  : m_pData->m_nAccountID2;	
	__int64 nLastTransaction = ( nServiceNo == 1 ) ? m_pData->m_nLastAccount1 : m_pData->m_nLastAccount2;
	int nMealSession		 = ( nServiceNo == 1 ) ? m_pData->m_nMealSession1 : m_pData->m_nMealSession2;

	CString strServiceID = ::FormatInt64Value( nServiceID );
	CString strLastPaymentID = ::FormatInt64Value( nLastTransaction );
	CString strMealSession;		strMealSession.Format   ( "%d", nMealSession );

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/GetPaymentReport3");

	s.BeginSoap ( "Body" );
		s.Begin ( "GetPaymentReport3" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );
			AddLoginDetails ( &s );							// loginCredentials
			s.Element ( "serviceID", strServiceID );
			s.Element ( "mealSessionID", strMealSession );
			s.Element ( "minPaymentID", strLastPaymentID );
			s.Element ( "maxPaymentID", "-1" );
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetPaymentLogin.xml"), TRUE);
	}

	if ( ( m_strSendError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( m_strValidateError = ValidateReply ( strTitle, &s, "SuccessState" ) ) == "" )
		{
			remove ( szCsvFilename );

			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "PaymentRecord3" ) > 0 )
			{
				CSSUnsortedDatabase db;
				if ( db.CreateSSDatabase ( szCsvFilename, "", szPAYMENTSHEADER, 10 ) == DB_ERR_NONE )				// create new db & open in Read\Write.
				{
					for ( int n = 0 ; n < arFound.GetSize() ; n++ )	// number of records
					{
						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);
						for ( int i = 0 ; i < node->GetSize() ; i++ )	// number of items per record
						{
							if ( node->GetAt(i)->IsText() == TRUE )
								db.Set ( node->GetAt(i)->GetName(), node->GetAt(i)->GetValue() );
						}
					}

					if (db.GetRecordCount() == 0 || m_pData->m_bKeepXmlFiles == FALSE)	//02/06/2017 - v1.15x
					{
						CFileRecycler::SendToRecycleBin(szLoginReply);								// delete original xml file
					}

					db.Close();
				}
				else
				{
					m_strValidateError = db.GetError();
				}
			}
		}
	}

	CString strReply = "";
		
	if (m_strSendError != "")
	{
		strReply.Format("%s: %s", 
			(const char*) strTitle, 
			(const char*) m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}

	return strReply;
}

//************************************************************************
//************************************************************************
//************************************************************************

CString CSSParentPay::RequestDinerData ( const char* szLoginReply, const char* szCsvFilename )
{
	m_strSendError		= "";
	m_strValidateError	= "";

	CString strTitle	= "GetDiner";
	CString strFlags	 = "4";									// 4=till controlled balance, 

	CString strDinerSnapshotID = ::FormatInt64Value( m_pData->m_nLastDinerSnapshotID );
	CString strBalanceSnapshotID = ::FormatInt64Value(m_pData->m_nLastBalanceSnapshotID );

	//FIDDLE
	strDinerSnapshotID = "0";

// Login ************************************************************

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/RequestDinerData35");

	s.BeginSoap ( "Body" );
		s.Begin ( "RequestDinerData35" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );
			AddLoginDetails ( &s );							// loginCredentials;
			s.Element ( "currentDinerSnapshotID", strDinerSnapshotID );
			s.Element ( "currentBalanceSnapshotID", strBalanceSnapshotID );
			s.Element ( "flags", strFlags );
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("GetDinerDataLogin.xml"), TRUE);
	}

	if ( ( m_strSendError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( m_strValidateError = ValidateReply ( "GetDiner", &s, "errorStatus" ) ) == "" )
		{
			CXMLNodeArray* p1 = s.FindNode ( "newDinerSnapshotID" );	// look for result label
			if ( p1 != NULL )
			{
				CString strValue = p1->m_strValue;
				m_pData->m_nLastDinerSnapshotID = _atoi64 ( strValue );
			}

			p1 = s.FindNode ( "newBalanceSnapshotID" );				// look for result label
			if ( p1 != NULL )
			{
				CString strValue = p1->m_strValue;
				m_pData->m_nLastBalanceSnapshotID =_atoi64 ( strValue );
			}

//			p1 = s.FindNode ( "connectingPOSID" );					// look for result label
//			if ( p1 != NULL )
//			{
//				CString strPosID = p1->m_strValue;
//				if ( strPosID == m_strPOSID )
//				{
//					p1 = s.FindNode ( "lastPOSTxnID" );
//					if ( p1 != NULL )
//						CString strlastPosTxnID = p1->m_strValue;
//				}
//			}

			remove ( szCsvFilename );

			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "PrimaryDinerDataRecord2" ) > 0 )
			{
				CSSUnsortedDatabase db;
				if ( db.CreateSSDatabase ( szCsvFilename, "", szDINERDATAHEADER, 10 ) == DB_ERR_NONE )				// create new db & open in Read\Write.
				{
					for ( int n = 0 ; n < arFound.GetSize() ; n++ )	// number of records
					{
						db.AddBlankRecord();
						CXMLNodeArray* node = arFound.GetAt(n);
						for (int i = 0; i < node->GetSize(); i++)	// number of items per record
						{
							if (node->GetAt(i)->IsText() == TRUE)
							{
								db.Set(node->GetAt(i)->GetName(), node->GetAt(i)->GetValue());
							}
						}
					}
					db.Close();
				}
				else
				{
					m_strValidateError = db.GetError();
				}
			}
		}
	}

	CString strReply = "";

	if (m_strSendError != "")
	{
		strReply.Format("%s: %s",
			(const char*) strTitle, 
			(const char*) m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}

	return strReply;
}

//**************************************************************************
// Diner Match Advice

CString CSSParentPay::SendDinerMatchAdvice ( const char* szSource, const char* szLoginReply )
{
	m_strSendError		= "";
	m_strValidateError	= "";

	CString strReply;
	CString strTitle = "DinerMatch";
		
	int nLineCount = ::GetFileLineCount ( szSource );			// number of lien to send
	nLineCount -= 1;											// ignore header line

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strReply.Format ( "%s: Cannot open source file ' %s ' ", 
			(const char*) strTitle, 
			szSource );

		return strReply;
	}

	CCSV csvHeader;
	if ( fileSource.Read ( csvHeader ) == FALSE || nLineCount < 1 )		// get file header
	{
		strReply.Format ( "%s: No data in source file ' %s ' ", 
			(const char*) strTitle, 
			szSource );

		return strReply;
	}

	m_nQueueLength = nLineCount;										// total number of lines to send

	if ((strReply = GetUploadToken(strTitle, m_nQueueLength)) != "")		// get token 
	{
		return strReply;
	}

	if ( m_nUploadBatchSize < 1 )										// check have enough to send at least 1 record
	{
		strReply.Format ( "%s: %s", 
			(const char*) strTitle, 
			szERROR_BATCHSIZE );
	
		return strReply;												// no - not enough
	}

	if (m_nQueueLength > m_nUploadBatchSize)
	{
		m_nQueueLength = m_nUploadBatchSize;
	}
	
	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/SendTransactionData3");

	s.BeginSoap ( "Body" );
		s.Begin ( "SendTransactionData3" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );

			AddTransactionDataHeader ( &s, m_nQueueLength );	// loginCredentials + other stuff

			AddNullProductSaleData ( &s );					// no product sale data
			AddNullRewardPointData ( &s );					// no reward point data
			AddNullFinancialTxnData ( &s );					// no financial data
			AddNullTxnEchoData ( &s );						// no echo payment data
			AddNullCancellationData ( &s );					// no cancellation data
			AddNullControlRecordData ( &s );				// no control data

//************************************************************************************************************
//			s.Begin ( "dinerMatchAdviceData" );
//				s.Begin ( "DinerMatchAdviceRecord" );
//					s.Element ( "connectingPOSTxnID", GetNextTransactionID() );
//					s.Element ( "parentPayAccountID", "6212822" );					// parentPayID Adam Adams
//					s.Element ( "posInternalAccountID", "002392" );					// cardno
//					s.Element ( "externalAccountReference", "J820200109003" );		// upn
//					s.Element ( "openingBalance", "0" );
//				s.End();
//			s.End();
//************************************************************************************************************

			s.Begin ( "dinerMatchAdviceData" );
				for ( int i = 0 ; i < m_nQueueLength ; i++ )
				{
					CCSV csvLine;
					fileSource.Read ( csvLine );

					s.Begin ( "DinerMatchAdviceRecord" );
					for ( int i = 0 ; i < csvLine.GetSize() ; i++ )
						s.Element ( csvHeader.GetString(i), csvLine.GetString(i) );
					s.End();
				}
			s.End();

		s.End();
	s.End();

	if (m_pData->m_bKeepXmlFiles == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("SendDinerMatchAdvice.xml"), TRUE);
	}

	if ((m_strSendError = s.SendHttps(szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString())) == "")
	{
		m_strValidateError = ValidateReply(strTitle, &s, "errorStatus");
	}

	strReply = "";
		
	if (m_strSendError != "")
	{
		strReply.Format("%s: %s", 
			(const char*) strTitle, 
			(const char*) m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}

	return strReply;
}

//**************************************************************************
// echo back parentpay payments

CString CSSParentPay::SendPaymentEchoData ( const char* szSource, const char* szLoginReply )
{
	m_strSendError		= "";
	m_strValidateError	= "";

	CString strReply;
	CString strTitle = "EchoPayment";

	int nLineCount = ::GetFileLineCount ( szSource );			// number of lien to send
	nLineCount -= 1;											// ignore header line

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strReply.Format ( "%s: Cannot open source file ' %s ' ",
			(const char*) strTitle, 
			szSource );

		return strReply;
	}

	CCSV csvHeader;
	if ( fileSource.Read ( csvHeader ) == FALSE || nLineCount < 1 )		// get file header
	{
		strReply.Format ( "%s: No data in source file ' %s ' ", 
			(const char*) strTitle,
			szSource );

		return strReply;
	}

	m_nQueueLength = nLineCount;										// lines to send

	if ( ( strReply = GetUploadToken ( strTitle, m_nQueueLength ) ) != "" )
		return strReply;

	if ( m_nUploadBatchSize < 1 )										// check have enough to send at least 1 record
	{
		strReply.Format ( "%s: %s", strTitle, szERROR_BATCHSIZE );
		return strReply;												// no - not enough
	}

	if (m_nQueueLength > m_nUploadBatchSize)
	{
		m_nQueueLength = m_nUploadBatchSize;
	}

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/SendTransactionData3");

	s.BeginSoap ( "Body" );
		s.Begin ( "SendTransactionData3" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );

			AddTransactionDataHeader ( &s, m_nQueueLength );		// loginCredentials + other stuff

			AddNullProductSaleData ( &s );							// no product sale data
			AddNullRewardPointData ( &s );							// no reward point data
			AddNullFinancialTxnData ( &s );							// no financial data
			
//***************************************************************************************
//			s.Begin ( "parentPayTxnEchoData" );
//				s.Begin ( "ParentPayTxnEchoRecord3" );
//					s.Element ( "connectingPOSTxnID", GetNextTransactionID() );
//					s.Element ( "mealSessionID", m_strSessionID );
//					s.Element ( "txnAmount", "0" );
//					s.Element ( "description", "Ignored" );
//					s.Element ( "accountID", "6212822" );		
//					s.Element ( "txnDateTime", strDate );
//					s.Element ( "txnType", "0" );
//					s.Element ( "originatingPOSID", m_strPOSID );	// minOccours = 0
//					s.Element ( "flags", strFlags );				// 2=flags in use, +8=till source balance
//					s.Element ( "newBalance", "0" );
//					s.Element ( "posInternalTxnID", "xxx" );
//					s.Element ( "parentPayPaymentID", "128389571" );
//				s.End();
//			s.End();
//***************************************************************************************

			s.Begin ( "parentPayTxnEchoData" );
				for ( int i = 0 ; i < m_nQueueLength; i++ )
				{
					CCSV csvLine;
					fileSource.Read ( csvLine );

					s.Begin ( "ParentPayTxnEchoRecord3" );
					for ( int i = 0 ; i < csvLine.GetSize() ; i++ )
						s.Element ( csvHeader.GetString(i), csvLine.GetString(i) );
					s.End();
				}
			s.End();

			AddNullCancellationData ( &s );					// no cancellation data
			AddNullControlRecordData ( &s );				// no control data
			AddNullDinerMatchAdviceData ( &s );				// no diner match data

		s.End();
	s.End();

	if (m_pData->m_bKeepXmlFiles == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("SendPaymentEchoData.xml"), TRUE);
	}

	if ((m_strSendError = s.SendHttps(szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString())) == "")
	{
		m_strValidateError = ValidateReply(strTitle, &s, "errorStatus");
	}

	strReply = "";
	if (m_strSendError != "")
	{
		strReply.Format("%s: %s", 
			(const char*) strTitle, 
			(const char*) m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}
	return strReply;
}

//**************************************************************************
// echo back parentpay payments

CString CSSParentPay::SendFinancialData ( const char* szSource, const char* szLoginReply )
{
	m_strSendError		= "";
	m_strValidateError	= "";

	CString strReply;
	CString strTitle = "SendData";

	int nLineCount = ::GetFileLineCount ( szSource );			// number of lines to send
	nLineCount -= 1;											// ignore header line

	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
	{
		strReply.Format ( "%s: Cannot open source file ' %s ' ", 
			(const char*) strTitle, 
			szSource );

		return strReply;
	}

	CCSV csvHeader;
	if ( fileSource.Read ( csvHeader ) == FALSE || nLineCount < 1 )		// get file header
	{
		strReply.Format ( "%s: No data in source file ' %s ' ", 
			(const char*) strTitle, 
			szSource );

		return strReply;
	}

	m_nQueueLength = nLineCount;									// lines to send

	if ((strReply = GetUploadToken(strTitle, m_nQueueLength)) != "")
	{
		return strReply;
	}

	if ( m_nUploadBatchSize < 1 )										// check have enough to send at least 1 record
	{
		strReply.Format ( "%s: %s", 
			(const char*) strTitle, 
			szERROR_BATCHSIZE );
		return strReply;												// no - not enough
	}

	if ( m_nQueueLength > m_nUploadBatchSize )
		m_nQueueLength = m_nUploadBatchSize;

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/SendTransactionData3");

	s.BeginSoap ( "Body" );
		s.Begin ( "SendTransactionData3" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );

			AddTransactionDataHeader ( &s, m_nQueueLength );		// loginCredentials + other stuff

			AddNullProductSaleData ( &s );							// no product sale data
			AddNullRewardPointData ( &s );							// no reward point data

//			s.Begin ( "financialTxnData" );
//				s.Begin ( "financialTxnRecord3" );
//					s.Element ( "connectingPOSTxnID", GetNextTransactionID() );
//					s.Element ( "mealSessionID", m_strSessionID );
//					s.Element ( "txnAmount", "0.99" );
//					s.Element ( "description", "Sandwich" );
//					s.Element ( "accountID", "6212822" );			// adam adams
//					s.Element ( "txnDateTime", strDate );
//					s.Element ( "txnType", "0" );
//					s.Element ( "originatingPOSID", m_strPOSID );
//					s.Element ( "flags", strFlags );
//					s.Element ( "newBalance", "9.01" );
//					s.Element ( "posInternalTxnID", "xxx" );	
//					s.Element ( "financialTransactionType", "6" );	// 6=adjustment
//					s.Element ( "vatRate", "0" );
//				s.End();
//			s.End();

			s.Begin ( "financialTxnData" );
				for ( int i = 0 ; i < m_nQueueLength ; i++ )
				{
					CCSV csvLine;
					fileSource.Read ( csvLine );

					s.Begin ( "FinancialTxnRecord3" );
					for (int i = 0; i < csvLine.GetSize(); i++)
					{
						s.Element(csvHeader.GetString(i), csvLine.GetString(i));
					}
					s.End();
				}
			s.End();

			AddNullTxnEchoData ( &s );						// no echo payment data
			AddNullCancellationData ( &s );					// no cancellation data
			AddNullControlRecordData ( &s );				// no control data
			AddNullDinerMatchAdviceData ( &s );				// no diner match data

		s.End();
	s.End();

	if (m_pData->m_bKeepXmlFiles == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("SendFinancialData.xml"), TRUE);
	}

	if ((m_strSendError = s.SendHttps(szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString())) == "")
	{
		m_strValidateError = ValidateReply(strTitle, &s, "errorStatus");
	}

	strReply = "";
	if (m_strSendError != "")
	{
		strReply.Format("%s: %s", 
			(const char*) strTitle, 
			m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}

	return strReply;
}

//**************************************************************************
//void CSSFourth::AddNullTxnRecord3 ( CSSSoap* pSoap )
//{
//	CString strDateTime = "";
//
//	pSoap->Begin ( "TxnRecord3" );							//
//		pSoap->Element ( "connectingPOSTxnID", "-1" );		//	
//		pSoap->Element ( "mealSessionID", "0" );			//
//		pSoap->Element ( "txnAmount", "0" );				//
//		pSoap->Element ( "description", "" );				// minoccurs = 0
//		pSoap->Element ( "accountID", "0" );				//
//		pSoap->Element ( "txnDateTime", strDateTime );		//
//		pSoap->Element ( "txnType", "0" );					//
//		pSoap->Element ( "originatingPOSID", "" );			// minoccurs = 0
//		pSoap->Element ( "flags", "0" );					//
//		pSoap->Element ( "newBalance", "0" );				//
//		pSoap->Element ( "posInternalTxnID", "" );			// minoccurs = 0
//	pSoap->End();
//}
//*******************************************************************

void CSSParentPay::AddLoginDetails ( CSSSoap* pSoap )
{
	pSoap->Begin ( "loginCredentials" );
		pSoap->Element ( "username", m_pData->m_strUsername );
		pSoap->Element ( "password", m_pData->m_strPassword );
		pSoap->Element ( "schoolID", m_pData->m_strSchoolID );
		pSoap->Element ( "connectingPOSID", m_pData->m_strPosID );
	pSoap->End();
}

void CSSParentPay::AddTransactionDataHeader ( CSSSoap* pSoap, int nQueueLength )
{
	CString strQueueLength;
	strQueueLength.Format ( "%d", nQueueLength );

	AddLoginDetails ( pSoap );								// loginCredentials;
	pSoap->Element ( "tillVendorName", m_pData->m_strVendorName );
	pSoap->Element ( "tillSoftwareVersion", szVERSION_SMARTPAY_ABOUT );
	pSoap->Element ( "uploadToken", m_strUploadToken );
	pSoap->Element ( "newTxnQueueLength", strQueueLength );
}

void CSSParentPay::AddNullProductSaleData ( CSSSoap* pSoap )		{ pSoap->Element ( "productSaleData", "" );		 }
void CSSParentPay::AddNullTxnEchoData ( CSSSoap* pSoap )			{ pSoap->Element ( "parentPayTxnEchoData", "" ); }
void CSSParentPay::AddNullRewardPointData ( CSSSoap* pSoap )		{ pSoap->Element ( "rewardPointData", "" );		 }
void CSSParentPay::AddNullFinancialTxnData ( CSSSoap* pSoap )		{ pSoap->Element ( "financialTxnData", "" );	 }
void CSSParentPay::AddNullCancellationData ( CSSSoap* pSoap )		{ pSoap->Element ( "cancellationData", "" );	 }
void CSSParentPay::AddNullControlRecordData ( CSSSoap* pSoap )		{ pSoap->Element ( "controlRecordData", "" );	 }
void CSSParentPay::AddNullDinerMatchAdviceData ( CSSSoap* pSoap )	{ pSoap->Element ( "dinerMatchAdviceData", "" ); }

//************************************************************************
// get Upload Token & set max allowable queue length
/* Reply 
<soap:Envelope>
 <soap:Body>
  <ReportQueueStatus2Response xmlns="https://www.parentpay.com/POSWS3/POSapi3">
   <ReportQueueStatus2Result>
    <errorStatus>0</errorStatus>
    <errorDescription>OK</errorDescription>
    <uploadToken>1054581608</uploadToken>
    <uploadBatchSize>190</uploadBatchSize>
    <serverStatus/>
    <lastReceivedPOSTxnID>0</lastReceivedPOSTxnID>
    <lastProcessedPOSTxnID>0</lastProcessedPOSTxnID>
   </ReportQueueStatus2Result>
  </ReportQueueStatus2Response>
 </soap:Body>
</soap:Envelope>
*******************************************************************/
// QueueLength = nu

CString CSSParentPay::GetUploadToken ( const char* szMainTitle, int nQueueLength )
{
	CString strTitle;		strTitle.Format ( "(%s)GetToken", szMainTitle );
	CString strLoginReply = m_pData->GetXmlLoginPath("TokenLoginReply.xml");

	CString strQueueLength;		strQueueLength.Format  ( "%d", nQueueLength );
	CString strLastPosTxnID = ::FormatInt64Value ( m_pData->GetLastPosTxnID() );

// login *********************************************

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.SetSoapAction ("https://www.parentpay.com/POSWS3/POSapi3/ReportQueueStatus2");

	s.BeginSoap ( "Body" );
		s.Begin ( "ReportQueueStatus2" );
		s.Attribute ( "xmlns", "https://www.parentpay.com/POSWS3/POSapi3" );
			AddLoginDetails ( &s );										// loginCredentials;
			s.Element ( "currentTxnQueueLength", strQueueLength );
			s.Element ( "lastPOSTxnID", strLastPosTxnID );				// get last transaction ID used
		s.End();
	s.End();

	if (m_pData->m_bKeepXmlLogin == TRUE)
	{
		s.WriteXML(m_pData->GetXmlLoginPath("ReportQueueStatusLogin.xml"), TRUE);
	}

	if ( ( m_strSendError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, strLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		if ( ( m_strValidateError = ValidateReply ( strTitle, &s, "errorStatus" ) ) == "" )
		{
			CXMLNodeArray* p1 = s.FindNode ( "uploadToken" );	// look for result label
			if (p1 != NULL)
			{
				m_strUploadToken = p1->m_strValue;
			}

			p1 = s.FindNode ( "uploadBatchSize" );				// look for result label
			if ( p1 != NULL )
			{
				m_nUploadBatchSize = atoi ( p1->m_strValue );		// max number of tx records that can be sent

				if ( m_pData->m_bKeepXmlFiles == TRUE )
				{
					CCSV csv;
					csv.Add ( CSSDate::GetCurrentDate().GetDate() );
					csv.Add ( CSSTime::GetCurrentTime().GetTime() );
					csv.Add ( strQueueLength );
					csv.Add ( m_nUploadBatchSize );
					CSSFile file;
					file.Open ( m_pData->GetXmlLoginPath ( "batchSize.log" ), "ab" );
					file.Write ( csv );
					file.Close();
				}
			}
		}
	}

	if (m_pData->m_bKeepXmlFiles == FALSE)
	{
		CFileRecycler::SendToRecycleBin(strLoginReply);
	}

	CString strReply = "";
	if (m_strSendError != "")
	{
		strReply.Format("%s: %s", 
			(const char*) strTitle, 
			m_strSendError);
	}
	else if (m_strValidateError != "")
	{
		strReply = m_strValidateError;
	}

	return strReply;
}

//*******************************************************************
// // Labels not consistance in upper & lower case

CString CSSParentPay::ValidateReply ( const char* szTitle, CSSSoap* pSoap, const char* szLabel )
{
	CString strReply = "";

	CXMLNodeArray* p1 = pSoap->FindNode ( szLabel );						// look for result label
	if ( p1 != NULL )
	{
		CString strStatus = p1->m_strValue;
		if (strStatus != "0")
		{
			CXMLNodeArray* p2 = pSoap->FindNode("ErrorDescription");		// look for result label
			if (p2 == NULL)
			{
				p2 = pSoap->FindNode("errorDescription");				// not found - try different spelling
			}

			if (p2 != NULL)
			{
				strReply.Format("%s: (%s %s) %s", 
					szTitle, 
					szLabel, 
					(const char*)strStatus,
					(const char*)p2->m_strValue);
			}
			else
			{
				strReply.Format("%s: (%s %s) No error description found in XML reply", 
					szTitle, 
					szLabel, 
					(const char*) strStatus);
			}
		}
	}
	else
	{
		strReply.Format("%s: No %s found in XML reply", szTitle, szLabel);
	}

	return strReply;
}

//****************************************************************
