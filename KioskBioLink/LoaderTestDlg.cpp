//*******************************************************************
#include "Defines.h"
#include "GlobalFunctions.h"
#include "ImageHandler.h"
#include "PayLinkAuditRecord.h"
#include "SystemData.h"
//*******************************************************************
#include "LoaderTestDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CLoaderTestDlg, CDialog)
//*******************************************************************

CLoaderTestDlg::CLoaderTestDlg(CWnd* pParent)
	: CSSColourDialog(CLoaderTestDlg::IDD, pParent)
{
}

//*******************************************************************

CLoaderTestDlg::~CLoaderTestDlg()
{
}

//*******************************************************************

void CLoaderTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CLoaderTestDlg, CSSColourDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//*******************************************************************

BOOL CLoaderTestDlg::OnInitDialog()
{
	CString strFilename = "";

	{
		CImageHandler image(System.m_strSysFilesImagePath, "EmptyCashBackground");

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}
	}

	bool bRounded = (strFilename == "");

	CSSColourDialog::OnInitDialog(RED, bRounded);
	CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);

	m_staticText2000.Init(IDC_STATIC_TEXT2000, this, BLACK, -1, "");
	m_staticText1000.Init(IDC_STATIC_TEXT1000, this, BLACK, -1, "");
	m_staticText500.Init(IDC_STATIC_TEXT500, this, BLACK, -1, "");
	m_staticText200.Init(IDC_STATIC_TEXT200, this, BLACK, -1, "");
	m_staticText100.Init(IDC_STATIC_TEXT100, this, BLACK, -1, "");
	m_staticText50.Init(IDC_STATIC_TEXT50, this, BLACK, -1, "");
	m_staticText20.Init(IDC_STATIC_TEXT20, this, BLACK, -1, "");
	m_staticText10.Init(IDC_STATIC_TEXT10, this, BLACK, -1, "");

	m_staticQty2000.Init(IDC_STATIC_QTY2000, this, BLACK, -1, "");
	m_staticQty1000.Init(IDC_STATIC_QTY1000, this, BLACK, -1, "");
	m_staticQty500.Init(IDC_STATIC_QTY500, this, BLACK, -1, "");
	m_staticQty200.Init(IDC_STATIC_QTY200, this, BLACK, -1, "");
	m_staticQty100.Init(IDC_STATIC_QTY100, this, BLACK, -1, "");
	m_staticQty50.Init(IDC_STATIC_QTY50, this, BLACK, -1, "");
	m_staticQty20.Init(IDC_STATIC_QTY20, this, BLACK, -1, "");
	m_staticQty10.Init(IDC_STATIC_QTY10, this, BLACK, -1, "");

	m_static2000.Init(IDC_STATIC_2000, this, BLACK, -1, "");
	m_static1000.Init(IDC_STATIC_1000, this, BLACK, -1, "");
	m_static500.Init(IDC_STATIC_500, this, BLACK, -1, "");
	m_static200.Init(IDC_STATIC_200, this, BLACK, -1, "");
	m_static100.Init(IDC_STATIC_100, this, BLACK, -1, "");
	m_static50.Init(IDC_STATIC_50, this, BLACK, -1, "");
	m_static20.Init(IDC_STATIC_20, this, BLACK, -1, "");
	m_static10.Init(IDC_STATIC_10, this, BLACK, -1, "");

	m_staticTotalValue.Init(IDC_STATIC_TOTALVALUE, this, BLACK, -1, "");

	{
		CImageHandler image(System.m_strSysFilesImagePath, "GeneralButton");

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}
		else
		{
			strFilename = "";
		}

		m_buttonOK.Init(IDOK, this, BLACK, MYCOLOUR_BUTTON, "", 14);
		m_buttonOK.SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	SetDisplay();
	StartTimer();
	return TRUE;
}

//*******************************************************************

void CLoaderTestDlg::StartTimer()
{
	m_hTimer = SetTimer(ID_TIMER_LOADERTEST, 500, NULL);
}

//**********************************************************************

void CLoaderTestDlg::StopTimer()
{
	if (m_hTimer != 0)
	{
		KillTimer(m_hTimer);
	}

	m_hTimer = 0;
}

//**********************************************************************

void CLoaderTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		if (FALSE == m_bInTimer)			// prevent re-entrance
		{
			m_bInTimer = TRUE;
			HandleTimer();
			m_bInTimer = FALSE;
		}
	}
}

//*******************************************************************

void CLoaderTestDlg::HandleTimer()
{
	// see if interface need initalising
	if (nSTART_REQD == m_nPayLinkRunning)
	{
		OpenPayLinkInterface();
	}

	if (nSTARTED == m_nPayLinkRunning)
	{
		long lPayLinkValue = m_paylink.GetCurrentValue();		// read current value from PayLink interface

		if (lPayLinkValue != m_lPayLinkCurrentValue)			// see if changed since last time
		{
			long lValueAdded = lPayLinkValue - m_lPayLinkCurrentValue;	// value of coin added
			m_lPayLinkCurrentValue += lValueAdded;

			LogPaylinkCurrentLevel();

			m_nTotalValue += lValueAdded;

			switch (lValueAdded)
			{
			case 2000:
				++m_nQty2000;
				m_nTotal2000 += lValueAdded;
				break;

			case 1000:
				++m_nQty1000;
				m_nTotal1000 += lValueAdded;
				break;

			case 500:
				++m_nQty500;
				m_nTotal500 += lValueAdded;
				break;

			case 200:
				++m_nQty200;
				m_nTotal200 += lValueAdded;
				break;

			case 100:
				++m_nQty100;
				m_nTotal100 += lValueAdded;
				break;

			case 50:
				++m_nQty50;
				m_nTotal50 += lValueAdded;
				break;

			case 20:
				++m_nQty20;
				m_nTotal20 += lValueAdded;
				break;

			case 10:
				++m_nQty10;
				m_nTotal10 += lValueAdded;
				break;
			}

			SetDisplay();
			Invalidate();
		}
	}
}

//*******************************************************************

void CLoaderTestDlg::OpenPayLinkInterface()
{
	if (m_paylink.Enable(this) == TRUE)							// enable Paylink interface box
	{
		m_lPayLinkCurrentValue = m_paylink.GetCurrentValue();	// save paylin StartLevel
		m_nPayLinkRunning = nSTARTED;						// set PayLink interface running
	}
	else
	{
		::DisplayMessage(this, m_paylink.GetError(), "PayLink Initialization FAILED", MB_ICONSTOP);
		m_nPayLinkRunning = nSTART_FAILED;
	}
}

//*******************************************************************

void CLoaderTestDlg::SetDisplay()
{
	CString strBuf = "";

	strBuf.Format("£ %.2lf", (double)m_nTotal2000 / 100);
	m_static2000.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal1000 / 100);
	m_static1000.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal500 / 100);
	m_static500.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal200 / 100);
	m_static200.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal100 / 100);
	m_static100.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal50 / 100);
	m_static50.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal20 / 100);
	m_static20.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotal10 / 100);
	m_static10.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", (double)m_nTotalValue / 100);
	m_staticTotalValue.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty2000);
	m_staticQty2000.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty1000);
	m_staticQty1000.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty500);
	m_staticQty500.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty200);
	m_staticQty200.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty100);
	m_staticQty100.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty50);
	m_staticQty50.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty20);
	m_staticQty20.SetWindowText(strBuf);

	strBuf.Format("%d", m_nQty10);
	m_staticQty10.SetWindowText(strBuf);
}

//*************************************************************;
// Update current Paylink Current Value level, used on checking for startup differs

void CLoaderTestDlg::LogPaylinkCurrentLevel()
{
	CSSFile file;
	if (file.Open(System.m_strPayLinkLevelFilename, "wb") == TRUE)	// save current Paylink Current Value
	{
		CPayLinkAuditRecord	audit;										// used on checking for startup differs
		audit.SetText("Kiosk Test");									// last person to use kiosk
		audit.SetCoinageAdded(m_lPayLinkCurrentValue);				// current PayLink EndLevel in pence
		audit.SaveLine(&file);
		file.Close();
	}
}

//**********************************************************************

void CLoaderTestDlg::OnOK()
{
	StopTimer();

	if (nSTARTED == m_nPayLinkRunning)
	{
		m_paylink.Disable();
	}

	CDialog::OnOK();
}

//**********************************************************************