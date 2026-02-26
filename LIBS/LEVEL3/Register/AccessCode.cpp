/**********************************************************************/

#define SYSSET_ECRMAN		1
#define SYSSET_STOCKMAN		2
#define SYSSET_SMARTMAN		3
#define SYSSET_SMART001		4
#define SYSSET_ECRMANLITE	5
#define SYSSET_CARDLINK		6
#define SYSSET_ECRMAN5000	7
#define SYSSET_STKMAN5000	8
#define SYSSET_HEADOFFICE	9
#define SYSSET_ECRMAN3		10
#define SYSSET_STKMAN3		11
#define SYSSET_GCRLINK		12
#define SYSSET_SYSENABLE		13
#define SYSSET_ECRMAN4		14
#define SYSSET_STKMAN4		15
#define SYSSET_SMARTMAN4		16
#define SYSSET_ECRMANX300	17
#define SYSSET_STKMANX300	18
#define SYSSET_SMARTSERVER	19
#define SYSSET_ECRMANX500	20
#define SYSSET_STKMANX500	21
#define SYSSET_CARDINIT		22
#define SYSSET_LOYALTY		23
#define SYSSET_SALESMAN		24
#define SYSSET_BARMAN		25
#define SYSSET_ECRMAN5		26
#define SYSSET_STKMAN5		27
#define SYSSET_SMARTMAN5		28
//#define SYSSET_ECRMANX300V2	29
//#define SYSSET_STKMANX300V2	30
#define SYSSET_ECRMANX500V2	31
#define SYSSET_STKMANX500V2	32
#define SYSSET_SMARTPAY		33
#define SYSSET_KIOSKBIOLINK	34
#define SYSSET_KIOSKCARDLINK	35
#define SYSSET_ECRMAN6		36
#define SYSSET_STKMAN6		37
#define SYSSET_SMARTMAN6		38
#define SYSSET_ECRMANX500V3	39
#define SYSSET_STKMANX500V3	40
#define SYSSET_LOYALTY2		41
#define SYSSET_ECRMANSPOSV4	42
#define SYSSET_STKMANSPOSV4	43
#define SYSSET_SMARTPAY2	44
#define SYSSET_LOYALTY3		45
#define SYSSET_SMARTPAY4	46
#define SYSSET_LOYALTY4		47

#define ENABLE_NETWORK_COMMS		1
#define ENABLE_PROGRAM			2
#define ENABLE_DEALER_EXPIRY		3
#define ENABLE_PROGRAM_MD5		4
#define ENABLE_DEALER_EXPIRY_MD5	5

#define DEALER_KEY "37432659"	// dealer key (used before md5)

/**********************************************************************/
/*
		12 digit registration number eg ( 5211 4103 1604 )
		----------------------------
	msd	0*	random digit
		1*	random digit
		2*	random digit
		3	enable type lsd

		4*	random digit
		5	prog type lsd
		6	enable type msd
		7*	random digit

		8*	random digit
		9*	random digit
		10	prog type msd
	lsd	11*	check digit


		16 digit registration number
		----------------------------

			0	1	2	3	4	5	6	7	8	9	(first digit)

	msd	0*	r1	r1	r1	r1	r1	r1	r1	r1	r1	r1
		1*	5	3	2	6	5	4	1	2	3	4
		2*	2	1	4	2	6	3	5	5	4	1
		3	pl	eh	el	ph	pl	eh	el	ph	pl	eh

		4*	r2	r2	r2	r2	r2	r2	r2	r2	r2	r2
		5	eh	el	ph	pl	eh	el	ph	pl	eh	el
		6	4	5	3	4	1	2	6	3	6	5
		7*	c1	c1	c1	c1	c1	c1	c1	c1	c1	c1

		8*	r3	r3	r3	r3	r3	r3	r3	r3	r3	r3
		9*	3	4	6	1	2	5	3	4	1	6
		10	el	ph	pl	eh	el	ph	pl	eh	el	ph
		11*	1	6	5	3	4	1	2	6	5	2

		12	c2	c2	c2	c2	c2	c2	c2	c2	c2	c2
		13	6	2	1	5	3	6	4	1	2	3
		14	ph	pl	eh	el	ph	pl	eh	el	ph	pl
	lsd	15	c3	c3	c3	c3	c3	c3	c3	c3	c3	c3

		1 = serial no msd
		6 = serial no lsd
*/

