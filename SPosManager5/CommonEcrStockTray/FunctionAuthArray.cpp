/**********************************************************************/
#include "FunctionAuthArray.h"
/**********************************************************************/

CFunctionAuthInfo::CFunctionAuthInfo()
{
	Reset();
}

/**********************************************************************/

void CFunctionAuthInfo::Reset()
{
	m_nFuncNo = 0;
	m_strName = "";
	m_nAuthLevelSystem = 0;
	m_bPasswordSystem = FALSE;
	m_bAuditSystem = FALSE;
	m_bLogSystem = FALSE;
	m_nAuthLevelDatabase = 0;
	m_bPasswordDatabase = FALSE;
	m_bAuditDatabase = FALSE;
	m_bLogDatabase = FALSE;
	m_nAuthLevelLocation = 0;
	m_bPasswordLocation = FALSE;
	m_bAuditLocation = FALSE;
	m_bLogLocation = FALSE;
}

/**********************************************************************/

void CFunctionAuthInfo::SetFuncNo( int n )
{
	if ((n >= 1) && (n <= 9000))
	{
		m_nFuncNo = n;
	}
}

/**********************************************************************/

void CFunctionAuthInfo::SetName( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= 40)
	{
		m_strName = str;
	}
}

/**********************************************************************/

void CFunctionAuthInfo::SetAuthLevelSystem( int n )
{
	if ((n >= 0) && (n <= 9))
	{
		m_nAuthLevelSystem = n;
	}
}

/**********************************************************************/

void CFunctionAuthInfo::SetAuthLevelDatabase( int n )
{
	if ((n >= -1) && (n <= 9))
	{
		m_nAuthLevelDatabase = n;
	}
}

/**********************************************************************/

void CFunctionAuthInfo::SetAuthLevelLocation( int n )
{
	if ((n >= -1) && (n <= 9))
	{
		m_nAuthLevelLocation = n;
	}
}

/**********************************************************************/

void CFunctionAuthInfo::Add (CFunctionAuthInfo& source )
{
}

/**********************************************************************/

