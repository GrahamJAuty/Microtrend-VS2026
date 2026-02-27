//***********************************************************************
// WebPayment import payments
//***********************************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
#include "..\SmartPay4Shared\MessageLogger.h"
//***********************************************************************
#include "BromComImport.h"
#include "ParentMailExport.h"
#include "ParentMailImport.h"
#include "ParentPayExport.h"
#include "ParentPayImport.h"
#include "PebbleImport.h"
#include "SchoolcommsImport.h"
#include "SquidExport.h"
#include "SquidImport.h"
#include "TucasiJSONExport.h"
#include "TucasiJSONImport.h"
#include "TucasiSOAPImport.h"
#include "WebPaymentDefines.h"
#include "WebPaymentExport.h"
#include "WebPaymentExportBromCom.h"
#include "WebPaymentExportPebble.h"
#include "WisePayImport.h"
//***********************************************************************
#include "WebPaymentImport.h"
//***********************************************************************

CWebPaymentImport::CWebPaymentImport( CWnd* pParent )
{
	m_pParent = pParent;
	m_nPaymentCount = 0;
	bExcludedByTime = FALSE;		
	m_webOptions.LoadRow(NULL);
}

//*******************************************************************

bool CWebPaymentImport::ImportPayments ( bool bNewPayments, bool& bTriedSendUpdates )
{
	bTriedSendUpdates = FALSE;

	CString strTxError = "";
	CString strRxError = "";
	bool bUpdatesSent = FALSE;
	bool bEODSent = FALSE;

	CString strCaption = "";

	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		strCaption.Format("%s Web Payment Import",
			(const char*)System.GetWebPaymentDisplayName());
	}
	else
	{
		strCaption = "Synchronise Pebble Transactions";
	}

// see if looking for new payments

	if ( bNewPayments == TRUE )									// see if looking for new payments
	{
		if (m_webOptions.m_Row.TimeForImports() == FALSE)			// see if time is ok for import of payments
		{
			bool bImportReqd = FALSE;
			
			if (CModuleGlobals::IsBackgroundModule() == FALSE)
			{
				CString strMsg;
				strMsg.Format("%s\n\nDo you still wish to continue?",
					(const char*) m_webOptions.m_Row.GetError());

				if (Prompter.YesNo(strMsg, strCaption) == IDYES)
				{
					bImportReqd = TRUE;
				}
			}
			
			if (bImportReqd == FALSE)
			{
				bExcludedByTime = TRUE;		
				return FALSE;
			}
		}

		if ((m_webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE) && (CModuleGlobals::IsBackgroundModule() == FALSE))
		{
			CString strPrompt = "Please confirm - ";

			if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
			{
				strPrompt += "Check for new payments now?";
			}
			else
			{
				strPrompt += "Synchronise transactions now?";
			}

			if (Prompter.YesNo(strPrompt, strCaption, TRUE) == IDNO)
			{
				return FALSE;
			}
		}

		if ( ::FileExists ( Filenames.GetWebPaymentStopIndicator() ) == TRUE )
		{
			CFileRecycler::SendToRecycleBin ( Filenames.GetWebPaymentStopIndicator() );						// delete background processor web payment suspended indicator file
			::LogError ( "Internet suspension cleared on import", Filenames.GetWebPaymentRxErrorLog() );	// "RxError.log"
		}
	}

