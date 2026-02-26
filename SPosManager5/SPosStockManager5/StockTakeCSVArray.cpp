/**********************************************************************/
#include "..\CommonStockTray\StockLevelsCSVArray.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StocktakeCSVArray.h"
/**********************************************************************/

CStocktakeCSVRecord::CStocktakeCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CStocktakeCSVRecord::ClearRecord()
{
	m_nStocktakeNo = 1;
	m_nStocktakeType = 0;
	m_bClosingStockZero = FALSE;
	m_strDate = "01012003000000";
	m_strCategory = "";
	m_strSupplier = "";
	m_bActiveOnly = FALSE;
	m_strComment = "";
	m_nItemCount = 0;
}

/**********************************************************************/

void CStocktakeCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetStocktakeNo( csv.GetInt(0) );

	int nVer = csv.GetInt(1);
	switch ( nVer )
	{
	case 1:	V1ConvertFromCSV ( csv );			break;
	case 2:	V2ConvertFromCSV ( csv );			break;
	case 3:	
	case 4: V3to4ConvertFromCSV ( csv, nVer );	break;
	}
}

/**********************************************************************/

void CStocktakeCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetStocktakeType( csv.GetInt(2) );
	SetCategory( csv.GetString(3) );
	SetComment( csv.GetString(4) );
	SetDate( csv.GetString(5) );
	SetItemCount( csv.GetInt(6) );
}

/**********************************************************************/

void CStocktakeCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetStocktakeType( csv.GetInt(2) );
	SetCategory( csv.GetString(3) );
	SetSupplier( csv.GetString(4) );
	SetComment( csv.GetString(5) );
	SetDate( csv.GetString(6) );
	SetItemCount( csv.GetInt(7) );
}

/**********************************************************************/

void CStocktakeCSVRecord::V3to4ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 2;
	SetStocktakeType( csv.GetInt(n++) );
	SetClosingStockZeroFlag( csv.GetBool(n++) );
	SetCategory( csv.GetString(n++) );
	SetSupplier( csv.GetString(n++) );

	if ( nVer >= 4 )
		SetActiveOnlyFlag( csv.GetBool(n++) );

	SetComment( csv.GetString(n++) );
	SetDate( csv.GetString(n++) );
	SetItemCount( csv.GetInt(n++) );
}

/**********************************************************************/
	
void CStocktakeCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nStocktakeNo );
	csv.Add ( STOCKTAKE_VERSION );
	csv.Add ( m_nStocktakeType );
	csv.Add ( m_bClosingStockZero );
	csv.Add ( m_strCategory );
	csv.Add ( m_strSupplier );
	csv.Add ( m_bActiveOnly );
	csv.Add ( m_strComment );
	csv.Add ( m_strDate );
	csv.Add ( m_nItemCount );
}

/**********************************************************************/

void CStocktakeCSVRecord::SetStocktakeNo( int n )
{
	if ( n >= 1 && n <= MAX_STOCKTAKES ) m_nStocktakeNo = n;
}

/**********************************************************************/

void CStocktakeCSVRecord::SetStocktakeType( int n )
{
	switch ( n )
	{
	case STOCKTAKE_TYPE_PERIOD:
	case STOCKTAKE_TYPE_YTD:
	case STOCKTAKE_TYPE_ADJUST:
		m_nStocktakeType = n;
		break;
	}
}

/**********************************************************************/

void CStocktakeCSVRecord::SetDate( const char* szDate )
{
	CString strDate = szDate;
	if ( strDate.GetLength() != 14 )
		return;
	
	CSSDate Date ( strDate.Left(8) );
	if ( Date.IsSet() == FALSE ) 
		return;

	CSSTime Time ( strDate.Right(6) );
	if ( Time.IsSet() == FALSE )
		return;

	m_strDate = strDate;
}

/**********************************************************************/

const char* CStocktakeCSVRecord::GetFormattedDate()
{
	m_strFormattedDate = "Unknown";

	if ( m_strDate.GetLength() != 14 )
		return m_strFormattedDate;

	CSSDate Date ( m_strDate.Left(8) );
	if ( Date.IsSet() == FALSE ) 
		return m_strFormattedDate;

	CSSTime Time ( m_strDate.Right(6) );
	if ( Time.IsSet() == FALSE )
		return m_strFormattedDate; 

	m_strFormattedDate.Format ( "%2.2d/%2.2d/%4.4d at %2.2d:%2.2d:%2.2d",
		Date.GetDay(),
		Date.GetMonth(),
		Date.GetYear(),
		Time.GetHour(),
		Time.GetMinute(),
		Time.GetSecond() );

	return m_strFormattedDate;
}

