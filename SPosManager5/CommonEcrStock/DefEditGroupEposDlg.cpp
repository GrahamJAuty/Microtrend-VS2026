/**********************************************************************/
#include "GroupCSVArrayEpos.h"
#include "SageOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DefEditGroupEposDlg.h"
/**********************************************************************/

CDefEditGroupEposDlg::CDefEditGroupEposDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditGroupEposDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_pListDataDlg = NULL;

	//{{AFX_DATA_INIT(CDefEditGroupEposDlg)
	m_strEposText = _T("");
	m_strRepText = _T("");
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
}

/**********************************************************************/

void CDefEditGroupEposDlg::DoDataExchange(CDataExchange* pDX)
{
	int nMaxLen = 32;
	if ( nMaxLen < EposGroup::RepText.Max ) nMaxLen = EposGroup::RepText.Max;
	if ( nMaxLen < EposGroup::EposText.Max ) nMaxLen = EposGroup::EposText.Max;

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditGroupEposDlg)
	DDX_Control(pDX, IDC_EDIT_EPOSTEXT, m_editEposText);
	DDX_Control(pDX, IDC_EDIT_REPTEXT, m_editRepText);
	DDX_Control(pDX, IDC_STATIC_SAGE, m_staticSageNominalCode);
	DDX_Control(pDX, IDC_CHECK_SPTBOOK, m_checkSptBook);
	DDX_Control(pDX, IDC_STATIC_SPTBOOK, m_staticSptBook);
	DDX_Control(pDX, IDC_SPIN, m_buttonSpin);
	DDX_Control(pDX, IDC_DEFAULT_BUTTON, m_buttonDefault);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_EPOSTEXT, m_strEposText);
	DDV_MaxChars(pDX, m_strEposText, nMaxLen);
	DDX_Text(pDX, IDC_EDIT_REPTEXT, m_strRepText);
	DDV_MaxChars(pDX, m_strRepText, nMaxLen);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditGroupEposDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditGroupEposDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED( IDC_CHECK_SPTBOOK, OnTickSportsBooker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditGroupEposDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SubclassEdit( IDC_EDIT_SAGE, SS_NUM, SageOptions.GetNominalCodeLength(), "%d" );
	
	GetData();

	int nDlgShrink = 0;
	
	if ( SysInfo.IsSageExportSystem() == FALSE )
	{
		m_staticSageNominalCode.ShowWindow( SW_HIDE );
		ShowAndEnableWindow( GetEditNominalCode(), FALSE );
		MoveControl( &m_checkSptBook, 7, 48 );
		MoveControl( &m_staticSptBook, 18, 49 );
		nDlgShrink += 20;
	}

	if ( SysInfo.IsSportsBookerSystem() == FALSE )
	{
		m_staticSptBook.ShowWindow( SW_HIDE );
		ShowAndEnableWindow( &m_checkSptBook, FALSE );
		nDlgShrink += 20;
	}

	if ( nDlgShrink != 0 )
	{
		MoveControl( &m_buttonSpin, 7, 123 - nDlgShrink );
		MoveControl( &m_buttonDefault, 94, 123 - nDlgShrink );
		MoveControl( &m_buttonOK, 176, 107 - nDlgShrink );
		MoveControl( &m_buttonCancel, 176, 123 - nDlgShrink );
		ResizeDialog( 233, 144 - nDlgShrink );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditGroupEposDlg::GetEditNominalCode(){ return GetEdit( IDC_EDIT_SAGE ); }
/**********************************************************************/

void CDefEditGroupEposDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditGroupEposDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditGroupEposDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex <= 1 )
			return;

		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nIndex );
	}
}

/**********************************************************************/

void CDefEditGroupEposDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManager.EposGroup.GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditGroupEposDlg::GetData()
{
	CString strTitle;

	CGroupCSVRecordEpos GroupRecord;
	DataManager.EposGroup.GetAt ( m_nIndex, GroupRecord );

	if ( GroupRecord.GetSageNominalCode() == 0 )
		GetEditNominalCode() -> SetWindowText( "" );
	else
	{
		CString str;
		int nMaxLen = SageOptions.GetNominalCodeLength();
		str.Format( "%*.*d", nMaxLen, nMaxLen, GroupRecord.GetSageNominalCode() );
		GetEditNominalCode() -> SetWindowText ( str );
	}
	
	m_editEposText.SetWindowText ( GroupRecord.GetEposText() );
	m_editRepText.SetWindowText ( GroupRecord.GetRepText() );
	strTitle.Format ( "G%2.2d %s", GroupRecord.GetGroupNo(), GroupRecord.GetDefaultText() );
	SetWindowText ( strTitle );

	m_editEposText.SetSel ( 0, -1 );
	m_editEposText.SetFocus();

	m_checkSptBook.SetCheck( GroupRecord.GetGroupNo() == DataManager.EposGroup.GetSportsBookerGroupNo() );
	OnTickSportsBooker();
}

/**********************************************************************/

void CDefEditGroupEposDlg::OnTickSportsBooker()
{
	if ( SysInfo.IsSportsBookerSystem() == TRUE )
	{
		CString strText = "Sports Booker default group";

		if ( m_checkSptBook.GetCheck() != 0 )
		{
			m_checkSptBook.EnableWindow( FALSE );
			m_staticSptBook.SetWindowText( strText );
		}
		else
		{
			m_checkSptBook.EnableWindow( TRUE );
			
			int nSptBookGroupNo = DataManager.EposGroup.GetSportsBookerGroupNo();

			if ( 0 != nSptBookGroupNo )
			{
				int nGroupIdx;
				if ( DataManager.EposGroup.FindGroupByNumber( nSptBookGroupNo, nGroupIdx ) == FALSE )
				{
					nSptBookGroupNo = 0;
					DataManager.EposGroup.SetSportsBookerGroupNo(0);
				}
			}

			if ( 0 == nSptBookGroupNo )
				m_staticSptBook.SetWindowText( strText + " (currently not set)" );
			else
			{
				CString strExtra;
				strExtra.Format ( " (currently group %d)", nSptBookGroupNo );
				m_staticSptBook.SetWindowText( strText + strExtra );
			}
		}
	}
}

/**********************************************************************/

void CDefEditGroupEposDlg::SaveData()
{
	CGroupCSVRecordEpos GroupRecord;
	DataManager.EposGroup.GetAt ( m_nIndex, GroupRecord );

	CString str;
	GetEditNominalCode() -> GetWindowText( str );
	
	GroupRecord.SetRepText ( m_strRepText );
	GroupRecord.SetEposText ( m_strEposText );
	GroupRecord.SetSageNominalCode ( atoi( str ) );

	if ( m_checkSptBook.GetCheck() != 0 )
		DataManager.EposGroup.SetSportsBookerGroupNo( GroupRecord.GetGroupNo() );
	
	DataManager.EposGroup.SetAt ( m_nIndex, GroupRecord );
}

/**********************************************************************/

void CDefEditGroupEposDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
