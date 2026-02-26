//*******************************************************************

static const char* szDAYS[]		= { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* szWEEKDAYS[]	= { "Sunday", "Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char* szMONTHS[]	= { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

//	  		 szHEADER_V1 = "GroupNo,Name,RFlag,RTrip,RTrip2,RType,RPts,RVal,LFlag,LTrip,LType,LPts,LVal,LCard,LTrig,LUlmt,OverSpend";
//			 szHEADER_V2 = "GroupNo,Name,RFType,RFAllow,RFVal,P1Allow,P2Allow,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,RFlag,RTrip,RTrip2,RType,RPts,RVal,LFlag,LTrip,LType,LPts,LVal,LCard,LTrig,LUlmt,OverSpend,OSV2,OSV3,OSV4,OSV5,OSV6";
//			 szHEADER_V3 = "GroupNo,Name,Compment,RFType,RFAllow,RFVal,P1Allow,P2Allow,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,RFlag,RTrip,RTrip2,RType,RPts,RVal,LFlag,LTrip,LType,LPts,LVal,LCard,LTrig,LUlmt,OverSpend,OSV2,OSV3,OSV4,OSV5,OSV6";
//			 szHEADER_V4 = "GroupNo,Name,Compment,RFType,RFAllow,RFVal,P1Allow,P2Allow,NoPts,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,RFlag,RTrip,RTrip2,RType,RPts,RVal,RDis,LFlag,LTrip,LType,LPts,LVal,LTrig,LUlmt,LDis,RDFlag,RDTrp,RDVal,RDTyp,RDDis,OverSpend,OSV2,OSV3,OSV4,OSV5,OSV6";
//			 szHEADER_V5 = "GroupNo,Name,Compment,RFType,RFAllow,RFVal,P1Allow,P2Allow,NoPts,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,RFlag,RTrip,RTrip2,RType,RPts,RVal,RPcnt,RDis,LFlag,LTrip,LType,LPts,LVal,LTrig,LUlmt,LDis,RDFlag,RDTrp,RDVal,RDTyp,RDDis,OverSpend,OSV2,OSV3,OSV4,OSV5,OSV6";
static const char* szHEADER_V6 = "GroupNo,Name,SchemeNo,Compment,RFType,RFAllow,RFVal,P1Allow,P2Allow,NoPts,MSType,MSV1,MSV2,MSV3,MSV4,MSV5,MSV6,RFlag,RTrip,RTrip2,RType,RPts,RVal,RPcnt,RDis,LFlag,LTrip,LType,LPts,LVal,LTrig,LUlmt,LDis,RDFlag,RDTrp,RDVal,RDTyp,RDDis,OverSpend,OSV2,OSV3,OSV4,OSV5,OSV6";
static const char* szSORTFIELD = "GroupNo,ascending";

//*******************************************************************
//	mode = DB_READONLY, DB_READWRITE

int CLoyaltyGroupDatabase::OpenReadOnlyForSharing ( const char* szFilename )
{
	return OpenForSharing( szFilename, "", DB_READONLY );
}

//*******************************************************************

int CLoyaltyGroupDatabase::OpenReadWriteForSharing ( const char* szFilename, const char* szBackupFolder )
{
	return OpenForSharing( szFilename, szBackupFolder, DB_READWRITE );
}

//*******************************************************************

int CLoyaltyGroupDatabase::OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase( szFilename, szBackupFolder, szHEADER_V6, szSORTFIELD, nRetries );			// create db if not found
	Close();

	int nReply;
	if ( nMode == DB_READONLY )
		nReply = OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRetries );
	else
	{
		nReply = OpenSSDatabaseReadWrite ( szFilename, szBackupFolder, szSORTFIELD, nRetries );
	
		int nIndex = GetHeaderIndex ( "SchemeNo" );						// check for latest header
		if ( nIndex == -1 )												// not found
		{
			Close();													// close file
			nReply = OpenSSDatabaseReadWriteNewHeader ( szFilename, szBackupFolder, szHEADER_V6, szSORTFIELD, nRetries );// read and write and modify header (file must exist)
		}
	}

	return nReply;
}	

//*********************************************************************

CString CLoyaltyGroupDatabase::GetListboxText()
{
	CString strBuf;					// first 3 chars must be group number so it can be extracted from listbox
	strBuf.Format ( "%3s : %s", (const char*) CSSDatabase::GetString(LoyaltyGroup::GroupNo.Label), (const char*) GetName() );

	return strBuf;
}

//*********************************************************************

int CLoyaltyGroupDatabase::IncrementGroupNo()
{
	int nGroupNo = GetGroupNo();						// get current card number

	if ( nGroupNo < LoyaltyGroup::GroupNo.Max )
		++nGroupNo;

	return nGroupNo;
}

//*******************************************************************

CString CLoyaltyGroupDatabase::GetRefreshTypeText()
{
	CString strText = "";
	switch ( GetRefreshType() )
	{
	case nREFRESHTYPE_NORMAL:		strText = "No";			break;
	case nREFRESHTYPE_DAILY:		strText = "Daily";		break;
	case nREFRESHTYPE_WEEKLY:		strText = "Weekly";		break;	
	case nREFRESHTYPE_MONTHLY:		strText = "Monthly";	break;
	case nREFRESHTYPE_YEARLY:		strText = "Yearly";		break;
	}
	return strText;
}

//******************************************************************
// nDay 0=Sun, Mon, 6 = Sat

bool CLoyaltyGroupDatabase::IsRefreshAllowed ( int nDay )
{
	bool bResult = FALSE;

	if ( ( nDay >= 0 ) && ( nDay <= 6 ) )
	{
		int nFlags = GetRefreshFlags();
		int nMask = 0x01 << nDay;
		bResult = ( ( nFlags & nMask ) == nMask );
	}
	
	return bResult;
}

