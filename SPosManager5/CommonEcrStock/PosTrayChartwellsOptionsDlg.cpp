/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\AESHelpers.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include <CkSFtp.h>
/**********************************************************************/
#include "PosTrayChartwellsOptionsDlg.h"
/**********************************************************************/

CPosTrayChartwellsOptionsDlg::CPosTrayChartwellsOptionsDlg( CChartwellsOptions& Options, CWnd* pParent )
	: CSSDialog(CPosTrayChartwellsOptionsDlg::IDD, pParent), m_Options( Options )
{
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
}

/**********************************************************************/

CPosTrayChartwellsOptionsDlg::~CPosTrayChartwellsOptionsDlg()
{
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_editServer);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_buttonPassword);
	DDX_Control(pDX, IDC_EDIT_SITENO, m_editSiteNo);
	DDX_Control(pDX, IDC_DATEPICKER_EXPORT, m_DatePickerExport);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticExport);
	DDX_Control(pDX, IDC_COMBO_EXPORT_DAY, m_comboExportDay);
	DDX_Control(pDX, IDC_TIMEPICKER_EXPORT, m_TimePickerExport);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPosTrayChartwellsOptionsDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword)
	ON_BN_CLICKED( IDC_BUTTON_TEST, OnButtonTest)
	ON_NOTIFY( DTN_DATETIMECHANGE, IDC_DATEPICKER_EXPORT, OnChangeDate)
	ON_CBN_SELCHANGE( IDC_COMBO_EXPORT_DAY, OnSelectExportDay)
	ON_NOTIFY( DTN_DATETIMECHANGE, IDC_TIMEPICKER_EXPORT, OnChangeExportTime)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPosTrayChartwellsOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SubclassEdit(IDC_EDIT_SMARTPAY, SS_NUM, 6, "%d");

	SetWindowText( "Configure Settings for Chartwells Export" );

	m_editServer.LimitText( MAX_LENGTH_FTP_SERVER );
	m_editUserName.LimitText( MAX_LENGTH_FTP_USERNAME );
	m_editSiteNo.LimitText(MAX_LENGTH_CHARTWELLS_SITENO);
	
	m_editServer.SetWindowText( m_Options.GetFTPServer() );
	m_editUserName.SetWindowText( m_Options.GetFTPUserName() );
	m_editSiteNo.SetWindowText(m_Options.GetChartwellsSiteNo());

	CString strPassword = m_Options.GetFTPPassword();
	
	if ( strPassword != "" )
	{
		m_editPassword.SetWindowText( "<The password has been set>" );
		m_bGotOldPassword = TRUE;
	}
	else
	{
		m_editPassword.SetWindowText( "<No password set>" );
		m_bGotOldPassword = FALSE;
	}

	m_comboExportDay.ResetContent();
	m_comboExportDay.AddString("Same Day");
	m_comboExportDay.AddString("Next Day");
	m_comboExportDay.SetCurSel(PosTrayOptions.GetChartwellsNextDayFlag() ? 1 : 0);

	{
		int nSerialNo = m_Options.GetSmartPaySerialNo();
		if ((nSerialNo < m_Options.GetMinSerialNo()) || (nSerialNo > m_Options.GetMaxSerialNo()))
		{
			nSerialNo = m_Options.GetDefaultSerialNo();
		}
		SetEditBoxInt(*GetEditSmartPay(), nSerialNo);
	}

	COleDateTime oleDateStart = COleDateTime( 2014, 1, 1, 0, 0, 0 );
	COleDateTime oleDateEnd = COleDateTime( 2500, 1, 1, 0, 0, 0 );

	m_DatePickerExport.SetRange ( &oleDateStart, &oleDateEnd );
	m_DatePickerExport.SetFormat( "ddd dd MMM yyy" );

	m_TimePickerExport.SetRange(&oleDateStart, &oleDateEnd);
	m_TimePickerExport.SetFormat("HH:mm");

	int nMinutes = PosTrayOptions.GetChartwellsExportTime();
	if ((nMinutes < 0) || (nMinutes > 1439))
	{
		nMinutes = 0;
	}

	m_TimePickerExport.SetTime(oleDateStart + COleDateTimeSpan(0, nMinutes / 60, nMinutes % 60, 0));

	CString strDate = m_Options.GetLastExportDate(TRUE );
	if (strDate == "")
	{
		strDate = m_Options.GetLastExportDate(FALSE);
	}

	COleDateTime oleDayToExport, oleExportTime;
	if (m_Options.GetNextExportTime(oleDayToExport, oleExportTime, strDate) == FALSE)
	{
		oleDayToExport = oleDateStart;
	}

	if ((oleDayToExport < oleDateStart) || (oleDayToExport > oleDateEnd))
	{
		oleDayToExport = oleDateStart;
	}

	m_DatePickerExport.SetTime( oleDayToExport );

	ShowNextExportTime( oleExportTime );
	
	return TRUE;
}

/**********************************************************************/
CEdit* CPosTrayChartwellsOptionsDlg::GetEditSmartPay() { return GetEdit(IDC_EDIT_SMARTPAY); }
/**********************************************************************/