// see if import payments required now to relevant web payment provider

	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PARENTPAY:
		ImportParentPay(strRxError, strTxError, bUpdatesSent, bNewPayments);
		break;

	case nWEBPAYMENT_TUCASI:
		ImportTucasi(strRxError, bNewPayments);
		break;

	case nWEBPAYMENT_TUCASIv2:
		ImportTucasiV2(strRxError, strTxError, bUpdatesSent, bNewPayments);
		break;

	case nWEBPAYMENT_WISEPAY:
		ImportWisePay(strRxError, bNewPayments);
		break;

	case nWEBPAYMENT_SCHOOLCOMMS:
		ImportSchoolComms(strRxError, bNewPayments);
		break;

	case nWEBPAYMENT_SQUID:
		ImportSquid(strRxError, strTxError, bUpdatesSent, bNewPayments);
		break;

	case nWEBPAYMENT_PARENTMAIL:
		ImportParentMail(strRxError, strTxError, bUpdatesSent, bEODSent, bNewPayments);
		break;

	case nWEBPAYMENT_BROMCOM:
		ImportBromCom(strRxError, bNewPayments);
		break;

	case nWEBPAYMENT_PEBBLE:
		ImportPebble(strRxError, bNewPayments);
		break;
	}

	if ( bUpdatesSent == TRUE )											// see if any updates sent before doing balances	
	{
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

//check for any errors ************************************************************

	CString strError = "";
		 
	if (strRxError != "")
	{
		strError = "RX Error: " + strRxError;
	}
	else if (strTxError != "")
	{
		strError = "TX Error: " + strTxError;
	}

	if ( strError != "" )														// check for any errors
	{
		::LogError ( strError, Filenames.GetWebPaymentRxErrorLog() );			// "RxError.log"

		if ( CModuleGlobals::IsBackgroundModule() == FALSE )
		{
			Prompter.Error( strError, strCaption );						// show any error if not in background mode
		}
		else
		{
			CBackgroundOptions options;											// get background options
			options.Read();		
		
			if ( options.m_bWebPaymentSuspendOnError == TRUE )
			{
				::LogError ( "Internet access suspended", Filenames.GetWebPaymentRxErrorLog() );	// "RxError.log"
				::LogError ( strError, Filenames.GetWebPaymentStopIndicator() );					// log error & stop any further backgroung web payments
			}
		}
		return FALSE;
	}

// to get here no errors detected *************************************************
	
	if ( bNewPayments == TRUE )										// see if import new payments (not re-processing exceptons )	
	{
		int nRetries = 0;
		while (++nRetries <= 10)
		{
			m_webOptions.LoadRow(NULL);
			m_webOptions.m_Row.SetLastPaymentDone();

			if (bEODSent == TRUE)										// set EOD done
			{
				m_webOptions.m_Row.SetLastBalanceDone();
			}

			if (m_webOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
			{
				break;
			}
		}

		// send any outstanding updates\ web topup & EOD balances if time is right

		// send any updates
		switch (System.GetWebPaymentType())
		{
		case nWEBPAYMENT_BROMCOM:
		{
			CWebPaymentExportBromCom web(m_pParent);
			web.SendAccountUpdates(FALSE);
			bTriedSendUpdates = TRUE;
		}
		break;

		case nWEBPAYMENT_PEBBLE:
		{
			CWebPaymentExportPebble web(m_pParent);
			web.SendAccountUpdates();
			bTriedSendUpdates = TRUE;
		}
		break;

		default:
		{
			CWebPaymentExport web(m_pParent);
			web.SendAccountUpdates(FALSE);		
			bTriedSendUpdates = TRUE;
		}
		break;
		}
	}
	
	return TRUE;
}

//******************************************************************

void CWebPaymentImport::ImportParentPay(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bNewPayments)
{
	CParentPayData data;												// need to define here as both update & import payments update parentpay data
	data.Read();														// Read System data
	if (data.IsSet() == TRUE)
	{
		CParentPayExport tx(&data, m_pParent);
		if ((bUpdatesSent = tx.SendAnyUpdates()) == FALSE)			// send retries (if any) then any transactions before looking for new payaments
		{
			strTxError = tx.GetError();
		}

		if (strTxError == "")
		{
			CParentPayImport rx(&data, m_pParent);
			if (rx.ImportPayments(bNewPayments) == FALSE)
			{
				strRxError = rx.GetError();		// will also export payment echos
			}
			else
			{
				m_nPaymentCount = rx.GetPaymentCount();
			}
		}
	}
	else
	{
		strTxError = szINTERFACENOTSET;
	}
}

//******************************************************************

void CWebPaymentImport::ImportTucasi(CString& strRxError, bool& bNewPayments)
{
	CTucasiSoapImport rx(m_pParent);
	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetError();
	}
	else
	{
		m_nPaymentCount = rx.GetPaymentCount();
	}
}

