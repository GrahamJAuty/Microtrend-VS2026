//**********************************************************************
// Test credentials
// App ID 		18
// Service 1	248
// Service 2	62592
//**********************************************************************
#include "WisePay.hpp"
//*********************************************************************

//static const char* szPASSWORD			= "microtrend8793345tP";
//static const char* szIPADDRESS		= "www.wisepay-api.co.uk";
static const char* szIPADDRESS			= "api.wisepay.co.uk";
static const char* szOBJECTNAME			= "/services/microtrend.asmx";

static const char* szHEADER_PAYMENTS	= "uID,cart_student_sims_id,cart_student_name,cart_timestamp,price_value";
static const char* szHEADER_ACCOUNTS	= "uID,acc_student_SIMS_unique_id,acc_student_first_name,acc_student_last_name,acc_user_email,acc_student_year,acc_student_dob,acc_student_class,acc_student_gender";

//*********************************************************************

CSSWisePay::CSSWisePay ( CWisePayData* pData )
	: m_pData(pData)
{
	m_strError = "";
}

/********************************************************************/
/*	Get Payments													*/
/********************************************************************
POST /services/microtrend.asmx HTTP/1.1
Host: www.wisepay.co.uk
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length

<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <WisePayGetPayments xmlns="https://www.wisepay.co.uk/microtrend_ws">
      <app_id>string</app_id>
      <meal_product_id>string</meal_product_id>
      <wp_password>string</wp_password>
      <last_transaction>string</last_transaction>
    </WisePayGetPayments>
  </soap12:Body>
</soap12:Envelope>
**********************************************************************/

