/**********************************************************************/

#include "local.h"
#include "SyssetDlg2.h"

/**********************************************************************/

CSyssetDlg2::CSyssetDlg2 ( CWnd* pParent )
	: CDialog ( CSyssetDlg2::IDD, pParent )
{
	m_strDealerPhone = _T("");
	m_timeExpiry = COleDateTime::GetCurrentTime();
}

/**********************************************************************/

void CSyssetDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSSET_STATIC_PHONE, m_staticPhone);
	DDX_Control(pDX, IDC_SYSSET_STATIC_EXPIRY, m_staticExpiry);
	DDX_Control(pDX, IDC_SYSSET_BUTTON_CLEAR_EXPIRY, m_buttonClearExpiry);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE6, m_staticLine6);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE5, m_staticLine5);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE4, m_staticLine4);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE3, m_staticLine3);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE2, m_staticLine2);
	DDX_Control(pDX, IDC_SYSSET_STATIC_LINE1, m_staticLine1);
	DDX_Text(pDX, IDC_SYSSET_EDIT_DEALER_PHONE, m_strDealerPhone);
	DDV_MaxChars(pDX, m_strDealerPhone, 19);
	DDX_DateTimeCtrl(pDX, IDC_SYSSET_DATETIMEPICKER1, m_timeExpiry);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSyssetDlg2, CDialog)
	ON_BN_CLICKED(IDC_SYSSET_BUTTON_CLEAR_EXPIRY, OnButtonClearExpiry)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSyssetDlg2::OnInitDialog()
{
	m_editDealerPhone.SubclassDlgItem(IDC_SYSSET_EDIT_DEALER_PHONE, this);
	m_editDealerPhone.SetValidChars(SYSSET_VALID_TELEPHONE);		// only allow valid telephone chars

	CDialog::OnInitDialog();

	SetWindowText("Dealer Expiry Date");

	m_staticLine1.SetWindowText("If you would like to set an expiry date for this program");
	m_staticLine2.SetWindowText("select the required date, enter your phone number and press OK.");
	m_staticLine3.SetWindowText("You will be responsible for providing the access code to the");
	m_staticLine4.SetWindowText("end user when the expiry date is reached.");
	m_staticLine5.SetWindowText("You must tell the end user when the software will expire.");
	m_staticLine6.SetWindowText("If you are not sure, do not set an expiry date.");

	m_staticPhone.SetWindowText("Your telephone number");
	m_staticExpiry.SetWindowText("Expiry date");

	if (m_strExpiry.GetLength() == 6)
	{
		int nDay = atoi(m_strExpiry.Mid(0, 2));
		int nMonth = atoi(m_strExpiry.Mid(2, 2));
		int nYear = atoi(m_strExpiry.Mid(4, 2)) + 2000;

		m_timeExpiry = COleDateTime(nYear, nMonth, nDay, 0, 0, 0);

		UpdateData(FALSE);
	}

	m_buttonClearExpiry.SetWindowText("Clear Expiry");

	return TRUE;
}

/**********************************************************************/

void CSyssetDlg2::OnOK() 
{
	if (UpdateData(TRUE) == TRUE)
	{
		CString strNow = COleDateTime::GetCurrentTime().Format("%Y%m%d");	// yyyymmdd
		CString strExpiry = m_timeExpiry.Format("%Y%m%d");				// yyyymmdd

		if (strExpiry >= strNow)	// today or future date
		{
			m_strExpiry = m_timeExpiry.Format("%d%m%y");	// ddmmyy
			EndDialog(IDOK);
		}
		else						// past date
		{
			AfxMessageBox("You must set a date in the future");
		}
	}
}

/**********************************************************************/

void CSyssetDlg2::OnButtonClearExpiry()
{
	m_timeExpiry = COleDateTime::GetCurrentTime();		// today's date means no expiry
	UpdateData(FALSE);
}

/**********************************************************************/
