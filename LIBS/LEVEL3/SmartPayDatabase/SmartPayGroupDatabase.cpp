/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

static const char* szDAYS[]		= { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* szWEEKDAYS[]	= { "Sunday", "Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char* szMONTHS[]	= { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

static const char* szSORTFIELD	= "GroupNo,ascending";

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CSmartPayGroupDatabase::OpenReadOnlyForSharing( const char* szFilename )
{
	return OpenForSharing( szFilename, "", DB_READONLY );
}

/******************************************************************************/

int CSmartPayGroupDatabase::OpenReadWriteForSharing( const char* szFilename, const char* szBackupFolder )
{
	return OpenForSharing( szFilename, szBackupFolder, DB_READWRITE );
}

/******************************************************************************/

int CSmartPayGroupDatabase::OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase( szFilename, szBackupFolder, m_strHeader, szSORTFIELD, nRetries );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRetries );

	int nReply = OpenSSDatabaseReadWrite ( szFilename, szBackupFolder, szSORTFIELD, nRetries );

	int nIndex = GetHeaderIndex ( "RDFlag" );						// check for latest header // 20/06/2017
	if ( nIndex == -1 )												// not found
	{
		Close();																			// close file
		nReply = OpenSSDatabaseReadWriteNewHeader ( szFilename, szBackupFolder, m_strHeader, "GroupNo", nRetries );	// read and write and modify header (file must exist)
	}

	return nReply;

}	

/**********************************************************************/

void CSmartPayGroupDatabase::SetNewRecord()
{
	SetUserType ( SmartPayGroup::UserType.Min );
	SetSetNo ( SmartPayGroup::SetNo.Min );
	SetPriceLevel ( SmartPayGroup::PriceLevel.Min );
	Set ( SmartPayGroup::RefreshAllowed.Label, SmartPayGroup::RefreshAllowed.Min );
	Set ( SmartPayGroup::Purse1Allowed.Label, SmartPayGroup::Purse1Allowed.Max );
	Set ( SmartPayGroup::Purse2Allowed.Label, SmartPayGroup::Purse2Allowed.Min );
	Set ( SmartPayGroup::Purse3Allowed.Label, SmartPayGroup::Purse3Allowed.Min );
}

CString CSmartPayGroupDatabase::GetListboxText()
{
	CString strLabel = GetString(SmartPayGroup::GroupNo.Label);
	CString strName = GetName();
	
	// first 3 chars must be group number so it can be extracted from listbox
	CString strBuf;					
	strBuf.Format ( "%3s : %s", (const char*) strLabel, (const char*) strName );

	return strBuf;
}

//*********************************************************************

int CSmartPayGroupDatabase::IncrementGroupNo()
{
	int nGroupNo = GetGroupNo();						// get current card number

	if ( nGroupNo < SmartPayGroup::GroupNo.Max )
		++nGroupNo;

	return nGroupNo;
}

//*******************************************************************

CString CSmartPayGroupDatabase::GetRefreshTypeText()
{
	CString strText = "";
	switch ( GetRefreshType() )
	{
	case nREFRESHTYPE_DISABLED:		strText = "No";			break;
	case nREFRESHTYPE_DAILY:		strText = "Daily";		break;
	case nREFRESHTYPE_WEEKLY:		strText = "Weekly";		break;	
	case nREFRESHTYPE_MONTHLY:		strText = "Monthly";	break;
	case nREFRESHTYPE_YEARLY:		strText = "Yearly";		break;
	case nREFRESHTYPE_MANUAL:		strText = "Manual";		break;
	case nREFRESHTYPE_PERIOD:		strText = "Period";		break;
	case nREFRESHTYPE_EOD:			strText = "EOD";		break;
	}
	return strText;
}

bool CSmartPayGroupDatabase::IsPurse2Refreshable()
{
	bool bReply = TRUE;						//  assume Purse2 can be refreshed

	switch ( GetRefreshType() )
	{
	case nREFRESHTYPE_DISABLED:		
	case nREFRESHTYPE_MANUAL:		
		bReply = FALSE;
		break;

	case nREFRESHTYPE_DAILY:		
	case nREFRESHTYPE_WEEKLY:		
	case nREFRESHTYPE_MONTHLY:		
	case nREFRESHTYPE_YEARLY:		
	case nREFRESHTYPE_PERIOD:	
	case nREFRESHTYPE_EOD:
	default:						
		bReply = TRUE;							
		break;
	}
	return bReply;
}


