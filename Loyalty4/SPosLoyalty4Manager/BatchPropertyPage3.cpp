//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "DbExportHandler.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "BatchPropertyPage3.h"
//$$******************************************************************

CBatchPropertyPage3::CBatchPropertyPage3()
	: CSSPropertyPage(CBatchPropertyPage3::IDD)
{
	ResetConditions();
}

//$$******************************************************************

void CBatchPropertyPage3::ResetConditions()
{
	m_strField1			= szPURSE1;
	m_strCondition1		= szDONTCARE;
	m_strAndOr			= szOR;
	m_strField2			= szPURSE1;
	m_strCondition2		= szDONTCARE;
	m_dValue1			= 0;
	m_dValue2			= 0;
}

//$$******************************************************************

void CBatchPropertyPage3::DoDataExchange(CDataExchange* pDX)
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
	DDV_MinMaxDouble(pDX, m_dValue1, Account::Purse1Balance.Min, Account::Purse1Balance.Max);
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_dValue2 );
	DDV_MinMaxDouble(pDX, m_dValue2, Account::Purse1Balance.Min, Account::Purse1Balance.Max);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage3, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelchangeComboField1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelchangeComboField2)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION1, OnSelchangeComboCondition1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION2, OnSelchangeComboCondition2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage3::OnInitDialog() 
{
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

	OnSelchangeComboField1();
	OnSelchangeComboField2();

	return TRUE;
}

//$$******************************************************************
CEdit* CBatchPropertyPage3::GetEditValue1() { return GetEdit(IDC_EDIT_VALUE1); }
CEdit* CBatchPropertyPage3::GetEditValue2() { return GetEdit(IDC_EDIT_VALUE2); }
//$$******************************************************************

void CBatchPropertyPage3::OnButtonClear() 
{
	ResetConditions();
	UpdateData(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage3::OnSelchangeComboField1() 
{
	CString strField;
	int nCurSel = m_comboField1.GetCurSel();
	m_comboField1.GetLBText(nCurSel,strField);

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

void CBatchPropertyPage3::OnSelchangeComboField2() 
{
	CString strField;
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

void CBatchPropertyPage3::OnSelchangeComboCondition1() 
{
	GetEditValue1()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage3::OnSelchangeComboCondition2() 
{
	GetEditValue1()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage3::GetSQLConditions(CDbExportHandler& DbExportHandler)
{
	DbExportHandler.SetField1(m_strField1);
	DbExportHandler.SetCondition1(m_strCondition1);
	DbExportHandler.SetValue1(m_dValue1);
	DbExportHandler.SetAndOr(m_strAndOr);
	DbExportHandler.SetField2(m_strField2);
	DbExportHandler.SetCondition2(m_strCondition2);
	DbExportHandler.SetValue2(m_dValue2);
}

//$$******************************************************************

