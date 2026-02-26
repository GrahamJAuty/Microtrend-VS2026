//**********************************************************************
#include "FamilyDatabase.h"
//**********************************************************************

//static const char* szHEADER	= "Forename,Surname,DOB,Gender,Added";
static const char* szHEADER_V2	= "Forename,Surname,DOB,Gender,MType,Added";
static const char* szSORTFIELD	= "Forename,ascending";

//**********************************************************************

CFamilyDatabase::CFamilyDatabase()
{
	m_strError = "";
	m_strKey = "";
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

bool CFamilyDatabase::OpenForSharing ( const char* szPathname, int nMode )
{
	int nRetries = 10;

	if ( nMode == DB_READWRITE )
	{
		CreateSSDatabase( szPathname, "", szHEADER_V2, szSORTFIELD, nRetries );		// create db if not found
		Close();
	}

	int nReply;
	if (nMode == DB_READONLY)
	{
		nReply = CSSStringDatabase::OpenSSDatabaseReadOnly(szPathname, szSORTFIELD, nRetries);
	}
	else
	{
		if ( ( nReply = CSSStringDatabase::OpenSSDatabaseReadWrite ( szPathname, "", szSORTFIELD, nRetries ) ) == DB_ERR_NONE )
		{
			int nIndex = GetHeaderIndex ( FamilyDatabase::MemberType.Label );		// check for latest header
			if ( nIndex == -1 )													// not found	
			{
				CSSStringDatabase::Close();										// close file
				nReply = CSSStringDatabase::OpenSSDatabaseReadWriteNewHeader ( szPathname, "", szHEADER_V2, "Forename", nRetries );// read and write and modify header (file must exist)
			}
		}
	}

	if (nReply == DB_ERR_NONE)
	{
		return TRUE;
	}

	m_strError.Format ( "Database Error Detected!\n\n%s.\n\n%s.", szPathname, CSSStringDatabase::GetError() );
	return FALSE;
}	

/**********************************************************************/

const char* CFamilyDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	m_strKey.MakeUpper();
	return m_strKey;
}

/**********************************************************************/

void CFamilyDatabase::SetDateAdded ( const char* szDate )
{
	CString strDate = szDate;

	CSSDate date;
	if ( strDate == "" )	date.SetCurrentDate();
	else					date.SetDate ( strDate );

	CSSDatabase::Set ( FamilyDatabase::AddedDate.Label, date.GetDate() );
}

/**********************************************************************/

//void CFamilyDatabase::SetLastUsedDate()
//{
//	CSSDate date;	date.SetCurrentDate();
//	CSSDatabase::Set ( FamilyDatabase::LastUsedDate.Label, date.GetDate() );
//}

//void CFamilyDatabase::SetLastUsedTime()
//{
//	CSSTime time;		time.SetCurrentTime();
//	CSSDatabase::Set ( FamilyDatabase::LastUsedTime.Label, time.GetTime() );
//}

/**********************************************************************/

void CFamilyDatabase::SetDateField ( const char* szLabel, const char* szDate )
{
	CString strDate = szDate;
	if (strDate == " ")									//v2.03x 	05/01/2016	Setting a Date \ Time field with a space will clear it
	{
		strDate = "";
	}

	bool bOk = TRUE;
	if ( strDate.GetLength() > 0 )
	{
		CSSDate date ( strDate );
		bOk = date.IsSet();
	}
	
	if (bOk == TRUE)
	{
		CSSDatabase::Set(szLabel, strDate);
	}
}

/**********************************************************************/

void CFamilyDatabase::SetTimeField ( const char* szLabel, const char* szTime )
{
	CString strTime = szTime;
	if (strTime == " ")									//v2.03x 	05/01/2016	Setting a Date \ Time field with a space will clear it
	{
		strTime = "";
	}

	bool bOk = TRUE;
	if ( strTime.GetLength() > 0 )
	{
		CSSTime time ( strTime );
		bOk = time.IsSet();
	}
	
	if (bOk == TRUE)
	{
		CSSDatabase::Set(szLabel, strTime);
	}
}

/**********************************************************************/

//void CFamilyDatabase::IncrementVisits()
//{
//	int nVisits = GetVisitCount();							// get current setting
//
//	CSSDatabase::Set ( FamilyDatabase::Visits.Label, ++nVisits );
//}

/**********************************************************************/
//
//int CBiometricRegister::GetPercentPosition()
//{
//	return  ( GetCurrentRow() * 100L ) / GetRecordCount();
//}
//
//*******************************************************************

void CFamilyDatabase::SetGender ( int nGender )
{
	CString strGender = "";								// set to unknown

	switch (nGender)
	{
	case 1:
		strGender = "M";
		break;

	case 2:
		strGender = "F";
		break;
	}

	CSSDatabase::Set ( FamilyDatabase::Gender.Label, strGender );
}

//*******************************************************************

void CFamilyDatabase::SetGender ( const char* szGender )
{
	CString strGender = szGender;
	
	strGender = strGender.Left(1);
	strGender.MakeUpper();

	if ( strGender == "M" )	
	{
		SetGender(1);
	}
	else if ( strGender == "F" )	
	{
		SetGender(2);
	}
	else
	{
		SetGender(0);
	}
}

//*******************************************************************

CString CFamilyDatabase::GetGenderText()
{
	CString strTest = GetGender();
	CString strGender = "";

	if (strTest == "M")
	{
		strGender = "Male";
	}
	else if (strTest == "F")
	{
		strGender = "Female";
	}
	else
	{
		strGender = "Unknown";
	}

	return strGender;
}

//********************************************************************

CString CFamilyDatabase::GetAgeText()
{
	CString strBuf = "";
	strBuf.Format ( "%d", GetAge() );
	return strBuf;
}

//********************************************************************

int CFamilyDatabase::GetAge()
{
	CSSDate dob ( GetDobDate() );

	if (dob.IsSet() == FALSE)
	{
		return -1;
	}

	CSSDate date;	date.SetCurrentDate();

	int nYear1 = dob.GetYear();						// birthday year
	int nYear2 = date.GetYear();					// todays year

	int nAge = nYear2 - nYear1;						// age next birthday

	if (dob.GetMonth() > date.GetMonth())			// month not yet reached his year
	{
		--nAge;
	}

	else if ( dob.GetMonth() == date.GetMonth() )	// birthday in this month
	{
		if (dob.GetDay() > date.GetDay())			// day of month not yet reached
		{
			--nAge;
		}
	}

	return nAge;
}

//********************************************************************

void CFamilyDatabase::CopyToDatabase ( CFamilyDatabase* pDbCopy )
{
	CString strKey = GetForename();
	bool bReply = pDbCopy->AddBlankRecord ( strKey );

	CString strLabel, strData;

	for ( int n = 0 ; n < GetHeaderSize() ; n++ )
	{
		strLabel = GetHeaderLabel ( n );

		strData = GetString ( strLabel );
		pDbCopy->Set ( strLabel, strData );
	}
}

//**************************************************************

CString CFamilyDatabase::GetMemberTypeText()
{
	CString strText = "Unknown";

	switch ( GetMemberType() )
	{
		case 0:	strText = "Standard";	break;
		case 1:	strText = "S.Needs";	break;
		case 2:	strText = "Free";		break;
	}

	return strText;
}

//**************************************************************
