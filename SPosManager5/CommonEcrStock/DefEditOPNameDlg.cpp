/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "DefEditOPNameDlg.h"
/**********************************************************************/

CDefEditOPNameDlg::CDefEditOPNameDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditOPNameDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditOPNameDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_pListDataDlg = NULL;
}

/**********************************************************************/

void CDefEditOPNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditOPNameDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditOPNameDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditOPNameDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditOPNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetData();
	return FALSE;  
}

/**********************************************************************/

void CDefEditOPNameDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDefEditOPNameDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditOPNameDlg::SpinPrevious()
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

void CDefEditOPNameDlg::SpinNext()
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

void CDefEditOPNameDlg::GetData()
{
	CString strTitle = "";

	COPNameMiniInfo infoMini;
	DataManagerNonDb.OPNameSystem.GetOPName( m_nIndex, infoMini );
	
	strTitle.Format ( "Edit Name Order Printer %d", 
		infoMini.m_nPrinterNo );
	
	SetWindowText ( strTitle );

	m_editName.SetWindowText( infoMini.m_strName );
	m_editName.LimitText( MAX_LENGTH_OPNAME );

	m_editName.SetSel ( 0, -1 );
	m_editName.SetFocus();
}

/**********************************************************************/

void CDefEditOPNameDlg::SaveData()
{
	COPNameMiniInfo infoMini;
	DataManagerNonDb.OPNameSystem.GetOPName( m_nIndex, infoMini );

	CString strName = "";
	m_editName.GetWindowText( strName );
	
	if (strName == "")
	{
		strName.Format("Printer %d", infoMini.m_nPrinterNo);
	}
	
	infoMini.m_strName = strName;

	DataManagerNonDb.OPNameSystem.UpdateOPName( m_nIndex, infoMini );

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditOPNameDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
