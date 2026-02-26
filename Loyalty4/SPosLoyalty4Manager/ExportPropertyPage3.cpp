//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "LoyaltyManager.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ExportPropertyPage3.h"
//$$******************************************************************

CExportPropertyPage3::CExportPropertyPage3() : CSSPropertyPage(CExportPropertyPage3::IDD)
{
	m_dValue1 = 0.0;
	m_dValue2 = 0.0;
	m_pDbExportHandler = NULL;
}

//$$******************************************************************

CExportPropertyPage3::~CExportPropertyPage3()
{
}

//$$******************************************************************

void CExportPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AND, m_comboAnd);
	DDX_Control(pDX, IDC_COMBO_CONDITION1, m_comboCondition1);
	DDX_Control(pDX, IDC_COMBO_CONDITION2, m_comboCondition2);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboField2);
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboField1);
	DDX_CBString(pDX, IDC_COMBO_FIELD1, m_strField1);
	DDX_CBString(pDX, IDC_COMBO_FIELD2, m_strField2);
	DDX_CBString(pDX, IDC_COMBO_CONDITION1, m_strCondition1);
	DDX_CBString(pDX, IDC_COMBO_CONDITION2, m_strCondition2);
	DDX_CBString(pDX, IDC_COMBO_AND, m_strAndOr);

	DDX_Text(pDX, IDC_EDIT_VALUE1, m_dValue1 );
	DDV_MinMaxDouble(pDX, m_dValue1, Account::Purse1Balance.Min, Account::Purse1Balance.Max );
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_dValue2 );
	DDV_MinMaxDouble(pDX, m_dValue2, Account::Purse1Balance.Min, Account::Purse1Balance.Max );
}

//$$******************************************************************

void CExportPropertyPage3::SetPointer(CDbExportHandler* pDbExportHandler)
{
	m_pDbExportHandler = pDbExportHandler;
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage3, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelectField1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelectField2)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION1, OnSelectCondition1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION2, OnSelectCondition2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

void CExportPropertyPage3::ExtractConditions()
{
	m_strField1			= m_pDbExportHandler->GetField1();
	m_strCondition1		= m_pDbExportHandler->GetCondition1();
	m_dValue1			= m_pDbExportHandler->GetValue1();
	m_strAndOr			= m_pDbExportHandler->GetAndOr();
	m_strField2			= m_pDbExportHandler->GetField2();
	m_strCondition2		= m_pDbExportHandler->GetCondition2();
	m_dValue2			= m_pDbExportHandler->GetValue2();

	if ( System.GetEnablePurse2Flag() == FALSE )
	{
		if ( m_strField1 == szPURSE2 || m_strField1 == szPURSE2SPENDTD )
		{
			m_strField1		= szPURSE1;
			m_strCondition1	= szDONTCARE;
			m_dValue1		= 0;
		}
			
		if ( m_strField2 == szPURSE2 || m_strField2 == szPURSE2SPENDTD )
		{
			m_strField2		= szPURSE1;
			m_strCondition2	= szDONTCARE;
			m_dValue2		= 0;
		}
	}
}

//$$******************************************************************

BOOL CExportPropertyPage3::OnInitDialog() 
{
	ExtractConditions();

	CPropertyPage::OnInitDialog();
	
	m_comboField1.AddString ( szPURSE1 );
	m_comboField2.AddString ( szPURSE1 );

	if ( System.GetEnablePurse2Flag() == TRUE )
	{
		m_comboField1.AddString ( szPURSE2 );
		m_comboField2.AddString ( szPURSE2 );
	}

	m_comboField1.AddString ( szPOINTS );
	m_comboField2.AddString ( szPOINTS );

	m_comboField1.AddString ( szPURSE1SPENDTD );
	m_comboField2.AddString ( szPURSE1SPENDTD );

	if ( System.GetEnablePurse2Flag() == TRUE )
	{
		m_comboField1.AddString ( szPURSE2SPENDTD );
		m_comboField2.AddString ( szPURSE2SPENDTD );
	}

	m_comboField1.AddString ( szCASHSPENDTD );
	m_comboField2.AddString ( szCASHSPENDTD );

	m_comboField1.AddString ( szTOTALSPENDTD );
	m_comboField2.AddString ( szTOTALSPENDTD );

	m_comboField1.AddString ( szPOINTSTD );
	m_comboField2.AddString ( szPOINTSTD );

	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField1, m_strField1, 0);
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField2, m_strField2, 0);
	CStandardCombos::FillAndOrCombo( m_comboAnd, m_strAndOr );
	CStandardCombos::FillConditionCombo( m_comboCondition1, m_strCondition1 );
	CStandardCombos::FillConditionCombo( m_comboCondition2, m_strCondition2 );

	OnSelectField1();
	OnSelectField2();

	return TRUE;
}

//$$******************************************************************
CEdit* CExportPropertyPage3::GetEditValue1() { return GetEdit(IDC_EDIT_VALUE1); }
CEdit* CExportPropertyPage3::GetEditValue2() { return GetEdit(IDC_EDIT_VALUE2); }
//$$******************************************************************

BOOL CExportPropertyPage3::OnKillActive() 
{
	if ( UpdateData() == TRUE )
	{
		m_pDbExportHandler->SetField1		( m_strField1 );
		m_pDbExportHandler->SetCondition1	( m_strCondition1 );
		m_pDbExportHandler->SetValue1		( m_dValue1 );
		m_pDbExportHandler->SetAndOr		( m_strAndOr );
		m_pDbExportHandler->SetField2		( m_strField2 );
		m_pDbExportHandler->SetCondition2	( m_strCondition2 );
		m_pDbExportHandler->SetValue2		( m_dValue2 );
	
		return CPropertyPage::OnKillActive();
	}
	return FALSE;
}

//$$******************************************************************

void CExportPropertyPage3::OnButtonClear() 
{
	m_pDbExportHandler->SetDefaultConditions();
	ExtractConditions();
	UpdateData(FALSE);
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectField1()
{
	CString strField = "";
	int nCurSel = m_comboField1.GetCurSel();
	m_comboField1.GetLBText(nCurSel, strField);

	if (strField == szPOINTS || strField == szPOINTSTD)
	{
		SubclassEdit(IDC_EDIT_VALUE1, SS_NUM_S, 8, "%.0f");
	}
	else
	{
		SubclassEdit(IDC_EDIT_VALUE1, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
	}
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectField2() 
{
	CString strField= "";
	int nCurSel = m_comboField2.GetCurSel();
	m_comboField1.GetLBText(nCurSel,strField);

	if (strField == szPOINTS || strField == szPOINTSTD)
	{
		SubclassEdit(IDC_EDIT_VALUE2, SS_NUM_S, 8, "%.0f");
	}
	else
	{
		SubclassEdit(IDC_EDIT_VALUE2, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
	}
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectCondition1() 
{
	GetEditValue1() -> SetFocus();
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectCondition2() 
{
	GetEditValue2()-> SetFocus();
}

//$$******************************************************************
