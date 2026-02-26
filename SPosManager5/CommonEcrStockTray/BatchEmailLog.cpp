/**********************************************************************/
#include "BatchEmailLog.h"
/**********************************************************************/

CBatchEmailLog::CBatchEmailLog()
{
	m_nLineCount = 0;
	m_bIsOpen = FALSE;
}

/**********************************************************************/

bool CBatchEmailLog::Open( int nType )
{
	if ( m_bIsOpen == TRUE )
		return FALSE;

	switch ( nType )
	{
	case BATCH_EMAIL_ORDER_SHEET:
		m_ReportFile.Open ( Super.EmailLog() );
		m_ReportFile.SetStyle1 ( "Order Processing Email Error Log" );
		m_ReportFile.AddColumn ( "Supplier", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Order Num", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Email", TA_LEFT, 500 );
		m_ReportFile.AddColumn ( "Problem", TA_LEFT, 700 );
		m_bIsOpen = TRUE;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CBatchEmailLog::Close()
{
	if ( m_bIsOpen == FALSE )
		return FALSE;

	m_ReportFile.Close();
	m_bIsOpen = FALSE;
	return TRUE;
}

/**********************************************************************/

CBatchEmailLog::~CBatchEmailLog()
{
	Close();
}

/**********************************************************************/

bool CBatchEmailLog::AddLine ( const char* szLine )
{
	if ( m_bIsOpen == FALSE )
		return FALSE;

	m_ReportFile.WriteLine ( szLine );
	m_nLineCount++;
	return TRUE;
}

/**********************************************************************/
