//*******************************************************************
#include "ImageHandler.h"
#include "SystemData.h"
//*******************************************************************
#include "WaitDlg.h"
//*******************************************************************

CWaitDlg::CWaitDlg(const char* szTitle, const char* szPrompt, CWnd* pParent)
	: CSSColourDialog()
{
	m_strTitle = szTitle;
	m_strPrompt = szPrompt;

	if (pParent == NULL)
	{
		pParent = AfxGetMainWnd();
	}

	CSSColourDialog::Create (CWaitDlg::IDD, pParent);
	CDialog::CenterWindow();
}

//*******************************************************************

CWaitDlg::~CWaitDlg()
{
	DestroyWindow();
}

//*******************************************************************

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CWaitDlg, CSSColourDialog)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CWaitDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "WaitBackground");

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

	m_staticTitle.Init(IDC_STATIC_TITLE, this, BLACK, -1, "");
	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "");

	m_staticTitle.SetWindowText(m_strTitle);
	m_staticPrompt.SetWindowText(m_strPrompt);

	return TRUE;
}

//*******************************************************************

void CWaitDlg::PumpMessages()
{
	ASSERT(m_hWnd != NULL);

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

//*******************************************************************
