//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRecordSetAuditPeriod.h"
//*******************************************************************
#include "SetupAuditPeriodsDlg.h"
//*******************************************************************

CSetupAuditPeriodsDlg::CSetupAuditPeriodsDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupAuditPeriodsDlg::IDD, pParent)
{
	SQLRowSetAuditPeriod.LoadPeriods();
}

//*******************************************************************

CSetupAuditPeriodsDlg::~CSetupAuditPeriodsDlg()
{
}

//*******************************************************************

void CSetupAuditPeriodsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcStartPeriod[0]);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtcStartPeriod[1]);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dtcStartPeriod[2]);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dtcStartPeriod[3]);
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_dtcStartPeriod[4]);
	DDX_Control(pDX, IDC_DATETIMEPICKER6, m_dtcStartPeriod[5]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT1, m_editPeriodText[0]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT2, m_editPeriodText[1]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT3, m_editPeriodText[2]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT4, m_editPeriodText[3]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT5, m_editPeriodText[4]);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT6, m_editPeriodText[5]);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CSetupAuditPeriodsDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CSetupAuditPeriodsDlg::CSSAutoShutdownPostInitDialog()
{
	for (int n = 0; n < 6; n++)
	{
		m_editPeriodText[n].LimitText(AuditPeriod::PeriodName.Max);
		m_dtcStartPeriod[n].SetFormat("HH:mm");
	}

	SetWindowText("Setup Daily Audit Periods");

	COleDateTime time;
	for (int n = 0; n < nMAXPERIODS; n++)
	{
		int nStartTime = SQLRowSetAuditPeriod.GetStartTime(n);
		time.SetDateTime(2022, 1, 1, nStartTime / 60, nStartTime % 60, 0);
		m_dtcStartPeriod[n].SetTime(time);
		m_editPeriodText[n].SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(n));
	}

	return TRUE;
}

//*******************************************************************

void CSetupAuditPeriodsDlg::OnOK()
{
	COleDateTime time;

	for (int n = 0; n < 6; n++)
	{
		CString strText = GetEditBoxText(m_editPeriodText[n]);

		if (strText == "")
		{
			SQLRowSetAuditPeriod.SetPeriodName(n, "");
			SQLRowSetAuditPeriod.SetStartTime(n, 0);
		}
		else
		{
			m_dtcStartPeriod[n].GetTime(time);
			SQLRowSetAuditPeriod.SetPeriodName(n, strText);
			SQLRowSetAuditPeriod.SetStartTime(n, (time.GetHour() * 60) + time.GetMinute());
		}
	}
	
	int nLastTime = 0;
	for (int n = 0; n < 6; n++)
	{
		int nStartTime = SQLRowSetAuditPeriod.GetStartTime(n);

		if (nStartTime > 0)
		{
			if (nStartTime < nLastTime)
			{
				MessageBeep(MB_ICONEXCLAMATION);
				Prompter.Error("Invalid start time specifed", "Period Setup");
				m_dtcStartPeriod[n].SetFocus();
				return;
			}

			nLastTime = nStartTime;
		}
	}

	SQLRowSetAuditPeriod.SavePeriods();

	CSSFile file;
	if (file.Open(Filenames.GetAuditPeriodTokenFilename(), "wb"))
	{
		file.WriteLine("SPOS");
	}

	EndDialog(IDOK);
}

//*******************************************************************