/**********************************************************************/

CSSAccessCode::CSSAccessCode()
{
	m_strSerialNo = "";
	m_strAccessCode = "";
	m_nProgramNo = 0;
	m_nEnableNo = 0;
}

/**********************************************************************/

int CSSAccessCode::xlat[10][10] =
{
	11,2,9,6,1,13,14,3,5,10,		// reg no starting with 0
	2,13,1,9,6,11,10,14,3,5,		// reg no starting with 1
	13,1,6,2,11,9,5,10,14,3,		// reg no starting with 2
	9,2,11,6,13,1,3,5,10,14,		// reg no starting with 3
	6,9,13,11,1,2,14,3,5,10,		// reg no starting with 4
	11,6,2,1,9,13,10,14,3,5,		// reg no starting with 5
	1,11,9,13,2,6,5,10,14,3,		// reg no starting with 6
	13,1,6,9,2,11,3,5,10,14,		// reg no starting with 7
	9,13,1,2,11,6,14,3,5,10,		// reg no starting with 8
	2,11,13,1,6,9,10,14,3,5,		// reg no starting with 9
};

/**********************************************************************/

bool CSSAccessCode::DecodeAccessCode ( const char* szAccessCode, bool bWebAccess )
{
	m_strSerialNo = "";
	m_strAccessCode = "";
	m_nProgramNo = 0;
	m_nEnableNo = 0;

	CString strAccessCode;

	for ( int i = 0 ; i < (int) strlen ( szAccessCode ) ; i++ )
	{
		char c = szAccessCode[i];

		if (( c >= '0' && c <= '9' ) || c == '*' )	// 0 - 9 and * ok
			strAccessCode += c;

		else if ( c == ' ' )		// remove spaces
			continue;

		else						// invalid char
			break;
	}

	if ( bWebAccess == TRUE && strAccessCode.GetLength() != 16 )	// only 16 digit numbers allowed if web access
		return FALSE;

	if ( strAccessCode.GetLength() == 16 )					// 16 digit code
	{
		m_strAccessCode = strAccessCode;

		if ( Checksum(strAccessCode,7) != strAccessCode[7] )	// check digit 1
			return FALSE;

		if ( Checksum(strAccessCode,12) != strAccessCode[12] )	// check digit 2
			return FALSE;

		if ( Checksum(strAccessCode,15) != strAccessCode[15] )	// check digit 3
			return FALSE;

		CString strTemp = "";

		int r1 = strAccessCode[0] - '0';		// random 1
		int r2 = strAccessCode[4] - '0';		// random 2
		int r3 = strAccessCode[8] - '0';		// random 3

		for ( int i = 0 ; i < 10 ; i++ )
		{
			strTemp += strAccessCode[xlat[r1][i]];	// reorder using random 1

			if (( i & 1 ) == 0 )							// even position
				strTemp.SetAt ( i, SubMod10 ( strTemp[i], r2 ) );	// sub random 2
			else											// odd position
				strTemp.SetAt ( i, SubMod10 ( strTemp[i], r3 ) );	// sub random 3

			strTemp.SetAt ( i, SubMod10 ( strTemp[i], i ) );		// sub position
		}

		m_strSerialNo = strTemp.Left(6);
		m_nProgramNo = atoi ( strTemp.Mid (6,2) );
		m_nEnableNo = atoi ( strTemp.Mid (8,2) );

		return TRUE;
	}
	else	if ( strAccessCode.GetLength() == 12 )		// 12 digit code
	{
		m_strAccessCode = strAccessCode;

		if ( Checksum(strAccessCode,11) != strAccessCode[11] )	// overall check digit
			return FALSE;

		CString strProgram;
		strProgram = strAccessCode[10];			// ms digit
		strProgram += strAccessCode[5];			// ls digit
		m_nProgramNo = atoi ( strProgram );

		CString strEnableNo;
		strEnableNo = strAccessCode[6];			// ms digit
		strEnableNo += strAccessCode[3];			// ls digit
		m_nEnableNo = atoi ( strEnableNo );

		return TRUE;
	}
	else if ( strAccessCode.GetLength() == 14 && strAccessCode.Left(2) == "**" && strAccessCode.Right(2) == "**" )
	{
		// **SSSSSSPPEE** S=SerialNo P=ProgramNo, E=EnableNo (for testing only)

		CString str1 = GenerateAccessCode ( atoi(strAccessCode.Mid(2,6)), atoi(strAccessCode.Mid(8,2)), atoi(strAccessCode.Mid(10,2)) );
		CString str2;
		str2.Format ( "%s %s %s %s", (const char*) str1.Mid(0,4), (const char*) str1.Mid(4,4), (const char*) str1.Mid(8,4), (const char*) str1.Mid(12,4) );
		AfxMessageBox ( str2 );

		return FALSE;
	}

	return FALSE;		// wrong number of digits
}

