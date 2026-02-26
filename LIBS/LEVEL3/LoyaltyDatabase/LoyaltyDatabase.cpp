//$$********************************************************************

static const char* Days[] = { "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* szMONTHS[13] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

//static const char* szHEADER_v1	= "CardNo,GroupNo,FullName,Purse,Points,CardSpendTD,CashSpendTD,PointsTD,OverSpend,ExpiryDate,LastUsedDate,LastUsedTime,HotlistCode,HotlistDate,HotlistTime,PreviousNo,NextNo,Address1,Address2,Address3,Address4,Address5,Telephone1,Telephone2,Info1,Info2,Info3,Info4,Dob";
//static const char* szHEADER_V2	= "CardNo,GroupNo,FullName,Dob,Gender,Points,PointsTD,Purse,LastUsedDate,LastUsedTime,CardSpend,CardSpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,CashSpendTD,MaxSpend,OverSpend,ExpiryDate,Alert,Inactive,HotlistCode,HotlistDate,HotlistTime,PreviousNo,NextNo,Address1,Address2,Address3,Address4,Address5,Telephone1,Telephone2,Info1,Info2,Info3,Info4";
//static const char* szHEADER_V3	= "CardNo,GroupNo,FullName,Dob,Gender,Points,PointsTD,Purse,LastUsedDate,LastUsedTime,CardSpend,CardSpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,CashSpendTD,MaxSpend,OverSpend,ExpiryDate,Alert,Inactive,HotlistCode,HotlistDate,HotlistTime,PreviousNo,NextNo,Address1,Address2,Address3,Address4,Address5,Telephone1,Telephone2,ExtRef,Info1,Info2,Info3,Info4";
//static const char* szHEADER_V4	= "CardNo,GroupNo,FullName,Forename,Surname,Dob,Gender,Points,PointsTD,Purse,LastUsedDate,LastUsedTime,CardSpend,CardSpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,CashSpendTD,MaxSpend,OverSpend,ExpiryDate,Alert,Inactive,HotlistCode,HotlistDate,HotlistTime,PreviousNo,NextNo,Address1,Address2,Address3,Address4,Address5,Telephone1,Telephone2,ExtRef,Info1,Info2,Info3,Info4";
//static const char* szHEADER_V5	= "CardNo,GroupNo,FullName,Forename,Surname,Dob,Gender,CTS,Points,PointsTD,Purse,LastUsedDate,LastUsedTime,CardSpend,CardSpendTD,Purse2,Purse2Date,Purse2Time,Purse2Spend,Purse2SpendTD,RefreshDate,RefreshTime,CashSpendTD,MaxSpend,OverSpend,ExpiryDate,Alert,Inactive,HotlistCode,HotlistDate,HotlistTime,PreviousNo,NextNo,Address1,Address2,Address3,Address4,Address5,Telephone1,Telephone2,ExtRef,Info1,Info2,Info3,Info4";

static const char* szSORTFIELD		= "CardNo,ascending";
static int nRETRIES = 10;

//$$********************************************************************

CLoyaltyDatabase::CLoyaltyDatabase( CLoyaltyHeaderHelpers* pHeader )
{
	m_strKey = "";
	m_strInUseFilename = "";

	if (pHeader != NULL)
	{
		InitHeaders(*pHeader);
	}
	else
	{
		m_strHeader = "";
		m_strLegacyHeaders = "";
	}
}

//$$********************************************************************

void CLoyaltyDatabase::InitHeaders(CLoyaltyHeaderHelpers& header)
{
	m_strHeader = header.GetHeaderFull();
	m_strLegacyHeaders = header.GetLegacyHeaders();
}

//$$********************************************************************

bool CLoyaltyDatabase::IsAvailable ( const char* szFilename )
{
	CString strInUseFile = MakeInUseFilename ( szFilename );

	CSSFile file;
	if ( file.Open ( strInUseFile, "wb", _SH_DENYWR, 0 ) == TRUE )		// no retries on opening
	{
		file.Close();
		::remove ( strInUseFile );
		return TRUE;
	}
	return FALSE;
}

//$$********************************************************************

CString CLoyaltyDatabase::MakeInUseFilename ( const char* szFilename )
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

//$$********************************************************************
//	mode = DB_READONLY, DB_READWRITE

int CLoyaltyDatabase::OpenReadOnlyForSharing( const char* szFilename )
{
	return OpenForSharing( szFilename, "", DB_READONLY );
}

//$$********************************************************************

int CLoyaltyDatabase::OpenReadWriteForSharing( const char* szFilename, const char* szBackupFolder )
{
	return OpenForSharing( szFilename, szBackupFolder, DB_READWRITE );
}

//$$********************************************************************

int CLoyaltyDatabase::OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode )
{
	m_strDatabaseFilename = szFilename;

	CString strHeader = GetHeader();
	CSSStringDatabase::CreateSSDatabase( m_strDatabaseFilename, szBackupFolder, strHeader, szSORTFIELD, nRETRIES );	// create db if not found
	CSSStringDatabase::Close();

	return ReOpenForSharing ( szBackupFolder, nMode );
}

