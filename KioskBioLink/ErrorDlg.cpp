//****************************************************************************
#include "CardLink.h"
#include "SystemData.h"
#include "ImageHandler.h"
//****************************************************************************
#include "ErrorDlg.h"
//****************************************************************************

CErrorDlg::CErrorDlg(const char* szTitle, const char* szPrompt, CWnd* pParent)
	: CSSColourDialog(CErrorDlg::IDD, pParent)
{
	m_strTitle = szTitle;
	m_strPrompt = szPrompt;
}

//****************************************************************************

CErrorDlg::~CErrorDlg()
{
}

//****************************************************************************

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//****************************************************************************
BEGIN_MESSAGE_MAP(CErrorDlg, CSSColourDialog)
END_MESSAGE_MAP()
//****************************************************************************

BOOL CErrorDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "ErrorScreen");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}

	bool bRounded = (strFilename == "");

	CSSColourDialog::OnInitDialog(RED, bRounded);
	CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);

	m_staticTitle.Init(IDC_STATIC_TITLE, this, BLACK, -1, "");
	m_staticTitle.SetWindowText(m_strTitle);

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "");
	m_staticPrompt.SetWindowText(m_strPrompt);

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

		m_buttonClose.Init(IDOK, this, BLACK, MYCOLOUR_BUTTON, "", 20);
		m_buttonClose.SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	return TRUE;
}

//****************************************************************************
