/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

static const char* szDAYS[]  = { "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static const char* szMONTHS[13] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static const char* szSORTFIELD = "UserID,ascending";
static int nRETRIES = 10;

/**********************************************************************/

CSmartPayDatabase::CSmartPayDatabase()
{
	m_strKey = "";
	m_strError = ""; 
	m_strInuseFilename = "";
	CreateHeaders();
}

//*********************************************************************

bool CSmartPayDatabase::IsAvailable ( const char* szFilename )
{
	CString strInuseFile = MakeInuseFilename ( szFilename );

	CSSFile file;
	if ( file.Open ( strInuseFile, "wb", _SH_DENYWR, 0 ) == TRUE )		// no retries on opening
	{
		file.Close();
		::remove ( strInuseFile );
		return TRUE;
	}
	return FALSE;
}

//*******************************************************************
// C:\Users\Public\SharpSoft\Ecrx500v3(inc SmartPay)\P33\SysFiles\file0002.dat

CString CSmartPayDatabase::MakeInuseFilename ( const char* szFilename )
{
	CString strReply = "";

	CString strFullPathname	= szFilename;
	strFullPathname.MakeLower();

	if ( strFullPathname.Right(4) == ".dat" )
	{
		int nLength = strFullPathname.GetLength();
		strReply = strFullPathname.Left ( nLength - 4 );
		strReply += ".run";
	}
	return strReply;
}

/******************************************************************************/
// Filename - MUST end in .dat as it gets changed when opened in Read\Write mode
//	mode = DB_READONLY, DB_READWRITE

int CSmartPayDatabase::OpenReadOnlyForSharing( const char* szFilename )
{
	return OpenForSharing( szFilename, "", DB_READONLY );
}

/******************************************************************************/

int CSmartPayDatabase::OpenReadWriteForSharing( const char* szFilename, const char* szBackupFolder )
{
	return OpenForSharing( szFilename, szBackupFolder, DB_READWRITE );
}

/******************************************************************************/

int CSmartPayDatabase::OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode )
{
	m_strDatabaseFilename = szFilename;

	CSSStringDatabase::CreateSSDatabase( m_strDatabaseFilename, szBackupFolder, m_strHeader, szSORTFIELD, nRETRIES );	// create db if not found
	CSSStringDatabase::Close();

	return ReOpenForSharing ( szBackupFolder, nMode );											// open file
}	

/******************************************************************************/

int CSmartPayDatabase::ReOpenReadOnlyForSharing()
{
	return ReOpenForSharing( "", DB_READONLY );
}

/******************************************************************************/

int CSmartPayDatabase::ReOpenReadWriteForSharing( const char* szBackupFolder )
{
	return ReOpenForSharing( szBackupFolder, DB_READWRITE );
}

/******************************************************************************/

int CSmartPayDatabase::ReOpenForSharing ( const char* szBackupFolder, int nMode )
{
	int nReply = DB_ERR_NONE;

	if (nMode == DB_READONLY)
	{
		nReply = CSSStringDatabase::OpenSSDatabaseReadOnly(m_strDatabaseFilename, szSORTFIELD, nRETRIES);
	}
	else
	{
		if ( ( nReply = CSSStringDatabase::OpenSSDatabaseReadWrite ( m_strDatabaseFilename, szBackupFolder, szSORTFIELD, nRETRIES ) ) == DB_ERR_NONE )
		{
			//CHECK FOR OLD HEADER
			bool bOldHeader = FALSE;
			{
				int nIndex = GetHeaderIndex(SmartPay::Purse1Credit.Label);		
				if (nIndex == -1)												
				{
					bOldHeader = TRUE;
				}
			}

			if ( FALSE == bOldHeader )
			{
				int nIndex = GetHeaderIndex("CTS");		
				if (nIndex != -1)									
				{
					bOldHeader = TRUE;
				}
			}

			if ( TRUE == bOldHeader )									
			{
				CSSStringDatabase::Close();										
				nReply = CSSStringDatabase::OpenSSDatabaseReadWriteNewHeader ( m_strDatabaseFilename, szBackupFolder, m_strHeader, "UserID", nRETRIES );// read and write and modify header (file must exist)
			}

			if ( nReply == DB_ERR_NONE )
			{
				m_strInuseFilename = MakeInuseFilename ( m_strDatabaseFilename );

				if (m_strInuseFilename != "")
				{
					m_fileInuse.Open(m_strInuseFilename, "wb", _SH_DENYWR);
				}
			}
		}
	}

	if (nReply != DB_ERR_NONE)
	{
		m_strError = CSSStringDatabase::GetError();
	}

	return nReply;
}	

/**********************************************************************/

bool CSmartPayDatabase::Close()
{
	if ( m_strInuseFilename != "" )
	{
		m_fileInuse.Close();
		::remove ( m_strInuseFilename );
	}

	return CSSStringDatabase::Close();
}

/**********************************************************************/

const char* CSmartPayDatabase::GetHeader()
{
	return m_strHeader;
}

/**********************************************************************/

// only used for database export - no Next / Previous / ExtRef fields
const char* CSmartPayDatabase::GetBiometricHeader()
{
	return m_strBioHeader;
}

/**********************************************************************/
// CardNo = database key

const char* CSmartPayDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, SmartPay::ExUserID.Max, '0' );
	return m_strKey;
}

/**********************************************************************/

int CSmartPayDatabase::GetPercentPosition()
{
	int nRecordCount = GetRecordCount();
	return ( nRecordCount <= 0 ) ? 0 : ( GetCurrentRow() * 100L ) / nRecordCount;
}

//*********************************************************************

bool CSmartPayDatabase::IsStringEmpty ( const char* szText )
{
	CString strText = szText;
	strText.TrimLeft ( ' ' );
	if ( strText == "" )
		return TRUE;

	return FALSE;
}

//*********************************************************************