//$$********************************************************************

int CLoyaltyDatabase::ReOpenReadOnlyForSharing()
{
	return ReOpenForSharing( "", DB_READONLY );
}

//$$********************************************************************

int CLoyaltyDatabase::ReOpenReadWriteForSharing( const char* szBackupFolder )
{
	return ReOpenForSharing( szBackupFolder, DB_READWRITE );
}

//$$********************************************************************

int CLoyaltyDatabase::ReOpenForSharing ( const char* szBackupFolder, int nMode )
{
	int nReply = DB_ERR_NONE;

	if (nMode == DB_READONLY)
	{
		nReply = CSSStringDatabase::OpenSSDatabaseReadOnly(m_strDatabaseFilename, szSORTFIELD, nRETRIES);
	}
	else
	{
		nReply = CSSStringDatabase::OpenSSDatabaseReadWrite(m_strDatabaseFilename, szBackupFolder, szSORTFIELD, nRETRIES);

		//CHECK FOR PREVIOUS VERSIONS OF HEADER
		bool bOldHeader = FALSE;
		{
			int nIndex = GetHeaderIndex("CashLastSpendDate");		
			if (nIndex == -1)												
			{
				bOldHeader = TRUE;
			}
		}

		if (FALSE == bOldHeader)
		{
			int nIndex = GetHeaderIndex("CTS");				
			if (nIndex != -1)									
			{
				bOldHeader = TRUE;
			}
		}

		if ( TRUE == bOldHeader )													// not found
		{
			CSSStringDatabase::Close();										// close file
			CString strHeader = GetHeader();
			nReply = CSSStringDatabase::OpenSSDatabaseReadWriteNewHeader ( m_strDatabaseFilename, szBackupFolder, strHeader, "CardNo", nRETRIES );// read and write and modify header (file must exist)
		}

		if ( nReply == DB_ERR_NONE )
		{
			m_strInUseFilename = MakeInUseFilename ( m_strDatabaseFilename );

			if (m_strInUseFilename != "")
			{
				m_fileInUse.Open(m_strInUseFilename, "wb", _SH_DENYWR);
			}
		}
	}

	return nReply;
}	

//$$********************************************************************

bool CLoyaltyDatabase::Close()
{
	if ( m_strInUseFilename != "" )
	{
		m_fileInUse.Close();
		::remove ( m_strInUseFilename );
	}

	return CSSStringDatabase::Close();
}

//$$********************************************************************
// CardNo = database key

const char* CLoyaltyDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, Loyalty::ExCardNo.Max, '0' );
	return m_strKey;
}

//$$********************************************************************

int CLoyaltyDatabase::GetPercentPosition()
{
	int nRecordCount = GetRecordCount();
	return ( nRecordCount <= 0 ) ? 0 : ( GetCurrentRow() * 100L ) / nRecordCount;
}

//$$********************************************************************

bool CLoyaltyDatabase::IsStringEmpty ( const char* szText )
{
	CString strText = szText;
	strText.TrimLeft ( ' ' );
	return ( strText == "" ) ? TRUE : FALSE;
}

//$$********************************************************************

bool CLoyaltyDatabase::IsRecordEmpty()
{
	if (IsStringEmpty(GetFullName()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetForename()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetSurname()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetExtRef()) == FALSE)	return FALSE;
	if (GetPoints() != 0)						return FALSE;
	if (GetPointsTD() != 0)						return FALSE;
	if (GetPurse1Balance() != 0)				return FALSE;
	if (GetPurse2Balance() != 0)				return FALSE;
	if (GetTotalSpendTD() != 0)					return FALSE;		// CardSpendTD CashSpendTD
	if (GetOverSpend() != 0)					return FALSE;
	if (IsStringEmpty(GetAdd1()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetAdd2()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetAdd3()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetAdd4()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetAdd5()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetTel1()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetTel2()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo1()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo2()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo3()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetInfo4()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetExpiryDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetDobDate()) == FALSE)			return FALSE;
	if (IsStringEmpty(GetPurse1LastSpendDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetPurse1LastSpendTime()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetPurse2LastSpendDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetPurse2LastSpendTime()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetCashLastSpendDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetCashLastSpendTime()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetHotlistDate()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetHotlistTime()) == FALSE)		return FALSE;
	if (IsStringEmpty(GetPreviousCardNo()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetNextCardNo()) == FALSE)		return FALSE;

	return TRUE;
}

