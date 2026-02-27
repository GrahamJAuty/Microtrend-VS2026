//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLWhereBuilder.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
#include "AllergiesDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "BatchPropertyPage3.h"
//$$******************************************************************

CBatchPropertyPage3Conditions::CBatchPropertyPage3Conditions()
{
	ResetConditions1();
	ResetConditions2();
}

//$$******************************************************************

void CBatchPropertyPage3Conditions::ResetConditions1()
{
	m_strField1 = szPURSE1_BALANCE;
	m_strCondition1 = szDONTCARE;
	m_strAndOr = szOR;
	m_strField2 = szPURSE1_BALANCE;
	m_strCondition2 = szDONTCARE;
	m_dValue1 = 0;
	m_dValue2 = 0;
}

//$$******************************************************************

void CBatchPropertyPage3Conditions::ResetConditions2()
{
	m_strCondition3 = szDONTCARE; // gender
	m_bCheckDietary = FALSE;
	m_strDietary = "";
	m_bCheckAllergies = FALSE;
	m_nAllergyCodes = 0;
	m_strAllergies = "";
	m_bCheckAlert = FALSE;
	m_nAlertCode = 0;
	m_strAlert = "";
	m_nInfoTextIndex = 0;
	m_strInfoFilterText = "";
	m_nBioRegisterCheck = 0;
}

//$$******************************************************************

CBatchPropertyPage3::CBatchPropertyPage3()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CBatchPropertyPage3::IDD)
{
	m_Allergy.Read();
	m_Alert.Open(Filenames.GetAlertFilename());
}

//$$******************************************************************

void CBatchPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboField1);
	DDX_Control(pDX, IDC_COMBO_CONDITION1, m_comboCondition1);
	DDX_Control(pDX, IDC_COMBO_AND, m_comboAnd);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboField2);
	DDX_Control(pDX, IDC_COMBO_CONDITION2, m_comboCondition2);
	/*****/
	DDX_Control(pDX, IDC_COMBO_CONDITION3, m_comboCondition3);
	DDX_Control(pDX, IDC_CHECK_DIETARY, m_checkDietary);
	DDX_Control(pDX, IDC_EDIT_DIETARY, m_editDietary);
	DDX_Control(pDX, IDC_CHECK_ALLERGIES, m_checkAllergies);
	DDX_Control(pDX, IDC_EDIT_ALLERGIES, m_editAllergies);
	DDX_Control(pDX, IDC_BUTTON_ALLERGIES, m_buttonAllergies);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_EDIT_ALERT, m_editAlert);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_COMBO_INFO, m_comboInfoText);
	DDX_Control(pDX, IDC_COMBO_INFOFILTER, m_comboInfoFilter);
	DDX_Control(pDX, IDC_STATIC_BIOREGISTER, m_staticBioRegister);
	DDX_Control(pDX, IDC_COMBO_BIOREGISTRY, m_comboBioRegister);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelectField1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION1, OnSelectCondition1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelectField2)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION2, OnSelectCondition2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR1, OnButtonClear1)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_DIETARY, OnToggleDietary)
	ON_BN_CLICKED(IDC_CHECK_ALLERGIES, OnToggleAllergies)
	ON_BN_CLICKED(IDC_BUTTON_ALLERGIES, OnButtonAllergies)
	ON_BN_CLICKED(IDC_CHECK_ALERT, OnToggleAlert)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	ON_CBN_SELCHANGE(IDC_COMBO_INFO, OnSelectInfo)
	ON_STN_DBLCLK(IDC_STATIC_DOT, OnDoubleClickStaticDot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR2, OnButtonClear2)
END_MESSAGE_MAP()

//$$******************************************************************

void CBatchPropertyPage3::AddToFieldCombos(const char* sz)
{
	m_comboField1.AddString(sz);
	m_comboField2.AddString(sz);
}

//$$******************************************************************

