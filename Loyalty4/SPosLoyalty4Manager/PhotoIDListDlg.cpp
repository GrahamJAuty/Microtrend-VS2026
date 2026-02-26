// PhotoIDListDlg.cpp : implementation file

#include "LoyaltyManager.h"
#include "PhotoIDListData.h"
#include "PhotoIDListDlg.h"

//*******************************************************************
// CPhotoIDListDlg dialog

CPhotoIDListDlg::CPhotoIDListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoIDListDlg::IDD, pParent)
	, m_bSkipNoPhoto(TRUE)
	, m_strLabelName(_T(""))
{
	m_bPrintReqd = FALSE;
}

void CPhotoIDListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_buttonPrint);
	DDX_Control(pDX, IDC_COMBO_LABELS, m_comboLabel);
	DDX_Check(pDX, IDC_CHECK_ZEROSKIP, m_bSkipNoPhoto);
	DDX_CBString(pDX, IDC_COMBO_LABELS, m_strLabelName);
}


BEGIN_MESSAGE_MAP(CPhotoIDListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnBnClickedButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_CBN_SELCHANGE(IDC_COMBO_LABELS, OnSelchangeComboLabels)
END_MESSAGE_MAP()


// CPhotoIDListDlg message handlers

BOOL CPhotoIDListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CPhotoIDListData data;

	switch ( data.CompileLabelCombo ( &m_comboLabel, FALSE ) )
	{
	case 0:		m_buttonPrint.EnableWindow(FALSE);
				m_buttonDisplay.EnableWindow(FALSE);
	case 1:		m_comboLabel.EnableWindow(FALSE);
	default:	break;
	}
	
	return TRUE;
}

//*******************************************************************

void CPhotoIDListDlg::OnBnClickedButtonDisplay()
{
	if ( UpdateData() == TRUE )
	{
		m_bPrintReqd = FALSE;
		CDialog::OnOK();
	}
}

void CPhotoIDListDlg::OnBnClickedButtonPrint()
{
	if ( UpdateData() == TRUE )
	{
		m_bPrintReqd = TRUE;
		CDialog::OnOK();
	}
}

void CPhotoIDListDlg::OnSelchangeComboLabels() 
{
	if ( UpdateData() == TRUE )
	{
		CPhotoIDListData data;
		data.MakeDefault ( m_strLabelName );
	}
}

//*******************************************************************
