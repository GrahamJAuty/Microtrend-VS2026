//*******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "SalesExportData.h"
#include "SchoolCommsData.h"
#include "SchoolCommsExport.h"
#include "TucasiJSONExport.h"
#include "TucasiSOAPExport.h"
#include "WebPaymentBalanceFile.h"
#include "WebPaymentDefines.h"
#include "WebPaymentExportCSVLog.h"
#include "WebPaymentPurchaseFile.h"
#include "WisePayExport.h"
//*******************************************************************
#include "WebPaymentExportTxLog.h"
//*******************************************************************
#include "WebPaymentExport.h"
//*******************************************************************

CWebPaymentExport::CWebPaymentExport( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_nBalanceCount = 0;
	m_nPurchaseCount = 0;
	m_bKeepXmlFiles = FALSE;
	m_bHaveService2 = FALSE;
	m_webOptions.LoadRow(NULL);
}

//*******************************************************************
// Send account balances - called from main menu

void CWebPaymentExport::SendAccountBalances()
{
	int nExportReqd = IDYES;											// assume export required
	if ( m_webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE )
	{
		nExportReqd = Prompter.YesNo( "Please confirm - Export account balances now?", "Send Acccount Balances", TRUE, TRUE );
	}

	if (nExportReqd == IDYES)
	{
		WebPaymentExportBalances();
	}
}

//*******************************************************************
// Called after importing payments
// or
// if bIsEndOfDay == TRUE from main menu 'Send End of Day' option
//
// sets errors message

bool CWebPaymentExport::SendAccountUpdates(bool bIsEndOfDay)
{
	m_strError = "";

	bool bIsSetUp = FALSE;
	bool bExportUpdates = FALSE;
	bool bExportBalances = FALSE;
	CString strDbBalanceList = "";

	if (bIsEndOfDay == TRUE)
	{
		bExportBalances = TRUE;
	}
	else
	{
		if (m_webOptions.m_Row.GetEODAfterPaymentFlag() == TRUE)
		{
			bExportBalances = m_webOptions.m_Row.TimeForEndOfDay();
		}
	}
//
	if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTPAY )				// NOTE ** this routine not required by ParentPay
	{
		bIsSetUp = TRUE;													// nothing to do
		bExportUpdates = FALSE;
		bExportBalances = FALSE;
	}	

	else if (System.GetWebPaymentType() == nWEBPAYMENT_TUCASI)
	{
		CTucasiSoapData data;
		data.Read();
		bIsSetUp = data.IsSet();
		bExportUpdates = data.m_bExportUpdates;
		m_bKeepXmlFiles = data.m_bKeepXmlFiles;

		if (data.m_bExportBalances == FALSE)
		{
			bExportBalances = FALSE;
		}
	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_WISEPAY)
	{
		CWisePayData data;
		data.Read();
		bIsSetUp = data.IsSet();
		m_bKeepXmlFiles = data.m_bKeepXmlFiles;
		m_bHaveService2 = data.m_bImportAccount2;					// see if using service2
		bExportUpdates = TRUE;
		bExportBalances = FALSE;
	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS)
	{
		CSchoolcommsData data;
		data.Read();
		bIsSetUp = data.IsSet();
		m_bKeepXmlFiles = data.m_bKeepXmlFiles;
		strDbBalanceList = data.GetBalanceDBListFilename();
		bExportUpdates = TRUE;
	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_TUCASIv2)
	{
		CTucasiJsonData data;
		data.Read();

		bIsSetUp = data.IsSet();

		if (data.m_bExportBalances == FALSE)
		{
			m_strError = szMSG_EXPORTDISABLED;
		}
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SQUID )			// NOTE ** this routine not required by sQuid
	{
		bIsSetUp = TRUE;												// nothing to do
		bExportUpdates = FALSE;
		bExportBalances = FALSE;
	}	

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTMAIL )
	{
		bIsSetUp = TRUE;												// nothing to do ( all done via import payments )
		bExportUpdates = FALSE;
		bExportBalances = FALSE;
	}
	
