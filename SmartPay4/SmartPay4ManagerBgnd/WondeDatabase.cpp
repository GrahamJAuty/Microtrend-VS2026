//*****************************************************************************
#include "WondeDatabase.h"
//*****************************************************************************

static const char* szHEADER		= "MIS_ID,Type,AdmissionNumber,PreferredFullName,LegalForename,LegalSurname,DOB,UPN,YearGroup,RegGroup,House,Gender,FSM,DietaryNeeds,AdmissionDate,LeavingDate,Email,Email2";
static const char* szSORTFIELD	= "MIS_ID,ascending";
static int nRETRIES				= 10;

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CWondeDatabase::Open ( const char* szFilename, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRETRIES );

	return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
}	

/**********************************************************************/
// CardNo = database key

const char* CWondeDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, 16, '0' );
	return m_strKey;
}

/**********************************************************************/

int CWondeDatabase::GetPercentPosition()
{
	return  ( GetCurrentRow() * 100L ) / GetRecordCount();
}

//******************************************************************

int CWondeDatabase::GetTypeNo()
{
	CString strType = GetType();

	if ( strType == "Pupil"	)	return nWONDETYPE_PUPIL;
	if ( strType == "Staff"	)	return nWONDETYPE_STAFF;

	return nWONDETYPE_NONE;
}

//******************************************************************

bool CWondeDatabase::IsFSM()
{
	CString strFSM = GetFSM();

	return ( strFSM.Left(1) == "T" ) ? TRUE : FALSE;
}

//******************************************************************
// "DOB": "yyyy-MM-dd"
// "AdmissionDate": "yyyy-MM-dd"
// "LeavingDate": "yyyy-MM-dd"

void CWondeDatabase::SetDOB ( const char* szYYYYMMDD )
{
	CString strDate = GetDate ( szYYYYMMDD );
	CSSDatabase::Set ( DBWonde::DOB.Label, strDate );
}

void CWondeDatabase::SetInDate ( const char* szYYYYMMDD )
{
	CString strDate = GetDate ( szYYYYMMDD );
	CSSDatabase::Set ( DBWonde::InDate.Label, strDate );
}

void CWondeDatabase::SetOutDate ( const char* szYYYYMMDD )
{
	CString strDate = GetDate ( szYYYYMMDD );
	CSSDatabase::Set ( DBWonde::OutDate.Label, strDate );
}

CString CWondeDatabase::GetDate ( const char* szYYYYMMDD )
{
	CString strDate = "";
	CString strWondeDate = szYYYYMMDD;

	if ( strWondeDate.GetLength() == 10 && strWondeDate != "yyyy-MM-dd" )
	{
		if ( strWondeDate.Mid(4,1) == '-' )
		{
			strDate  = strWondeDate.Right(2);
			strDate += '/';
			strDate += strWondeDate.Mid(5,2);
			strDate += '/';
			strDate += strWondeDate.Left(4);
		}
	}

	return strDate;
}

//*****************************************************************************

void CWondeDatabase::SetDietaryNeed ( const char* szText )
{
	CCSV csv ( GetDietaryNeeds() );
	csv.Add ( szText );
	CSSDatabase::Set ( DBWonde::DietaryNeeds.Label, csv.GetLine() );
}

//*****************************************************************************

