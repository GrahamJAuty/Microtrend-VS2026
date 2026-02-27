//**********************************************************************
#include "TucasiJSON.h"
//**********************************************************************

extern const char* szVERSION_SMARTPAY_ABOUT;

//static 	const char* szIPADDRESS = "www.scopay.com";
//static 	const char* szIPADDRESS = "192.168.0.103";

//**********************************************************************

CSSTucasiJson::CSSTucasiJson ( CTucasiJsonData* pData )
	: m_pData(pData)
{
}

//**********************************************************************
//	bool bUseHttps = TRUE;
//	const char* szIPAddress = "vm1007uat24.tucasi.com";
//	const char* szObjectName = "GatewayWebapp/data/schoolid/2009888/cashlessv2/transactions?lastTransactionNum=0";
//	const char* szBody = "";
//	const char* szResponseFilename = "transactions.json";
//	const char* szUsername = "sharp_test@tucasi.com";
//	const char* szPassword = "Sh@rp123";
//	const char* szProviderName = "Microtrend";
//	const char* szProviderVersion = "0.01";
//
//	CString strReply = TucasiGet ( bUseHttps, szIPAddress, szObjectName, szBody, szResponseFilename, szUsername, szPassword, szProviderName, szProviderVersion );

CString CSSTucasiJson::GetPayments ( const char* szCsvFilename, const char* szResponseFilename )
{
	CString strError = "";

	bool bUseHttps = TRUE;
	CString strObject = "";
	strObject.Format ( "GatewayWebapp/data/schoolid/%s/cashlessv2/transactions?lastTransactionNum=%I64d", 
		(const char*) m_pData->m_strSchoolID, 
		m_pData->m_nLastPaymentID );

	const char* szBody = "";

//	if ( m_pData->m_bKeepJsonFile == TRUE )	
//	{
//		CSSFile file;
//		file.Open ( m_pData->GetXmlLoginPath("GetPaymentLog.txt"), "ab" );
//
//		CSSTime time;
//		time.SetCurrentTime();
//
//		CString strLine;	strLine.Format ( "%s %2.2d:%2.2d:%2.2d %s", CSSDate::GetCurrentDate().GetDate(), time.GetHour(), time.GetMinute(), time.GetSecond(), strObject );
//		file.WriteLine ( strLine );
//		file.Close();	
//	}

	/*
	{
		CSSFile fileTucasiLog;
		if (fileTucasiLog.Open(".\\TucasiLog.txt", "ab") == TRUE)
		{
			CSSTime time;
			time.SetCurrentTime();

			CString strLine = "";
			strLine.Format("%s %2.2d:%2.2d:%2.2d", 
				CSSDate::GetCurrentDate().GetDate(), time.GetHour(), time.GetMinute(), time.GetSecond() );

			fileTucasiLog.WriteLine("**********");
			fileTucasiLog.WriteLine("");
			fileTucasiLog.WriteLine("Tucasi Get");
			fileTucasiLog.WriteLine("");
			fileTucasiLog.WriteLine(strLine);

			fileTucasiLog.WriteLine(bUseHttps ? "HTTPS" : "NOT HTTPS");
			fileTucasiLog.WriteLine(m_pData->m_strIPAddress);
			fileTucasiLog.WriteLine(strObject);
			fileTucasiLog.WriteLine(szBody);
			fileTucasiLog.WriteLine(szResponseFilename);
			fileTucasiLog.WriteLine(m_pData->m_strUsername);
			fileTucasiLog.WriteLine(m_pData->m_strPassword);
			fileTucasiLog.WriteLine(m_pData->m_strProviderName);
			fileTucasiLog.WriteLine(szVERSION_SMARTPAY_ABOUT);
			fileTucasiLog.WriteLine(System.GetInternetAgentString());
			fileTucasiLog.Close();
		}
	}
	*/

	CString strReply = TucasiGet ( bUseHttps, m_pData->m_strIPAddress, strObject, szBody, szResponseFilename, m_pData->m_strUsername, m_pData->m_strPassword, m_pData->m_strProviderName, szVERSION_SMARTPAY_ABOUT, System.GetInternetAgentString() );

	/*
	{
		CSSFile fileTucasiLog;
		if (fileTucasiLog.Open(".\\TucasiLog.txt", "ab") == TRUE)
		{
			CString strLogReply = "Reply: ";
			strLogReply += strReply;
			fileTucasiLog.WriteLine("");
			fileTucasiLog.WriteLine(strLogReply);
			fileTucasiLog.WriteLine("");
			fileTucasiLog.Close();
		}
	}
	*/

	if ( strReply == "" )	// no comms error
	{
		CTucasiJson j;
		if ( j.ParseFile ( szResponseFilename ) == TRUE )
		{
			int n = j.GetObjectCount();		// number of objects
			if ( n > 0 )
			{
				if ( n == 1 )				// may be status or one line of data
				{
					if ( j.FindLabel ( "statusCode" ) == TRUE )
					{
						strError.Format ( "statusCode: %s\nmessage: %s", j.GetValue ( "statusCode", 0 ), j.GetValue ( "message", 0 ) );
						return strError;
					}
				}

				// must be data

				CSSUnsortedDatabase db;
				if ( db.CreateSSDatabase ( szCsvFilename, "", m_pData->m_strPaymentHeader, 10 ) == DB_ERR_NONE )	// create new db & open in Read\Write.
				{
					for ( int n = 0 ; n < j.GetObjectCount() ; n++ )			// get all objects
					{
						db.AddBlankRecord();
						int x = j.GetLabelCount();
						for ( int i = 0 ; i < j.GetLabelCount() ; i++ )			// get all available labels
						{
							CString strLabel = j.GetLabel(i);
							CString strValue = j.GetValue ( strLabel, n );
							db.Set ( strLabel, strValue );
						}
					}

					db.Close();
				}
				else
					strError = db.GetError();
			}
			else																// just a message
			{
				CSSFile file;
				if ( file.Open ( szResponseFilename, "rb" ) == TRUE )
				{
					CString strBuf;
					while ( file.ReadLine ( strBuf ) == TRUE )				// may be more than one line
					{
						if ( strBuf == "[]" )								// no data 
							break;

						strError += strBuf;
					}
					file.Close();
				}

			}
		}
		else
			strError = "Unable to open JSON response file!";
	}
	else												// comms error
		strError = strReply;

	return strError;
}