CString CSSWisePay::GetPayments ( int nAccountNo, const char* szLoginReply, const char* szCsvFilename )
{
	int nProductID				= ( nAccountNo == 1 ) ? m_pData->m_nAccountID1 : m_pData->m_nAccountID2;
	__int64 nLastTransaction	= ( nAccountNo == 1 ) ? m_pData->m_nLastAccount1 : m_pData->m_nLastAccount2;

	CString strProductID = "";
	strProductID.Format ( "%d", nProductID );
	CString strLastTransaction = FormatInt64Value( nLastTransaction );

	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
		s.Begin ( "WisePayGetPayments" );
		s.Attribute ( "xmlns", "https://www.wisepay.co.uk/microtrend_ws" );
			s.Element ( "app_id", m_pData->m_strSchoolID );
			s.Element ( "meal_product_id", strProductID );
			s.Element ( "wp_password", m_pData->m_strPassword );
			s.Element ( "last_transaction", strLastTransaction );
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("PaymentLogin.xml"), TRUE );

	if ( ( m_strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		remove ( szCsvFilename );

		CSSUnsortedDatabase db;
		if ( db.CreateSSDatabase ( szCsvFilename, "", szHEADER_PAYMENTS, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
		{
			CXMLNodeArray arFound;
			if ( s.FindNodes ( arFound, "data" ) > 0 )
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
			m_strError = db.GetError();
	}

	return m_strError;
}

/**********************************************************************
POST /services/microtrend.asmx HTTP/1.1
Host: www.wisepay.co.uk
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length

<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <WisePaySendTransactions xmlns="https://www.wisepay.co.uk/microtrend_ws">
      <xmltext>xml</xmltext>
    </WisePaySendTransactions>
  </soap12:Body>
</soap12:Envelope>

/**********************************************************************
<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:mic="https://www.wisepay.co.uk/microtrend_ws">
   <soap:Header/>
   <soap:Body>
      <mic:WisePaySendTransactions>
         <!--Optional:-->
         <mic:xmltext>
	  	   <root>
			    <wp_password>##########</wp_password>
			    <app_id>18</app_id>
			    <transactions>
				    <transaction>
					    <student_id>Z123456789A</student_id>
					    <description>Dinner, Coke, Yoghurt</description>
					    <value>3.30</value>
					    <transaction_id>123456</transaction_id>
					    <balance>26.00</balance>
					    <cart_date_time>2009-05-15 08:01:00</cart_date_time>
				    </transaction>
				    <transaction>
					    ..
				    </transaction>
			    </transactions>
		   </root>
         </mic:xmltext>
      </mic:WisePaySendTransactions>
   </soap:Body>
</soap:Envelope>
**********************************************************************/

CString CSSWisePay::SendTransactions ( const char* szSource, const char* szLoginReply )
{
	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == TRUE )
	{
		CCSV csvHeader;
		fileSource.Read(csvHeader);	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "mic:WisePaySendTransactions" );
			s.Attribute ( "xmlns:mic", "https://www.wisepay.co.uk/microtrend_ws" );
				s.Begin ( "mic:xmltext" );
					s.Begin ( "root" );
						s.Element ( "wp_password", m_pData->m_strPassword );	// "<wp_password>microtrend8793345tP</wp_password>"
						s.Element ( "app_id", m_pData->m_strSchoolID );			// "<app_id>18</app_id>"
						s.Begin ( "transactions" );
						{
							CCSV csv;
							while ( fileSource.Read ( csv ) == TRUE )
							{
								s.Begin ( "transaction" );
									for ( int i = 0 ; i < csv.GetSize() ; i++ )
										s.Element ( csvHeader.GetString(i), csv.GetString(i) );
								s.End();
							}
							fileSource.Close();
						}
						s.End();
					s.End();
				s.End();
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendTransaction1Data.xml"), TRUE );

		m_strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	}
	else
		m_strError.Format ( "Cannot open source file ' %s ' ", szSource );

	return m_strError;
}

/**********************************************************************
<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:mic="https://www.wisepay.co.uk/microtrend_ws">
   <soap:Header/>
   <soap:Body>
      <mic:WisePaySendTransactionsMultiplePurse>
         <!--Optional:-->
         <mic:xmltext>
	  	   <root>
			    <wp_password>##########</wp_password>
			    <app_id>18</app_id>
			    <transactions>
				    <transaction>
					    <student_id>Z123456789A</student_id>
					    <description>Dinner, Coke, Yoghurt</description>
					    <value>3.30</value>
						<product_id>nn</product_id>
					    <transaction_id>123456</transaction_id>
					    <balance>26.00</balance>
					    <cart_date_time>2009-05-15 08:01:00</cart_date_time>
				    </transaction>
				    <transaction>
					    ..
				    </transaction>
			    </transactions>
		   </root>
         </mic:xmltext>
      </mic:WisePaySendTransactions>
   </soap:Body>
</soap:Envelope>
**********************************************************************/
CString CSSWisePay::SendTransactions2 ( const char* szSource, const char* szLoginReply )
{
	CSSFile fileSource;
	if ( fileSource.Open ( szSource, "rb" ) == TRUE )
	{
		CCSV csvHeader;
		fileSource.Read(csvHeader);	

		CSSSoap s ( m_pData->m_strSoapVersion );
		s.BeginSoap ( "Body" );
			s.Begin ( "mic:WisePaySendTransactionsMultiplePurse" );
			s.Attribute ( "xmlns:mic", "https://www.wisepay.co.uk/microtrend_ws" );
				s.Begin ( "mic:xmltext" );
					s.Begin ( "root" );
						s.Element ( "wp_password", m_pData->m_strPassword );	// "<wp_password>microtrend8793345tP</wp_password>"
						s.Element ( "app_id", m_pData->m_strSchoolID );			// "<app_id>18</app_id>"
						s.Begin ( "transactions" );
						{
							CCSV csv;
							while ( fileSource.Read ( csv ) == TRUE )
							{
								s.Begin ( "transaction" );
									for ( int i = 0 ; i < csv.GetSize() ; i++ )
										s.Element ( csvHeader.GetString(i), csv.GetString(i) );
								s.End();
							}
							fileSource.Close();
						}
						s.End();
					s.End();
				s.End();
			s.End();
		s.End();

		if ( m_pData->m_bKeepXmlFiles == TRUE )
			s.WriteXML ( m_pData->GetXmlLoginPath("SendTransaction2Data.xml"), TRUE );

		m_strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString());
	}
	else
		m_strError.Format ( "Cannot open source file ' %s ' ", szSource );

	return m_strError;
}


//**********************************************************************
/*<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:mic="https://www.wisepay.co.uk/microtrend_ws">
   <soap:Header/>
   <soap:Body>
      <mic:WisePaySendBalances>
         <!--Optional:-->
         <mic:xmltext>
            <root>
			    <wp_password>##########</wp_password>
			    <app_id>18</app_id>
			    <balances>
				    <balance>
					    <student_id>3702216206</student_id>
					    <balance>10.00</balance>
					    <balance2>10.00</balance>
				    </balance>
				    <balance>
					    <student_id>6912161647</student_id>
					    <balance>20.00</balance>
					    <balance2>10.00</balance>
				    </balance>
			    </balances>
            </root>
         </mic:xmltext>
      </mic:WisePaySendBalances>
   </soap:Body>
</soap:Envelope>
**********************************************************************/

