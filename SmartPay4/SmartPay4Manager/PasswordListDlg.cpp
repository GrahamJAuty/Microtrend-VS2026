/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
/**********************************************************************/
#include "PasswordLogDlg.h"
#include "SelectMultiplePasswordDlg.h"
#include "SetupPasswordOptionsDlg.h"
/**********************************************************************/
#include "PasswordListDlg.h"
/**********************************************************************/

CPasswordListDlg::CPasswordListDlg( CPasswordData* pPasswords, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CPasswordListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordListDlg)
	//}}AFX_DATA_INIT
	m_pPasswords = pPasswords;
}

/**********************************************************************/

void CPasswordListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordListDlg)
	DDX_Control(pDX, IDC_EDIT_PWMASTER, m_editMasterPassword);
	DDX_Control(pDX, IDC_BUTTON_LOG, m_buttonShowLog);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPasswordListDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CPasswordListDlg)
	
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonShowLog)
	/*****/
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonEditPassword)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDeletePassword)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, OnButtonClonePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPasswordListDlg::CSSAutoShutdownPostInitDialog()
{
	m_listPasswords.SubclassDlgItem(IDC_LIST, this);
	m_listPasswords.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listPasswords.InsertColumn(0, "No", LVCFMT_LEFT, 40);
	m_listPasswords.InsertColumn(1, "User Name", LVCFMT_LEFT, 220);
	m_listPasswords.InsertColumn(2, "Password", LVCFMT_LEFT, 220);

	m_editMasterPassword.LimitText(MAX_PASSWORD_LEN);
	m_editMasterPassword.SetWindowText(m_pPasswords->GetPassword(0));

	FillListBox();

	m_listPasswords.SetCurSel(0);

	return TRUE;
}

/**********************************************************************/

void CPasswordListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  if ( nIDCtl == IDC_LIST )  
   {
	   m_listPasswords.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPasswordListDlg::OnButtonShowLog() 
{
	CPasswordLogDlg dlg(this);
	dlg.DoModal();
}

/**********************************************************************/

void CPasswordListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditPassword();
	*pResult = 0;
}

/**********************************************************************/

void CPasswordListDlg::OnButtonEditPassword() 
{
	int nIndex = m_listPasswords.GetCurSel();
	
	if ( ( nIndex >= 0 ) && ( nIndex < 20 ))
	{
		CSetupPasswordOptionsDlg dlg(nIndex + 1, m_pPasswords);
		if (dlg.DoModal() == IDOK)
		{
			FillListBox();
			m_listPasswords.SetCurSel(nIndex);
		}
	}
}

/**********************************************************************/

void CPasswordListDlg::OnButtonDeletePassword() 
{
	int nIndex = m_listPasswords.GetCurSel();

	if ((nIndex >= 0) && (nIndex < 20))
	{
		if ( Prompter.YesNo ( "Are you sure you wish to remove this user" ) == IDYES )
		{
			m_pPasswords->SetUserDefault(nIndex + 1);
			FillListBox();
		}
	}
}

/**********************************************************************/

void CPasswordListDlg::OnButtonClonePassword()
{
	int nSourceIdx = m_listPasswords.GetCurSel() + 1;
	if ( ( nSourceIdx < 1 ) || ( nSourceIdx >= 20 ) )
	{
		Prompter.Error ( "You must select a password to clone\nbefore you can use this option." );
		return;
	}

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayEntityInfo;
	for ( int n = 1; n <= 20; n++ )
	{
		if ( n != nSourceIdx )
		{
			CSelectMultipleEntityInfo info;
			info.m_nPasswordIdx = n;
			arrayEntityInfo.Add( info );
		}
	}

	CSelectMultiplePasswordDlg dlg ( m_pPasswords, arrayEntityInfo, nSourceIdx, this );

	if ( dlg.DoModal() == IDOK )
	{
		bool bCloned = FALSE;
		for ( int nIndex = 0; nIndex < arrayEntityInfo.GetSize(); nIndex++ )
		{
			if ( TRUE == arrayEntityInfo[nIndex].m_bSelected  )
			{
				int nDestIdx = arrayEntityInfo[nIndex].m_nPasswordIdx;

				m_pPasswords->CopyOptions(nSourceIdx, nDestIdx);

				CString strUserName = m_pPasswords->GetUserName(nDestIdx);
				::TrimSpacesFromString(strUserName, FALSE);

				if ((strUserName == "") || (strUserName.Left(7) == "Copy of"))
				{
					strUserName.Format("Copy of password %2.2d", nSourceIdx);
					m_pPasswords->SetUserName(nDestIdx, strUserName);
				}

				bCloned = TRUE;
			}
		}

		if ( TRUE == bCloned )
		{
			FillListBox();
			m_listPasswords.SetCurSel ( nSourceIdx - 1 );
			Prompter.Info ( "The selected password settings\nhave been cloned as requested." );
		}
	}
}

/**********************************************************************/

void CPasswordListDlg::FillListBox()
{
	m_listPasswords.DeleteAllItems();

	for (int nIndex = 1; nIndex <= 20; nIndex++)
	{
		CString strNum;
		strNum.Format("%2.2d", nIndex);

		m_listPasswords.InsertItem(nIndex - 1, "");
		m_listPasswords.SetItem(nIndex - 1, 0, LVIF_TEXT, strNum, 0, 0, 0, NULL);
		m_listPasswords.SetItem(nIndex - 1, 1, LVIF_TEXT, m_pPasswords->GetUserName(nIndex), 0, 0, 0, NULL);
		m_listPasswords.SetItem(nIndex - 1, 2, LVIF_TEXT, m_pPasswords->GetPassword(nIndex), 0, 0, 0, NULL);
	}
}

/**********************************************************************/

void CPasswordListDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON_SETTINGS );
	*pResult = 0;
}

/**********************************************************************/

void CPasswordListDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CPasswordListDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		CString strMasterPassword = GetEditBoxText(m_editMasterPassword);
		::TrimSpacesFromString(strMasterPassword, FALSE);
		m_pPasswords->SetMasterPassword(strMasterPassword);
		EndDialog(IDOK);
	}
}

/**********************************************************************/

void CPasswordListDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

/**********************************************************************/
