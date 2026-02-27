//$$******************************************************************
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\GroupShiftValidator.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\UserTypeText.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
#include "AllergiesDlg.h"
#include "EditRecordPropertySheet.h"
#include "GroupComboHelpers.h"
#include "PhotoIDDlg.h"
#include "RefreshDaysDlg.h"
#include "SetDailyAlternateGroupDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "EditRecordPropertyPageDetail.h"
//$$******************************************************************
const int EDITDETAIL_EXTRAGROUP = 0;
const int EDITDETAIL_MEMBER = 1;
const int EDITDETAIL_EXTREF1 = 2;
const int EDITDETAIL_EXTREF2 = 3;
const int EDITDETAIL_BIOREG = 4;
const int EDITDETAIL_ALERT = 5;
//$$******************************************************************

CEditRecordPropertyPageDetail::CEditRecordPropertyPageDetail(CSQLRowAccountFull& Row )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEditRecordPropertyPageDetail::IDD)
	, m_SQLRowAccount(Row)
{
	m_terminal.Read();													// reader Mifare reader data in case needed
	m_bReaderMode = FALSE;
	m_strPhotoFilename = "";
	m_strNoPhotoFilename = "";
	m_nAllergyCodes = 0;
	m_bEnableHotlistEdit = FALSE;
	m_bEnableExtRefToggle = FALSE;
	m_nVisibleExtRef = 1;
	m_nMode = 0;
	m_pGroupShiftValidator = NULL;
	m_pLeaversData = NULL;
	m_pParentSheet = NULL;
	m_pEditAccountBuffer = NULL;
}

//$$******************************************************************

CEditRecordPropertyPageDetail::~CEditRecordPropertyPageDetail()
{
	if ((HBITMAP)m_image != NULL)
	{
		m_image.Destroy();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::SetPointer(void* pParentSheet, CEditAccountBuffer* pEditAccountBuffer, int nMode, CGroupShiftValidator* pGroupShiftValidator, CLeaversHandler* pLeaversData)
{
	m_pParentSheet = pParentSheet;
	m_pEditAccountBuffer = pEditAccountBuffer;
	m_pGroupShiftValidator = pGroupShiftValidator;
	m_pLeaversData = pLeaversData;
	m_nMode = nMode;
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	/*****/
	DDX_Control(pDX, IDC_STATIC_USERNAME, m_staticUsername);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_EDIT_FORENAME, m_editForename);
	DDX_Control(pDX, IDC_EDIT_SURNAME, m_editSurname);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_CHECK_DOB, m_checkDOB);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DOB, m_DatePickerDOB);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo2);
	DDX_Control(pDX, IDC_STATIC_INFO3, m_staticInfo3);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo3);
	DDX_Control(pDX, IDC_STATIC_INFO4, m_staticInfo4);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo4);
	DDX_Control(pDX, IDC_STATIC_INFO5, m_staticInfo5);
	DDX_Control(pDX, IDC_EDIT_INFO5, m_editInfo5);
	DDX_Control(pDX, IDC_STATIC_INFO6, m_staticInfo6);
	DDX_Control(pDX, IDC_EDIT_INFO6, m_editInfo6);
	DDX_Control(pDX, IDC_STATIC_INFO7, m_staticInfo7);
	DDX_Control(pDX, IDC_EDIT_INFO7, m_editInfo7);
	DDX_Control(pDX, IDC_STATIC_INFO8, m_staticInfo8);
	DDX_Control(pDX, IDC_EDIT_INFO8, m_editInfo8);
	DDX_Control(pDX, IDC_CHECK_TICK1, m_checkTick1);
	DDX_Control(pDX, IDC_CHECK_TICK2, m_checkTick2);
	DDX_Control(pDX, IDC_CHECK_TICK3, m_checkTick3);
	DDX_Control(pDX, IDC_CHECK_TICK4, m_checkTick4);
	DDX_Control(pDX, IDC_EDIT_DIETARY, m_editDietary);
	DDX_Control(pDX, IDC_BUTTON_ALLERGIES, m_buttonAllergies);
	DDX_Control(pDX, IDC_EDIT_ALLERGIES, m_editAllergies);
	DDX_Control(pDX, IDC_CHECK_EXPIRY, m_checkExpiry);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EXPIRY, m_DatePickerExpiry);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_STATIC_USERTYPE1, m_staticUserType1);
	DDX_Control(pDX, IDC_COMBO_GROUP1, m_tabcomboGroup1);
	DDX_Control(pDX, IDC_BUTTON_SETGROUP, m_buttonSetGroup);
	DDX_Control(pDX, IDC_STATIC_USERTYPE2, m_staticUserType2);
	DDX_Control(pDX, IDC_COMBO_GROUP2, m_tabcomboGroup2);
	DDX_Control(pDX, IDC_STATIC_MEMBERID, m_staticMemberID);
	DDX_Control(pDX, IDC_EDIT_MEMBERID, m_editMemberID);
	DDX_Control(pDX, IDC_BUTTON_EXTREF_TOGGLE, m_buttonExtRefToggle);
	DDX_Control(pDX, IDC_STATIC_EXTREF, m_staticExtRef);
	DDX_Control(pDX, IDC_EDIT_EXTREF, m_editExtRef);
	DDX_Control(pDX, IDC_BUTTON_READER,  m_buttonReadCard);
	DDX_Control(pDX, IDC_STATIC_EXTREF2, m_staticExtRef2);
	DDX_Control(pDX, IDC_EDIT_EXTREF2, m_editExtRef2);
	DDX_Control(pDX, IDC_BUTTON_READER2, m_buttonReadCard2);
	DDX_Control(pDX, IDC_STATIC_BIOREG, m_staticBioReg);
	DDX_Control(pDX, IDC_EDIT_BIOREG, m_editBioReg);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERTTEXT, m_editAlert);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_STATIC_PREVIOUSCARDNO, m_staticPreviousCardNo);
	DDX_Control(pDX, IDC_EDIT_PREVIOUSCARDNO, m_editPreviousCardNo);
	DDX_Control(pDX, IDC_STATIC_NEXTCARDNO, m_staticNextCardNo);
	DDX_Control(pDX, IDC_EDIT_NEXTCARDNO, m_editNextCardNo);
	DDX_Control(pDX, IDC_CHECK_OWNMAXSPEND, m_checkOwnMaxSpend);
	DDX_Control(pDX, IDC_BUTTON_PHOTO, m_buttonPhoto);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staticPicture);
	DDX_Control(pDX, IDC_STATIC_NOIMAGE, m_staticNoImage);
	/*****/
}	

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditRecordPropertyPageDetail, CSSAutoShutdownPropertyPage)
	ON_STN_DBLCLK(IDC_STATIC_USERNAME, OnDoubleClickStaticUsername)
	ON_BN_CLICKED(IDC_BUTTON_ALLERGIES, OnButtonAllergies)
	ON_BN_CLICKED(IDC_CHECK_EXPIRY, OnToggleExpiry)
	ON_BN_CLICKED(IDC_CHECK_DOB, OnToggleDOB)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP1, OnSelectGroup1)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP2, OnSelectGroup2)
	ON_BN_CLICKED(IDC_BUTTON_SETGROUP, OnButtonSetGroup)
	ON_STN_DBLCLK(IDC_STATIC_EXTREF, OnDoubleClickStaticExtRef)
	ON_STN_DBLCLK(IDC_STATIC_EXTREF2, OnDoubleClickStaticExtRef2)
	ON_BN_CLICKED(IDC_BUTTON_EXTREF_TOGGLE, OnButtonExtRefToggle)
	ON_BN_CLICKED(IDC_BUTTON_READER,  OnButtonReader)
	ON_BN_CLICKED(IDC_BUTTON_READER2, OnButtonReader2)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_PHOTO, OnButtonPhoto)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//$$******************************************************************

