/**********************************************************************/
/*														*/
/*	0 = ok												*/
/*	1 = serial number has not been issued						*/
/*	2 = software has already been registered					*/
/*	3 = Registration has been disabled							*/
/*														*/
/**********************************************************************/

static int Register ( CSSIntDatabase& db, const char* szSerialNo, bool bUserReg, bool bCheckDisable )
{
	CTime tm = CTime::GetCurrentTime();
	CString strDate = tm.Format ( "%d%m%Y" );	// ddmmyyyy

	if ( db.FindRecord ( atoi(szSerialNo) ) == TRUE )
	{
		if ( bCheckDisable == TRUE )
		{
			if ( db.GetBool ( "RegDisable" ) == TRUE )
				return 3;		// registration has been disabled
		}

		int nCount = db.GetInt ( "RegCount" );
		if ( nCount == 0 )		// has not been yet been registered
			db.Set ( "FirstReg", strDate );	// date first registered

		nCount++;		// increment count

		if ( bUserReg == FALSE )		// unlimited registrations
		{
			db.Set ( "RegCount", nCount );
			if ( nCount > 1 )
				db.Set ( "LastReg", strDate );
		}
		else						// end user registration
		{
			if ( nCount <= db.GetInt ( "MaxUserReg" ) )	// max number of user registrations allowed
			{
				db.Set ( "RegCount", nCount );
				if ( nCount > 1 )
					db.Set ( "LastReg", strDate );
			}
			else
				return 2;		// software has already been registered
		}
		return 0;		// ok
	}
	else
		return 1;		// serial number has not been issued
}

/**********************************************************************/
/*														*/
/*	0 = ok												*/
/*	1 = invalid access code									*/
/*	2 = software has already been registered					*/
/*	3 = Registration has been disabled							*/
/*	4 = server is busy										*/
/*														*/
/**********************************************************************/

int CSSRegister::RegisterSoftware ( const char* szProgramName, const char* szAccessCode, CString& strUserCode, bool bUserReg, bool bCheckDisable )
{
	strUserCode = "";
	CString strSerialNo;
	int nReply;

	CSSAccessCode code;

	if ( code.DecodeAccessCode ( szAccessCode, TRUE ) == TRUE )
	{
		CString strMsg;

		if ( code.GetProgramText ( strMsg ) == TRUE )
		{
			if ( code.GetEnableText ( strMsg ) == TRUE )
			{
				strSerialNo = code.GetSerialNo();

				CSSIntDatabase db;
				if ( db.OpenReadWrite ( m_strRegFilename, "SerialNo,ascending", 10 ) == DB_ERR_NONE )
				{
					nReply = Register ( db, strSerialNo, bUserReg, bCheckDisable );	// limit number of registrations (user reg)
					db.Close();
				}
				else				// cannot open database
					nReply = 4;
			}
			else
				nReply = 1;
		}
		else
			nReply = 1;
	}
	else
		nReply = 1;

	switch ( nReply )
	{
	case 0:	strUserCode = code.GetUserCode();					break;
	case 1:	strUserCode = "Invalid Access Code";				break;
	case 2:	strUserCode = "Software has already been registered";	break;
	case 3:	strUserCode = "Invalid Access Code";				break;
	case 4:	strUserCode = "Server is busy";					break;
	}

	CString strProgramName;
	strProgramName.Format ( "Register (%s)", szProgramName );

	LogRegistration ( strSerialNo, szAccessCode, strUserCode, strProgramName );

	return nReply;
}

/**********************************************************************/