bool CSmartPayDatabase::IsRecordEmpty()
{
	if (IsStringEmpty(GetMemberID()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetExtRef()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetExtRef2()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetUserName()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetForename()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetSurname()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetGender()) == FALSE)				return FALSE;
	if (GetPoints() != 0)										return FALSE;
	if (GetPointsTD() != 0)									return FALSE;
	if (GetCashSpendTD() != 0)								return FALSE;

	if (GetPurse1Liability() != 0)									return FALSE;
	if (GetPurse1Credit() != 0)										return FALSE;
	if (IsStringEmpty(GetPurse1DateLastUsed()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse1TimeLastUsed()) == FALSE)	return FALSE;
	if (GetPurse1LastSpend() != 0)							return FALSE;
	if (GetPurse1SpendTD() != 0)								return FALSE;

	if (GetPurse2Balance() != 0)										return FALSE;
	if (IsStringEmpty(GetPurse2DateLastUsed()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse2TimeLastUsed()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetRefreshedDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetRefreshedTime()) == FALSE)		return FALSE;
	if (GetPurse2LastSpend() != 0)							return FALSE;
	if (GetPurse2SpendTD() != 0)								return FALSE;

	if (GetPurse3Liability() != 0)										return FALSE;
	if (GetPurse3Credit() != 0)									return FALSE;
	if (IsStringEmpty(GetPurse3DateLastUsed()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse3TimeLastUsed()) == FALSE)	return FALSE;
	if (GetPurse3LastSpend() != 0)							return FALSE;
	if (GetPurse3SpendTD() != 0)								return FALSE;

	if (GetOverSpend() != 0)									return FALSE;
	if (GetMaxSpendValue() != 0)								return FALSE;
	if (GetAllergyCodes() != 0) 								return FALSE;
	if (GetAlertCode() != 0)									return FALSE;
	if (IsStringEmpty(GetDietary()) == FALSE)				return FALSE;

	if (IsStringEmpty(GetExpiryDate()) == FALSE)			return FALSE;
	if (IsStringEmpty(GetDobDate()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetDailyGroups()) == FALSE)			return FALSE;

	if (IsStringEmpty(GetEmail()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo1()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo2()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo3()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo4()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo5()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo6()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo7()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo8()) == FALSE)		return FALSE;

	return TRUE;
}

//*********************************************************************

int CSmartPayDatabase::GetGroupNo()
{
	int nGroupNo = CSSDatabase::GetInt ( SmartPay::GroupNo.Label );
	if ( nGroupNo == 0 )
		nGroupNo = 1;

	return nGroupNo;
}

CString CSmartPayDatabase::GetGroupNoString()
{
	CString strMsg;
	strMsg.Format ( "%d", GetGroupNo() );
	return strMsg;
}

//*********************************************************************

CString CSmartPayDatabase::GetNextFreeUserID ( const char* szStartFrom, const char* szMaxID )
{
	int nRecordIndex = GetCurrentRow();					// get current record index

	__int64 nUserID	= _atoi64 ( szStartFrom );
	__int64 nMaxID	= _atoi64 ( szMaxID );

	CString strCardNo = "";
	do
	{
		if ( nUserID == nMaxID )
		{
			strCardNo = "";
			break;
		}

		strCardNo.Format ( "%I64d", nUserID++ );
		if ( FindRecord ( strCardNo ) == FALSE )
			break;
	}
	while ( TRUE );

	MoveTo ( nRecordIndex );							// restore current index pointer

	return strCardNo;
}

CString CSmartPayDatabase::IncrementUserID ( const char* szUserID, const char* szMaxID )
{
	CString strUserID = "";

	__int64 nUserID	= _atoi64 ( szUserID );
	__int64 nMaxID	= _atoi64 ( szMaxID );

	if ( nUserID < nMaxID  )							// see if less than  9999999999
		strUserID.Format ( "%I64d", nUserID + 1 );		// inc user ID number

	return strUserID;
}

//*******************************************************************
//
//__int64 CSmartPayDatabase::GetNextFreeUserID()
//{
//	int nRecordIndex = GetCurrentRow();					// get current record index
//
//	MoveLast();
//
//	__int64 nUserID = _atoi64 ( GetUserID() ) + 1;		// get ID of last record & inc
//
//	MoveTo ( nRecordIndex );							// restore current index pointer
//
//	return nUserID;
//}
//
//
//CString CSmartPayDatabase::IncrementUserID ( __int64 nMaxID )
//{
//	CString strUserID = GetUserID();					// get current ID
//
//	__int64 nUserID = _atoi64 ( strUserID );
//
//	if ( nUserID < nMaxID  )							// see if less than  9999999999
//		strUserID.Format ( "%I64d", nUserID + 1 );		// inc user ID number
//
//	return strUserID;
//}
//
//*********************************************************************

bool CSmartPayDatabase::GetLastUsed ( CSSDate& date, CSSTime& time )
{
	CSSDateTime datePurse1 ( GetPurse1DateLastUsed(), GetPurse1TimeLastUsed() );
	CSSDateTime datePurse2 ( GetPurse2DateLastUsed(), GetPurse2TimeLastUsed() );
	CSSDateTime datePurse3 ( GetPurse3DateLastUsed(), GetPurse3TimeLastUsed() );

	if ( datePurse1.IsSet() == TRUE )						// purse1 used
	{
		if ( datePurse1 >= datePurse2 && datePurse1 >= datePurse3 )
		{
			date.SetDate ( GetPurse1DateLastUsed() );
			time.SetTime ( GetPurse1TimeLastUsed() );
			return TRUE;
		}
	}

	if ( datePurse2.IsSet() == TRUE )
	{
		if ( datePurse2 >= datePurse1 && datePurse2 >= datePurse3 )
		{
			date.SetDate ( GetPurse2DateLastUsed() );
			time.SetTime ( GetPurse2TimeLastUsed() );
			return TRUE;
		}
	}

	if ( datePurse3.IsSet() == TRUE )
	{
		if ( datePurse3 >= datePurse1 && datePurse3 >= datePurse2 )
		{
			date.SetDate ( GetPurse3DateLastUsed() );
			time.SetTime ( GetPurse3TimeLastUsed() );
			return TRUE;
		}
	}

	return FALSE;
}

//*********************************************************************

