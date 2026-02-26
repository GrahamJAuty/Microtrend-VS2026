/**********************************************************************/
#include "ProcessedOrderDetails.h"
/**********************************************************************/

CProcessedOrderDetails::CProcessedOrderDetails( bool bEmailMode )
{
	m_bEmailMode = bEmailMode;

	m_strTopName = "";
	m_strTopAddress1 = "";
	m_strTopAddress2 = "";
	m_strTopAddress3 = "";
	m_strTopPostcode = "";
	m_strTopPhone = "";
	m_strTopFax = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strPostcode = "";
	m_strPhone = "";
	m_strFax = "";
	m_strEmail = "";
	m_strName = "";
	m_strTopEmail = "";
	m_bDeliverAsOrdered = TRUE;
	
	ReadDeliveryInstructions();
}

/**********************************************************************/

void CProcessedOrderDetails::ReadDeliveryInstructions()
{
	CStringArray strInstructions;

	for ( int nIndex = 0; nIndex < 5; nIndex++ )
		strInstructions.Add ( "" );

	CString strFilename;
	
	if ( m_bEmailMode == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::OrderInstructions );
		strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::EmailInstructions );
		strFilename = FnUp.GetFilenameToUse();
	}
	
	CSSFile fileInstructions;
	if ( fileInstructions.Open ( strFilename, "rb" ) == TRUE )
	{
		CString strLine = "";
		int nCount = 0;
	
		while ( ( fileInstructions.ReadString ( strLine ) == TRUE ) && ( nCount < 5 ) )
		{
			strInstructions.SetAt ( nCount, strLine );
			nCount++;
		}
	}
	
	m_strInstructions1 = strInstructions.GetAt(0);
	m_strInstructions2 = strInstructions.GetAt(1);
	m_strInstructions3 = strInstructions.GetAt(2);
	m_strInstructions4 = strInstructions.GetAt(3);
	m_strInstructions5 = strInstructions.GetAt(4);
}

/**********************************************************************/

void CProcessedOrderDetails::WriteDeliveryInstructions()
{
	CString strFilename;
	
	if ( m_bEmailMode == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::OrderInstructions );
		strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::EmailInstructions );
		strFilename = FnUp.GetFilenameToUse();
	}
	
	CSSFile fileInstructions;
	if ( fileInstructions.Open ( strFilename, "wb" ) == TRUE )
	{
		fileInstructions.WriteLine ( m_strInstructions1 );
		fileInstructions.WriteLine ( m_strInstructions2 );
		fileInstructions.WriteLine ( m_strInstructions3 );
		fileInstructions.WriteLine ( m_strInstructions4 );
		fileInstructions.WriteLine ( m_strInstructions5 );
	}
}

/**********************************************************************/