//**********************************************************************
/*
	j.AddObject();
	j.Add ( "transactionNumber", "123" );
	j.Add ( "transactionItemNumber", "1", FALSE );		// FALSE (no quotes) - see received data (not same as documentation)
	j.Add ( "paymentDate", "2014-10-24 12:00:00.000" );
	j.Add ( "paymentAmount", "-150" );
	j.Add ( "description", "Pizza" );
	j.Add ( "accountID", "12345" );
	j.Add ( "accountName", "John Smith" );

	j.AddObject();
	j.Add ( "transactionNumber", "123" );
	j.Add ( "transactionItemNumber", "2", FALSE );
	j.Add ( "paymentDate", "2014-10-24 12:00:00.000" );
	j.Add ( "paymentAmount", "-100" );
	j.Add ( "description", "Chips" );
	j.Add ( "balance", "-250" );
	j.Add ( "accountID", "12345" );
	j.Add ( "accountName", "John Smith" );
*/

CString CSSTucasiJson::SendUpdates(const char* szCsvDataFile, const char* szResponseFilename)
{
	CString strError = "";

	CSSFile file;
	CCSV csvHeader;
	if (file.Open(szCsvDataFile, "rb") == TRUE)
	{
		file.Read(csvHeader);
		file.Close();
	}

	CSSUnsortedDatabase dbUpdates;
	if (dbUpdates.OpenSSDatabaseReadOnly(szCsvDataFile, 10) == DB_ERR_NONE)	// open intermediate csv importfile
	{
		dbUpdates.MoveFirst();

		CString strLabel, strValue;
		CTucasiJsonOut j;

		do
		{
			j.AddObject();

			for (int i = 0; i < csvHeader.GetSize(); i++)
			{
				strLabel = csvHeader.GetString(i);
				dbUpdates.Get(strLabel, strValue);

				j.Add(strLabel, strValue);
			}
		} while (dbUpdates.MoveNext() == TRUE);

		CString strBody;
		j.GetJson(strBody);

		if (m_pData->m_bKeepJsonFile == TRUE)
		{
			CSSFile file;
			file.Open(m_pData->GetXmlLoginPath("SendUpdateData.json"), "wb");
			file.WriteLine(strBody);
			file.Close();
		}

		CString strObject = "";
		strObject.Format("GatewayWebapp/data/schoolid/%s/cashlessv2/data?lastTransactionNumber=%I64d", 
			(const char*) m_pData->m_strSchoolID, 
			m_pData->m_nLastPaymentID);

		//		if ( m_pData->m_bKeepJsonFile == TRUE )	
		//		{
		//			CSSFile file;
		//			file.Open ( m_pData->GetXmlLoginPath("SendUpdateLog.txt"), "ab" );
		//
		//			CString strLine;	strLine.Format ( "%s %s %s", CSSDate::GetCurrentDate().GetDate(), CSSTime::GetCurrentTime().GetTime(), strObject );
		//			file.WriteLine ( strLine );
		//			file.Close();	
		//		}

		bool bUseHttps = TRUE;

		/*
		{
			CSSFile fileTucasiLog;
			if (fileTucasiLog.Open(".\\TucasiLog.txt", "ab") == TRUE)
			{
				CSSTime time;
				time.SetCurrentTime();

				CString strLine = "";
				strLine.Format("%s %2.2d:%2.2d:%2.2d",
					CSSDate::GetCurrentDate().GetDate(), time.GetHour(), time.GetMinute(), time.GetSecond());

				fileTucasiLog.WriteLine("**********");
				fileTucasiLog.WriteLine("");
				fileTucasiLog.WriteLine("Tucasi Post");
				fileTucasiLog.WriteLine("");
				fileTucasiLog.WriteLine(strLine);

				fileTucasiLog.WriteLine(bUseHttps ? "HTTPS" : "NOT HTTPS");
				fileTucasiLog.WriteLine(m_pData->m_strIPAddress);
				fileTucasiLog.WriteLine(strObject);
				fileTucasiLog.WriteLine(strBody);
				fileTucasiLog.WriteLine(szResponseFilename);
				fileTucasiLog.WriteLine(m_pData->m_strUsername);
				fileTucasiLog.WriteLine(m_pData->m_strPassword);
				fileTucasiLog.WriteLine(m_pData->m_strProviderName);
				fileTucasiLog.WriteLine(szVERSION_SMARTPAY_ABOUT);
				fileTucasiLog.WriteLine(System.GetInternetAgentString());
				fileTucasiLog.Close();
			}
		}
		*/

		strError = TucasiPost(bUseHttps, m_pData->m_strIPAddress, strObject, strBody, szResponseFilename, m_pData->m_strUsername, m_pData->m_strPassword, m_pData->m_strProviderName, szVERSION_SMARTPAY_ABOUT, System.GetInternetAgentString());

		/*
		{
			CSSFile fileTucasiLog;
			if (fileTucasiLog.Open(".\\TucasiLog.txt", "ab") == TRUE)
			{
				CString strLogReply = "Reply: ";
				strLogReply += strError;
				fileTucasiLog.WriteLine("");
				fileTucasiLog.WriteLine(strLogReply);
				fileTucasiLog.WriteLine("");
				fileTucasiLog.Close();
			}
		}
		*/
	}
	else				// database error
	{
		strError = dbUpdates.GetError();
	}

	return strError;
}