bool CSmartPayDatabase::IsBirthdayToday()
{
	bool bIsBirthday = FALSE;

	CSSDate dateDob ( GetDobDate() );
	if ( dateDob.IsSet() == TRUE )
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if ( dateDob.GetMonth() == dateNow.GetMonth() )
		{
			if ( dateDob.GetDay() == dateNow.GetDay() )
				bIsBirthday = TRUE;
		}
	}

	return bIsBirthday;
}

const char* CSmartPayDatabase::GetBirthdayMonth()
{
	CSSDate dob ( GetDobDate() );
	return szMONTHS[dob.GetMonth()];
}

/******************************************************************************/
// return age of card holder, -1 = not dob set
// if bNextBDay = true - gets age at next birthday

CString CSmartPayDatabase::GetAgeNextBirthday()
{
	int nAge;
	CString strAge = "";

	if ( ( nAge = GetAge(TRUE) ) != -1 )	strAge.Format("%d", nAge );
	return strAge;
}

int CSmartPayDatabase::GetAge ( bool bNextBDay )
{
	CSSDate dob ( GetDobDate() );

	if ( dob.IsSet() == FALSE )
		return -1;

	CSSDate date;
	date.SetCurrentDate();

	int nYear1 = dob.GetYear();				// birthday year
	int nYear2 = date.GetYear();				// todays year

	int nAge = nYear2 - nYear1;					// age next birthday

	if ( bNextBDay == FALSE )					// adjust for birth not yet this year
	{
		if ( dob.GetMonth() > date.GetMonth() )	// month not yet reached his year
			--nAge;

		else if ( dob.GetMonth() == date.GetMonth() )	// birthday in this month
		{
			if ( dob.GetDay() > date.GetDay() )		// day of month not yet reached
				--nAge;
		}
	}
	else											// age next birthday
	{
		if ( dob.GetMonth() < date.GetMonth() )	// month already gone this year
			++nAge;

		else if ( dob.GetMonth() == date.GetMonth() )	// birthday in this month
		{
			if ( dob.GetDay() <date.GetDay() )		// day of month alreday gone				++Age;
			++nAge;	
		}
	}

	return nAge;
}

//*******************************************************************
// get day of week for next birtday

const char* CSmartPayDatabase::GetDOBDayofWeek()
{
	CSSDate dob ( GetDobDate() );

	if ( dob.IsSet() == FALSE )
		return "";

	CSSDate date;
	date.SetCurrentDate();								// current date

	int nYear = date.GetYear();							// current year
	if ( dob.GetMonth() < date.GetMonth() )				// month already gone this year
		++nYear;
	else if ( dob.GetMonth() == date.GetMonth()	)		// birthday in this month
	{
		if ( dob.GetDay() < date.GetDay() )				// day of month alreday gone				++Age;
			++nYear;	
	}

	CTime time ( nYear, dob.GetMonth(), dob.GetDay(), 0, 0, 0 );
	return szDAYS[ time.GetDayOfWeek() ];
}

//*********************************************************************

bool CSmartPayDatabase::HasUserExpired()
{
	bool bHasExpired = FALSE;

	CSSDate dateExpiry ( GetExpiryDate() );
	if ( dateExpiry.IsSet() == TRUE )
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if ( dateExpiry < dateNow )
			bHasExpired = TRUE;
	}

	return bHasExpired;
}

//*********************************************************************

void CSmartPayDatabase::SetUserAlert ( int nAlertCode )
{
	if ( nAlertCode == 0 )
	{
		SetAlertCode(0);
		SetAlertDate("");
		SetAlertTime("");
	}
	else
	{
		SetAlertCode ( nAlertCode );
		SetAlertDate ( CSSDate::GetCurrentDate().GetDate() );
		SetAlertTime ( CSSTime::GetCurrentTime().GetTime() );
	}
}

//*********************************************************************

#define COLOUR_NORMAL		0x00		// black
//							0x01		// dark blue 
//							0x02		// dark green
//							0x06		// dark yellow
#define COLOUR_INACTIVE		0x09		// light blue
#define COLOUR_EXPIRED		0x0c		// light red
#define COLOUR_ALERT		0x0d		// cyan


int CSmartPayDatabase::GetRecordStatusColour()
{
	int nColour = COLOUR_NORMAL;

	if ( HasUserExpired() == TRUE )
		nColour = COLOUR_EXPIRED;

	else if ( IsAlertSet() == TRUE )
		nColour = COLOUR_ALERT;

	else if ( GetInactive() == TRUE )
		nColour = COLOUR_INACTIVE;

	return nColour;
}

//*********************************************************************

void CSmartPayDatabase::UpdatePoints ( int nPoints )
{
	AddToPoints ( nPoints );
	AddToPointsTD ( nPoints );
}

//*********************************************************************
//*** validation checking**********************************************
//*********************************************************************

void CSmartPayDatabase::SetField ( const char* strLabel, const char* szText, int nMax )
{
	CString strText = szText;
	strText.TrimLeft(' ');							// loose leading spaces

	int nLen = strText.GetLength();
	if ( nLen > nMax )
	{
		strText = strText.Left(nMax);
		nLen = nMax;
	}

	for ( int i = 0 ; i < nLen ; i++ )
	{
		if ( strText.GetAt(i) < ' ' )				//	convert Ctrl chars to space
			strText.SetAt(i, ' ' );
	}

	CSSDatabase::Set ( strLabel, strText );
}

void CSmartPayDatabase::SetField ( const char* szLabel, int nValue, int nMin, int nMax )
{
	if ( nValue < nMin )	return;
	if ( nValue > nMax )	return;
	CSSDatabase::Set ( szLabel, nValue );
}

void CSmartPayDatabase::SetField(const char* szLabel, __int64 nValue, __int64 nMin, __int64 nMax)
{
	if (nValue < nMin)		return;
	if (nValue > nMax)		return;
	CSSDatabase::Set(szLabel, nValue);
}

void CSmartPayDatabase::SetField ( const char* szLabel, double dValue, double dMin, double dMax )
{
	if ( dValue < dMin )	return;
	if ( dValue > dMax )	return;
	CSSDatabase::Set ( szLabel, dValue, 2 );
}

