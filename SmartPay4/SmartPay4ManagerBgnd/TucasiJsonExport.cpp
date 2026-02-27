//*******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "TucasiJSON.h"
//*******************************************************************
#include "WebPaymentPurchaseFile.h"
//*******************************************************************
#include "TucasiJSONExport.h"
//*******************************************************************

extern const char* szINTERNETMESSAGE;							// "Accessing WebPayment internet site"

//*******************************************************************

CTucasiJsonExport::CTucasiJsonExport ( CTucasiJsonData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	m_strError = "";
}

//**********************************************************************
// csvfile in tucasi balance file format ( with header line )

bool CTucasiJsonExport::SendBalances ( const char* szCsvFilename )
{
	CString strTmpFile = System.GetWebPaymentPath( ::GetUniqueTempFilename("$11"), nWEBPAYMENT_TUCASIv2 );
	::CopyFile ( szCsvFilename, strTmpFile, FALSE );				// make copy as upload function will clear the file as it sends it

	SendTransactions ( strTmpFile );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//*******************************************************************
// Send all audit updates that have topped up purse values + any outstanding updates still lurking
// if reply = FALSE = m_strError is set

bool CTucasiJsonExport::SendUpdates()
{
	CString strAtcUpdates = Filenames.GetWebPaymentUpdatesFilename ( "dat" );	// webupdate.dat 
	CString strWebUpdates = Filenames.GetWebPaymentUpdatesFilename ( "dbs" );	// webupdates.dbs 

	CWebPaymentAtcTransactionFile webFile;									// internal format atc web update file
		
	if ( webFile.Open( strWebUpdates ) == DB_ERR_NONE )						// open intermediate file \ create if not found
	{
		bool bReply = webFile.AddNewAtcUpdates ( strAtcUpdates );			// convert new atc audits ( if found ) & add to internal web atc format
		webFile.Close();													// then delete file if all ok

		if ( bReply == TRUE )
			CFileRecycler::SendToRecycleBin ( strAtcUpdates);
	}

	CString strCsvFilename = m_pData->GetNewUpdatesFilename();				// tv2new.dat

	CWebPaymentPurchaseFile csvPurchases ( nWEBPAYMENT_TUCASIv2 );			// export web updates
	csvPurchases.ConvertWebUpdates ( strWebUpdates, strCsvFilename );

	if ( ( m_strError = csvPurchases.GetError() ) == "" )
		SendTransactions ( strCsvFilename );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//**********************************************************************
// csvfile in tucasi transaction file format ( with header line )

void CTucasiJsonExport::SendTransactions ( const char* szCsvFilename )
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText( szINTERNETMESSAGE );

	CString strResponseFile	= System.GetWebPaymentPath ( "TxResponse.json" );					// reply to send updates
	
	while ( TRUE )
	{
		if ( ::GetFileLineCount ( szCsvFilename ) > 1 )				// number of transaction lines to send excluding header
		{
			CString strTmpFile = System.GetWebPaymentPath( ::GetUniqueTempFilename("$12"), nWEBPAYMENT_TUCASIv2);
			int nTxCount = ::CopyFileLines ( szCsvFilename, strTmpFile, m_pData->m_nMaxTransactionLines, TRUE );	// make up a file of maximum number of lines or less

			CFileRecycler::SendToRecycleBin ( strResponseFile );					//02/02/2018 clear response file if found

			CSSTucasiJson tucasi ( m_pData );
			m_strError = tucasi.SendUpdates ( strTmpFile, strResponseFile );

			CFileRecycler::SendToRecycleBin ( strTmpFile );

			if ( m_strError != "" )
				break;

			tucasi.LogResponse ( strResponseFile );

			::RemoveFileLines ( szCsvFilename, nTxCount, TRUE );
		}

		if ( ::GetFileLineCount ( szCsvFilename ) < 2 )
		{
			CFileRecycler::SendToRecycleBin ( szCsvFilename );
			break;
		}
	}
}

//*******************************************************************
