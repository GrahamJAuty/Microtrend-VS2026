/**********************************************************************/
#include "Password.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SalesXXXDetector.h"
/**********************************************************************/

CSalesXXXDetector::CSalesXXXDetector()
{
	m_nSpIdx = 0;
	m_nSalesXXXSpIdx = -1;
	m_nDelayType = SALESXXX_DELAY_10MIN;

	m_bReadyToGo = FALSE;
	m_oleTimeNextCheck = COleDateTime( 1980, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CSalesXXXDetector::CheckSpIdx()
{
	if ( m_nSpIdx < 0 || m_nSpIdx >= dbStockpoint.GetSize() )
		m_nSpIdx = 0;

	if ( PasswordArray.CheckLocationAccessSpIdx( m_nSpIdx ) == FALSE )
	{
		m_nSpIdx = -1;

		switch( PasswordArray.GetLocationAccessType() )
		{
		case LOCATION_ACCESS_LOC:
			m_nSpIdx = PasswordArray.GetLocationAccessSpIdx();
			break;

		case LOCATION_ACCESS_DB:
			{
				int nStartIdx, nEndIdx;
				dbStockpoint.GetSpIdxRange( PasswordArray.GetLocationAccessDbIdx(), nStartIdx, nEndIdx );

				if ( nEndIdx >= nStartIdx )
					m_nSpIdx = nStartIdx;
				else
					m_nSpIdx = -1;
			}
			break;

		case LOCATION_ACCESS_SYSTEM:
			m_nSpIdx = 0;
			break;

		default:
			m_nSpIdx = -1;
			break;
		}
	}
}

/**********************************************************************/

void CSalesXXXDetector::FindNextSp()
{
	m_nSpIdx++;
	if ( m_nSpIdx < 0 || m_nSpIdx >= dbStockpoint.GetSize() )
		m_nSpIdx = 0;
}

/**********************************************************************/

void CSalesXXXDetector::SetDelayType( int n )
{
	switch ( n )
	{
	case SALESXXX_DELAY_FOREVER:
	case SALESXXX_DELAY_10MIN:
	case SALESXXX_DELAY_20MIN:
	case SALESXXX_DELAY_30MIN:
	case SALESXXX_DELAY_1HOUR:
	case SALESXXX_DELAY_2HOUR:
	case SALESXXX_DELAY_3HOUR:
		m_nDelayType = n; 
	}
}

/**********************************************************************/

void CSalesXXXDetector::StartDelay()
{
	m_nSalesXXXSpIdx = -1;
	
	int nHour = 0;
	int nMinute = 0;
	bool bCheck = TRUE;

	switch ( m_nDelayType )
	{
	case SALESXXX_DELAY_10MIN:	nMinute = 10;		break;
	case SALESXXX_DELAY_20MIN:	nMinute = 20;		break;
	case SALESXXX_DELAY_30MIN:	nMinute = 30;		break;
	case SALESXXX_DELAY_1HOUR:	nHour = 1;		break;
	case SALESXXX_DELAY_2HOUR:	nHour = 2;		break;
	case SALESXXX_DELAY_3HOUR:	nHour = 3;		break;
	case SALESXXX_DELAY_FOREVER:
	default:					bCheck = FALSE;	break;
	}

	if ( bCheck == TRUE )
	{
		m_oleTimeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan( 0, nHour, nMinute, 0 );
		m_bReadyToGo = TRUE;
	}
	else
	{
		m_oleTimeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan( 7, 0, 0, 0 );
		m_bReadyToGo = FALSE;
	}
}

/**********************************************************************/

void CSalesXXXDetector::LookForSalesXXX()
{
	if ( m_bReadyToGo == FALSE )
		return;

	if ( m_nSalesXXXSpIdx != -1 )
		return;

	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	if ( timeNow < m_oleTimeNextCheck )
		return;

	CheckSpIdx();
	if ( -1 == m_nSpIdx )
		return;

	if ( dbStockpoint.GetSize() == 0 )
		return;
	
	CString strPath = dbStockpoint.GetFolderPathSalesXXX( m_nSpIdx );

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPath + "\\*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//IGNORE FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		m_nSalesXXXSpIdx = m_nSpIdx;
		break;
	}
	
	FindNextSp();
	m_oleTimeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan( 0, 0, 0, 1 );
}

/**********************************************************************/

bool CSalesXXXDetector::CheckSpSalesXXX()
{
	if ( m_nSalesXXXSpIdx < 0 || m_nSalesXXXSpIdx >= dbStockpoint.GetSize() )
	{
		m_nSalesXXXSpIdx = -1;
		return FALSE;
	}

	CFileFind FileFinder;	
	CString strPath = dbStockpoint.GetFolderPathSalesXXX( m_nSalesXXXSpIdx );
	bool bWorking = ( FileFinder.FindFile ( strPath + "\\*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//IGNORE FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		return TRUE;
	}
	
	m_nSalesXXXSpIdx = -1;
	return FALSE;
}

/**********************************************************************/
