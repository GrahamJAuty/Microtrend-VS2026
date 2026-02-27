//********************************************************************
#include "ReportCSVFile.h"
#include "WebpaymentResendDlg.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayExport.h"
//********************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//************************************************************************
//*** Create ppOut-04 (starting balance) file from a list file ***********
//************************************************************************

bool CParentPayExport::SendOpeningBalanceList(const char* szListFilename)
{
	bool bReply = FALSE;

	CString strUpdatesFile = m_pData->GetStartingBalanceFilename();
	CString strTitle = "Send Opening Balances";

	int nCount = CreateStartingBalancesFile(szListFilename, strUpdatesFile);
	if (nCount > 0)
	{
		CString strMsg = GetListPromptMessage("balance", nCount);

		if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDYES)
		{
			CCSVFileReport report("R217", strUpdatesFile, m_pParent);
			report.SetMode(P_WAIT);
			report.Show(strTitle);
		}

		if (Prompter.YesNo("Do you wish to send the account balances updates to ParentPay now?", strTitle) == IDYES)
		{
			m_pData->Save();												// 09/01/2014 - v1.05c LastPosTxnID will have been incremented = so save new value
			bReply = SendTransactionData(strUpdatesFile, TRUE);		// log required 
		}
	}
	else
	{
		m_strError = "No valid account records found to send balances for!";
	}

	return bReply;
}

//*******************************************************************
//*** Create Diner match ppOut-01 file from list files **************
//*******************************************************************

bool CParentPayExport::SendDinerMatchList(const char* szListFilename)
{
	bool bReply = FALSE;

	CString strUpdatesFile = m_pData->GetDinerMatchAdviceFilename();
	CString strTitle = "Send Diner Match Data";

	int nCount = CreateDinerMatchFile(szListFilename, strUpdatesFile);
	if (nCount > 0)
	{
		CString strMsg = GetListPromptMessage("Diner Match", nCount);

		if (Prompter.YesNo(strMsg, strTitle) == IDYES)
		{
			CCSVFileReport report( "R219", strUpdatesFile, m_pParent);
			report.SetMode(P_WAIT);
			report.Show(strTitle);
		}

		if (Prompter.YesNo("Do you wish to send the Diner Match updates to ParentPay now?", strTitle) == IDYES)
		{
			m_pData->Save();												// //09/01/2015 - v1.05c LastPosTxnID will have been incremented = so save new value
			bReply = SendDinerMatch(strUpdatesFile);					// will add to echo log
		}
	}
	else
	{
		m_strError = "No valid account records found to Diner Match on!";
	}

	return bReply;
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

void CParentPayExport::ResendDinerMatch ( const char* szCsvWebFilename )
{
	CString strNewFile	= System.GetWebPaymentPath( ::GetUniqueTempFilename("$14") );

	// connectingPOSTxnID,parentPayAccountID,posInternalAccountID,externalAccountReference,openingBalance
	CString strDisplayHeader = "posInternalAccountID,parentPayAccountID,externalAccountReference,openingBalance";				

	CWebpaymentResendDlg dlg ( "Diner Match", szCsvWebFilename, strNewFile, strDisplayHeader, m_pParent );
	dlg.SetConnectingPOSTxnID ( 0, m_pData->GetNextPosTxnID ( FALSE ) );
	dlg.SetSortField ( "posInternalAccountID", DB_SORT_INT );

	if ( dlg.DoModal() == IDOK )										// at lesat 1 line selected
	{	
		if ( dlg.GetSendConfirmation() == TRUE )
		{
			m_pData->SetNextPosTxnID ( dlg.GetNextConnectingPOSTxnID() );	// save new NextPosTxnID
			m_pData->Save();

			SendDinerMatch ( strNewFile );								// will save log before re-sending diner match
		}
	}
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

void CParentPayExport::ResendPaymentEcho ( const char* szCsvWebFilename )
{
	CString strNewFile	= System.GetWebPaymentPath( ::GetUniqueTempFilename("$15") );

	//connectingPOSTxnID,mealSessionID,txnAmount,description,accountID,txnDateTime,txnType,originatingPOSID,flags,newBalance,posInternalTxnID,parentPayPaymentID
	CString strDisplayHeader = "parentPayPaymentID,accountID,description,txnAmount,newBalance";				

	CWebpaymentResendDlg dlg ( "Payment Echoes", szCsvWebFilename, strNewFile, strDisplayHeader, m_pParent );
	dlg.SetConnectingPOSTxnID ( 0, m_pData->GetNextPosTxnID ( FALSE ) );
	dlg.SetSortField ( "parentPayPaymentID", DB_SORT_INT);

	if ( dlg.DoModal() == IDOK )												// at lesat 1 line selected
	{
		if ( dlg.GetSendConfirmation() == TRUE )
		{
			m_pData->SetNextPosTxnID ( dlg.GetNextConnectingPOSTxnID() );			// save new NextPosTxnID
			m_pData->Save();

			SendPaymentEcho ( strNewFile );								// will save log before resending
		}
	}
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

void CParentPayExport::ResendTransactions ( const char* szCsvWebFilename )
{
	CString strNewFile	= System.GetWebPaymentPath( ::GetUniqueTempFilename("$16", "TX" ) );

	//connectingPOSTxnID,mealSessionID,txnAmount,description,accountID,txnDateTime,txnType,originatingPOSID,flags,newBalance,posInternalTxnID,financialTransactionType,vatRate
	CString strDisplayHeader = "txnDateTime,accountID,description,txnAmount,newBalance";				

	CWebpaymentResendDlg dlg ( "Tranasctions", szCsvWebFilename, strNewFile, strDisplayHeader, m_pParent );
	dlg.SetConnectingPOSTxnID ( 0, m_pData->GetNextPosTxnID ( FALSE ) );

	if ( dlg.DoModal() == IDOK )								// at lesat 1 line selected
	{	
		if ( dlg.GetSendConfirmation() == TRUE )
		{
			m_pData->SetNextPosTxnID ( dlg.GetNextConnectingPOSTxnID() );	// save new NextTxID
			m_pData->Save();

			SendTransactionData ( strNewFile, TRUE );			// will save log before re-sending financial update data
		}
	}
}

//*******************************************************************
