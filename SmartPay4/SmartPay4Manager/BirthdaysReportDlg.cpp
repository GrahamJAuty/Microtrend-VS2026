//**********************************************************************
#include "SmartPay4Manager.h"
//**********************************************************************
#include "BirthdaysReportDlg.h"
//**********************************************************************

CBirthdaysReportDlg::CBirthdaysReportDlg( CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CBirthdaysReportDlg::IDD, pParent)
{
	m_bCheckMonth1 = FALSE;
	m_bCheckMonth2 = FALSE;
	m_bCheckMonth3 = FALSE;
	m_bCheckMonth4 = FALSE;
	m_bCheckMonth5 = FALSE;
	m_bCheckMonth6 = FALSE;
	m_bCheckMonth7 = FALSE;
	m_bCheckMonth8 = FALSE;
	m_bCheckMonth9 = FALSE;
	m_bCheckMonth10 = FALSE;
	m_bCheckMonth11 = FALSE;
	m_bCheckMonth12 = FALSE;
	m_bDobReqd = FALSE;
	m_bExpiryReqd = FALSE;

	m_nSelectAll = 0;

	CTime t = CTime::GetCurrentTime();
	m_nCurrentMonth = t.GetMonth();
	m_nNextMonth = m_nCurrentMonth;
}

//**********************************************************************

CBirthdaysReportDlg::~CBirthdaysReportDlg()
{
}

//**********************************************************************

void CBirthdaysReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_MONTH1, m_bCheckMonth1);
	DDX_Check(pDX, IDC_CHECK_MONTH2, m_bCheckMonth2);
	DDX_Check(pDX, IDC_CHECK_MONTH3, m_bCheckMonth3);
	DDX_Check(pDX, IDC_CHECK_MONTH4, m_bCheckMonth4);
	DDX_Check(pDX, IDC_CHECK_MONTH5, m_bCheckMonth5);
	DDX_Check(pDX, IDC_CHECK_MONTH6, m_bCheckMonth6);
	DDX_Check(pDX, IDC_CHECK_MONTH7, m_bCheckMonth7);
	DDX_Check(pDX, IDC_CHECK_MONTH8, m_bCheckMonth8);
	DDX_Check(pDX, IDC_CHECK_MONTH9, m_bCheckMonth9);
	DDX_Check(pDX, IDC_CHECK_MONTH10, m_bCheckMonth10);
	DDX_Check(pDX, IDC_CHECK_MONTH11, m_bCheckMonth11);
	DDX_Check(pDX, IDC_CHECK_MONTH12, m_bCheckMonth12);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Check(pDX, IDC_CHECK_DOB, m_bDobReqd);
	DDX_Check(pDX, IDC_CHECK_EXPIRY, m_bExpiryReqd);
}

//**********************************************************************

BEGIN_MESSAGE_MAP(CBirthdaysReportDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnBnClickedButtonAll)
END_MESSAGE_MAP()


//*******************************************************************

BOOL CBirthdaysReportDlg::CSSAutoShutdownPostInitDialog()
{
	if ( MonthsSelected() < 12 )
	{
		m_buttonAll.SetWindowText ( "&All" );
		m_nSelectAll = 0;
	}
	else
	{
		m_nSelectAll = 1;
		m_buttonAll.SetWindowText ( "N&one" );
	}
	
	return TRUE;  
}

//**********************************************************************

void CBirthdaysReportDlg::OnOK()
{
	if ( MonthsSelected() == 0 )
	{
		MessageBeep( MB_ICONEXCLAMATION );
		Prompter.Error( "No month criteria selected", "Cannot continue" );
		return;
	}

	if ( m_bDobReqd == FALSE && m_bExpiryReqd == FALSE )
	{
		MessageBeep( MB_ICONEXCLAMATION );
		Prompter.Error( "No date criteria selected", "Cannot continue" );
		return;
	}

	EndDialog(IDOK);
}

//**********************************************************************

void CBirthdaysReportDlg::OnBnClickedButtonNext()
{
	if ( UpdateData(TRUE) == TRUE )
	{
		if (++m_nNextMonth > 12)
		{
			m_nNextMonth = 1;
		}

		switch ( m_nNextMonth )
		{
		case 1:		m_bCheckMonth1 = !m_bCheckMonth1;		break;
		case 2:		m_bCheckMonth2 = !m_bCheckMonth2;		break;
		case 3:		m_bCheckMonth3 = !m_bCheckMonth3;		break;
		case 4:		m_bCheckMonth4 = !m_bCheckMonth4;		break;
		case 5:		m_bCheckMonth5 = !m_bCheckMonth5;		break;
		case 6:		m_bCheckMonth6 = !m_bCheckMonth6;		break;
		case 7:		m_bCheckMonth7 = !m_bCheckMonth7;		break;
		case 8:		m_bCheckMonth8 = !m_bCheckMonth8;		break;
		case 9:		m_bCheckMonth9 = !m_bCheckMonth9;		break;
		case 10:	m_bCheckMonth10 = !m_bCheckMonth10;		break;
		case 11:	m_bCheckMonth11 = !m_bCheckMonth11;		break;
		case 12:	m_bCheckMonth12 = !m_bCheckMonth12;		break;
		}

		UpdateData(FALSE);
	}
}

//**********************************************************************

void CBirthdaysReportDlg::OnBnClickedButtonAll()
{
	if ( UpdateData() == TRUE )
	{
		m_nSelectAll ^= 0x01;
		bool bReqd = ( m_nSelectAll == 1 ) ? TRUE : FALSE;

		m_bCheckMonth1 = bReqd;
		m_bCheckMonth2 = bReqd;
		m_bCheckMonth3 = bReqd;
		m_bCheckMonth4 = bReqd;
		m_bCheckMonth5 = bReqd;
		m_bCheckMonth6 = bReqd;
		m_bCheckMonth7 = bReqd;
		m_bCheckMonth8 = bReqd;
		m_bCheckMonth9 = bReqd;
		m_bCheckMonth10 = bReqd;
		m_bCheckMonth11 = bReqd;
		m_bCheckMonth12 = bReqd;

		if (bReqd == TRUE)
		{
			m_buttonAll.SetWindowText("N&one");
		}
		else
		{
			m_buttonAll.SetWindowText("&All");
		}

		m_nNextMonth = m_nCurrentMonth;

		UpdateData(FALSE);
	}
}

//**********************************************************************

int CBirthdaysReportDlg::MonthsSelected() 
{
	int nCount = 0;

	if ( UpdateData(TRUE) == TRUE )
	{
		if ( m_bCheckMonth1 == TRUE )	++nCount;
		if ( m_bCheckMonth2 == TRUE )	++nCount;
		if ( m_bCheckMonth3 == TRUE )	++nCount;
		if ( m_bCheckMonth4 == TRUE )	++nCount;
		if ( m_bCheckMonth5 == TRUE )	++nCount;
		if ( m_bCheckMonth6 == TRUE )	++nCount;
		if ( m_bCheckMonth7 == TRUE )	++nCount;
		if ( m_bCheckMonth8 == TRUE )	++nCount;
		if ( m_bCheckMonth9 == TRUE )	++nCount;
		if ( m_bCheckMonth10 == TRUE )	++nCount;
		if ( m_bCheckMonth11 == TRUE )	++nCount;
		if ( m_bCheckMonth12 == TRUE )	++nCount;
	}

	return nCount;
}

//*******************************************************************
