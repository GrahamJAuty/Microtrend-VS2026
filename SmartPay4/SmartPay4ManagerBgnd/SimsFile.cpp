//#include "GlobalFunctions.h"
#include "SimsFile.h"

//*********************************************************************

CSimsFile::CSimsFile(void)
{
	m_bFileOpen			= FALSE;
	m_strError			= "";

	m_bSIMSRequired		= FALSE;			//
	m_bUPNRequired		= FALSE;			// set if must exist in header line
	m_bPIDRequired		= FALSE;			//

	m_nIndexPID			= -1;				// Person_ID
	m_nIndexUPN			= -1;				// Unique Pupil Number
	m_nIndexAdno		= -1;				// SIMS Admission Number
	m_nIndexForename	= -1;
	m_nIndexSurname		= -1;
	m_nIndexCardName	= -1;
	m_nIndexDOB			= -1;
	m_nIndexYear		= -1;
	m_nIndexReg			= -1;
	m_nIndexEmail		= -1;
	m_nIndexGender		= -1;
	m_nIndexFreeMeal	= -1;

	m_strUPN			= "";
	m_strPID			= "";
	m_strAdno			= "";
	m_strForename		= "";
	m_strSurname		= "";
	m_strCardName		= "";
	m_strDOB			= "";
	m_strYear			= "";
	m_strReg			= "";
	m_strEmail			= "";
	m_strFreeMeal		= "";
	m_strGender			= "";
}

CSimsFile::~CSimsFile(void)
{
	Close();
}

//*********************************************************************

bool CSimsFile::Open ( const char* szFilename )
{
	if ( ( m_bFileOpen = m_file.Open ( szFilename, "rb" ) ) == FALSE )
	{
		m_strError = "Cannot open SIMS import file for reading";
		return FALSE;
	}

	CCSV csv;
	if ( m_file.Read ( csv ) == TRUE )				// read first line (header line)
	{
		CString strFieldName;

		for ( int nIndex = 0 ; nIndex < csv.GetSize() ; nIndex++ )
		{
			strFieldName = csv.GetString(nIndex);

			if ( strFieldName.CompareNoCase("person_id") == 0 )
				m_nIndexPID = nIndex;

			else if ( strFieldName.CompareNoCase("upn") == 0 )
				m_nIndexUPN = nIndex;

			else if ( strFieldName.CompareNoCase("adno") == 0 )
				m_nIndexAdno = nIndex;

			else if ( strFieldName.CompareNoCase("forename") == 0 )
				m_nIndexForename = nIndex;

			else if ( strFieldName.CompareNoCase("legal surname") == 0 )
				m_nIndexSurname = nIndex;

			else if ( strFieldName.CompareNoCase("name") == 0 )
				m_nIndexCardName = nIndex;

			else if ( strFieldName.CompareNoCase("dob") == 0 )
				m_nIndexDOB = nIndex;

			else if ( strFieldName.CompareNoCase("year") == 0 )
				m_nIndexYear = nIndex;

			else if ( strFieldName.CompareNoCase("reg") == 0 )
				m_nIndexReg = nIndex;

			else if ( strFieldName.CompareNoCase("email_address") == 0 )
				m_nIndexEmail = nIndex;

			else if ( strFieldName.CompareNoCase("primary email") == 0 )
			{
				if ( m_nIndexEmail == -1 )								// see if already set
					m_nIndexEmail = nIndex;
			}

			else if ( strFieldName.CompareNoCase("eligible for free meals") == 0 )
				m_nIndexFreeMeal = nIndex;

			else if ( strFieldName.CompareNoCase("gender") == 0 )
				m_nIndexGender = nIndex;
		}
	}

	if ( ( m_nIndexUPN == -1 && m_bUPNRequired == TRUE ) || ( m_nIndexAdno == -1 && m_bSIMSRequired == TRUE ) || ( m_nIndexPID == -1 && m_bPIDRequired == TRUE ) )
	{
		m_strError = "Missing field in import file header";
			
		if ( m_nIndexUPN == -1 )	m_strError += "\n\nUPN";
		if ( m_nIndexAdno == -1 )	m_strError += "\n\nSIMS Adno";
		if ( m_nIndexPID == -1 )	m_strError += "\n\nPerson_id";
		return FALSE;
	}
	
	return TRUE;
}

//*********************************************************************

void CSimsFile::Close()
{
	if ( m_bFileOpen == TRUE )
		m_file.Close();
}

//*********************************************************************

int CSimsFile::GetPercentPosition()
{
	return (int)( ( m_file.Tell() * 100L ) / m_file.GetLength() );
}

//*******************************************************************

bool CSimsFile::ReadLine()
{
	while ( TRUE )
	{
		CCSV csv;
		if ( m_file.Read ( csv ) == FALSE )
			return FALSE;

		if ( csv.GetSize() > 0 )
		{
			GetTextFromCSVLine( csv, m_nIndexAdno, m_strAdno );

			if ( ::IsStringNumeric ( m_strAdno, 1, 6 ) == TRUE )		// padout numberic adno to 6
			{
				::AddLeading ( m_strAdno, 6, '0' );
			}

			GetTextFromCSVLine( csv, m_nIndexPID, m_strPID );
			GetTextFromCSVLine( csv, m_nIndexUPN, m_strUPN );

			if ( ( m_strAdno == "" ) && ( m_strUPN == "" ) && ( m_strPID == "" ) )
				continue;

			GetTextFromCSVLine( csv, m_nIndexForename, m_strForename );
			GetTextFromCSVLine( csv, m_nIndexSurname, m_strSurname );
			GetTextFromCSVLine( csv, m_nIndexCardName, m_strCardName );
			GetTextFromCSVLine( csv, m_nIndexYear, m_strYear );
			GetTextFromCSVLine( csv, m_nIndexReg, m_strReg );
			GetTextFromCSVLine( csv, m_nIndexEmail, m_strEmail );
			GetTextFromCSVLine( csv, m_nIndexFreeMeal, m_strFreeMeal );
			GetTextFromCSVLine( csv, m_nIndexGender, m_strGender );

			if ( m_nIndexDOB == -1 )
			{
				m_strDOB = "";
			}
			else
			{
				CString strTemp = csv.GetString(m_nIndexDOB);
				::TrimSpacesFromString( strTemp );
				m_strDOB = ::ExtractDate( strTemp );
			}

			return TRUE;
		}
	}
}

//*******************************************************************

void CSimsFile::GetTextFromCSVLine( CCSV& csv, int nIndex, CString& strText )
{
	if ( nIndex == -1 )
	{
		strText = "";
	}
	else
	{
		strText = csv.GetString(nIndex);
		::TrimSpacesFromString( strText );
	}
}

//*******************************************************************