//******************************************************************

void CWebPaymentImport::ImportTucasiV2(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bNewPayments)
{
	CTucasiJsonData data;
	data.Read();

	CTucasiJsonExport tx(&data, m_pParent);
	if ((bUpdatesSent = tx.SendUpdates()) == FALSE)				// send any transaction before looking for new payaments
	{
		strTxError = tx.GetError();
	}

	if (strTxError == "")
	{
		CTucasiJsonImport rx(m_pParent);

		if (rx.Import(bNewPayments) == FALSE)
		{
			strRxError = rx.GetError();
		}
		else
		{
			m_nPaymentCount = rx.GetPaymentCount();
		}
	}
}

//******************************************************************

void CWebPaymentImport::ImportWisePay(CString& strRxError, bool& bNewPayments)
{
	CWisePayImport rx(m_pParent);
	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetError();
	}
	else
	{
		m_nPaymentCount = rx.GetPaymentCount();
	}
}

//******************************************************************

void CWebPaymentImport::ImportSchoolComms(CString& strRxError, bool& bNewPayments)
{
	CSchoolcommsImport rx(m_pParent);

	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetError();
	}
	else
	{
		m_nPaymentCount = rx.GetPaymentCount();
	}
}

//******************************************************************

void CWebPaymentImport::ImportSquid(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bNewPayments)
{
	CSquidExport tx(m_pParent);

	if ((bUpdatesSent = tx.SendUpdates()) == FALSE)				// send any lurking transactions
	{
		strTxError = tx.GetError();
	}

	if (strTxError == "")
	{
		CSquidImport rx(m_pParent);
		if (rx.Import(bNewPayments) == FALSE)
		{
			strRxError = rx.GetError();
		}

		if (strRxError == "")												// NB import reply could be FALSE but with no error
		{
			m_nPaymentCount = rx.GetPaymentCount();

			if (tx.SendUpdates() == TRUE)
			{
				bUpdatesSent = TRUE;		// send rplu transaction back to squid
			}
			else
			{
				strTxError = tx.GetError();
			}
		}
	}
}

//******************************************************************

void CWebPaymentImport::ImportParentMail(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bEODSent, bool& bNewPayments)
{
	CParentMailImport rx(m_pParent);

	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetError();
	}
	else
	{
		m_nPaymentCount = rx.GetPaymentCount();
	}

	if (strRxError == "")												// NB import reply could be FALSE but with no error
	{
		CParentMailExport tx(m_pParent);
		if (tx.SendUpdates(TRUE, TRUE) == TRUE)
		{
			bUpdatesSent = TRUE;										// post data & EOD if time right
			bEODSent = tx.GetEODSent();									// see if EOD sent transaction sent
		}
		else
		{
			strTxError = tx.GetError();
		}
	}
}

//******************************************************************

void CWebPaymentImport::ImportBromCom(CString& strRxError, bool& bNewPayments)
{
	CBromComImport rx(m_pParent);
	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetBromComError();
	}
	else
	{
		m_nPaymentCount = rx.GetValidPaymentCount();
	}
}

//******************************************************************

void CWebPaymentImport::ImportPebble(CString& strRxError, bool& bNewPayments)
{
	CPebbleImport rx(m_pParent);
	if (rx.Import(bNewPayments) == FALSE)
	{
		strRxError = rx.GetPebbleError();
	}
	else
	{
		m_nPaymentCount = rx.GetValidPaymentCount();
	}
}

//******************************************************************

