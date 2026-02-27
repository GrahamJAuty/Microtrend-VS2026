//*********************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*********************************************************************
#include "SelectPurseDlg.h"
//*********************************************************************
IMPLEMENT_DYNAMIC(CSelectPurseDlg, CDialog)
//*********************************************************************

CSelectPurseDlg::CSelectPurseDlg(CWnd* pParent)
	: CSSColourDialog(CSelectPurseDlg::IDD, pParent)
{
}

//*********************************************************************

CSelectPurseDlg::~CSelectPurseDlg()
{
}

//*********************************************************************

void CSelectPurseDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*********************************************************************

BEGIN_MESSAGE_MAP(CSelectPurseDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_TOPUP, OnButtonTopup1)
	ON_BN_CLICKED(IDC_BUTTON_TOPUP2, OnButtonTopup2)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSelectPurseDlg::OnInitDialog()
{
	CString strFilename = "";

	{
		CImageHandler image(System.m_strSysFilesImagePath, "SelectPurseBackground");

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}

		bool bRounded = (strFilename == "");

		CSSColourDialog::OnInitDialog(BLACK, bRounded);
		CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	CImageHandler image(System.m_strSysFilesImagePath, "PurseButton");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, WHITE, -1, "");

	m_buttonTopup1.Init(IDC_BUTTON_TOPUP, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonTopup1.SetImage(strFilename, TRANSPARENT_COLOUR);
	m_buttonTopup1.SetWindowText(System.m_strPurse1Text);

	m_buttonTopup2.Init(IDC_BUTTON_TOPUP2, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonTopup2.SetImage(strFilename, TRANSPARENT_COLOUR);
	m_buttonTopup2.SetWindowText(System.m_strPurse3Text);

	m_buttonCancel.Init(IDCANCEL, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonCancel.SetImage(strFilename, TRANSPARENT_COLOUR);

	return TRUE;
}

//*******************************************************************

void CSelectPurseDlg::OnButtonTopup1()
{
	m_nTopupPurse = 1;
	CDialog::OnOK();
}

//*******************************************************************

void CSelectPurseDlg::OnButtonTopup2()
{
	m_nTopupPurse = 3;
	CDialog::OnOK();
}

//*******************************************************************