//

	if ( bIsSetUp == FALSE || m_strError != "" )						// check interface setup or error message set
	{
		if ( CModuleGlobals::IsBackgroundModule() == TRUE )
		{
			m_strError = "";
		}
		else if ( m_strError == "" )
		{
			m_strError = szINTERFACENOTSET;								// inteface not set up
		}
		
		return FALSE;
	}

//
	if (bExportUpdates == FALSE && bExportBalances == FALSE)			// see if any export required
	{
		return FALSE;
	}
//

	if (m_webOptions.m_Row.TimeForUpload() == FALSE)						// upload not allowed at this point in time
	{
		return FALSE;
	}
// 
	int nExportReqd = IDYES;											// assume export required
	CString strEODCaption = "";
	strEODCaption.Format ( "%s End of Day", 
		(const char*) System.GetWebPaymentDisplayName() );

	if ( bIsEndOfDay == TRUE )
	{
		if ( m_webOptions.m_Row.TimeForEndOfDay() == FALSE )					// see if web access allowed for EndofDay balances
		{	
			if ( CModuleGlobals::IsBackgroundModule() == FALSE )
			{
				CString strMsg = m_webOptions.m_Row.GetError();;
				strMsg += "\n\nDo you still wish to continue?";
		
				if ( Prompter.YesNo( strMsg, strEODCaption, FALSE, FALSE ) == IDNO )
				{
					nExportReqd = IDNO;									// EOD balances not required
				}
			}

			bExportBalances = FALSE;								// EOD balances not required
		}
	}

//
	if ( ( bExportUpdates == TRUE || bExportBalances == TRUE ) && nExportReqd == IDYES )
	{
		if ( bIsEndOfDay == TRUE )
		{
			if ( m_webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE)
			{
				nExportReqd = Prompter.YesNo( "Please confirm - Export End of Day now?", strEODCaption, TRUE, TRUE );
			}
		}

		if ( nExportReqd == IDYES )
		{
			if (bExportUpdates == TRUE)
			{
				ExportTransactions();										// export any new purchases
			}

			if (strDbBalanceList != "")									// balance list file may be set
			{
				if (::FileExists(strDbBalanceList) == TRUE)			// see if file exists, may have been added to by Export transactions
				{
					bExportBalances = TRUE;
				}
			}

			if (bExportBalances == TRUE && m_strError == "")
			{
				WebPaymentExportBalances(strDbBalanceList);						// export balance list or EOD balances

				if (m_strError == "" && strDbBalanceList != "")			// see if balance list file been created
				{
					CFileRecycler::SendToRecycleBin(strDbBalanceList);				// clear balance list file
				}
			}
		}
	}

	return ( nExportReqd == IDYES ) ? TRUE : FALSE;
}

//*******************************************************************
//*** End of Day Transactions ***************************************
// used by:		nWEBPAYMENT_TUCASI
//				nWEBPAYMENT_WISEPAY ( 2 services )
//				nWEBPAYMENT_SCHOOLCOMMS
//
// not used:	nWEBPAYMENT_PARENTPAY
//				nWEBPAYMENT_SQUID
//				nWEBPAYMENT_PARENTMAIl
//*******************************************************************

