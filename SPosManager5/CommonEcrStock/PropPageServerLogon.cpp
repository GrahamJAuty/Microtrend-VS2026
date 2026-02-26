/**********************************************************************/
#include "LocationCSVArray.h"
#include "NamespaceServerLogon.h"
/**********************************************************************/
#include <sstream>
#include <ranges>
/**********************************************************************/
#include "PropPageServerLogon.h"
/**********************************************************************/

CPropPageServerLogon::CPropPageServerLogon() : CSSPropertyPage(CPropPageServerLogon::IDD)
{
	//{{AFX_DATA_INIT(CPropPageServerLogon)
	//}}AFX_DATA_INIT
	m_terminal.Read();
}

/**********************************************************************/

CPropPageServerLogon::~CPropPageServerLogon()
{
}

/**********************************************************************/

void CPropPageServerLogon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageServerLogon)
	DDX_Control(pDX, IDC_EDIT_NAME_RECEIPT, m_editNameReceipt);
	DDX_Control(pDX, IDC_EDIT_NAME_FULL, m_editNameFull);
	DDX_Control(pDX, IDC_EDIT_LOGON, m_editLogon);
	DDX_Control(pDX, IDC_BUTTON_REMOVECARD, m_buttonRemoveCard);
	DDX_Control(pDX, IDC_BUTTON_NEWCARD, m_buttonNewCard);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageServerLogon, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageServerLogon)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_REMOVECARD, OnButtonRemoveCard)
	ON_BN_CLICKED(IDC_BUTTON_NEWCARD, OnButtonNewCard)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageServerLogon::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_editNameReceipt.LimitText( Server::ReceiptName.Max );
	m_editNameFull.LimitText( Server::FullName.Max );

	m_listLogon.SubclassDlgItem(IDC_LIST, this);
	m_listLogon.InsertColumn(0, "Hex", LVCFMT_LEFT, 150);
	m_listLogon.InsertColumn(1, "Decimal", LVCFMT_LEFT, 150);
    m_listLogon.ModifyStyle(LVS_SINGLESEL, 0, 0);
	m_listLogon.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

void CPropPageServerLogon::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listLogon.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CPropPageServerLogon::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		CString strCell = "";

		int nIndex = pDispInfo->item.iItem;
		
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayLogons.GetCount() ) )
		{
			__int64 nLogon = m_arrayLogons[nIndex];

			switch (pDispInfo->item.iSubItem)
			{
			case 0: // Hex
				strCell.Format("0x%I64X", nLogon);
				break;

			case 1: //decimal
				strCell.Format("%I64d", nLogon);
				break;
			}
		}
	
		lstrcpy(pDispInfo->item.pszText, strCell);
	}
	*pResult = 0;
}

/**********************************************************************/

BOOL CPropPageServerLogon::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageServerLogon::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageServerLogon::GetRecordData()
{
	DataManagerNonDb.ServerLogon.GetLogonListForServer(m_pServerRecord->GetServerNo(), m_arrayLogons);
}

/**********************************************************************/

void CPropPageServerLogon::OnButtonRemoveCard()
{
	CArray<int, int> arraySelected;

	POSITION pos = m_listLogon.GetFirstSelectedItemPosition();

	while (pos)
	{
		arraySelected.Add(m_listLogon.GetNextSelectedItem(pos));
	}

	if ( arraySelected.GetSize() == 0 )
	{
		Prompter.Error("No logons are selected for removal");
		return;
	}

	if ( Prompter.YesNo("Are you sure you want to remove the selected logons ?") != IDYES )
	{
		return;
	}

	for ( int n = arraySelected.GetCount() - 1; n >= 0; n--)
	{
		int nIndex = arraySelected[n];
		if ((nIndex >= 0) && (nIndex < m_arrayLogons.GetCount()))
		{
			m_arrayLogons.RemoveAt(nIndex);
		}
	}

	m_listLogon.SetItemCountEx(m_arrayLogons.GetSize());
	m_listLogon.SetCurSel(0);
	m_listLogon.Invalidate();
}

/**********************************************************************/