//$$********************************************************************

int CLoyaltyDatabase::GetGroupNo()
{
	int nGroupNo = CSSDatabase::GetInt ( Loyalty::GroupNo.Label );
	
	if (nGroupNo == 0)
	{
		nGroupNo = 1;
	}

	return nGroupNo;
}

//$$********************************************************************

CString CLoyaltyDatabase::GetGroupNoString()
{
	CString strBuf = "";
	strBuf.Format ( "%d", GetGroupNo() );
	return strBuf;
}

//$$********************************************************************

CString CLoyaltyDatabase::IncrementCardNo ( __int64 nMaxNo )
{
	CString strCardNo = GetCardNo();				// get current card no

	__int64 nCardNo = _atoi64 ( strCardNo );
	
	if (nCardNo < nMaxNo)							// see if less than  9999999999
	{
		strCardNo.Format("%I64d", nCardNo + 1);		// inc user ID number
	}

	return strCardNo;
}

//$$********************************************************************

bool CLoyaltyDatabase::IsCardUsed()
{
	if ( GetPointsTD() != 0 )		return TRUE;
	if ( GetPurse1SpendTD() != 0 )	return TRUE;
	if ( GetPurse2SpendTD() != 0 )	return TRUE;
	if ( GetCashSpendTD() != 0)		return TRUE;
	return FALSE;
}

//$$********************************************************************

bool CLoyaltyDatabase::GetLastUsed ( CSSDate& date, CSSTime& time )
{
	CSSDateTime datePurse1 ( GetPurse1LastSpendDate(), GetPurse1LastSpendTime() );
	CSSDateTime datePurse2 ( GetPurse2LastSpendDate(), GetPurse2LastSpendTime() );
	CSSDateTime dateCash( GetCashLastSpendDate(), GetCashLastSpendTime());

	CSSDateTime dateLastUsed = datePurse1;	
	
	int nDateToUse(datePurse1.IsSet() ? 1 : 0);
	
	if (datePurse2.IsSet() == TRUE)
	{
		if (( 0 == nDateToUse) || (datePurse2 > dateLastUsed))
		{
			dateLastUsed = datePurse2;
			nDateToUse = 2;
		}
	}

	if (dateCash.IsSet() == TRUE)
	{
		if ((0 == nDateToUse) || (dateCash > dateLastUsed))
		{
			nDateToUse = 3;
		}
	}

	switch (nDateToUse)
	{
	case 1:
		date.SetDate(GetPurse1LastSpendDate());
		time.SetTime(GetPurse1LastSpendTime());
		return TRUE;

	case 2:
		date.SetDate(GetPurse2LastSpendDate());
		time.SetTime(GetPurse2LastSpendTime());
		return TRUE;

	case 3:
		date.SetDate(GetCashLastSpendDate());
		time.SetTime(GetCashLastSpendTime());
		return TRUE;

	case 0:
	default:
		return FALSE;
	}
}

//$$********************************************************************

bool CLoyaltyDatabase::IsBirthdayToday()
{
	bool bIsBirthday = FALSE;

	CSSDate dateDob ( GetDobDate() );
	if ( dateDob.IsSet() == TRUE )
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if ( dateDob.GetMonth() == dateNow.GetMonth() )
		{
			if (dateDob.GetDay() == dateNow.GetDay())
			{
				bIsBirthday = TRUE;
			}
		}
	}

	return bIsBirthday;
}

//$$********************************************************************

const char* CLoyaltyDatabase::GetBirthdayMonth()
{
	CSSDate dob ( GetDobDate() );
	return szMONTHS[dob.GetMonth()];
}

//$$********************************************************************
// return age of card holder, -1 = not dob set
// if bNextBDay = true - gets age at next birthday

CString CLoyaltyDatabase::GetAgeNextBirthday()
{
	int nAge = GetAge(TRUE);
	
	CString strAge = "";

	if ( nAge != -1)
	{
		strAge.Format("%d", nAge);
	}

	return strAge;
}

//$$********************************************************************