//CString CSSWisePay::SendBalances ( const char* szSource, const char* szLoginReply )
//{
//	CSSFile fileSource;
//	if ( fileSource.Open ( szSource, "rb" ) == TRUE )
//	{
//		CCSV csvHeader;
//		fileSource.Read ( csvHeader );	
//
//		CSSSoap s ( m_pData->m_strSoapVersion );
//		s.BeginSoap ( "Body" );
//			s.Begin ( "mic:WisePaySendBalances" );
//			s.Attribute ( "xmlns:mic", "https://www.wisepay.co.uk/microtrend_ws" );
//				s.Begin ( "mic:xmltext" );
//					s.Begin ( "root" );
//						s.Element ( "wp_password", m_pData->m_strPassword );
//						s.Element ( "app_id", m_pData->m_strSchoolID );
//						s.Begin ( "balances" );
//						{
//							CCSV csv;
//							while ( fileSource.Read ( csv ) == TRUE )
//							{
//								s.Begin ( "balance" );	
//									for ( int i = 0 ; i < csv.GetSize() ; i++ )
//										s.Element ( csvHeader.GetString(i), csv.GetString(i) );
//								s.End();
//							}
//							fileSource.Close();
//						}
//						s.End();
//					s.End();
//				s.End();
//			s.End();
//		s.End();
//
//		if ( m_pData->m_bKeepXmlFiles == TRUE )
//			s.WriteXML ( m_pData->GetXmlLoginPath("SendBalanceData.xml"), TRUE );
//
//		m_strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply );
//	}
//	else
//		m_strError = ::OpenFileFailureMessage ( szSource, DB_READONLY );		// "Cannot open file ' %s ' for Reading"
//
//	return m_strError;
//}
//
/********************************************************************/
/*	Child Accounts													*/
/********************************************************************
POST /services/microtrend.asmx HTTP/1.1
Host: www.wisepay.co.uk
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length

<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <WisePayChildAccounts xmlns="https://www.wisepay.co.uk/microtrend_ws">
      <app_id>string</app_id>
      <wp_password>string</wp_password>
    </WisePayChildAccounts>
  </soap12:Body>
</soap12:Envelope>

<acc_application_id>18</acc_application_id>
<acc_student_SIMS_unique_id>1122334455</acc_student_SIMS_unique_id>
<acc_student_first_name>Simon</acc_student_first_name>
<acc_student_last_name>Watmore</acc_student_last_name>
<acc_guardian_first_name>Simon</acc_guardian_first_name>
<acc_guardian_last_name>Watmore</acc_guardian_last_name>
<acc_user_email>simon@watmore.com</acc_user_email>
<acc_student_year>11</acc_student_year>
<acc_student_dob>1968-07-02T00:00:00+01:00</acc_student_dob>
<acc_student_class>abc</acc_student_class>
<acc_student_gender>M</acc_student_gender>
<acc_phone>01932 224939</acc_phone>
<acc_mobile_phone>07939 289499</acc_mobile_phone>
<acc_address_1>53 Mount Felix</acc_address_1>
<acc_address_2>Rivermount</acc_address_2>
<acc_city>Walton on Thames</acc_city>
<acc_postcode>KT12 2PJ</acc_postcode>
<acc_county>Surrey</acc_county>

**********************************************************************/

CString CSSWisePay::GetChildAccounts ( const char* szLoginReply, const char* szCsvFilename )
{
	CSSSoap s ( m_pData->m_strSoapVersion );
	s.BeginSoap ( "Body" );
		s.Begin ( "WisePayChildAccounts" );
		s.Attribute ( "xmlns", "https://www.wisepay.co.uk/microtrend_ws" ); 
			s.Element ( "app_id", m_pData->m_strSchoolID );
			s.Element ( "wp_password", m_pData->m_strPassword );
		s.End();
	s.End();

	if ( m_pData->m_bKeepXmlLogin == TRUE )
		s.WriteXML ( m_pData->GetXmlLoginPath("AccountsLogin.xml"), TRUE );

	if ( ( m_strError = s.SendHttps ( szIPADDRESS, szOBJECTNAME, szLoginReply, TRUE, System.GetInternetAgentString()) ) == "" )
	{
		remove ( szCsvFilename );

		CXMLNodeArray arFound;
		if ( s.FindNodes ( arFound, "data" ) > 0 )
		{
			CSSUnsortedDatabase db;
			if ( db.CreateSSDatabase ( szCsvFilename, "", szHEADER_ACCOUNTS, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
			{
				for ( int n = 0 ; n < arFound.GetSize() ; n++ )			// number of records
				{
					db.AddBlankRecord();
					CXMLNodeArray* node = arFound.GetAt(n);
					for ( int i = 0 ; i < node->GetSize() ; i++ )		// number of items per record
					{
						if ( node->GetAt(i)->IsText() == TRUE )
							db.Set ( node->GetAt(i)->GetName(), node->GetAt(i)->GetValue() );
					}
				}
				db.Close();
			}
			else
				m_strError = db.GetError();
		}	
	}
	return m_strError;
}

//********************************************************************
