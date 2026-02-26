/**********************************************************************/
#include "local.h"
#include "sysset3.h"
/**********************************************************************/
#include "SyssetDlg4.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CSyssetDlg4, CDialog)

/**********************************************************************/

CSyssetDlg4::CSyssetDlg4(CWnd* pParent /*=NULL*/)
	: CDialog(CSyssetDlg4::IDD, pParent)
{
}

/**********************************************************************/

CSyssetDlg4::~CSyssetDlg4()
{
}

/**********************************************************************/

void CSyssetDlg4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSSET_LIST_USERS, m_listUsers);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSyssetDlg4, CDialog)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SYSSET_LIST_USERS, OnColumnClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSyssetDlg4::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText("Status");		

	m_listUsers.AddColumn("", 25, LVCFMT_LEFT);				//InstanceNo
	m_listUsers.AddColumn("WinUser", 120, LVCFMT_LEFT);	
	m_listUsers.AddColumn("Host PC", 120, LVCFMT_LEFT);
	m_listUsers.AddColumn("Session", 120, LVCFMT_LEFT);
	m_listUsers.AddColumn("Host ID", 120, LVCFMT_LEFT);
	m_listUsers.AddColumn("Date", 80, LVCFMT_LEFT);
	m_listUsers.AddColumn("Time", 80, LVCFMT_LEFT);
	m_listUsers.AddColumn("Status", 80, LVCFMT_LEFT);	

	DisplayUsers();

	m_nTimer = SetTimer(1, 2000, NULL);

	return TRUE;
}

/**********************************************************************/

void CSyssetDlg4::OnTimer(UINT_PTR nIDEvent)
{
	DisplayUsers();
	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CSyssetDlg4::DisplayUsers()
{
	if (m_pSysset != NULL)
	{
		m_listUsers.DeleteAllItems();		// delete current rows

		int nMaxUsers = m_pSysset->GetMaxUsers();

		for (int nInstNo = 1; nInstNo <= nMaxUsers; nInstNo++)
		{
			CCSV csv;

			CString strInstanceNo = "";
			strInstanceNo.Format("%d", nInstNo);	// InstanceNo

			CString strFilename = m_pSysset->GetShareFilename(nInstNo);

			CSSFile file;
			if (file.Open(strFilename, "rb") == TRUE)
			{
				CString strBuf = "";
				CString strProgUser = "";

				file.ReadString(strBuf);

				if (TRUE == m_bShowProgUser)
				{
					CSSFile filePG;
					if (filePG.Open(m_pSysset->GetProgUserFilename(nInstNo), "rb") == TRUE)
					{
						CString strCheck = "";
						filePG.ReadString(strCheck);

						if (strCheck == strBuf)
						{
							filePG.ReadString(strProgUser);
						}
						filePG.Close();
					}
				}

				CCSV csvBuf(strBuf);

				if (m_pSysset->GetInstanceNo() == nInstNo)
				{
					strInstanceNo += "*";		// highlight current instance
				}

				CString strUser = "";
				if (TRUE == m_bShowProgUser)
				{
					strUser = strProgUser;
				}
				else
				{
					strUser = csvBuf.GetString(0);		// WinUser
				}

				CString strHostOrClientName = "";
				if (TRUE == m_bShowClientPC)
				{
					strHostOrClientName = csvBuf.GetString(2);	// Client
				}
				else
				{
					strHostOrClientName = csvBuf.GetString(1);	// Host
				}

				CString strHostOrClientID = "";
				if (TRUE == m_bShowClientPC)
				{
					strHostOrClientID = csvBuf.GetString(5);	// ID (PCnnnnnnnnnn)
				}
				else
				{
					strHostOrClientID = csvBuf.GetString(4);	// ID (PCnnnnnnnnnn)
				}

				csv.Add(strInstanceNo);
				csv.Add(strUser);
				csv.Add(strHostOrClientName);
				csv.Add(csvBuf.GetString(3));	// Session
				csv.Add(strHostOrClientID);		// ID (PCnnnnnnnnnn)
				csv.Add(csvBuf.GetString(6));	// Date
				csv.Add(csvBuf.GetString(7));	// Time
				csv.Add("In Use");
				file.Close();

				remove(strFilename);			// try to delete in case file has been left hanging around
				// wont be deleted if file really is still being used
			}
			else
			{
				csv.Add(strInstanceNo);		// InstanceNo
				csv.Add("");
				csv.Add("");
				csv.Add("");
				csv.Add("");
				csv.Add("");
				csv.Add("");
				csv.Add("Available");
			}

			m_listUsers.AddRow(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CSyssetDlg4::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nColumn = pNMLV->iSubItem;
	
	switch(nColumn)
	{	
	case 1:
	{
		m_bShowProgUser = !m_bShowProgUser;
		SetColumnHeader(1, m_bShowProgUser ? "ProgUser" : "WinUser");
		DisplayUsers();
	}
	break;

	case 2:
	case 4:
	{
		m_bShowClientPC = !m_bShowClientPC;
		SetColumnHeader(2, m_bShowClientPC ? "Client PC" : "Host PC");
		SetColumnHeader(4, m_bShowClientPC ? "Client ID" : "Host ID");
		DisplayUsers();
	}
	break;
	}
	
	*pResult = 0;
}

/**********************************************************************/

void CSyssetDlg4::SetColumnHeader(int nColumn, CString strHeader)
{
	LVCOLUMN lvCol;
	::ZeroMemory(&lvCol, sizeof(LVCOLUMN));
	lvCol.mask = LVCF_TEXT;
	lvCol.pszText = strHeader.GetBuffer();
	m_listUsers.SetColumn(nColumn, &lvCol); // Update column at index 
}

/**********************************************************************/

void CSyssetDlg4::OnCancel()
{
	KillTimer ( m_nTimer );   

	CDialog::OnCancel();
}

/**********************************************************************/
