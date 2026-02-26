/**********************************************************************/
#include "local.h"
/**********************************************************************/
#define	szREADER	"Gemalto Prox-P2 Contactless"
/**********************************************************************/

CGemaltoProxP2::CGemaltoProxP2 ( CMifareReader* pReader )
{
	m_pReader = pReader;
	m_bPortOpen = FALSE;
	m_bExit = FALSE;
	m_nUIDFormat = 0;	// format 1;
}

/**********************************************************************/

CGemaltoProxP2::~CGemaltoProxP2()
{
	Terminate();
	ClosePort();
}

/**********************************************************************/

bool CGemaltoProxP2::OpenPort(const char* szPort, const char* szBaud)
{
	if (m_bPortOpen == FALSE)
	{
		m_bPortOpen = m_comms.OpenPort(szPort, szBaud);

		if (m_bPortOpen == TRUE)
		{
			CByteArray ar;
			ar.Add(0xe6);		// read mode just to see if reader responds to current baud rate

			if (m_comms.TxCommand(ar, 50) != 0)	// 50ms (4 retries)
			{
				m_strError = "Reader is not responding";
				ClosePort();
			}
		}
		else
		{
			m_strError = "Cannot open comms port";
		}
	}

	return m_bPortOpen;
}

/**********************************************************************/

void CGemaltoProxP2::ClosePort()
{
	if (m_bPortOpen == TRUE)
	{
		m_comms.ClosePort();
		m_bPortOpen = FALSE;
	}
}

/**********************************************************************/

bool CGemaltoProxP2::IsValid(const char* szReader)
{
	return (strcmp(szReader, szREADER) == 0) ? TRUE : FALSE;
}

/**********************************************************************/

int CGemaltoProxP2::GetReaders(CStringArray& arReaders)
{
	int nCount = 1;
	arReaders.Add(szREADER);
	return nCount;		// return number of readers
}

/**********************************************************************/

int CGemaltoProxP2::RFReset()
{
	CByteArray ar;
	ar.Add(0xe1);	// rf reset

	int nReply = m_comms.TxCommand(ar, 1000);	// 1 second (4 retries)
	if (nReply == 0)
	{
		nReply = m_comms.GetRxStatus();
	}

	return nReply;		// 0 = ok, < 0 are comms errors, > 0 are reader status errors
}

/**********************************************************************/

int CGemaltoProxP2::GetFirstCard(BYTE ucTimeout)
{
	CByteArray ar;
	ar.Add(0xe2);		// get first card
	ar.Add(0x01);
	ar.Add(ucTimeout);	// ucTimeout*100ms

	int nReply = m_comms.TxCommand(ar, 1000);		// 1 second (4 retries)
	if (nReply == 0)
	{
		nReply = m_comms.GetRxStatus();
	}

	return nReply;		// 0 = ok, < 0 are comms errors, > 0 are reader status errors
}

/**********************************************************************/

int CGemaltoProxP2::GetUID(const char* szReader, bool bWaitForCard, bool bLogRead)
{
	int nReply = 1;		// assume error
	m_strError = "";

	if (m_bPortOpen == TRUE)
	{
		OnDisplayPresentCard();		// display Present Card message

		int nStatus = RFReset();
		if (nStatus == 0 || nStatus == 0xfb)		// no error or card not present
		{
			while (TRUE)			// wait for card to be presented
			{
				nStatus = GetFirstCard(10);	// 10 * 100ms

				if (nStatus == 0)				// card present
				{
					break;
				}
				else if (nStatus == 0xfb)		// card not present
				{
					if (bWaitForCard == FALSE)
					{
						nReply = 2;			// card not present
						break;
					}
				}

				if (::WaitExit(m_bExit, 500, 50) == TRUE)	// wait 500 msecs or m_bExit = TRUE (returns TRUE if m_bExit = TRUE)
				{
					break;
				}
			}

			if (m_bExit == FALSE && nStatus == 0)
			{
				BYTE ucLen = m_comms.GetRxLen();

				if (TRUE == bLogRead)
				{
					CString strLogFile = ".\\SCARDLOG.TXT";
					CString strLog = "GEMALTOPROXP2   ";

					for (int n = 0; n < m_comms.GetRxLen(); n++)
					{
						CString strByte = "";
						strByte.Format("%2.2X ", m_comms.GetRxBuf(n));
						strLog += strByte;
					}

					CSSFile fileLog;
					if (fileLog.Open(strLogFile, "ab") == TRUE)
					{
						fileLog.WriteLine(strLog);
						fileLog.Close();
					}
				}

				if (ucLen == 12 && m_comms.GetRxBuf(3) == 0x3b && m_comms.GetRxBuf(4) == 0x87 && m_comms.GetRxBuf(5) == 0x01)	// mifare card with 4 byte UID
				{
					CString strUID = "";

					if (m_nUIDFormat == 0)
					{
						strUID.Format("%2.2X%2.2X%2.2X%2.2X", m_comms.GetRxBuf(8 + 0), m_comms.GetRxBuf(8 + 1), m_comms.GetRxBuf(8 + 2), m_comms.GetRxBuf(8 + 3));	// format 1
					}
					else
					{
						strUID.Format("%2.2X%2.2X%2.2X%2.2X", m_comms.GetRxBuf(8 + 3), m_comms.GetRxBuf(8 + 2), m_comms.GetRxBuf(8 + 1), m_comms.GetRxBuf(8 + 0));	// format 2
					}

					OnGotUID(strUID);	// do something with Card UID

					nReply = 0;	// got UID

					OnDisplayRemoveCard();	// display Remove Card message

					while (TRUE)			// wait for card to be removed
					{
						nStatus = GetFirstCard(3);	// 3 * 100ms

						if (nStatus == 0xfb)		// card not present
						{
							break;
						}

						if (::WaitExit(m_bExit, 200, 50) == TRUE)	// wait 200 msecs or m_bExit = TRUE (returns TRUE if m_bExit = TRUE)
						{
							break;
						}
					}
				}
				else
				{
					m_strError = "Unknown card type";
				}
			}
		}
		else
		{
			if (nStatus < 0)
			{
				m_strError.Format("Comms error: %d", nStatus);
			}
			else
			{
				m_strError.Format("Reader error: 0x%2.2X", nStatus);
			}
		}
	}
	else
	{
		m_strError = "Comms port has not been opened";
	}

	if (m_bExit == TRUE)
	{
		nReply = -1;		// terminated
	}

	return nReply;
}

/**********************************************************************/
