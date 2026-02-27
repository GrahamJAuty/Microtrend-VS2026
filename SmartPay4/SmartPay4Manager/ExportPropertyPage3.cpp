//$$******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
#include "AllergiesDlg.h"
#include "SmartPay4Manager.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ExportPropertyPage3.h"
//$$******************************************************************

CExportPropertyPage3::CExportPropertyPage3()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CExportPropertyPage3::IDD)
{
	m_Allergy.Read();
	m_Alert.Open (Filenames.GetAlertFilename() );
	m_nAlertCode = 0;
	m_nAllergyCodes = 0;
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
	
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboField1);
	DDX_Control(pDX, IDC_COMBO_CONDITION1, m_comboCondition1);
	DDX_Control(pDX, IDC_COMBO_AND, m_comboAnd);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboField2);
	DDX_Control(pDX, IDC_COMBO_CONDITION2, m_comboCondition2);
	/****/
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_COMBO_INFO, m_comboInfoText);
	DDX_Control(pDX, IDC_COMBO_INFOFILTER, m_comboInfoFilter);
	DDX_Control(pDX, IDC_STATIC_BIOREGISTER1, m_staticBioRegister1);
	DDX_Control(pDX, IDC_STATIC_BIOREGISTER2, m_staticBioRegister2);
	DDX_Control(pDX, IDC_STATIC_BIOREGISTER3, m_staticBioRegister3);
	DDX_Control(pDX, IDC_COMBO_BIOREGISTRY, m_comboBioRegister);
	DDX_Control(pDX, IDC_CHECK_DIETARY, m_checkDietary);
	DDX_Control(pDX, IDC_EDIT_DIETARY, m_editDietary);
	DDX_Control(pDX, IDC_CHECK_ALLERGIES, m_checkAllergies);
	DDX_Control(pDX, IDC_BUTTON_ALLERGIES, m_buttonAllergies);
	DDX_Control(pDX, IDC_EDIT_ALLERGIES, m_editAllergies);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERT, m_editAlert);
}

//$$******************************************************************

void CExportPropertyPage3::SetPointer(CDbExportHandler* pDbExportHandler)
{
	m_pDbExportHandler = pDbExportHandler;
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelectField1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION1, OnSelectCondition1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelectField2)
	ON_CBN_SELCHANGE(IDC_COMBO_CONDITION2, OnSelectCondition2)
	/*****/
	ON_CBN_SELCHANGE(IDC_COMBO_INFO, OnSelectInfo)
	ON_STN_DBLCLK(IDC_STATIC_DOT, OnDoubleClickStaticDot)
	ON_BN_CLICKED(IDC_CHECK_DIETARY, OnToggleDietary)
	ON_BN_CLICKED(IDC_CHECK_ALLERGIES, OnToggleAllergies)
	ON_BN_CLICKED(IDC_BUTTON_ALLERGIES, OnButtonAllergies)
	ON_BN_CLICKED(IDC_CHECK_ALERT, OnToggleAlert)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
END_MESSAGE_MAP()

//$$******************************************************************

bool CExportPropertyPage3::IsPointsField(CSSComboBox& combo, CString& strField)
{
	strField = "";
	combo.GetWindowText(strField);
	return ((strField == szPOINTS) || (strField == szPOINTSTD));
}

//$$******************************************************************