int CLoyaltyDatabase::GetAge ( bool bNextBDay )
{
	CSSDate dob ( GetDobDate() );

	if (dob.IsSet() == FALSE)
	{
		return -1;
	}

	CSSDate date;
	date.SetCurrentDate();

	int nYear1 = dob.GetYear();				// birthday year
	int nYear2 = date.GetYear();				// todays year

	int nAge = nYear2 - nYear1;					// age next birthday

	if ( bNextBDay == FALSE )					// adjust for birth not yet this year
	{
		if (dob.GetMonth() > date.GetMonth())	// month not yet reached his year
		{
			--nAge;
		}
		else if ( dob.GetMonth() == date.GetMonth() )	// birthday in this month
		{
			if (dob.GetDay() > date.GetDay())		// day of month not yet reached
			{
				--nAge;
			}
		}
	}
	else											// age next birthday
	{
		if (dob.GetMonth() < date.GetMonth())	// month already gone this year
		{
			++nAge;
		}
		else if ( dob.GetMonth() == date.GetMonth() )	// birthday in this month
		{
			if (dob.GetDay() < date.GetDay())		// day of month alreday gone				++Age;
			{
				++nAge;
			}
		}
	}

	return nAge;
}

//$$********************************************************************
// get day of week for next birtday

const char* CLoyaltyDatabase::GetDOBDayofWeek()
{
	CSSDate dob ( GetDobDate() );

	if (dob.IsSet() == FALSE)
	{
		return "";
	}

	CSSDate date;
	date.SetCurrentDate();								// current date

	int nYear = date.GetYear();							// current year
	if (dob.GetMonth() < date.GetMonth())				// month already gone this year
	{
		++nYear;
	}
	else if ( dob.GetMonth() == date.GetMonth()	)		// birthday in this month
	{
		if (dob.GetDay() < date.GetDay())				// day of month alreday gone				++Age;
		{
			++nYear;
		}
	}

	CTime time ( nYear, dob.GetMonth(), dob.GetDay(), 0, 0, 0 );
	return Days[ time.GetDayOfWeek() ];
}

//$$********************************************************************

bool CLoyaltyDatabase::HasCardExpired()
{
	bool bHasExpired = FALSE;

	CSSDate dateExpiry ( GetExpiryDate() );
	if ( dateExpiry.IsSet() == TRUE )
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if (dateExpiry < dateNow)
		{
			bHasExpired = TRUE;
		}
	}

	return bHasExpired;
}

//$$********************************************************************

void CLoyaltyDatabase::SetHotlistCard ( int nHotlistCode )
{
	if ( nHotlistCode == 0 )
	{
		SetHotlistCode(0);
		SetHotlistDate("");
		SetHotlistTime("");
	}
	else
	{
		SetHotlistCode ( nHotlistCode );
		SetHotlistDate ( CSSDate::GetCurrentDate().GetDate() );
		SetHotlistTime ( CSSTime::GetCurrentTime().GetTime() );
	}
}

//$$********************************************************************

#define COLOUR_NORMAL		0x00		// black
//							0x01		// dark blue
#define COLOUR_NOTUSED		0x02		// dark green
//							0x06		// dark yellow
#define COLOUR_INACTIVE		0x09		// light blue
#define COLOUR_HOTLISTED	0x0c		// light red
#define COLOUR_EXPIRED		0x0d		// cyan

int CLoyaltyDatabase::GetRecordStatusColour()
{
	int nColour = COLOUR_NORMAL;

	if (HasCardExpired() == TRUE)
	{
		nColour = COLOUR_EXPIRED;
	}
	if (IsCardHotlisted() == TRUE)
	{
		nColour = COLOUR_HOTLISTED;
	}
	else if (GetInactive() == TRUE)
	{
		nColour = COLOUR_INACTIVE;
	}
	else if (IsCardUsed() == FALSE)
	{
		nColour = COLOUR_NOTUSED;
	}

	return nColour;
}

//$$********************************************************************

void CLoyaltyDatabase::UpdatePoints ( int nPoints )
{
	AddToPoints ( nPoints );
	AddToPointsTD ( nPoints );
}

//$$********************************************************************

CString CLoyaltyDatabase::GetPointsString()
{
	CString strBuf = "";
	strBuf.Format ( "%d", GetPoints() );
	return strBuf;
}

//$$********************************************************************

CString CLoyaltyDatabase::GetPointsTDString()
{
	CString strBuf = "";
	strBuf.Format ( "%d", GetPointsTD() );
	return strBuf;
}

