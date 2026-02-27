//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "WebpaymentResendDlg.h"
//$$******************************************************************

CWebpaymentResendDlg::CWebpaymentResendDlg ( const char* szTitle, const char* szDbFilename, const char* szDestination, const char* szDisplayHeader, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CWebpaymentResendDlg::IDD, pParent)
	, m_csvDisplayHeader(szDisplayHeader)
{
	m_strTitle = szTitle;
	m_strDbFilename = szDbFilename;
	m_strDestination = szDestination;
	m_pParent = pParent;
	m_nTotal = 0;
	m_nCount = 0;
	m_nConnectingPOSFieldNo = -1;
	m_nConnectingPOSTxnID = 0;
	m_strSortField = "";
	m_nSortType = DB_SORT_INT;

	m_db.OpenSSDatabaseReadWrite ( szDbFilename, "", 10 );				// may need to write back to remove multiple entries	
}

//$$******************************************************************

void CWebpaymentResendDlg::SetConnectingPOSTxnID(int nConnectingPOSFieldNo, __int64 nConnectingPOSTxnID)
{
	m_nConnectingPOSFieldNo = nConnectingPOSFieldNo;
	m_nConnectingPOSTxnID = nConnectingPOSTxnID;
}

//$$******************************************************************

void CWebpaymentResendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CARDPICKER, m_listctrlPicker);
	DDX_Control(pDX, IDC_SPIN_CARDPICKER, m_spinCardPicker);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_nTotal);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CWebpaymentResendDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_CARDPICKER, OnItemChangingList)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CARDPICKER, OnSpinCardPicker)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CWebpaymentResendDlg::CSSAutoShutdownPostInitDialog()
{
	CString strMsg = "";
	strMsg.Format("Re send Web Payment %s",
		(const char*)m_strTitle);

	SetWindowText(strMsg);

	for (int i = 0; i < m_csvDisplayHeader.GetSize(); i++)
	{
		m_listctrlPicker.InsertColumn(i, m_csvDisplayHeader.GetString(i), LVCFMT_LEFT, 100);
	}

	m_listctrlPicker.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	if (m_strSortField != "")
	{
		m_db.AddSort(m_strSortField, m_nSortType, DB_SORT_ASCENDING);
		m_db.Sort();
		RemoveMultiples(m_csvDisplayHeader.GetString(0));
	}

	FillList();

	m_listctrlPicker.SetCurSel(0);
	m_listctrlPicker.SetFocus();
	return FALSE;
}

//$$******************************************************************

bool CWebpaymentResendDlg::GetSendConfirmation()
{
	CString strMsg = "";

	strMsg.Format ( "%d lines have been selected to be re sent.\n\nDo you wish to send the information now?", m_nCount );

	return  ( Prompter.YesNo( strMsg, m_strTitle ) == IDYES ) ? TRUE : FALSE;
}

//$$******************************************************************

void CWebpaymentResendDlg::FillList()
{
	m_db.MoveFirst();

	do
	{
		CCSV csv ( '\t' );
		for (int i = 0; i < m_csvDisplayHeader.GetSize(); i++)
		{
			csv.Add(m_db.GetString(m_csvDisplayHeader.GetString(i)));
		}

		m_listctrlPicker.AddString ( csv.GetLine() );
	}
	while ( m_db.MoveNext() == TRUE );

	EnableSpinFind ( 0 );

	m_nCount = 0;
	m_nTotal = m_listctrlPicker.GetItemCount();
	UpdateData(FALSE);
}

//$$******************************************************************

void CWebpaymentResendDlg::OnItemChangingList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    UINT uNewState = ( pNMListView->uNewState & LVIS_STATEIMAGEMASK );
//	int nItem = pNMListView->iItem;

	if (uNewState == 0x2000)
	{
		m_nCount++;		// about to be ticked
	}
	if (uNewState == 0x1000)
	{
		m_nCount--;		// about to be cleared
	}

	UpdateData(FALSE);
	EnableSpinFind ( m_nCount );

	*pResult = FALSE;
}

//$$******************************************************************

void CWebpaymentResendDlg::OnSpinCardPicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nIndex = m_listctrlPicker.GetCurSel();
	
	bool bNextFound = FALSE;

	if ( pNMUpDown->iDelta > 0 )			// previous / Down
	{	
		for ( --nIndex ; nIndex >= 0 ; nIndex-- )
		{
			if ( m_listctrlPicker.GetCheck(nIndex) == TRUE )
			{
				bNextFound = TRUE;
				break;
			}
		}
	}
	else									// next / Up
	{
		for ( ++nIndex ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
		{
			if ( m_listctrlPicker.GetCheck(nIndex) == TRUE )
			{
				bNextFound = TRUE;
				break;
			}
		}
	}

	if ( bNextFound == TRUE )
	{
		m_listctrlPicker.SetCurSel(nIndex);
		m_listctrlPicker.SetFocus();
	}

	*pResult = 0;
}