void CExportPropertyPage3::ExtractConditionsToControls()
{
	CString strText = "";

	m_nAllergyCodes = m_pDbExportHandler->GetAllergyCodes();
	m_nAlertCode = m_pDbExportHandler->GetAlertCode();

	CStandardCombos::FillExportFieldCombo(m_comboField1, m_pDbExportHandler->GetField1());
	OnSelectField1();
	CStandardCombos::FillConditionCombo(m_comboCondition1, m_pDbExportHandler->GetCondition1());

	{
		double dValue = m_pDbExportHandler->GetValue1();

		CString strField = "";
		if ( IsPointsField( m_comboField1, strField ) == TRUE )
		{ 
			SetEditBoxInt( *GetEditPoints1(), int(floor(dValue)));
		}
		else
		{
			SetEditBoxDouble(*GetEditValue1(), dValue, System.GetDPValue());
		}
	}

	CStandardCombos::FillAndOrCombo(m_comboAnd, m_pDbExportHandler->GetAndOr());

	CStandardCombos::FillExportFieldCombo(m_comboField2, m_pDbExportHandler->GetField2());
	OnSelectField2();
	CStandardCombos::FillConditionCombo(m_comboCondition2, m_pDbExportHandler->GetCondition2());
	
	{
		double dValue = m_pDbExportHandler->GetValue2();

		CString strField = "";
		if (IsPointsField(m_comboField2, strField) == TRUE)
		{
			SetEditBoxInt(*GetEditPoints2(), int(floor(dValue)));
		}
		else
		{
			SetEditBoxDouble(*GetEditValue2(), dValue, System.GetDPValue());
		}
	}
	
	CStandardCombos::FillGenderConditionCombo(m_comboGender, m_pDbExportHandler->GetCondition3());

	m_comboInfoText.SetCurSel(m_pDbExportHandler->GetInfoFilterIndex());
	m_comboInfoFilter.ResetContent();
	m_comboInfoFilter.AddString(m_pDbExportHandler->GetInfoFilterText());
	OnSelectInfo();

	CStandardCombos::FillBioRegisterCombo(m_comboBioRegister, m_pDbExportHandler->GetBioRegisterCheck());

	m_checkAlert.SetCheck( (m_Alert.GetAlertCount() > 0) && ( m_pDbExportHandler->GetCheckAlertCode()));
	OnToggleAlert();
	m_editAlert.SetWindowText(GetAlertString());
	
	m_checkAllergies.SetCheck((m_Allergy.GetAllergyCount() > 0) && ( m_pDbExportHandler->GetCheckAllergies()));
	OnToggleAllergies();
	m_editAllergies.SetWindowText(GetAllergyString());

	m_checkDietary.SetCheck(m_pDbExportHandler->GetDietaryCheck());
	OnToggleDietary();
	m_editDietary.SetWindowText(m_pDbExportHandler->GetDietaryText());
}

//$$******************************************************************

BOOL CExportPropertyPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_VALUE1, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
	SubclassEdit(IDC_EDIT_POINTS1, SS_NUM_S, 8, "%.0f");
	SubclassEdit(IDC_EDIT_VALUE2, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
	SubclassEdit(IDC_EDIT_POINTS2, SS_NUM_S, 8, "%.0f");
	
// Info Filters

	m_comboInfoText.AddString ( szDONTCARE );
	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		m_comboInfoText.AddString(RepoText.GetDisplayInfoText(n));
	}

// alerts

	if ( m_Alert.GetAlertCount() == 0 )
	{
		m_checkAlert.ShowWindow(SW_HIDE);
		m_buttonAlert.ShowWindow(SW_HIDE);
		m_editAlert.ShowWindow(SW_HIDE);
	}
	else
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_checkAlert.SetWindowText("Hotlist");
		}
	}

// allergies
	if ( m_Allergy.GetAllergyCount() == 0 )
	{
		m_checkAllergies.ShowWindow(SW_HIDE);
		m_buttonAllergies.ShowWindow(SW_HIDE);
		m_editAllergies.ShowWindow(SW_HIDE);
	}
	
	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		break;

	default:
		m_staticBioRegister1.ShowWindow(SW_HIDE);
		m_staticBioRegister2.ShowWindow(SW_HIDE);
		m_staticBioRegister3.ShowWindow(SW_HIDE);
		m_comboBioRegister.ShowWindow(SW_HIDE);
		break;
	}

	ExtractConditionsToControls();
	
	return TRUE;
}

//$$******************************************************************
CEdit* CExportPropertyPage3::GetEditValue1() { return GetEdit(IDC_EDIT_VALUE1); }
CEdit* CExportPropertyPage3::GetEditPoints1() { return GetEdit(IDC_EDIT_POINTS1); }
CEdit* CExportPropertyPage3::GetEditValue2() { return GetEdit(IDC_EDIT_VALUE2); }
CEdit* CExportPropertyPage3::GetEditPoints2() { return GetEdit(IDC_EDIT_POINTS2); }
//$$******************************************************************

