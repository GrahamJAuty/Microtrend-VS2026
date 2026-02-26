/**********************************************************************/
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderLog.h"
/**********************************************************************/

CProcessedOrderLog::CProcessedOrderLog()
{
	m_nLineCount = 0;
	m_bIsOpen = FALSE;
}

/**********************************************************************/

bool CProcessedOrderLog::Open()
{
	if ( m_bIsOpen == TRUE )
		return FALSE;

	m_ReportFile.Open ( Super.OrderLog() );
	m_ReportFile.SetStyle1 ( "Order Generation Error Log" );
	m_ReportFile.AddColumn ( "Stockpoint", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Problem", TA_LEFT, 700 );
	m_bIsOpen = TRUE;
	return TRUE;
}

/**********************************************************************/

bool CProcessedOrderLog::Close()
{
	if ( m_bIsOpen == FALSE )
		return FALSE;

	m_ReportFile.Close();
	m_bIsOpen = FALSE;
	return TRUE;
}

/**********************************************************************/

CProcessedOrderLog::~CProcessedOrderLog()
{
	Close();
}

/**********************************************************************/

bool CProcessedOrderLog::AddLine ( int nSpIdx, int nProblem )
{
	if ( m_bIsOpen == FALSE )
		return FALSE;

	CCSV csv( '\t' );
	csv.Add( dbStockpoint.GetName( nSpIdx ) );

	switch( nProblem )
	{
	case 1:		csv.Add( "Unable to open order file" );		break;
	case 2:		csv.Add( "Unable to write order file" );	break;
	default:	csv.Add( "Unknown problem" );				break;
	}

	m_ReportFile.WriteLine ( csv.GetLine() );
	m_ReportFile.WriteLine ( "<LI>" );

	m_nLineCount++;
	return TRUE;
}

/**********************************************************************/