//$$********************************************************************

CString CLoyaltyDatabase::GetPurse1BalanceString()
{
	CString strBuf = "";
	strBuf.Format ( "%.2lf", GetPurse1Balance() );
	return strBuf;
}

//$$********************************************************************

CString CLoyaltyDatabase::GetPurse2BalanceString()
{
	CString strBuf = "";
	strBuf.Format ( "%.2lf", GetPurse2Balance() );
	return strBuf;
}

//$$********************************************************************

CString CLoyaltyDatabase::GetTotalSpendTDString()
{
	CString strBuf = "";
	strBuf.Format ( "%.2lf", GetTotalSpendTD() );
	return strBuf;
}

//$$********************************************************************
// 0=unknown, 1=male, 2=female

void CLoyaltyDatabase::SetGender ( int nGender )
{	
	switch (nGender)
	{
	case 1:
		SetField(Loyalty::Gender.Label, "M", Loyalty::Gender.Max);
		break;

	case 2:
		SetField(Loyalty::Gender.Label, "F", Loyalty::Gender.Max);
		break;
	
	default:
		SetField(Loyalty::Gender.Label, "", Loyalty::Gender.Max);
		break;
	}
}

//$$********************************************************************

void CLoyaltyDatabase::SetGender(const char* szGender)
{
	CString strGender = szGender;
	strGender.MakeUpper();

	if (strGender.Left(1) == "M")
	{
		SetField(Loyalty::Gender.Label, "M", Loyalty::Gender.Max);
	}
	else if (strGender.Left(1) == "F")
	{
		SetField(Loyalty::Gender.Label, "F", Loyalty::Gender.Max);
	}
	else
	{
		SetField(Loyalty::Gender.Label, "", Loyalty::Gender.Max);
	}
}

//$$********************************************************************

CString CLoyaltyDatabase::GetGenderText()
{
	CString strGender = GetGender();
	
	if (strGender == "M")
	{
		strGender = "Male";
	}
	else if (strGender == "F")
	{
		strGender = "Female";
	}
	else
	{
		strGender = "unknown";
	}
	
	return strGender;
}

//$$********************************************************************
//$$* validation checking***********************************************
//$$********************************************************************

void CLoyaltyDatabase::SetField ( const char* strLabel, const char* szText, int nMax )
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
		if (strText.GetAt(i) < ' ')			//	conver Ctrl chars to space
		{
			strText.SetAt(i, ' ');
		}
	}

	CSSDatabase::Set ( strLabel, strText );
}

//$$********************************************************************

void CLoyaltyDatabase::SetField ( const char* szLabel, int nValue, int nMin, int nMax )
{
	if (nValue < nMin)
	{
		return;
	}

	if (nValue > nMax)
	{
		return;
	}

	CSSDatabase::Set ( szLabel, nValue );
}

//$$********************************************************************

void CLoyaltyDatabase::SetField(const char* szLabel, __int64 nValue, __int64 nMin, __int64 nMax)
{
	if (nValue < nMin)		return;
	if (nValue > nMax)		return;
	CSSDatabase::Set(szLabel, nValue);
}

//$$********************************************************************

void CLoyaltyDatabase::SetField ( const char* szLabel, double dValue, double dMin, double dMax )
{
	if ( dValue < dMin )		return;
	if ( dValue > dMax )		return;
	CSSDatabase::Set ( szLabel, dValue, 2 );
}

//$$********************************************************************

void CLoyaltyDatabase::SetDateField ( const char* szLabel, const char* szDate )
{
	CString strDate = szDate;
	if (strDate == " ")									//v2.03x 	05/01/2016	Setting a Date \ Time field with a space will clear it
	{
		strDate = "";
	}

	bool bOk = TRUE;
	int nLength = strDate.GetLength();
	if ( nLength > 0 )
	{
		if ( nLength == 8 )
		{
			if ( strDate.Find ( '/' ) == -1 )				// see if ddmmyyyy )
			{
				CString strNewDate;
				strNewDate.Format ( "%s/%s/%s", (const char*) strDate.Left(2), (const char*) strDate.Mid(2,2), (const char*) strDate.Right(4) );
				strDate = strNewDate;
			}
		}

		CSSDate date ( strDate );
		bOk = date.IsSet();
	}
	
	if (bOk == TRUE)
	{
		CSSDatabase::Set(szLabel, strDate);
	}
}

//$$********************************************************************