void CSmartPayDatabase::SetDateField ( const char* szLabel, const char* szDate )
{
	CString strDate = szDate;
	if ( strDate == " " )									// v1.07a 07/11/2016	Setting a Date \ Time field with a space will clear it
		strDate = "";

	bool bOk = TRUE;
	if ( strDate.GetLength() > 0 )
	{
		CSSDate date ( strDate );
		bOk = date.IsSet();
	}
	
	if ( bOk == TRUE )
		CSSDatabase::Set ( szLabel, strDate );
}

void CSmartPayDatabase::SetTimeField ( const char* szLabel, const char* szTime )
{
	CString strTime = szTime;
	if ( strTime == " " )									//v1.07a 07/11/2016	Setting a Date \ Time field with a space will clear it
		strTime = "";

	bool bOk = TRUE;
	if ( strTime.GetLength() > 0 )
	{
		CSSTime time ( strTime );
		bOk = time.IsSet();
	}
	
	if ( bOk == TRUE )
		CSSDatabase::Set(szLabel, strTime );
}

bool CSmartPayDatabase::IsValidUserID ( const char* szCardNo, CString& strCardNo, int nMaxLength )
{
	strCardNo = szCardNo;
	strCardNo.TrimLeft('0');							// loose leading 0's

	int nLen = strCardNo.GetLength();

	if ( nLen > 0 && nLen <= nMaxLength )			// see if length ok
	{
		for ( int i = 0 ; i < nLen ; i++ )
		{
			if ( strCardNo.GetAt(i) < '0' || strCardNo.GetAt(i) > '9' )
				return FALSE;
		}

		return ( _atoi64 ( strCardNo ) > 0 ) ? TRUE : FALSE;
	}
	return FALSE;
}

void CSmartPayDatabase::SetNextCardNo ( const char* szCardNo )
{
	CString strCardNo = szCardNo;
	strCardNo.TrimLeft('0');

	Set ( SmartPay::ExNextCardNo.Label, strCardNo );

}
void CSmartPayDatabase::SetPreviousCardNo ( const char* szCardNo )
{
	CString strCardNo = szCardNo;
	strCardNo.TrimLeft('0');

	Set ( SmartPay::ExPreviousCardNo.Label, strCardNo );
}

//*******************************************************************

CString CSmartPayDatabase::GetUserName()
{
	CString strName = (CString)CSSDatabase::GetString ( SmartPay::UserName.Label );

	if ( strName == "" )							// if not set return Surname
		strName = GetSurname();
	
	return strName;
}

CString CSmartPayDatabase::GetAccountDisplayName()
{
	CString strReply = "";

	CString strUserName = (CString)CSSDatabase::GetString ( SmartPay::UserName.Label );
	CString strSurname	= GetSurname();
	
	if ( strUserName.GetLength() > strSurname.GetLength() )
		strReply = strUserName;
	else
	{
		CString strForename = GetForename();
		if ( strForename != "" )
		{
			strReply = strForename;
			strReply += " " ;
		}

		if ( strSurname  != "" )
			strReply += strSurname;
	}

	return strReply;
}

//*******************************************************************

void CSmartPayDatabase::SetMemberID ( const char* szMemberID )	{ Set ( SmartPay::MemberID.Label, szMemberID ); }
void CSmartPayDatabase::SetExtRef  ( const char* szText )		{ SetField ( SmartPay::ExtRef.Label, szText, SmartPay::ExtRef.Max ); }
void CSmartPayDatabase::SetExtRef2 ( const char* szText )		{ SetField ( SmartPay::ExtRef2.Label, szText, SmartPay::ExtRef2.Max ); }
void CSmartPayDatabase::SetUserName ( const char* szText )		{ SetField ( SmartPay::UserName.Label, szText, SmartPay::UserName.Max );}	
void CSmartPayDatabase::SetForename ( const char* szText )		{ SetField ( SmartPay::Forename.Label, szText, SmartPay::Forename.Max );}	
void CSmartPayDatabase::SetSurname ( const char* szText )		{ SetField ( SmartPay::Surname.Label, szText, SmartPay::Surname.Max );}	
void CSmartPayDatabase::SetEmail ( const char* szText )			{ SetField ( SmartPay::Email.Label, szText, SmartPay::Email.Max );}	

void CSmartPayDatabase::SetGroupNo ( int nGroupNo )				{ SetField ( SmartPay::GroupNo.Label, nGroupNo, SmartPay::GroupNo.Min, SmartPay::GroupNo.Max );	}

void CSmartPayDatabase::SetPoints ( int nPoints )				{ SetField ( SmartPay::Points.Label, nPoints, SmartPay::Points.Min, SmartPay::Points.Max );		}
void CSmartPayDatabase::SetPointsTD ( int nPoints )				{ SetField ( SmartPay::PointsTD.Label, nPoints, SmartPay::PointsTD.Min, SmartPay::PointsTD.Max );	}

void CSmartPayDatabase::SetCashSpendTD ( double dValue )		{ SetField ( SmartPay::CashSpendTD.Label, dValue, SmartPay::CashSpendTD.Min, SmartPay::CashSpendTD.Max );	}

void CSmartPayDatabase::SetPurse1Liability ( double dValue )	{ SetField ( SmartPay::Purse1.Label, dValue, SmartPay::Purse1.Min, SmartPay::Purse1.Max );	}
void CSmartPayDatabase::SetPurse2Balance ( double dValue )		{ SetField ( SmartPay::Purse2.Label, dValue, SmartPay::Purse2.Min, SmartPay::Purse2.Max );	}
void CSmartPayDatabase::SetPurse3Liability ( double dValue )	{ SetField ( SmartPay::Purse3.Label, dValue, SmartPay::Purse3.Min, SmartPay::Purse3.Max );	}

void CSmartPayDatabase::SetPurse1Credit(double dValue) { SetField(SmartPay::Purse1Credit.Label, dValue, SmartPay::Purse1.Min, SmartPay::Purse1.Max); }
void CSmartPayDatabase::SetPurse3Credit(double dValue) { SetField(SmartPay::Purse3Credit.Label, dValue, SmartPay::Purse3.Min, SmartPay::Purse3.Max); }