void CWebPaymentExport::ExportTransactions()
{
	m_strError = "";
	CString strLogComment = "";
	int nResultCode = nWEBEXPORT_ERROR;										// assume have post error
	CString strWebAtcUpdates = Filenames.GetWebPaymentUpdatesFilename("dat");		// ..\{web}\webupdates.dat
	CString strWebCsvFile1 = System.GetWebPaymentPath("updates.csv");			// was called "purchases.csv"
	CString strWebCsvFile2 = System.GetWebPaymentPath("updates2.csv");			// Servce 2 transactions

	if (strWebAtcUpdates != "")														// see if this used for current web payment
	{
		if (::FileExists(strWebAtcUpdates) == TRUE)								// see if have any web updates lurking
		{
			CString strTmpWebFile = Filenames.GetWebPaymentUpdatesFilename("dbs");	// internedit file for atc lines to web file format 

			CWebPaymentAtcTransactionFile webFile;								// internal format atc web update file
			if (webFile.Open(strTmpWebFile) == DB_ERR_NONE)					// open intermediate file \ create if not found
			{
				webFile.AddNewAtcUpdates(strWebAtcUpdates);					// convert new atc audits ( if found ) & add to internal web atc format
				webFile.Close();												// then delete file if all ok

				CFileRecycler::SendToRecycleBin(strWebAtcUpdates);

				CWebPaymentPurchaseFile csvPurchases(System.GetWebPaymentType());	// convert web atc file format lines to web export csv file
				csvPurchases.ConvertWebUpdates(strTmpWebFile, strWebCsvFile1, strWebCsvFile2);
				m_strError = csvPurchases.GetError();
			}
		}
	}

	if (m_strError == "")
	{
		int nCount1 = 0;
		int nCount2 = 0;

		if (::FileExists(strWebCsvFile1) == TRUE)
		{
			// diner service
			nCount1 = GetFileLineCount(strWebCsvFile1) - 1;				// count lines in csv file ( excluding header line )
		}

		if (::FileExists(strWebCsvFile2) == TRUE)
		{
			// secondary service
			nCount2 += GetFileLineCount(strWebCsvFile2) - 1;				// count lines in 2nd csv file ( excluding header line )
		}

		m_nPurchaseCount = nCount1 + nCount2;
		if (m_nPurchaseCount > 0)										// see if have any data
		{
			PostTransactions(strWebCsvFile1, strWebCsvFile2);

			if (m_strError == "")											// check for no erros on post purchases
			{
				if (m_bHaveService2 == TRUE)
				{
					strLogComment.Format("%d / %d lines", nCount1, nCount2);		// log number of lines sent in summary log
				}
				else
				{
					strLogComment.Format("%d lines", m_nPurchaseCount);		// log number of lines sent in summary log
				}

				nResultCode = nWEBEXPORT_NOERROR;

				int nRetries = 0;
				while (++nRetries <= 10)
				{
					m_webOptions.LoadRow(NULL);
					m_webOptions.m_Row.SetLastTransactionDone();
					if (m_webOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
					{
						break;
					}
				}
			}
		}
	}

	if (m_strError != "")								// check for any errors reported
	{
		strLogComment = m_strError;						// log post error
	}

	if (strLogComment != "")							// check have something to log
	{
		CWebPaymentExportTxLog log;
		log.SaveLog(nWEBEXPORT_UPDATES, nResultCode, strLogComment);
	}

	CFileRecycler::SendToRecycleBin(strWebCsvFile1);
	CFileRecycler::SendToRecycleBin(strWebCsvFile2);
}

//******************************************************************
// used by:		nWEBPAYMENT_TUCASI (Soap)
//				nWEBPAYMENT_WISEPAY
//				nWEBPAYMENT_SCHOOLCOMMS
//
// not used:	nWEBPAYMENT_PARENTPAY
//				nWEBPAYMENT_TUCASI.v2 (Json)
//				nWEBPAYMENT_SQUID
//				nWEBPAYMENT_PARENTMAIl
//*******************************************************************
// Post Purchase history file to web site
// on entry m_strError may have an error set

void CWebPaymentExport::PostTransactions ( const char* szWebCsvFile1, const char* szWebCsvFile2 )
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText( szINTERNETMESSAGE );

	CString strLogin1Reply = System.GetWebPaymentPath ( "PostTransactionsLoginReply.xml" );
	CString strLogin2Reply = System.GetWebPaymentPath ( "PostTransactions2LoginReply.xml" );

	if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASI )
	{
		CTucasiSoapExport tucasi;
		if (tucasi.SendTransactions(szWebCsvFile1, strLogin1Reply) == FALSE)
		{
			m_strError = tucasi.GetError();
		}
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_WISEPAY )
	{
		CWisePayExport wisepay( m_pParent );
		if ( ::FileExists ( szWebCsvFile1 ) == TRUE )
		{
			if (wisepay.SendTransactions1(szWebCsvFile1, strLogin1Reply) == FALSE)
			{
				m_strError = wisepay.GetError();
			}
		}

		if ( m_strError == "" && ::FileExists ( szWebCsvFile2 ) == TRUE )
		{
			if (wisepay.SendTransactions2(szWebCsvFile2, strLogin2Reply) == FALSE)
			{
				m_strError = wisepay.GetError();
			}
		}
	}
	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS )
	{
		CSchoolcommsExport schoolcomms;
		if (schoolcomms.SendTransactions(szWebCsvFile1, strLogin1Reply) == FALSE)
		{
			m_strError = schoolcomms.GetError();
		}
	}

	if ( m_strError == "" && m_bKeepXmlFiles == FALSE )
	{
		CFileRecycler::SendToRecycleBin ( strLogin1Reply );
		CFileRecycler::SendToRecycleBin ( strLogin2Reply );
	}
}