/**********************************************************************/

CString CSSAccessCode::GenerateAccessCode ( int nSerialNo, int nProgram, int nEnableNo )
{
	CString strAccessCode;

	if ( nProgram >= 0 )		// TestRegistration must have been called first
	{
		strAccessCode = "0000000000000000";		// 16 digits

		srand ( ::GetTickCount() );

		strAccessCode.SetAt (  0, (char) ( rand() % 10 ) + '0' );	// random 1
		strAccessCode.SetAt (  4, (char) ( rand() % 10 ) + '0' );	// random 2
		strAccessCode.SetAt (  8, (char) ( rand() % 10 ) + '0' );	// random 3

		CString strCode;
		strCode.Format ( "%6.6d%2.2d%2.2d", nSerialNo, nProgram, nEnableNo );

		int r1 = strAccessCode[0] - '0';		// random 1
		int r2 = strAccessCode[4] - '0';		// random 2
		int r3 = strAccessCode[8] - '0';		// random 3

		for ( int i = 0 ; i < 10 ; i++ )
		{
			strCode.SetAt ( i, AddMod10 ( strCode[i], i ) );		// add position

			if (( i & 1 ) == 0 )							// even position
				strCode.SetAt ( i, AddMod10 ( strCode[i], r2 ) );	// add random 2
			else											// odd position
				strCode.SetAt ( i, AddMod10 ( strCode[i], r3 ) );	// add random 3

			strAccessCode.SetAt ( xlat[r1][i], strCode[i] );	// reorder using random 1
		}

		strAccessCode.SetAt (  7, Checksum(strAccessCode, 7) );	// check digit 1
		strAccessCode.SetAt ( 12, Checksum(strAccessCode,12) );	// check digit 2
		strAccessCode.SetAt ( 15, Checksum(strAccessCode,15) );	// check digit 3
	}

	return strAccessCode;
}

/**********************************************************************/

static CString GetMD5 ( CString strAccessCode )
{
	CString strMD5 = CSSMD5::CalcMD5(strAccessCode).MakeUpper();
	CString strBuf;

	for ( int i = 0 ; i < 8 ; i++ )
	{
		char c = strMD5.GetAt(i);
		int j = ( c >= 'A' && c <= 'F' ) ? c-0x37 : c-0x30;
		c = char ( j % 10 ) + '0';
		strBuf += c;
	}

	return strBuf;
}

/**********************************************************************/

