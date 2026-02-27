//********************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//********************************************************************
#include "WebPaymentStatsDlg.h"
#include "WebPaymentReporter.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayImport.h"
//********************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************

void CParentPayImport::DisplayMessage ( CString strMsg, UINT nType )
{
	Prompter.DisplayMessage( strMsg, m_strTitle, nType );
}

//******************************************************************

void CParentPayImport::DisplayImportResults( int nServiceNo, int nRecordCount )
{
	if ( m_nValidCount == 0 )									// check for any valid imports
	{
		CString strMsg = NoNewPaymentMessage ( nServiceNo );	// No new payments ... or  No outstanding payments have been re-processed.

		if ( m_nExceptionCount == 0 )							// no exceptions
			DisplayMessage ( strMsg, MB_ICONINFORMATION );
		else
		{
			strMsg += szMSG_HAVEEXCEPTIONS;						// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions (y/n)";

			if ( Prompter.YesNo( strMsg, m_strTitle ) == IDYES )
			{
				CWebPaymentReporter report ( "R203", nWEBPAYMENT_PARENTPAY, m_strTempExceptionFile, m_pParent );
				report.Show ( P_WAIT );
			}
		}
	}
	else			// lines have been imported
	{
		CWebPaymentStatsDlg dlg ( nWEBPAYMENT_PARENTPAY, m_pParent );
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

