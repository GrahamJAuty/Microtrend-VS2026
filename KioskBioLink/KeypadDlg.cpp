//*******************************************************************
#include "Cardlink.h"
#include "ImageHandler.h"
#include "SystemData.h"
//*******************************************************************
#include "KeypadDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CKeypadDlg, CDialog)
//*******************************************************************

CKeypadDlg::CKeypadDlg(bool bAdminPassword, CWnd* pParent)
	: CSSColourDialog(CKeypadDlg::IDD, pParent)
{
	m_bForAccountPINNumber = (FALSE == bAdminPassword) && (TRUE == System.GetActualPINModeFlag());
}

//*******************************************************************

CKeypadDlg::~CKeypadDlg()
{
}

//*******************************************************************

void CKeypadDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DISPLAY, m_strDisplay);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_strPrompt);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CKeypadDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, OnButton0)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CKeypadDlg::OnInitDialog()
{
	{
		CString strFilename = "";
		CImageHandler image(System.m_strSysFilesImagePath, "KeypadBackground");

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}
		else
		{
			strFilename = "";
		}

		bool bRounded = (strFilename == "");
		CSSColourDialog::OnInitDialog(MYCOLOUR_DIALOG, bRounded);
		CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "KeypadButton");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	m_buttonOK.Init(IDOK, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonOK.SetImage(strFilename, TRANSPARENT_COLOUR);

	if (TRUE == m_bForAccountPINNumber)
	{
		m_buttonOK.ShowWindow(SW_HIDE);
		m_buttonOK.EnableWindow(FALSE);
	}

	m_buttonCancel.Init(IDCANCEL, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonCancel.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonClear.Init(IDC_BUTTON_CLEAR, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonClear.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button1.Init(IDC_BUTTON_1, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button1.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button2.Init(IDC_BUTTON_2, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button2.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button3.Init(IDC_BUTTON_3, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button3.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button4.Init(IDC_BUTTON_4, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button4.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button5.Init(IDC_BUTTON_5, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button5.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button6.Init(IDC_BUTTON_6, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button6.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button7.Init(IDC_BUTTON_7, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button7.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button8.Init(IDC_BUTTON_8, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button8.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button9.Init(IDC_BUTTON_9, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button9.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_button0.Init(IDC_BUTTON_0, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_button0.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_staticDisplay.Init(IDC_STATIC_DISPLAY, this, BLACK, -1, "");
	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "");

	return TRUE;
}

//*******************************************************************
void CKeypadDlg::OnButton0()	{ AddDigit ( "0" );	}
void CKeypadDlg::OnButton1()	{ AddDigit ( "1" );	}
void CKeypadDlg::OnButton2()	{ AddDigit ( "2" );	}
void CKeypadDlg::OnButton3()	{ AddDigit ( "3" );	}
void CKeypadDlg::OnButton4()	{ AddDigit ( "4" );	}
void CKeypadDlg::OnButton5()	{ AddDigit ( "5" );	}
void CKeypadDlg::OnButton6()	{ AddDigit ( "6" );	}
void CKeypadDlg::OnButton7()	{ AddDigit ( "7" );	}
void CKeypadDlg::OnButton8()	{ AddDigit ( "8" );	}
void CKeypadDlg::OnButton9()	{ AddDigit ( "9" );	}
//*******************************************************************

void CKeypadDlg::AddDigit(CString strNo)
{
	m_strInput += strNo;
	m_strSecretText += strNo;

	if (TRUE == m_bSecretInput)
	{
		m_strDisplay += "*";
	}
	else
	{
		m_strDisplay += strNo;
	}

	UpdateData(FALSE);
	Invalidate();

	if (TRUE == m_bForAccountPINNumber)
	{
		if (m_strInput.GetLength() >= 4 )
		{
			OnOK();
		}
	}
}

//*******************************************************************

void CKeypadDlg::OnButtonClear()
{
	m_strSecretText += "c";
	m_strInput = "";
	m_strDisplay = "";
	UpdateData(FALSE);
	Invalidate();
}

//*******************************************************************

void CKeypadDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strBuf = m_strInput;
		
		if (FALSE == m_bForAccountPINNumber)
		{
			strBuf.TrimLeft('0');
		}

		if (strBuf == "")
		{
			m_strInput = "";
			CSSColourDialog::OnCancel();
		}
		else
		{
			CSSColourDialog::OnOK();
		}
	}
}

//*******************************************************************