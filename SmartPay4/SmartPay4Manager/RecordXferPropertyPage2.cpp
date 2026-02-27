//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
#include "AllergiesDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "RecordXferPropertyPage2.h"
//$$******************************************************************

CRecordXferPropertyPage2::CRecordXferPropertyPage2( CSQLRowAccountFull& RowAccountFrom, CRecordXferData& RecordXferData )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CRecordXferPropertyPage2::IDD)
	,m_RowAccountFrom(RowAccountFrom)
	, m_RecordXferData( RecordXferData )
{
	m_nAlertCode = 0;

	for (int n = 0; n <= MAXINFOTICK; n++)
	{
		m_bInfoHide[n] = FALSE;
	}

	CSQLRepositoryPebbleConfig PebbleConfig;
	PebbleConfig.LoadRow(NULL);

	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		{
			int n = PebbleConfig.m_Row.GetMISInfoIndex();
			if ((n >= 1) && (n <= MAXINFOTICK))
			{
				m_bInfoHide[n] = TRUE;
			}
		}

		{
			int n = PebbleConfig.m_Row.GetUPNInfoIndex();
			if ((n >= 1) && (n <= MAXINFOTICK))
			{
				m_bInfoHide[n] = TRUE;
			}
		}
	}

	m_bHotlistReplace = FALSE;
	m_bSelectAllDetails = FALSE;
	m_bTransferGender = FALSE;
	m_nAllergyCodes = 0;
}

//$$******************************************************************

void CRecordXferPropertyPage2::SetPointer(bool bHotlistReplace)
{
	m_bHotlistReplace = bHotlistReplace;
}

//$$******************************************************************

CRecordXferPropertyPage2::~CRecordXferPropertyPage2()
{
}

//$$******************************************************************

void CRecordXferPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK_DOB, m_checkDOB);
	DDX_Control(pDX, IDC_CHECK_EMAIL, m_checkEmail);
	DDX_Control(pDX, IDC_CHECK_INFO1, m_checkInfo[1]);
	DDX_Control(pDX, IDC_CHECK_INFO2, m_checkInfo[2]);
	DDX_Control(pDX, IDC_CHECK_INFO3, m_checkInfo[3]);
	DDX_Control(pDX, IDC_CHECK_INFO4, m_checkInfo[4]);
	DDX_Control(pDX, IDC_CHECK_INFO5, m_checkInfo[5]);
	DDX_Control(pDX, IDC_CHECK_INFO6, m_checkInfo[6]);
	DDX_Control(pDX, IDC_CHECK_INFO7, m_checkInfo[7]);
	DDX_Control(pDX, IDC_CHECK_INFO8, m_checkInfo[8]);
	DDX_Control(pDX, IDC_CHECK_DIETARY, m_checkDietary);
	DDX_Control(pDX, IDC_CHECK_ALLERGIES, m_checkAllergies);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_CHECK_TICK1, m_checkTick[1]);
	DDX_Control(pDX, IDC_CHECK_TICK2, m_checkTick[2]);
	DDX_Control(pDX, IDC_CHECK_TICK3, m_checkTick[3]);
	DDX_Control(pDX, IDC_CHECK_TICK4, m_checkTick[4]);
	/*****/
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo[1]);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo[2]);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo[3]);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo[4]);
	DDX_Control(pDX, IDC_EDIT_INFO5, m_editInfo[5]);
	DDX_Control(pDX, IDC_EDIT_INFO6, m_editInfo[6]);
	DDX_Control(pDX, IDC_EDIT_INFO7, m_editInfo[7]);
	DDX_Control(pDX, IDC_EDIT_INFO8, m_editInfo[8]);
	DDX_Control(pDX, IDC_EDIT_DIETARY, m_editDietary);
	DDX_Control(pDX, IDC_BUTTON_ALLERGIES, m_buttonAllergies);
	DDX_Control(pDX, IDC_EDIT_ALLERGIES, m_editAllergies);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERTTEXT, m_editAlert);
	DDX_Control(pDX, IDC_CHECK_RECORDTICK1, m_checkRecordTick[1]);
	DDX_Control(pDX, IDC_CHECK_RECORDTICK2, m_checkRecordTick[2]);
	DDX_Control(pDX, IDC_CHECK_RECORDTICK3, m_checkRecordTick[3]);
	DDX_Control(pDX, IDC_CHECK_RECORDTICK4, m_checkRecordTick[4]);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_buttonSelectAll);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CRecordXferPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_DOB, OnToggleDOB)
	ON_BN_CLICKED(IDC_CHECK_EMAIL, OnToggleEmail)
	ON_BN_CLICKED(IDC_CHECK_INFO1, OnToggleInfo1)
	ON_BN_CLICKED(IDC_CHECK_INFO2, OnToggleInfo2)
	ON_BN_CLICKED(IDC_CHECK_INFO3, OnToggleInfo3)
	ON_BN_CLICKED(IDC_CHECK_INFO4, OnToggleInfo4)
	ON_BN_CLICKED(IDC_CHECK_INFO5, OnToggleInfo5)
	ON_BN_CLICKED(IDC_CHECK_INFO6, OnToggleInfo6)
	ON_BN_CLICKED(IDC_CHECK_INFO7, OnToggleInfo7)
	ON_BN_CLICKED(IDC_CHECK_INFO8, OnToggleInfo8)
	ON_BN_CLICKED(IDC_CHECK_DIETARY, OnToggleDietary)
	ON_BN_CLICKED(IDC_CHECK_ALLERGIES, OnToggleAllergies)
	ON_BN_CLICKED(IDC_BUTTON_ALLERGIES, OnButtonAllergies)
	ON_BN_CLICKED(IDC_CHECK_ALERT, OnToggleAlert)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	ON_BN_CLICKED(IDC_CHECK_TICK1, OnToggleTick1)
	ON_BN_CLICKED(IDC_CHECK_TICK2, OnToggleTick2)
	ON_BN_CLICKED(IDC_CHECK_TICK3, OnToggleTick3)
	ON_BN_CLICKED(IDC_CHECK_TICK4, OnToggleTick4)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnButtonSelect)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CRecordXferPropertyPage2::OnInitDialog()
{
	m_alert.Open(Filenames.GetAlertFilename());		// readonly

	m_bTransferGender = m_bHotlistReplace;					// only used in hotlist/replace	
	m_bSelectAllDetails = !m_bHotlistReplace;
	m_nAllergyCodes = m_RowAccountFrom.GetAllergyCodes();
	m_nAlertCode = m_RowAccountFrom.GetAlertCode();

	CPropertyPage::OnInitDialog();

	m_checkDOB.SetCheck(m_bHotlistReplace);
	m_checkEmail.SetCheck(m_bHotlistReplace);

	for (int n = 1; n <= MAXINFOTICK; n++)
	{
		bool bAllow = (FALSE == m_bInfoHide[n]);
		m_checkInfo[n].SetCheck(m_bHotlistReplace && bAllow);
		m_checkInfo[n].EnableWindow(bAllow);
		m_editInfo[n].SetWindowText(m_RowAccountFrom.GetInfo(n));
	}

	m_checkDietary.SetCheck(m_bHotlistReplace);
	m_checkAllergies.SetCheck(m_bHotlistReplace);
	m_checkAlert.SetCheck(m_bHotlistReplace);
	/*****/

	for (int n = 1; n <= MAXRECORDTICK; n++)
	{
		m_checkTick[n].SetCheck(m_bHotlistReplace);
		m_checkRecordTick[n].SetCheck(m_RowAccountFrom.GetTick(n));
	}

	GetEditDOB()->SetWindowText(m_RowAccountFrom.GetDOB());
	m_editEmail.SetWindowText(m_RowAccountFrom.GetEmail());
	CStandardCombos::FillGenderCombo(m_comboGender, m_RowAccountFrom.GetGenderText());
	
	m_editDietary.SetWindowText(m_RowAccountFrom.GetDietary());

	CString strAlertText = "";
	if (m_RowAccountFrom.IsAlertSet() == TRUE)
	{
		strAlertText = m_alert.GetText(m_nAlertCode);
	}

	m_editAlert.SetWindowText(strAlertText);

	SubclassEdit(IDC_EDIT_DOB, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_POINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTSTD, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_PURSE1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND, SS_NUM_SDP, 11, "%.2f");

	CSQLRepositoryUserText RepoText;
	m_checkDOB.SetWindowText(RepoText.GetDOBText("Dob"));
	m_checkInfo[1].SetWindowText(RepoText.GetInfo1Text("Info1"));
	m_checkInfo[2].SetWindowText(RepoText.GetInfo2Text("Info2"));
	m_checkInfo[3].SetWindowText(RepoText.GetInfo3Text("Info3"));
	m_checkInfo[4].SetWindowText(RepoText.GetInfo4Text("Info4"));
	m_checkInfo[5].SetWindowText(RepoText.GetInfo5Text("Info5"));
	m_checkInfo[6].SetWindowText(RepoText.GetInfo6Text("Info6"));
	m_checkInfo[7].SetWindowText(RepoText.GetInfo7Text("Info7"));
	m_checkInfo[8].SetWindowText(RepoText.GetInfo8Text("Info8"));
	m_checkTick[1].SetWindowText(RepoText.GetTick1Text("Tick1"));
	m_checkTick[2].SetWindowText(RepoText.GetTick2Text("Tick2"));
	m_checkTick[3].SetWindowText(RepoText.GetTick3Text("Tick3"));
	m_checkTick[4].SetWindowText(RepoText.GetTick4Text("Tick4"));

	if (m_bTransferGender == FALSE)
	{
		m_comboGender.ShowWindow(SW_HIDE);
	}

	if (m_alert.GetAlertCount() == 0 || m_bHotlistReplace == TRUE)
	{
		m_checkAlert.ShowWindow(SW_HIDE);
		m_buttonAlert.ShowWindow(SW_HIDE);
		m_editAlert.ShowWindow(SW_HIDE);
	}

	SetDisplay();
	OnButtonSelect();
	SetDisplayAllergy();

	return TRUE;
}

