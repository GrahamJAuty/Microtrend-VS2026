//*******************************************************************
#include "SetupFieldsDlg.h"
//*******************************************************************
#define nMAX_COUNT			9
//*******************************************************************

CSetupFieldsDlg::CSetupFieldsDlg( const char* szFilename, const char* szCaption, const char* szDefault, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupFieldsDlg::IDD, pParent)
	, m_strFilename(szFilename)
	, m_strField1(_T(""))
	, m_strField2(_T(""))
	, m_strField3(_T(""))
	, m_strField4(_T(""))
	, m_strField5(_T(""))
	, m_strField6(_T(""))
	, m_strField7(_T(""))
	, m_strField8(_T(""))
	, m_strField9(_T(""))
{
	m_strCaption = szCaption;
	m_strDefault = szDefault;

	m_bOpenFileError = FALSE;
	if ( m_csvarray.Open ( m_strFilename, DB_READWRITE ) != DB_ERR_SHARE )
	{
		for ( int i = m_csvarray.GetSize() ; i < nMAX_COUNT ; i++ )	// make up shortfall
			m_csvarray.Add ( "" );
	}
	else
		m_bOpenFileError = TRUE;
}

//*******************************************************************

void CSetupFieldsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIELD1, m_strField1);
	DDX_Text(pDX, IDC_EDIT_FIELD2, m_strField2);
	DDX_Text(pDX, IDC_EDIT_FIELD3, m_strField3);
	DDX_Text(pDX, IDC_EDIT_FIELD4, m_strField4);
	DDX_Text(pDX, IDC_EDIT_FIELD5, m_strField5);
	DDX_Text(pDX, IDC_EDIT_FIELD6, m_strField6);
	DDX_Text(pDX, IDC_EDIT_FIELD7, m_strField7);
	DDX_Text(pDX, IDC_EDIT_FIELD8, m_strField8);
	DDX_Text(pDX, IDC_EDIT_FIELD9, m_strField9);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupFieldsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//*******************************************************************
// CSetupFieldsDlg message handlers

void CSetupFieldsDlg::CSSAutoShutdownPreInitDialog()
{
	SetData();
}

//*******************************************************************

BOOL CSetupFieldsDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strCaption);
	return TRUE;
}

//*******************************************************************

void CSetupFieldsDlg::SetData()
{
	m_strField1 = m_csvarray.GetAt(0);
	m_strField2 = m_csvarray.GetAt(1);
	m_strField3 = m_csvarray.GetAt(2);
	m_strField4 = m_csvarray.GetAt(3);
	m_strField5 = m_csvarray.GetAt(4);
	m_strField6 = m_csvarray.GetAt(5);
	m_strField7 = m_csvarray.GetAt(6);
	m_strField8 = m_csvarray.GetAt(7);
	m_strField9 = m_csvarray.GetAt(8);
}

//*******************************************************************

void CSetupFieldsDlg::OnBnClickedButtonDefault()
{
	for ( int i = 0 ; i < nMAX_COUNT ; i++ )
	{
		CString strText;
		strText.Format ( "%s %d", m_strDefault, i+1 );
		m_csvarray.SetAt ( i, strText );
	}

	SetData();
	UpdateData(FALSE);
}

//*******************************************************************

void CSetupFieldsDlg::OnBnClickedButtonClear()
{
	for ( int i = 0 ; i < nMAX_COUNT ; i++ )
		m_csvarray.SetAt ( i, "" );

	SetData();
	UpdateData(FALSE);
}

//*******************************************************************

void CSetupFieldsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_csvarray.SetAt(0, m_strField1);
		m_csvarray.SetAt(1, m_strField2);
		m_csvarray.SetAt(2, m_strField3);
		m_csvarray.SetAt(3, m_strField4);
		m_csvarray.SetAt(4, m_strField5);
		m_csvarray.SetAt(5, m_strField6);
		m_csvarray.SetAt(6, m_strField7);
		m_csvarray.SetAt(7, m_strField8);
		m_csvarray.SetAt(8, m_strField9);

		bool bDelete = TRUE;
		for (int i = 0; i < nMAX_COUNT; i++)
		{
			CString strText = m_csvarray.GetAt(i);
			if (strText != "")
			{
				bDelete = FALSE;
				break;
			}
		}

		m_csvarray.Close();
		if (bDelete == TRUE)
			CFileRecycler::SendToRecycleBin(m_strFilename);

		EndDialog(IDOK);
	}
}

//*******************************************************************

void CSetupFieldsDlg::OnCancel()
{
	m_csvarray.SetModifiedFlag(FALSE);
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CSetupFieldsDlg::CSSAutoShutdownLogOff()
{
	OnCancel();
}

//*******************************************************************