void CEditRecordPropertyPageDetail::InitPhotoFilename()
{
	if (m_pLeaversData == NULL)									// check not leavers database
	{
		CString strUserID = m_SQLRowAccount.GetUserIDString();

		if (strUserID != "0")												// check to see if looking at cash account 0
		{																	// no 		
			CImageHandler photo(FALSE);
			if ((m_strPhotoFilename = photo.GetImageFilepath(strUserID)) == "")		// if no actual photo
			{
				m_strNoPhotoFilename = photo.GetImageFilepath("0");					// see if have default photo file
			}
		}
	}
	else															// using leavers database
	{
		CString strName = "";
		strName.Format("%s.*", 
			(const char*) System.FormatCardNo(m_SQLRowAccount.GetUserIDString()));

		CString strMask = m_pLeaversData->GetPhotoIDFilename(strName);	// P33\Leavers\2015-2015\PhotoId\nnnnnnn.* )

		CFileFind FileFinder;
		BOOL bWorking = FileFinder.FindFile(strMask);

		while (bWorking)
		{
			bWorking = FileFinder.FindNextFile();
			if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
			{
				continue;
			}

			CString strFilename = FileFinder.GetFileName();
			strFilename.MakeUpper();
			if (strFilename == "THUMBS.DB")								// skip windows generated file 
			{
				continue;
			}

			m_strPhotoFilename = FileFinder.GetFilePath();;
		}
	}
}

//$$******************************************************************

