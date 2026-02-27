//*********************************************************************
#include "ServiceTimesDlg.h"
//*********************************************************************
IMPLEMENT_DYNAMIC(CServiceTimesDlg, CDialog)
//*********************************************************************

CServiceTimesDlg::CServiceTimesDlg(CWnd* pParent)
	: CDialog(CServiceTimesDlg::IDD, pParent)
{
	m_ServiceTimes.Read();

	m_bInService1 = m_ServiceTimes.GetReqd(0);
	m_bInService2 = m_ServiceTimes.GetReqd(1);
	m_bInService3 = m_ServiceTimes.GetReqd(2);
	m_bInService4 = m_ServiceTimes.GetReqd(3);
	m_bInService5 = m_ServiceTimes.GetReqd(4);

	m_bBalanceOnly1 = m_ServiceTimes.GetBalanceOnly(0);
	m_bBalanceOnly2 = m_ServiceTimes.GetBalanceOnly(1);
	m_bBalanceOnly3 = m_ServiceTimes.GetBalanceOnly(2);
	m_bBalanceOnly4 = m_ServiceTimes.GetBalanceOnly(3);
	m_bBalanceOnly5 = m_ServiceTimes.GetBalanceOnly(4);
}

//*********************************************************************

void CServiceTimesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ENABLESERVICE1, m_bInService1);
	DDX_Check(pDX, IDC_ENABLESERVICE2, m_bInService2);
	DDX_Check(pDX, IDC_ENABLESERVICE3, m_bInService3);
	DDX_Check(pDX, IDC_ENABLESERVICE4, m_bInService4);
	DDX_Check(pDX, IDC_ENABLESERVICE5, m_bInService5);
	DDX_Check(pDX, IDC_CHECK_BALANCEONLY1, m_bBalanceOnly1);
	DDX_Check(pDX, IDC_CHECK_BALANCEONLY2, m_bBalanceOnly2);
	DDX_Check(pDX, IDC_CHECK_BALANCEONLY3, m_bBalanceOnly3);
	DDX_Check(pDX, IDC_CHECK_BALANCEONLY4, m_bBalanceOnly4);
	DDX_Check(pDX, IDC_CHECK_BALANCEONLY5, m_bBalanceOnly5);
	DDX_Control(pDX, IDC_CHECK_BALANCEONLY1, m_checkBalanceOnly1);
	DDX_Control(pDX, IDC_CHECK_BALANCEONLY2, m_checkBalanceOnly2);
	DDX_Control(pDX, IDC_CHECK_BALANCEONLY3, m_checkBalanceOnly3);
	DDX_Control(pDX, IDC_CHECK_BALANCEONLY4, m_checkBalanceOnly4);
	DDX_Control(pDX, IDC_CHECK_BALANCEONLY5, m_checkBalanceOnly5);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcStart1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtcEnd1);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dtcStart2);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dtcEnd2);
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_dtcStart3);
	DDX_Control(pDX, IDC_DATETIMEPICKER6, m_dtcEnd3);
	DDX_Control(pDX, IDC_DATETIMEPICKER7, m_dtcStart4);
	DDX_Control(pDX, IDC_DATETIMEPICKER8, m_dtcEnd4);
	DDX_Control(pDX, IDC_DATETIMEPICKER9, m_dtcStart5);
	DDX_Control(pDX, IDC_DATETIMEPICKER10, m_dtcEnd5);
}

//*********************************************************************

BEGIN_MESSAGE_MAP(CServiceTimesDlg, CDialog)
	ON_BN_CLICKED(IDC_ENABLESERVICE1, OnButtonEnableService1)
	ON_BN_CLICKED(IDC_ENABLESERVICE2, OnButtonEnableService2)
	ON_BN_CLICKED(IDC_ENABLESERVICE3, OnButtonEnableService3)
	ON_BN_CLICKED(IDC_ENABLESERVICE4, OnButtonEnableService4)
	ON_BN_CLICKED(IDC_ENABLESERVICE5, OnButtonEnableService5)
END_MESSAGE_MAP()

//*********************************************************************