void CSmartPayDatabase::SetPurse1LastSpend ( double dValue )	{ SetField ( SmartPay::Purse1LastSpend.Label, dValue, SmartPay::Purse1LastSpend.Min, SmartPay::Purse1LastSpend.Max );	}
void CSmartPayDatabase::SetPurse2LastSpend ( double dValue )	{ SetField ( SmartPay::Purse2LastSpend.Label, dValue, SmartPay::Purse2LastSpend.Min, SmartPay::Purse2LastSpend.Max );	}
void CSmartPayDatabase::SetPurse3LastSpend ( double dValue )	{ SetField ( SmartPay::Purse3LastSpend.Label, dValue, SmartPay::Purse3LastSpend.Min, SmartPay::Purse3LastSpend.Max );	}

void CSmartPayDatabase::SetPurse1SpendTD ( double dValue )	{ SetField ( SmartPay::Purse1SpendTD.Label, dValue, SmartPay::Purse1SpendTD.Min, SmartPay::Purse1SpendTD.Max );	}
void CSmartPayDatabase::SetPurse2SpendTD ( double dValue )	{ SetField ( SmartPay::Purse2SpendTD.Label, dValue, SmartPay::Purse2SpendTD.Min, SmartPay::Purse2SpendTD.Max );	}
void CSmartPayDatabase::SetPurse3SpendTD ( double dValue )	{ SetField ( SmartPay::Purse3SpendTD.Label, dValue, SmartPay::Purse3SpendTD.Min, SmartPay::Purse3SpendTD.Max );	}

void CSmartPayDatabase::SetInfo1 ( const char* szText )		{ SetField ( SmartPay::Info1.Label, szText, SmartPay::Info1.Max );	}
void CSmartPayDatabase::SetInfo2 ( const char* szText )		{ SetField ( SmartPay::Info2.Label, szText, SmartPay::Info2.Max );	}
void CSmartPayDatabase::SetInfo3 ( const char* szText )		{ SetField ( SmartPay::Info3.Label, szText, SmartPay::Info3.Max );	}
void CSmartPayDatabase::SetInfo4 ( const char* szText )		{ SetField ( SmartPay::Info4.Label, szText, SmartPay::Info4.Max );	}
void CSmartPayDatabase::SetInfo5 ( const char* szText )		{ SetField ( SmartPay::Info5.Label, szText, SmartPay::Info5.Max );	}
void CSmartPayDatabase::SetInfo6 ( const char* szText )		{ SetField ( SmartPay::Info6.Label, szText, SmartPay::Info6.Max );	}
void CSmartPayDatabase::SetInfo7 ( const char* szText )		{ SetField ( SmartPay::Info7.Label, szText, SmartPay::Info7.Max );	}
void CSmartPayDatabase::SetInfo8 ( const char* szText )		{ SetField ( SmartPay::Info8.Label, szText, SmartPay::Info8.Max );	}

void CSmartPayDatabase::SetTick1 ( bool bSet )				{ Set ( SmartPay::Tick1.Label, bSet );	}
void CSmartPayDatabase::SetTick2 ( bool bSet )				{ Set ( SmartPay::Tick2.Label, bSet );	}
void CSmartPayDatabase::SetTick3 ( bool bSet )				{ Set ( SmartPay::Tick3.Label, bSet );	}
void CSmartPayDatabase::SetTick4 ( bool bSet )				{ Set ( SmartPay::Tick4.Label, bSet );	}

void CSmartPayDatabase::SetExpiry ( const char* szDate )	{ SetDateField ( SmartPay::Expiry.Label, szDate );	}
void CSmartPayDatabase::SetDob ( const char* szDate )		{ SetDateField ( SmartPay::Dob.Label, szDate );	}
void CSmartPayDatabase::SetAlertCode ( int nCode )			{ SetField ( SmartPay::AlertCode.Label, nCode, SmartPay::AlertCode.Min, SmartPay::AlertCode.Max ); }
void CSmartPayDatabase::SetAllergyCodes ( int nCodes )		{ SetField ( SmartPay::AllergyCodes.Label, nCodes, SmartPay::AllergyCodes.Min, SmartPay::AllergyCodes.Max ); }

void CSmartPayDatabase::SetDietary ( const char* szText )	{ SetField ( SmartPay::Dietary.Label, szText, SmartPay::Dietary.Max );}	

//*******************************************************************

void CSmartPayDatabase::SetGender ( const char* szGender )
{
	CString strGender = szGender;
	strGender.MakeUpper();

		 if ( strGender.Left(1) == "M" )	SetField ( SmartPay::Gender.Label, "M", SmartPay::Gender.Max );	
	else if ( strGender.Left(1) == "F" )	SetField ( SmartPay::Gender.Label, "F", SmartPay::Gender.Max );	
	else									SetField ( SmartPay::Gender.Label, "", SmartPay::Gender.Max );	
}

CString CSmartPayDatabase::GetGenderText()
{
	CString strGender = GetGender();

		 if ( strGender == "M" )		strGender = "Male";
	else if ( strGender == "F" )		strGender = "Female";
	else								strGender = "unknown";

	return strGender;
}

//*******************************************************************

void CSmartPayDatabase::RevaluePurse2 ( double dValue, const char* szDate, const char* szTime )
{
	if ( lstrlen ( szDate ) == 0 )	SetRefreshedDate ( CSSDate::GetCurrentDate().GetDate() );
	else							SetRefreshedDate ( szDate );

	if ( lstrlen ( szTime ) == 0 )	SetRefreshedTime ( CSSTime::GetCurrentTime().GetTime() );
	else							SetRefreshedTime ( szTime );

	SetPurse2Balance ( dValue );
	SetPurse2LastSpend ( 0 );
}

//*******************************************************************