BOOL CBatchPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetAllergyString();
	SetAlertString();

	AddToFieldCombos(szPURSE1_BALANCE);

	if (System.GetCreditOptionsFlag() == TRUE)
	{
		AddToFieldCombos(szPURSE1_LIABILITY);
		AddToFieldCombos(szPURSE1_CREDIT);
	}

	AddToFieldCombos(szPURSE2_BALANCE);
	AddToFieldCombos(szPURSE3_BALANCE);
	
	if (System.GetCreditOptionsFlag() == TRUE)
	{
		AddToFieldCombos(szPURSE3_LIABILITY);
		AddToFieldCombos(szPURSE3_CREDIT);
	}

	AddToFieldCombos(szPOINTS);
	AddToFieldCombos(szCASHSPENDTD);
	AddToFieldCombos(szPURSE1SPENDTD);
	AddToFieldCombos(szPURSE2SPENDTD);
	AddToFieldCombos(szPURSE3SPENDTD);
	AddToFieldCombos(szTOTALSPENDTD);
	AddToFieldCombos(szPOINTSTD);

	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField1, m_Conditions.m_strField1, 0);
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField2, m_Conditions.m_strField2, 0);
	CStandardCombos::FillAndOrCombo(m_comboAnd, m_Conditions.m_strAndOr);
	CStandardCombos::FillConditionCombo(m_comboCondition1, m_Conditions.m_strCondition1);
	CStandardCombos::FillConditionCombo(m_comboCondition2, m_Conditions.m_strCondition2);
	CStandardCombos::FillGenderConditionCombo(m_comboCondition3, m_Conditions.m_strCondition3);
	CStandardCombos::FillBioRegisterCombo(m_comboBioRegister, m_Conditions.m_nBioRegisterCheck);

	OnSelectField1();
	OnSelectField2();

	{
		CString strField = GetComboBoxText(m_comboField1);
		int nDPs = (strField == szPOINTS || strField == szPOINTSTD) ? 0 : 2;
		SetEditBoxDouble(*GetEditValue1(), m_Conditions.m_dValue1, nDPs);
	}

	{
		CString strField = GetComboBoxText(m_comboField2);
		int nDPs = (strField == szPOINTS || strField == szPOINTSTD) ? 0 : 2;
		SetEditBoxDouble(*GetEditValue2(), m_Conditions.m_dValue2, nDPs);
	}

	m_checkDietary.SetCheck(m_Conditions.m_bCheckDietary);
	OnToggleDietary();
	m_editDietary.SetWindowText(m_Conditions.m_strDietary);

	if (m_Allergy.GetAllergyCount() == 0)
	{
		m_buttonAllergies.ShowWindow(SW_HIDE);
		m_checkAllergies.ShowWindow(SW_HIDE);
		m_editAllergies.ShowWindow(SW_HIDE);
	}
	else
	{
		m_checkAllergies.SetCheck(m_Conditions.m_bCheckAllergies);
		OnToggleAllergies();
	}

	if (m_Alert.GetAlertCount() == 0)
	{
		m_buttonAlert.ShowWindow(SW_HIDE);
		m_checkAlert.ShowWindow(SW_HIDE);
		m_editAlert.ShowWindow(SW_HIDE);
	}
	else
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_checkAlert.SetWindowText("Hotlist");
		}

		m_checkAlert.SetCheck(m_Conditions.m_bCheckAlert);
		OnToggleAlert();
	}

	m_comboInfoText.AddString(szDONTCARE);

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		m_comboInfoText.AddString(RepoText.GetDisplayInfoText(n));
	}

	m_comboInfoText.SetCurSel(m_Conditions.m_nInfoTextIndex);
	OnSelectInfo();

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		m_comboBioRegister.SetCurSel(m_Conditions.m_nBioRegisterCheck);
		break;

	default:
		m_staticBioRegister.ShowWindow(SW_HIDE);
		m_comboBioRegister.ShowWindow(SW_HIDE);
		break;
	}

	return TRUE;
}

//$$******************************************************************

void CBatchPropertyPage3::SetAllergyString()
{
	if (m_Conditions.m_nAllergyCodes == 0)
	{
		m_editAllergies.SetWindowText("");
	}
	else
	{
		m_Allergy.SetAllergyCodes(m_Conditions.m_nAllergyCodes);
		m_editAllergies.SetWindowText(m_Allergy.GetAllergyString());
	}
}

//$$******************************************************************

void CBatchPropertyPage3::SetAlertString()
{
	if (m_Conditions.m_nAlertCode == 0)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		m_editAlert.SetWindowText(m_Alert.GetText(m_Conditions.m_nAlertCode));
	}
}

//$$******************************************************************

