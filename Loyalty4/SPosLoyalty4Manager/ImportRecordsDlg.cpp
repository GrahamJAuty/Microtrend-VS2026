//*******************************************************************
#include "LoyaltyManager.h"
#include "ReportCSVFile.h"
//*******************************************************************
#include "ImportRecordsDlg.h"
//*******************************************************************

CImportRecordsDlg::CImportRecordsDlg( const char* szFilename, CWnd* pParent /*=NULL*/)
	: CDialog(CImportRecordsDlg::IDD, pParent)
{
	m_strFilename = szFilename;
	m_pParent = pParent;
	m_nImportMethod = 1;
}

//*******************************************************************

void CImportRecordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CImportRecordsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnBnClickedButtonView)
END_MESSAGE_MAP()

//*******************************************************************

void CImportRecordsDlg::OnBnClickedButtonView()
{
	CCSVFileReport csv ("R117", m_strFilename, m_pParent);
	csv.Show();
}

//*******************************************************************