CString CSmartPayDatabase::GetPurseBalanceString(int nPurseNo)
{
	double dPurse = 0;

	switch (nPurseNo)
	{
	case 1:	dPurse = GetPurse1Balance();	break;
	case 2:	dPurse = GetPurse2Balance();	break;
	case 3:	dPurse = GetPurse3Balance();	break;
	}

	CString strMsg = "";
	strMsg.Format("%.2f", dPurse);
	return strMsg;
}

//*******************************************************************

CString CSmartPayDatabase::GetTotalSpendTDString()
{
	CString strMsg;		strMsg.Format ( "%.2f", GetTotalSpendTD() );
	return strMsg;
}

CString CSmartPayDatabase::GetPointsString()
{
	CString strMsg;		strMsg.Format ( "%d", GetPoints() );		// v1.05b 23/07/2015
	return strMsg;
}

CString CSmartPayDatabase::GetPointsTDString()
{
	CString strMsg;		strMsg.Format ( "%d", GetPointsTD() );
	return strMsg;
}

//*******************************************************************

CString CSmartPayDatabase::GetInfo ( int n )
{
	CString strText = "";

	switch ( n )
	{
	case 1:		strText = GetInfo1();		break;
	case 2:		strText = GetInfo2();		break;
	case 3:		strText = GetInfo3();		break;
	case 4:		strText = GetInfo4();		break;
	case 5:		strText = GetInfo5();		break;
	case 6:		strText = GetInfo6();		break;
	case 7:		strText = GetInfo7();		break;
	case 8:		strText = GetInfo8();		break;
	}
	return strText;
}

void CSmartPayDatabase::SetInfo ( int n, const char* szText )
{
	switch ( n )
	{
	case 1:		SetInfo1 ( szText );		break;
	case 2:		SetInfo2 ( szText );		break;
	case 3:		SetInfo3 ( szText );		break;
	case 4:		SetInfo4 ( szText );		break;
	case 5:		SetInfo5 ( szText );		break;
	case 6:		SetInfo6 ( szText );		break;
	case 7:		SetInfo7 ( szText );		break;
	case 8:		SetInfo8 ( szText );		break;
	}
}

bool CSmartPayDatabase::GetTick ( int n )
{
	bool bReply = FALSE;

	switch ( n )
	{
	case 1:		bReply = GetTick1();		break;
	case 2:		bReply = GetTick2();		break;
	case 3:		bReply = GetTick3();		break;
	case 4:		bReply = GetTick4();		break;
	}
	return bReply;
}

void CSmartPayDatabase::SetTick ( int n, bool bSet )
{
	switch ( n )
	{
	case 1:		SetTick1 ( bSet );		break;
	case 2:		SetTick2 ( bSet );		break;
	case 3:		SetTick3 ( bSet );		break;
	case 4:		SetTick4 ( bSet );		break;
	}
}

//******************************************************************
// nDay 0= use group settings otherwise 1=Sun,.. 7 = Sat
//
// NOTE unlike group where 0=Sun... 6=Sat

bool CSmartPayDatabase::IsRefreshAllowed ( int nDay )
{
	if ( nDay > 0 )
	{
		--nDay;
		int nMask = 0x01;
		int nFlags = GetRefreshFlags();

		nMask = nMask << nDay;
		return ( (nFlags & nMask) == nMask ) ? TRUE : FALSE;
	}
	return FALSE;
}

void CSmartPayDatabase::SetRefreshAllowed ( int nDay, bool bSet )
{
	int nMask = 0x01;
	int nFlags = GetRefreshFlags();

	--nDay;
	nMask = nMask << nDay;
	if ( bSet == TRUE )		nFlags |= nMask;
	else					nFlags &= ~nMask;

	SetRefreshFlags ( nFlags);
}

CString CSmartPayDatabase::GetRefreshTextDays()
{
	int nCount = 0;
	CString strMsg;

	for ( int nDay = 1 ; nDay <= 7 ; nDay++ )
	{
		if ( IsRefreshAllowed ( nDay ) == TRUE )
		{
			if ( nCount > 0 )
				strMsg += ",";

			strMsg += szDAYS[nDay];
			++nCount;
		}
	}
	return strMsg;
}

//***************************************************************
// set refresh days given "Mon,Tue,Wed.." or "1,2,3" where 1=Mon, 2=Tues..7=Sun

void CSmartPayDatabase::SetRefreshRef ( const char* szDaysText )
{
	ClearRefreshFlags();								// clear current setting

	CCSV csv ( szDaysText );
	for ( int i = 0 ; i < csv.GetSize() ; i++ )
	{
		CString strDay = csv.GetString(i);				// get single day
		strDay.TrimLeft ( ' ' );						// just in case

		int nDay;
		for ( nDay = 1 ; nDay <= 7 ; nDay++ )		// assume days are set to "Mon,Tue.."
		{
			if ( strDay.Left(3) == szDAYS[nDay] )
			{
				SetRefreshAllowed( nDay, TRUE );
				break;
			}
		}

		nDay = 0;										// look for days set to "1,2..."
		switch ( atoi ( strDay ) )
		{
		case 1:		nDay = 2;		break;				// Monday
		case 2:		nDay = 3;		break;				// Tuesday
		case 3:		nDay = 4;		break;				// Wednesday
		case 4:		nDay = 5;		break;				// Thursday
		case 5:		nDay = 6;		break;				// Friday
		case 6:		nDay = 7;		break;				// Saturday
		case 7:		nDay = 1;		break;				// Sunday
		}

		if ( nDay != 0 )
			SetRefreshAllowed( nDay, TRUE );
	}
}

//**************************************************************
// on exit if TRUE pointing to the ToCardNo record

