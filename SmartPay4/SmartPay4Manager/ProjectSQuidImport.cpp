//*******************************************************************
#include "..\SmartPay4ManagerBgnd\Squid.hpp"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//*******************************************************************
#include "WebPaymentStatsDlg.h"
#include "WebPaymentReporter.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SquidImport.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CSquidImport::DisplayImportResults( int nRecordCount )
{
	if ( m_nValidCount == 0 )								// check for any valid imports
	{
		CString strMsg;
		if ( m_bNewPayments == TRUE )	strMsg = szMSG_NONEWPAYMENTS;		// "No new payments have been received.";
		else							strMsg = szMSG_NOREPROCESSED;		// "No outstanding payments have been re-processed.";

		if ( m_nExceptionCount == 0 )										// no exceptions
		{
			DisplayMessage ( strMsg, MB_ICONINFORMATION, IDOK );
		}
		else
		{
			strMsg += szMSG_HAVEEXCEPTIONS;									// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions (y/n)";

			if ( DisplayMessage ( strMsg, MB_YESNO, IDYES ) == IDYES )
			{
				CWebPaymentReporter report ( "R203", nWEBPAYMENT_SQUID, m_strTempExceptionFile, m_pParent );
				report.Show ( P_WAIT );
			}
		}
	}
	else			//  have some ValidCount
	{
		CWebPaymentStatsDlg dlg ( nWEBPAYMENT_SQUID, m_pParent );
		dlg.m_nRead					= nRecordCount;
		dlg.m_nValid				= m_nValidCount;
		dlg.m_nExceptions			= m_nExceptionCount;
		dlg.m_strImportFilename		= m_strTempLogFile;			// file showing valid import lines
		dlg.m_strExceptionFilename	= m_strTempExceptionFile;	// file showing import exception lines
		dlg.DoModal();
		
// sort out WebPayment list file & update main pending files if required

		CString strFile;	strFile.Format ( "%s.dat", m_strLogFileName );		// master import log file
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//*******************************************************************

int CSquidImport::DisplayMessage ( CString strMsg, UINT nType, int nBackgroundResult  )
{
	return Prompter.DisplayMessage( strMsg, m_strTitle, nType );
}

//******************************************************************
