//$$******************************************************************
#include "AllergiesDlg.h"
//$$******************************************************************

CAllergiesDlg::CAllergiesDlg( const char* szUserID, int nAllergyCodes, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAllergiesDlg::IDD, pParent)
{
	m_strUserID = szUserID;

	m_AllergyText.Read();
	m_AllergyText.SetAllergyCodes ( nAllergyCodes );
}

//$$******************************************************************

void CAllergiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ALLERGY1, m_checkAllergen[1]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY2, m_checkAllergen[2]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY3, m_checkAllergen[3]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY4, m_checkAllergen[4]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY5, m_checkAllergen[5]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY6, m_checkAllergen[6]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY7, m_checkAllergen[7]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY8, m_checkAllergen[8]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY9, m_checkAllergen[9]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY10, m_checkAllergen[10]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY11, m_checkAllergen[11]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY12, m_checkAllergen[12]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY13, m_checkAllergen[13]);
	DDX_Control(pDX, IDC_CHECK_ALLERGY14, m_checkAllergen[14]);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAllergiesDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAllergiesDlg::CSSAutoShutdownPostInitDialog()
{
	{
		CString strCaption = "";

		if (m_strUserID == "")
		{
			strCaption = "Select Allergies";
		}
		else
		{
			strCaption.Format("Select Allergies for User %s", (const char*)m_strUserID);
		}

		SetWindowText(strCaption);
	}

	for (int n = 1; n <= 14; n++)
	{
		m_checkAllergen[n].SetCheck(m_AllergyText.HaveAllergy(n));
		SetCheckboxText(n);
	}

	return TRUE;
}

//$$******************************************************************

void CAllergiesDlg::SetCheckboxText(int nCode)
{
	CString strText = "";

	if (m_AllergyText.FindRecord(nCode) == TRUE)
	{
		strText = m_AllergyText.GetText();
	}

	int nPtr = 0;
	int nPos = 0;
	while ((nPtr = strText.Find('&', nPos)) != -1)
	{
		strText.Insert(++nPtr, '&');
		nPos = ++nPtr;
	}

	m_checkAllergen[nCode].SetWindowText(strText);
	m_checkAllergen[nCode].EnableWindow(strText != "");
}

//$$******************************************************************

void CAllergiesDlg::OnButtonClear()
{
	for (int n = 1; n <= 14; n++)
	{
		m_checkAllergen[n].SetCheck(FALSE);
	}
}

//**************************************************************

void CAllergiesDlg::OnOK()
{
	for (int n = 1; n <= 14; n++)
	{
		m_AllergyText.SetAllergy(n, IsTicked(m_checkAllergen[n]));
	}

	EndDialog(IDOK);
}

//$$******************************************************************

