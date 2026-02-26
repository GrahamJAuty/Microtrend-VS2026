/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "PresetReportStockAudit.h"
/**********************************************************************/

CPresetReportInfoStockAudit::CPresetReportInfoStockAudit()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoStockAudit::Reset()
{
	m_nReportType = AUDIT_DELIVERY;
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_bStockRange = FALSE;
	m_strStockFrom = "";
	m_strStockTo = "";
	m_bOnePerPage = FALSE;
	m_strHeaderReference = "";
	m_strLineReference = "";
}

/**********************************************************************/

void CPresetReportInfoStockAudit::ToString( CString& str )
{
	CCSV csv;

	csv.Add( m_nReportType );
	csv.Add( m_strDateType );
	csv.Add( m_strDateFrom );
	csv.Add( m_strDateTo );
	csv.Add( m_bStockRange );
	csv.Add( m_strStockFrom );
	csv.Add( m_strStockTo );
	csv.Add( m_bOnePerPage );
	csv.Add( m_strHeaderReference );
	csv.Add( m_strLineReference );
	
	str = csv.GetLine();
}


/**********************************************************************/

void CPresetReportInfoStockAudit::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_nReportType = csv.GetInt(nPos++);
	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_bStockRange = csv.GetBool(nPos++);
	m_strStockFrom = csv.GetString(nPos++);
	m_strStockTo = csv.GetString(nPos++);
	m_bOnePerPage = csv.GetBool(nPos++);
	m_strHeaderReference = csv.GetString(nPos++);
	m_strLineReference = csv.GetString(nPos++);
}

/**********************************************************************/

void CPresetReportInfoStockAudit::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
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

void CPresetReportInfoStockAudit::V1ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
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
	SetStockRangeFlag( csv.GetBool(nPos++) );
	SetStockFrom( csv.GetString(nPos++) );
	SetStockTo( csv.GetString(nPos++) );
	SetOnePerPageFlag( csv.GetBool(nPos++) );
	SetHeaderReference( csv.GetString(nPos++) );
	SetLineReference( csv.GetString(nPos++) );
}

/**********************************************************************/

void CPresetReportInfoStockAudit::ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	csv.Add ( infoBase.GetName() );
	csv.Add ( PRESET_STOCKAUDIT_VERSION );
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
	csv.Add ( m_bStockRange );
	csv.Add ( m_strStockFrom );
	csv.Add ( m_strStockTo );
	csv.Add ( m_bOnePerPage );
	csv.Add ( m_strHeaderReference );
	csv.Add ( m_strLineReference );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayStockAudit::CPresetReportArrayStockAudit(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayStockAudit::~CPresetReportArrayStockAudit(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayStockAudit::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepStockAudit, 0, 0 );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayStockAudit::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoStockAudit infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayStockAudit::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoStockAudit infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