//$$******************************************************************
CEdit* CRecordXferPropertyPage2::GetEditDOB(){ return GetEdit( IDC_EDIT_DOB ); }
//$$******************************************************************

void CRecordXferPropertyPage2::OnButtonSelect()
{
	if (UpdateData() == TRUE)
	{
		m_bSelectAllDetails = !m_bSelectAllDetails;
		CString strButton = (m_bSelectAllDetails == TRUE) ? "Clear" : "Select All";
		m_buttonSelectAll.SetWindowText(strButton);

		m_checkDOB.SetCheck(m_bSelectAllDetails);
		m_checkEmail.SetCheck(m_bSelectAllDetails);

		m_checkDietary.SetCheck(m_bSelectAllDetails);

		m_checkAllergies.SetCheck(m_bSelectAllDetails);

		if (m_bHotlistReplace == FALSE)
		{
			m_checkAlert.SetCheck(m_bSelectAllDetails);
		}

		for (int n = 1; n <= MAXINFOTICK; n++)
		{
			if (FALSE == m_bInfoHide[n])
			{
				m_checkInfo[n].SetCheck(m_bSelectAllDetails);
			}
			else
			{
				m_checkInfo[n].SetCheck(FALSE);
			}
		}

		for (int n = 1; n <= MAXRECORDTICK; n++)
		{
			m_checkTick[n].SetCheck(m_bSelectAllDetails);
		}

		UpdateData(FALSE);
		SetDisplay();
	}
}

//$$******************************************************************
void CRecordXferPropertyPage2::OnToggleEmail()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo1()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo2()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo3()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo4()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo5()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo6()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo7()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleInfo8()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleTick1()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleTick2()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleTick3()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleTick4()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleDOB()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleAllergies()	{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleAlert()		{	SetDisplay();	}
void CRecordXferPropertyPage2::OnToggleDietary()	{	SetDisplay();	}
//$$******************************************************************

void CRecordXferPropertyPage2::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		GetEditDOB()->EnableWindow(IsTicked(m_checkDOB));
		m_editEmail.EnableWindow(IsTicked(m_checkEmail));

		int nShow = ( IsTicked( m_checkAllergies ) ) ? SW_SHOW : SW_HIDE;
		m_buttonAllergies.ShowWindow(nShow);

		if ( m_bHotlistReplace == FALSE )
		{
			nShow = ( IsTicked( m_checkAlert ) ) ? SW_SHOW : SW_HIDE;
			m_buttonAlert.ShowWindow(nShow);
		}

		m_editDietary.EnableWindow(IsTicked(m_checkDietary));

		for (int n = 1; n <= MAXINFOTICK; n++)
		{
			if (FALSE == m_bInfoHide[n])
			{
				m_editInfo[n].EnableWindow(IsTicked(m_checkInfo[n]));
			}
			else
			{
				m_editInfo[n].EnableWindow(FALSE);
			}			
		}

		for (int n = 1; n <= MAXRECORDTICK; n++)
		{
			m_checkRecordTick[n].EnableWindow(IsTicked(m_checkTick[n]));
		}
	}
}