//******************************************************************
// nDay 0=Sun, Mon, 6 = Sat

bool CSmartPayGroupDatabase::IsRefreshAllowed ( int nDay )
{
	if ( nDay >= 0 )
	{
		int nMask = 0x01;
		int nFlags = GetRefreshFlags();

		nMask = nMask << nDay;
		return ( (nFlags & nMask) == nMask ) ? TRUE : FALSE;
	}
	return FALSE;
}

void CSmartPayGroupDatabase::SetRefreshAllowed ( int nDay, bool bSet )
{
	int nMask = 0x01;
	int nFlags = GetRefreshFlags();

	nMask = nMask << nDay;
	if ( bSet == TRUE )		nFlags |= nMask;
	else					nFlags &= ~nMask;

	SetRefreshFlags ( nFlags);
}

CString CSmartPayGroupDatabase::GetRefreshDaysText()
{
	int nCount = 0;
	CString strMsg;

	for ( int nDay = 0 ; nDay < 7 ; nDay++ )
	{
		if ( IsRefreshAllowed ( nDay ) == TRUE )
		{
			if ( nCount > 0 )
				strMsg += ", ";

			strMsg += szDAYS[nDay];
			++nCount;
		}
	}
	return strMsg;
}

//** weekly refresh

CString CSmartPayGroupDatabase::GetRefreshWeekText()
{
	CString strMsg;
	for ( int nDay = 0 ; nDay < 7 ; nDay++ )
	{
		if ( IsRefreshAllowed ( nDay ) == TRUE )
		{
			strMsg = szWEEKDAYS[nDay];
			break;
		}
	}
	return strMsg;
}

int CSmartPayGroupDatabase::GetRefreshWeeklyDayNo ( int nDefaultDayNo )
{
	for ( int nDay = 0 ; nDay < 7 ; nDay++ )
	{
		if ( IsRefreshAllowed ( nDay ) == TRUE )
			return nDay;
	}
	return nDefaultDayNo;										// 0=Sun, 1-Mon etc
}

void CSmartPayGroupDatabase::SetRefreshWeeklyDayNo ( int nDayNo )
{
	ClearRefreshFlags();
	SetRefreshAllowed ( nDayNo, TRUE );
}

CString CSmartPayGroupDatabase::GetRefreshMonthText()
{
	CString strMsg = "Once per Month";
	return strMsg;
}

//** Yearly refresh

void CSmartPayGroupDatabase::GetRefreshYearDate( int& nDayNo, int& nMonthNo )
{
	int nFactor = GetRefreshFlags();
	if ( nFactor >100 && nFactor <= 1231 )				// 01Jan = ( 1 x 100 + 1 ) = 31Dec = ( 12 x 100 + 31 )
	{
		nMonthNo = nFactor / 100;
		nDayNo = nFactor - ( nMonthNo * 100 );
	}
	else
	{
	 	nDayNo = 1;
		nMonthNo = 1;
	}
}

void CSmartPayGroupDatabase::SetRefreshYearDate( int nDayNo, int nMonthNo )
{
	int nFactor = ( nMonthNo * 100 ) + nDayNo;
	SetRefreshFlags ( nFactor );
}

CString  CSmartPayGroupDatabase::GetRefreshYearText()
{
	int nDayNo;
	int nMonthNo;
	GetRefreshYearDate( nDayNo, nMonthNo );

	CString strMsg;
	strMsg.Format ( "%2.2d %s", nDayNo, szMONTHS[nMonthNo-1] );

	return strMsg;
}

//******************************************************************
// nPurse 1 0 2 , AuditPeriod 0 - 5

