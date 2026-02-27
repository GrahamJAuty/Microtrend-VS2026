//$$******************************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\BackgroundTimeTable.h"
//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "BackgroundTimetableDlg.h"
//$$******************************************************************

CBackgroundTimetableDlg::CBackgroundTimetableDlg(CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CBackgroundTimetableDlg::IDD, pParent)
{
}

//$$******************************************************************

void CBackgroundTimetableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME2, m_editTime[2]);
	DDX_Control(pDX, IDC_EDIT_TIME3, m_editTime[3]);
	DDX_Control(pDX, IDC_EDIT_TIME4, m_editTime[4]);
	DDX_Control(pDX, IDC_EDIT_TIME5, m_editTime[5]);
	DDX_Control(pDX, IDC_EDIT_TIME6, m_editTime[6]);
	DDX_Control(pDX, IDC_EDIT_TIME7, m_editTime[7]);
	DDX_Control(pDX, IDC_EDIT_TIME8, m_editTime[8]);
	DDX_Control(pDX, IDC_EDIT_TIME9, m_editTime[9]);
	DDX_Control(pDX, IDC_EDIT_TIME10, m_editTime[10]);
	DDX_Control(pDX, IDC_EDIT_DATE4, m_editDate4);
	DDX_Control(pDX, IDC_EDIT_STARTOFWEEK6, m_editWeekday6);
	DDX_Control(pDX, IDC_EDIT_STARTOFWEEK9, m_editWeekday9);
	DDX_Control(pDX, IDC_EDIT_STARTOFWEEK10, m_editWeekday10);
	DDX_Control(pDX, IDC_STATIC_WEBPAYMENT_MESSAGE, m_staticWebPaymentMessage);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CBackgroundTimetableDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CBackgroundTimetableDlg::CSSAutoShutdownPostInitDialog()
{
	int nPurchaseDay = 0;
	int nWeeklyPointsDay = 0;
	int nAccountLinkDay = 0;

	CBackgroundTimeTable timetable;
	if (timetable.ReadTimetable() == TRUE)
	{
		for (int n = 2; n <= 10; n++)
		{
			CString strTime = timetable.GetTime(n);

			if (strTime == "")
			{
				strTime = "not set";
			}

			m_editTime[n].SetWindowText(strTime);
		}

		nPurchaseDay = timetable.GetPurchaseDay();
		nWeeklyPointsDay = timetable.GetWeeklyPointsDay();
		nAccountLinkDay = timetable.GetAccountLinkDay();
	}

	CBackgroundOptions options;									// get background options
	options.Read();
	
	CString strText = timetable.GetTime(4);
	if (strText == "")
	{
		ShowAndEnableWindow(&m_editDate4, FALSE);
	}
	else
	{
		m_editDate4.SetWindowText(options.m_strDateNextEODCheck);
	}

	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BACKGROUND), TRUE); UpdateWeekdayBox(timetable.GetTime(6), m_editWeekday6, nPurchaseDay);

	UpdateWeekdayBox(timetable.GetTime(6), m_editWeekday6, nPurchaseDay);
	UpdateWeekdayBox(timetable.GetTime(9), m_editWeekday9, nWeeklyPointsDay);
	UpdateWeekdayBox(timetable.GetTime(10), m_editWeekday10, nAccountLinkDay);	

	if (::FileExists(Filenames.GetWebPaymentStopIndicator()) == TRUE)
	{
		m_staticWebPaymentMessage.SetWindowText("SUSPENDED");
	}
	else
	{
		m_staticWebPaymentMessage.SetWindowText("");
	}

	return TRUE;
}

//$$******************************************************************

CString CBackgroundTimetableDlg::GetWeekdayString(int nWeekday)
{
	CString strWeekday = "";

	switch (nWeekday)
	{
	case 0:
		strWeekday = "Sunday";
		break;

	case 1:
		strWeekday = "Monday";
		break;

	case 2:
		strWeekday = "Tuesday";
		break;

	case 3:
		strWeekday = "Wednesday";
		break;

	case 4:
		strWeekday = "Thursday";
		break;

	case 5:
		strWeekday = "Friday";
		break;

	case 6:
		strWeekday = "Saturday";
		break;

	default:
		break;
	}

	return strWeekday;
}

//$$******************************************************************

void CBackgroundTimetableDlg::UpdateWeekdayBox(CString strTime, CEdit& editBox, int nDay)
{
	if (strTime == "")
	{
		ShowAndEnableWindow(&editBox, FALSE);
	}
	else
	{
		editBox.SetWindowText(GetWeekdayString(nDay));
	}
}

//$$******************************************************************