CString CSSAccessCode::GetUserCode ( int n )
{
	CString strUserCode;
	CString strTemp;

	switch ( m_nEnableNo )
	{
	case ENABLE_NETWORK_COMMS:
	case ENABLE_PROGRAM:
		strUserCode +=  (char) (('9' - m_strAccessCode[8]) + '0');
		strUserCode += m_strAccessCode[2];
		strUserCode += (char) (('9' - m_strAccessCode[4]) + '0');
		strUserCode += (char) (('9' - m_strAccessCode[1]) + '0');
		strUserCode += " ";
		strUserCode += m_strAccessCode[0];
		strUserCode += (char) (('9' - m_strAccessCode[7]) + '0');
		strUserCode += (char) (('9' - m_strAccessCode[11]) + '0');
		strUserCode += m_strAccessCode[9];
		break;

	case ENABLE_DEALER_EXPIRY:
		strUserCode += GetDigit ( 9, 0 );
		strUserCode += GetDigit ( 7, 1 );
		strUserCode += GetDigit ( 11, 2 );
		strUserCode += GetDigit ( 4, 3 );
		strUserCode += " ";
		strUserCode += (char) ((char) n + '0' );
		strUserCode += GetDigit ( 2, 5 );
		strUserCode += GetDigit ( 12, 6 );
		strUserCode += GetDigit ( 8, 7 );
		break;

	case ENABLE_PROGRAM_MD5:
		strTemp = GetMD5 ( m_strAccessCode );
		strUserCode += strTemp[0];
		strUserCode += strTemp[1];
		strUserCode += strTemp[2];
		strUserCode += strTemp[3];
		strUserCode += ' ';
		strUserCode += strTemp[4];
		strUserCode += strTemp[5];
		strUserCode += strTemp[6];
		strUserCode += strTemp[7];
		break;

	case ENABLE_DEALER_EXPIRY_MD5:
		strTemp = GetMD5 ( m_strAccessCode );
		strUserCode += strTemp[0];
		strUserCode += strTemp[1];
		strUserCode += strTemp[2];
		strUserCode += strTemp[3];
		strUserCode += ' ';
		strUserCode += (char) ((char) n + '0' );
		strUserCode += strTemp[5];
		strUserCode += strTemp[6];
		strUserCode += strTemp[7];
		break;

	default:
		break;
	}

	return strUserCode;
}

/**********************************************************************/

bool CSSAccessCode::GetProgramText ( CString& strText )
{
	bool bReply = TRUE;

	switch ( m_nProgramNo )
	{
	case SYSSET_ECRMAN:			strText = "ECR Manager";				break;
	case SYSSET_STOCKMAN:		strText = "Stock Manager";				break;
	case SYSSET_SMARTMAN:		strText = "Smartcard Manager";			break;
	case SYSSET_SMART001:		strText = "Smart001";					break;
	case SYSSET_ECRMANLITE:		strText = "ECR Manager Lite";			break;
	case SYSSET_CARDLINK:		strText = "CardLink";					break;
	case SYSSET_ECRMAN5000:		strText = "ECR Manager 5000";			break;
	case SYSSET_STKMAN5000:		strText = "Stock Manager 5000";			break;
	case SYSSET_ECRMAN3:		strText = "ECR Manager 3";				break;
	case SYSSET_STKMAN3:		strText = "Stock Manager 3";			break;
	case SYSSET_GCRLINK:		strText = "GCR Link";					break;
	case SYSSET_SYSENABLE:		strText = "SysEnable - FOR DEALERS ONLY!";	break;
	case SYSSET_ECRMAN4:		strText = "ECR Manager 4";				break;
	case SYSSET_STKMAN4:		strText = "Stock Manager 4";			break;
	case SYSSET_SMARTMAN4:		strText = "Smartcard Manager 4";		break;
	case SYSSET_ECRMANX300:		strText = "ECR Manager X300";			break;
	case SYSSET_STKMANX300:		strText = "Stock Manager X300";			break;
	case SYSSET_SMARTSERVER:	strText = "Smartcard Server";			break;
	case SYSSET_ECRMANX500:		strText = "ECR Manager X500";			break;
	case SYSSET_STKMANX500:		strText = "Stock Manager X500";			break;
	case SYSSET_CARDINIT:		strText = "CardInit";					break;
	case SYSSET_LOYALTY:		strText = "Loyalty Manager X500";			break;
	case SYSSET_SALESMAN:		strText = "Sales Manager";				break;
	case SYSSET_BARMAN:			strText = "SPos Barcode Manager";		break;
	case SYSSET_ECRMAN5:		strText = "ECR Manager 5";				break;
	case SYSSET_STKMAN5:		strText = "Stock Manager 5";			break;
	case SYSSET_SMARTMAN5:		strText = "Smartcard Manager 5";		break;
//	case SYSSET_ECRMANX300V2:	strText = "ECR Manager X300 V2";		break;
//	case SYSSET_STKMANX300V2:	strText = "Stock Manager X300 V2";		break;
	case SYSSET_ECRMANX500V2:	strText = "ECR Manager X500 V2";		break;
	case SYSSET_STKMANX500V2:	strText = "Stock Manager X500 V2";		break;
	case SYSSET_SMARTPAY:		strText = "SPos SmartPay Manager";		break;
	case SYSSET_KIOSKBIOLINK:	strText = "Kiosk BioLink";				break;
	case SYSSET_KIOSKCARDLINK:	strText = "Kiosk CardLink";				break;
	case SYSSET_ECRMAN6:		strText = "ECR Manager 6";				break;
	case SYSSET_STKMAN6:		strText = "Stock Manager 6";			break;
	case SYSSET_SMARTMAN6:		strText = "Smartcard Manager 6";		break;
	case SYSSET_ECRMANX500V3:	strText = "ECR Manager X500 V3";		break;
	case SYSSET_STKMANX500V3:	strText = "Stock Manager X500 V3";		break;
	case SYSSET_LOYALTY2:		strText = "SPos Loyalty Manager";		break;
	case SYSSET_ECRMANSPOSV4:	strText = "SPos ECR Manager";			break;
	case SYSSET_STKMANSPOSV4:	strText = "SPos Stock Manager";			break;
	case SYSSET_SMARTPAY2:		strText = "SPos SmartPay2 Manager";		break;
	case SYSSET_LOYALTY3:		strText = "SPos Loyalty3 Manager";		break;
	case SYSSET_SMARTPAY4:		strText = "SPos SmartPay4 Manager";		break;
	case SYSSET_LOYALTY4:		strText = "SPos Loyalty4 Manager";		break;
	case 99:					strText = "Paul's Test Program";		break;

	default:
		strText.Format ( "Unknown Program %d", m_nProgramNo );
		bReply = FALSE;
		break;
	}

	return bReply;
}