//$$******************************************************************

void CWebpaymentResendDlg::RemoveMultiples ( const char* szSortLabel )
{
	m_db.MoveFirst();
	CString strLabel = m_db.GetString ( szSortLabel );				// first sort label

	CString strNextLabel = "";
	bool bGetNextRecord = TRUE;

	int nMax = m_db.GetRecordCount();
	for ( int i = 0 ; i < nMax ; i++ )
	{
		if (bGetNextRecord == TRUE)
		{
			if (m_db.MoveNext() == FALSE)
			{
				break;
			}
		}

		strNextLabel = m_db.GetString ( szSortLabel );
		if ( strNextLabel != strLabel )
		{
			strLabel = strNextLabel;
			bGetNextRecord = TRUE;
		}
		else
		{
			m_db.DeleteRecord();
			bGetNextRecord = FALSE;
		}
	}
}

//$$******************************************************************

int CWebpaymentResendDlg::GetSelectedCount()
{
	int nCount = 0;
	for (int nIndex = 0; nIndex < m_listctrlPicker.GetItemCount(); nIndex++)
	{
		if (m_listctrlPicker.GetCheck(nIndex) == TRUE)
		{
			++nCount;
		}
	}

	EnableSpinFind ( nCount );
	return nCount;
}

//$$******************************************************************

void CWebpaymentResendDlg::EnableSpinFind ( int nCount )
{
	bool bEnable = ( nCount == 0 ) ? FALSE : TRUE;
	m_spinCardPicker.EnableWindow(bEnable);
}

//$$******************************************************************

void CWebpaymentResendDlg::OnButtonInvert()
{
	CWaitCursor cursor;
	for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
	{
		BOOL bReqd = m_listctrlPicker.GetCheck(nIndex);
		m_listctrlPicker.SetCheck(nIndex, !bReqd);
	}

	m_nCount = GetSelectedCount();
	UpdateData(FALSE);
}

//$$******************************************************************

void CWebpaymentResendDlg::OnButtonAll()
{ 
	SetAll ( TRUE );	
}

//$$******************************************************************

void CWebpaymentResendDlg::OnButtonClear()	
{ 
	SetAll ( FALSE );
}

//$$******************************************************************

void CWebpaymentResendDlg::SetAll ( bool bReqd )
{
	CWaitCursor cursor;

	for (int nIndex = 0; nIndex < m_listctrlPicker.GetItemCount(); nIndex++)
	{
		m_listctrlPicker.SetCheck(nIndex, bReqd);
	}

	m_nCount = GetSelectedCount();
	UpdateData(FALSE);
}

//$$******************************************************************

void CWebpaymentResendDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (m_nCount == 0)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			Prompter.Error("No records have been selected", "Error");
			return;
		}

		m_db.Close();											// save & close database

		{
			bool bAllowRetries = FALSE;

			if (Server.Read(Filenames.GetServerDataFilename()) == TRUE)
			{
				bAllowRetries = Server.GetAllowRetriesFlag();
			}

			if (FALSE == bAllowRetries)
			{
				Prompter.Info("Retries have been blocked.\nPlease consult your dealer for further advice.");
				EndDialog(IDCANCEL);
				return;
			}
		}

		CSSFile fileIn;
		if (fileIn.Open(m_strDbFilename, "rb") == TRUE)
		{
			CSSFile fileOut;
			if (fileOut.Open(m_strDestination, "wb") == TRUE)
			{
				CString strBuf;
				fileIn.ReadLine(strBuf);						// read header
				fileOut.WriteString(strBuf);					// save header

				for (int nIndex = 0; nIndex < m_listctrlPicker.GetItemCount(); nIndex++)
				{
					fileIn.ReadString(strBuf);

					if (m_listctrlPicker.GetCheck(nIndex) == TRUE)
					{
						if (m_nConnectingPOSFieldNo != -1)	// see if data needs txid updating
						{
							CCSV csv(strBuf);
							csv.SetAt(m_nConnectingPOSFieldNo, m_nConnectingPOSTxnID++);
							fileOut.Write(csv);
						}
						else
						{
							fileOut.WriteLine(strBuf);
						}
					}
				}

				fileOut.Close();
			}
			fileIn.Close();
		}
	}

	EndDialog(IDOK);
}

//*******************************************************************
