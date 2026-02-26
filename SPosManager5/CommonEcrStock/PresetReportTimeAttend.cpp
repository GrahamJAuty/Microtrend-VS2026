/**********************************************************************/
#include "PresetReportTimeAttend.h"
/**********************************************************************/

CPresetReportInfoTimeAttend::CPresetReportInfoTimeAttend()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoTimeAttend::Reset()
{
	m_nReportType = 0;
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_nRoundToType = 0;
	m_strSessionName = "";
	m_nGroupBy = 0;
	m_nEditMode = 0;
	m_nServerPickType = 0;
	m_nServerStart = 1;
	m_nServerEnd = 9999;
	m_bClosingTime = FALSE;
	m_bOpenShift = FALSE;
	m_bEventsBySecond = FALSE;
}

/**********************************************************************/

void CPresetReportInfoTimeAttend::ToString(CString& str)
{
	CCSV csv;

	csv.Add(m_nReportType);
	csv.Add(m_strDateType);
	csv.Add(m_strDateFrom);
	csv.Add(m_strDateTo);
	csv.Add(m_nRoundToType);
	csv.Add(m_strSessionName);
	csv.Add(m_nGroupBy);
	csv.Add(m_nEditMode);
	csv.Add(m_nServerPickType);
	csv.Add(m_nServerStart);
	csv.Add(m_nServerEnd);
	csv.Add(m_bClosingTime);
	csv.Add(m_bEventsBySecond);
	csv.Add(m_bOpenShift);

	str = csv.GetLine();
}


/**********************************************************************/

void CPresetReportInfoTimeAttend::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;

	m_nReportType = csv.GetInt(nPos++);
	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_nRoundToType = csv.GetInt(nPos++);
	m_strSessionName = csv.GetString(nPos++);
	m_nGroupBy = csv.GetInt(nPos++);
	m_nEditMode = csv.GetInt(nPos++);
	m_nServerPickType = csv.GetInt(nPos++);
	m_nServerStart = csv.GetInt(nPos++);
	m_nServerEnd = csv.GetInt(nPos++);
	m_bClosingTime = csv.GetBool(nPos++);
	m_bEventsBySecond = csv.GetBool(nPos++);
	m_bOpenShift = csv.GetBool(nPos++);
}

/**********************************************************************/

void CPresetReportInfoTimeAttend::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	Reset();
	infoBase.Reset();
	infoBase.SetName ( csv.GetString (0) );
	
	switch (csv.GetInt(1))
	{
	case 1:	V1To3ConvertFromCSV(csv, infoBase, 1);	break;
	case 2:	V1To3ConvertFromCSV(csv, infoBase, 2);	break;
	case 3:	V1To3ConvertFromCSV(csv, infoBase, 3);	break;
	}
}

/**********************************************************************/

void CPresetReportInfoTimeAttend::V1To3ConvertFromCSV(CCSV& csv, CPresetReportInfo& infoBase, int nVer)
{
	int nPos = 2;

	infoBase.SetEntityNo(csv.GetInt(nPos++));
	SetReportType(csv.GetInt(nPos++));
	SetDateType(csv.GetString(nPos++));
	SetDateFrom(csv.GetString(nPos++));
	SetDateTo(csv.GetString(nPos++));
	SetRoundToType(csv.GetInt(nPos++));
	SetSessionName(csv.GetString(nPos++));

	infoBase.SetPresetConLevel(csv.GetInt(nPos++));
	infoBase.SetPresetEntityNo1(csv.GetInt(nPos++));
	infoBase.SetPresetEntityNo2(csv.GetInt(nPos++));
	infoBase.SetPasswordEnableFlag(csv.GetBool(nPos++));
	infoBase.SetPasswordConLevel(csv.GetInt(nPos++));
	infoBase.SetPasswordEntityNo1(csv.GetInt(nPos++));
	infoBase.SetPasswordEntityNo2(csv.GetInt(nPos++));

	SetGroupBy(csv.GetInt(nPos++));
	SetEditMode(csv.GetInt(nPos++));
	SetServerPickType(csv.GetInt(nPos++));
	SetServerStart(csv.GetInt(nPos++));
	SetServerEnd(csv.GetInt(nPos++));

	if (nVer >= 2)
	{
		SetClosingTimeFlag(csv.GetBool(nPos++));
		SetEventsBySecondFlag(csv.GetBool(nPos++));
	}

	if (nVer >= 3)
	{
		SetOpenShiftFlag(csv.GetBool(nPos++));
	}
}

/**********************************************************************/

void CPresetReportInfoTimeAttend::ConvertToCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	csv.Add(infoBase.GetName());
	csv.Add(PRESET_TIMEATTEND_VERSION);
	csv.Add(infoBase.GetEntityNo());
	csv.Add(m_nReportType);
	csv.Add(m_strDateType);
	csv.Add(m_strDateFrom);
	csv.Add(m_strDateTo);
	csv.Add(GetRoundToType());
	csv.Add(GetSessionName());
	csv.Add(infoBase.GetPresetConLevel());
	csv.Add(infoBase.GetPresetEntityNo1());
	csv.Add(infoBase.GetPresetEntityNo2());
	csv.Add(infoBase.GetPasswordEnableFlag());
	csv.Add(infoBase.GetPasswordConLevel());
	csv.Add(infoBase.GetPasswordEntityNo1());
	csv.Add(infoBase.GetPasswordEntityNo2());
	csv.Add(m_nGroupBy);
	csv.Add(m_nEditMode);
	csv.Add(m_nServerPickType);
	csv.Add(m_nServerStart);
	csv.Add(m_nServerEnd);
	csv.Add(m_bClosingTime);
	csv.Add(m_bEventsBySecond);
	csv.Add(m_bOpenShift);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayTimeAttend::CPresetReportArrayTimeAttend(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayTimeAttend::~CPresetReportArrayTimeAttend(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayTimeAttend::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepTimeAttend );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayTimeAttend::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoTimeAttend infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayTimeAttend::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoTimeAttend infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
