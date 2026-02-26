/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSRoomCSVArray.h"
/**********************************************************************/

CPMSRoomCSVRecord::CPMSRoomCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPMSRoomCSVRecord::ClearRecord()
{
	m_nRoomNo = 1;
	m_strRoomName = "";
	m_nSleeps = 1;
	m_nRoomType = 0;
	m_nRoomGroup = 1;
	m_bRoomSingleRate = FALSE;
	m_dRoomOnlyRate = 0.0;
	m_dRoomBBRate = 0.0;
	m_dRoomDBBRate = 0.0;
	m_dRoomOnlySingleRate = 0.0;
	m_dRoomBBSingleRate = 0.0;
	m_dRoomDBBSingleRate = 0.0;
	m_bRoomAllowExtraBed = FALSE;
	m_dRoomExtraBedRate = 0.0;
	m_bRoomAllowExtraCot = FALSE;
	m_dRoomExtraCotRate = 0.0;
}

/**********************************************************************/
/*	convert to csv											*/
/**********************************************************************/

void CPMSRoomCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nRoomNo );
	csv.Add ( ROOMDB_VERSION );
	csv.Add ( m_strRoomName );
	csv.Add ( m_nSleeps );
	csv.Add ( m_nRoomType );
	csv.Add ( m_nRoomGroup );
	csv.Add ( m_bRoomSingleRate );
	csv.Add ( m_dRoomOnlyRate, 2 );
	csv.Add ( m_dRoomBBRate, 2 );
	csv.Add ( m_dRoomDBBRate, 2 );
	csv.Add ( m_dRoomOnlySingleRate, 2 );
	csv.Add ( m_dRoomBBSingleRate, 2 );
	csv.Add ( m_dRoomDBBSingleRate, 2 );
	csv.Add ( m_bRoomAllowExtraBed );
	csv.Add	( m_dRoomExtraBedRate, 2 );
	csv.Add ( m_bRoomAllowExtraCot );
	csv.Add	( m_dRoomExtraCotRate, 2 );
}

/**********************************************************************/
/*	convert from csv												*/
/*																	*/
/*	For future compatibility, if count not stored in file:			*/
/*																	*/
/*	1) Cannot use defines											*/
/*	2) Cannot make arrays smaller than previous versions			*/
/*																	*/
/**********************************************************************/

void CPMSRoomCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetRoomNo ( csv.GetInt(0) );
	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1To5ConvertFromCSV ( csv, 1 );	break;
	case 2:	V1To5ConvertFromCSV ( csv, 2 );	break;
	case 3:	V1To5ConvertFromCSV ( csv, 3 );	break;
	case 4:	V1To5ConvertFromCSV ( csv, 4 );	break;
	case 5:	V1To5ConvertFromCSV ( csv, 5 );	break;
	}
}

/**********************************************************************/

void CPMSRoomCSVRecord::V1To5ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nPos = 2;
	
	SetRoomName ( csv.GetString(nPos++) );
	SetSleeps ( csv.GetInt(nPos++) );
	SetRoomType( csv.GetInt(nPos++) );

	if ( nVer >= 5 )
		SetRoomGroup( csv.GetInt(nPos++) );

	if ( nVer >= 3 )
		SetRoomSingleRateFlag( csv.GetBool(nPos++) );
	
	if ( nVer >= 2 )
	{
		SetRoomOnlyRate( csv.GetDouble(nPos++) );
		SetRoomBBRate( csv.GetDouble(nPos++) );
		SetRoomDBBRate( csv.GetDouble(nPos++) );
	}

	if ( nVer >= 3 )
	{
		SetRoomOnlySingleRate( csv.GetDouble(nPos++) );
		SetRoomBBSingleRate( csv.GetDouble(nPos++) );
		SetRoomDBBSingleRate( csv.GetDouble(nPos++) );
	}

	if ( nVer >= 4 )
	{
		SetRoomAllowExtraBedFlag( csv.GetBool(nPos++) );
		SetRoomExtraBedRate( csv.GetDouble(nPos++) );
		SetRoomAllowExtraCotFlag( csv.GetBool(nPos++) );
		SetRoomExtraCotRate( csv.GetDouble(nPos++) );
	}
}
	
/**********************************************************************/

CString CPMSRoomCSVRecord::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

const char* CPMSRoomCSVRecord::GetDefaultName()
{
	m_strDefaultName.Format ( "Room %d", m_nRoomNo );
	return m_strDefaultName;
}

/**********************************************************************/

const char* CPMSRoomCSVRecord::GetRoomTypeName( bool bBrackets )
{
	CString strType = DataManagerNonDb.PMSRoomType.GetText( m_nSleeps, m_nRoomType );
	::TrimSpaces( strType, FALSE );

	if ( strType == "" )
		m_strRoomTypeName = "";
	else
	{
		if ( TRUE == bBrackets )
		{
			m_strRoomTypeName = "(";
			m_strRoomTypeName += strType;
			m_strRoomTypeName += ")";
		}
		else
			m_strRoomTypeName = strType;
	}

	return m_strRoomTypeName;
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomNo ( int n )
{
	Set ( m_nRoomNo, n, PMSRoom::RoomNo );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomName ( const char* sz )
{
	CString str = StripSpaces( sz );

	if ( str.GetLength() > PMSRoom::RoomName.Max )
		str = str.Left( PMSRoom::RoomName.Max );

	m_strRoomName = str;
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetSleeps ( int n )
{
	Set ( m_nSleeps, n, PMSRoom::Sleeps );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomType ( int n )
{
	Set ( m_nRoomType, n, PMSRoom::RoomType );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomGroup ( int n )
{
	Set ( m_nRoomGroup, n, PMSRoom::RoomGroup );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomOnlyRate( double d )
{
	Set ( m_dRoomOnlyRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomBBRate( double d )
{
	Set ( m_dRoomBBRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomDBBRate( double d )
{
	Set ( m_dRoomDBBRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomOnlySingleRate( double d )
{
	Set ( m_dRoomOnlySingleRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomBBSingleRate( double d )
{
	Set ( m_dRoomBBSingleRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomDBBSingleRate( double d )
{
	Set ( m_dRoomDBBSingleRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomExtraBedRate( double d )
{
	Set ( m_dRoomExtraBedRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSRoomCSVRecord::SetRoomExtraCotRate( double d )
{
	Set ( m_dRoomExtraCotRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/
#endif
/**********************************************************************/
