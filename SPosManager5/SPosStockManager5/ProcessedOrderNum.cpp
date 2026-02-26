/**********************************************************************/
#include "ProcessedOrderNum.h"
/**********************************************************************/

int CProcessedOrderNum::GetOrderNum()
{
	CSSFile fileOrderNum;
	CFilenameUpdater FnUp ( SysFiles::OrderNum );
	if ( fileOrderNum.Open ( FnUp.GetFilenameToUse(), "rb" ) == FALSE )
		return 1;

	CString strBuffer;
	fileOrderNum.ReadString ( strBuffer );
	
	int nOrderNum = atoi ( strBuffer );
	
	if ( nOrderNum < 0 ) nOrderNum = 0;
	if ( nOrderNum > 999998 ) nOrderNum = 999998;

	return nOrderNum + 1;
}

/**********************************************************************/

void CProcessedOrderNum::NextOrder()
{
	int nNextOrder = 1;

	CFilenameUpdater FnUp( SysFiles::OrderNum );

	CSSFile fileOrderNum;
	if ( fileOrderNum.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		fileOrderNum.ReadString ( strBuffer );
		nNextOrder = atoi ( strBuffer ) + 1;
		fileOrderNum.Close();
	}

	if ( nNextOrder == 999999 || nNextOrder < 0 ) nNextOrder = 0;

	if ( fileOrderNum.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CString strLine;
		strLine.Format ( "%d", nNextOrder );
		fileOrderNum.WriteLine ( strLine );
	}
}

/**********************************************************************/
