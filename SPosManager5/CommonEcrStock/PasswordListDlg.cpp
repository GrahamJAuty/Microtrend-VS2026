/**********************************************************************/
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "PropertySheetPassword.h"
#include "RepSpawn.h"
#include "SelectMultiplePasswordDlg.h"
#include "PasswordListDlg.h"
/**********************************************************************/

CPasswordListDlg::CPasswordListDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CPasswordListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordListDlg)
	m_bLogPasswords = FALSE;
	//}}AFX_DATA_INIT
	m_strMasterPassword = PasswordArray.GetMasterPassword();
	m_bLogExists = ::FileExists ( Super.UserLog() );
}

/**********************************************************************/

void CPasswordListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordListDlg)
	DDX_Control(pDX, IDC_BUTTON_DELETELOG, m_buttonDeleteLog);
	DDX_Control(pDX, IDC_BUTTON_LOG, m_buttonLog);
	DDX_Control(pDX, IDC_EDIT_PWMASTER, m_editMasterPassword);
	DDX_Check(pDX, IDC_CHECK_LOG, m_bLogPasswords);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PWMASTER, m_strMasterPassword);
	DDV_MaxChars(pDX, m_strMasterPassword, Super.MaxPasswordLen() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPasswordListDlg, CDialog)
	//{{AFX_MSG_MAP(CPasswordListDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonSettings)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonLog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)
	ON_BN_CLICKED(IDC_BUTTON_DELETELOG, OnButtonDeleteLog)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, OnButtonClone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPasswordListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listPasswords.SubclassDlgItem ( IDC_LIST, this );
	m_listPasswords.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listPasswords.InsertColumn ( 0, "No", LVCFMT_LEFT, 30 );
	m_listPasswords.InsertColumn ( 1, "User Name", LVCFMT_LEFT, 180 );
	m_listPasswords.InsertColumn ( 2, "Password", LVCFMT_LEFT, 180 );
	FillListBox();

	if ( PasswordArray.GetSize() > 1 )
		m_listPasswords.SetCurSel(0);

	m_buttonLog.EnableWindow ( m_bLogPasswords && m_bLogExists );
	m_buttonDeleteLog.EnableWindow ( m_bLogPasswords && m_bLogExists);
	m_buttonDeleteLog.ShowWindow( SW_HIDE );

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

void CPasswordListDlg::OnButtonLog() 
{
	CFilenameUpdater FnUp ( SysFiles::UserLogPrm );

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.UserLog(), FnUp.GetFilenameToUse(), "", this );	
}

/**********************************************************************/

void CPasswordListDlg::OnButtonDeleteLog() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to\ndelete the password log" ) == IDYES )
	{
		CFileRemover FileRemover ( Super.UserLogOld() );
		rename ( Super.UserLog(), Super.UserLogOld() );
		m_bLogExists = FALSE;
		m_buttonLog.EnableWindow ( FALSE );
		m_buttonDeleteLog.EnableWindow ( FALSE );
	}
}

/**********************************************************************/

void CPasswordListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonSettings();
	*pResult = 0;
}

/**********************************************************************/

void CPasswordListDlg::OnButtonSettings() 
{
	int nIndex = m_listPasswords.GetCurSel();

	if ( nIndex >= 0 )
	{
		CPropertySheetPassword propSheet ( IDC_SPIN, nIndex + 1, this);
		
		if ( propSheet.DoModal() == IDOK )
			propSheet.SaveRecord();
		
		FillListBox();
		m_listPasswords.SetCurSel( propSheet.GetIndex() - 1 );
	}
}

/**********************************************************************/

void CPasswordListDlg::OnButtonDelete() 
{
	int nIndex = m_listPasswords.GetCurSel();

	if ( nIndex >= 0 )
	{
		CString strMsg = "Are you sure you wish\n";
		strMsg += "to remove this user";

		if ( Prompter.YesNo ( strMsg ) == IDYES )
		{
			CPasswordCSVRecord PasswordRecord;
			PasswordArray.SetAt ( nIndex + 1, PasswordRecord );
			FillListBox();
		}
	}
}

/**********************************************************************/

void CPasswordListDlg::FillListBox()
{
	m_listPasswords.DeleteAllItems();

	for ( int nIndex = 1; nIndex < PasswordArray.GetSize(); nIndex++ )
	{
		CString strNum;
		strNum.Format( "%2.2d", nIndex );

		m_listPasswords.InsertItem( nIndex - 1, "" );
		m_listPasswords.SetItem( nIndex - 1, 0, LVIF_TEXT, strNum, 0, 0, 0, NULL );
		m_listPasswords.SetItem( nIndex - 1, 1, LVIF_TEXT, PasswordArray.GetUserName( nIndex ), 0, 0, 0, NULL );
		m_listPasswords.SetItem( nIndex - 1, 2, LVIF_TEXT, PasswordArray.GetPassword( nIndex ), 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CPasswordListDlg::OnCheckLog() 
{
	UpdateData( TRUE, FALSE );
	m_buttonLog.EnableWindow ( m_bLogPasswords && m_bLogExists );	
	m_buttonDeleteLog.EnableWindow ( m_bLogPasswords && m_bLogExists );
}

/**********************************************************************/

void CPasswordListDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strMasterPassword, FALSE );
		PasswordArray.SetMasterPassword ( m_strMasterPassword );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CPasswordListDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
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

void CPasswordListDlg::OnButtonClone() 
{
	int nSourceIdx = m_listPasswords.GetCurSel() + 1;
	if ( ( nSourceIdx < 1 ) || ( nSourceIdx >= PasswordArray.GetSize() ) )
	{
		Prompter.Error ( "You must select a password to clone\nbefore you can use this option." );
		return;
	}

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayEntityInfo;
	for ( int n = 1; n < PasswordArray.GetSize(); n++ )
	{
		if ( n != nSourceIdx )
		{
			CSelectMultipleEntityInfo info;
			info.m_nPasswordIdx = n;
			arrayEntityInfo.Add( info );
		}
	}

	CSelectMultiplePasswordDlg dlg ( arrayEntityInfo, nSourceIdx, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		bool bCloned = FALSE;
		for ( int nIndex = 0; nIndex < arrayEntityInfo.GetSize(); nIndex++ )
		{
			if ( TRUE == arrayEntityInfo[nIndex].m_bSelected  )
			{
				int nDestIdx = arrayEntityInfo[nIndex].m_nPasswordIdx;

				CPasswordCSVRecord PasswordSource, PasswordDest;
				PasswordArray.GetAt ( nSourceIdx, PasswordSource );
				PasswordArray.GetAt ( nDestIdx, PasswordDest );

				CString strUserName = PasswordDest.GetUserName();
				::TrimSpaces ( strUserName, FALSE );
				if ( ( strUserName == "" ) || ( strUserName.Left(7) == "Copy of" ) ) 
					strUserName.Format ( "Copy of password %2.2d", nSourceIdx );

				PasswordSource.SetUserName ( strUserName );
				PasswordSource.SetPassword ( PasswordDest.GetPassword() );
				PasswordArray.SetAt ( nDestIdx, PasswordSource );
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
