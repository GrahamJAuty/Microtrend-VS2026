/**********************************************************************/
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSInvoiceNum.h"
/**********************************************************************/

int CPMSInvoiceNum::GetInvoiceNum()
{
	CSSFile fileInvoiceNum;
	CFilenameUpdater FnUp ( SysFiles::PMSInvoiceNumber );
	if ( fileInvoiceNum.Open ( FnUp.GetFilenameToUse(), "rb" ) == FALSE )
		return 1;

	CString strBuffer;
	fileInvoiceNum.ReadString ( strBuffer );
	
	int nInvoiceNum = atoi ( strBuffer );
	
	if ( nInvoiceNum < 0 ) nInvoiceNum = 0;
	if ( nInvoiceNum > 99999998 ) nInvoiceNum = 99999998;

	return nInvoiceNum + 1;
}

/**********************************************************************/

void CPMSInvoiceNum::NextInvoiceNum()
{
	int nNextInvoice = 1;

	CFilenameUpdater FnUp( SysFiles::PMSInvoiceNumber );

	CSSFile fileInvoiceNum;
	if ( fileInvoiceNum.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		fileInvoiceNum.ReadString ( strBuffer );
		nNextInvoice = atoi ( strBuffer ) + 1;
		fileInvoiceNum.Close();
	}

	if ( nNextInvoice == 99999999 || nNextInvoice < 0 ) nNextInvoice = 0;

	if ( fileInvoiceNum.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CString strLine;
		strLine.Format ( "%d", nNextInvoice );
		fileInvoiceNum.WriteLine ( strLine );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
