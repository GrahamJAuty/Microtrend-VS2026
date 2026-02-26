/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayEposConsolParamsArray.h"
/**********************************************************************/

CPosTrayEposConsolParamsArray::CPosTrayEposConsolParamsArray()
{
	m_nBatchNo = 0;
	m_strBatchFilename = "";
	m_strTempFilename = "";
}

/**********************************************************************/

void CPosTrayEposConsolParamsArray::SetBatchNo( int nBatchNo )
{
	m_nBatchNo = nBatchNo;

	{
		CFilenameUpdater FnUp( SysFiles::PosTrayConsolEpos, 0, nBatchNo );
		m_strBatchFilename = FnUp.GetFilenameToUse();
	}

	{
		CFilenameUpdater FnUp( SysFiles::PosTrayConsolTemp, 0, nBatchNo );
		m_strTempFilename = FnUp.GetFilenameToUse();
	}
}

/**********************************************************************/

void CPosTrayEposConsolParamsArray::Read( bool bModifiedBatch )
{
	m_arrayParams.RemoveAll();

	CString strFilename = ( bModifiedBatch ? m_strTempFilename : m_strBatchFilename );

	CSSFile fileData;
	if ( fileData.Open( strFilename, "rb" ) == TRUE )
	{
		CString strBuffer = "";
		while ( fileData.ReadString( strBuffer ) == TRUE )
		{
			CCSV csv( strBuffer );
			CSortedStringByInt item;
			item.m_nItem = csv.GetInt(0);
			item.m_strItem = strBuffer;
			m_arrayParams.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CPosTrayEposConsolParamsArray::Write()
{
	CSSFile fileData;
	if ( fileData.Open( m_strTempFilename, "wb" ) == TRUE )
	{
		for ( int n = 0; n < m_arrayParams.GetSize(); n++ )
		{
			CSortedStringByInt item;
			m_arrayParams.GetAt( n, item );
			fileData.WriteLine( item.m_strItem );
		}
	}
}

/**********************************************************************/

void CPosTrayEposConsolParamsArray::GetParams( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams )
{
	infoParams.Reset();

	CSortedStringByInt item;
	item.m_nItem = infoReport.GetReportType();
	
	int nPos;
	if ( m_arrayParams.Find( item, nPos ) == TRUE )
	{
		m_arrayParams.GetAt( nPos, item );
		
		CCSV csv( item.m_strItem );

		int nOffset = 1;
		bool bAccept = TRUE;

		if ( infoReport.GetReportType() >= EPOS_CUSTOM_FIRST_REPORT )
		{
			bAccept = ( csv.GetInt(1) == infoReport.GetCustomReportSerialNo() );
			nOffset = 2;
		}
	
		if ( TRUE == bAccept )
		{
			switch( csv.GetInt( nOffset++ ) )
			{
			case 1:
				infoParams.m_bOverride = csv.GetBool(nOffset++);
				infoParams.m_nConsolPrefsType = csv.GetInt(nOffset++);
				infoParams.m_bCustomPrefs = csv.GetBool(nOffset++);
				infoParams.m_strCustomPrefs = csv.GetString(nOffset++);
				break;
			}
		}
	}
}

/**********************************************************************/

void CPosTrayEposConsolParamsArray::SetParams( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams )
{
	CCSV csvParams;
	csvParams.Add( infoReport.GetReportType() );

	if ( infoReport.GetReportType() >= EPOS_CUSTOM_FIRST_REPORT )
		csvParams.Add( infoReport.GetCustomReportSerialNo() );

	csvParams.Add(1);
	csvParams.Add( infoParams.m_bOverride );
	csvParams.Add( infoParams.m_nConsolPrefsType );
	csvParams.Add( infoParams.m_bCustomPrefs );
	csvParams.Add( infoParams.m_strCustomPrefs );

	CSortedStringByInt item;
	item.m_nItem = infoReport.GetReportType();
	item.m_strItem = csvParams.GetLine();
	
	int nPos;
	if ( m_arrayParams.Find( item, nPos ) == FALSE )
		m_arrayParams.InsertAt( nPos, item );
	else
		m_arrayParams.SetAt( nPos, item );
}

/**********************************************************************/