//**********************************************************************

bool CSSTucasiJson::LogResponse ( const char* szResponseFile )
{
	bool bReply = FALSE;

	CTucasiJson j;
	if ( j.ParseFile ( szResponseFile ) == TRUE )
	{
		CSSFile fileLog;
		if ( ( bReply = fileLog.Open ( m_pData->GetResponseLogFilename(), "ab" ) ) == TRUE )
		{
			CString strLine;

			strLine.Format ( "#DATE,%s,%s", CSSDate::GetCurrentDate().GetDate(), CSSTime::GetCurrentTime().GetTime() );
			fileLog.WriteLine ( strLine );

			int nObjectCount = j.GetObjectCount();											// number of objects
			if ( nObjectCount > 0 )
			{
				if ( nObjectCount == 1 && j.FindLabel ( "statusCode" ) == TRUE )			// may be status or one line of data
				{
					strLine.Format ( "#STAT,%s,%s", j.GetValue ( "statusCode", 0 ), j.GetValue ( "message", 0 ) );
					fileLog.WriteLine ( strLine );
					fileLog.WriteLine ( "" );
				}
				else															// must be data
				{
					for ( int nIdxRecord = 0 ; nIdxRecord < nObjectCount ; nIdxRecord++ )	// get all objects
					{
						strLine = "#RESP";

						for ( int i = 0 ; i < j.GetLabelCount(nIdxRecord) ; i++ )
						{
							CString strLabel = j.GetLabel ( nIdxRecord, i );
							CString strValue = j.GetValue ( nIdxRecord, i ); 
							
							CString strBuf = "";
							strBuf.Format (",%s,%s", 
								(const char*) strLabel, 
								(const char*) strValue );

							strLine += strBuf;
						}
						fileLog.WriteLine ( strLine );
					}

					if ( nObjectCount > 0 )
					{
						strLine.Format ( "#RXLN,%d", nObjectCount );
						fileLog.WriteLine ( strLine );
						fileLog.WriteLine ( "" );
					}
				}
			}
			else															// just a message
			{
				strLine = "";
				CSSFile file;
				if ( file.Open ( szResponseFile, "rb" ) == TRUE )
				{
					CString strBuf;
					while ( file.ReadLine ( strBuf ) == TRUE )				// may be more than one line
					{
						if ( strBuf == "[]" )								// no data 
							break;

						if ( strLine == "" )
							strLine = "#MESS,";

							strLine += strBuf;
					}
					file.Close();
				}

				if ( strLine != "" )
				{
					fileLog.WriteLine ( strLine );
					fileLog.WriteLine ( "" );
				}
			}

			fileLog.Close();
		}
	}

	return bReply;
}

//********************************************************************