void CLoyaltyDatabase::SetTimeField ( const char* szLabel, const char* szTime )
{
	CString strTime = szTime;
	if (strTime == " ")									//v2.03x 	05/01/2016	Setting a Date \ Time field with a space will clear it
	{
		strTime = "";
	}

	bool bOk = TRUE;
	int nLength = strTime.GetLength();
	if ( nLength > 0 )
	{
		if ( nLength == 4 )									// see if hhmm
		{
			if ( strTime.Find ( ':' ) == -1 )
			{
				CString strNewTime;
				strNewTime.Format ( "%s:%s", (const char*) strTime.Left(2), (const char*) strTime.Right(2) );
				strTime = strNewTime;
			}
		}

		CSSTime time ( strTime );
		bOk = time.IsSet();
	}
	
	if (bOk == TRUE)
	{
		CSSDatabase::Set(szLabel, strTime);
	}
}

//$$********************************************************************

void CLoyaltyDatabase::SetCardNoField ( const char* szLabel, const char* szCardNo )
{
	CString strCardNo = szCardNo;

	if (IsValidCardNo(szCardNo, strCardNo, Loyalty::ExCardNo.Max) == TRUE)
	{
		CSSDatabase::Set(szLabel, strCardNo);
	}
}

//$$********************************************************************

bool CLoyaltyDatabase::IsValidCardNo ( const char* szCardNo, CString& strCardNo, int nMaxLength )
{
	strCardNo = szCardNo;
	strCardNo.TrimLeft('0');							// loose leading 0's

	int nLen = strCardNo.GetLength();

	if ( nLen > 0 && nLen <= nMaxLength )			// see if length ok
	{
		for ( int i = 0 ; i < nLen ; i++ )
		{
			if (strCardNo.GetAt(i) < '0' || strCardNo.GetAt(i) > '9')
			{
				return FALSE;
			}
		}

		return ( _atoi64 ( strCardNo ) > 0 ) ? TRUE : FALSE;
	}
	return FALSE;
}

//$$********************************************************************

void CLoyaltyDatabase::SetNextCardNo ( const char* szCardNo )
{
	CString strCardNo = szCardNo;
	strCardNo.TrimLeft('0');

	Set ( Loyalty::ExNextCardNo.Label, strCardNo );
}

//$$********************************************************************

void CLoyaltyDatabase::SetPreviousCardNo ( const char* szCardNo )
{
	CString strCardNo = szCardNo;
	strCardNo.TrimLeft('0');

	Set ( Loyalty::ExPreviousCardNo.Label, strCardNo );
}

//$$********************************************************************

