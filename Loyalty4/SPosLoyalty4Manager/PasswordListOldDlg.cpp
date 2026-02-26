/**********************************************************************/
//#include "FileRemover.h"
//#include "RepSpawn.h"
#include "SelectMultiplePasswordDlg.h"
#include "SetupPasswordOptionsDlg.h"
/**********************************************************************/
#include "PasswordListDlg.h"
/**********************************************************************/

CPasswordListDlg::CPasswordListDlg( CPasswordData* pPasswords, bool bHaveReader, CWnd* pParent /*=NULL*/)
	: CSSDialog( CPasswordListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordListDlg)
	//}}AFX_DATA_INIT
	m_pPasswords = pPasswords;
	m_bHaveReader = bHaveReader;
	/*
	m_bLogPasswords = FALSE;
	m_bLogExists = ::FileExists ( Super.UserLog() );
	*/
}

/**********************************************************************/

void CPasswordListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordListDlg)
	DDX_Control(pDX, IDC_EDIT_PWMASTER, m_editMasterPassword);
	DDX_Control(pDX, IDC_STATIC_CARD, m_staticCard);
	DDX_Control(pDX, IDC_EDIT_CARD, m_editCard);
	DDX_Control(pDX, IDC_CHECK_LOG, m_checkLogPasswords);
	DDX_Control(pDX, IDC_BUTTON_LOG, m_buttonShowLog);
	DDX_Control(pDX, IDC_BUTTON_DELETELOG, m_buttonDeleteLog);
	DDX_Control(pDX, IDC_BUTTON_SETTINGS, m_buttonEditPassword);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDeletePassword);
	DDX_Control(pDX, IDC_BUTTON_CLONE, m_buttonClonePassword);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPasswordListDlg, CSSDialog)
	//{{AFX_MSG_MAP(CPasswordListDlg)
	
	ON_BN_CLICKED(IDC_CHECK_LOG, OnToggleLog)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonShowLog)
	ON_BN_CLICKED(IDC_BUTTON_DELETELOG, OnButtonDeleteLog)
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

BOOL CPasswordListDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();

	m_listPasswords.SubclassDlgItem(IDC_LIST, this);
	m_listPasswords.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listPasswords.InsertColumn(0, "No", LVCFMT_LEFT, 40);
	m_listPasswords.InsertColumn(1, "Password", LVCFMT_LEFT, 220);

	if (TRUE == m_bHaveReader)
	{
		m_listPasswords.InsertColumn(2, "Card No", LVCFMT_LEFT, 220);
	}

	m_editMasterPassword.LimitText(MAX_PASSWORD_LEN);
	m_editMasterPassword.SetWindowText(m_pPasswords->GetPassword(0));

	m_editCard.LimitText(MAX_PASSWORD_CARDNO_LEN);
	m_editCard.SetWindowText(m_pPasswords->GetCardNo(0));

	FillListBox();

	m_listPasswords.SetCurSel(0);

	/*
	m_buttonLog.EnableWindow ( m_bLogPasswords && m_bLogExists );
	m_buttonDeleteLog.EnableWindow ( m_bLogPasswords && m_bLogExists);
	m_buttonDeleteLog.ShowWindow( SW_HIDE );
	*/

	ShowAndEnableWindow(&m_checkLogPasswords, FALSE);
	ShowAndEnableWindow(&m_buttonShowLog, FALSE);
	ShowAndEnableWindow(&m_buttonDeleteLog, FALSE);

	if (FALSE == m_bHaveReader)
	{
		ShowAndEnableWindow(&m_staticCard, FALSE);
		ShowAndEnableWindow(&m_editCard, FALSE);
		MoveControl(&m_buttonEditPassword, 394, 7);
		MoveControl(&m_buttonDeletePassword, 394, 44);
		MoveControl(&m_buttonClonePassword, 394, 162);
		MoveControl(&m_buttonOK, 394, 320);
		MoveControl(&m_buttonCancel, 394, 336);
		MoveControl(&m_listPasswords, 189, 9, 190, 340);
		ResizeDialog(451,358);
	}

	return TRUE;  
}

/**********************************************************************/

void CPasswordListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listPasswords.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CPasswordListDlg::OnToggleLog()
{
	/*
	UpdateData( TRUE, FALSE );
	m_buttonLog.EnableWindow ( m_bLogPasswords && m_bLogExists );
	m_buttonDeleteLog.EnableWindow ( m_bLogPasswords && m_bLogExists );
	*/
}

/**********************************************************************/

void CPasswordListDlg::OnButtonShowLog() 
{
	/*
	CFilenameUpdater FnUp ( SysFiles::UserLogPrm );

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.UserLog(), FnUp.GetFilenameToUse(), "", this );	
	*/
}

/**********************************************************************/

void CPasswordListDlg::OnButtonDeleteLog() 
{
	/*
	if ( Prompter.YesNo ( "Are you sure you wish to\ndelete the password log" ) == IDYES )
	{
		CFileRemover FileRemover ( Super.UserLogOld() );
		rename ( Super.UserLog(), Super.UserLogOld() );
		m_bLogExists = FALSE;
		m_buttonLog.EnableWindow ( FALSE );
		m_buttonDeleteLog.EnableWindow ( FALSE );
	}
	*/
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
		CSetupPasswordOptionsDlg dlg(nIndex + 1, m_pPasswords, m_bHaveReader, this);
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

				/*
				CString strUserName = PasswordDest.GetUserName();
				::TrimSpaces ( strUserName, FALSE );
				if ( ( strUserName == "" ) || ( strUserName.Left(7) == "Copy of" ) )
					strUserName.Format ( "Copy of password %2.2d", nSourceIdx );
				*/

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

	for ( int nIndex = 1; nIndex <= 20; nIndex++ )
	{
		CString strNum;
		strNum.Format( "%2.2d", nIndex );

		m_listPasswords.InsertItem( nIndex - 1, "" );
		m_listPasswords.SetItem( nIndex - 1, 0, LVIF_TEXT, strNum, 0, 0, 0, NULL );
		m_listPasswords.SetItem( nIndex - 1, 1, LVIF_TEXT, m_pPasswords->GetPassword(nIndex), 0, 0, 0, NULL );

		if (TRUE == m_bHaveReader)
		{
			m_listPasswords.SetItem(nIndex - 1, 2, LVIF_TEXT, m_pPasswords->GetCardNo(nIndex), 0, 0, 0, NULL);
		}
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
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CString strMasterPassword = GetEditBoxText(m_editMasterPassword);
		::TrimSpacesFromString ( strMasterPassword, FALSE );
		m_pPasswords -> SetMasterPassword ( strMasterPassword );

		if (TRUE == m_bHaveReader)
		{
			CString strCardNo = GetEditBoxText(m_editCard);
			::TrimSpacesFromString(strCardNo, FALSE);
			m_pPasswords->SetMasterCardNo(strCardNo);

		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CPasswordListDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

/**********************************************************************/
