//****************************************************************************
#include "ImageHandler.h"
#include "SystemData.h"
//****************************************************************************
#include "EmptyCashDlg.h"
//****************************************************************************
IMPLEMENT_DYNAMIC(CEmptyCashDlg, CDialog)
//****************************************************************************

CEmptyCashDlg::CEmptyCashDlg( CGTData* pGTData, CWnd* pParent)
	: CSSColourDialog(CEmptyCashDlg::IDD, pParent)
	, m_pGTData(pGTData)
{
}

//****************************************************************************

CEmptyCashDlg::~CEmptyCashDlg()
{
}

//****************************************************************************

void CEmptyCashDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//****************************************************************************

BEGIN_MESSAGE_MAP(CEmptyCashDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnButtonNo)
END_MESSAGE_MAP()

//*************************************************************************

BOOL CEmptyCashDlg::OnInitDialog()
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

	CSSColourDialog::OnInitDialog(MYCOLOUR_DIALOG, bRounded);
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
	m_staticTotalText.Init(IDC_STATIC_TOTALTEXT, this, BLACK, -1, "");

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "");

	CImageHandler image(System.m_strSysFilesImagePath, "EmptyCashButton");

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	m_buttonYes.Init(IDC_BUTTON_YES, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonYes.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonNo.Init(IDC_BUTTON_NO, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonNo.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_static2000.SetWindowText(m_pGTData->GetValue2000());
	m_static1000.SetWindowText(m_pGTData->GetValue1000());
	m_static500.SetWindowText(m_pGTData->GetValue500());
	m_static200.SetWindowText(m_pGTData->GetValue200());
	m_static100.SetWindowText(m_pGTData->GetValue100());
	m_static50.SetWindowText(m_pGTData->GetValue50());
	m_static20.SetWindowText(m_pGTData->GetValue20());
	m_static10.SetWindowText(m_pGTData->GetValue10());

	m_staticTotalValue.SetWindowText(m_pGTData->GetCIDValue());

	CString strBuf = "";
	strBuf.Format("%d", m_pGTData->GetQty2000());
	m_staticQty2000.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty1000());
	m_staticQty1000.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty500());
	m_staticQty500.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty200());
	m_staticQty200.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty100());
	m_staticQty100.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty50());
	m_staticQty50.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty20());
	m_staticQty20.SetWindowText(strBuf);

	strBuf.Format("%d", m_pGTData->GetQty10());
	m_staticQty10.SetWindowText(strBuf);

	return TRUE;
}

//*************************************************************

void CEmptyCashDlg::OnButtonYes()
{
	m_nReply = IDYES;
	CDialog::OnOK();
}

//*************************************************************

void CEmptyCashDlg::OnButtonNo()
{
	m_nReply = IDNO;
	CDialog::OnOK();
}

//*************************************************************