BOOL CEditRecordPropertyPageDetail::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_alert.Open(Filenames.GetAlertFilename());			// readonly
	m_allergy.Read();

	m_nAllergyCodes = m_SQLRowAccount.GetAllergyCodes();

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	SubclassEdit(IDC_EDIT_DOB, SS_DATE, 10, "");

	InitPhotoFilename();

	if (m_strPhotoFilename != "")
	{
		m_image.Load(m_strPhotoFilename);
		m_staticNoImage.ShowWindow(SW_HIDE);
	}
	else
	{
		if (m_strNoPhotoFilename != "")
		{
			m_image.Load(m_strNoPhotoFilename);
			m_staticNoImage.ShowWindow(SW_HIDE);
		}
		m_buttonPhoto.ShowWindow(SW_HIDE);
	}

	m_SQLRowGroup.SetGroupNo(m_SQLRowAccount.GetGroupNo());

	{
		CSQLRepositoryGroup RepoGroup;
		RepoGroup.SelectRow(m_SQLRowGroup, NULL);
	}
	m_strOriginalMemberID = m_SQLRowAccount.GetMemberID();
	m_strOriginalExtRef = m_SQLRowAccount.GetExternalRef1();
	m_strOriginalExtRef2 = m_SQLRowAccount.GetExternalRef2();

	m_editUsername.SetWindowText(m_SQLRowAccount.GetUsername());
	m_editForename.SetWindowText(m_SQLRowAccount.GetForename());
	m_editSurname.SetWindowText(m_SQLRowAccount.GetSurname());
	CStandardCombos::FillGenderCombo(m_comboGender, m_SQLRowAccount.GetGenderText());
	m_editEmail.SetWindowText(m_SQLRowAccount.GetEmail());
	m_editInfo1.SetWindowText(m_SQLRowAccount.GetInfo1());
	m_editInfo2.SetWindowText(m_SQLRowAccount.GetInfo2());
	m_editInfo3.SetWindowText(m_SQLRowAccount.GetInfo3());
	m_editInfo4.SetWindowText(m_SQLRowAccount.GetInfo4());
	m_editInfo5.SetWindowText(m_SQLRowAccount.GetInfo5());
	m_editInfo6.SetWindowText(m_SQLRowAccount.GetInfo6());
	m_editInfo7.SetWindowText(m_SQLRowAccount.GetInfo7());
	m_editInfo8.SetWindowText(m_SQLRowAccount.GetInfo8());
	m_checkTick1.SetCheck(m_SQLRowAccount.GetTick1());
	m_checkTick2.SetCheck(m_SQLRowAccount.GetTick2());
	m_checkTick3.SetCheck(m_SQLRowAccount.GetTick3());
	m_checkTick4.SetCheck(m_SQLRowAccount.GetTick4());
	m_editDietary.SetWindowText(m_SQLRowAccount.GetDietary());
	UpdateAllergyEditBox();

	//DOB
	{
		COleDateTime oleMin = CSQLFieldValidatorAccount::GetMinDob();
		COleDateTime oleMax = CSQLFieldValidatorAccount::GetMaxDob();
		m_DatePickerDOB.SetRange(&oleMin, &oleMax);

		COleDateTime oleDOB;
		bool bGotDOB = FALSE;
		if (CSQLFieldValidatorAccount::StringToOleDate(m_SQLRowAccount.GetDOB(), oleDOB) == TRUE)
		{
			if (CSQLFieldValidatorAccount::ValidateDOB(oleDOB) == TRUE)
			{
				m_checkDOB.SetCheck(TRUE);
				m_DatePickerDOB.SetTime(oleDOB);
				bGotDOB = TRUE;
			}
		}

		if (FALSE == bGotDOB)
		{
			m_checkDOB.SetCheck(FALSE);
			m_DatePickerDOB.SetTime(COleDateTime::GetCurrentTime());
		}
	}

	//EXPIRY
	{
		COleDateTime oleMin = CSQLFieldValidatorAccount::GetMinExpiry();
		COleDateTime oleMax = CSQLFieldValidatorAccount::GetMaxExpiry();
		m_DatePickerExpiry.SetRange(&oleMin, &oleMax);

		COleDateTime oleExpiry;
		bool bGotExpiry = FALSE;
		if (CSQLFieldValidatorAccount::StringToOleDate(m_SQLRowAccount.GetExpiryDate(), oleExpiry) == TRUE)
		{
			if (CSQLFieldValidatorAccount::ValidateExpiry(oleExpiry) == TRUE)
			{
				m_checkExpiry.SetCheck(TRUE);
				m_DatePickerExpiry.SetTime(oleExpiry);
				bGotExpiry = TRUE;
			}
		}

		if (FALSE == bGotExpiry)
		{
			m_checkExpiry.SetCheck(FALSE);
			m_DatePickerExpiry.SetTime(COleDateTime::GetCurrentTime());
		}
	}

	m_checkInactive.SetCheck(m_SQLRowAccount.GetInactive());

	//GROUP
	switch (m_SQLRowAccount.GetGroupShiftStatus())
	{
	case 1:
	{
		int nUsualGroup = m_SQLRowAccount.GetGroupShiftFutureGroup();
		int nShiftedGroup = m_SQLRowAccount.GetGroupNo();
		CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup1, nUsualGroup, FALSE);
		FillGroupShiftCombo(nUsualGroup, nShiftedGroup);
		MoveControl(&m_buttonSetGroup, 594, 58);
	}
	break;

	case 2:
	{
		int nUsualGroup = m_SQLRowAccount.GetGroupNo();
		int nShiftedGroup = m_SQLRowAccount.GetGroupShiftFutureGroup();
		CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup1, nUsualGroup, FALSE);
		FillGroupShiftCombo(nUsualGroup, nShiftedGroup);
	}
	break;

	case 0:
		CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup1, m_SQLRowAccount.GetGroupNo(), FALSE);
		break;
	}

	m_editMemberID.SetWindowText(m_strOriginalMemberID);
	m_editExtRef.SetWindowText(m_strOriginalExtRef);
	
	if (System.GetAllowPINNumbersFlag() == FALSE)
	{
		m_editExtRef2.SetWindowText(m_strOriginalExtRef2);
		m_buttonReadCard2.SetWindowText("Read Card");
	}
	else
	{
		m_editExtRef2.EnableWindow(TRUE);
		m_editExtRef2.SetReadOnly(TRUE);

		CSQLRowPINNumber RowPIN;
		RowPIN.SetUserID(m_SQLRowAccount.GetUserIDString());

		CSQLRepositoryPINNumber RepoPIN;
		if (RepoPIN.SelectRowByUserID(RowPIN, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			m_editExtRef2.SetWindowText(RowPIN.GetPINNumber());
		}
		else
		{
			m_editExtRef2.SetWindowText("");
		}

		m_buttonReadCard2.SetWindowText("New PIN");
	}

	if (m_SQLRowAccount.IsAlertSet() == FALSE)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		m_editAlert.SetWindowText(m_alert.GetText(m_SQLRowAccount.GetAlertCode()));
	}

	{
		CString strCardNo = System.FormatCardNo(m_SQLRowAccount.GetPreviousUserID(), TRUE);

		if (strCardNo == System.GetBlankCardNo())
		{
			strCardNo = "";
		}

		m_editPreviousCardNo.SetWindowText(strCardNo);
	}

	{
		CString strCardNo = System.FormatCardNo(m_SQLRowAccount.GetNextUserID(), TRUE);

		if (strCardNo == System.GetBlankCardNo())
		{
			strCardNo = "";
		}

		m_editNextCardNo.SetWindowText(strCardNo);
	}

	// Points & other fields
	TidyReferenceControls();
	
	bool bGeneralAllowed = (m_pLeaversData == NULL);

	if ((m_nMode == DB_READONLY) || (Passwords.IsEditGeneralAllowed() == FALSE))
	{
		bGeneralAllowed = FALSE;
		m_editUsername.EnableWindow(FALSE);
		m_editForename.EnableWindow(FALSE);
		m_editSurname.EnableWindow(FALSE);
	}

	m_DatePickerDOB.EnableWindow(IsTicked(m_checkDOB));
	m_DatePickerExpiry.EnableWindow(IsTicked(m_checkExpiry));
	m_checkOwnMaxSpend.SetCheck(m_SQLRowAccount.GetOwnMaxSpend());

	SelectCurrentGroup();									// sets RefreshAllowed flag

	CSQLRepositoryUserText Repo;
	ShowField(Repo.GetDOBText(), &m_checkDOB, &m_DatePickerDOB);
	ShowField(Repo.GetInfo1Text(), &m_staticInfo1, &m_editInfo1);
	ShowField(Repo.GetInfo2Text(), &m_staticInfo2, &m_editInfo2);
	ShowField(Repo.GetInfo3Text(), &m_staticInfo3, &m_editInfo3);
	ShowField(Repo.GetInfo4Text(), &m_staticInfo4, &m_editInfo4);
	ShowField(Repo.GetInfo5Text(), &m_staticInfo5, &m_editInfo5);
	ShowField(Repo.GetInfo6Text(), &m_staticInfo6, &m_editInfo6);
	ShowField(Repo.GetInfo7Text(), &m_staticInfo7, &m_editInfo7);
	ShowField(Repo.GetInfo8Text(), &m_staticInfo8, &m_editInfo8);
	ShowField(Repo.GetTick1Text(), &m_checkTick1, NULL);
	ShowField(Repo.GetTick2Text(), &m_checkTick2, NULL);
	ShowField(Repo.GetTick3Text(), &m_checkTick3, NULL);
	ShowField(Repo.GetTick4Text(), &m_checkTick4, NULL);
	m_staticExtRef.SetWindowText(Repo.GetExtRef1Text());
	
	if ( System.GetAllowPINNumbersFlag() == FALSE)
	{
		m_staticExtRef2.SetWindowText(Repo.GetExtRef2Text());
	}
	else
	{
		m_staticExtRef2.SetWindowText("PIN Number");
	}

	// Allergies
	if (m_allergy.GetAllergyCount() == 0)
	{
		m_buttonAllergies.ShowWindow(SW_HIDE);
		m_editAllergies.ShowWindow(SW_HIDE);
	}

	// Alert code & Next / Previous CardNo
	if (System.GetInterfaceType() != nINTERFACE_MIFAREv1)
	{
		m_editPreviousCardNo.ShowWindow(SW_HIDE);
		m_staticPreviousCardNo.ShowWindow(SW_HIDE);
		m_editNextCardNo.ShowWindow(SW_HIDE);
		m_staticNextCardNo.ShowWindow(SW_HIDE);
	}
	else
	{
		m_editPreviousCardNo.EnableWindow(FALSE);
		m_editNextCardNo.EnableWindow(FALSE);

		if (m_SQLRowAccount.IsAlertSet() == FALSE)
		{
			m_buttonAlert.EnableWindow(FALSE);
		}
	}

	if (FALSE == bGeneralAllowed)
	{
		m_tabcomboGroup1.EnableWindow(FALSE);
		m_buttonSetGroup.EnableWindow(FALSE);
		m_tabcomboGroup2.EnableWindow(FALSE);
		m_comboGender.EnableWindow(FALSE);
		m_checkDOB.EnableWindow(FALSE);
		m_DatePickerDOB.EnableWindow(FALSE);
		m_checkExpiry.EnableWindow(FALSE);
		m_DatePickerExpiry.EnableWindow(FALSE);
		m_editInfo1.EnableWindow(FALSE);
		m_editInfo2.EnableWindow(FALSE);
		m_editInfo3.EnableWindow(FALSE);
		m_editInfo4.EnableWindow(FALSE);
		m_editInfo5.EnableWindow(FALSE);
		m_editInfo6.EnableWindow(FALSE);
		m_editInfo7.EnableWindow(FALSE);
		m_editInfo8.EnableWindow(FALSE);
		m_editEmail.EnableWindow(FALSE);
		m_editDietary.EnableWindow(FALSE);
		m_checkInactive.EnableWindow(FALSE);
		m_buttonAlert.EnableWindow(FALSE);
		m_buttonAllergies.EnableWindow(FALSE);
	}

	return TRUE;
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::TidyReferenceControls()
{
	m_staticUserType2.ShowWindow(SW_HIDE);
	m_tabcomboGroup2.ShowWindow(SW_HIDE);
	m_staticMemberID.ShowWindow(SW_HIDE);
	m_editMemberID.ShowWindow(SW_HIDE);
	m_staticExtRef.ShowWindow(SW_HIDE);
	m_editExtRef.ShowWindow(SW_HIDE);
	m_buttonReadCard.ShowWindow(SW_HIDE);
	m_staticExtRef2.ShowWindow(SW_HIDE);
	m_editExtRef2.ShowWindow(SW_HIDE);
	m_buttonExtRefToggle.ShowWindow(SW_HIDE);
	m_buttonReadCard2.ShowWindow(SW_HIDE);
	m_staticBioReg.ShowWindow(SW_HIDE);
	m_editBioReg.ShowWindow(SW_HIDE);
	m_editAlert.ShowWindow(SW_HIDE);
	m_buttonAlert.ShowWindow(SW_HIDE);

	bool bShowExtRef1 = FALSE;
	bool bShowExtRef2 = FALSE;
	bool bShowMemberID = FALSE;
	bool bShowBioReg = FALSE;
	bool bEditMemberID = TRUE;
	CString strBioRegTime = "";

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_DUAL:
		bShowExtRef1 = TRUE;
		break;
	}

	CSQLRepositoryUserText Repo;
	if ( (Repo.IsExtRef2Reqd() == TRUE) || ( System.GetAllowPINNumbersFlag() == TRUE))
	{
		bShowExtRef2 = TRUE;
	}

	if (m_terminal.m_strTerminal == szTERMINAL_NONE)
	{
		bShowExtRef1 = FALSE;

		if (System.GetAllowPINNumbersFlag() == FALSE)
		{
			bShowExtRef2 = FALSE;
		}
	}

	// check editing leavers database
	if (m_pLeaversData != NULL)							
	{
		bShowExtRef1 = FALSE;
		bShowExtRef2 = FALSE;
		bEditMemberID = FALSE;
	}
	else
	{
		switch (System.GetInterfaceType())
		{
		case nINTERFACE_BIOMETRIC:
		case nINTERFACE_DUAL:
			{
				bShowBioReg = TRUE;

				CSQLRowBioRegister RowBioRegister;
				RowBioRegister.SetUserID(m_SQLRowAccount.GetUserID());

				CSQLRepositoryBioRegister Repo;
				if (Repo.SelectRow(RowBioRegister, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					m_staticUsername.SetWindowText("Bio User");
					
					CString strDate = RowBioRegister.GetRegDate();
					CString strTime = RowBioRegister.GetRegTime();
					
					if ((strDate.GetLength() == 8) && (strTime.GetLength() >= 4) && (RowBioRegister.GetUnRegFlag() == FALSE))
					{
						strBioRegTime.Format("%s/%s/%s @ %s:%s",
							(const char*)strDate.Left(2),
							(const char*)strDate.Mid(2, 2),
							(const char*)strDate.Right(4),
							(const char*)strTime.Left(2),
							(const char*)strTime.Mid(2, 2));
					}
				}
			}
			break;

		default:
			bShowBioReg = FALSE;
			break;
		}
	}

	if ((m_nMode == DB_READONLY) || (Passwords.IsEditGeneralAllowed() == FALSE))
	{
		bEditMemberID = FALSE;
		bShowExtRef1 = FALSE;
		bShowExtRef2 = FALSE;
	}

	{
		CString strText = Repo.GetMemberIDText();
		if (strText == "")
		{
			bShowMemberID = FALSE;
		}
		else
		{
			bShowMemberID = TRUE;
			m_staticMemberID.SetWindowText(strText);
			m_editMemberID.EnableWindow(bEditMemberID);
		}
	}

	CArray<int, int> arrayControls;

	if (m_SQLRowAccount.GetGroupShiftStatus() != 0)
	{
		arrayControls.Add(EDITDETAIL_EXTRAGROUP);
	}

	if (TRUE == bShowMemberID)
	{
		arrayControls.Add(EDITDETAIL_MEMBER);
	}

	if (TRUE == bShowExtRef1)
	{
		arrayControls.Add(EDITDETAIL_EXTREF1);
	}

	if (TRUE == bShowExtRef2)
	{
		arrayControls.Add(EDITDETAIL_EXTREF2);
	}

	if (TRUE == bShowBioReg)
	{
		arrayControls.Add(EDITDETAIL_BIOREG);
	}

	if (m_alert.GetAlertCount() != 0)
	{
		arrayControls.Add(EDITDETAIL_ALERT);
	}

	int nYPos = 60;

	for (int n = 0; n < arrayControls.GetSize(); n++)
	{
		switch (arrayControls.GetAt(n))
		{
		case EDITDETAIL_EXTRAGROUP:
			{
				m_staticUserType2.ShowWindow(SW_SHOW);
				m_tabcomboGroup2.ShowWindow(SW_SHOW);
				MoveControl(&m_staticUserType2, 355, nYPos);
				MoveControl(&m_tabcomboGroup2, 433, nYPos - 2);
				m_staticUserType1.SetWindowText("Usual Group");
				m_staticUserType2.SetWindowText("Shift Group");
			}
			break;

		case EDITDETAIL_MEMBER:
			{
				m_staticMemberID.ShowWindow(SW_SHOW);
				m_editMemberID.ShowWindow(SW_SHOW);
				MoveControl(&m_staticMemberID, 355, nYPos);
				MoveControl(&m_editMemberID, 433, nYPos - 2);
			}
			break;

		case EDITDETAIL_EXTREF1:
			{
				m_staticExtRef.ShowWindow(SW_SHOW);
				m_editExtRef.ShowWindow(SW_SHOW);
				m_buttonReadCard.ShowWindow(SW_SHOW);
				MoveControl(&m_staticExtRef, 355, nYPos);
				MoveControl(&m_editExtRef, 433, nYPos - 2);
				MoveControl(&m_buttonReadCard, 516, nYPos - 2);

				if ((arrayControls.GetSize() >= 5) && (TRUE == bShowExtRef2))
				{
					arrayControls.RemoveAt(n + 1);
					m_buttonExtRefToggle.ShowWindow(SW_SHOW);
					MoveControl(&m_buttonExtRefToggle, 331, nYPos - 2);
					MoveControl(&m_staticExtRef2, 355, nYPos);
					MoveControl(&m_editExtRef2, 433, nYPos - 2);
					MoveControl(&m_buttonReadCard2, 516, nYPos - 2);

					m_bEnableExtRefToggle = TRUE;
					m_nVisibleExtRef = 2;
					OnButtonExtRefToggle();
				}
			}
			break;

		case EDITDETAIL_EXTREF2:
			m_staticExtRef2.ShowWindow(SW_SHOW);
			m_editExtRef2.ShowWindow(SW_SHOW);
			m_buttonReadCard2.ShowWindow(SW_SHOW);
			MoveControl(&m_staticExtRef2, 355, nYPos);
			MoveControl(&m_editExtRef2, 433, nYPos - 2);
			MoveControl(&m_buttonReadCard2, 516, nYPos - 2);
			break;

		case EDITDETAIL_BIOREG:
			m_staticBioReg.ShowWindow(SW_SHOW);
			m_editBioReg.ShowWindow(SW_SHOW);
			MoveControl(&m_staticBioReg, 355, nYPos);
			MoveControl(&m_editBioReg, 433, nYPos - 2);
			m_editBioReg.SetWindowText(strBioRegTime);
			break;

		case EDITDETAIL_ALERT:
			m_buttonAlert.ShowWindow(SW_SHOW);
			m_editAlert.ShowWindow(SW_SHOW);
			MoveControl(&m_buttonAlert, 355, nYPos);
			MoveControl(&m_editAlert, 433, nYPos - 2);
			break;
		}

		nYPos += 20;
	}

	if (arrayControls.GetSize() >= 5)
	{
		m_staticPreviousCardNo.SetWindowText("Prev / Next Card");
		m_staticNextCardNo.ShowWindow(SW_HIDE);
		MoveControl(&m_staticPreviousCardNo, 355, 169);
		MoveControl(&m_editPreviousCardNo, 433, 167);
		MoveControl(&m_editNextCardNo, 493, 167);
	}
}