//*******************************************************************
//*** Send Balances ( or EndOfDay Balances ) ************************
// used by:		nWEBPAYMENT_TUCASI
//				nWEBPAYMENT_SCHOOLCOMMS
//				nWEBPAYMENT_TUCASI.v2 (Json)
//
// not used:	nWEBPAYMENT_PARENTPAY
//				nWEBPAYMENT_WISEPAY
//				nWEBPAYMENT_SQUID
//				nWEBPAYMENT_PARENTMAIl
//*******************************************************************

void CWebPaymentExport::WebPaymentExportBalances(const char* szDbBalanceList)
{
	CString strLogComment = "";
	int nResultCode = nWEBEXPORT_ERROR;						// assume have post error

	CWebPaymentBalanceFile csvBalances{};
	if (csvBalances.Create(szDbBalanceList) == TRUE)			// create ..\{web}\balances.csv
	{
		if ((m_nBalanceCount = csvBalances.GetCount()) > 0)
		{
			CString strWebCsvFilename = csvBalances.GetCSVFilename();

			PostBalances(strWebCsvFilename);

			if (m_strError == "")
			{
				strLogComment.Format("%d lines", m_nBalanceCount);	// summary tx log comment lines
				nResultCode = nWEBEXPORT_NOERROR;

				CWebPaymentExportCsvLog logTz;							// individual balances exported
				logTz.SaveLog("Tz", strWebCsvFilename, m_nBalanceCount);

				int nRetries = 0;
				while (++nRetries <= 10)
				{
					m_webOptions.LoadRow(NULL);
					m_webOptions.m_Row.SetLastBalanceDone();
					if (m_webOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
					{
						break;
					}
				}
			}

			CFileRecycler::SendToRecycleBin(strWebCsvFilename);
		}
	}
	else
	{
		m_strError = csvBalances.GetError();				// error in creating csv balance file
	}

	// log export balance details

	if (m_strError != "")									// check for any errors reported
	{
		strLogComment = m_strError;
	}

	if (strLogComment != "")								// check have something to log
	{
		CWebPaymentExportTxLog log;
		log.SaveLog(nWEBEXPORT_BALANCES, nResultCode, strLogComment);
	}
}

//*******************************************************************
// used by:		nWEBPAYMENT_TUCASI
//				nWEBPAYMENT_WISEPAY
//				nWEBPAYMENT_SCHOOLCOMMS
//				nWEBPAYMENT_TUCASI.v2 (Json)
//
// not used:	nWEBPAYMENT_PARENTPAY
//				nWEBPAYMENT_SQUID
//				nWEBPAYMENT_PARENTMAIl
//*******************************************************************

void CWebPaymentExport::PostBalances ( const char* szWebCsvFile )
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText( szINTERNETMESSAGE );

	CString strLoginReply = System.GetWebPaymentPath ( "PostBalancesLoginReply.xml" );

	if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASI )
	{
		CTucasiSoapExport tucasi;
		if (tucasi.SendBalances(szWebCsvFile, strLoginReply) == FALSE)
		{
			m_strError = tucasi.GetError();
		}
	}

