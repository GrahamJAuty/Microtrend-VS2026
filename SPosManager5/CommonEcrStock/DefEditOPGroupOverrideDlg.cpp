/**********************************************************************/
#include "DefEditOPGroupOverrideDlg.h"
/**********************************************************************/

CDefEditOPGroupOverrideDlg::CDefEditOPGroupOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditOPGroupOverrideDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditOPGroupOverrideDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nConLevel = ( NODE_DATABASE == nConLevel ) ? NODE_DATABASE : NODE_LOCATION; 
	m_pListDataDlg = NULL;
	
	m_IDLocPrinter[0] = IDC_CHECK_LOCPRINTER1;
	m_IDLocPrinter[1] = IDC_CHECK_LOCPRINTER2;
	m_IDLocPrinter[2] = IDC_CHECK_LOCPRINTER3;
	m_IDLocPrinter[3] = IDC_CHECK_LOCPRINTER4;
	m_IDLocPrinter[4] = IDC_CHECK_LOCPRINTER5;
	m_IDLocPrinter[5] = IDC_CHECK_LOCPRINTER6;
	m_IDLocPrinter[6] = IDC_CHECK_LOCPRINTER7;
	m_IDLocPrinter[7] = IDC_CHECK_LOCPRINTER8;
	m_IDLocPrinter[8] = IDC_CHECK_LOCPRINTER9;
	m_IDLocPrinter[9] = IDC_CHECK_LOCPRINTER10;
	m_IDLocPrinter[10] = IDC_CHECK_LOCPRINTER11;
	m_IDLocPrinter[11] = IDC_CHECK_LOCPRINTER12;
	m_IDLocPrinter[12] = IDC_CHECK_LOCPRINTER13;
	m_IDLocPrinter[13] = IDC_CHECK_LOCPRINTER14;
	m_IDLocPrinter[14] = IDC_CHECK_LOCPRINTER15;
	m_IDLocPrinter[15] = IDC_CHECK_LOCPRINTER16;

	m_IDSysPrinter[0] = IDC_CHECK_SYSPRINTER1;
	m_IDSysPrinter[1] = IDC_CHECK_SYSPRINTER2;
	m_IDSysPrinter[2] = IDC_CHECK_SYSPRINTER3;
	m_IDSysPrinter[3] = IDC_CHECK_SYSPRINTER4;
	m_IDSysPrinter[4] = IDC_CHECK_SYSPRINTER5;
	m_IDSysPrinter[5] = IDC_CHECK_SYSPRINTER6;
	m_IDSysPrinter[6] = IDC_CHECK_SYSPRINTER7;
	m_IDSysPrinter[7] = IDC_CHECK_SYSPRINTER8;
	m_IDSysPrinter[8] = IDC_CHECK_SYSPRINTER9;
	m_IDSysPrinter[9] = IDC_CHECK_SYSPRINTER10;
	m_IDSysPrinter[10] = IDC_CHECK_SYSPRINTER11;
	m_IDSysPrinter[11] = IDC_CHECK_SYSPRINTER12;
	m_IDSysPrinter[12] = IDC_CHECK_SYSPRINTER13;
	m_IDSysPrinter[13] = IDC_CHECK_SYSPRINTER14;
	m_IDSysPrinter[14] = IDC_CHECK_SYSPRINTER15;
	m_IDSysPrinter[15] = IDC_CHECK_SYSPRINTER16;

	m_IDStaticPrinter[0] = IDC_STATIC_SYSPRINTER1;
	m_IDStaticPrinter[1] = IDC_STATIC_SYSPRINTER2;
	m_IDStaticPrinter[2] = IDC_STATIC_SYSPRINTER3;
	m_IDStaticPrinter[3] = IDC_STATIC_SYSPRINTER4;
	m_IDStaticPrinter[4] = IDC_STATIC_SYSPRINTER5;
	m_IDStaticPrinter[5] = IDC_STATIC_SYSPRINTER6;
	m_IDStaticPrinter[6] = IDC_STATIC_SYSPRINTER7;
	m_IDStaticPrinter[7] = IDC_STATIC_SYSPRINTER8;
	m_IDStaticPrinter[8] = IDC_STATIC_SYSPRINTER9;
	m_IDStaticPrinter[9] = IDC_STATIC_SYSPRINTER10;
	m_IDStaticPrinter[10] = IDC_STATIC_SYSPRINTER11;
	m_IDStaticPrinter[11] = IDC_STATIC_SYSPRINTER12;
	m_IDStaticPrinter[12] = IDC_STATIC_SYSPRINTER13;
	m_IDStaticPrinter[13] = IDC_STATIC_SYSPRINTER14;
	m_IDStaticPrinter[14] = IDC_STATIC_SYSPRINTER15;
	m_IDStaticPrinter[15] = IDC_STATIC_SYSPRINTER16;
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditOPGroupOverrideDlg)
	DDX_Control(pDX, IDC_STATIC_SETTINGS, m_staticSettings);
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride);
	DDX_Control(pDX, IDC_STATIC_ORIGIN, m_staticOrigin);
	//}}AFX_DATA_MAP
	
	for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
	{
		DDX_Control(pDX, m_IDLocPrinter[n], m_checkLocPrinter[n]);
		DDX_Control(pDX, m_IDSysPrinter[n], m_checkSysPrinter[n]);
		DDX_Control(pDX, m_IDStaticPrinter[n], m_staticSysPrinter[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditOPGroupOverrideDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditOPGroupOverrideDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_CHECK_OVERRIDE, OnToggleOverride)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditOPGroupOverrideDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (NODE_DATABASE == m_nConLevel)
	{
		m_staticSettings.SetWindowText("Database Settings");
	}
	else
	{
		m_staticSettings.SetWindowText("Location Settings");
	}

	for (int n = 0; n < 16; n++)
	{
		COPNameMiniInfo infoMini;
		infoMini.m_bDefer = TRUE;

		if (NODE_LOCATION == m_nConLevel)
		{
			DataManagerNonDb.OPNameLocation.GetOPName(n, infoMini);
		}

		if (TRUE == infoMini.m_bDefer)
		{
			DataManagerNonDb.OPNameDatabase.GetOPName(n, infoMini);
		}

		if (TRUE == infoMini.m_bDefer)
		{
			DataManagerNonDb.OPNameSystem.GetOPName(n, infoMini);
		}

		CString strName = infoMini.m_strName;
		if (strName == "")
		{
			strName.Format("Printer %d", infoMini.m_nPrinterNo);
		}

		m_checkLocPrinter[n].SetWindowText(strName);
		m_staticSysPrinter[n].SetWindowText(strName);
	}

	GetData();
	return FALSE;
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDefEditOPGroupOverrideDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::SpinPrevious()
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

void CDefEditOPGroupOverrideDlg::SpinNext()
{
	if (UpdateData(TRUE) == TRUE)
	{
		if (m_nIndex < DataManagerNonDb.OPGroupSystem.GetOPGroupCount() - 1)
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

void CDefEditOPGroupOverrideDlg::GetData()
{
	CString strTitle;

	COPGroupMiniInfo infoMini;

	if ( NODE_DATABASE == m_nConLevel )
	{
		m_staticOrigin.SetWindowText("(From system)");
		DataManagerNonDb.OPGroupDatabase.GetOPGroup( m_nIndex, infoMini );
		DataManagerNonDb.OPGroupSystem.GetOPGroup( m_nIndex, m_infoDefault );
	}
	else
	{
		DataManagerNonDb.OPGroupLocation.GetOPGroup( m_nIndex, infoMini );
		DataManagerNonDb.OPGroupDatabase.GetOPGroup( m_nIndex, m_infoDefault );
		
		if ( TRUE == m_infoDefault.m_bDefer )
		{
			m_staticOrigin.SetWindowText( "(From system)" );
			DataManagerNonDb.OPGroupSystem.GetOPGroup( m_nIndex, m_infoDefault );
		}
		else
		{
			m_staticOrigin.SetWindowText( "(From database)" );
		}
	}
	
	{
		strTitle.Format ( "G%2.2d %s", infoMini.m_nGroupNo, (const char*) infoMini.m_strName );
		SetWindowText ( strTitle );
	}

	{
		COPGroupInfo OPGroupInfo;
		OPGroupInfo.SetEnableFlagsSystem( m_infoDefault.m_nFlags );

		for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
		{
			m_checkSysPrinter[n].SetCheck(OPGroupInfo.GetEnableSystem(n));
		}
	}

	if ( FALSE == infoMini.m_bDefer )
	{
		m_checkOverride.SetCheck( TRUE );

		COPGroupInfo OPGroupInfo;
		OPGroupInfo.SetEnableFlagsSystem( infoMini.m_nFlags );

		for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
		{
			m_checkLocPrinter[n].EnableWindow(TRUE);
			m_checkLocPrinter[n].SetCheck( OPGroupInfo.GetEnableSystem(n) );
		}
	}
	else
	{
		m_checkOverride.SetCheck( FALSE );
		CopyDefaultSettings();
		m_checkOverride.SetFocus();
	}
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::CopyDefaultSettings()
{
	COPGroupInfo OPGroupInfo;
	OPGroupInfo.SetEnableFlagsSystem( m_infoDefault.m_nFlags );

	for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
	{
		m_checkLocPrinter[n].EnableWindow(FALSE);
		m_checkLocPrinter[n].SetCheck( OPGroupInfo.GetEnableSystem(n) );
	}
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::SaveData()
{
	COPGroupMiniInfo infoMini;

	if (NODE_DATABASE == m_nConLevel)
	{
		DataManagerNonDb.OPGroupDatabase.GetOPGroup(m_nIndex, infoMini);
	}
	else
	{
		DataManagerNonDb.OPGroupLocation.GetOPGroup(m_nIndex, infoMini);
	}

	if (m_checkOverride.GetCheck() != 0)
	{
		infoMini.m_bDefer = FALSE;

		COPGroupInfo OPGroupInfo;
		OPGroupInfo.SetEnableFlagsLocation(0);

		for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
		{
			if (m_checkLocPrinter[n].GetCheck() != 0)
			{
				OPGroupInfo.SetEnableLocation(n, TRUE);
			}
		}

		infoMini.m_nFlags = OPGroupInfo.GetEnableFlagsLocation();
	}
	else
	{
		infoMini.m_bDefer = TRUE;
		infoMini.m_nFlags = 0;
	}

	if (NODE_DATABASE == m_nConLevel)
	{
		DataManagerNonDb.OPGroupDatabase.UpdateOPGroup(m_nIndex, infoMini);
	}
	else
	{
		DataManagerNonDb.OPGroupLocation.UpdateOPGroup(m_nIndex, infoMini);
	}

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::OnToggleOverride()
{
	if (m_checkOverride.GetCheck() != 0)
	{
		for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
		{
			m_checkLocPrinter[n].EnableWindow(TRUE);
		}
	}
	else
	{
		CopyDefaultSettings();
	}
}

/**********************************************************************/

void CDefEditOPGroupOverrideDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