bool CPosTrayChartwellsOptionsDlg::ValidateSerialNo()
{
	bool bResult = TRUE;

	{
		int nSerialNo = GetEditBoxInt(*GetEditSmartPay());
		if ((nSerialNo < m_Options.GetMinSerialNo()) || (nSerialNo > m_Options.GetMaxSerialNo()))
		{
			CString strError = "";
			strError.Format("Please specify a Serial Number between %d and %d",
				m_Options.GetMinSerialNo(),
				m_Options.GetMaxSerialNo());

			AfxMessageBox(strError);
			GetEditSmartPay()->SetSel(0, -1);
			GetEditSmartPay()->SetFocus();
			bResult = FALSE;
		}
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::UpdateNextExportTime()
{
	COleDateTime dateExport;
	m_DatePickerExport.GetTime( dateExport );

	if (dateExport.GetStatus() == COleDateTime::valid)
	{
		dateExport -= COleDateTimeSpan(1, 0, 0, 0);

		CString strDate;
		strDate.Format("%4.4d%2.2d%2.2d",
			dateExport.GetYear(),
			dateExport.GetMonth(),
			dateExport.GetDay());

		COleDateTime oleDayToExport, oleExportTime;
		if (m_Options.GetNextExportTime(oleDayToExport, oleExportTime, strDate) == TRUE)
		{
			ShowNextExportTime(oleExportTime);
		}
	}
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::ShowNextExportTime( COleDateTime& time )
{
	CString strExport = "";
	strExport.Format( "( Export due at %2.2d:%2.2d at %2.2d/%2.2d/%4.4d )",
		time.GetHour(),
		time.GetMinute(),
		time.GetDay(),
		time.GetMonth(),
		time.GetYear() );
	m_staticExport.SetWindowText( strExport );
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateNextExportTime();
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnChangeExportTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnSelectExportDay();
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnSelectExportDay()
{
	bool bTemp = PosTrayOptions.GetChartwellsNextDayFlag();
	int nTemp = PosTrayOptions.GetChartwellsExportTime();

	COleDateTime timeExport;
	m_TimePickerExport.GetTime(timeExport);
	PosTrayOptions.SetChartwellsExportTime((timeExport.GetHour() * 60) + timeExport.GetMinute());
	PosTrayOptions.SetChartwellsNextDayFlag(m_comboExportDay.GetCurSel() == 1);
	
	UpdateNextExportTime();

	PosTrayOptions.SetChartwellsNextDayFlag(bTemp);
	PosTrayOptions.SetChartwellsExportTime(nTemp);
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnButtonPassword()
{
	CDefEditDlg dlg("Set Chartwells Password", "", MAX_LENGTH_FTP_PASSWORD, "Password", this);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strName != "")
		{
			CAESHelpers::PlainTextToAESHex(dlg.m_strName, m_strNewPassword);

			if (TRUE == m_bGotOldPassword)
			{
				m_editPassword.SetWindowText("<The password has been changed>");
			}
			else
			{
				m_editPassword.SetWindowText("<The password has been set>");
			}
		}
	}
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnButtonTest()
{
	CString strServer = GetEditBoxText(m_editServer);
	CString strUserName = GetEditBoxText(m_editUserName);
	
	CString strPasswordAES = m_strNewPassword;
	if ((strPasswordAES) == "" && (m_bGotOldPassword == TRUE))
	{
		strPasswordAES = m_Options.GetFTPPassword();
	}

	CkSFtp SFTPConnect;

	SFTPConnect.put_ConnectTimeoutMs(5000);
	SFTPConnect.put_IdleTimeoutMs(10000);

	// Connect to the SSH server.  
	CString strError = "";
	bool bOK = FALSE;

	if (SFTPConnect.Connect(strServer, 22) == TRUE)
	{
		CString strPasswordPT = "";
		CAESHelpers::AESHexToPlainText(strPasswordAES, strPasswordPT);

		if (SFTPConnect.AuthenticatePw(strUserName, strPasswordPT) == TRUE)
		{
			if (SFTPConnect.InitializeSftp() == TRUE)
			{
				strError = "Test connection to FTP Server was successful";
				bOK = TRUE;
			}
			else
			{
				strError = "Failed to initialise with FTP Server";
			}
		}
		else
		{
			strError = "Failed to authenticate with FTP Server";
		}

		SFTPConnect.Disconnect();
	}
	else
	{
		strError = "Failed to connect to FTP Server";
	}

	if (TRUE == bOK)
	{
		Prompter.Info(strError);
	}
	else
	{
		Prompter.Error(strError);
	}
}

/**********************************************************************/

void CPosTrayChartwellsOptionsDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		if (ValidateSerialNo() == FALSE)
		{
			return;
		}

		m_Options.SetFTPServer(GetEditBoxText(m_editServer));
		m_Options.SetFTPUserName(GetEditBoxText(m_editUserName));
		m_Options.SetChartwellsSiteNo(GetEditBoxText(m_editSiteNo));

		if (m_strNewPassword != "")
		{
			m_Options.SetFTPPassword(m_strNewPassword);
		}

		m_Options.SetSmartPaySerialNo(GetEditBoxInt(*GetEditSmartPay()));

		PosTrayOptions.SetChartwellsNextDayFlag(m_comboExportDay.GetCurSel() == 1);

		COleDateTime timeExport;
		m_TimePickerExport.GetTime(timeExport);
		PosTrayOptions.SetChartwellsExportTime((timeExport.GetHour() * 60) + timeExport.GetMinute());

		m_DatePickerExport.GetTime(m_oleDateNextExport);

		EndDialog(IDOK);
	}
}

/**********************************************************************/

