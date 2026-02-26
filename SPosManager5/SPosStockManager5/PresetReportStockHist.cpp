/**********************************************************************/
#include "PresetReportStockHist.h"
/**********************************************************************/

CPresetReportInfoStockHist::CPresetReportInfoStockHist()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoStockHist::Reset()
{
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_nRoundToType = 0;
	m_strSessionName = "";
	
	m_bEposSales = FALSE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;
	
	m_bStockRange = FALSE;
	m_strStockFrom = "";
	m_strStockTo = "";
	m_nCategoryFilter = 0;
	m_nCategoryFilterType = 0;
	m_nSupplierFilter = 0;

	m_strReportName = "";
	m_bByMonth = FALSE;
	m_bCatSort = FALSE;
	m_bPercentSales = FALSE;
	m_bEstProfit = FALSE;
	m_bApparent = FALSE;

	for ( int n = 0; n <= 5; n++ )
		m_nCustomFieldFilter[n] = 0;
}

/**********************************************************************/

void CPresetReportInfoStockHist::ToString( CString& str )
{
	CCSV csv;

	csv.Add( m_strDateType );
	csv.Add( m_strDateFrom );
	csv.Add( m_strDateTo );
	csv.Add( m_nRoundToType );
	csv.Add( m_strSessionName );
	
	csv.Add( m_bEposSales );
	csv.Add( m_bManualSales );
	csv.Add( m_bImportSales );
	
	csv.Add( m_bStockRange );
	csv.Add( m_strStockFrom );
	csv.Add( m_strStockTo );
	csv.Add( m_nCategoryFilter );
	csv.Add( m_nCategoryFilterType );
	csv.Add( m_nSupplierFilter );

	csv.Add( m_strReportName );
	csv.Add( m_bByMonth );
	csv.Add( m_bCatSort );
	csv.Add( m_bPercentSales );
	csv.Add( m_bEstProfit );
	csv.Add( m_bApparent );

	for ( int n = 0; n <= 5; n++ )
		csv.Add( m_nCustomFieldFilter[n]  );
	
	str = csv.GetLine();
}


/**********************************************************************/

void CPresetReportInfoStockHist::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_nRoundToType = csv.GetInt(nPos++);
	m_strSessionName = csv.GetString(nPos++);
	
	m_bEposSales = csv.GetBool(nPos++);
	m_bManualSales = csv.GetBool(nPos++);
	m_bImportSales = csv.GetBool(nPos++);
	
	m_bStockRange = csv.GetBool(nPos++);
	m_strStockFrom = csv.GetString(nPos++);
	m_strStockTo = csv.GetString(nPos++);
	m_nCategoryFilter = csv.GetInt(nPos++);
	m_nCategoryFilterType = csv.GetInt(nPos++);
	m_nSupplierFilter = csv.GetInt(nPos++);

	m_strReportName = csv.GetString(nPos++);
	m_bByMonth = csv.GetBool(nPos++);
	m_bCatSort = csv.GetBool(nPos++);
	m_bPercentSales = csv.GetBool(nPos++);
	m_bEstProfit = csv.GetBool(nPos++);
	m_bApparent = csv.GetBool(nPos++);

	for ( int n = 0; n <= 5; n++ )
		m_nCustomFieldFilter[n] = csv.GetInt(nPos++);
}

/**********************************************************************/

void CPresetReportInfoStockHist::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	Reset();
	infoBase.Reset();
	infoBase.SetName ( csv.GetString (0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1To4ConvertFromCSV ( csv, infoBase, 1 );	break;
	case 2:	V1To4ConvertFromCSV ( csv, infoBase, 2 );	break;
	case 3:	V1To4ConvertFromCSV ( csv, infoBase, 3 );	break;
	case 4:	V1To4ConvertFromCSV ( csv, infoBase, 4 );	break;
	}
}

/**********************************************************************/