/**********************************************************************/

const char* CStocktakeCSVRecord::GetStocktakeTypeText()
{
	switch ( m_nStocktakeType )
	{
	case STOCKTAKE_TYPE_PERIOD:	m_strStocktakeType = "PERIOD";	break;
	case STOCKTAKE_TYPE_YTD:		m_strStocktakeType = "YTD";		break;
	case STOCKTAKE_TYPE_ADJUST:	m_strStocktakeType = "ADJUST";	break;
	default:					m_strStocktakeType = "Unknown";	break;
	}
	return m_strStocktakeType;			
}

/**********************************************************************/

COleDateTime CStocktakeCSVRecord::GetOleDateTime()
{
	if ( m_strDate.GetLength() == 14 )
	{
		COleDateTime date;
		date.SetDateTime(	atoi ( m_strDate.Mid(4,4) ), 
							atoi ( m_strDate.Mid(2,2) ),
							atoi ( m_strDate.Left(2) ),
							atoi ( m_strDate.Mid(8,2) ),
							atoi ( m_strDate.Mid(10,2) ),
							atoi ( m_strDate.Mid(12,2) ) );

		if ( date.GetStatus() == COleDateTime::valid )
			return date;
	}
	return COleDateTime::GetCurrentTime();	
}

/**********************************************************************/

CStocktakeCSVArray::CStocktakeCSVArray() : CCSVArray()
{
}

/**********************************************************************/

int CStocktakeCSVArray::GetNextFreeNumber()
{
	if ( GetSize() >= MAX_STOCKTAKES )
		return 0;

	for ( int nStocktakeIdx = 0; nStocktakeIdx < GetSize(); nStocktakeIdx++ )
	{
		CStocktakeCSVRecord Stocktake;
		GetAt( nStocktakeIdx, Stocktake );

		if ( Stocktake.GetStocktakeNo() != nStocktakeIdx + 1 )
			return nStocktakeIdx + 1;
	}
	
	return GetSize() + 1;
}

/**********************************************************************/

void CStocktakeCSVArray::AddStocktake( int& nStocktakeIdx, int& nStocktakeNo )
{
	nStocktakeIdx = -1;
	nStocktakeNo = GetNextFreeNumber();
	
	if ( nStocktakeNo > 0 )
	{
		nStocktakeIdx = nStocktakeNo - 1;
		CStocktakeCSVRecord Stocktake;
		Stocktake.SetStocktakeNo( nStocktakeNo );
		InsertAt( nStocktakeIdx, Stocktake );
	}
}

/**********************************************************************/

void CStocktakeCSVArray::RemoveStocktake( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetSize() )
		RemoveAt( nIndex );
}

/**********************************************************************/

bool CStocktakeCSVArray::Read( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
		return FALSE;

	CFilenameUpdater FnUp( SysFiles::StockTakes, nSpIdx );
	CString strFilename = FnUp.GetFilenameToUse();
	
	bool bResult = CCSVArray::Read ( strFilename );

	int nFirstInvalidIdx = MAX_STOCKTAKES;
	int nLastStocktakeNo = 0;
	
	for ( int nStocktakeIdx = 0; nStocktakeIdx < GetSize(); nStocktakeIdx++ )
	{
		CStocktakeCSVRecord Stocktake;
		GetAt( nStocktakeIdx, Stocktake );

		int nStocktakeNo = Stocktake.GetStocktakeNo();
		if ( ( nStocktakeNo <= nLastStocktakeNo ) || ( nStocktakeNo > MAX_STOCKTAKES ) )
		{
			nFirstInvalidIdx = nStocktakeIdx;
			break;
		}

		nLastStocktakeNo = nStocktakeNo;
	}

	for ( int nStocktakeIdx = GetSize() - 1; nStocktakeIdx >= nFirstInvalidIdx; nStocktakeIdx-- )
		RemoveAt( nStocktakeIdx );
	
	return bResult;
}

/**********************************************************************/

bool CStocktakeCSVArray::Write( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
		return FALSE;

	CFilenameUpdater FnUp( SysFiles::StockTakes, nSpIdx );
	CString	strFilename = FnUp.GetFilenameToUse();
	
	return CCSVArray::Write ( strFilename );
}

/**********************************************************************/
