/**********************************************************************/
#include "PresetReportCustHist.h"
/**********************************************************************/

CPresetReportInfoCustHist::CPresetReportInfoCustHist()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoCustHist::Reset()
{	
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_nRoundToType = 0;
	m_strSessionName = "";
	m_strTimeSliceName = "";
	
	m_bPluRange = FALSE;
	m_strPluFrom = "";
	m_strPluTo = "";
	m_nDepartmentFilter = 0;
	m_nDepartmentFilterType = 0;
	m_nTaxFilter = 0;
	m_nPriceLevelFilter = 0;
	m_nTaxCustomFilter = 0;
	m_nPriceLevelCustomFilter = 0;

	m_bSales = TRUE;
	m_bRA = TRUE;
	m_bTax = TRUE;
	m_bCharges = TRUE;
	m_bPayMeth = TRUE;
	m_bCustomerList = FALSE;
	m_bCustomerPage = FALSE;

	m_strReportName = "";
}

/**********************************************************************/

void CPresetReportInfoCustHist::ToString( CString& str )
{
	CCSV csv;

	csv.Add( m_strDateType );
	csv.Add( m_strDateFrom );
	csv.Add( m_strDateTo );
	csv.Add( m_nRoundToType );
	csv.Add( m_strSessionName );
	csv.Add( m_strTimeSliceName );
	
	csv.Add( m_bPluRange );
	csv.Add( m_strPluFrom );
	csv.Add( m_strPluTo );
	csv.Add( m_nDepartmentFilter );
	csv.Add( m_nDepartmentFilterType );
	csv.Add( m_nTaxFilter );
	csv.Add( m_nPriceLevelFilter );
	csv.Add( m_nTaxCustomFilter );
	csv.Add( m_nPriceLevelCustomFilter );

	csv.Add( m_bSales );
	csv.Add( m_bRA );
	csv.Add( m_bTax );
	csv.Add( m_bCharges );
	csv.Add( m_bPayMeth );
	csv.Add( m_bCustomerList );
	csv.Add( m_bCustomerPage );

	csv.Add( m_strReportName );
	
	str = csv.GetLine();
}


/**********************************************************************/

void CPresetReportInfoCustHist::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_nRoundToType = csv.GetInt(nPos++);
	m_strSessionName = csv.GetString(nPos++);
	m_strTimeSliceName = csv.GetString(nPos++);
	
	m_bPluRange = csv.GetBool(nPos++);
	m_strPluFrom = csv.GetString(nPos++);
	m_strPluTo = csv.GetString(nPos++);
	m_nDepartmentFilter = csv.GetInt(nPos++);
	m_nDepartmentFilterType = csv.GetInt(nPos++);
	m_nTaxFilter = csv.GetInt(nPos++);
	m_nPriceLevelFilter = csv.GetInt(nPos++);
	m_nTaxCustomFilter = csv.GetInt(nPos++);
	m_nPriceLevelCustomFilter = csv.GetInt(nPos++);

	m_bSales = csv.GetBool(nPos++);
	m_bRA = csv.GetBool(nPos++);
	m_bTax = csv.GetBool(nPos++);
	m_bCharges = csv.GetBool(nPos++);
	m_bPayMeth = csv.GetBool(nPos++);
	m_bCustomerList = csv.GetBool(nPos++);
	m_bCustomerPage = csv.GetBool(nPos++);

	m_strReportName = csv.GetString(nPos++);
}

/**********************************************************************/

void CPresetReportInfoCustHist::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
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

