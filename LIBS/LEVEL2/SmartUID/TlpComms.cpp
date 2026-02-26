/**********************************************************************/

#include "local.h"

/**********************************************************************/

#define	cACK		0x60
#define	cNAK		0xE0
#define	cEOT		0x03

#define ERR_NONE			0				// no error
#define ERR_NOCARD			(-1)				// card not inserted
#define ERR_CARD			(-2)				// smartcard error
#define ERR_TIMEOUT			(-3)				// timeout
#define ERR_RETRY			(-4)				// too many retries
#define ERR_OPEN_COMMS		(-5)				// cannot open comms port
#define ERR_USER_TERMINATE	(-6)				// comms terminated by user
#define ERR_UNKNOWN_CARD		(-7)				// unknown card type
#define ERR_UNKNOWN_READER	(-8)				// unknown reader type
#define ERR_DLL_LOAD		(-9)				// cannot load dll
#define ERR_COMM_ENABLE		(-10)			// cannot enable comms
#define ERR_COMM_TIMER		(-11)			// cannot enable timer
#define ERR_FUNCTION		(-12)			// function not supported

/**********************************************************************/

static const int MAX_RETRIES = 5;
static const int MAX_NAKS = 5;

/**********************************************************************/

CTLPComms::CTLPComms() : CComms(), m_TxBuf{0}
{
	m_nState = 0;
	m_nRetryCount = 0;
	m_nNakCount = 0;
	m_nRxCount = 0;
	m_bExit = FALSE;
	m_nErrno = 0;
	m_nRxTimeout = 0;
}

/**********************************************************************/

int CTLPComms::TxCommand ( CByteArray& ar, int nRxTimeout )
{
	m_nRxTimeout = nRxTimeout;
	m_nState = 0;
	m_nRetryCount = 0;
	m_nNakCount = 0;
	m_nRxCount = 0;

	int nLen = ar.GetSize();
	ar.InsertAt ( 0, cACK );		// insert ack
	ar.InsertAt ( 1, nLen );		// insert length
							// data
	ar.Add ( CalcCRC(ar) );		// append crc

	int n = 0;
	for ( int i = 0 ; i < ar.GetSize() ; i++ )	// copy to tx buffer
	{
		m_TxBuf[n++] = GetMSN ( ar.GetAt(i) );
		m_TxBuf[n++] = GetLSN ( ar.GetAt(i) );
	}

	m_TxBuf[n++] = cEOT;		// eot
	m_TxBuf[n] = 0;			// end of string

	if ( CommEnableNotification ( EV_RXCHAR ) == TRUE )
	{
		m_bExit = FALSE;

		State1();

		::WaitExit ( m_bExit, 10 );
	}
	else
		m_nErrno = ERR_COMM_ENABLE;

	return m_nErrno;
}

/**********************************************************************/

void CTLPComms::State1()
{
	m_nState = 1;

	TxData();

	if ( CommStartTimer ( m_nRxTimeout ) == FALSE )
		Exit ( ERR_COMM_TIMER );
}

/**********************************************************************/

void CTLPComms::TxData()
{
	CommTxBlock ( (const char*) m_TxBuf, strlen((const char*)m_TxBuf) );
	m_nRxCount = 0;
}

/**********************************************************************/

void CTLPComms::TxNak()
{
	m_TxBuf[0] = 'E';
	m_TxBuf[1] = '0';
	m_TxBuf[2] = '0';
	m_TxBuf[3] = '0';
	m_TxBuf[4] = 'E';
	m_TxBuf[5] = '0';
	m_TxBuf[6] = cEOT;
	m_TxBuf[7] = 0;

	TxData();
}

/**********************************************************************/

