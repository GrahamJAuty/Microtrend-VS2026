/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "CCSVReportLine.h"
/**********************************************************************/

void CCSVReportLine::AppendLineQty ( double dQty, int nDps )
{
	if ( nDps < 0 )
		Add ( dQty, SysInfo.GetDPQty(), FALSE );
	else
		Add ( dQty, nDps, FALSE );
}

/**********************************************************************/

void CCSVReportLine::AppendVal ( double dVal, int nDps )
{
	if ( nDps < 0 )
		Add ( dVal, SysInfo.GetDPValue(), FALSE );
	else
		Add ( dVal, nDps, FALSE );
}

/**********************************************************************/

void CCSVReportLine::AppendString ( const char* szText )
{
	Add ( szText );
}

/**********************************************************************/

void CCSVReportLine::AppendYesOrBlank( bool bYes )
{
	Add ( ( bYes == TRUE ) ? "Yes" : "" );
}

/**********************************************************************/

void CCSVReportLine::AppendInt ( int nVal )
{
	Add ( nVal );
}

/**********************************************************************/

void CCSVReportLine::AppendBlanks ( int nCount )
{
	while ( nCount > 0 )
	{
		Add ( "" );
		nCount--;
	}
}

/**********************************************************************/

void CCSVReportLine::AppendPercent ( double dVal, int nDps )
{
	if ( nDps < 0 )
		nDps = SysInfo.GetDPValue();

	CString str;
	str.Format( "%.2f%%", dVal );
	Add( str );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

const char* CStockReportLine::GetStockLevelString ( double dQty, int nSubUnits )
{
	m_strStockLevel = "";
		
	if ( dQty < 0.0 )
	{
		m_strStockLevel = "-";
		dQty = -dQty;
	}

	int nStock = ( int ) dQty;
	int nSub = ( int ) ( ( dQty * nSubUnits ) - ( nStock * nSubUnits ) + 0.5 );
	
	CString strTemp;
	strTemp.Format ( "%d/%d", nStock, nSub );
	m_strStockLevel += strTemp;
	
	return m_strStockLevel;
}

/**********************************************************************/

void CStockReportLine::AppendStockQty ( double dQty, int nSubUnits, bool bSingleItem, int nDps )
{
	if ( nDps < 0 )
		nDps = SysInfo.GetDPQty();

	if ( bSingleItem == FALSE || nSubUnits <= 1 )
		Add ( dQty, nDps, FALSE );
	else
		Add ( GetStockLevelString ( dQty, nSubUnits ) );
}

/**********************************************************************/
#endif
/**********************************************************************/
