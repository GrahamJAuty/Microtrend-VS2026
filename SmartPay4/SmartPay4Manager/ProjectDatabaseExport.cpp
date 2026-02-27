//**********************************************************************
// Export database records
//**********************************************************************
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
#include "..\SmartPay4ManagerBgnd\DatabaseExport.h"
/**********************************************************************/

void CExportDatabase::Execute ( bool bShowProgress )
{
	int nErrorNo = CreateFile ( bShowProgress );			// create report & get report title

	CRepmanHandler repman ( m_strTitle, m_pParent );
	
	if (TRUE == bShowProgress)
	{
		repman.DisplayError(nErrorNo);

		if (nErrorNo == nREPORT_NOERROR)
		{
			CString strMsg;
			strMsg.Format("Number of lines exported to '%s' = %d", m_strExportFilename, m_nExportCount);
			repman.DisplayMessage(strMsg, MB_ICONINFORMATION);
		}
	}
}

//*******************************************************************

void CExportDatabase::ShowUserExportStats ( CWnd* pParent )
{
	CString strBuf;
	strBuf.Format ( "Number of lines exported to '%s' = %d", m_strExportFilename, m_nExportCount );
	Prompter.Info( strBuf, m_strTitle );
}

//**********************************************************************