int CFunctionAuthInfo::Compare( CFunctionAuthInfo& source, int nHint )
{
	if (m_nFuncNo != source.m_nFuncNo)
	{
		return ((m_nFuncNo > source.m_nFuncNo) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CFunctionAuthTable::CFunctionAuthTable(void)
{
	m_nMacroFunctionCount = 0;

	AddFunction(1, "Server Number");
	AddFunction(2, "Table Number");
	AddFunction(3, "PLU");
	AddFunction(4, "Quantity");
	AddFunction(5, "Sub Total");
	AddFunction(6, "Receipt");
	AddFunction(7, "Bill");
	AddFunction(8, "Table Release");
	AddFunction(9, "Clear");
	AddFunction(10, "Re-Display");
	AddFunction(11, "Item Void");
	AddFunction(12, "Transaction Void");
	AddFunction(13, "Error Correct");
	AddFunction(14, "No Sale");
	AddFunction(15, "Screen");
	AddFunction(16, "SPARE");
	AddFunction(17, "Preset Server");
	AddFunction(18, "Payment authority 1");
	AddFunction(19, "Payment authority 2");
	AddFunction(20, "Payment authority 3");
	AddFunction(21, "Payment authority 4");
	AddFunction(22, "Payment authority 5");
	AddFunction(23, "Payment authority 6");
	AddFunction(24, "Payment authority 7");
	AddFunction(25, "Payment authority 8");
	AddFunction(26, "Payment authority 9");
	AddFunction(27, "Payment authority 10");
	AddFunction(28, "Payment authority 11");
	AddFunction(29, "Payment authority 12");
	AddFunction(30, "Payment authority 13");
	AddFunction(31, "Payment authority 14");
	AddFunction(32, "Payment authority 15");
	AddFunction(33, "Payment authority 16");
	AddFunction(34, "Auto Receipt");
	AddFunction(35, "List");
	AddFunction(36, "VIP");
	AddFunction(38, "Split");
	AddFunction(39, "Refund");
	AddFunction(40, "Manager");
	AddFunction(41, "Hotel Room Manual Entry");
	AddFunction(42, "List paging");
	AddFunction(43, "SYSTEM");
	AddFunction(44, "Loyalty Manual Entry");
	AddFunction(45, "Open Acc Table");
	AddFunction(46, "Time&Att Manager");
	AddFunction(47, "EFT Manager");
	AddFunction(48, "SYSTEM");
	AddFunction(49, "Open Table List");
	AddFunction(50, "Open Accounts");
	AddFunction(51, "CURRENCY");
	AddFunction(52, "EAN");
	AddFunction(53, "Price Enquiry");
	AddFunction(54, "Price Override");
	AddFunction(55, "Item Promotion");
	AddFunction(56, "Promotion Override");
	AddFunction(57, "Sub Total Promotion");
	AddFunction(58, "Cover Set");
	AddFunction(59, "Loyalty");
	AddFunction(60, "MOD1");
	AddFunction(61, "MOD2");
	AddFunction(62, "MOD3");
	AddFunction(63, "MOD4");
	AddFunction(64, "MOD5");
	AddFunction(65, "MOD6");
	AddFunction(66, "MOD7");
	AddFunction(67, "MOD8");
	AddFunction(68, "MOD9");
	AddFunction(70, "KP Hold");
	AddFunction(71, "KP GO");
	AddFunction(72, "Bank");
	AddFunction(73, "Differ Sub Total");
	AddFunction(74, "Differ Discount");
	AddFunction(75, "Sold Out");
	AddFunction(76, "PLU Shift");
	AddFunction(77, "X Reports");
	AddFunction(78, "Loyalty Check");
	AddFunction(79, "Force On Line");
	AddFunction(80, "EFT");
	AddFunction(81, "Transaction Reverse");
	AddFunction(82, "Active Tables");
	AddFunction(83, "Stock Display");
	AddFunction(84, "Extended Function");
	AddFunction(85, "Authoris");
	AddFunction(86, "Shift");
	AddFunction(87, "Sell Again");
	AddFunction(88, "Flip");
	AddFunction(89, "Floor View");
	AddFunction(90, "Force Server Off");
	AddFunction(91, "Price band override");
	AddFunction(92, "Price band override 1 shot");
	AddFunction(93, "View Messsages");
	AddFunction(94, "Compose Messsages");
	AddFunction(95, "External device export");
	AddFunction(96, "Transaction History");
	AddFunction(97, "Void after");
	AddFunction(98, "Wastage");
	AddFunction(99, "86 Set Stock");
	AddFunction(100, "86 Authorized Oversell");
	AddFunction(101, "Product Information");
	AddFunction(102, "Tables Finalize");
	AddFunction(103, "Table Transfer");
	AddFunction(104, "Sell one more");
	AddFunction(105, "Sell one less");
	AddFunction(106, "Table grouping");
	AddFunction(107, "Receipt mode override");
	AddFunction(108, "Time & Attendance");
	AddFunction(109, "Noticeboard");
	AddFunction(110, "Deposit Info");
	AddFunction(111, "Table Limit Show");
	AddFunction(112, "Table Limit Change");
	AddFunction(113, "Server switch");
	AddFunction(114, "Table switch");
	AddFunction(115, "Table Status");
	AddFunction(116, "Prepay with Deposit");
	AddFunction(117, "Execute user program");
	AddFunction(118, "MM Control Global");
	AddFunction(119, "MM Control Single");
	AddFunction(120, "Biometric Enrolment");
	AddFunction(121, "Biometric Flat file export");
	AddFunction(122, "Booking Manager");
	AddFunction(123, "Analysis Shift");
	AddFunction(124, "Quantity down");
	AddFunction(125, "New Balance (NBAL)");
	AddFunction(126, "Order by Seat");
	AddFunction(127, "Item time");
	AddFunction(128, "Table merge");
	AddFunction(129, "Age verify");
	AddFunction(130, "Set Price Band");
	AddFunction(131, "Customers");
	AddFunction(132, "Received on Account");
	AddFunction(133, "Account View");
	AddFunction(134, "Room");
	AddFunction(136, "View/Pay Room Account");
	AddFunction(137, "PLU Edit");
	AddFunction(138, "Paid in/out");
	AddFunction(139, "Tax Shift");
	AddFunction(140, "Operate Gate");
	AddFunction(141, "Set Global Price Band");
	AddFunction(142, "Apply promo package");
	AddFunction(143, "Void surcharge");
	AddFunction(144, "Set Authorization");
	AddFunction(145, "Wastage item void");
	AddFunction(146, "Preselect printer till");
	AddFunction(147, "Allergy override");
	AddFunction(148, "Expand items");
	AddFunction(149, "External Booking Access");
	AddFunction(150, "Move Item");
	AddFunction(151, "Transfer Room");
	AddFunction(152, "Scanner");
	AddFunction(153, "EFT Login/Logout");
	AddFunction(154, "Bill Progressive");
	AddFunction(155, "Noticeboard Show");
	m_nMacroFunctionCount = m_arrayFunctions.GetSize();
	/*****/
	AddFunction(200, "Start of Day");
	AddFunction(201, "End of Day");
	AddFunction(202, "Open Session");
	AddFunction(203, "Close Session");
	AddFunction(204, "Trading");
	AddFunction(205, "Paid In");
	AddFunction(206, "Paid Out");
	AddFunction(207, "Reports");
	AddFunction(208, "Training Off");
	AddFunction(209, "Training On");
	AddFunction(210, "Set Date Time");
	AddFunction(211, "Set Preset Server");
	AddFunction(212, "Utilities");
	AddFunction(213, "System Shutdown");
	AddFunction(214, "Secure");
	AddFunction(215, "Time & Attendance");
	AddFunction(300, "CCD Report");
	AddFunction(301, "Operator Report");
	AddFunction(302, "Daily Sales Report");
	AddFunction(303, "Period Sales Report");
	AddFunction(304, "Account Tables Report");
	AddFunction(305, "Open Tables Report");
	AddFunction(306, "Promotion Usage Report");
	AddFunction(307, "Promotions Period Report");
	AddFunction(308, "Covers Report");
	AddFunction(309, "Period Covers Report");
	AddFunction(310, "Current Sales Report");
	AddFunction(311, "Period Csah Report");
	AddFunction(312, "Reasons Report");
	AddFunction(313, "House Cash Report");
	AddFunction(400, "Delete Spool Files");
	AddFunction(401, "Set EFT System");
	AddFunction(402, "Set EFT Manual");
	AddFunction(403, "Set Price Band");
	AddFunction(404, "Redirect Dispense Printers");
	AddFunction(405, "Update Database from Master");
	AddFunction(406, "Transaction History");
	AddFunction(408, "Export fixed data");
	AddFunction(409, "Clear all sales");
	AddFunction(410, "Registration");
	AddFunction(411, "Transaction Restore");
	AddFunction(412, "Manage Clerk Keys");
	AddFunction(413, "Venta Net Test");
	AddFunction(414, "Clear Noticeboard");
	AddFunction(415, "Assign Drawers");
	AddFunction(416, "Customers");
	AddFunction(417, "Customers - New");
	AddFunction(418, "Customers - Edit");
	AddFunction(419, "Customers - View");
	AddFunction(420, "Customers - Set Credit");
	AddFunction(421, "Customers - Delete");
	AddFunction(424, "Bookings - New");
	AddFunction(425, "Bookings - Edit");
	AddFunction(426, "Bookings - View");
	AddFunction(427, "Empty Drawer");
	AddFunction(430, "Perform End of Period 2");
	AddFunction(431, "Open Drawers");
	AddFunction(432, "Exit to OS");
	AddFunction(433, "Cancel Booking");
	AddFunction(440, "PLU Edit Dept");
	AddFunction(441, "Diagnostics");
	AddFunction(442, "Grand Totals Reset");
	AddFunction(9000, "User Function 1");
}

/**********************************************************************/

void CFunctionAuthTable::AddFunction( int nFuncNo, const char* szName )
{
	CFunctionAuthInfo infoFunction;
	infoFunction.SetFuncNo( nFuncNo );
	infoFunction.SetName( szName );
	infoFunction.SetAuthLevelSystem( 1 );
	infoFunction.SetAuthLevelDatabase( -1 );
	infoFunction.SetAuthLevelLocation( -1 );
	m_arrayFunctions.DirectAdd( infoFunction );
}

/**********************************************************************/

void CFunctionAuthTable::FixTaxFunction()
{
	CFunctionAuthInfo infoFunction;
	infoFunction.SetFuncNo(139);

	int nFuncIdx;
	if ( m_arrayFunctions.Find( infoFunction, nFuncIdx ) == TRUE )
	{
		m_arrayFunctions.GetAt( nFuncIdx, infoFunction );
		infoFunction.SetName( EcrmanOptions.GetTaxString( "%T Shift" ) );
		m_arrayFunctions.SetAt( nFuncIdx, infoFunction );
	}
}

/**********************************************************************/

void CFunctionAuthTable::ResetFunctionList( int nFunctionMode )
{
	switch( nFunctionMode )
	{
	case FUNCTIONFILE_SYSTEM:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CFunctionAuthInfo infoFunction;
			m_arrayFunctions.GetAt( n, infoFunction );
			infoFunction.SetAuthLevelSystem( 1 );
			infoFunction.SetPasswordFlagSystem( FALSE );
			infoFunction.SetAuditFlagSystem( FALSE );
			infoFunction.SetLogFlagSystem( FALSE );
			m_arrayFunctions.SetAt( n, infoFunction );
		}
		break;

	case FUNCTIONFILE_DATABASE:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CFunctionAuthInfo infoFunction;
			m_arrayFunctions.GetAt( n, infoFunction );
			infoFunction.SetAuthLevelDatabase( -1 );
			infoFunction.SetPasswordFlagDatabase( FALSE );
			infoFunction.SetAuditFlagDatabase( FALSE );
			infoFunction.SetLogFlagDatabase( FALSE );
			m_arrayFunctions.SetAt( n, infoFunction );
		}
		break;

	case FUNCTIONFILE_LOCATION:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CFunctionAuthInfo infoFunction;
			m_arrayFunctions.GetAt( n, infoFunction );
			infoFunction.SetAuthLevelLocation( -1 );
			infoFunction.SetPasswordFlagLocation( FALSE );
			infoFunction.SetAuditFlagLocation( FALSE );
			infoFunction.SetLogFlagLocation( FALSE );
			m_arrayFunctions.SetAt( n, infoFunction );
		}
		break;
	}
}