//$$********************************************************************

void CEditRecordPropertyPageDetail::FillGroupShiftCombo(int nUsualGroup, int nShiftedGroup)
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(nUsualGroup);
	
	{
		CSQLRepositoryGroup RepoGroup;
		RepoGroup.SelectRow(RowGroup, NULL);
	}

	CString strIn = RowGroup.GetShiftGroups();
	CString strOut = "";
	CString strError = "";
	RowGroup.ValidateShiftGroups(strIn, strOut, strError);

	CArray<int, int> arrayShiftGroups;

	CCSV csvGroups(strOut, ':');
	for (int n = 0; n < csvGroups.GetSize(); n++)
	{
		arrayShiftGroups.Add(csvGroups.GetInt(n));
	}

	CGroupShiftValidator::InsertIntoArray(arrayShiftGroups, nShiftedGroup);
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup2, arrayShiftGroups, nShiftedGroup);
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::DivideCardName(const char* szCardName, CString& strFirstName, CString& strSecondName)
{
	CString strFullName = szCardName;
	int nLength = strFullName.GetLength();

	strFirstName = "";
	strSecondName = "";

	int i;
	for (i = nLength - 1; i >= 0; i--)
	{
		char c = strFullName.GetAt(i);
		if (c == ' ')									// look for space in cardname ( 'Fred Smith' )
		{
			break;
		}

		strSecondName += c;								// compile surname backwards
	}
	strSecondName.MakeReverse();						// get surname right way round

	if (i > 0)										// see if any cardname left 
	{
		strFirstName = strFullName.Left(i);
	}
}

