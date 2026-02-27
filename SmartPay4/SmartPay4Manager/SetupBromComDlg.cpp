//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
//*******************************************************
#include "SetupBromComDlg.h"
//*******************************************************

CSetupBromComDlg::CSetupBromComDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupBromComDlg::IDD, pParent)
{
}

//*******************************************************

CSetupBromComDlg::~CSetupBromComDlg()
{
}

//*******************************************************

void CSetupBromComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SCHOOLID, m_editSchoolID);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXmlFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXmlLogin);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLFILES, m_checkKeepXmlFiles);
	DDX_Control(pDX, IDC_CHECK_KEEPXMLLOGIN, m_checkKeepXmlLogin);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	/*****/
	DDX_Control(pDX, IDC_CHECK_PURSE1ONLY, m_checkPurse1Only);
	DDX_Control(pDX, IDC_EDIT_REFRESH, m_editRefresh);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupBromComDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

//*******************************************************

void CSetupBromComDlg::CSSAutoShutdownPreInitDialog()
{
	m_data.Read();
}

//*******************************************************

BOOL CSetupBromComDlg::CSSAutoShutdownPostInitDialog()
{
	m_checkKeepXmlFiles.SetCheck(m_data.m_bKeepXmlFiles);
	m_checkKeepXmlLogin.SetCheck(m_data.m_bKeepXmlLogin);
	m_editSchoolID.SetWindowText(m_data.GetSchoolID());
	m_editUserName.SetWindowText(m_data.GetUsername());
	m_editPassword.SetWindowText(m_data.GetPassword());

	SubclassEdit(IDC_EDIT_ADJUSTSTART, SS_NUM, 4, "%d");

	m_DatePickerFrom.SetFormat("ddd dd MMM yyy");
	m_TimePickerFrom.SetFormat("HH:mm:ss");

	if ( ( FALSE == m_data.m_bKeepXmlFiles ) && ( FALSE == m_data.m_bKeepXmlLogin ) )
	{
		m_checkKeepXmlFiles.ShowWindow(SW_HIDE);
		m_checkKeepXmlLogin.ShowWindow(SW_HIDE);
		m_buttonDelete.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow( SW_HIDE );

	COleDateTime dateMin = COleDateTime(2020, 1, 1, 0, 0, 0);
	COleDateTime dateMax = COleDateTime(2200, 1, 1, 0, 0, 0);
	COleDateTime dateSet = COleDateTime(2022, 1, 1, 0, 0, 0);

	{
		CString strDate = m_data.GetBromComLastDate();
		if (strDate.GetLength() == 8)
		{
			COleDateTime dateTry = COleDateTime(atoi(strDate.Left(4)), atoi(strDate.Mid(4, 2)), atoi(strDate.Right(2)), 0, 0, 0);
			if (dateTry.m_status == COleDateTime::valid)
			{
				if ((dateTry >= dateMin) && (dateTry <= dateMax))
				{
					dateSet = dateTry;
				}
			}
		}
	}

	m_DatePickerFrom.SetTime(dateSet);
	m_DatePickerFrom.SetRange(&dateMin, &dateMax);

	{
		CString strTime = m_data.GetBromComLastTime();
		if (strTime.GetLength() == 6)
		{
			COleDateTime dateTry = COleDateTime( 2022, 1, 1, atoi(strTime.Left(2)), atoi(strTime.Mid(2,2)), atoi(strTime.Right(2)));
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateSet = dateTry;
			}
		}
	}

	m_TimePickerFrom.SetTime(dateSet);

	SetEditBoxInt(*GetEditAdjustStart(), m_data.GetSQLFilterAdjustMinutes());

	m_checkPurse1Only.SetCheck(m_data.GetPurse1OnlyBalanceFlag());
	m_editRefresh.LimitText(20);
	m_editRefresh.SetWindowTextA(m_data.GetPurse2RefreshText());

	return TRUE;
}

//*******************************************************

void CSetupBromComDlg::OnButtonAltKey1()
{
	m_checkKeepXmlFiles.ShowWindow(SW_SHOW);
	m_checkKeepXmlLogin.ShowWindow(SW_SHOW);
	m_buttonDelete.ShowWindow(SW_SHOW);
}

//*******************************************************

void CSetupBromComDlg::OnButtonDelete()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks(Request, this);
	
	if ( LOCKMANAGER_OK == nLockError )						
	{
		CString strMask	 = System.GetWebPaymentPath ( "*.xml", nWEBPAYMENT_BROMCOM );
		::DeleteWithConfirmation ( strMask, "Delete XML Files", this );
	}
}

//*******************************************************

void CSetupBromComDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_data.m_bKeepXmlFiles = IsTicked(m_checkKeepXmlFiles);
		m_data.m_bKeepXmlLogin = IsTicked(m_checkKeepXmlLogin);
		m_data.SetUsername(GetEditBoxText(m_editUserName));
		m_data.SetPassword(GetEditBoxText(m_editPassword));
		m_data.SetSchoolID(GetEditBoxText(m_editSchoolID));
		m_data.SetSQLFilterAdjustMinutes(GetEditBoxInt(*GetEditAdjustStart()));

		{
			COleDateTime dateFrom;
			m_DatePickerFrom.GetTime(dateFrom);
			CString strLastDate = "";
			strLastDate.Format("%4.4d%2.2d%2.2d", dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay());
			m_data.SetBromComLastDate(strLastDate);
		}

		{
			COleDateTime timeFrom;
			m_TimePickerFrom.GetTime(timeFrom);
			CString strLastTime = "";
			strLastTime.Format("%2.2d%2.2d%2.2d", timeFrom.GetHour(), timeFrom.GetMinute(), timeFrom.GetSecond());
			m_data.SetBromComLastTime(strLastTime);
		}

		m_data.SetPurse1OnlyBalanceFlag(IsTicked(m_checkPurse1Only));
		m_data.SetPurse2RefreshText(GetEditBoxText(m_editRefresh));

		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************