bool CSmartPayGroupDatabase::IsPurseAllowed ( int nPurseNo, int nPeriodIndex )
{

	CString strLabel;
	switch ( nPurseNo )
	{
	case 1:	strLabel = SmartPayGroup::Purse1Allowed.Label;	break;
	case 2:	strLabel = SmartPayGroup::Purse2Allowed.Label;	break;
	case 3:	strLabel = SmartPayGroup::Purse3Allowed.Label;	break;
	}

	int nFlags = CSSDatabase::GetInt(strLabel);
	int nMask = 0x01;

	nMask = nMask << nPeriodIndex;
	return ( (nFlags & nMask) == nMask ) ? TRUE : FALSE;
}

void CSmartPayGroupDatabase::SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet )
{
	CString strLabel;
	switch ( nPurseNo )
	{
	case 1:	strLabel = SmartPayGroup::Purse1Allowed.Label;	break;
	case 2:	strLabel = SmartPayGroup::Purse2Allowed.Label;	break;
	case 3:	strLabel = SmartPayGroup::Purse3Allowed.Label;	break;
	}

	int nFlags = CSSDatabase::GetInt(strLabel);
	int nMask = 0x01;
	nMask = nMask << nPeriodIndex;
	if ( bSet == TRUE )		nFlags |= nMask;
	else					nFlags &= ~nMask;

	CSSDatabase::Set (strLabel, nFlags);
}

//*********************************************************************
// 6 period refresh values ( using indexes 0 - 5 )

