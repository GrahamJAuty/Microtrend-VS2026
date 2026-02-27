//*******************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*******************************************************************
#include "YesNoDlg.h"
//*******************************************************************

CYesNoDlg::CYesNoDlg(const char* szPrompt, CWnd* pParent)
	: CSSColourDialog(CYesNoDlg::IDD, pParent)
{
	m_strPrompt = szPrompt;
	m_nReply = IDNO;
}

//*******************************************************************

void CYesNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CYesNoDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnButtonNo)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CYesNoDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "YesNoBackground");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}

	bool bRounded = (strFilename == "");

	CSSColourDialog::OnInitDialog(RED, bRounded);
	CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonYes.Init(IDC_BUTTON_YES, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonYes.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonNo.Init(IDC_BUTTON_NO, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonNo.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "");
	m_staticPrompt.SetWindowText(m_strPrompt);

	return TRUE;
}

//*******************************************************************

void CYesNoDlg::OnButtonYes()
{
	m_nReply = IDYES;
	CDialog::OnOK();
}

//*******************************************************************

void CYesNoDlg::OnButtonNo()
{
	m_nReply = IDNO;
	CDialog::OnOK();
}

//*******************************************************************