//$$********************************************************************

void CEditRecordPropertyPageDetail::OnDoubleClickStaticUsername()
{
	CString strUsername = GetEditBoxText(m_editUsername);
	CString strForename = GetEditBoxText(m_editForename);
	CString strSurname = GetEditBoxText(m_editSurname);

	if (strUsername != "")
	{
		if ((strForename == "") && (strSurname == ""))
		{
			strUsername.TrimLeft(' ');
			strUsername.TrimRight(' ');

			DivideCardName(strUsername, strForename, strSurname);		// assume Forename first
		}
		else
		{
			CString strBuf = strForename;					// swap forename & surname
			strForename = strSurname;
			strSurname = strBuf;
		}

		m_editUsername.SetWindowText(strUsername);
		m_editForename.SetWindowText(strForename);
		m_editSurname.SetWindowText(strSurname);
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnPaint()
{
	CPaintDC dc(this);

	if ((HBITMAP)m_image != NULL)
	{
		CRect rect;
		m_staticPicture.GetWindowRect(rect);
		ScreenToClient(&rect);

		//		if ( keep aspect ratio )
		{
			CPoint ptCentreFrame = rect.CenterPoint();

			double dcxMax = rect.Width();
			double dcyMax = rect.Height();
			double dAspectRatio = (double)m_image.GetWidth() / (double)m_image.GetHeight();
			double dcxNew = dcxMax;
			double dcyNew = dcxNew / dAspectRatio;

			if (dcyNew > dcyMax)
			{
				dcyNew = dcyMax;
				dcxNew = dcyNew * dAspectRatio;
			}
			rect.right = rect.left + (int)dcxNew;
			rect.bottom = rect.top + (int)dcyNew;

			CPoint ptCentreImage = rect.CenterPoint();
			int dx = ptCentreFrame.x - ptCentreImage.x;
			int dy = ptCentreFrame.y - ptCentreImage.y;
			rect.OffsetRect(dx, dy);
		}

		dc.SetStretchBltMode(COLORONCOLOR);
		m_image.StretchBlt(dc.m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), 0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY);
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::ShowField( CString strText, CWnd* pLabelWnd, CWnd* pEditWnd )
{
	if (strText == "")
	{
		pLabelWnd->ShowWindow(SW_HIDE);

		if (pEditWnd != NULL)
		{
			pEditWnd->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		pLabelWnd->SetWindowText(strText);
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::UpdateAllergyEditBox()
{
	// tranlate codes to a string
	m_allergy.SetAllergyCodes(m_nAllergyCodes);					
	m_editAllergies.SetWindowText( m_allergy.GetAllergyString() );
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonAllergies()
{
	CAllergiesDlg dlg(m_SQLRowAccount.GetUserIDString(), m_nAllergyCodes, this);
	if (dlg.DoModal() == IDOK)
	{
		m_nAllergyCodes = dlg.GetAllergyCodes();
		UpdateAllergyEditBox();
	}
}

//$$******************************************************************

CString CEditRecordPropertyPageDetail::GetExpiryDate()
{
	CString strExpiry = "";
	if (IsTicked(m_checkExpiry) == TRUE)
	{
		CTime time;
		m_DatePickerExpiry.GetTime(time);

		CSSDate date;
		date.SetDate(time);
		strExpiry = date.GetDate();
	}
	return strExpiry;
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::UpdateExpiryOptions()
{
	CString strExpiry = "";
	if (IsTicked(m_checkExpiry))
	{
		COleDateTime dateExpiry;
		m_DatePickerExpiry.GetTime(dateExpiry);

		if (CSQLFieldValidatorAccount::ValidateExpiry(dateExpiry) == TRUE)
		{
			strExpiry = CSQLFieldValidatorAccount::OleDateToString(dateExpiry);
		}
	}
	m_SQLRowAccount.SetExpiryDate(strExpiry);
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::UpdateDOBOptions()
{
	CString strDOB = "";
	if (IsTicked(m_checkDOB))
	{
		COleDateTime dateDOB;
		m_DatePickerDOB.GetTime(dateDOB);

		if (CSQLFieldValidatorAccount::ValidateDOB(dateDOB) == TRUE)
		{
			strDOB = CSQLFieldValidatorAccount::OleDateToString(dateDOB);
		}
	}
	m_SQLRowAccount.SetDOB(strDOB);
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnToggleExpiry()
{
	m_DatePickerExpiry.EnableWindow(IsTicked(m_checkExpiry));
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnToggleDOB()
{
	m_DatePickerDOB.EnableWindow(IsTicked(m_checkDOB));
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnSelectGroup1()
{
	if (m_SQLRowAccount.GetGroupShiftStatus() != 1)
	{
		SelectCurrentGroup();
	}

	if (m_SQLRowAccount.GetGroupShiftStatus() != 0)
	{
		int nUsualGroupNo = ExtractUsualGroupNo();
		int nShiftedGroupNo = ExtractShiftedGroupNo();
		FillGroupShiftCombo(nUsualGroupNo, nShiftedGroupNo);
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnSelectGroup2()
{
	if (m_SQLRowAccount.GetGroupShiftStatus() == 1)
	{
		SelectCurrentGroup();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::SelectCurrentGroup()
{
	bool bEnableOwnMaxSpend = FALSE;
	bool bShowMaxSpendTab = FALSE;
	bool bShowPurchaseControlTab = FALSE;

	CString strUserTypeMsg = "Group";

	m_SQLRowGroup.Reset();
	
	int nUsualGroupNo = ExtractUsualGroupNo();
	int nShiftedGroupNo = 0;

	switch (m_SQLRowAccount.GetGroupShiftStatus())
	{
	case 1:
		nShiftedGroupNo = ExtractShiftedGroupNo();
		m_SQLRowGroup.SetGroupNo(nShiftedGroupNo);
		break;
		
	case 2:
		nShiftedGroupNo = ExtractShiftedGroupNo();
		m_SQLRowGroup.SetGroupNo(nUsualGroupNo);
		break;

	case 0:
	default:
		m_SQLRowGroup.SetGroupNo(nUsualGroupNo);
		break;
	}

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(m_SQLRowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CUserTypeText usertext;
		usertext.Open(Filenames.GetUserTypeFilename());	// readonly
		strUserTypeMsg = usertext.GetText(m_SQLRowGroup.GetUserType());

		switch (m_SQLRowGroup.GetMaxSpendType())
		{
		case nGRPSPENDTYPE_DAILY_RECORD2:
			bEnableOwnMaxSpend = TRUE;
			break;

		case nGRPSPENDTYPE_PERIOD_LOCKED:
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			bShowMaxSpendTab = TRUE;
			break;
		}

		bShowPurchaseControlTab = m_SQLRowGroup.GetPurchaseControlFlag();
	}

	if (FALSE == bShowPurchaseControlTab)
	{
		CSQLRepositoryPurchaseControlStamp RepoStamp;
		bShowPurchaseControlTab = (RepoStamp.CountRowsByUserID(m_SQLRowAccount.GetUserID(), NULL).GetSQLResult() != 0);
	}

	if (m_SQLRowAccount.GetGroupShiftStatus() == 0)
	{
		m_staticUserType1.SetWindowText(strUserTypeMsg);
	}

	ShowAndEnableWindow(&m_checkOwnMaxSpend, bEnableOwnMaxSpend);

	bool bRedrawOptionalTabs = FALSE;

	if (TRUE == bShowMaxSpendTab)
	{
		bRedrawOptionalTabs |= ((CEditRecordPropertySheet*)m_pParentSheet)->AddMaxSpendTab();
	}
	else
	{
		bRedrawOptionalTabs |= ((CEditRecordPropertySheet*)m_pParentSheet)->RemoveMaxSpendTab();
	}

	if (TRUE == bShowPurchaseControlTab)
	{
		bRedrawOptionalTabs |= ((CEditRecordPropertySheet*)m_pParentSheet)->AddPurchaseControlTab();
	}
	else
	{
		bRedrawOptionalTabs |= ((CEditRecordPropertySheet*)m_pParentSheet)->RemovePurchaseControlTab();
	}

	if (TRUE == bRedrawOptionalTabs)
	{
		((CEditRecordPropertySheet*)m_pParentSheet)->AddOptionalTabs();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonSetGroup()
{
	switch (m_SQLRowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_DAILY:
	case nREFRESHTYPE_PERIOD:
		SetRefreshDays();
		break;

	default:
		SetAlternateGroup();
		break;
	}
}

//$$******************************************************************

int CEditRecordPropertyPageDetail::ExtractUsualGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup1, Group::GroupNo.Min);
}

//$$******************************************************************

int CEditRecordPropertyPageDetail::ExtractShiftedGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup2, Group::GroupNo.Min);
}

//$$******************************************************************

bool CEditRecordPropertyPageDetail::SetRefreshDays()
{
	bool bReply = FALSE;

	CRefreshDaysDlg dlg{};

	if (m_SQLRowAccount.IsRefreshSet() == TRUE)				// see if record refresh set
	{
		for (int n = 1; n <= 7; n++)
		{
			dlg.m_bDayFlag[n-1] = m_SQLRowAccount.IsRefreshAllowed(n);	// 1=sun, 2=mon...
		}
	}
	else													// use group default
	{
		for (int n = 1; n <= 7; n++)
		{
			dlg.m_bDayFlag[n - 1] = m_SQLRowGroup.IsRefreshAllowed(n-1);	// 1=sun, 2=mon...
		}
	}

	int nDayGroup = 0;

	// get group number in case it's just been changed
	int nDefaultGroupNo = (1 == m_SQLRowAccount.GetGroupShiftStatus()) ? ExtractShiftedGroupNo() : ExtractUsualGroupNo();				

	for (int n = 1; n <= 7; n++)
	{
		nDayGroup = m_SQLRowAccount.GetDailyGroupNo(n-1);
		dlg.m_nGroupDayNumber[n-1] = (nDayGroup == 0) ? nDefaultGroupNo : nDayGroup;
	}

	dlg.m_bGroupsAllowed = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		bReply = TRUE;

		for (int n = 1; n <= 7; n++)
		{
			nDayGroup = dlg.m_nGroupDayNumber[n - 1];
			if ((nDayGroup == nDefaultGroupNo) || (dlg.m_bDayFlag[n - 1] == FALSE))
			{
				m_SQLRowAccount.SetDailyGroupNo(n-1, 0);
			}
			else
			{
				m_SQLRowAccount.SetDailyGroupNo(n-1, nDayGroup);
			}
		}

		CString strMsg = "";

		if (dlg.HaveDaysSet() == FALSE)
		{
			strMsg = "Refresh days have been reset to Group defaults";
			m_SQLRowAccount.ClearRefreshFlags();
		}
		else
		{
			for (int n = 1; n <= 7; n++)
			{
				m_SQLRowAccount.SetRefreshAllowed(n, (dlg.m_bDayFlag[n-1] != 0));
			}

			if (ValidateRefreshDays() == FALSE)			// see if record settings are the same as Group settings
			{
				m_SQLRowAccount.ClearRefreshFlags();			// yes - clear record settings
				strMsg = "Refresh days are currently set to Group defaults";
			}
		}

		if (strMsg != "")
		{
			Prompter.Info(strMsg, "Daily Purse2 Refresh");
		}
	}

	return bReply;
}

//$$******************************************************************

bool CEditRecordPropertyPageDetail::SetAlternateGroup()
{
	bool bReply = FALSE;

	CSetDailyAlternateGroupDlg dlg;
	int nDayGroup = 0;
	int nDefaultGroupNo = (1 == m_SQLRowAccount.GetGroupShiftStatus()) ? ExtractShiftedGroupNo() : ExtractUsualGroupNo();

	for (int n = 1; n <= 7; n++)
	{
		nDayGroup = m_SQLRowAccount.GetDailyGroupNo(n-1);		
		dlg.m_nGroupDayNumber[n-1] = (nDayGroup == 0) ? nDefaultGroupNo : nDayGroup;
	}

	if (dlg.DoModal() == IDOK)
	{
		bReply = TRUE;

		for (int n = 1; n <= 7; n++)
		{
			nDayGroup = dlg.m_nGroupDayNumber[n-1];
			if (nDayGroup == nDefaultGroupNo)
			{
				m_SQLRowAccount.SetDailyGroupNo(n-1, 0);
			}
			else
			{
				m_SQLRowAccount.SetDailyGroupNo(n-1, nDayGroup);
			}
		}
	}

	return bReply;
}

//$$******************************************************************

bool CEditRecordPropertyPageDetail::ValidateRefreshDays()
{
	for (int n = 1; n <= 7; n++)
	{
		if (m_SQLRowAccount.IsRefreshAllowed(n) != m_SQLRowGroup.IsRefreshAllowed(n - 1))
		{
			return TRUE;	
		}
	}

	return FALSE;
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnDoubleClickStaticExtRef()
{
	if ( m_bReaderMode == FALSE )
	{
		m_editExtRef.EnableWindow(TRUE);
		m_editExtRef.SetFocus();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnDoubleClickStaticExtRef2()
{
	if ((m_bReaderMode == FALSE) && (System.GetAllowPINNumbersFlag() == TRUE))
	{
		m_editExtRef2.EnableWindow(TRUE);
		m_editExtRef2.SetFocus();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonExtRefToggle()
{
	if (TRUE == m_bEnableExtRefToggle)
	{
		if (1 == m_nVisibleExtRef)
		{
			ShowAndEnableWindow(&m_staticExtRef, FALSE);
			ShowAndEnableWindow(&m_editExtRef, FALSE);
			ShowAndEnableWindow(&m_buttonReadCard, FALSE);
			ShowAndEnableWindow(&m_staticExtRef2, TRUE);
			ShowAndEnableWindow(&m_editExtRef2, TRUE, FALSE);
			ShowAndEnableWindow(&m_buttonReadCard2, TRUE);
			m_nVisibleExtRef = 2;
		}
		else
		{
			ShowAndEnableWindow(&m_staticExtRef, TRUE);
			ShowAndEnableWindow(&m_editExtRef, TRUE, FALSE);
			ShowAndEnableWindow(&m_buttonReadCard, TRUE);
			ShowAndEnableWindow(&m_staticExtRef2, FALSE);
			ShowAndEnableWindow(&m_editExtRef2, FALSE);
			ShowAndEnableWindow(&m_buttonReadCard2, FALSE);
			m_nVisibleExtRef = 1;
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonReader()
{
	if ( m_bReaderMode == TRUE )
	{
		m_editExtRef.SetWindowText( m_strOriginalExtRef );								// put back Ext Ref 
		m_reader.Terminate();
	}
	else
	{
		m_buttonReadCard2.EnableWindow(FALSE);
		m_editExtRef.EnableWindow(FALSE);

		m_bReaderMode = TRUE;
		m_buttonReadCard.SetWindowText ( "Cancel" );

		if ( m_reader.SelectReader ( m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud ) == TRUE )
		{
			m_reader.SetUIDFormat ( m_terminal.m_nUIDFormat1 );
			m_reader.SetDataFormat ( m_terminal.m_nDataFormat1 );
			m_reader.m_pWnd = &m_editExtRef;						// NOTE: sets strExtRef variable to "Present card"
			m_reader.m_bWaitForCard = TRUE;

			while ( TRUE )
			{
				int nReply;
				if ( ( nReply = m_reader.GetUID ( TRUE, Server.GetLogCardReadsFlag() ) ) == 0 )	// 0=no error, 1=error, -1 = terminate
				{
					CString strMiFareNo = m_reader.GetCardNo();

					if (ValidateMiFareNo(strMiFareNo, 1) == TRUE)
					{
						m_editExtRef.SetWindowText( strMiFareNo);
					}

					break;
				}
				else if (nReply == -1)								// terminate 
				{
					break;
				}
				else
				{
					// error
					Prompter.Error(m_reader.GetError(), "Reader Error");
				}
			}

			m_reader.DeselectReader();
		}
		else
		{
			CString strError = m_reader.GetError();
			Prompter.Error( strError, "Hardware Error" );
		}
	}

	m_bReaderMode = FALSE;
	m_buttonReadCard.SetWindowText ( "Read Card" );
	m_buttonReadCard2.EnableWindow(TRUE);
}

//*******************************************************************

void CEditRecordPropertyPageDetail::OnButtonReader2()
{
	if (System.GetAllowPINNumbersFlag() == FALSE)
	{
		HandleButtonReader2();
	}
	else
	{
		HandleButtonNewPIN();
	}
}

//*******************************************************************

void CEditRecordPropertyPageDetail::HandleButtonReader2()
{
	if ( m_bReaderMode == TRUE )
	{
		m_editExtRef2.SetWindowText( m_strOriginalExtRef2 );
		m_reader.Terminate();
	}
	else
	{
		m_buttonReadCard.EnableWindow(FALSE);

		m_editExtRef2.EnableWindow(FALSE);

		m_bReaderMode = TRUE;
		m_buttonReadCard2.SetWindowText ( "Cancel" );

		if ( m_reader.SelectReader ( m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud ) == TRUE )
		{
			m_reader.SetUIDFormat ( m_terminal.m_nUIDFormat2 );
			m_reader.SetDataFormat ( m_terminal.m_nDataFormat2 );
			m_reader.m_pWnd = &m_editExtRef2;						// NOTE: sets strExtRef variable to "Present card"
			m_reader.m_bWaitForCard = TRUE;

			while ( TRUE )
			{
				int nReply;
				if ( ( nReply = m_reader.GetUID ( TRUE, Server.GetLogCardReadsFlag() ) ) == 0 )	// 0=no error, 1=error, -1 = terminate
				{
					CString strMiFareNo = m_reader.GetCardNo();

					if (ValidateMiFareNo(strMiFareNo, 2) == TRUE)
					{
						m_editExtRef2.SetWindowText( strMiFareNo );
					}

					break;
				}
				else if (nReply == -1)
				{
					break;
				}
				else
				{
					// error
					Prompter.Error(m_reader.GetError(), "Reader Error");
				}
			}

			m_reader.DeselectReader();
		}
		else
		{
			CString strError = m_reader.GetError();
			Prompter.Error( strError, "Hardware Error" );
		}
	}

	m_bReaderMode = FALSE;
	m_buttonReadCard2.SetWindowText ( "Read Card" );
	m_buttonReadCard.EnableWindow(TRUE);
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::HandleButtonNewPIN()
{
	if (Prompter.YesNo("Do you wish to set a new random PIN number for this account?", "Set New PIN") != IDYES)
	{
		return;
	}

	CString strError = "";
	CSQLRepositoryPINNumber RepoPINNumber;
	CString strPINNumber = RepoPINNumber.GetNewPINNumber(strError, NULL);

	if (strError != "")
	{
		Prompter.Error(strError, "PIN Assignment Error");
	}
	else
	{
		m_editExtRef2.SetWindowText(strPINNumber);
		m_strNewPINNumber = strPINNumber;
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonAlert()
{
	int nCode = m_SQLRowAccount.GetAlertCode();

	CAlertTextSelectDlg dlg(this);
	dlg.m_nAlertCode = nCode;

	if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
	{
		dlg.m_bClearAllowed = m_bEnableHotlistEdit;				// allow clear code if doing hidden editing for MiFare-v1
	}

	if (dlg.DoModal() == IDOK)
	{
		m_SQLRowAccount.SetUserAlert(dlg.m_nAlertCode);
		
		if (m_SQLRowAccount.IsAlertSet() == FALSE)
		{
			m_editAlert.SetWindowText("");
		}
		else
		{
			m_editAlert.SetWindowText(m_alert.GetText(m_SQLRowAccount.GetAlertCode()));
		}
	}
}

//$$******************************************************************
//05/06/2015 - v1.07c - MiFare-v1 - enable editing of database hotlist fields

void CEditRecordPropertyPageDetail::OnButtonAltKey1()
{
	if (m_bEnableHotlistEdit == FALSE)
	{
		if (Prompter.YesNo("Are you sure you wish to enable the editing of hotlisted references?", "Please confirm") == IDYES)
		{
			m_bEnableHotlistEdit = TRUE;

			m_editPreviousCardNo.EnableWindow(TRUE);
			m_editNextCardNo.EnableWindow(TRUE);
			m_buttonAlert.EnableWindow(TRUE);

			m_editPreviousCardNo.SetFocus();
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageDetail::OnButtonPhoto()
{
	CPhotoIDDlg dlg(m_SQLRowAccount, m_strPhotoFilename, GetEditBoxText(m_editForename), GetEditBoxText(m_editSurname), this);
	dlg.DoModal();
}

//$$******************************************************************

/* FIDDLE WAS THIS EVER USED ?
void CEditRecordPropertyPageDetail::OnButtonPurseViewRefresh()
{
	CPeriodRefreshListDlg dlg ( m_pDbSmartPay, &m_dbGroup, this );
	dlg.DoModal();
}
*/

//******************************************************************
//******************************************************************
//******************************************************************

void CEditRecordPropertyPageDetail::SaveChanges (CSQLRowAccountFull& Row)
{
	// leavers database no Memebers \ ExtRef database
	if (m_pLeaversData != NULL)										
	{
		return;
	}

// Check Member ID

	CSQLRepositoryAccount RepoAccount;

	CString strMemberID = Row.GetMemberID();
	if (m_strOriginalMemberID != strMemberID)							
	{
		RepoAccount.RemoveMemberID(strMemberID,NULL);
	}

// Check External Ref1

	CString strExtRef1 = Row.GetExternalRef1();
	if (m_strOriginalExtRef != strExtRef1)							
	{
		RepoAccount.RemoveExtRef1(strExtRef1,NULL);
	}

// Check External Ref2

	CString strExtRef2 = Row.GetExternalRef2();
	if (m_strOriginalExtRef2 != strExtRef2)
	{
		RepoAccount.RemoveExtRef2(strExtRef2,NULL);
	}
}

//*******************************************************************

bool CEditRecordPropertyPageDetail::IsHotlisted()
{
	bool bResult = FALSE;

	if (m_SQLRowAccount.IsAlertSet() == TRUE)
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			bResult = TRUE;
		}
	}
	
	return bResult;
}

//*******************************************************************

bool CEditRecordPropertyPageDetail::ValidateMiFareNo ( const char* szMiFareNo, int nMiFareDbNo )
{
	CString strMiFareNo = szMiFareNo;
	CString strErrorMsg = "";
	CSQLRepositoryUserText Repo;
	CString strExtRefText = ( nMiFareDbNo == 2 ) ? Repo.GetExtRef2Text() : Repo.GetExtRef1Text();

	if (strMiFareNo != "")
	{
		CString strLocateOn = (nMiFareDbNo == 2) ? Account::ExternalRef2.Label : Account::ExternalRef1.Label;

		CSQLRepositoryAccount RepoAccount;
		__int64 nUserID = RepoAccount.LookupIndex(strLocateOn, strMiFareNo, NULL).GetSQLResult();
		
		if ( ( nUserID != 0 ) && ( nUserID != m_SQLRowAccount.GetUserID()))
		{ 
			strErrorMsg.Format("ATTENTION : %s CardID already assigned!\n\nCardID %s is currently assigned to account %I64d!", 
				(const char*) strExtRefText, 
				(const char*) strMiFareNo,
				nUserID);
		}
	}

	if (strErrorMsg == "")
	{
		return TRUE;
	}

	CString strBuf = "";
	strBuf.Format ( "\n\nThe %s reference will remain unchanged!", 
		(const char*) strExtRefText );

	strErrorMsg += strBuf;

	Prompter.Error( strErrorMsg, "Database Access Error" );
	return FALSE;
}

//*****************************************************************************

void CEditRecordPropertyPageDetail::OnOK()
{
	if (m_bReaderMode == TRUE)
	{
		m_reader.Terminate();
		m_editExtRef.SetWindowText(m_strOriginalExtRef);
		m_editExtRef2.SetWindowText(m_strOriginalExtRef2);
		UpdateData(FALSE);
	}

	if (UpdateData() == TRUE)
	{
		CString strMemberID = GetEditBoxText(m_editMemberID);
		CString strExtRef = GetEditBoxText(m_editExtRef);
		CString strExtRef2 = GetEditBoxText(m_editExtRef2);

		::TrimSpacesFromString(strMemberID);
		::TrimSpacesFromString(strExtRef);
		::TrimSpacesFromString(strExtRef2);

		m_SQLRowAccount.SetUsername(GetEditBoxText(m_editUsername));
		m_SQLRowAccount.SetForename(GetEditBoxText(m_editForename));
		m_SQLRowAccount.SetSurname(GetEditBoxText(m_editSurname));

		{
			CString strGender = "";
			m_comboGender.GetWindowText(strGender);
			m_SQLRowAccount.SetGender(strGender);
		}

		UpdateDOBOptions();
		UpdateExpiryOptions();
		m_SQLRowAccount.SetEmail(GetEditBoxText(m_editEmail));
		m_SQLRowAccount.SetInfo1(GetEditBoxText(m_editInfo1));
		m_SQLRowAccount.SetInfo2(GetEditBoxText(m_editInfo2));
		m_SQLRowAccount.SetInfo3(GetEditBoxText(m_editInfo3));
		m_SQLRowAccount.SetInfo4(GetEditBoxText(m_editInfo4));
		m_SQLRowAccount.SetInfo5(GetEditBoxText(m_editInfo5));
		m_SQLRowAccount.SetInfo6(GetEditBoxText(m_editInfo6));
		m_SQLRowAccount.SetInfo7(GetEditBoxText(m_editInfo7));
		m_SQLRowAccount.SetInfo8(GetEditBoxText(m_editInfo8));
		m_SQLRowAccount.SetTick1(IsTicked(m_checkTick1));
		m_SQLRowAccount.SetTick2(IsTicked(m_checkTick2));
		m_SQLRowAccount.SetTick3(IsTicked(m_checkTick3));
		m_SQLRowAccount.SetTick4(IsTicked(m_checkTick4));
		m_SQLRowAccount.SetDietary(GetEditBoxText(m_editDietary));
		m_SQLRowAccount.SetAllergyCodes(m_nAllergyCodes);
		m_SQLRowAccount.SetInactive(IsTicked(m_checkInactive));
		
		switch (m_SQLRowAccount.GetGroupShiftStatus())
		{
		case 1:
			{
				int nUsualGroup = ExtractUsualGroupNo();
				int nShiftedGroup = ExtractShiftedGroupNo();
				m_SQLRowAccount.SetGroupNo(nShiftedGroup);
				m_SQLRowAccount.SetGroupShiftFutureGroup(nUsualGroup);
				m_pGroupShiftValidator->SetEditedGroupNow(nShiftedGroup);
				m_pGroupShiftValidator->SetEditedGroupShiftFutureGroup(nUsualGroup);
			}
			break;

		case 2:
			{
				int nUsualGroup = ExtractUsualGroupNo();
				int nShiftedGroup = ExtractShiftedGroupNo();
				m_SQLRowAccount.SetGroupNo(nUsualGroup);
				m_SQLRowAccount.SetGroupShiftFutureGroup(nShiftedGroup);
				m_pGroupShiftValidator->SetEditedGroupNow(nUsualGroup);
				m_pGroupShiftValidator->SetEditedGroupShiftFutureGroup(nShiftedGroup);
			}
			break;

		case 0:
		default:
			m_SQLRowAccount.SetGroupNo(ExtractUsualGroupNo());
			break;
		}

		switch (m_SQLRowAccount.GetGroupShiftStatus())
		{
		case 1:
		case 2:
			{
				CSQLRowAccountFull AccountDummy;
				if (m_pGroupShiftValidator->UpdateAccount(AccountDummy) == TRUE)
				{
					CGroupShiftLogger Logger;
					Logger.LogManualChange(m_SQLRowAccount, *m_pGroupShiftValidator, 4);
				}
			}
			break;
		}

		m_SQLRowAccount.SetMemberID(strMemberID);
		m_SQLRowAccount.SetExternalRef1(strExtRef);
		m_SQLRowAccount.SetExternalRef2(strExtRef2);

		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_SQLRowAccount.SetPreviousUserID(GetEditBoxInt64(m_editPreviousCardNo));
			m_SQLRowAccount.SetNextUserID(GetEditBoxInt64(m_editNextCardNo));
		}

		if (m_checkOwnMaxSpend.IsWindowEnabled() == TRUE)
		{
			m_SQLRowAccount.SetOwnMaxSpend(m_checkOwnMaxSpend.GetCheck() != 0);
		}

		CPropertyPage::OnOK();
	}
}

//********************************************************************

BOOL CEditRecordPropertyPageDetail::OnKillActive()
{
	m_pEditAccountBuffer->m_strCardName = GetEditBoxText(m_editUsername);
	
	if (m_SQLRowAccount.GetGroupShiftStatus() == 1)
	{
		m_pEditAccountBuffer->m_nGroupNo = ExtractShiftedGroupNo();
	}
	else
	{
		m_pEditAccountBuffer->m_nGroupNo = ExtractUsualGroupNo();
	}
	
	m_pEditAccountBuffer->m_strNewPINNumber = m_strNewPINNumber;
	m_pEditAccountBuffer->m_bOwnMaxSpend = IsTicked(m_checkOwnMaxSpend);
	return CSSPropertyPage::OnKillActive();
}

//********************************************************************

BOOL CEditRecordPropertyPageDetail::OnQueryCancel()
{
	return CEditRecordPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditRecordPropertyPageDetail::OnCancel()
{
	if (m_bReaderMode == TRUE)
	{
		m_reader.Terminate();
	}
	else
	{
		CSSPropertyPage::OnCancel();	
	}
}

//********************************************************************