void CExportPropertyPage3::AddInfoSelections( CString strInfoFilterText )
{
	CString strCommentFile = Filenames.InfoListSelections ( m_comboInfoText.GetCurSel() );
	if ( strCommentFile != m_strLastInfoFile )
	{
		m_InfoList.ReadComments ( strCommentFile );
		m_strLastInfoFile = strCommentFile;					// stop same file from being read again 
	}

	m_comboInfoFilter.ResetContent();						// new list

	for (int i = 0; i < m_InfoList.GetSize(); i++)		// read comments
	{
		m_comboInfoFilter.AddString(m_InfoList.GetAt(i));
	}

	int nCurSel = 0;
	for ( int i = 0 ; i < m_InfoList.GetSize() ; i++ )		// do it outside abount loop as combo items get sorted
	{
		CString strInfo = "";
		m_comboInfoFilter.GetLBText ( i, strInfo );
		if (strInfo.CompareNoCase(strInfoFilterText) == 0)
		{
			nCurSel = i;
		}
	}

	m_comboInfoFilter.SetCurSel(nCurSel);
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectField1()
{
	CString strField = "";
	m_comboField1.GetWindowText(strField);
	bool bPoints = (strField == szPOINTS || strField == szPOINTSTD);
	ShowAndEnableWindow(GetEditValue1(), FALSE == bPoints);
	ShowAndEnableWindow(GetEditPoints1(), TRUE == bPoints);
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectCondition1()
{
	GetEditValue1()->SetFocus();
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectField2()
{
	CString strField = "";
	m_comboField2.GetWindowText(strField);
	bool bPoints = (strField == szPOINTS || strField == szPOINTSTD);
	ShowAndEnableWindow(GetEditValue2(), FALSE == bPoints);
	ShowAndEnableWindow(GetEditPoints2(), TRUE == bPoints);
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectCondition2()
{
	GetEditValue2()->SetFocus();
}

//$$******************************************************************

void CExportPropertyPage3::OnSelectInfo()
{
	CString strInfoFilterText = "";
	m_comboInfoFilter.GetWindowText(strInfoFilterText);

	AddInfoSelections(strInfoFilterText);

	bool bAllowed = (m_comboInfoText.GetCurSel() == 0) ? FALSE : TRUE;
	m_comboInfoFilter.EnableWindow(bAllowed);
	m_comboInfoFilter.SetFocus();
}

//$$******************************************************************

void CExportPropertyPage3::OnDoubleClickStaticDot()
{
	CString strInfoFilterText = "";
	m_comboInfoFilter.GetWindowText(strInfoFilterText);

	if ((strInfoFilterText != "") && (m_comboInfoText.GetCurSel() > 0))
	{
		m_InfoList.Delete(strInfoFilterText);		// delete comment
		AddInfoSelections("");
	}
}

//$$******************************************************************

void CExportPropertyPage3::OnToggleDietary()
{
	m_editDietary.EnableWindow(IsTicked(m_checkDietary));
	m_editDietary.SetFocus();
}

//$$******************************************************************

void CExportPropertyPage3::OnToggleAllergies()
{
	bool bEnable = IsTicked(m_checkAllergies);

	m_buttonAllergies.EnableWindow(bEnable);

	if (FALSE == bEnable)
	{
		m_editAllergies.SetWindowText("");
	}
	else
	{
		m_editAllergies.SetWindowText(GetAllergyString());
	}
}

//$$******************************************************************

void CExportPropertyPage3::OnButtonAllergies()
{
	CAllergiesDlg dlg("", m_nAllergyCodes, this);
	if (dlg.DoModal() == IDOK)
	{
		m_nAllergyCodes = dlg.GetAllergyCodes();
		m_editAllergies.SetWindowText(GetAllergyString());
	}
}

//$$******************************************************************

CString CExportPropertyPage3::GetAllergyString()
{
	if (m_nAllergyCodes == 0)
	{
		return "";
	}
	else
	{
		m_Allergy.SetAllergyCodes(m_nAllergyCodes);		// translate codes to a string
		return m_Allergy.GetAllergyString();
	}

	return "";
}

//$$******************************************************************

void CExportPropertyPage3::OnToggleAlert()
{
	bool bEnable = IsTicked(m_checkAlert);

	m_buttonAlert.EnableWindow(bEnable);

	if (FALSE == bEnable)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		m_editAlert.SetWindowText(GetAlertString());
	}

	UpdateData(FALSE);
}

//$$******************************************************************

void CExportPropertyPage3::OnButtonAlert()
{
	CAlertTextSelectDlg dlg(this);
	dlg.m_nAlertCode = m_nAlertCode;
	if (dlg.DoModal() == IDOK)
	{
		m_nAlertCode = dlg.m_nAlertCode;
		m_editAlert.SetWindowText(GetAlertString());
	}
}

//$$******************************************************************

CString CExportPropertyPage3::GetAlertString()
{
	if (m_nAlertCode == 0)
	{
		return "";
	}
	else
	{
		return m_Alert.GetText(m_nAlertCode);
	}
}

//$$******************************************************************

void CExportPropertyPage3::OnButtonClear() 
{
	m_pDbExportHandler->SetDefaultConditions();
	ExtractConditionsToControls();
	UpdateData(FALSE);
}

//$$******************************************************************

BOOL CExportPropertyPage3::OnKillActive()
{
	m_pDbExportHandler->SetCondition1(GetComboBoxText(m_comboCondition1));

	{
		CString strField1 = "";
		if (IsPointsField(m_comboField1, strField1))
		{
			m_pDbExportHandler->SetValue1(GetEditBoxInt(*GetEditPoints1()));
		}
		else
		{
			m_pDbExportHandler->SetValue1(GetEditBoxDouble(*GetEditValue1()));
		}
		m_pDbExportHandler->SetField1(strField1);
	}

	m_pDbExportHandler->SetAndOr(GetComboBoxText(m_comboAnd));
	m_pDbExportHandler->SetCondition2(GetComboBoxText(m_comboCondition2));
	
	{
		CString strField2 = "";
		if (IsPointsField(m_comboField2, strField2))
		{
			m_pDbExportHandler->SetValue2(GetEditBoxInt(*GetEditPoints2()));
		}
		else
		{
			m_pDbExportHandler->SetValue2(GetEditBoxDouble(*GetEditValue2()));
		}
		m_pDbExportHandler->SetField2(strField2);
	}

	m_pDbExportHandler->SetCondition3(GetComboBoxText(m_comboGender));

	m_pDbExportHandler->SetInfoFilterIndex(m_comboInfoText.GetCurSel());
	{
		CString strInfoFilterText = GetComboBoxText(m_comboInfoFilter);
		m_pDbExportHandler->SetInfoFilterText(strInfoFilterText);

		if (m_comboInfoText.GetCurSel() > 0)
		{
			m_InfoList.SetMaxLength(strInfoFilterText.GetLength());	// stop entry being truncated
			m_InfoList.Save(strInfoFilterText);						// save terminal list
		}
	}

	m_pDbExportHandler->SetBioRegisterCheck(m_comboBioRegister.GetCurSel());
	m_pDbExportHandler->SetDietaryCheck(IsTicked(m_checkDietary));
	m_pDbExportHandler->SetDietaryText(GetEditBoxText(m_editDietary));

	m_pDbExportHandler->SetCheckAllergies(IsTicked(m_checkAllergies));
	m_pDbExportHandler->SetAllergyCodes(m_nAllergyCodes);
	m_pDbExportHandler->SetCheckAlertCode(IsTicked(m_checkAlert));
	m_pDbExportHandler->SetAlertCode(m_nAlertCode);
	
	return CPropertyPage::OnKillActive();	
}

//$$******************************************************************
