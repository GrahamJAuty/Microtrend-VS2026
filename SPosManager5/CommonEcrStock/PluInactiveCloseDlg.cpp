/**********************************************************************/
#include "PluInactiveCloseDlg.h"
/**********************************************************************/

CPluInactiveCloseDlg::CPluInactiveCloseDlg( int nCount, CWnd* pParent)
	: CDialog(CPluInactiveCloseDlg::IDD, pParent)
{
	m_nCount = nCount;
	m_nAction = PLUINACTIVE_CANCEL;
}

/**********************************************************************/

CPluInactiveCloseDlg::~CPluInactiveCloseDlg()
{
}

/**********************************************************************/

void CPluInactiveCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_buttonConfirm);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluInactiveCloseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnButtonContinue)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluInactiveCloseDlg::OnInitDialog()
{
	SetDefID( IDC_BUTTON_CANCEL );
	
	CDialog::OnInitDialog();

	{
		CString strTitle;
		strTitle.Format( "Flag Selected Plu Item%s As Inactive", ( m_nCount > 1 ) ? "s" : "" );
		SetWindowText( strTitle );
	}

	{
		CString strInfo;
		strInfo.Format( "%d item%s been selected to flag as inactive.",
			m_nCount,
			( m_nCount > 1 ) ? "s have" : " has" );

		m_static1.SetWindowText( strInfo );
	}

	{
		CString strInfo;
		strInfo.Format( "Do you wish to proceed with %s ?",
			( m_nCount > 1 ) ? "these changes" : "this change" );

		m_static2.SetWindowText( strInfo );
	}

	{
		CString strButton;
		strButton.Format( "Confirm\nChange%s", ( m_nCount > 1 ) ? "s" : "" );
		m_buttonConfirm.SetWindowText( strButton );
	}

	{
		CString strButton;
		strButton.Format( "Abandon\nChange%s", ( m_nCount > 1 ) ? "s" : "" );
		m_buttonCancel.SetWindowText( strButton );
	}
	
	return FALSE;  
}

/**********************************************************************/

void CPluInactiveCloseDlg::OnOK()
{
	OnButtonCancel();
}

/**********************************************************************/

void CPluInactiveCloseDlg::OnCancel()
{
	OnButtonCancel();
}

/**********************************************************************/

void CPluInactiveCloseDlg::OnButtonConfirm()
{
	m_nAction = PLUINACTIVE_CONFIRM;
	EndDialog( IDOK );
}

/**********************************************************************/

void CPluInactiveCloseDlg::OnButtonCancel()
{
	m_nAction = PLUINACTIVE_CANCEL;
	EndDialog( IDOK );
}

/**********************************************************************/

void CPluInactiveCloseDlg::OnButtonContinue()
{
	m_nAction = PLUINACTIVE_CONTINUE;
	EndDialog( IDOK );
}

/**********************************************************************/
