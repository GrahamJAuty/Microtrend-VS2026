/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
#include "..\CommonEcrTray\ReportPluStockLevel.h"
/**********************************************************************/
#include "PresetReportEcrStock.h"
/**********************************************************************/

CPresetReportInfoEcrStock::CPresetReportInfoEcrStock()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoEcrStock::Reset()
{
	m_nReportType = REPORT_PLUSTOCK_APPARENT;
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";

	m_bPluRange = FALSE;
	m_strPluFrom = "";
	m_strPluTo = "";
	m_nDepartmentFilter = 0;
	m_nSupplierFilter = ECRMANSUPPLIER_ALL;
	m_bZeroSkip = FALSE;

	m_bDepartmentSort = FALSE;
	m_bSupplierSort = FALSE;
}

/**********************************************************************/

void CPresetReportInfoEcrStock::ToString( CString& str )
{
	CCSV csv;

	csv.Add( m_nReportType );
	csv.Add( m_strDateType );
	csv.Add( m_strDateFrom );
	csv.Add( m_strDateTo );

	csv.Add( m_bPluRange );
	csv.Add( m_strPluFrom );
	csv.Add( m_strPluTo );
	csv.Add( m_nDepartmentFilter );
	csv.Add( m_nSupplierFilter  );
	csv.Add( m_bZeroSkip  );

	csv.Add( m_bDepartmentSort );
	csv.Add( m_bSupplierSort );
	
	str = csv.GetLine();
}


/**********************************************************************/

void CPresetReportInfoEcrStock::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_nReportType = csv.GetInt(nPos++);
	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);

	m_bPluRange = csv.GetBool(nPos++);
	m_strPluFrom = csv.GetString(nPos++);
	m_strPluTo = csv.GetString(nPos++);
	m_nDepartmentFilter = csv.GetInt(nPos++);
	m_nSupplierFilter = csv.GetInt(nPos++);
	m_bZeroSkip = csv.GetBool(nPos++);

	m_bDepartmentSort = csv.GetBool(nPos++);
	m_bSupplierSort = csv.GetBool(nPos++);
}

/**********************************************************************/

void CPresetReportInfoEcrStock::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	Reset();
	infoBase.Reset();
	infoBase.SetName ( csv.GetString (0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv, infoBase );	break;
	}
}

/**********************************************************************/

void CPresetReportInfoEcrStock::V1ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	int nPos = 2;

	infoBase.SetEntityNo( csv.GetInt( nPos++ ) );
	SetReportType( csv.GetInt( nPos++ ) );
	SetDateType( csv.GetString(nPos++) );
	SetDateFrom( csv.GetString(nPos++) );
	SetDateTo( csv.GetString(nPos++) );

	infoBase.SetPresetConLevel( csv.GetInt(nPos++) );
	infoBase.SetPresetEntityNo1( csv.GetInt(nPos++) );
	infoBase.SetPresetEntityNo2( csv.GetInt(nPos++) );
	infoBase.SetPasswordEnableFlag( csv.GetBool(nPos++) );
	infoBase.SetPasswordConLevel( csv.GetInt(nPos++) );
	infoBase.SetPasswordEntityNo1( csv.GetInt(nPos++) );
	infoBase.SetPasswordEntityNo2( csv.GetInt(nPos++) );

	SetPluRangeFlag( csv.GetBool(nPos++) );
	SetPluFrom( csv.GetString(nPos++) );
	SetPluTo( csv.GetString(nPos++) );
	SetDepartmentFilter( csv.GetInt(nPos++) );
	SetSupplierFilter( csv.GetInt(nPos++) );
	SetZeroSkipFlag( csv.GetBool(nPos++) );

	SetDepartmentSortFlag( csv.GetBool(nPos++) );
	SetSupplierSortFlag( csv.GetBool(nPos++) );
}

/**********************************************************************/

void CPresetReportInfoEcrStock::ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	csv.Add ( infoBase.GetName() );
	csv.Add ( PRESET_ECRSTOCK_VERSION );
	csv.Add ( infoBase.GetEntityNo() );
	csv.Add ( m_nReportType );
	csv.Add ( m_strDateType );
	csv.Add ( m_strDateFrom );
	csv.Add ( m_strDateTo );
	csv.Add ( infoBase.GetPresetConLevel() );
	csv.Add ( infoBase.GetPresetEntityNo1() );
	csv.Add ( infoBase.GetPresetEntityNo2() );
	csv.Add ( infoBase.GetPasswordEnableFlag() );
	csv.Add ( infoBase.GetPasswordConLevel() );
	csv.Add ( infoBase.GetPasswordEntityNo1() );
	csv.Add ( infoBase.GetPasswordEntityNo2() );
	csv.Add ( m_bPluRange );
	csv.Add ( m_strPluFrom );
	csv.Add ( m_strPluTo );
	csv.Add ( m_nDepartmentFilter );
	csv.Add ( m_nSupplierFilter );
	csv.Add ( m_bZeroSkip );
	csv.Add ( m_bDepartmentSort );
	csv.Add ( m_bSupplierSort );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayEcrStock::CPresetReportArrayEcrStock(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayEcrStock::~CPresetReportArrayEcrStock(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayEcrStock::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepEcrStock );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayEcrStock::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoEcrStock infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayEcrStock::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoEcrStock infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