void CPresetReportInfoStockHist::V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer )
{
	int nPos = 2;

	if ( nVer >= 3 )
		infoBase.SetEntityNo( csv.GetInt( nPos++ ) );

	SetDateType( csv.GetString(nPos++) );
	SetDateFrom( csv.GetString(nPos++) );
	SetDateTo( csv.GetString(nPos++) );
	SetRoundToType( csv.GetInt(nPos++) );
	SetSessionName( csv.GetString(nPos++) );

	if ( nVer >= 4 )
	{
		infoBase.SetPresetConLevel( csv.GetInt(nPos++) );
		infoBase.SetPresetEntityNo1( csv.GetInt(nPos++) );
		infoBase.SetPresetEntityNo2( csv.GetInt(nPos++) );
		infoBase.SetPasswordEnableFlag( csv.GetBool(nPos++) );
		infoBase.SetPasswordConLevel( csv.GetInt(nPos++) );
		infoBase.SetPasswordEntityNo1( csv.GetInt(nPos++) );
		infoBase.SetPasswordEntityNo2( csv.GetInt(nPos++) );
	}
	else
	{
		infoBase.SetPresetConLevel( csv.GetInt(nPos++) );
		int nLegacyIdx = csv.GetInt(nPos++);

		int nEntityNo1, nEntityNo2;
		CPresetReportArray::GetLegacyConLevelInfo( infoBase.GetPresetConLevel(), nLegacyIdx, nEntityNo1, nEntityNo2 );
		infoBase.SetPresetEntityNo1( nEntityNo1 );
		infoBase.SetPresetEntityNo2( nEntityNo2 );
	}

	SetEposSalesFlag( csv.GetBool(nPos++) );
	SetManualSalesFlag( csv.GetBool(nPos++) );
	SetImportSalesFlag( csv.GetBool(nPos++) );
	SetStockRangeFlag( csv.GetBool(nPos++) );
	SetStockFrom( csv.GetString(nPos++) );
	SetStockTo( csv.GetString(nPos++) );
	SetCategoryFilter( csv.GetInt(nPos++) );
	SetCategoryFilterType( csv.GetInt(nPos++) );
	SetSupplierFilter( csv.GetInt(nPos++) );
	SetReportName( csv.GetString(nPos++) );
	SetByMonthFlag( csv.GetBool(nPos++) );
	SetCatSortFlag( csv.GetBool(nPos++) );
	SetPercentSalesFlag( csv.GetBool(nPos++) );
	SetEstProfitFlag( csv.GetBool(nPos++) );

	if ( nVer >= 2 )
	{
		SetApparentFlag( csv.GetBool(nPos++) );

		for ( int n = 1; n <= 5; n++ )
			SetCustomFieldFilter( n, csv.GetInt( nPos++ ) );
	}
}

/**********************************************************************/

void CPresetReportInfoStockHist::ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	csv.Add ( infoBase.GetName() );
	csv.Add ( PRESET_STOCKHIST_VERSION );
	csv.Add ( infoBase.GetEntityNo() );
	csv.Add ( m_strDateType );
	csv.Add ( m_strDateFrom );
	csv.Add ( m_strDateTo );
	csv.Add ( m_nRoundToType );
	csv.Add ( m_strSessionName );
	csv.Add ( infoBase.GetPresetConLevel() );
	csv.Add ( infoBase.GetPresetEntityNo1() );
	csv.Add ( infoBase.GetPresetEntityNo2() );
	csv.Add ( infoBase.GetPasswordEnableFlag() );
	csv.Add ( infoBase.GetPasswordConLevel() );
	csv.Add ( infoBase.GetPasswordEntityNo1() );
	csv.Add ( infoBase.GetPasswordEntityNo2() );
	csv.Add ( m_bEposSales );
	csv.Add ( m_bManualSales );
	csv.Add ( m_bImportSales );
	csv.Add ( m_bStockRange );
	csv.Add ( m_strStockFrom );
	csv.Add ( m_strStockTo );
	csv.Add ( m_nCategoryFilter );
	csv.Add ( m_nCategoryFilterType );
	csv.Add ( m_nSupplierFilter );
	csv.Add ( m_strReportName );
	csv.Add ( m_bByMonth );
	csv.Add ( m_bCatSort );
	csv.Add ( m_bPercentSales );
	csv.Add ( m_bEstProfit );
	csv.Add ( m_bApparent );

	for ( int n = 1; n <= 5; n++ )
		csv.Add( m_nCustomFieldFilter[n] );
}

/**********************************************************************/

int CPresetReportInfoStockHist::GetCustomFieldFilter( int nFieldNo )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= 5 ) )
		return m_nCustomFieldFilter[nFieldNo];
	else
		return 0;
}

/**********************************************************************/
			
void CPresetReportInfoStockHist::SetCustomFieldFilter( int nFieldNo, int n )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= 5 ) )
		m_nCustomFieldFilter[nFieldNo] = n;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayStockHist::CPresetReportArrayStockHist(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayStockHist::~CPresetReportArrayStockHist(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayStockHist::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepStockHist );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayStockHist::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoStockHist infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayStockHist::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoStockHist infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