//	else if ( System.GetWebPaymentType() ==  nWEBPAYMENT_WISEPAY )
//	{
//		CWisePayExport wisepay( m_pParent );
//		if ( wisepay.SendBalances ( szWebCsvFile, strLoginReply ) == FALSE )
// 	    {
//			m_strError = wisepay.GetError();
// 	    }
//	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS)
	{
		CSchoolcommsExport schoolcomms;
		if (schoolcomms.SendBalances(szWebCsvFile, strLoginReply) == FALSE)
		{
			m_strError = schoolcomms.GetError();
		}
	}
	else if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASIv2 )
	{
		CTucasiJsonData data;
		data.Read();

		CTucasiJsonExport tucasi ( &data, m_pParent );
		if (tucasi.SendBalances(szWebCsvFile) == FALSE)
		{
			m_strError = tucasi.GetError();
		}
	}

	if (m_strError == "" && m_bKeepXmlFiles == FALSE)
	{
		CFileRecycler::SendToRecycleBin(strLoginReply);
	}
}

//******************************************************************
//******************************************************************
// Special export of Sales \ Topups to csv file ********************
//******************************************************************

void CWebPaymentExport::SpecialSalesExport()
{
	CString strWebAtcUpdates = Filenames.GetWebPaymentUpdatesFilename ( nWEBPAYMENT_EXPORTSALES, "dat" );		// ..\{web}\webupdates.dat
	if ( ::FileExists ( strWebAtcUpdates ) == TRUE )						// see if file exists
	{
		CSalesExportData data; 
		data.Read();
		CString strWebCsvFilename = data.GetInternalExportFilename();
	
		CString strTmpWebFile = Filenames.GetWebPaymentUpdatesFilename ( nWEBPAYMENT_EXPORTSALES, "dbs" );	// internedit file for atc lines to web file format 

		CWebPaymentAtcTransactionFile webFile;								// internal format atc web update file
		if (webFile.Open(strTmpWebFile) == DB_ERR_NONE)					// open intermediate file \ create if not found
		{
			webFile.AddNewAtcUpdates(strWebAtcUpdates);					// convert new atc audits ( if found ) & add to internal web atc format
			webFile.Close();												// then delete file if all ok

			CFileRecycler::SendToRecycleBin(strWebAtcUpdates);

			CWebPaymentPurchaseFile csvPurchases(nWEBPAYMENT_EXPORTSALES);	// convert web atc file format lines to web export csv file
			csvPurchases.ConvertWebUpdates(strTmpWebFile, strWebCsvFilename);
			m_strError = csvPurchases.GetError();

			if (data.m_nCsvExternalDelimiter != ',')						// if not CSV comvert data
			{
				CSSFile fileIn;
				if (fileIn.Open(strWebCsvFilename, "rb") == TRUE)
				{
					CSSFile fileOut;
					if (fileOut.Open(data.GetExternalExportFilename(), "wb") == TRUE)
					{
						CCSV csvIn;
						CCSV csvOut(data.m_nCsvExternalDelimiter, '"', FALSE, FALSE, FALSE);

						while (fileIn.Read(csvIn) == TRUE)
						{
							csvOut.RemoveAll();

							for (int i = 0; i < csvIn.GetSize(); i++)
							{
								csvOut.Add(csvIn.GetString(i));
							}

							fileOut.Write(csvOut);
						}

						fileOut.Close();
					}
					fileIn.Close();
				}
			}

			if (m_strError == "")
			{
				m_nPurchaseCount = csvPurchases.GetExportLineCount();	// new purchase lines added to csv export file
			}
			else
			{
				m_nPurchaseCount = -1;					// set have error nessage
			}
		}
	 }
}

//******************************************************************