BOOL CServiceTimesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dtcStart1.SetFormat("HH:mm");
	m_dtcStart2.SetFormat("HH:mm");
	m_dtcStart3.SetFormat("HH:mm");
	m_dtcStart4.SetFormat("HH:mm");
	m_dtcStart5.SetFormat("HH:mm");
	m_dtcEnd1.SetFormat("HH:mm");
	m_dtcEnd2.SetFormat("HH:mm");
	m_dtcEnd3.SetFormat("HH:mm");
	m_dtcEnd4.SetFormat("HH:mm");
	m_dtcEnd5.SetFormat("HH:mm");

	int nHours = 0, nMins = 0;
	COleDateTime time;

	m_ServiceTimes.GetStartTime(0, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcStart1.SetTime(time);

	m_ServiceTimes.GetEndTime(0, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcEnd1.SetTime(time);

	m_ServiceTimes.GetStartTime(1, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcStart2.SetTime(time);

	m_ServiceTimes.GetEndTime(1, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcEnd2.SetTime(time);

	m_ServiceTimes.GetStartTime(2, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcStart3.SetTime(time);

	m_ServiceTimes.GetEndTime(2, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcEnd3.SetTime(time);

	m_ServiceTimes.GetStartTime(3, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcStart4.SetTime(time);

	m_ServiceTimes.GetEndTime(3, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcEnd4.SetTime(time);

	m_ServiceTimes.GetStartTime(4, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcStart5.SetTime(time);

	m_ServiceTimes.GetEndTime(4, nHours, nMins);
	time.SetTime(nHours, nMins, 0);
	m_dtcEnd5.SetTime(time);

	SetDisplay();

	return TRUE;
}

//*******************************************************************

void CServiceTimesDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		COleDateTime time;

		m_ServiceTimes.SetReqd(0, (m_bInService1 != 0));
		m_ServiceTimes.SetBalanceOnly(0, (m_bBalanceOnly1 != 0));
		m_dtcStart1.GetTime(time);
		m_ServiceTimes.SetStartTime(0, time.GetHour(), time.GetMinute());
		m_dtcEnd1.GetTime(time);
		m_ServiceTimes.SetEndTime(0, time.GetHour(), time.GetMinute());

		m_ServiceTimes.SetReqd(1, (m_bInService2 != 0));
		m_ServiceTimes.SetBalanceOnly(1, (m_bBalanceOnly2 != 0));
		m_dtcStart2.GetTime(time);
		m_ServiceTimes.SetStartTime(1, time.GetHour(), time.GetMinute());
		m_dtcEnd2.GetTime(time);
		m_ServiceTimes.SetEndTime(1, time.GetHour(), time.GetMinute());

		m_ServiceTimes.SetReqd(2, (m_bInService3 != 0));
		m_ServiceTimes.SetBalanceOnly(2, (m_bBalanceOnly3 != 0));
		m_dtcStart3.GetTime(time);
		m_ServiceTimes.SetStartTime(2, time.GetHour(), time.GetMinute());
		m_dtcEnd3.GetTime(time);
		m_ServiceTimes.SetEndTime(2, time.GetHour(), time.GetMinute());

		m_ServiceTimes.SetReqd(3, (m_bInService4 != 0));
		m_ServiceTimes.SetBalanceOnly(3, (m_bBalanceOnly4 != 0));
		m_dtcStart4.GetTime(time);
		m_ServiceTimes.SetStartTime(3, time.GetHour(), time.GetMinute());
		m_dtcEnd4.GetTime(time);
		m_ServiceTimes.SetEndTime(3, time.GetHour(), time.GetMinute());

		m_ServiceTimes.SetReqd(4, (m_bInService5 != 0));
		m_ServiceTimes.SetBalanceOnly(4, (m_bBalanceOnly5 != 0));
		m_dtcStart5.GetTime(time);
		m_ServiceTimes.SetStartTime(4, time.GetHour(), time.GetMinute());
		m_dtcEnd5.GetTime(time);
		m_ServiceTimes.SetEndTime(4, time.GetHour(), time.GetMinute());

		m_ServiceTimes.Write();

		CDialog::OnOK();
	}
}

//*******************************************************************
void CServiceTimesDlg::OnButtonEnableService1()	{ SetDisplay();	}
void CServiceTimesDlg::OnButtonEnableService2() { SetDisplay(); }
void CServiceTimesDlg::OnButtonEnableService3() { SetDisplay(); }
void CServiceTimesDlg::OnButtonEnableService4() { SetDisplay(); }
void CServiceTimesDlg::OnButtonEnableService5() { SetDisplay(); }
//*******************************************************************

void CServiceTimesDlg::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		bool bEnable = FALSE;

		bEnable = (m_bInService1 != 0);
		m_checkBalanceOnly1.EnableWindow(bEnable);
		m_dtcStart1.EnableWindow(bEnable);
		m_dtcEnd1.EnableWindow(bEnable);

		bEnable = (m_bInService2 != 0);
		m_checkBalanceOnly2.EnableWindow(bEnable);
		m_dtcStart2.EnableWindow(bEnable);
		m_dtcEnd2.EnableWindow(bEnable);

		bEnable = (m_bInService3 != 0);
		m_checkBalanceOnly3.EnableWindow(bEnable);
		m_dtcStart3.EnableWindow(bEnable);
		m_dtcEnd3.EnableWindow(bEnable);

		bEnable = (m_bInService4 != 0);
		m_checkBalanceOnly4.EnableWindow(bEnable);
		m_dtcStart4.EnableWindow(bEnable);
		m_dtcEnd4.EnableWindow(bEnable);

		bEnable = (m_bInService5 != 0);
		m_checkBalanceOnly5.EnableWindow(bEnable);
		m_dtcStart5.EnableWindow(bEnable);
		m_dtcEnd5.EnableWindow(bEnable);
	}
}

//*******************************************************************