/**********************************************************************/

bool CFunctionAuthTable::GetFunction( CFunctionAuthMiniInfo& infoMini )
{
	bool bResult = FALSE;

	CFunctionAuthInfo infoFunction;
	infoFunction.SetFuncNo( infoMini.m_nFuncNo );

	int nFuncIdx;
	if ( m_arrayFunctions.Find( infoFunction, nFuncIdx ) == TRUE )
		bResult = GetFunction( nFuncIdx, infoMini );

	return bResult;
}

/**********************************************************************/

bool CFunctionAuthTable::GetFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini )
{
	bool bResult = FALSE;

	if ( ( nFuncIdx >= 0 ) && ( nFuncIdx < m_arrayFunctions.GetSize() ) )
	{
		CFunctionAuthInfo infoFunction;
		m_arrayFunctions.GetAt( nFuncIdx, infoFunction );

		infoMini.m_nFuncNo = infoFunction.GetFuncNo();
		infoMini.m_strName = infoFunction.GetName();
			
		switch( infoMini.m_nType )
		{
		case FUNCTIONFILE_SYSTEM:
			infoMini.m_nAuthLevel = infoFunction.GetAuthLevelSystem();
			infoMini.m_bPassword = infoFunction.GetPasswordFlagSystem();
			infoMini.m_bAudit = infoFunction.GetAuditFlagSystem();
			infoMini.m_bLog = infoFunction.GetLogFlagSystem();
			bResult = TRUE;
			break;

		case FUNCTIONFILE_DATABASE:
			infoMini.m_nAuthLevel = infoFunction.GetAuthLevelDatabase();
			infoMini.m_bPassword = infoFunction.GetPasswordFlagDatabase();
			infoMini.m_bAudit = infoFunction.GetAuditFlagDatabase();
			infoMini.m_bLog = infoFunction.GetLogFlagDatabase();
			bResult = TRUE;
			break;

		case FUNCTIONFILE_LOCATION:
			infoMini.m_nAuthLevel = infoFunction.GetAuthLevelLocation();
			infoMini.m_bPassword = infoFunction.GetPasswordFlagLocation();
			infoMini.m_bAudit = infoFunction.GetAuditFlagLocation();
			infoMini.m_bLog = infoFunction.GetLogFlagLocation();
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

/**********************************************************************/

void CFunctionAuthTable::UpdateFunction( CFunctionAuthMiniInfo& infoMini )
{
	CFunctionAuthInfo infoFunction;
	infoFunction.SetFuncNo( infoMini.m_nFuncNo );

	int nFuncIdx;
	if ( m_arrayFunctions.Find( infoFunction, nFuncIdx ) == TRUE )
		UpdateFunction( nFuncIdx, infoMini );
}

/**********************************************************************/

void CFunctionAuthTable::UpdateFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini )
{
	if ( ( nFuncIdx >= 0 ) && ( nFuncIdx < m_arrayFunctions.GetSize() ) )
	{
		CFunctionAuthInfo infoFunction;
		m_arrayFunctions.GetAt( nFuncIdx, infoFunction );

		if ( infoFunction.GetFuncNo() == infoMini.m_nFuncNo )
		{
			switch( infoMini.m_nType )
			{
			case FUNCTIONFILE_SYSTEM:
				infoFunction.SetAuthLevelSystem( infoMini.m_nAuthLevel );
				infoFunction.SetPasswordFlagSystem( infoMini.m_bPassword );
				infoFunction.SetAuditFlagSystem( infoMini.m_bAudit );
				infoFunction.SetLogFlagSystem( infoMini.m_bLog );
				break;

			case FUNCTIONFILE_DATABASE:
				infoFunction.SetAuthLevelDatabase( infoMini.m_nAuthLevel );
				infoFunction.SetPasswordFlagDatabase( infoMini.m_bPassword );
				infoFunction.SetAuditFlagDatabase( infoMini.m_bAudit );
				infoFunction.SetLogFlagDatabase( infoMini.m_bLog );
				break;

			case FUNCTIONFILE_LOCATION:
				infoFunction.SetAuthLevelLocation( infoMini.m_nAuthLevel );
				infoFunction.SetPasswordFlagLocation( infoMini.m_bPassword );
				infoFunction.SetAuditFlagLocation( infoMini.m_bAudit );
				infoFunction.SetLogFlagLocation( infoMini.m_bLog );
				break;
			}

			m_arrayFunctions.SetAt( nFuncIdx, infoFunction );
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CFunctionAuthArray::CFunctionAuthArray( int nFunctionMode, CFunctionAuthTable& FunctionTable ) : CSharedStringArray(), m_FunctionTable( FunctionTable )
{
	m_nFunctionMode = nFunctionMode;
}

/**********************************************************************/

int CFunctionAuthArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	m_FunctionTable.ResetFunctionList( m_nFunctionMode );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for( int n = 0; n < GetSize(); n++ )
		{
			CCSV csv( GetAt(n) );

			if ( csv.GetInt(0) == 1 )
			{
				CFunctionAuthMiniInfo infoMini;
				infoMini.m_nFuncNo = csv.GetInt(1);
				infoMini.m_nType = m_nFunctionMode;
				infoMini.m_nAuthLevel = csv.GetInt(2);
				infoMini.m_bPassword = csv.GetBool(3);
				infoMini.m_bAudit = csv.GetBool(4);
				infoMini.m_bLog = csv.GetBool(5);
				m_FunctionTable.UpdateFunction( infoMini );
			}
		}
	}

	RemoveAt( 0, GetSize() );
	return nReply;
}

/**********************************************************************/

void CFunctionAuthArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int nIdx = 0; nIdx < m_FunctionTable.GetSize(); nIdx++ )
	{
		CFunctionAuthMiniInfo infoMini;
		infoMini.m_nType = m_nFunctionMode;
		m_FunctionTable.GetFunction( nIdx, infoMini );

		CCSV csv;
		csv.Add(1);
		csv.Add(infoMini.m_nFuncNo );
		csv.Add(infoMini.m_nAuthLevel );
		csv.Add(infoMini.m_bPassword );
		csv.Add(infoMini.m_bAudit );
		csv.Add(infoMini.m_bLog );
		Add( csv.GetLine() );
	}
}

/**********************************************************************/

void CFunctionAuthArray::UpdateFunction( CFunctionAuthMiniInfo& infoMini )
{
	infoMini.m_nType = m_nFunctionMode;
	m_FunctionTable.UpdateFunction( infoMini );
}

/**********************************************************************/

void CFunctionAuthArray::UpdateFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini )
{
	infoMini.m_nType = m_nFunctionMode;
	m_FunctionTable.UpdateFunction( nFuncIdx, infoMini );
}

/**********************************************************************/

void CFunctionAuthArray::GetFunction( CFunctionAuthMiniInfo& infoMini )
{
	infoMini.m_nType = m_nFunctionMode;
	m_FunctionTable.GetFunction( infoMini );
}

/**********************************************************************/

void CFunctionAuthArray::GetFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini )
{
	infoMini.m_nType = m_nFunctionMode;
	m_FunctionTable.GetFunction( nFuncIdx, infoMini );
}

/**********************************************************************/

void CFunctionAuthArray::ResetFunctionList()
{
	m_FunctionTable.ResetFunctionList( m_nFunctionMode );
}

/**********************************************************************/
