/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "DefEditOPGroupDlg.h"
/**********************************************************************/

CDefEditOPGroupDlg::CDefEditOPGroupDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditOPGroupDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditOPGroupDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_pListDataDlg = NULL;
	m_IDPrinter[0] = IDC_CHECK_PRINTER1;
	m_IDPrinter[1] = IDC_CHECK_PRINTER2;
	m_IDPrinter[2] = IDC_CHECK_PRINTER3;
	m_IDPrinter[3] = IDC_CHECK_PRINTER4;
	m_IDPrinter[4] = IDC_CHECK_PRINTER5;
	m_IDPrinter[5] = IDC_CHECK_PRINTER6;
	m_IDPrinter[6] = IDC_CHECK_PRINTER7;
	m_IDPrinter[7] = IDC_CHECK_PRINTER8;
	m_IDPrinter[8] = IDC_CHECK_PRINTER9;
	m_IDPrinter[9] = IDC_CHECK_PRINTER10;
	m_IDPrinter[10] = IDC_CHECK_PRINTER11;
	m_IDPrinter[11] = IDC_CHECK_PRINTER12;
	m_IDPrinter[12] = IDC_CHECK_PRINTER13;
	m_IDPrinter[13] = IDC_CHECK_PRINTER14;
	m_IDPrinter[14] = IDC_CHECK_PRINTER15;
	m_IDPrinter[15] = IDC_CHECK_PRINTER16;
}

/**********************************************************************/

void CDefEditOPGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditOPGroupDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP

	for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
	{
		DDX_Control(pDX, m_IDPrinter[n], m_checkPrinter[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditOPGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditOPGroupDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditOPGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetData();

	for (int n = 0; n < 16; n++)
	{
		COPNameMiniInfo info;
		DataManagerNonDb.OPNameSystem.GetOPName(n, info);
		CString strName = info.m_strName;

		if (strName == "")
		{
			strName.Format("Printer %d", info.m_nPrinterNo);
		}

		m_checkPrinter[n].SetWindowText(strName);
	}

	return FALSE;  
}

/**********************************************************************/

void CDefEditOPGroupDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult)
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

void CDefEditOPGroupDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditOPGroupDlg::SpinPrevious()
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

void CDefEditOPGroupDlg::SpinNext()
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

void CDefEditOPGroupDlg::GetData()
{
	CString strTitle;

	COPGroupMiniInfo infoMini;
	DataManagerNonDb.OPGroupSystem.GetOPGroup( m_nIndex, infoMini );
	
	strTitle.Format ( "G%2.2d %s", infoMini.m_nGroupNo, (const char*) infoMini.m_strName );
	SetWindowText ( strTitle );

	m_editName.SetWindowText( infoMini.m_strName );
	m_editName.LimitText( MAX_LENGTH_OPGROUP_NAME );

	COPGroupInfo OPGroupInfo;
	OPGroupInfo.SetEnableFlagsSystem( infoMini.m_nFlags );

	for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
	{
		m_checkPrinter[n].SetCheck(OPGroupInfo.GetEnableSystem(n));
	}
	
	m_editName.SetSel ( 0, -1 );
	m_editName.SetFocus();
}

/**********************************************************************/

void CDefEditOPGroupDlg::SaveData()
{
	COPGroupMiniInfo infoMini;
	DataManagerNonDb.OPGroupSystem.GetOPGroup(m_nIndex, infoMini);

	CString strName;
	m_editName.GetWindowText(strName);
	infoMini.m_strName = strName;

	COPGroupInfo OPGroupInfo;
	OPGroupInfo.SetEnableFlagsSystem(0);

	for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
	{
		if (m_checkPrinter[n].GetCheck() != 0)
		{
			OPGroupInfo.SetEnableSystem(n, TRUE);
		}
	}

	infoMini.m_nFlags = OPGroupInfo.GetEnableFlagsSystem();

	DataManagerNonDb.OPGroupSystem.UpdateOPGroup(m_nIndex, infoMini);

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditOPGroupDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
