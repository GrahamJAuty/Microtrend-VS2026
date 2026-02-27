// ReadCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadCardDlg.h"


// CReadCardDlg dialog

IMPLEMENT_DYNAMIC(CReadCardDlg, CDialog)

CReadCardDlg::CReadCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadCardDlg::IDD, pParent)
	, m_staticPrompt(_T(""))
{

}

CReadCardDlg::~CReadCardDlg()
{
}

void CReadCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
}


BEGIN_MESSAGE_MAP(CReadCardDlg, CDialog)
END_MESSAGE_MAP()


// CReadCardDlg message handlers
