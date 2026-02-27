//$$******************************************************************
#include "SimulateReaderDlg.h"
//$$******************************************************************

CSimulateReaderDlg::CSimulateReaderDlg( CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CSimulateReaderDlg::IDD, pParent)
{
}

//$$******************************************************************

void CSimulateReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MIFARE, m_editMiFareNo );
	DDX_Text(pDX, IDC_EDIT_MIFARE, m_strInputMiFareNo);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSimulateReaderDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
	ON_BN_CLICKED(IDC_BUTTON_ERROR, OnButtonError)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE, OnButtonTerminate)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSimulateReaderDlg::CSSAutoShutdownPostInitDialog()
{
	m_editMiFareNo.LimitText(20);
	return TRUE;
}

//$$******************************************************************

void CSimulateReaderDlg::OnButtonAccept()
{
	UpdateData(TRUE);
	m_nResult = 0;
	EndDialog( IDOK );
}

//$$******************************************************************

void CSimulateReaderDlg::OnButtonError()
{
	m_nResult = 1;
	EndDialog( IDOK );
}

//$$******************************************************************

void CSimulateReaderDlg::OnButtonTerminate()
{
	m_nResult = -1;
	EndDialog( IDOK );
}

//$$******************************************************************

void CSimulateReaderDlg::OnOK()
{
}

//$$******************************************************************

void CSimulateReaderDlg::OnCancel()
{
}

//$$******************************************************************