//$$******************************************************************

bool CRecordXferPropertyPage2::StartAllowed()
{
	if (IsTicked(m_checkDOB)) return TRUE;
	if (IsTicked(m_checkEmail))	return TRUE;

	for (int n = 1; n <= MAXINFOTICK; n++)
	{
		if (FALSE == m_bInfoHide[n])
		{
			if (IsTicked(m_checkInfo[n]))
			{
				return TRUE;
			}
		}
	}

	if (IsTicked(m_checkDietary)) return TRUE;
	if (IsTicked(m_checkAllergies)) return TRUE;
	if (IsTicked(m_checkAlert))	return TRUE;

	return FALSE;
}

//$$******************************************************************

void CRecordXferPropertyPage2::OnButtonAllergies()
{
	CAllergiesDlg dlg ( "", m_RowAccountFrom.GetAllergyCodes(), this );
	if ( dlg.DoModal() == IDOK )
	{
		m_nAllergyCodes = dlg.GetAllergyCodes();
		SetDisplayAllergy();
	}
}

//$$******************************************************************

void CRecordXferPropertyPage2::SetDisplayAllergy()
{
	if (0 == m_nAllergyCodes)
	{
		m_editAllergies.SetWindowText("");
	}
	else
	{
		CAllergyText allergy;
		allergy.Read();
		allergy.SetAllergyCodes(m_nAllergyCodes);		// translate codes to a string
		m_editAllergies.SetWindowText(allergy.GetAllergyString());
	}
}

//$$******************************************************************

void CRecordXferPropertyPage2::OnButtonAlert()
{
	if ( UpdateData() == TRUE )
	{
		CAlertTextSelectDlg dlg ( this );
		dlg.m_nAlertCode = m_nAlertCode;

		if ((System.GetInterfaceType() == nINTERFACE_MIFAREv1) && (m_nAlertCode != 0))
		{
			dlg.m_bClearAllowed = FALSE;
		}

		if ( dlg.DoModal() == IDOK )
		{
			m_nAlertCode = dlg.m_nAlertCode;
			m_editAlert.SetWindowText( m_alert.GetText ( m_nAlertCode ) );
		}
	}
}

//$$******************************************************************

void CRecordXferPropertyPage2::OnOK()
{
	m_RecordXferData.m_bDOB = IsTicked( m_checkDOB );
	m_RecordXferData.m_bEmail = IsTicked( m_checkEmail );
	m_RecordXferData.m_bTransferGender = m_bTransferGender;

	m_RecordXferData.m_bDietary = IsTicked( m_checkDietary );
	m_RecordXferData.m_bAllergyCodes = IsTicked( m_checkAllergies );
	m_RecordXferData.m_bAlertCode = IsTicked( m_checkAlert );

	GetEditDOB() -> GetWindowText( m_RecordXferData.m_strDOB );
	m_editEmail.GetWindowText( m_RecordXferData.m_strEmail );
	m_comboGender.GetWindowText( m_RecordXferData.m_strGender );

	m_editDietary.GetWindowText(m_RecordXferData.m_strDietary);
	m_RecordXferData.m_nAllergyCodes = m_nAllergyCodes;
	m_RecordXferData.m_nAlertCode = m_nAlertCode;
	
	for (int n = 1; n <= MAXINFOTICK; n++)
	{
		if (FALSE == m_bInfoHide[n])
		{
			m_RecordXferData.m_bInfoText[n] = IsTicked(m_checkInfo[n]);
			m_RecordXferData.m_strInfo[n] = GetEditBoxText(m_editInfo[n]);
		}
		else
		{
			m_RecordXferData.m_bInfoText[n] = FALSE;
			m_RecordXferData.m_strInfo[n] = "";
		}
	}

	for (int n = 1; n <= MAXRECORDTICK; n++)
	{
		m_RecordXferData.m_bTick[n] = IsTicked(m_checkTick[n]);
		m_RecordXferData.m_bRecordTick[n] = IsTicked(m_checkRecordTick[n]);
	}
	
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

