//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\TucasiSOAP.hpp"
#include "..\SmartPay4ManagerBgnd\TucasiSOAPImport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//*******************************************************************
#include "WebPaymentStatsDlg.h"
#include "WebPaymentReporter.h"
//*******************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//*******************************************************************

void CTucasiSoapImport::DisplayImportResults( int nRecordCount )
{
	if ( m_nValidCount == 0 )								// check for any valid imports
	{
		CString strMsg;
		if ( m_bNewPayments == TRUE )	
		{
			strMsg = szMSG_NONEWPAYMENTS;			// "No new web payments have been received.";
		}
		else							
		{
			strMsg = szMSG_NOREPROCESSED;			//  "No outstanding web payments have been re-processed.";
		}

		if ( m_nExceptionCount == 0 )						// no exceptions
		{
			DisplayMessage ( strMsg, MB_ICONINFORMATION, IDOK );
		}
		else
		{
			strMsg += szMSG_HAVEEXCEPTIONS;										// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions (y/n)";

			if ( DisplayMessage ( strMsg, MB_YESNO, IDYES ) == IDYES )
			{
				CWebPaymentReporter report ( "R203", nWEBPAYMENT_TUCASI, m_strTempExceptionFile, m_pParent );
				report.Show ( P_WAIT );
			}
		}
	}
	else			// lines have been imported
	{
		CWebPaymentStatsDlg dlg ( nWEBPAYMENT_TUCASI, m_pParent );
		dlg.m_nRead					= nRecordCount;
		dlg.m_nValid				= m_nValidCount;
		dlg.m_nExceptions			= m_nExceptionCount;
		dlg.m_strImportFilename		= m_strTempLogFile;			// file showing valid import lines
		dlg.m_strExceptionFilename	= m_strTempExceptionFile;	// file showing import exception lines
		dlg.DoModal();
		
		CString strFile;										// master import log file
		strFile.Format ( "%s.dat", m_strLogFileName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempLogFile, strFile );
	}
}

//*******************************************************************

int CTucasiSoapImport::DisplayMessage ( CString strMsg, UINT nType, int nBackgroundReply  )
{
	return Prompter.DisplayMessage( strMsg, m_strTitle, nType );
}

//******************************************************************
