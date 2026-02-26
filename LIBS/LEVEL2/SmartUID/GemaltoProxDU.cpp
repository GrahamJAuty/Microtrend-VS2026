/**********************************************************************/
#include "local.h"
/**********************************************************************/

CGemaltoProxDU::CGemaltoProxDU ( CMifareReader* pReader )
{
	m_pReader = pReader;
	m_bExit = FALSE;
	m_nUIDFormat = 0;	// format 1;
}

/**********************************************************************/

CGemaltoProxDU::~CGemaltoProxDU()
{
	Terminate();
}

/**********************************************************************/

bool CGemaltoProxDU::IsValid(const char* szReader)
{
	CString strReader = szReader;

	if (strReader.Left(27) == "Gemalto Prox-DU Contactless")
	{
		return TRUE;
	}

	if (strReader.Left(27) == "Gemalto Prox-SU Contactless")
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

int CGemaltoProxDU::GetReaders(CStringArray& arReaders)
{
	SCARDCONTEXT hContext = NULL;
	DWORD dwReaders = 0;
	int nCount = 0;

	if (SCardListReaders(hContext, NULL, NULL, &dwReaders) == SCARD_S_SUCCESS)	// get required buffer size
	{
		DWORD dwBufSize = dwReaders;

		char* bufReaders = new char[dwBufSize];

		if (SCardListReaders(hContext, NULL, bufReaders, &dwReaders) == SCARD_S_SUCCESS)
		{
			CString str = "";

			for (DWORD i = 0; (i < dwReaders) && (i < dwBufSize); i++)
			{
				if (bufReaders[i] != 0)
				{
					str += bufReaders[i];
				}
				else
				{
					if (str != "")
					{
						if (IsValid(str) == TRUE)
						{
							arReaders.Add(str);
							nCount++;
						}
					}
					str = "";
				}
			}
		}

		if (bufReaders != NULL)
		{
			delete[] bufReaders;
		}
	}

	return nCount;		// return number of readers
}

/**********************************************************************/

int CGemaltoProxDU::GetUID ( const char* szReader, bool bWaitForCard, bool bLogRead )
{
	int nReply = 1;		// assume error
	m_strError = "";

	OnDisplayPresentCard();		// display Present Card message

	SCARDCONTEXT hContext = NULL;
	DWORD dwScope = SCARD_SCOPE_USER;
	long lReply = 0;

	if (( lReply = SCardEstablishContext ( dwScope, NULL, NULL, &hContext )) == SCARD_S_SUCCESS )
	{
		SCARDHANDLE hCard;
		DWORD dwActiveProtocol = 0;

		// wait for card to be presented

		while (( lReply = SCardConnect ( hContext, szReader, SCARD_SHARE_DIRECT, SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol )) != SCARD_S_SUCCESS )
		{
			if (FALSE == m_bQuickScan)
			{
				if (::WaitExit(m_bExit, 500, 50) == TRUE)	// wait 500 msecs or m_bExit = TRUE (returns TRUE if m_bExit = TRUE)
				{
					break;
				}
			}

			if ((bWaitForCard == FALSE) || (TRUE == m_bQuickScan))
			{
				nReply = 2;			// card not present
				break;
			}
		}

		if ((m_bExit == FALSE) && (lReply == SCARD_S_SUCCESS))
		{
			BYTE TxBuffer[5] = { 0xff, 0xca, 0x00, 0x00, 0x00 };
			BYTE RxBuffer[32];
			SCARD_IO_REQUEST request{};
			request.dwProtocol = dwActiveProtocol;
			request.cbPciLength = sizeof(SCARD_IO_REQUEST);
			DWORD dwTxLen = sizeof ( TxBuffer );
			DWORD dwRxLen = sizeof ( RxBuffer );

			if ((lReply = SCardTransmit(hCard, &request, TxBuffer, dwTxLen, NULL, RxBuffer, &dwRxLen)) == SCARD_S_SUCCESS)
			{
				if (TRUE == bLogRead)
				{
					CString strLogFile = ".\\SCARDLOG.TXT";
					CString strLog = "GEMALTOPROXDU   ";

					for (UINT n = 0; n < dwRxLen; n++)
					{
						CString strByte = "";
						strByte.Format("%2.2X ", RxBuffer[n]);
						strLog += strByte;
					}

					CSSFile fileLog;
					if (fileLog.Open(strLogFile, "ab") == TRUE)
					{
						fileLog.WriteLine(strLog);
						fileLog.Close();
					}
				}

				switch (dwRxLen)
				{
				case 6:		// 4 byte UID + SW1 + SW2
				case 9:		// 7 byte UID + SW1 + SW2
				{
					// command executed successfully
					if ((RxBuffer[dwRxLen - 2]) == 0x90 && (RxBuffer[dwRxLen - 1] == 0x00))
					{
						CString strUID = "";

						if (m_nUIDFormat == 0)
						{
							for (DWORD n = 0; n <= dwRxLen - 3; n++)
							{
								CString strByte = "";
								strByte.Format("%2.2X", RxBuffer[dwRxLen - (3 + n)]);
								strUID += strByte;
							}
						}
						else
						{
							for (DWORD n = 0; n <= dwRxLen - 3; n++)
							{
								CString strByte = "";
								strByte.Format("%2.2X", RxBuffer[n]);
								strUID += strByte;
							}
						}

						if (9 == dwRxLen)
						{
							strUID = strUID.Right(8);
						}

						OnGotUID(strUID);	// do something with Card UID
						nReply = 0;
					}
					else
					{
						m_strError.Format("SCardTransmit: response error %2.2X%2.2X", RxBuffer[dwRxLen - 2], RxBuffer[dwRxLen - 1]);
					}

					OnDisplayRemoveCard();		// display Remove Card message

					DWORD dwCardStatus = 0;
					int nWaitTime = m_bQuickScan ? 100 : 500;
					bool bFirstCheck = TRUE;

					// wait for card to be removed
					do
					{
						if ((TRUE == m_bQuickScan) || (FALSE == bFirstCheck))
						{
							if (GetCardStatus(hCard, dwCardStatus) != SCARD_S_SUCCESS)
							{
								break;
							}
						}

						bFirstCheck = FALSE;

						// wait alloted time or m_bExit = TRUE (returns TRUE if m_bExit = TRUE)
						if (::WaitExit(m_bExit, nWaitTime, 50) == TRUE)
						{
							break;
						}

					} while (dwCardStatus == SCARD_PRESENT || dwCardStatus == SCARD_SPECIFIC);
				}
				break;

				default:
					m_strError.Format("SCardTransmit: length error %d", dwRxLen);
					break;
				}
			}
			else
			{
				m_strError.Format ( "SCardTransmit: error 0x%8.8X", lReply );
			}

			SCardDisconnect ( hCard, SCARD_EJECT_CARD );
		}

		SCardReleaseContext ( hContext );
	}
	else
	{
		m_strError.Format ( "SCardEstablishContext: error 0x%8.8X", lReply );
	}

	if (m_bExit == TRUE)
	{
		nReply = -1;		// terminated
	}

	return nReply;
}

/**********************************************************************/

long CGemaltoProxDU::GetCardStatus ( SCARDHANDLE hCard, DWORD& dwCardStatus )
{
	char bufReaders[1024];
	DWORD dwReaders = sizeof(bufReaders);
	DWORD dwState;
	DWORD dwProtocol;
	BYTE ucAtr[32] = { 0 };
	DWORD dwAtrLen = sizeof ( ucAtr );

	long lReply = SCardStatus ( hCard, bufReaders, &dwReaders, &dwState, &dwProtocol, (BYTE*)&ucAtr, &dwAtrLen );

	dwCardStatus = ( lReply == SCARD_S_SUCCESS ) ? dwState : 0;

	return lReply;
}

/**********************************************************************/
