/**********************************************************************/
#include "SalesHistoryFields.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "PresetReportStockLevel.h"
/**********************************************************************/

CPresetReportInfoStockLevel::CPresetReportInfoStockLevel()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoStockLevel::Reset()
{
	m_nReportType = STOCK_SALES;
	m_nSortOrder = SH_STKLVL_NONE;
	m_bShowItems = FALSE;
	m_nTimeType = 0;
	m_bSuppName = FALSE;
	m_bSuppRef = FALSE;
	m_bStockUnit = FALSE;
	m_bDrawLines = FALSE;
	m_bAlphaSort = FALSE;
	m_nSupplierFilter = 0;
	m_nCategoryFilter = 0;

	for ( int n = 0; n <= 5; n++ )
		m_nCustomFieldFilter[n] = 0;

	m_bStockRange = FALSE;
	m_strStockFrom = "";
	m_strStockTo = "";
	m_bExcludeInactive = FALSE;
	m_nStocktake = 0;
}

/**********************************************************************/

void CPresetReportInfoStockLevel::ToString( CString& str )
{
	CCSV csv;

	csv.Add( m_nReportType );
	csv.Add( m_nSortOrder );
	csv.Add( m_bShowItems );
	csv.Add( m_nTimeType );
	csv.Add( m_bSuppName );
	csv.Add( m_bSuppRef );
	csv.Add( m_bStockUnit );
	csv.Add( m_bDrawLines );
	csv.Add( m_bAlphaSort );
	csv.Add( m_nSupplierFilter );
	csv.Add( m_nCategoryFilter );

	for ( int n = 0; n <= 5; n++ )
		csv.Add( m_nCustomFieldFilter[n] );

	csv.Add( m_bStockRange );
	csv.Add( m_strStockFrom );
	csv.Add( m_strStockTo );
	csv.Add( m_bExcludeInactive );
	csv.Add( m_nStocktake );
	
	str = csv.GetLine();
}

/**********************************************************************/

void CPresetReportInfoStockLevel::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_nReportType = csv.GetInt(nPos++);
	m_nSortOrder = csv.GetInt(nPos++);
	m_bShowItems = csv.GetBool(nPos++);
	m_nTimeType = csv.GetInt(nPos++);
	m_bSuppName = csv.GetBool(nPos++);
	m_bSuppRef = csv.GetBool(nPos++);
	m_bStockUnit = csv.GetBool(nPos++);
	m_bDrawLines = csv.GetBool(nPos++);
	m_bAlphaSort = csv.GetBool(nPos++);
	m_nSupplierFilter = csv.GetInt(nPos++);
	m_nCategoryFilter = csv.GetInt(nPos++);

	for ( int n = 0; n <= 5; n++ )
		m_nCustomFieldFilter[n] = csv.GetInt(nPos++);;

	m_bStockRange = csv.GetBool(nPos++);
	m_strStockFrom = csv.GetString(nPos++);
	m_strStockTo = csv.GetString(nPos++);
	m_bExcludeInactive = csv.GetBool(nPos++);
	m_nStocktake = csv.GetInt(nPos++);
}

/**********************************************************************/

void CPresetReportInfoStockLevel::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	Reset();
	infoBase.Reset();
	infoBase.SetName ( csv.GetString (0) );
	
	int nVer = csv.GetInt(1);
	switch ( nVer )
	{
	case 1:
	case 2:
	case 3:	
	case 4: V1To4ConvertFromCSV ( csv, infoBase, nVer );	break;
	}
}

/**********************************************************************/

void CPresetReportInfoStockLevel::V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer )
{
	int nPos = 2;

	if ( nVer >= 2 )
		infoBase.SetEntityNo( csv.GetInt(nPos++)) ;

	SetReportType( csv.GetInt(nPos++) );
	SetSortOrder( csv.GetInt(nPos++) );
	SetShowItemsFlag( csv.GetBool(nPos++) );
	SetTimeType( csv.GetInt(nPos++) );
	
	if ( nVer >= 3 )
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

	SetSuppNameFlag( csv.GetBool(nPos++) );
	SetSuppRefFlag( csv.GetBool(nPos++) );
	SetStockUnitFlag( csv.GetBool(nPos++) );
	SetDrawLinesFlag( csv.GetBool(nPos++) );
	SetSupplierFilter( csv.GetInt(nPos++) );
	SetCategoryFilter( csv.GetInt(nPos++) );

	for ( int n = 1; n <= 5; n++ )
		SetCustomFieldFilter( n, csv.GetInt( nPos++ ) );

	SetStockRangeFlag( csv.GetBool(nPos++) );
	SetStockFrom( csv.GetString(nPos++) );
	SetStockTo( csv.GetString(nPos++) );
	SetExcludeInactiveFlag( csv.GetBool(nPos++) );

	if ( nVer >= 4 )
		SetItemFilterFlag( csv.GetBool(nPos++) );

	SetStocktake( csv.GetInt(nPos++) );
	SetAlphaSortFlag( csv.GetBool(nPos++) );
}

/**********************************************************************/

void CPresetReportInfoStockLevel::ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	csv.Add ( infoBase.GetName() );
	csv.Add ( PRESET_STOCKLEVEL_VERSION );
	csv.Add ( infoBase.GetEntityNo() );
	csv.Add ( m_nReportType );
	csv.Add ( m_nSortOrder );
	csv.Add ( m_bShowItems );
	csv.Add ( m_nTimeType );
	csv.Add ( infoBase.GetPresetConLevel() );
	csv.Add ( infoBase.GetPresetEntityNo1() );
	csv.Add ( infoBase.GetPresetEntityNo2() );
	csv.Add ( infoBase.GetPasswordEnableFlag() );
	csv.Add ( infoBase.GetPasswordConLevel() );
	csv.Add ( infoBase.GetPasswordEntityNo1() );
	csv.Add ( infoBase.GetPasswordEntityNo2() );
	csv.Add ( m_bSuppName );
	csv.Add ( m_bSuppRef );
	csv.Add ( m_bStockUnit );
	csv.Add ( m_bDrawLines );
	csv.Add ( m_nSupplierFilter );
	csv.Add ( m_nCategoryFilter );

	for ( int n = 1; n <= 5; n++ )
		csv.Add( m_nCustomFieldFilter[n] );

	csv.Add ( m_bStockRange );
	csv.Add ( m_strStockFrom );
	csv.Add ( m_strStockTo );
	csv.Add ( m_bExcludeInactive );
	csv.Add ( m_bItemFilter );
	csv.Add ( m_nStocktake );
	csv.Add ( m_bAlphaSort );
}

/**********************************************************************/

int CPresetReportInfoStockLevel::GetCustomFieldFilter( int nFieldNo )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= 5 ) )
		return m_nCustomFieldFilter[nFieldNo];
	else
		return 0;
}

/**********************************************************************/
			
void CPresetReportInfoStockLevel::SetCustomFieldFilter( int nFieldNo, int n )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= 5 ) )
		m_nCustomFieldFilter[nFieldNo] = n;
}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayStockLevel::CPresetReportArrayStockLevel(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayStockLevel::~CPresetReportArrayStockLevel(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayStockLevel::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepStockLevel, 0, m_bStocktake ? 1 : 0 );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayStockLevel::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoStockLevel infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayStockLevel::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoStockLevel infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