bool CSmartPayDatabase::CopyRecord(const char* szFromCardNo, const char* szToCardNo, bool bFailIfExists)
{
	bool bReply = FALSE;							// assume no copy done
	int nIndexFrom = -1;
	int nIndexTo = -1;

	int nCurrentIndex = GetCurrentRow();					// save current database pointer

	if (FindRecord(szFromCardNo) == TRUE)				// source record
	{
		nIndexFrom = GetCurrentRow();

		if (FindRecord(szToCardNo) == TRUE)			// destination record
		{
			if (FALSE == bFailIfExists)
			{
				nIndexTo = GetCurrentRow();
			}
		}
		else
		{
			if (AddBlankRecord(szToCardNo) == TRUE)
			{
				nIndexTo = GetCurrentRow();
			}
		}
	}

	if ((nIndexFrom == -1) || (nIndexTo == -1))
	{
		MoveTo(nCurrentIndex);
		return FALSE;
	}

	CString strLabel = "";
	CString strData = "";

	for (int n = 0; n < GetHeaderSize(); n++)
	{
		strLabel = GetHeaderLabel(n);

		bool bCopyField = TRUE;

		// don't overwrite main record key
		if (strLabel == SmartPay::ExUserID.Label)
		{
			bCopyField = FALSE;
		}
		
		if (TRUE == bCopyField)
		{
			MoveTo(nIndexFrom);
			strData = GetString(strLabel);

			MoveTo(nIndexTo);
			Set(strLabel, strData);
		}
	}

	return TRUE;
}

//**************************************************************

CString CSmartPayDatabase::CopyRecordToString()
{
	CCSV csvLine;

	for ( int n = 0 ; n < GetHeaderSize() ; n++ )
	{
		CString strField = GetString ( GetHeaderLabel ( n ) );	// read any field as a string

		if ( strField == "0" || strField == "0.00" )			// zero skip '0' fields
			csvLine.Add ( "" );
		else
			csvLine.Add ( strField );
	}

	CString strLine = csvLine.GetLine();
	return strLine;
}

//**************************************************************
// cannot use GetHeaderLabel in case database not been changed to latest header
// after new fields have been added

bool CSmartPayDatabase::CopyRecordToFile ( const char* szFilename )
{
	bool bReply;
	CSSFile file;
	if ( ( bReply = file.Open ( szFilename, "wb" ) ) == TRUE )
	{
		if ( ( bReply = file.WriteLine ( m_strHeader ) ) == TRUE )
		{
			CCSV csvLine;
			CCSV csvHeader ( m_strHeader );

			for ( int n = 0 ; n < csvHeader.GetSize() ; n++ )
				csvLine.Add ( GetString ( csvHeader.GetString(n) ) );

			if ( ( bReply = file.Write ( csvLine ) ) == FALSE )
				m_strError = "CRF Error 3";
		}
		else
			m_strError = "CRF Error 2";

		file.Close();
	}
	else
		m_strError = "CRF Error 1";

	return bReply;
}

//**************************************************************

bool CSmartPayDatabase::CopyFileToRecord ( const char* szFilename )
{
	bool bReply;
	CSSFile file;
	if ( ( bReply = file.Open ( szFilename, "rb" ) ) == TRUE )
	{
		CCSV csvHeader;
		if ( ( bReply = file.Read ( csvHeader ) ) == TRUE )
		{
			CCSV csvLine;
			if ( ( bReply = file.Read ( csvLine ) ) == TRUE )
			{
				for ( int n = 0 ; n < csvHeader.GetSize() ; n++ )
					Set ( csvHeader.GetString(n), csvLine.GetString(n) );
			}
			else
				m_strError = "CFR Error 3";
		}
		else
			m_strError = "CFR Error 2";

		file.Close();
	}
	else
		m_strError = "CFR Error 1";

	return bReply;
}

//**************************************************************

void CSmartPayDatabase::CopyToDatabase ( CSmartPayDatabase* pDbCopy )
{
	bool bReply = pDbCopy->AddBlankRecord ( GetUserID() );

	CString strLabel = "";
	CString strData = "";

	for ( int n = 0 ; n < GetHeaderSize() ; n++ )
	{
		strLabel = GetHeaderLabel ( n );

		strData = GetString ( strLabel );
		pDbCopy->Set ( strLabel, strData );
	}
}

//**************************************************************
// szText = "nn,nn,nn,nn,nn,nn,nn" for daily gouups for "Sun,Mon...Sat"

void CSmartPayDatabase::SetDailyGroups ( const char* szText )
{
	CCSV csvData ( szText );
	int nCount = csvData.GetSize();							// number of elements

	if ( nCount > 0 && nCount <= 7 )
	{
		CCSV csv ( GetDailyGroups() );						// current data

		for ( int nDayIndex = 0 ; nDayIndex < nCount ; nDayIndex++ )
		{
			int nGroupNo = csvData.GetInt ( nDayIndex );

				  if ( nGroupNo < SmartPay::GroupNo.Min )	csv.SetAt ( nDayIndex, "" );
			 else if ( nGroupNo <= SmartPay::GroupNo.Max )	csv.SetAt ( nDayIndex, nGroupNo );
		}

		CString strText = csv.GetLine();
		SetField ( SmartPay::DayGroups.Label, strText, SmartPay::DayGroups.Max );
	}
}

