/**********************************************************************/
#include "DefEditMacroSystemDlg.h"
/**********************************************************************/

CDefEditMacroSystemDlg::CDefEditMacroSystemDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditMacroSystemDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditMacroSystemDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_pMacroHelpDlg = NULL;
	m_pListDataDlg = NULL;
	m_bNoHelpButton = FALSE;
}

/**********************************************************************/

CDefEditMacroSystemDlg::~CDefEditMacroSystemDlg()
{
	if (m_pMacroHelpDlg != NULL)
	{
		delete (m_pMacroHelpDlg);
	}
}

/**********************************************************************/

void CDefEditMacroSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditMacroSystemDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	DDX_Control(pDX, IDC_BUTTON_MACROHELP, m_buttonMacroHelp);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditMacroSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditMacroSystemDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON_MACROHELP, OnButtonMacroHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditMacroSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editName.LimitText( CascadingMacro::MacroName.Max );
	m_editData.LimitText( CascadingMacro::MacroData.Max );

	if (TRUE == m_bNoHelpButton)
	{
		ShowAndEnableWindow(&m_buttonMacroHelp, FALSE);
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditMacroSystemDlg::GetData()
{
	CCascadingMacroCSVRecord Macro;
	DataManagerNonDb.CCMacroSystem.GetAt( m_nIndex, Macro );

	CString strName = Macro.GetLocalMacroName();

	CString strTitle;
	strTitle.Format( "M%3.3d %s", Macro.GetMacroNo(), (const char*) strName );
	SetWindowText( strTitle );

	m_editName.SetWindowText( strName );
	m_editData.SetWindowText( Macro.GetLocalMacroData() );
}

/**********************************************************************/

void CDefEditMacroSystemDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditMacroSystemDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditMacroSystemDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine ( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditMacroSystemDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.CCMacroSystem.GetSize() - 1 )
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

void CDefEditMacroSystemDlg::OnButtonMacroHelp()
{
	if (FALSE == m_bNoHelpButton)
	{
		if (m_pMacroHelpDlg == NULL)
		{
			m_pMacroHelpDlg = new CMacroHelpDlg(&m_pMacroHelpDlg, this);
			m_pMacroHelpDlg->Create(IDD_MACRO_HELP, this);
		}

		m_pMacroHelpDlg->SetFocus();
	}
}

/**********************************************************************/

void CDefEditMacroSystemDlg::SaveData()
{
	CCascadingMacroCSVRecord Macro;
	DataManagerNonDb.CCMacroSystem.GetAt( m_nIndex, Macro );
	
	Macro.SetLocalMacroName( GetEditBoxText( m_editName ) );
	Macro.SetLocalMacroData( GetEditBoxText( m_editData ) );
	
	DataManagerNonDb.CCMacroSystem.SetAt( m_nIndex, Macro );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditMacroSystemDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
