// EditCustomListDlg.cpp : implementation file
//

#include "EditCustomListDlg.h"


// CEditCustomListDlg dialog

CEditCustomListDlg::CEditCustomListDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CEditCustomListDlg::IDD, pParent)
	, m_strCustomField(_T(""))
	, m_strFieldName(_T(""))
{

}

CEditCustomListDlg::~CEditCustomListDlg()
{
}

void CEditCustomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CUSTOMFIELD, m_strCustomField);
	DDX_Text(pDX, IDC_STATIC_FIELDNAME, m_strFieldName);
}


BEGIN_MESSAGE_MAP(CEditCustomListDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()


// CEditCustomListDlg message handlers
