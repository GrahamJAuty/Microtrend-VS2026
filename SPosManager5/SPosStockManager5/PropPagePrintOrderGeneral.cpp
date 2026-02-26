/**********************************************************************/
 
/**********************************************************************/
#include "MaxLengths.h"
 
/**********************************************************************/
#include "PropPagePrintOrderGeneral.h"
/**********************************************************************/

CPropPagePrintOrderGeneral::CPropPagePrintOrderGeneral() : CPropertyPage(CPropPagePrintOrderGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePrintOrderGeneral)
	m_strLine1 = _T("");
	m_strLine2 = _T("");
	m_strLine3 = _T("");
	m_strLine4 = _T("");
	m_strLine5 = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPropPagePrintOrderGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePrintOrderGeneral)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LINE1, m_strLine1);
	DDV_MaxChars(pDX, m_strLine1, MAX_LENGTH_INSTRUCTIONS);
	DDX_Text(pDX, IDC_EDIT_LINE2, m_strLine2);
	DDV_MaxChars(pDX, m_strLine2, MAX_LENGTH_INSTRUCTIONS);
	DDX_Text(pDX, IDC_EDIT_LINE3, m_strLine3);
	DDV_MaxChars(pDX, m_strLine3, MAX_LENGTH_INSTRUCTIONS);
	DDX_Text(pDX, IDC_EDIT_LINE4, m_strLine4);
	DDV_MaxChars(pDX, m_strLine4, MAX_LENGTH_INSTRUCTIONS);
	DDX_Text(pDX, IDC_EDIT_LINE5, m_strLine5);
	DDV_MaxChars(pDX, m_strLine5, MAX_LENGTH_INSTRUCTIONS);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePrintOrderGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePrintOrderGeneral)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePrintOrderGeneral::OnInitDialog() 
{
	RestoreDefaults();
	CPropertyPage::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePrintOrderGeneral::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePrintOrderGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	m_pOrderDetails -> m_strInstructions1 = m_strLine1;
	m_pOrderDetails -> m_strInstructions2 = m_strLine2;
	m_pOrderDetails -> m_strInstructions3 = m_strLine3;
	m_pOrderDetails -> m_strInstructions4 = m_strLine4;
	m_pOrderDetails -> m_strInstructions5 = m_strLine5;
	return bResult;
}

/**********************************************************************/

void CPropPagePrintOrderGeneral::OnButtonClear() 
{
	CString strMsg = "Are you sure you wish to clear the delivery instructions.";
	if ( Prompter.YesNo ( strMsg ) == IDYES )
	{
		m_strLine1 = "";
		m_strLine2 = "";
		m_strLine3 = "";
		m_strLine4 = "";
		m_strLine5 = "";
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CPropPagePrintOrderGeneral::OnButtonRestore() 
{
	RestoreDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPagePrintOrderGeneral::RestoreDefaults() 
{
	m_pOrderDetails -> ReadDeliveryInstructions();
	m_strLine1 = m_pOrderDetails -> m_strInstructions1;
	m_strLine2 = m_pOrderDetails -> m_strInstructions2;
	m_strLine3 = m_pOrderDetails -> m_strInstructions3;
	m_strLine4 = m_pOrderDetails -> m_strInstructions4;
	m_strLine5 = m_pOrderDetails -> m_strInstructions5;
}

/**********************************************************************/

void CPropPagePrintOrderGeneral::OnButtonSave() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_pOrderDetails -> m_strInstructions1 = m_strLine1;
		m_pOrderDetails -> m_strInstructions2 = m_strLine2;
		m_pOrderDetails -> m_strInstructions3 = m_strLine3;
		m_pOrderDetails -> m_strInstructions4 = m_strLine4;
		m_pOrderDetails -> m_strInstructions5 = m_strLine5;
		m_pOrderDetails -> WriteDeliveryInstructions();
	}
}

/**********************************************************************/