bool CSmartPayDatabase::HaveDailyGroups()
{
	bool bReply = FALSE;

	CCSV csv ( GetDailyGroups() );

	for ( int nDayIndex = 0 ; nDayIndex < csv.GetSize() ; nDayIndex++ )
	{
		if ( csv.GetInt ( nDayIndex ) > SmartPay::GroupNo.Min )
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

//*****************************************************************
// nDayNo - 0=sunday, 1=monday...

void CSmartPayDatabase::SetDailyGroupNo ( int nDayIndex, int nGroupNo )
{
	if ( nDayIndex >= 0 && nDayIndex <= 6 )
	{
		CCSV csv ( GetDailyGroups() );

			 if ( nGroupNo < SmartPay::GroupNo.Min )	csv.SetAt ( nDayIndex, "" );
		else if ( nGroupNo <= SmartPay::GroupNo.Max )	csv.SetAt ( nDayIndex, nGroupNo );

		CString strText = csv.GetLine();
		SetField ( SmartPay::DayGroups.Label, strText, SmartPay::DayGroups.Max );
	}
}	

int CSmartPayDatabase::GetDailyGroupNo ( int nDayIndex )
{
	int nGroupNo = 0;

	if ( nDayIndex >= 0 && nDayIndex <= 6 )
	{
		CCSV csv ( GetDailyGroups() ); 

		if ( csv.IsEmpty ( nDayIndex ) != TRUE )
			nGroupNo = csv.GetInt ( nDayIndex );
	}

	return nGroupNo;
}

//******************************************************************
// nPurse 1 0 2 , AuditPeriod 0 - 5

bool CSmartPayDatabase::IsPurseAllowed ( int nPurseNo, int nPeriodIndex )
{

	CString strLabel;
	switch ( nPurseNo )
	{
	case 1:	strLabel = SmartPay::Purse1Allowed.Label;	break;
	case 2:	strLabel = SmartPay::Purse2Allowed.Label;	break;
	case 3:	strLabel = SmartPay::Purse3Allowed.Label;	break;
	}

	int nFlags = CSSDatabase::GetInt(strLabel);
	int nMask = 0x01;

	nMask = nMask << nPeriodIndex;
	return ( (nFlags & nMask) == nMask ) ? TRUE : FALSE;
}

void CSmartPayDatabase::SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet )
{
	CString strLabel;
	switch ( nPurseNo )
	{
	case 1:	strLabel = SmartPay::Purse1Allowed.Label;	break;
	case 2:	strLabel = SmartPay::Purse2Allowed.Label;	break;
	case 3:	strLabel = SmartPay::Purse3Allowed.Label;	break;
	}

	int nFlags = CSSDatabase::GetInt(strLabel);
	int nMask = 0x01;
	nMask = nMask << nPeriodIndex;
	if ( bSet == TRUE )		nFlags |= nMask;
	else					nFlags &= ~nMask;

	CSSDatabase::Set (strLabel, nFlags);
}

//******************************************************************

void CSmartPayDatabase::CopyGroupMaxSpendSettings( CSmartPayGroupDatabase* pGroupDatabase )
{
	if ( pGroupDatabase -> FindRecord ( GetGroupNo() ) == TRUE )
	{
		for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
		{
			for ( int nPurse = 1; nPurse <= 3; nPurse++ )
			{
				SetPurseAllowed( nPurse, nPeriod - 1, pGroupDatabase -> IsPurseAllowed( nPurse, nPeriod - 1 ) ); 
			}
		}

		SetMaxSpendPeriod1 ( pGroupDatabase -> GetMaxSpendPeriod1() );
		SetMaxSpendPeriod2 ( pGroupDatabase -> GetMaxSpendPeriod2() );
		SetMaxSpendPeriod3 ( pGroupDatabase -> GetMaxSpendPeriod3() );
		SetMaxSpendPeriod4 ( pGroupDatabase -> GetMaxSpendPeriod4() );
		SetMaxSpendPeriod5 ( pGroupDatabase -> GetMaxSpendPeriod5() );
		SetMaxSpendPeriod6 ( pGroupDatabase -> GetMaxSpendPeriod6() );

		SetMaxSpendPeriod1P3 ( pGroupDatabase -> GetMaxSpendPeriod1P3() );
		SetMaxSpendPeriod2P3 ( pGroupDatabase -> GetMaxSpendPeriod2P3() );
		SetMaxSpendPeriod3P3 ( pGroupDatabase -> GetMaxSpendPeriod3P3() );
		SetMaxSpendPeriod4P3 ( pGroupDatabase -> GetMaxSpendPeriod4P3() );
		SetMaxSpendPeriod5P3 ( pGroupDatabase -> GetMaxSpendPeriod5P3() );
		SetMaxSpendPeriod6P3 ( pGroupDatabase -> GetMaxSpendPeriod6P3() );

		SetOverSpendPeriod1 ( pGroupDatabase -> GetOverSpendPeriod1() );
		SetOverSpendPeriod2 ( pGroupDatabase -> GetOverSpendPeriod2() );
		SetOverSpendPeriod3 ( pGroupDatabase -> GetOverSpendPeriod3() );
		SetOverSpendPeriod4 ( pGroupDatabase -> GetOverSpendPeriod4() );
		SetOverSpendPeriod5 ( pGroupDatabase -> GetOverSpendPeriod5() );
		SetOverSpendPeriod6 ( pGroupDatabase -> GetOverSpendPeriod6() );

		SetOverSpendPeriod1P3 ( pGroupDatabase -> GetOverSpendPeriod1P3() );
		SetOverSpendPeriod2P3 ( pGroupDatabase -> GetOverSpendPeriod2P3() );
		SetOverSpendPeriod3P3 ( pGroupDatabase -> GetOverSpendPeriod3P3() );
		SetOverSpendPeriod4P3 ( pGroupDatabase -> GetOverSpendPeriod4P3() );
		SetOverSpendPeriod5P3 ( pGroupDatabase -> GetOverSpendPeriod5P3() );
		SetOverSpendPeriod6P3 ( pGroupDatabase -> GetOverSpendPeriod6P3() );
	}
}

//******************************************************************************************

void CSmartPayDatabase::AllocatePurse1Spend(double dSpend, double& dLiabilitySpend, double& dCreditSpend )
{
	dLiabilitySpend = 0.0;
	dCreditSpend = 0.0;

	if (dSpend < 0.0)
	{
		dLiabilitySpend = dSpend;
	}
	else
	{
		double dCredit = GetPurse1Credit();

		if ( dSpend < dCredit )
		{
			dCreditSpend = dSpend;
		}
		else
		{
			dCreditSpend = dCredit;
			dLiabilitySpend = dSpend - dCreditSpend;
		}
	}
}

//******************************************************************************************

void CSmartPayDatabase::AllocatePurse3Spend(double dSpend, double& dLiabilitySpend, double& dCreditSpend)
{
	dLiabilitySpend = 0.0;
	dCreditSpend = 0.0;

	if (dSpend < 0.0)
	{
		dLiabilitySpend = dSpend;
	}
	else
	{
		double dCredit = GetPurse3Credit();

		if (dSpend < dCredit)
		{
			dCreditSpend = dSpend;
		}
		else
		{
			dCreditSpend = dCredit;
			dLiabilitySpend = dSpend - dCreditSpend;
		}
	}
}

//******************************************************************************************