void CLoyaltyDatabase::SetExtRef ( const char* szText )		
{ 
	SetField ( Loyalty::ExtRef.Label, szText, Loyalty::ExtRef.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetGroupNo ( int nGroupNo )			
{ 
	SetField ( Loyalty::GroupNo.Label, nGroupNo, Loyalty::GroupNo.Min, Loyalty::GroupNo.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPoints ( int nPoints )			
{ 
	SetField ( Loyalty::Points.Label, nPoints, Loyalty::Points.Min, Loyalty::Points.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPointsTD ( int nPoints )			
{ 
	SetField ( Loyalty::PointsTD.Label, nPoints, Loyalty::PointsTD.Min, Loyalty::PointsTD.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse1Balance ( double dValue )	
{ 
	SetField ( Loyalty::Purse1.Label, dValue, Loyalty::Purse1.Min, Loyalty::Purse1.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse1LastSpendDate(const char* szDate) 
{ 
	SetDateField(Loyalty::Purse1LastSpendDate.Label, szDate); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse1LastSpendTime(const char* szTime) 
{ 
	SetTimeField(Loyalty::Purse1LastSpendTime.Label, szTime); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse1LastSpend(double dValue) 
{ 
	SetField(Loyalty::Purse1LastSpend.Label, dValue, Loyalty::Purse1LastSpend.Min, Loyalty::Purse1LastSpend.Max); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse1SpendTD(double dValue) 
{ 
	SetField(Loyalty::Purse1SpendTD.Label, dValue, Loyalty::Purse1SpendTD.Min, Loyalty::Purse1SpendTD.Max); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2Balance ( double dValue )	
{ 
	SetField ( Loyalty::Purse2.Label, dValue, Loyalty::Purse2.Min, Loyalty::Purse2.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2LastSpendDate(const char* szDate) 
{ 
	SetDateField(Loyalty::Purse2LastSpendDate.Label, szDate); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2LastSpendTime(const char* szTime) 
{ 
	SetTimeField(Loyalty::Purse2LastSpendTime.Label, szTime);
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2LastSpend ( double dValue )		
{ 
	SetField ( Loyalty::Purse2LastSpend.Label, dValue, Loyalty::Purse2LastSpend.Min, Loyalty::Purse2LastSpend.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2SpendTD(double dValue) 
{ 
	SetField(Loyalty::Purse2SpendTD.Label, dValue, Loyalty::Purse2SpendTD.Min, Loyalty::Purse2SpendTD.Max); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2RefreshedDate(const char* szDate) 
{ 
	SetDateField(Loyalty::Purse2RefreshedDate.Label, szDate); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetPurse2RefreshedTime(const char* szTime) 
{ 
	SetTimeField(Loyalty::Purse2RefreshedTime.Label, szTime); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetCashLastSpendDate(const char* szDate) 
{ 
	SetDateField(Loyalty::CashLastSpendDate.Label, szDate); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetCashLastSpendTime(const char* szTime) 
{ 
	SetTimeField(Loyalty::CashLastSpendTime.Label, szTime); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetCashLastSpend(double dValue)			
{ 
	SetField ( Loyalty::CashLastSpend.Label, dValue, Loyalty::CashLastSpend.Min, Loyalty::CashLastSpend.Max); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetMaxSpend ( double dValue )	
{	
	SetField ( Loyalty::MaxSpend.Label, dValue, Loyalty::MaxSpend.Min, Loyalty::MaxSpend.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetOverSpend ( double dValue )	
{	
	SetField ( Loyalty::OverSpend.Label, dValue, Loyalty::OverSpend.Min, Loyalty::OverSpend.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetCashSpendTD ( double dValue )	
{	
	SetField ( Loyalty::CashSpendTD.Label, dValue, Loyalty::CashSpendTD.Min, Loyalty::CashSpendTD.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetFullName ( const char* szText )	
{	
	SetField ( Loyalty::FullName.Label, szText, Loyalty::FullName.Max );
}

//$$********************************************************************

void CLoyaltyDatabase::SetForename ( const char* szText )	
{	
	SetField ( Loyalty::Forename.Label, szText, Loyalty::Forename.Max );
}

//$$********************************************************************

void CLoyaltyDatabase::SetSurname ( const char* szText )	
{	
	SetField ( Loyalty::Surname.Label,  szText, Loyalty::Surname.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetAdd1 ( const char* szText )		
{
	SetField ( Loyalty::Add1.Label, szText, Loyalty::Add1.Max );
}

//$$********************************************************************

void CLoyaltyDatabase::SetAdd2 ( const char* szText )		
{	
	SetField ( Loyalty::Add2.Label, szText, Loyalty::Add2.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetAdd3 ( const char* szText )		
{	
	SetField ( Loyalty::Add3.Label, szText, Loyalty::Add3.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetAdd4 ( const char* szText )		
{	
	SetField ( Loyalty::Add4.Label, szText, Loyalty::Add4.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetAdd5 ( const char* szText )		
{	
	SetField ( Loyalty::Add5.Label, szText, Loyalty::Add5.Max );
}

//$$********************************************************************

void CLoyaltyDatabase::SetTel1 ( const char* szText )		
{ 
	SetField ( Loyalty::Tel1.Label, szText, Loyalty::Tel1.Max );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetTel2 ( const char* szText )		
{ 
	SetField ( Loyalty::Tel2.Label, szText, Loyalty::Tel2.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetInfo1 ( const char* szText )		
{ 	
	SetField ( Loyalty::Info1.Label, szText, Loyalty::Info1.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetInfo2 ( const char* szText )		
{ 	
	SetField ( Loyalty::Info2.Label, szText, Loyalty::Info2.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetInfo3 ( const char* szText )		
{ 	
	SetField ( Loyalty::Info3.Label, szText, Loyalty::Info3.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetInfo4 ( const char* szText )		
{ 	
	SetField ( Loyalty::Info4.Label, szText, Loyalty::Info4.Max );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetAlertCodes ( const char* szCodes )	 
{ 
	SetField ( Loyalty::AlertCodes.Label, szCodes, Loyalty::AlertCodes.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::SetExpiry ( const char* szDate )		 
{	
	SetDateField ( Loyalty::ExpiryDate.Label, szDate );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetDob ( const char* szDate )		 
{	
	SetDateField ( Loyalty::Dob.Label, szDate );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetHotlistDate ( const char* szDate ) 
{	
	SetDateField ( Loyalty::HotlistDate.Label, szDate );		
}

//$$********************************************************************

void CLoyaltyDatabase::SetHotlistTime ( const char* szTime ) 
{	
	SetTimeField ( Loyalty::HotlistTime.Label, szTime );	
}

//$$********************************************************************

void CLoyaltyDatabase::SetHotlistCode ( int nCode )			 
{	
	SetField ( Loyalty::HotlistCode.Label, nCode, Loyalty::HotlistCode.Min, Loyalty::HotlistCode.Max ); 
}

//$$********************************************************************

void CLoyaltyDatabase::RefreshPurse2 ( double dValue, const char* szDate, const char* szTime )
{
	if (lstrlen(szDate) == 0)
	{
		SetPurse2RefreshedDate(CSSDate::GetCurrentDate().GetDate());
	}
	else
	{
		SetPurse2RefreshedDate(szDate);
	}

	if (lstrlen(szTime) == 0)
	{
		SetPurse2RefreshedTime(CSSTime::GetCurrentTime().GetTime());
	}
	else
	{
		SetPurse2RefreshedTime(szTime);
	}

	SetPurse2Balance( dValue );
	SetPurse2LastSpend ( 0 );
	SetPurse2LastSpendDate ( "" );
	SetPurse2LastSpendTime ( "" );
}

//$$********************************************************************
//$$ "Y" or "-" if exported report or bool if exported database

bool CLoyaltyDatabase::GetBoolData ( const char* szData )
{
	CString strData = szData;
	strData.MakeUpper();

	if ( strData == "1" )			return TRUE;			// bool 'yes'
	if ( strData.Left(1) == "Y" )	return TRUE;			// Y'es'
	if ( strData.Left(1) == "T" )	return TRUE;			// T'rue'
	if ( strData == "X" )			return TRUE;			// old indicator ( now 'Y' used )

	return FALSE;
}

//$$********************************************************************
//$$ split fullname into Forename & Surname

void CLoyaltyDatabase::DivideCardName ( const char* szCardName, CString& strFirstName, CString& strSecondName )
{
	CString strFullName = szCardName;
	int nLength = strFullName.GetLength();

	strFirstName	= "";
	strSecondName	= "";

	int i;
	for ( i = nLength-1 ; i >= 0 ; i-- )
	{
		char c = strFullName.GetAt(i);
		if (c == ' ')									// look for space in cardname ( 'Fred Smith' )
		{
			break;
		}

		strSecondName += c;								// compile surname backwards
	}
	strSecondName.MakeReverse();						// get surname right way round

	if (i > 0)										// see if any cardname left 
	{
		strFirstName = strFullName.Left(i);
	}
}

//$$********************************************************************

void CLoyaltyDatabase::CopyToDatabase ( CLoyaltyDatabase* pDbCopy )
{
	bool bReply = pDbCopy->AddBlankRecord(GetCardNo());

	CString strLabel = "";
	CString strData = "";

	for (int n = 0; n < GetHeaderSize(); n++)
	{
		strLabel = GetHeaderLabel(n);

		strData = GetString(strLabel);
		pDbCopy->Set(strLabel, strData);
	}
}

//$$********************************************************************

bool CLoyaltyDatabase::CopyRecordToFile ( const char* szFilename )
{
	bool bReply;
	CSSFile file;
	if ( ( bReply = file.Open ( szFilename, "wb" ) ) == TRUE )
	{
		if ( ( bReply = file.WriteLine ( m_strHeader ) ) == TRUE )
		{
			CCSV csvLine;
			CCSV csvHeader ( m_strHeader );

			for (int n = 0; n < csvHeader.GetSize(); n++)
			{
				csvLine.Add(GetString(csvHeader.GetString(n)));
			}

			if ((bReply = file.Write(csvLine)) == FALSE)
			{
				m_strError = "CRF Error 3";
			}
		}
		else
		{
			m_strError = "CRF Error 2";
		}

		file.Close();
	}
	else
	{
		m_strError = "CRF Error 1";
	}

	return bReply;
}

//$$********************************************************************

bool CLoyaltyDatabase::CopyFileToRecord ( const char* szFilename )
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
				for (int n = 0; n < csvHeader.GetSize(); n++)
				{
					Set(csvHeader.GetString(n), csvLine.GetString(n));
				}
			}
			else
			{
				m_strError = "CFR Error 3";
			}
		}
		else
		{
			m_strError = "CFR Error 2";
		}

		file.Close();
	}
	else
	{
		m_strError = "CFR Error 1";
	}

	return bReply;
}

//$$********************************************************************
