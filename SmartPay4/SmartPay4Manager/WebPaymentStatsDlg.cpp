//************************************************************
#include "WebPaymentReporter.h"
//************************************************************
#include "WebPaymentStatsDlg.h"
//************************************************************

CWebPaymentStatsDlg::CWebPaymentStatsDlg(int nWebPaymentType, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CWebPaymentStatsDlg::IDD, pParent)
{
	m_nWebPaymentType = nWebPaymentType;
	m_strExceptionFilename = "";
	m_strImportFilename = "";
	m_nExceptions = 0;
	m_nRead = 0;
	m_nValid = 0;
}

//************************************************************

void CWebPaymentStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTIONS, m_buttonExceptions);
	DDX_Control(pDX, IDC_EDIT_EXCEPTIONS, m_editExceptions);
	DDX_Text(pDX, IDC_EDIT_EXCEPTIONS, m_nExceptions);
	DDX_Text(pDX, IDC_EDIT_LINESREAD, m_nRead);
	DDX_Text(pDX, IDC_EDIT_LINESVALID, m_nValid);
}

//************************************************************

BEGIN_MESSAGE_MAP(CWebPaymentStatsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTIONS, OnButtonExceptions)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
END_MESSAGE_MAP()

//************************************************************

BOOL CWebPaymentStatsDlg::CSSAutoShutdownPostInitDialog()
{
	CString strTitle = "";
	strTitle.Format ( "%s Web Payment Import Statistics", 
		(const char*) System.GetWebPaymentDisplayName() );

	SetWindowText ( strTitle );

	if ( m_nExceptions == 0 )
	{
		m_buttonExceptions.ShowWindow(SW_HIDE);
		m_editExceptions.EnableWindow(FALSE);
	}

	return TRUE;  
}

//************************************************************

void CWebPaymentStatsDlg::OnButtonExceptions() 
{
	CWebPaymentReporter report ( "R203", m_nWebPaymentType, m_strExceptionFilename, this );
	report.Show();
}

//************************************************************

void CWebPaymentStatsDlg::OnButtonView() 
{
	CWebPaymentReporter report ( "R204", m_nWebPaymentType, m_strImportFilename, this );
	report.Show();
}

//************************************************************