void CPresetReportInfoCustHist::V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer )
{
	int nPos = 2;

	if ( nVer >= 2 )
		infoBase.SetEntityNo( csv.GetInt( nPos++ ) );

	SetDateType( csv.GetString(nPos++) );
	SetDateFrom( csv.GetString(nPos++) );
	SetDateTo( csv.GetString(nPos++) );
	SetRoundToType( csv.GetInt(nPos++) );
	SetSessionName( csv.GetString(nPos++) );
	SetTimeSliceName( csv.GetString(nPos++) );

	if ( nVer >= 3 )
	{
		infoBase.SetPresetConLevel( csv.GetInt(nPos++) );
		infoBase.SetPresetEntityNo1( csv.GetInt(nPos++) );
		infoBase.SetPresetEntityNo2( csv.GetInt(nPos++) );
		infoBase.SetTerminalIdx( csv.GetInt(nPos++) );
		infoBase.SetPasswordEnableFlag( csv.GetBool(nPos++) );
		infoBase.SetPasswordConLevel( csv.GetInt(nPos++) );
		infoBase.SetPasswordEntityNo1( csv.GetInt(nPos++) );
		infoBase.SetPasswordEntityNo2( csv.GetInt(nPos++) );
	}
	else
	{
		infoBase.SetPresetConLevel( csv.GetInt(nPos++) );
		int nLegacyIdx = csv.GetInt(nPos++);
		
		infoBase.SetTerminalIdx( csv.GetInt(nPos++) );
		int nEntityNo1, nEntityNo2;
		CPresetReportArray::GetLegacyConLevelInfo(infoBase.GetPresetConLevel(), nLegacyIdx, nEntityNo1, nEntityNo2 );
		infoBase.SetPresetEntityNo1( nEntityNo1 );
		infoBase.SetPresetEntityNo2( nEntityNo2 );
	}

	SetPluRangeFlag( csv.GetBool(nPos++) );
	SetPluFrom( csv.GetString(nPos++) );
	SetPluTo( csv.GetString(nPos++) );
	SetDepartmentFilter( csv.GetInt(nPos++) );
	SetDepartmentFilterType( csv.GetInt(nPos++) );
	SetTaxFilter( csv.GetInt(nPos++) );
	SetReportName( csv.GetString(nPos++) );
	SetPriceLevelFilter( csv.GetInt(nPos++) );
	SetSalesFlag( csv.GetBool(nPos++) );
	SetRAFlag( csv.GetBool(nPos++) );
	SetChargesFlag( csv.GetBool(nPos++) );
	SetCustomerListFlag( csv.GetBool(nPos++) );
	SetCustomerPageFlag( csv.GetBool(nPos++) );
	SetPayMethFlag( csv.GetBool(nPos++) );
	SetTaxFlag( csv.GetBool(nPos++) );

	if ( nVer >= 4 )
	{
		SetTaxCustomFilter( csv.GetInt(nPos++) );
		SetPriceLevelCustomFilter( csv.GetInt(nPos++) );
	}
}

/**********************************************************************/

void CPresetReportInfoCustHist::ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	csv.Add ( infoBase.GetName() );
	csv.Add ( PRESET_CUSTHIST_VERSION );
	csv.Add ( infoBase.GetEntityNo() );
	csv.Add ( m_strDateType );
	csv.Add ( m_strDateFrom );
	csv.Add ( m_strDateTo );
	csv.Add ( m_nRoundToType );
	csv.Add ( m_strSessionName );
	csv.Add ( m_strTimeSliceName );
	csv.Add ( infoBase.GetPresetConLevel() );
	csv.Add ( infoBase.GetPresetEntityNo1() );
	csv.Add ( infoBase.GetPresetEntityNo2() );
	csv.Add ( infoBase.GetTerminalIdx() );
	csv.Add ( infoBase.GetPasswordEnableFlag() );
	csv.Add ( infoBase.GetPasswordConLevel() );
	csv.Add ( infoBase.GetPasswordEntityNo1() );
	csv.Add ( infoBase.GetPasswordEntityNo2() );
	csv.Add ( m_bPluRange );
	csv.Add ( m_strPluFrom );
	csv.Add ( m_strPluTo );
	csv.Add ( m_nDepartmentFilter );
	csv.Add ( m_nDepartmentFilterType );
	csv.Add ( m_nTaxFilter );
	csv.Add ( m_strReportName );
	csv.Add ( m_nPriceLevelFilter );
	csv.Add ( m_bSales );
	csv.Add ( m_bRA );
	csv.Add ( m_bCharges );
	csv.Add ( m_bCustomerList );
	csv.Add ( m_bCustomerPage );
	csv.Add ( m_bPayMeth );
	csv.Add ( m_bTax );
	csv.Add ( m_nTaxCustomFilter );
	csv.Add ( m_nPriceLevelCustomFilter );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayCustHist::CPresetReportArrayCustHist(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayCustHist::~CPresetReportArrayCustHist(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayCustHist::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepCustHist );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayCustHist::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoCustHist infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayCustHist::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoCustHist infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
