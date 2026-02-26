/**********************************************************************/

CCommsConnect::CCommsConnect ( const char* szDialFilename, const char* szDllFilename )
{
	m_strDialFilename = szDialFilename;
	m_strDllFilename = szDllFilename;
	m_bPortOpen = FALSE;
	m_bRemote = FALSE;
	m_bNetworkComms = FALSE;
}

CCommsConnect::~CCommsConnect()
{
	DropLine();
}

/**********************************************************************/

bool CCommsConnect::ConnectNetwork ( int nSyssetNetworkComms )
{
	m_bNetworkComms = TRUE;
	m_bPortOpen = FALSE;

	switch ( nSyssetNetworkComms )
	{
	case 2:	m_strError = "Ok";				return TRUE;
	case 1:	m_strError = "Not Enabled";		return FALSE;
	default:	m_strError = "Bad Network Link";	return FALSE;
	}
}

/**********************************************************************/

bool CCommsConnect::Connect ( const char* szCommMode, const char* szPhoneParams, CWnd* pParent, bool bProtect )
{
	bool bReply = FALSE;

	if ( m_Port.Open ( szCommMode ) == TRUE )
	{
		m_bPortOpen = TRUE;

		COMMTIMEOUTS cto;
		if ( ::GetCommTimeouts ( m_Port.GetHandle(), &cto ) != 0 )
		{
			cto.ReadIntervalTimeout = 0;
			cto.ReadTotalTimeoutMultiplier = 0;
			cto.ReadTotalTimeoutConstant = 0;
			cto.WriteTotalTimeoutMultiplier = 0;
			cto.WriteTotalTimeoutConstant = 5000;		/* TxTimeout 5 seconds */

			::SetCommTimeouts ( m_Port.GetHandle(), &cto );
		}

		if ( strlen ( szPhoneParams ) == 0 )		/* local comms */
		{
			m_bRemote = FALSE;

			DCB dcb;
			if ( m_Port.GetCommState ( &dcb ) == TRUE )
			{
				dcb.fOutxCtsFlow = FALSE;
				dcb.fOutxDsrFlow = FALSE;
				m_Port.SetCommState ( &dcb );
			}

			if ( bProtect == TRUE )
			{
				bReply = TestProtect();
				if ( bReply == TRUE )
					m_strError = "Ok";
				else
					m_strError = "Bad Comms Link";
			}
			else
			{
				m_Port.RaiseDtr();
				m_Port.RaiseRts();
				WaitLoop(500);
				m_strError = "Ok";
				bReply = TRUE;
			}
		}
		else								/* remote comms */
		{
			m_bRemote = TRUE;

			DCB dcb;
			if ( m_Port.GetCommState ( &dcb ) == TRUE )
			{
				dcb.fOutxCtsFlow = TRUE;
				dcb.fOutxDsrFlow = TRUE;
				m_Port.SetCommState ( &dcb );
			}

			m_Port.RaiseDtr();
			m_Port.RaiseRts();
			WaitLoop(1000);		/* 1 second because of U.S Robotics */

			CCSV csv ( szPhoneParams );	// param 0=phone number, 1=message (NODISPLAY stops dialog being displayed)

			CModemDial Modem ( m_strDialFilename, m_strDllFilename );
			bReply = Modem.Dial ( &m_Port, csv.GetString(0), csv.GetString(1) );
			if ( bReply == TRUE )
				WaitLoop(2000);	/* wait 2 seconds after dcd goes high */

			m_strError = Modem.GetError();
		}

		if ( bReply == FALSE )	/* port has been opened but connect failed */
			DropLine();
	}
	else
		m_strError = "Cannot open comms port";

	return bReply;
}

/**********************************************************************/

void CCommsConnect::Disconnect()
{
	m_strError = "Ok";

	if ( m_bNetworkComms == FALSE )
	{
		if ( m_bPortOpen == TRUE )
		{
			m_Port.DropRts();
			m_Port.DropDtr();

			if ( m_bRemote == TRUE )
			{
				int i;
				for ( i = 0 ; i < 50 ; i++ )			/* max 5 seconds */
				{
					if ( m_Port.CommGetDCD() == FALSE )	/* check if dcd dropped */
						break;

					WaitLoop(100);						/* 0.1 seconds */
				}

				if ( i < 50 )
					m_strError.Format ( "Ok (%d)", i );
				else
					m_strError = "Warning: unable to terminate call";
			}

			m_Port.Close();
			WaitLoop(1000);
			m_bPortOpen = FALSE;
		}
	}
	else
		m_bNetworkComms = FALSE;
}

/**********************************************************************/

void CCommsConnect::DropLine()
{
	if ( m_bNetworkComms == FALSE )
	{
		if ( m_bPortOpen == TRUE )
		{
			m_Port.DropRts();
			m_Port.DropDtr();
			WaitLoop(1000);
			m_Port.Close();
			WaitLoop(500);
			m_bPortOpen = FALSE;
		}
	}
	else
		m_bNetworkComms = FALSE;
}

/**********************************************************************/

bool CCommsConnect::TestProtect()
{
	m_Port.RaiseDtr();
	m_Port.RaiseRts();
	WaitLoop(200);
	m_Port.DropRts();
	WaitLoop(100);
	m_Port.RaiseRts();
	WaitLoop(100,TRUE);		// need accurate timing - disable sleep

	if ( m_Port.CommGetCTS() == FALSE )
	{
		WaitLoop(200,TRUE);		// need accurate timing - disable sleep
		if ( m_Port.CommGetCTS() == TRUE )
			return TRUE;
	}

	m_Port.DropRts();
	m_Port.DropDtr();
	return FALSE;
}

/**********************************************************************/