/**********************************************************************/

bool CSSAccessCode::GetEnableText ( CString& strText )
{
	bool bReply = TRUE;

	switch ( m_nEnableNo )
	{
	case ENABLE_NETWORK_COMMS:		strText = "Enable Network Comms";	break;
	case ENABLE_PROGRAM:			strText = "Enable Program";		break;
	case ENABLE_DEALER_EXPIRY:		strText = "Dealer Expiry";		break;
	case ENABLE_PROGRAM_MD5:			strText = "Enable Program (md5)";	break;
	case ENABLE_DEALER_EXPIRY_MD5:	strText = "Dealer Expiry (md5)";	break;
	default:
		strText.Format ( "Unknown Enable Type %d", m_nEnableNo );
		bReply = FALSE;
		break;
	}

	return bReply;
}

/**********************************************************************/

bool CSSAccessCode::IsDealerExpiry()
{
	return ( m_nEnableNo == ENABLE_DEALER_EXPIRY || m_nEnableNo == ENABLE_DEALER_EXPIRY_MD5 ) ? TRUE : FALSE;
}

/**********************************************************************/

char CSSAccessCode::Checksum ( const char* szBuf, int nLen )
{
	int nChecksum = 0;

	for ( int i = 0 ; i < nLen ; i++ )
		nChecksum += ( szBuf[i] - '0' );

	return (( nChecksum % 10 ) + '0' );
}

/**********************************************************************/

char CSSAccessCode::SubMod10 ( char cDigit, int nOffset )
{
	int n = (int) ( cDigit - '0' ) - nOffset;
	if ( n < 0 )
		n += 10;

	return ( n +'0' );
}

/**********************************************************************/

char CSSAccessCode::AddMod10 ( char cDigit, int nOffset )
{
	int n = (int) ( cDigit - '0' ) + nOffset;
	if ( n >= 10 )
		n -= 10;

	return ( n +'0' );
}

/**********************************************************************/

char CSSAccessCode::GetDigit ( int i, int j )
{
	const char* szKey = DEALER_KEY;

	int nRand = m_strAccessCode[i] - '0';
	int nKey = szKey[j] - '0';

	int n = (( nRand + nKey ) % 10 );

	return n + '0';
}

/**********************************************************************/
