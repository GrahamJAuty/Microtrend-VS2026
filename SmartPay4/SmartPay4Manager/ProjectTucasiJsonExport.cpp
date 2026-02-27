//*******************************************************************
#include "WebpaymentResendDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\TucasiJSONExport.h"
//*******************************************************************

//*******************************************************************
// szCsvWebFilename = current log file with redundant lines removed
//header = transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,balance,accountID,accountName
//*******************************************************************

void CTucasiJsonExport::ResendTransactions ( const char* szCsvWebFilename )
{
	CString strNewFile = System.GetWebPaymentPath( ::GetUniqueTempFilename("$16"),nWEBPAYMENT_TUCASIv2 );

	CWebpaymentResendDlg dlg ( "Transactions", szCsvWebFilename, strNewFile, m_pData->m_strUploadPurchaseHeader, m_pParent );
	if ( dlg.DoModal() == IDOK )								// at lesat 1 line selected
	{	
		if ( dlg.GetSendConfirmation() == TRUE )
			SendTransactions ( strNewFile );					// re-send financial data
	}
}

//**********************************************************************
