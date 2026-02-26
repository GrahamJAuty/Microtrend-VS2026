/**********************************************************************/
#include "DefEditOPNameOverrideDlg.h"
/**********************************************************************/

CDefEditOPNameOverrideDlg::CDefEditOPNameOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditOPNameOverrideDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditOPNameOverrideDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nConLevel = ( NODE_DATABASE == nConLevel ) ? NODE_DATABASE : NODE_LOCATION; 
	m_pListDataDlg = NULL;
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditOPNameOverrideDlg)
	DDX_Control(pDX, IDC_STATIC_SETTINGS, m_staticSettings);
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride);
	DDX_Control(pDX, IDC_STATIC_ORIGIN, m_staticOrigin);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editLocName);
	DDX_Control(pDX, IDC_EDIT_DEFNAME, m_editSysName);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditOPNameOverrideDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditOPNameOverrideDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_CHECK_OVERRIDE, OnToggleOverride)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditOPNameOverrideDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( NODE_DATABASE == m_nConLevel )
	{
		m_staticSettings.SetWindowText( "Database Settings" );
	}
	else
	{
		m_staticSettings.SetWindowText( "Location Settings" );
	}

	GetData();
	return FALSE;  
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if (pNMUpDown->iDelta > 0)
	{
		SpinPrevious();
	}
	else
	{
		SpinNext();
	}

	*pResult = 0;
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::SpinPrevious()
{
	if (UpdateData(TRUE) == TRUE)
	{
		if (m_nIndex <= 0)
		{
			return;
		}

		SaveData();
		m_nIndex--;
		GetData();

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nIndex);
		}
	}
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if (m_nIndex < DataManagerNonDb.OPNameSystem.GetOPNameCount() - 1)
		{
			SaveData();
			m_nIndex++;
			GetData();

			if (m_pListDataDlg != NULL)
			{
				m_pListDataDlg->SelectLine(m_nIndex);
			}
		}
	}
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::GetData()
{
	CString strTitle;

	COPNameMiniInfo infoMini;

	if ( NODE_DATABASE == m_nConLevel )
	{
		m_staticOrigin.SetWindowText("(From system)");
		DataManagerNonDb.OPNameDatabase.GetOPName( m_nIndex, infoMini );
		DataManagerNonDb.OPNameSystem.GetOPName( m_nIndex, m_infoDefault );
	}
	else
	{
		DataManagerNonDb.OPNameLocation.GetOPName( m_nIndex, infoMini );
		DataManagerNonDb.OPNameDatabase.GetOPName( m_nIndex, m_infoDefault );
		
		if ( TRUE == m_infoDefault.m_bDefer )
		{
			m_staticOrigin.SetWindowText( "(From system)" );
			DataManagerNonDb.OPNameSystem.GetOPName( m_nIndex, m_infoDefault );
		}
		else
		{
			m_staticOrigin.SetWindowText( "(From database)" );
		}
	}
	
	{
		strTitle.Format("Edit Name Order Printer %d",
			infoMini.m_nPrinterNo);

		SetWindowText(strTitle);
	}

	m_editSysName.SetWindowText(m_infoDefault.m_strName);

	if ( FALSE == infoMini.m_bDefer )
	{
		m_checkOverride.SetCheck( TRUE );
		m_editLocName.EnableWindow(TRUE);
		m_editLocName.SetWindowText(infoMini.m_strName);
	}
	else
	{
		m_checkOverride.SetCheck( FALSE );
		CopyDefaultSettings();
		m_checkOverride.SetFocus();
	}
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::CopyDefaultSettings()
{
	m_editLocName.SetWindowText(m_infoDefault.m_strName);
	m_editLocName.EnableWindow(FALSE);
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::SaveData()
{
	COPNameMiniInfo infoMini;

	if (NODE_DATABASE == m_nConLevel)
	{
		DataManagerNonDb.OPNameDatabase.GetOPName(m_nIndex, infoMini);
	}
	else
	{
		DataManagerNonDb.OPNameLocation.GetOPName(m_nIndex, infoMini);
	}

	if ( m_checkOverride.GetCheck() != 0 )
	{
		infoMini.m_bDefer = FALSE;
		infoMini.m_strName = GetEditBoxText(m_editLocName);
	}
	else
	{
		infoMini.m_bDefer = TRUE;
		infoMini.m_strName = "";
	}
	
	if (NODE_DATABASE == m_nConLevel)
	{
		DataManagerNonDb.OPNameDatabase.UpdateOPName(m_nIndex, infoMini);
	}
	else
	{
		DataManagerNonDb.OPNameLocation.UpdateOPName(m_nIndex, infoMini);
	}

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditOPNameOverrideDlg::OnToggleOverride() 
{
	if ( m_checkOverride.GetCheck() != 0 )
	{
		m_editLocName.EnableWindow(TRUE);
	}
	else
	{
		CopyDefaultSettings();
	}
}

/**********************************************************************/

void CDefEditOPNameOverrideDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