void CBatchPropertyPage3::OnSelectField1()
{
	CString strField = GetComboBoxText(m_comboField1);
	
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

void CBatchPropertyPage3::OnSelectField2()
{
	CString strField = "";
	int nCurSel = m_comboField2.GetCurSel();
	m_comboField2.GetLBText(nCurSel, strField);

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

void CBatchPropertyPage3::OnSelectCondition1()
{
	GetEditValue1()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage3::OnSelectCondition2()
{
	GetEditValue2()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage3::OnButtonClear1()
{
	m_Conditions.ResetConditions1();
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField1, m_Conditions.m_strField1, 0);
	CStandardCombos::FillConditionCombo(m_comboCondition1, m_Conditions.m_strCondition1);
	CStandardCombos::FillAndOrCombo(m_comboAnd, m_Conditions.m_strAndOr);
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboField2, m_Conditions.m_strField2, 0);
	CStandardCombos::FillConditionCombo(m_comboCondition2, m_Conditions.m_strCondition2);
	SetEditBoxDouble(*GetEditValue1(), m_Conditions.m_dValue1, 2);
	SetEditBoxDouble(*GetEditValue1(), m_Conditions.m_dValue2, 2);
}

//$$******************************************************************

void CBatchPropertyPage3::OnToggleDietary()
{
	m_editDietary.EnableWindow(IsTicked(m_checkDietary));
	m_editDietary.SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage3::OnToggleAllergies()
{
	bool bEnable = IsTicked(m_checkAllergies);
	m_buttonAllergies.EnableWindow(bEnable);

	if (FALSE == bEnable)
	{
		m_editAllergies.SetWindowText("");
	}
	else
	{
		SetAllergyString();
	}
}

//$$******************************************************************

void CBatchPropertyPage3::OnButtonAllergies()
{
	CAllergiesDlg dlg("", m_Conditions.m_nAllergyCodes, this);
	if (dlg.DoModal() == IDOK)
	{
		m_Conditions.m_nAllergyCodes = dlg.GetAllergyCodes();
		SetAllergyString();
	}
}

//$$******************************************************************

void CBatchPropertyPage3::OnToggleAlert()
{
	bool bEnable = IsTicked(m_checkAlert);

	m_buttonAlert.EnableWindow(bEnable);

	if (FALSE == bEnable)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		SetAlertString();
	}

	UpdateData(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage3::OnButtonAlert()
{
	CAlertTextSelectDlg dlg(this);
	dlg.m_nAlertCode = m_Conditions.m_nAlertCode;
	if (dlg.DoModal() == IDOK)
	{
		m_Conditions.m_nAlertCode = dlg.m_nAlertCode;
		SetAlertString();
	}
}

//$$******************************************************************

void CBatchPropertyPage3::OnSelectInfo()
{
	m_Conditions.m_nInfoTextIndex = m_comboInfoText.GetCurSel();

	if (UpdateData() == TRUE)
	{
		AddInfoSelections();
		m_comboInfoFilter.EnableWindow(m_Conditions.m_nInfoTextIndex != 0);
		m_comboInfoFilter.SetFocus();
	}
}

//$$******************************************************************

void CBatchPropertyPage3::AddInfoSelections()
{
	CString strCommentFile = Filenames.InfoListSelections(m_Conditions.m_nInfoTextIndex);

	if (strCommentFile != m_strLastInfoFile)
	{
		if (m_strLastInfoFile != "")
		{
			m_Conditions.m_strInfoFilterText = GetComboBoxText(m_comboInfoFilter);
		}

		m_InfoList.ReadComments(strCommentFile);
		m_strLastInfoFile = strCommentFile;					// stop same file from being read again 
	}

	m_comboInfoFilter.ResetContent();						// new list

	for (int i = 0; i < m_InfoList.GetSize(); i++)		// read comments
	{
		m_comboInfoFilter.AddString(m_InfoList.GetAt(i));
	}

	int nCurSel = 0;
	for (int i = 0; i < m_InfoList.GetSize(); i++)		// do it outside abount loop as combo items get sorted
	{
		CString strInfo = "";
		m_comboInfoFilter.GetLBText(i, strInfo);
		if (strInfo.CompareNoCase(m_Conditions.m_strInfoFilterText) == 0)
		{
			nCurSel = i;
		}
	}

	m_comboInfoFilter.SetCurSel(nCurSel);
}

//$$******************************************************************

void CBatchPropertyPage3::OnDoubleClickStaticDot()
{
	m_Conditions.m_strInfoFilterText = GetComboBoxText(m_comboInfoFilter);

	if ((m_Conditions.m_strInfoFilterText != "") && (m_Conditions.m_nInfoTextIndex > 0))
	{
		m_InfoList.Delete(m_Conditions.m_strInfoFilterText);		// delete comment
		AddInfoSelections();
	}
}

//$$******************************************************************

void CBatchPropertyPage3::OnButtonClear2() 
{
	m_Conditions.ResetConditions2();
	
	CStandardCombos::FillGenderConditionCombo(m_comboCondition3, m_Conditions.m_strCondition3);
	
	m_checkAllergies.SetCheck(m_Conditions.m_bCheckAllergies);
	OnToggleAllergies();
	
	m_checkAlert.SetCheck(m_Conditions.m_bCheckAlert);
	OnToggleAlert();

	m_comboInfoText.SetCurSel(m_Conditions.m_nInfoTextIndex);
	OnSelectInfo();

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		m_comboBioRegister.SetCurSel(m_Conditions.m_nBioRegisterCheck);
		break;
	}

	m_checkDietary.SetCheck(m_Conditions.m_bCheckDietary);
	OnToggleDietary();
	m_editDietary.SetWindowText(m_Conditions.m_strDietary);
}

//$$******************************************************************

BOOL CBatchPropertyPage3::OnKillActive()
{
	m_Conditions.m_strField1 = GetComboBoxText(m_comboField1);
	m_Conditions.m_strCondition1 = GetComboBoxText(m_comboCondition1);
	m_Conditions.m_strAndOr = GetComboBoxText(m_comboAnd);
	m_Conditions.m_strField2 = GetComboBoxText(m_comboField2);
	m_Conditions.m_strCondition2 = GetComboBoxText(m_comboCondition2);

	m_Conditions.m_dValue1 = GetEditBoxDouble(*GetEditValue1());
	if ((m_Conditions.m_dValue1 < Account::Purse1Liability.Min) || (m_Conditions.m_dValue1 > Account::Purse1Liability.Max))
	{
		CString strMsg = "";
		strMsg.Format("Please enter a value between %.2f and %.2f",
			Account::Purse1Liability.Min,
			Account::Purse1Liability.Max);

		Prompter.Error(strMsg);
		GetEditValue1()->SetFocus();
		GetEditValue1()->SetSel(0,-1);
		return FALSE;
	}

	m_Conditions.m_dValue2 = GetEditBoxDouble(*GetEditValue2());
	if ((m_Conditions.m_dValue2 < Account::Purse1Liability.Min) || (m_Conditions.m_dValue2 > Account::Purse1Liability.Max))
	{
		CString strMsg = "";
		strMsg.Format("Please enter a value between %.2f and %.2f",
			Account::Purse1Liability.Min,
			Account::Purse1Liability.Max);

		Prompter.Error(strMsg);
		GetEditValue2()->SetFocus();
		GetEditValue2()->SetSel(0, -1);
		return FALSE;
	}

	m_Conditions.m_strCondition3 = GetComboBoxText(m_comboCondition3);
	m_Conditions.m_bCheckDietary = IsTicked(m_checkDietary);
	m_Conditions.m_strDietary = GetEditBoxText(m_editDietary);
	m_Conditions.m_bCheckAllergies = IsTicked(m_checkAllergies);
	m_Conditions.m_strAllergies = GetEditBoxText(m_editAllergies);
	m_Conditions.m_bCheckAlert = IsTicked(m_checkAlert);
	m_Conditions.m_strAlert = GetEditBoxText(m_editAlert);
	m_Conditions.m_nInfoTextIndex = m_comboInfoText.GetCurSel();
	m_Conditions.m_strInfoFilterText = GetComboBoxText(m_comboInfoFilter);
	m_Conditions.m_nBioRegisterCheck = m_comboBioRegister.GetCurSel();

	if ( m_Conditions.m_strInfoFilterText != "" )
	{
		if (m_comboInfoText.GetCurSel() > 0)
		{
			m_InfoList.SetMaxLength(m_Conditions.m_strInfoFilterText.GetLength());	// stop entry being truncated
			m_InfoList.Save(m_Conditions.m_strInfoFilterText);						// save info
		}
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

bool CBatchPropertyPage3::CheckConditional(CSQLRowAccountFull& RowAccount)
{
	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		if (m_Conditions.m_nBioRegisterCheck != 0)								// 0= no check, 1=Include if found, 2=exclude if found
		{
			CSQLRowBioRegister RowBioRegister;
			RowBioRegister.SetUserID(RowAccount.GetUserID());

			CSQLRepositoryBioRegister Repo;
			bool bFound = (Repo.SelectRow(RowBioRegister, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

			if ((1 == m_Conditions.m_nBioRegisterCheck) && (FALSE == bFound))
			{
				return FALSE;			// include if found
			}

			if ((2 == m_Conditions.m_nBioRegisterCheck) && (TRUE == bFound))
			{
				return FALSE;			// include if not found
			}
		}
		break;
	}

	if (m_Conditions.m_nInfoTextIndex > 0)															// 0=don't care, info1-8
	{
		CString strFilterText = ::RemoveSpaces(m_Conditions.m_strInfoFilterText);
		if (strFilterText != "")
		{
			CString strInfoText = ::RemoveSpaces(RowAccount.GetInfo(m_Conditions.m_nInfoTextIndex));
			if (strFilterText.CompareNoCase(strInfoText) != 0)
			{
				return FALSE;
			}
		}
	}

	if (m_Conditions.m_bCheckDietary == TRUE)
	{
		CString strDietaryText = ::RemoveSpaces(RowAccount.GetDietary());
		if (strDietaryText == "")
		{
			return FALSE;
		}

		CString strFilterText = ::RemoveSpaces(m_Conditions.m_strDietary);
		strFilterText.MakeUpper();

		strDietaryText.MakeUpper();
		if (strDietaryText.Find(strFilterText) == -1)
		{
			return FALSE;
		}
	}

	if (m_Conditions.m_bCheckAllergies == TRUE)
	{
		CAllergyText allergy;
		allergy.Read();
		allergy.SetAllergyCodes(m_Conditions.m_nAllergyCodes);

		if (allergy.MatchAllergies(RowAccount.GetAllergyCodes()) == 0)	// see how many allergies match
		{
			return FALSE;
		}
	}

	if (m_Conditions.m_bCheckAlert == TRUE)
	{
		if (RowAccount.GetAlertCode() != m_Conditions.m_nAlertCode)
		{
			return FALSE;
		}
	}

	if (m_Conditions.m_strCondition3 != szDONTCARE)			// gender
	{
		if (m_Conditions.m_strCondition3 == szMALE && RowAccount.IsGenderMale() == FALSE)
		{
			return FALSE;
		}
		else if (m_Conditions.m_strCondition3 == szFEMALE && RowAccount.IsGenderFemale() == FALSE)
		{
			return FALSE;
		}
		else if (m_Conditions.m_strCondition3 == szUNKNOWN && RowAccount.IsGenderUnknown() == FALSE)
		{
			return FALSE;
		}
	}

	if (m_Conditions.m_strCondition1 != szDONTCARE)
	{
		double dRecordValue = CDbExportHandler::GetValueByFieldName(m_Conditions.m_strField1, RowAccount);

		bool bCondition = FALSE;

		if (m_Conditions.m_strCondition1 == szLESSTHAN && dRecordValue < m_Conditions.m_dValue1)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition1 == szGREATER && dRecordValue > m_Conditions.m_dValue1)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition1 == szEQUAL && dRecordValue == m_Conditions.m_dValue1)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition1 == szNOTEQUAL && dRecordValue != m_Conditions.m_dValue1)
		{
			bCondition = TRUE;
		}

		if (bCondition == TRUE)
		{
			if (m_Conditions.m_strAndOr == szOR || m_Conditions.m_strCondition2 == szDONTCARE)
			{
				return TRUE;
			}
		}
		else
		{
			if (m_Conditions.m_strAndOr == szAND || m_Conditions.m_strCondition2 == szDONTCARE)
			{
				return FALSE;
			}
		}
	}

	if (m_Conditions.m_strCondition2 != szDONTCARE)
	{
		double dRecordValue = CDbExportHandler::GetValueByFieldName(m_Conditions.m_strField2, RowAccount);

		bool bCondition = FALSE;
		
		if (m_Conditions.m_strCondition2 == szLESSTHAN && dRecordValue < m_Conditions.m_dValue2)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition2 == szGREATER && dRecordValue > m_Conditions.m_dValue2)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition2 == szEQUAL && dRecordValue == m_Conditions.m_dValue2)
		{
			bCondition = TRUE;
		}
		else if (m_Conditions.m_strCondition2 == szNOTEQUAL && dRecordValue != m_Conditions.m_dValue2)
		{
			bCondition = TRUE;
		}

		return bCondition;
	}
	
	return TRUE;
}

//$$******************************************************************