void CLoyaltyGroupDatabase::SetRefreshAllowed ( int nDay, bool bSet )
{
	if ( ( nDay >= 0 ) && ( nDay <= 6 ) )
	{
		int nFlags = GetRefreshFlags();
		int nMask = 0x01 << nDay;

		if ( bSet == TRUE )		nFlags |= nMask;
		else					nFlags &= ~nMask;

		nFlags &= 0x7F;

		SetRefreshFlags ( nFlags);
	}
}

CString CLoyaltyGroupDatabase::GetRefreshDaysText()
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

CString CLoyaltyGroupDatabase::GetRefreshWeekText()
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

int CLoyaltyGroupDatabase::GetRefreshWeeklyDayNo ( int nDefaultDayNo )
{
	for ( int nDay = 0 ; nDay < 7 ; nDay++ )
	{
		if ( IsRefreshAllowed ( nDay ) == TRUE )
			return nDay;
	}
	return nDefaultDayNo;										// 0=Sun, 1-Mon etc
}

void CLoyaltyGroupDatabase::SetRefreshWeeklyDayNo ( int nDayNo )
{
	ClearRefreshFlags();
	SetRefreshAllowed ( nDayNo, TRUE );
}

CString CLoyaltyGroupDatabase::GetRefreshMonthText()
{
	CString strMsg = "Once per Month";
	return strMsg;
}

//** Yearly refresh

bool CLoyaltyGroupDatabase::ValidateYearlyRefreshFactor( int nFactor )
{
	if ( ( nFactor >= 101 ) && ( nFactor <= 131 ) )
		return TRUE;
	else if ( ( nFactor >= 201 ) && ( nFactor <= 228 ) )
		return TRUE;
	else if ( ( nFactor >= 301 ) && ( nFactor <= 331 ) )
		return TRUE;
	else if ( ( nFactor >= 401 ) && ( nFactor <= 430 ) )
		return TRUE;
	else if ( ( nFactor >= 501 ) && ( nFactor <= 531 ) )
		return TRUE;
	else if ( ( nFactor >= 601 ) && ( nFactor <= 630 ) )
		return TRUE;
	else if ( ( nFactor >= 701 ) && ( nFactor <= 731 ) )
		return TRUE;
	else if ( ( nFactor >= 801 ) && ( nFactor <= 831 ) )
		return TRUE;
	else if ( ( nFactor >= 901 ) && ( nFactor <= 930 ) )
		return TRUE;
	else if ( ( nFactor >= 1001 ) && ( nFactor <= 1031 ) )
		return TRUE;
	else if ( ( nFactor >= 1101 ) && ( nFactor <= 1130 ) )
		return TRUE;
	else if ( ( nFactor >= 1200 ) && ( nFactor <= 1231 ) )
		return TRUE;
	else
		return FALSE;
}

void CLoyaltyGroupDatabase::GetRefreshYearDate( int& nDayNo, int& nMonthNo )
{
	int nFactor = GetRefreshFlags();
	
	if ( ValidateYearlyRefreshFactor( nFactor ) == TRUE )
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

void CLoyaltyGroupDatabase::SetRefreshYearDate( int nDayNo, int nMonthNo )
{
	if ( ( nMonthNo >= 1 ) && ( nMonthNo <= 12 ) )
	{
		if ( ( nDayNo >= 1 ) && ( nDayNo <= 31 ) )
		{
			int nFactor = ( nMonthNo * 100 ) + nDayNo;

			if ( ValidateYearlyRefreshFactor( nFactor ) == TRUE )
				SetRefreshFlags ( nFactor );
		}
	}
}

CString  CLoyaltyGroupDatabase::GetRefreshYearText()
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

bool CLoyaltyGroupDatabase::IsPurseAllowed ( int nPurseNo, int nPeriodIndex )
{
	int nFlags;
	int nMask = 0x01;
	
	if ( nPurseNo == 1 )	nFlags = CSSDatabase::GetInt(LoyaltyGroup::Purse1Allowed.Label);
	else					nFlags = CSSDatabase::GetInt(LoyaltyGroup::Purse2Allowed.Label);

	nMask = nMask << nPeriodIndex;
	return ( (nFlags & nMask) == nMask ) ? TRUE : FALSE;
}

void CLoyaltyGroupDatabase::SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet )
{
	CString strLabel;
	if ( nPurseNo == 1 )	strLabel = LoyaltyGroup::Purse1Allowed.Label;
	else					strLabel = LoyaltyGroup::Purse2Allowed.Label;

	int nFlags = CSSDatabase::GetInt(strLabel);

	int nMask = 0x01;
	nMask = nMask << nPeriodIndex;
	if ( bSet == TRUE )		nFlags |= nMask;
	else					nFlags &= ~nMask;

	CSSDatabase::Set (strLabel, nFlags);
}

//*********************************************************************

int CLoyaltyGroupDatabase::GetRevaluationPercent()
{
	int nPcnt = CSSDatabase::GetInt ( LoyaltyGroup::RevaluePercent.Label );
	if ( ( nPcnt >= LoyaltyGroup::RevaluePercent.Min ) && ( nPcnt <= LoyaltyGroup::RevaluePercent.Max ) )
		return nPcnt;
	else
		return 0;
}

//*********************************************************************

void CLoyaltyGroupDatabase::SetRevaluationPercent(int nPcnt)
{
	if ( ( nPcnt >= LoyaltyGroup::RevaluePercent.Min ) && ( nPcnt <= LoyaltyGroup::RevaluePercent.Max ) )
		CSSDatabase::Set ( LoyaltyGroup::RevaluePercent.Label, nPcnt );
}

//*********************************************************************
