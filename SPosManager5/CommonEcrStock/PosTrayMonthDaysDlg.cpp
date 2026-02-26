/**********************************************************************/
#include "PosTrayMonthDaysDlg.h"
/**********************************************************************/

CPosTrayMonthDaysDlg::CPosTrayMonthDaysDlg( CWnd* pParent )
	: CSSDialog(CPosTrayMonthDaysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPosTrayMonthDaysDlg)
	//}}AFX_DATA_INIT
	m_nMonthDays = 0;
}

/**********************************************************************/

void CPosTrayMonthDaysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayMonthDaysDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_MONTHDAY1, m_checkMonthDays[0]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY2, m_checkMonthDays[1]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY3, m_checkMonthDays[2]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY4, m_checkMonthDays[3]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY5, m_checkMonthDays[4]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY6, m_checkMonthDays[5]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY7, m_checkMonthDays[6]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY8, m_checkMonthDays[7]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY9, m_checkMonthDays[8]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY10, m_checkMonthDays[9]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY11, m_checkMonthDays[10]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY12, m_checkMonthDays[11]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY13, m_checkMonthDays[12]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY14, m_checkMonthDays[13]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY15, m_checkMonthDays[14]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY16, m_checkMonthDays[15]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY17, m_checkMonthDays[16]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY18, m_checkMonthDays[17]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY19, m_checkMonthDays[18]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY20, m_checkMonthDays[19]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY21, m_checkMonthDays[20]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY22, m_checkMonthDays[21]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY23, m_checkMonthDays[22]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY24, m_checkMonthDays[23]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY25, m_checkMonthDays[24]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY26, m_checkMonthDays[25]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY27, m_checkMonthDays[26]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY28, m_checkMonthDays[27]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY29, m_checkMonthDays[28]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY30, m_checkMonthDays[29]);
	DDX_Control(pDX, IDC_CHECK_MONTHDAY31, m_checkMonthDays[30]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayMonthDaysDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayMonthDaysDlg)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayMonthDaysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	int nMask = 1;

	for (int i = 0; i < 31; i++)
	{
		m_checkMonthDays[i].SetCheck((m_nMonthDays & nMask) != 0);
		nMask <<= 1;
	}

	return TRUE;  
}

/**********************************************************************/

void CPosTrayMonthDaysDlg::OnButtonAll()
{
	for (int i = 0; i < 31; i++)
	{
		m_checkMonthDays[i].SetCheck(TRUE);
	}
}

/**********************************************************************/

void CPosTrayMonthDaysDlg::OnButtonFirst()
{
	for (int i = 0; i < 31; i++)
	{
		m_checkMonthDays[i].SetCheck(0 == i);
	}
}

/**********************************************************************/

void CPosTrayMonthDaysDlg::OnOK() 
{
	m_nMonthDays = 0;

	int nMask = 1;

	for (int i = 0; i < 31; i++)
	{
		if (m_checkMonthDays[i].GetCheck())
		{
			m_nMonthDays |= nMask;
		}

		nMask <<= 1;
	}	

	if (0 == m_nMonthDays)
	{
		Prompter.Error("You must select at least one day of the month.");
		return;
	}

	EndDialog(IDOK);
}

/**********************************************************************/

