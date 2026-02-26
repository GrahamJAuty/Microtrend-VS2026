/**********************************************************************/
#include "SPOSKeyboardDefines.h"
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/
#include "SPOSKeyboardDefaultTextArray.h"
/**********************************************************************/

CSPOSKeyboardDefaultTextRecord::CSPOSKeyboardDefaultTextRecord()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::Reset()
{
	m_nValue = 0;
	m_nBackground = 4;
	m_nForeground = 1;
	m_nLegacyTextSize = 1;
	m_nFontSize = 8;
	m_nHAlign = 1;
	m_nVAlign = 1;
	m_bBold = TRUE;
	m_strImageFilename = "";
	m_nImageScale = 100;
	
	m_strSystemDefaultText = "";
	m_strUserDefaultText = "";
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::GetDefaultText(CString& strText)
{
	GetUserDefaultText(strText);

	if (strText == "")
	{
		GetSystemDefaultText(strText);
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::GetUserDefaultText( CString& strText )
{
	strText = m_strUserDefaultText;
	::TrimSpaces( strText, FALSE );
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::GetSystemDefaultText( CString& strText )
{
	strText = m_strSystemDefaultText;
	::TrimSpaces( strText, FALSE );
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetSystemDefaultText( const char* szText )
{
	m_strSystemDefaultText = szText;
	
	CSPOSKeyboardKeyHelpers KeyHelpers;
	KeyHelpers.ValidateKeyText( m_strSystemDefaultText );
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetUserDefaultText( const char* szText )
{
	m_strUserDefaultText = szText;

	CSPOSKeyboardKeyHelpers KeyHelpers;
	KeyHelpers.ValidateKeyText( m_strUserDefaultText );
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetForeground( int n )
{
	if (n >= 1 && n <= 64)
	{
		m_nForeground = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetBackground(int n)
{
	if ((n >= 1) && (n <= 64))
	{
		m_nBackground = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetFontSize(int n)
{
	if (CSPOSKeyboardKeyRecord::IsValidFontSize(n))
	{
		m_nFontSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetHAlign(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nHAlign = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetVAlign(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nVAlign = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetImageFilename( const char* sz )
{
	CString str = sz;
	int nLength = str.GetLength();

	if ( nLength >= 5 && nLength <= MAX_BITMAP_FILENAME + 4 )
	{
		CString strTest = str.Right(4);
		strTest.MakeUpper();

		if (strTest == ".BMP" || strTest == ".GIF" || strTest == ".JPG")
		{
			m_strImageFilename = str;
		}
	}
	else if (nLength == 0)
	{
		m_strImageFilename = "";
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextRecord::SetImageScale(int n)
{
	if ((n >= 0) && (n <= 100))
	{
		m_nImageScale = n;
	}
}

/**********************************************************************/

CSPOSKeyboardDefaultTextArray::CSPOSKeyboardDefaultTextArray()
{
	m_strFilename = "";
	m_nKeyType = 0;
	m_nMaxValue = 0;
	m_nMinValue = 0;
}

/**********************************************************************/

bool CSPOSKeyboardDefaultTextArray::GetTextRecord( int nTextIdx, CSPOSKeyboardDefaultTextRecord& record )
{
	bool bResult = FALSE;
	record.Reset();

	if ( nTextIdx >= 0 && nTextIdx < GetSize() )
	{
		record = m_arrayDefaultTexts.GetAt( nTextIdx );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardDefaultTextArray::SetTextRecord( int nTextIdx, CSPOSKeyboardDefaultTextRecord& record )
{
	bool bResult = FALSE;
	
	if ( nTextIdx >= 0 && nTextIdx < GetSize() )
	{
		m_arrayDefaultTexts.SetAt( nTextIdx, record );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::SetTextType( const char* szDataPath, int nType )
{
	RemoveAll();
	m_nKeyType = nType;
	m_strFilename.Format ( "%s\\ssoft%3.3d.dat", szDataPath, m_nKeyType );

	switch ( m_nKeyType )
	{
	case KEY_TYPE_LIST:		
		AddDefaultLists();	
		break;

	case KEY_TYPE_FUNCTION:	
		AddDefaultFunctions();
		break;

	case KEY_TYPE_PAYMENT:	
		AddDefaultPayments();
		break;

	case KEY_TYPE_CONTROL:	
		AddDefaultControls();
		break;

	case KEY_TYPE_MODIFIER:	
		AddDefaultModifiers();	
		break;

	case KEY_TYPE_SVRPRESET:
		AddDefaultSvrPresets();
		break;

	case KEY_TYPE_MACRO:	
		AddDefaultMacros();	
		break;

	case KEY_TYPE_EMPTY:	
		AddDefaultEmptys();	
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::RemoveAll()
{
	m_arrayDefaultTexts.RemoveAll();
}

/**********************************************************************/

bool CSPOSKeyboardDefaultTextArray::FindTextByValue( __int64 nValue, int& nIndex )
{
	int nStart = 0;
	int nEnd = m_arrayDefaultTexts.GetSize() - 1;

	while (nStart <= nEnd)
	{
		nIndex = (nStart + nEnd) / 2;

		__int64 nArrayValue = m_arrayDefaultTexts[nIndex].GetValue();

		if (nArrayValue == nValue)
		{
			return TRUE;
		}
		else if (nArrayValue < nValue)
		{
			nStart = nIndex + 1;
		}
		else
		{
			nEnd = nIndex - 1;
		}
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddText ( __int64 nValue, const char* szText )
{
	int nIndex = 0;
	if (FindTextByValue(nValue, nIndex) == TRUE)
	{
		return;
	}

	CSPOSKeyboardDefaultTextRecord TextRecord;

	TextRecord.SetValue ( nValue );
	TextRecord.SetSystemDefaultText ( szText );
	m_arrayDefaultTexts.InsertAt ( nIndex, TextRecord );
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultFunctions()
{
	//FIDDLE - SOME FUNCTIONS COMMENTED OUT INTENTIONALLY
	//BECAUSE VPM DOES NOT ALLOW THEM TO BE SELECTED

	AddText(1, "Server|Number");
	AddText(2, "Table|Number");
	AddText(3, "PLU");
	AddText(4, "Quantity");
	AddText(5, "Sub|Total");
	AddText(6, "Receipt");
	AddText(7, "Bill");
	AddText(8, "Table|Release");
	//AddText ( 9, "Clear" );
	//AddText ( 10,"Redisp );
	AddText(11, "Item|Void");
	AddText(12, "Transact|Void");
	AddText(13, "Error|Correct");
	AddText(14, "No|Sale");
	AddText(15, "Screen");
	//AddText ( 16,"Spare" );
	AddText(17, "Preset Server");
	//AddText ( 18,"Payment|1" );
	//AddText ( 19,"Payment|2" );
	//AddText ( 20,"Payment|3" );
	//AddText ( 21,"Payment|4" );
	//AddText ( 22,"Payment|5" );
	//AddText ( 23,"Payment|6" );
	//AddText ( 24,"Payment|7" );
	//AddText ( 25,"Payment|8" );
	//AddText ( 26,"Payment|9" );
	//AddText ( 27,"Payment|10" );
	//AddText ( 28,"Payment|11" );
	//AddText ( 29,"Payment|12" );
	//AddText ( 30,"Payment|13" );
	//AddText ( 31,"Payment|14" );
	//AddText ( 32,"Payment|15" );
	//AddText ( 33,"Payment|16" );
	AddText(34, "Auto|Receipt");
	//AddText ( 35,"List" );
	AddText(36, "VIP");
	//AddText ( 37,"W|Server" );
	AddText(38, "Split");
	AddText(39, "Refund");
	//AddText ( 40,"Mananger" );
	//AddText ( 41,"Spare" );
	//AddText ( 42,"System" );
	//AddText ( 43,"System" );
	//AddText ( 44,"System" );
	//AddText ( 45,"System" );
	//AddText ( 46,"System" );
	//AddText ( 47,"System" );
	AddText(48, "System");
	AddText(49, "Open|Table|List");
	AddText(50, "Open|Accounts");
	AddText(51, "Currency");
	AddText(52, "EAN");
	AddText(53, "Price|Enquiry");
	AddText(54, "Price|Override");
	AddText(55, "Item|Promotion");
	AddText(56, "Promotion|Override");
	AddText(57, "Sub|Total|Promotion");
	AddText(58, "Cover|Set");
	AddText(59, "Loyalty");
	//AddText ( 60,"MOD 1" );
	//AddText ( 61,"MOD 2" );
	//AddText ( 62,"MOD 3" );
	//AddText ( 63,"MOD 4" );
	//AddText ( 64,"MOD 5" );
	//AddText ( 65,"MOD 6" );
	//AddText ( 66,"MOD 7" );
	//AddText ( 67,"MOD 8" );
	//AddText ( 68,"MOD 9" );
	//AddText ( 69,"FUNC|69" );
	AddText(70, "KP|Hold");
	AddText(71, "KP|Go");
	AddText(72, "Bank");
	AddText(73, "Differ|Sub|Total");
	AddText(74, "Differ|Discount");
	AddText(75, "Sold|Out");
	AddText(76, "PLU|Shift");
	AddText(77, "X|Reports");
	AddText(78, "Loyalty|Check");
	AddText(79, "Force|Online");
	AddText(80, "EFT");
	AddText(81, "Transact|Reverse");
	AddText(82, "Active|Tables");
	AddText(83, "Stock|Display");
	AddText(84, "Extended|Function");
	AddText(85, "Authoris");
	AddText(86, "Shift");
	AddText(87, "Sell|Again");
	AddText(88, "Flip");
	AddText(89, "Floor|View");
	AddText(90, "Force|Server|Off");
	AddText(91, "Price|Band|Override");
	AddText(92, "P.Band|Override|OneShot");
	AddText(93, "View|Messages");
	AddText(94, "Compose|Messages");
	AddText(95, "External|Device|Export");
	AddText(96, "Trans|History");
	AddText(97, "Void|After");
	AddText(98, "Wastage");
	AddText(99, "86|Set|Stock");
	//AddText ( 100, "86|Author.|Oversell" );
	AddText(101, "Product|Info");
	AddText(102, "Tables|Finalise");
	AddText(104, "Sell|One More");
	AddText(105, "Sell|One Less");
	AddText(106, "Table|Grouping");
	AddText(107, "Receipt|Mode|Override");
	AddText(108, "Time|and|Attend");
	AddText(109, "Notice|Board");
	AddText(110, "Deposit|Info");
	AddText(111, "Table|Limit|Show");
	AddText(112, "Table|Limit|Change");
	//AddText ( 113, "Server|Switch" );
	//AddText ( 114, "Table|Switch" );
	AddText(115, "Table|Status");
	AddText(116, "Prepay|With|Deposit");
	AddText(117, "Execute|User|Program");
	AddText(118, "MM|Control|Global");
	AddText(119, "MM|Control|Single");
	AddText(120, "Biometric|Enrolment");
	AddText(121, "Biometric|Flat|File");
	AddText(122, "Booking|Manager");
	AddText(123, "Analysis|Shift");
	AddText(124, "Quantity|Down");
	AddText(125, "New|Balance|(NBAL)");
	AddText(126, "Order|By|Seat");
	AddText(127, "Item|Time");
	AddText(128, "Table|Merge");
	AddText(129, "Age|Verify");
	AddText(130, "Set|Price|Band");
	AddText(131, "Customers");
	AddText(132, "Received|On|Account");
	AddText(133, "Account|View");
	AddText(134, "Room");
	AddText(135, "Set Room");
	AddText(136, "View/Pay|Room|Account");
	AddText(137, "PLU Edit");
	AddText(138, "Paid|In/Out");
	AddText(139, "VAT|Shift");
	AddText(140, "Operate|Gate");
	AddText(141, "Set Global|Price Band");
	AddText(142, "Apply|Promo|Package");
	AddText(143, "Void|Surcharge");
	AddText(144, "Set|Authorization");
	AddText(145, "Wastage|Item|Void");
	AddText(146, "Preselect|Printer|Till");
	AddText(147, "Allergy|Override");
	AddText(148, "Expand|Items");
	AddText(149, "External|Booking|Access");
	AddText(150, "Move|Item");
	AddText(151, "Transfer|Room");
	AddText(152, "Scanner");
	AddText(153, "EFT|Login/Logout");
	AddText(154, "Bill|Progressive");
	AddText(155, "Noticeboard|Show");

	m_nMinValue = 1;
	m_nMaxValue = 155;
}

/**********************************************************************/

bool CSPOSKeyboardDefaultTextArray::IsValidValue( __int64 nValue )
{
	bool bResult = FALSE;

	switch( m_nKeyType )
	{
	case KEY_TYPE_FUNCTION:
		bResult |= ( nValue >= 1 && nValue <= 8 );
		bResult |= ( nValue >= 11 && nValue <= 15 );
		bResult |= ( nValue == 17 );
		bResult |= ( nValue == 34 );
		bResult |= ( nValue == 36 );
		bResult |= ( nValue >= 38 && nValue <= 39 );
		bResult |= ( nValue >= 48 && nValue <= 59 );
		bResult |= ( nValue >= 70 && nValue <= 99 );
		bResult |= ( nValue >= 101 && nValue <= 112 );
		bResult |= ( nValue >= 115 && nValue <= 155 );
		break;

	default:
		bResult |=  ( ( nValue >= m_nMinValue ) && ( nValue <= m_nMaxValue ) );
		break;
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultPayments()
{
	for (int nPay = 1; nPay <= 50; nPay++)
	{
		AddText(nPay, "");
	}

	for ( int nPay = 1; nPay <= 50; nPay++ )
	{
		CString strImport =	DataManager.Payment.GetDisplayName( nPay );
		
		CSPOSKeyboardDefaultTextRecord TextRecord;
		GetTextRecord( nPay - 1, TextRecord );
	
		if (strImport != "")
		{
			TextRecord.SetSystemDefaultText(strImport);
		}
		else
		{
			CString strPay = "";
			strPay.Format ( "Payment|%d", nPay );
			TextRecord.SetSystemDefaultText( strPay );
		}

		SetTextRecord( nPay - 1, TextRecord );
	}

	m_nMinValue = 1;
	m_nMaxValue = 50;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultControls()
{
	AddText ( 4, "Exit|List" );
	AddText ( 5, "Enter|List" );
	AddText ( 6, "List|Home" );
	AddText ( 7, "Goto|Page" );	
	m_nMinValue = 4;
	m_nMaxValue = 7;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultModifiers()
{
	for (int nMod = 0; nMod <= 9; nMod++)
	{
		AddText(nMod, "");
	}
	
	CSPOSKeyboardDefaultTextRecord TextRecord;
	GetTextRecord( 0, TextRecord );
	TextRecord.SetSystemDefaultText( "Full Product" );
	SetTextRecord( 0, TextRecord );
	
	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		CString strImport = DataManager.Modifier.GetDisplayName( nMod );
		
		CSPOSKeyboardDefaultTextRecord TextRecord;
		GetTextRecord( nMod, TextRecord );
	
		if (strImport != "")
		{
			TextRecord.SetSystemDefaultText(strImport);
		}
		else
		{
			CString strMod = "";
			strMod.Format ( "Mod|%d", nMod );
			TextRecord.SetSystemDefaultText( strMod );
		}

		SetTextRecord( nMod, TextRecord );
	}

	m_nMinValue = 0;
	m_nMaxValue = 9;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultSvrPresets()
{
	for ( int nPre = 1; nPre <= 50; nPre++ )
	{
		CString strPre;
		strPre.Format ( "Svr|Preset|%d", nPre );
		AddText ( nPre, strPre );
	}
	m_nMinValue = 1;
	m_nMaxValue = 50;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultMacros()
{
	m_nMinValue = 1;
	m_nMaxValue = 9999;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultEmptys()
{
	AddText ( 0, "" );
	m_nMinValue = 0;
	m_nMaxValue = 0;
}

/**********************************************************************/

void CSPOSKeyboardDefaultTextArray::AddDefaultLists()
{
	m_nMinValue = 1;
	m_nMaxValue = 32767;
}

/**********************************************************************/
		
bool CSPOSKeyboardDefaultTextArray::Read()
{
	if (m_strFilename == "")
	{
		return FALSE;
	}

	CSSFile fileIn;
	if (fileIn.Open(m_strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CString strBuffer;
	while ( fileIn.ReadString ( strBuffer ) )
	{
		CCSV csvIn( strBuffer );
		int nValue = csvIn.GetInt(0);

		if ( IsValidValue( nValue ) == TRUE )
		{
			int nTextIdx;

			switch ( m_nKeyType )
			{
			case KEY_TYPE_LIST:
				if ( FindTextByValue( nValue, nTextIdx ) == FALSE )
				{
					CString strListNo;
					strListNo.Format ( "List|%d", nValue );
					AddText( nValue, strListNo );
				}
				break;

			case KEY_TYPE_MACRO:
				if ( FindTextByValue( nValue, nTextIdx ) == FALSE )
				{
					CString strMacroNo;
					strMacroNo.Format ( "Macro|%d", nValue );
					AddText( nValue, strMacroNo );
				}
				break;
			}
	
			if ( FindTextByValue( nValue, nTextIdx ) == TRUE )
			{
				CSPOSKeyboardDefaultTextRecord TextRecord;
				GetTextRecord( nTextIdx, TextRecord );

				CString strText1 = csvIn.GetString(1);
				CString strText2 = csvIn.GetString(2);
				CString strText3 = csvIn.GetString(3);

				CString strText = strText1;
					
				if ( strText2 + strText3 != "" )
				{
					strText += "|";
					strText += strText2;
					strText += "|";
					strText += strText3;
				}
				
				TextRecord.SetUserDefaultText( strText );
			
				TextRecord.SetForeground ( csvIn.GetInt(4) );
				TextRecord.SetBackground ( csvIn.GetInt(5) );
				TextRecord.SetLegacyTextSize( csvIn.GetInt(6) );
				
				//LEGACY DATA WILL NOT HAVE THESE FIELDS SO USE DEFAULT VALUES
				if ( !csvIn.IsEmpty(7) ) TextRecord.SetFontSize ( csvIn.GetInt(7) );
				if ( !csvIn.IsEmpty(8) ) TextRecord.SetHAlign( csvIn.GetBool(8) );
				if ( !csvIn.IsEmpty(9) ) TextRecord.SetVAlign( csvIn.GetBool(9) );
				if ( !csvIn.IsEmpty(10) ) TextRecord.SetBoldFlag ( csvIn.GetBool(10) );
				if ( !csvIn.IsEmpty(11) ) TextRecord.SetImageFilename( csvIn.GetString(11) );
				if ( !csvIn.IsEmpty(12) ) TextRecord.SetImageScale( csvIn.GetInt(12) );
	
				SetTextRecord( nTextIdx, TextRecord );
			}
		}
	}
	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardDefaultTextArray::Write()
{
	if (m_strFilename == "")
	{
		return FALSE;
	}

	CSSFile fileOut;
	if (fileOut.Open(m_strFilename, "wb") == FALSE)
	{
		return FALSE;
	}

	for ( int nTextIdx = 0; nTextIdx < GetSize(); nTextIdx++ )
	{
		CSPOSKeyboardDefaultTextRecord TextRecord;
		GetTextRecord( nTextIdx, TextRecord );

		CString strText;
		TextRecord.GetUserDefaultText( strText );

		CCSV csvOut;
		csvOut.Add ( TextRecord.GetValue() );
		csvOut.Add ( strText );
		csvOut.Add ( "" );
		csvOut.Add ( "" );
		csvOut.Add ( TextRecord.GetForeground() );
		csvOut.Add ( TextRecord.GetBackground() );
		csvOut.Add ( TextRecord.GetLegacyTextSize() );
		csvOut.Add ( TextRecord.GetFontSize() );
		csvOut.Add ( TextRecord.GetHAlign() );
		csvOut.Add ( TextRecord.GetVAlign() );
		csvOut.Add ( TextRecord.GetBoldFlag() );
		csvOut.Add ( TextRecord.GetImageFilename() );
		csvOut.Add ( TextRecord.GetImageScale() );

		fileOut.WriteLine ( csvOut.GetLine() );
		
	}
	return TRUE;
}

/**********************************************************************/