bool CSmartPayGroupDatabase::HavePeriodRefresh()
{
	bool bReply = FALSE;

	for ( int n = 0 ; n < 6 ; n++ )
	{
		if ( GetPeriodRefreshValue ( n ) != 0 )					// see if period refesh value set
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

double CSmartPayGroupDatabase::GetPeriodRefreshValue( int nIndex )
{
	CString strLabel = "";

	switch ( nIndex )
	{
	case 0:	strLabel = SmartPayGroup::RefreshPeriod1.Label;	break;
	case 1:	strLabel = SmartPayGroup::RefreshPeriod2.Label;	break;
	case 2:	strLabel = SmartPayGroup::RefreshPeriod3.Label;	break;
	case 3:	strLabel = SmartPayGroup::RefreshPeriod4.Label;	break;
	case 4:	strLabel = SmartPayGroup::RefreshPeriod5.Label;	break;
	case 5:	strLabel = SmartPayGroup::RefreshPeriod6.Label;	break;
	}
	
	return ( strLabel == "" ) ? 0 : CSSDatabase::GetDouble ( strLabel );
}

void CSmartPayGroupDatabase::SetPeriodRefreshValue ( int nIndex, double dValue )
{
	CString strLabel = "";

	switch ( nIndex )
	{
	case 0:	strLabel = SmartPayGroup::RefreshPeriod1.Label;	break;
	case 1:	strLabel = SmartPayGroup::RefreshPeriod2.Label;	break;
	case 2:	strLabel = SmartPayGroup::RefreshPeriod3.Label;	break;
	case 3:	strLabel = SmartPayGroup::RefreshPeriod4.Label;	break;
	case 4:	strLabel = SmartPayGroup::RefreshPeriod5.Label;	break;
	case 5:	strLabel = SmartPayGroup::RefreshPeriod6.Label;	break;
	}

	if ( strLabel != "" )
		CSSDatabase::Set ( strLabel, dValue, 2 );
}

int CSmartPayGroupDatabase::GetRefreshPeriodType ( int nIndex )
{
	CString strLabel = "";

	switch ( nIndex )
	{
	case 0:	strLabel = SmartPayGroup::RefreshPeriod1Type.Label;	break;
	case 1:	strLabel = SmartPayGroup::RefreshPeriod2Type.Label;	break;
	case 2:	strLabel = SmartPayGroup::RefreshPeriod3Type.Label;	break;
	case 3:	strLabel = SmartPayGroup::RefreshPeriod4Type.Label;	break;
	case 4:	strLabel = SmartPayGroup::RefreshPeriod5Type.Label;	break;
	case 5:	strLabel = SmartPayGroup::RefreshPeriod6Type.Label;	break;
	}

	return ( strLabel == "" ) ? 0 : CSSDatabase::GetInt ( strLabel );
}

void CSmartPayGroupDatabase::SetRefreshPeriodType ( int nIndex, int nType )
{
	CString strLabel = "";

	switch ( nIndex )
	{
	case 0:	strLabel = SmartPayGroup::RefreshPeriod1Type.Label;	break;
	case 1:	strLabel = SmartPayGroup::RefreshPeriod2Type.Label;	break;
	case 2:	strLabel = SmartPayGroup::RefreshPeriod3Type.Label;	break;
	case 3:	strLabel = SmartPayGroup::RefreshPeriod4Type.Label;	break;
	case 4:	strLabel = SmartPayGroup::RefreshPeriod5Type.Label;	break;
	case 5:	strLabel = SmartPayGroup::RefreshPeriod6Type.Label;	break;
	}
	if ( strLabel != "" )
		CSSDatabase::Set ( strLabel, nType );
}

CString CSmartPayGroupDatabase::GetRefreshPeriodTypeText ( int nIndex )
{
	CString strText = "";

	switch ( GetRefreshPeriodType ( nIndex ) )
	{
	case 0:	strText = "Daily";		break;
	case 1:	strText = "Weekly";		break;
	case 2:	strText = "Monthly";	break;
	case 3:	strText = "Yearly";		break;
	}

	return strText;
}

//*********************************************************************
// 20:21:22-30
bool CSmartPayGroupDatabase::ValidateDeptInhibitList ( const char* szText )
{
	bool bReply = TRUE;
	CString strList = szText;

	for ( int i = 0 ; i < strList.GetLength() ; i++ )
	{
		unsigned char uc = strList.GetAt(i);

		if ( uc == ':' || uc == '-' || uc == ' ' )				// see if separator
			continue;

		if ( uc < '0' || uc > '9' )
		{
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

void CSmartPayGroupDatabase::DisplayDeptListError ( CWnd* pParent )
{
	CString strMsg = "The list contains invalid characters!\n\nUse only digits 0 through 9, '-' or ':'.\n\nFor example:  1:20:25-30";
	pParent->MessageBox ( strMsg, "Department Inhbit List" );
}

//*********************************************************************

CString CSmartPayGroupDatabase::RemoveSpaces ( const char* szText )
{
	CString strList = szText;
	int nPtr = 0;
	int nLength = strList.GetLength();

	for ( int i = 0 ; i < nLength ; i++ )
	{
		unsigned char uc = strList.GetAt(i);

		if ( uc == ' ' )						// see if space
			continue;							// yes skip
		else
			strList.SetAt ( nPtr++, uc );		// save non space
	}

	if ( nPtr < nLength )						// see if spaces been removed
		strList.SetAt ( nPtr, 0 );				// yes - set new end of string

	return strList;
}

//*********************************************************************

/*
bool CSmartPayGroupDatabase::HaveDeptRestrictionsP3()
{
	CString strList = GetDeptRestrictedP3List();
	return ( strList.GetLength() == 0 ) ? FALSE : TRUE;
}
*/

//*********************************************************************

int CSmartPayGroupDatabase::GetMaxSpendType()
{
	int nType = CSSDatabase::GetInt ( SmartPayGroup::MaxSpendType.Label );

	switch( nType )
	{
	case nOLD_GRPSPENDTYPE_DAILY:
		return nGRPSPENDTYPE_DAILY_LOCKED;

	case nOLD_GRPSPENDTYPE_RECORD:
		return nGRPSPENDTYPE_DAILY_RECORD1;

	case nOLD_GRPSPENDTYPE_PERIOD:
		return nGRPSPENDTYPE_PERIOD_LOCKED;
	
	case nGRPSPENDTYPE_DAILY_LOCKED:
	case nGRPSPENDTYPE_DAILY_RECORD1:
	case nGRPSPENDTYPE_DAILY_RECORD2:
	case nGRPSPENDTYPE_PERIOD_LOCKED:
	case nGRPSPENDTYPE_PERIOD_RECORD1:
	case nGRPSPENDTYPE_PERIOD_RECORD2:
		return nType;

	default:
		return nGRPSPENDTYPE_DAILY_LOCKED;
	}
}

//*********************************************************************