bool CTLPComms::OpenPort ( const char* szPort, const char* szBaud )
{
	CString strComInit;
	strComInit.Format ( "%s:%s,n,8,1", szPort, szBaud );		/* eg COM1:9600,n,8,1 */

	if ( Open ( strComInit ) == TRUE )
	{
		DCB dcb;
		if ( GetCommState ( &dcb ) == TRUE )
		{
			dcb.fOutxCtsFlow = FALSE;	// disable cts handshaking
			dcb.fOutxDsrFlow = FALSE;	// disable dsr handshaking
			SetCommState ( &dcb );
		}
		RaiseDtr();
		::WaitLoop ( 200 );

		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CTLPComms::ClosePort()
{
	DropDtr();
	Close();
}

/**********************************************************************/

void CTLPComms::OnCommTimer()
{
	switch ( m_nState )
	{
	case 1:
		if ( ++m_nRetryCount < MAX_RETRIES )
		{
			TxData();
			break;
		}
		else
			Exit ( ERR_TIMEOUT );		/* timeout */
	}
}

/**********************************************************************/

void CTLPComms::OnCommRxChar ( char c )
{
	switch ( m_nState )
	{
	case 1:							/* waiting for data */
		if ( m_nRxCount < MAX_BLOCK - 1 )
		{
			m_RxBuf[m_nRxCount++] = c;
			if ( c == cEOT )
			{
				m_RxBuf[m_nRxCount] = 0;			/* end of string */
				if ( ValidateBlock() == TRUE )
				{
					if ( m_RxBuf[0] == cACK )
						Exit ( ERR_NONE );		/* ok */
					else						/* must be nak */
						TxData();				/* resend block */
				}
				else
				{
					if ( ++m_nNakCount < MAX_NAKS )
						TxNak();
					else
						Exit ( ERR_RETRY );		/* too many naks */
				}
			}
		}
		break;
	}
}

/**********************************************************************/

bool CTLPComms::ValidateBlock()
{
	int asclen = strlen ( (const char*) m_RxBuf );
	int hexlen = asclen / 2;

	int d = 0;
	for ( int i = 0 ; i < asclen ; i+=2 )
		m_RxBuf[d++] = GetByte ( m_RxBuf[i], m_RxBuf[i+1] );

	if ( m_RxBuf[0] != cACK && m_RxBuf[0] != cNAK )	/* 1st byte must be ack or nak */
		return FALSE;				/* invalid block */

	if ( m_RxBuf[1] != ( hexlen-3 ) )
		return FALSE;				/* invalid length */

	if ( m_RxBuf[hexlen-1] != CalcCRC ( m_RxBuf ) )
		return FALSE;				/* invalid crc */

	return TRUE;
}

/**********************************************************************/

void CTLPComms::Terminate()
{
	Exit ( ERR_USER_TERMINATE );		/* comms terminated by user */
}

/**********************************************************************/

void CTLPComms::Exit ( int nErrno )
{
	CommStopTimer();
	CommDisableNotification();

	m_nErrno = nErrno;
	m_bExit = TRUE;
}

/**********************************************************************/
/* general purpose subroutines								*/
/**********************************************************************/

unsigned char CTLPComms::GetLSN ( unsigned char c )
{
	c &= 0x0f;
	c += '0';			/* convert to ascii */
	if ( c > '9' )
		c+= 7;		/* ascii A to F */
	return ( c );
}

/**********************************************************************/

unsigned char CTLPComms::AsciiToHex ( unsigned char c )
{
	c -= '0';
	if ( c > 9 )
		c -= 7;
	return ( c );
}

/**********************************************************************/

unsigned char CTLPComms::GetByte ( unsigned char msd, unsigned char lsd )
{
	unsigned char digit = (( AsciiToHex ( msd ) << 4 ) | AsciiToHex ( lsd ) );
	return ( digit );
}

/**********************************************************************/

unsigned char CTLPComms::GetMSN ( unsigned char c )
{
	return ( GetLSN ( c >> 4 ) );
}

/**********************************************************************/

unsigned char CTLPComms::CalcCRC ( unsigned char* buf )
{
	unsigned char crc = 0;
	int len = (int) buf[1]+2;

	for ( int i = 0 ; i < len ; i++ )
		crc ^= buf[i];

	return ( crc );
}

/**********************************************************************/

unsigned char CTLPComms::CalcCRC ( CByteArray& ar )
{
	unsigned char  crc = 0;

	for ( int i = 0 ; i < ar.GetSize() ; i++ )
		crc ^= ar.GetAt(i);

	return crc;
}

/**********************************************************************/