void CPropPageServerLogon::OnButtonNewCard() 
{
	if (m_terminal.m_strTerminal == szTERMINAL_NONE)
	{
		CString strError = "";
		strError += "No MiFare terminal has been selected.\n\n";
		strError += "Please select a terminal in system options, then try again.";
		Prompter.Error(strError);
		return;
	}

	bool bShownNumberError = FALSE;

	if (m_bReaderMode == TRUE)
	{
		m_reader.Terminate();
	}
	else
	{
		m_bReaderMode = TRUE;
		m_buttonNewCard.SetWindowText("Cancel\nRead");

		if (m_reader.SelectReader(m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud) == TRUE)
		{
			m_reader.SetUIDFormat(m_terminal.m_nUIDFormat1);
			m_reader.SetDataFormat(m_terminal.m_nDataFormat1);
			m_reader.m_pWnd = &m_editLogon;						// NOTE: sets strExtRef variable to "Present card"
			m_reader.m_bWaitForCard = TRUE;

			while (TRUE)
			{
				int nReply;
				if ((nReply = m_reader.GetUID(TRUE, FALSE)) == 0)	// 0=no error, 1=error, -1 = terminate
				{
					CString strMiFareNo = m_reader.GetCardNo();

					CString strNumberError = "";
					if (ValidateMiFareNo(strMiFareNo, strNumberError) == TRUE)
					{
						m_arrayLogons.Add(_atoi64(strMiFareNo));
						m_listLogon.SetItemCountEx(m_arrayLogons.GetSize());

                        for (int i = 0; i < m_listLogon.GetItemCount(); ++i)
                        {
                            m_listLogon.SetItemState(i, 0, LVIS_SELECTED);
                        }

						m_listLogon.SetCurSel(m_arrayLogons.GetSize() - 1);
						m_listLogon.Invalidate();
					}
					else
					{
						m_editLogon.SetWindowText(strNumberError);
						bShownNumberError = TRUE;
					}

					break;
				}
				else if (nReply == -1)								// terminate 
				{
					break;
				}
				else
				{
					Prompter.Error(m_reader.GetError(), "Reader Error");
				}
			}

			m_reader.DeselectReader();
		}
		else
		{
			CString strError = m_reader.GetError();
			Prompter.Error(strError, "Hardware Error");
		}
	}

	m_bReaderMode = FALSE;
	m_buttonNewCard.SetWindowText("Read\nCard");

	if ( FALSE == bShownNumberError )
	{
		m_editLogon.SetWindowText("");
	}
}

/**********************************************************************/

void CPropPageServerLogon::SetRecordControls()
{
	m_editNameReceipt.SetWindowText(m_pServerRecord->GetReceiptEditName());
	m_editNameFull.SetWindowText(m_pServerRecord->GetFullName());
	m_editNameReceipt.SetFocus();
	m_editNameReceipt.SetSel(0, -1);
	m_listLogon.SetItemCountEx(m_arrayLogons.GetSize());
	m_listLogon.SetCurSel(0);
	m_listLogon.Invalidate();
}

/**********************************************************************/

bool CPropPageServerLogon::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageServerLogon::SaveRecord()
{
	m_pServerRecord->SetReceiptName(GetEditBoxText(m_editNameReceipt));
	m_pServerRecord->SetFullName(GetEditBoxText(m_editNameFull));
	DataManagerNonDb.ServerLogon.SetLogonListForServer(m_pServerRecord->GetServerNo(), m_arrayLogons);
}

/**********************************************************************/

void CPropPageServerLogon::Refresh()
{
	GetRecordData();
	UpdateData(FALSE);
	SetRecordControls();
}

/**********************************************************************/

bool CPropPageServerLogon::ValidateMiFareNo(CString strMiFareNo, CString& strNumberError) const
{
	if (strMiFareNo.IsEmpty() || strMiFareNo.GetLength() > 11)
	{
		strNumberError = "Range error 1";
		return FALSE;
	}

	if (::TestNumeric(strMiFareNo) == FALSE )
	{
		strNumberError = "Not numeric";
		return FALSE;
	}
	
	__int64 nLogon = _atoi64(strMiFareNo);
	if ((nLogon < ServerLogon::LogonNo.Min) || (nLogon > ServerLogon::LogonNo.Max))
	{
		strNumberError = "Range error 2";
		return FALSE;
	}

	for ( int n = 0; n < m_arrayLogons.GetCount(); n++ )
	{
		if (m_arrayLogons[n] == nLogon)
		{
			strNumberError = "Already linked";
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/

void CPropPageServerLogon::OnCancel()
{
	if ( TRUE == m_bReaderMode)
	{
		m_reader.Terminate();
	}
	else
	{
		CSSPropertyPage::OnCancel();
	}
}

/**********************************************************************/

void CPropPageServerLogon::OnOK()
{
	if (TRUE == m_bReaderMode)
	{
		m_reader.Terminate();
	}

	CSSPropertyPage::OnOK();
}

/**********************************************************************/
