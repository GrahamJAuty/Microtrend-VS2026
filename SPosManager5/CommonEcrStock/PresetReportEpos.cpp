/**********************************************************************/
#include "PresetReportEpos.h"
/**********************************************************************/

CPresetReportInfoEpos::CPresetReportInfoEpos()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoEpos::Reset()
{
	m_nBatchNo = 0;
	m_nReportType = 0;
	
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_strSessionName = "";
	m_nPluTimeType = 0;
	
	m_nPMSConsolidateType = 0;
	m_bPMSPending = FALSE;
	m_bPMSActive = FALSE;
	m_bPMSComplete = FALSE;
	m_bPMSCancelled = FALSE;

	m_nBlockType = 0;

	m_nDepartmentFilter = 0;
	m_nPluFilterNo = 0;
	m_bItemServer = FALSE;

	m_nTermDateType = 0;
}

/**********************************************************************/

void CPresetReportInfoEpos::SetBatchNo(int n)
{
	if ((n >= 0) && (n <= 999))
	{
		m_nBatchNo = n;
	}
}

/**********************************************************************/

void CPresetReportInfoEpos::ToString(CString& str)
{
	CCSV csv;
	csv.Add(m_nBatchNo);
	csv.Add(m_nReportType);
	csv.Add(m_strDateType);
	csv.Add(m_strDateFrom);
	csv.Add(m_strDateTo);
	csv.Add(m_strSessionName);
	csv.Add(m_nPluTimeType);
	csv.Add(m_nPMSConsolidateType);
	csv.Add(m_bPMSPending);
	csv.Add(m_bPMSActive);
	csv.Add(m_bPMSComplete);
	csv.Add(m_bPMSCancelled);
	csv.Add(m_nBlockType);
	csv.Add(m_nDepartmentFilter);
	csv.Add(m_nPluFilterNo);
	csv.Add(m_bItemServer);
	csv.Add(m_nTermDateType);
	str = csv.GetLine();
}

/**********************************************************************/

void CPresetReportInfoEpos::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;
	m_nBatchNo = csv.GetInt(nPos++);
	m_nReportType = csv.GetInt(nPos++);
	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_strSessionName = csv.GetString(nPos++);
	m_nPluTimeType = csv.GetInt(nPos++);
	m_nPMSConsolidateType = csv.GetInt(nPos++);
	m_bPMSPending = csv.GetBool(nPos++);
	m_bPMSActive = csv.GetBool(nPos++);
	m_bPMSComplete = csv.GetBool(nPos++);
	m_bPMSCancelled = csv.GetBool(nPos++);
	m_nBlockType = csv.GetInt(nPos++);
	m_nDepartmentFilter = csv.GetInt(nPos++);
	m_nPluFilterNo = csv.GetInt(nPos++);
	m_bItemServer = csv.GetBool(nPos++);
	m_nTermDateType = csv.GetInt(nPos++);
}

/**********************************************************************/

void CPresetReportInfoEpos::ConvertFromCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	Reset();
	infoBase.Reset();
	infoBase.SetName(csv.GetString(0));

	switch (csv.GetInt(1))
	{
	case 1:	V1To3ConvertFromCSV(csv, infoBase, 1);	break;
	case 2:	V1To3ConvertFromCSV(csv, infoBase, 2);	break;
	case 3:	V1To3ConvertFromCSV(csv, infoBase, 3);	break;
	}
}

/**********************************************************************/

void CPresetReportInfoEpos::V1To3ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer )
{
	int nPos = 2;
	infoBase.SetEntityNo( csv.GetInt( nPos++ ) );
		
	SetBatchNo( csv.GetInt( nPos++ ) );
	SetReportType( csv.GetInt(nPos++) );

	SetDateType( csv.GetString(nPos++) );
	SetDateFrom( csv.GetString(nPos++) );
	SetDateTo( csv.GetString(nPos++) );
	SetSessionName( csv.GetString(nPos++) );
	SetPluTimeType( csv.GetInt(nPos++) );

	SetPMSConsolidateType( csv.GetInt(nPos++) );
	SetPMSPendingFlag( csv.GetBool(nPos++) );
	SetPMSActiveFlag( csv.GetBool(nPos++) );
	SetPMSCompleteFlag( csv.GetBool(nPos++) );
	SetPMSCancelledFlag( csv.GetBool(nPos++) );

	SetBlockType( csv.GetInt(nPos++) );

	if ( nVer >= 2 )
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

		int nEntityNo1 = 0;
		int nEntityNo2 = 0;
		CPresetReportArray::GetLegacyConLevelInfo( infoBase.GetPresetConLevel(), nLegacyIdx, nEntityNo1, nEntityNo2 );
		infoBase.SetPresetEntityNo1( nEntityNo1 );
		infoBase.SetPresetEntityNo2( nEntityNo2 );
	}
		
	SetDepartmentFilter( csv.GetInt(nPos++) );
	SetPluFilterNo( csv.GetInt(nPos++) );
	SetItemServerFlag( csv.GetBool(nPos++) );

	SetTermDateType(csv.GetInt(nPos++));
}

/**********************************************************************/

void CPresetReportInfoEpos::ConvertToCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	csv.Add(infoBase.GetName());
	csv.Add(PRESET_EPOS_VERSION);
	csv.Add(infoBase.GetEntityNo());
	csv.Add(GetBatchNo());
	csv.Add(GetReportType());
	csv.Add(GetDateType());
	csv.Add(GetDateFrom());
	csv.Add(GetDateTo());
	csv.Add(GetSessionName());
	csv.Add(GetPluTimeType());
	csv.Add(GetPMSConsolidateType());
	csv.Add(GetPMSPendingFlag());
	csv.Add(GetPMSActiveFlag());
	csv.Add(GetPMSCompleteFlag());
	csv.Add(GetPMSCancelledFlag());
	csv.Add(GetBlockType());
	csv.Add(infoBase.GetPresetConLevel());
	csv.Add(infoBase.GetPresetEntityNo1());
	csv.Add(infoBase.GetPresetEntityNo2());
	csv.Add(infoBase.GetTerminalIdx());
	csv.Add(infoBase.GetPasswordEnableFlag());
	csv.Add(infoBase.GetPasswordConLevel());
	csv.Add(infoBase.GetPasswordEntityNo1());
	csv.Add(infoBase.GetPasswordEntityNo2());
	csv.Add(GetDepartmentFilter());
	csv.Add(GetPluFilterNo());
	csv.Add(GetItemServerFlag());
	csv.Add(GetTermDateType());
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayEpos::CPresetReportArrayEpos(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayEpos::~CPresetReportArrayEpos(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayEpos::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepEpos );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayEpos::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoEpos infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayEpos::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoEpos infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
