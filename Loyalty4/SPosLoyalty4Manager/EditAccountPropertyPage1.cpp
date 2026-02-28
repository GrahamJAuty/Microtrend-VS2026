//$$******************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
//$$******************************************************************
#include "EditAccountPropertySheet.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "PhotoIDEditDlg.h"
#include "SelectAlertDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "EditAccountPropertyPage1.h"
//$$******************************************************************

CEditAccountPropertyPage1::CEditAccountPropertyPage1(CSQLRowAccountFull& RowAccount)
	: CSSPropertyPage(CEditAccountPropertyPage1::IDD), m_RowAccount(RowAccount)
{
	m_pParentSheet = NULL;
	m_pEditAccountBuffer = NULL;

	m_bReaderMode = FALSE;
	m_pPhotoIDFilenameStack = NULL;
	m_strAlertCodes = "";
	m_alertText.Open(Filenames.GetAlertTextFilename());
	m_terminal.Read();

	m_bDateInfo1 = FALSE;
	m_bDateInfo2 = FALSE;
	m_bDateInfo3 = FALSE;
	m_bDateInfo4 = FALSE;
	m_bShowQR = TRUE;
	m_bAllowExtRef = FALSE;
	m_bManualEditExtRef = FALSE;
}

//$$******************************************************************

CEditAccountPropertyPage1::~CEditAccountPropertyPage1()
{
	if ((HBITMAP)m_image != NULL)
	{
		m_image.Destroy();
	}
}

//$$******************************************************************

void CEditAccountPropertyPage1::SetPointer ( void* pParentSheet, CEditAccountBuffer* pEditAccountBuffer )
{
	m_pParentSheet = pParentSheet;
	m_pEditAccountBuffer = pEditAccountBuffer;
}

//$$******************************************************************

void CEditAccountPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_USERNAME, m_staticUsername);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_STATIC_FORENAME, m_staticForename);
	DDX_Control(pDX, IDC_EDIT_FORENAME, m_editForename);
	DDX_Control(pDX, IDC_STATIC_SURNAME, m_staticSurname);
	DDX_Control(pDX, IDC_EDIT_SURNAME, m_editSurname);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_CHECK_ANNIVERSARY, m_checkDOB);
	DDX_Control(pDX, IDC_DATEPICKER_ANNIVERSARY, m_datePickerDOB);
	DDX_Control(pDX, IDC_STATIC_ADD1, m_staticAddress1);
	DDX_Control(pDX, IDC_STATIC_ADD2, m_staticAddress2);
	DDX_Control(pDX, IDC_STATIC_ADD3, m_staticAddress3);
	DDX_Control(pDX, IDC_STATIC_ADD4, m_staticAddress4);
	DDX_Control(pDX, IDC_STATIC_ADD5, m_staticAddress5);
	DDX_Control(pDX, IDC_STATIC_TEL1, m_staticTel1);
	DDX_Control(pDX, IDC_STATIC_TEL2, m_staticTel2);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDC_STATIC_INFO3, m_staticInfo3);
	DDX_Control(pDX, IDC_STATIC_INFO4, m_staticInfo4);
	DDX_Control(pDX, IDC_CHECK_INFO1, m_checkInfo1);
	DDX_Control(pDX, IDC_CHECK_INFO2, m_checkInfo2);
	DDX_Control(pDX, IDC_CHECK_INFO3, m_checkInfo3);
	DDX_Control(pDX, IDC_CHECK_INFO4, m_checkInfo4);
	DDX_Control(pDX, IDC_EDIT_ADD1, m_editAddress1);
	DDX_Control(pDX, IDC_EDIT_ADD2, m_editAddress2);
	DDX_Control(pDX, IDC_EDIT_ADD3, m_editAddress3);
	DDX_Control(pDX, IDC_EDIT_ADD4, m_editAddress4);
	DDX_Control(pDX, IDC_EDIT_ADD5, m_editAddress5);
	DDX_Control(pDX, IDC_EDIT_TEL1, m_editTel1);
	DDX_Control(pDX, IDC_EDIT_TEL2, m_editTel2);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo1);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo2);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo3);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo4);
	DDX_Control(pDX, IDC_DATEPICKER_INFO1, m_datePickerInfo1);
	DDX_Control(pDX, IDC_DATEPICKER_INFO2, m_datePickerInfo2);
	DDX_Control(pDX, IDC_DATEPICKER_INFO3, m_datePickerInfo3);
	DDX_Control(pDX, IDC_DATEPICKER_INFO4, m_datePickerInfo4);
	DDX_Control(pDX, IDC_CHECK_EXPIRY, m_checkExpiry);
	DDX_Control(pDX, IDC_DATEPICKER_EXPIRY, m_datePickerExpiry);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_SCHEME, m_comboScheme);
	DDX_Control(pDX, IDC_BUTTON_QR, m_buttonQR);
	DDX_Control(pDX, IDC_STATIC_EXTREF, m_staticExtRef);
	DDX_Control(pDX, IDC_EDIT_EXTREF, m_editExtRef);
	DDX_Control(pDX, IDC_EDIT_QRINFO, m_editQRInfo);
	DDX_Control(pDX, IDC_BUTTON_EXTREF, m_buttonExtRef);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERTTEXT, m_editAlert);
	DDX_Control(pDX, IDC_STATIC_HOTLIST, m_staticHotlist);
	DDX_Control(pDX, IDC_EDIT_HOTLIST_DATE, m_editHotlistDate);
	DDX_Control(pDX, IDC_EDIT_HOTLIST_TIME, m_editHotlistTime);
	DDX_Control(pDX, IDC_STATIC_PREVIOUS, m_staticPrevious);
	DDX_Control(pDX, IDC_EDIT_PREVIOUS, m_editPrevious);
	DDX_Control(pDX, IDC_STATIC_NEXT, m_staticNext);
	DDX_Control(pDX, IDC_EDIT_NEXT, m_editNext);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staticPicture);
	DDX_Control(pDX, IDC_STATIC_NOIMAGE, m_staticNoPicture);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CRightClickStatic, CStatic)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditAccountPropertyPage1, CSSPropertyPage)
	ON_WM_PAINT()
	ON_STN_DBLCLK(IDC_STATIC_USERNAME, OnDoubleClickStaticUsername)
	ON_BN_CLICKED(IDC_CHECK_ANNIVERSARY, OnToggleDOB)
	ON_BN_CLICKED(IDC_CHECK_EXPIRY, OnToggleExpiry)
	ON_BN_CLICKED(IDC_CHECK_INFO1, OnToggleInfo1)
	ON_BN_CLICKED(IDC_CHECK_INFO2, OnToggleInfo2)
	ON_BN_CLICKED(IDC_CHECK_INFO3, OnToggleInfo3)
	ON_BN_CLICKED(IDC_CHECK_INFO4, OnToggleInfo4)
	ON_BN_CLICKED(IDC_BUTTON_QR, OnButtonQR)
	ON_BN_CLICKED(IDC_BUTTON_EXTREF, OnButtonExtRef)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOID, OnButtonPhotoID)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_MESSAGE(WM_APP + 1, OnRightClickExtRef)
END_MESSAGE_MAP()

//$$******************************************************************

LRESULT CEditAccountPropertyPage1::OnRightClickExtRef(WPARAM wParam, LPARAM lParam)
{
	if ( (FALSE == m_bShowQR) && ( TRUE == m_bAllowExtRef ) && ( FALSE == m_bManualEditExtRef ) )
	{
		if (FALSE == m_bReaderMode)
		{
			m_bManualEditExtRef = TRUE;
			m_editExtRef.EnableWindow(TRUE);
			m_editExtRef.SetFocus();
		}
	}

	return 0l;
}

//$$******************************************************************

void CEditAccountPropertyPage1::LoadDate(CButton& checkDate, CDateTimeCtrl& datePicker, CString strDate, bool bDOB)
{
	if (TRUE == bDOB)
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinDob();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxDob();
		datePicker.SetRange(&oleMin, &oleMax);
	}
	else
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinExpiry();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxExpiry();
		datePicker.SetRange(&oleMin, &oleMax);
	}

	COleDateTime oleDate;
	bool bGotDate = FALSE;
	if (CAccountFieldValidator::StringToOleDate(strDate, oleDate) == TRUE)
	{
		bool bValid = FALSE;
		if (TRUE == bDOB)
		{
			bValid = CAccountFieldValidator::ValidateDOB(oleDate);
		}
		else
		{
			bValid = CAccountFieldValidator::ValidateExpiry(oleDate);
		}

		if ( TRUE == bValid)
		{
			checkDate.SetCheck(TRUE);
			datePicker.SetTime(oleDate);
			bGotDate = TRUE;
		}
	}

	if (FALSE == bGotDate)
	{
		checkDate.SetCheck(FALSE);
		datePicker.SetTime(COleDateTime::GetCurrentTime());
	}
}

//$$******************************************************************

BOOL CEditAccountPropertyPage1::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_texts.Read();
	m_bDateInfo1 = m_texts.GetDateFlag(LOYALTYFIELD_INFO1);
	m_bDateInfo2 = m_texts.GetDateFlag(LOYALTYFIELD_INFO2);
	m_bDateInfo3 = m_texts.GetDateFlag(LOYALTYFIELD_INFO3);
	m_bDateInfo4 = m_texts.GetDateFlag(LOYALTYFIELD_INFO4);

	//ACCOUNT NAME
	InitStaticText(m_staticUsername, LOYALTYFIELD_FULLNAME);
	InitStaticText(m_staticForename, LOYALTYFIELD_FORENAME);
	InitStaticText(m_staticSurname, LOYALTYFIELD_SURNAME);
	InitStaticText(m_staticAddress1, LOYALTYFIELD_ADDRESS1);
	InitStaticText(m_staticAddress2, LOYALTYFIELD_ADDRESS2);
	InitStaticText(m_staticAddress3, LOYALTYFIELD_ADDRESS3);
	InitStaticText(m_staticAddress4, LOYALTYFIELD_ADDRESS4);
	InitStaticText(m_staticAddress5, LOYALTYFIELD_ADDRESS5);
	InitStaticText(m_staticTel1, LOYALTYFIELD_PHONE1);
	InitStaticText(m_staticTel2, LOYALTYFIELD_PHONE2);
	InitStaticText(m_staticInfo1, LOYALTYFIELD_INFO1);
	InitStaticText(m_staticInfo2, LOYALTYFIELD_INFO2);
	InitStaticText(m_staticInfo3, LOYALTYFIELD_INFO3);
	InitStaticText(m_staticInfo4, LOYALTYFIELD_INFO4);

	InitCheckBoxText(m_checkDOB, LOYALTYFIELD_DOB, "");
	InitCheckBoxText(m_checkInfo1, LOYALTYFIELD_INFO1, "Info1");
	InitCheckBoxText(m_checkInfo2, LOYALTYFIELD_INFO2, "Info2");
	InitCheckBoxText(m_checkInfo3, LOYALTYFIELD_INFO3, "Info3");
	InitCheckBoxText(m_checkInfo4, LOYALTYFIELD_INFO4, "Info4");

	MoveControl(&m_checkInfo1, 17, 260);
	MoveControl(&m_checkInfo2, 17, 280);
	MoveControl(&m_checkInfo3, 17, 300);
	MoveControl(&m_checkInfo4, 17, 320);

	MoveControl(&m_datePickerInfo1, 79, 258);
	MoveControl(&m_datePickerInfo2, 79, 278);
	MoveControl(&m_datePickerInfo3, 79, 298);
	MoveControl(&m_datePickerInfo4, 79, 318);

	if (FALSE == m_bDateInfo1)
	{
		ShowAndEnableWindow(&m_checkInfo1, FALSE);
		ShowAndEnableWindow(&m_datePickerInfo1, FALSE);
	}
	else
	{
		ShowAndEnableWindow(&m_staticInfo1, FALSE);
		ShowAndEnableWindow(&m_editInfo1, FALSE);
	}

	if (FALSE == m_bDateInfo2)
	{
		ShowAndEnableWindow(&m_checkInfo2, FALSE);
		ShowAndEnableWindow(&m_datePickerInfo2, FALSE);
	}
	else
	{
		ShowAndEnableWindow(&m_staticInfo2, FALSE);
		ShowAndEnableWindow(&m_editInfo2, FALSE);
	}

	if (FALSE == m_bDateInfo3)
	{
		ShowAndEnableWindow(&m_checkInfo3, FALSE);
		ShowAndEnableWindow(&m_datePickerInfo3, FALSE);
	}
	else
	{
		ShowAndEnableWindow(&m_staticInfo3, FALSE);
		ShowAndEnableWindow(&m_editInfo3, FALSE);
	}

	if (FALSE == m_bDateInfo4)
	{
		ShowAndEnableWindow(&m_checkInfo4, FALSE);
		ShowAndEnableWindow(&m_datePickerInfo4, FALSE);
	}
	else
	{
		ShowAndEnableWindow(&m_staticInfo4, FALSE);
		ShowAndEnableWindow(&m_editInfo4, FALSE);
	}

	//USER NAME
	m_editUsername.SetWindowText(m_RowAccount.GetFullname());
	m_editForename.SetWindowText(m_RowAccount.GetForename());
	m_editSurname.SetWindowText(m_RowAccount.GetSurname());

	//GENDER
	CStandardCombos::FillGenderCombo(m_comboGender, m_RowAccount.GetGenderText());

	//DOB
	LoadDate(m_checkDOB, m_datePickerDOB, m_RowAccount.GetDOB(), TRUE);
	
	//ADDRESS AND INFO
	m_editAddress1.SetWindowText(m_RowAccount.GetAddress1());
	m_editAddress2.SetWindowText(m_RowAccount.GetAddress2());
	m_editAddress3.SetWindowText(m_RowAccount.GetAddress3());
	m_editAddress4.SetWindowText(m_RowAccount.GetAddress4());
	m_editAddress5.SetWindowText(m_RowAccount.GetAddress5());
	m_editTel1.SetWindowText(m_RowAccount.GetPhone1());
	m_editTel2.SetWindowText(m_RowAccount.GetPhone2());
	
	if (FALSE == m_bDateInfo1)
	{
		m_editInfo1.SetWindowText(m_RowAccount.GetInfo1());
	}
	else
	{
		LoadDate(m_checkInfo1, m_datePickerInfo1, m_RowAccount.GetInfo1(), FALSE);
	}

	if (FALSE == m_bDateInfo2)
	{
		m_editInfo2.SetWindowText(m_RowAccount.GetInfo2());
	}
	else
	{
		LoadDate(m_checkInfo2, m_datePickerInfo2, m_RowAccount.GetInfo2(), FALSE);
	}

	if (FALSE == m_bDateInfo3)
	{
		m_editInfo3.SetWindowText(m_RowAccount.GetInfo3());
	}
	else
	{
		LoadDate(m_checkInfo3, m_datePickerInfo3, m_RowAccount.GetInfo3(), FALSE);
	}

	if (FALSE == m_bDateInfo4)
	{
		m_editInfo4.SetWindowText(m_RowAccount.GetInfo4());
	}
	else
	{
		LoadDate(m_checkInfo4, m_datePickerInfo4, m_RowAccount.GetInfo4(), FALSE);
	}
	
	//EXPIRY
	LoadDate(m_checkExpiry, m_datePickerExpiry, m_RowAccount.GetExpiryDate(), FALSE);

	//INACTIVE
	m_checkInactive.SetCheck(m_RowAccount.GetInactive());

	//GROUP
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_RowAccount.GetGroupNo(), FALSE);
	CGroupComboHelpers::FillSchemeCombo(m_comboScheme, m_RowAccount.GetGroupNo(), m_RowAccount.GetSchemeNo(), TRUE, TRUE);

	//EXTERNAL REFERENCE
	m_strCurrentExtRef = m_RowAccount.GetExternalRef();
	m_strCurrentQRInfo = m_RowAccount.GetQRInfo();
	m_editExtRef.SetWindowText(m_strCurrentExtRef);
	m_editQRInfo.SetWindowText(m_RowAccount.GetQRInfo());

	m_bShowQR = TRUE;
	m_bAllowExtRef = FALSE;
	switch (System.GetInterfaceType())
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		m_bShowQR = System.GetShowQRInfoFlag();
		m_bAllowExtRef = TRUE;
		break;
	}

	if ( FALSE == m_bShowQR)
	{
		CUserTexts texts;
		texts.Read();
		m_staticExtRef.SetWindowText(texts.GetText(LOYALTYFIELD_EXTERNALREF, ""));
		ShowAndEnableWindow(&m_editExtRef, TRUE, FALSE);
		ShowAndEnableWindow(&m_buttonExtRef, TRUE, TRUE);
		ShowAndEnableWindow(&m_editQRInfo, FALSE);
	}
	else
	{
		m_staticExtRef.SetWindowText("QR Info" );
		ShowAndEnableWindow(&m_editExtRef, FALSE);
		ShowAndEnableWindow(&m_buttonExtRef, FALSE);
		ShowAndEnableWindow(&m_editQRInfo, TRUE);
	}

	MoveControl(&m_editQRInfo, 433, 78, 157, 14);

	if (FALSE == m_bAllowExtRef)
	{
		ShowAndEnableWindow(&m_buttonQR, FALSE);
	}

	//ALERT CODES
	m_strAlertCodes = m_RowAccount.GetAlertCodes();
	m_editAlert.SetWindowText(m_alertText.GetDisplayText(m_strAlertCodes));

	if (m_alertText.GetAlertCount() == 0)
	{
		m_buttonAlert.ShowWindow(SW_HIDE);
		m_editAlert.ShowWindow(SW_HIDE);
	}

	//MISC INFORMATION
	{
		bool bShowHotlisted = m_RowAccount.IsCardHotlisted();
		bool bShowPrevious = m_RowAccount.HavePreviousUserID();
		bool bShowNext = m_RowAccount.HaveNextUserID();

		if (TRUE == bShowHotlisted)
		{
			switch (System.GetInterfaceType())
			{
			case nINTERFACE_MIFAREv2:
			case nINTERFACE_MIFAREv3:
				break;

			default:
				bShowPrevious = TRUE;
				bShowNext = TRUE;
				break;
			}
		}

		ShowAndEnableWindow(&m_staticHotlist, bShowHotlisted);
		ShowAndEnableWindow(&m_editHotlistDate, bShowHotlisted);
		ShowAndEnableWindow(&m_editHotlistTime, bShowHotlisted);
		ShowAndEnableWindow(&m_staticPrevious, bShowPrevious);
		ShowAndEnableWindow(&m_editPrevious, bShowPrevious);
		ShowAndEnableWindow(&m_staticNext, bShowNext);
		ShowAndEnableWindow(&m_editNext, bShowNext);

		m_editHotlistDate.SetWindowText(m_RowAccount.GetHotlistDate());
		m_editHotlistTime.SetWindowText(m_RowAccount.GetHotlistTime());
		m_editPrevious.SetWindowText(SimpleFormatInt64Value(m_RowAccount.GetPreviousUserID()));
		m_editNext.SetWindowText(SimpleFormatInt64Value(m_RowAccount.GetNextUserID()));

		int nNextYPos = 132;

		if (TRUE == bShowHotlisted)
		{
			nNextYPos += 20;
		}

		MoveControl(&m_staticPrevious, 355, nNextYPos + 2);
		MoveControl(&m_editPrevious, 432, nNextYPos);

		if (TRUE == bShowPrevious)
		{
			nNextYPos += 20;
		}

		MoveControl(&m_staticNext, 355, nNextYPos + 2);
		MoveControl(&m_editNext, 432, nNextYPos);
	}

	//PHOTO ID
	UpdatePhotoID(1);

	//PASSWORD CONTROL
	{
		bool bEditGeneral = Passwords.IsEditGeneralAllowed();
		m_editUsername.EnableWindow(bEditGeneral);
		m_editForename.EnableWindow(bEditGeneral);
		m_editSurname.EnableWindow(bEditGeneral);
		m_comboGender.EnableWindow(bEditGeneral);
		m_comboGroup.EnableWindow(bEditGeneral && (m_comboGroup.GetCount() > 1));
		m_checkExpiry.EnableWindow(bEditGeneral);
		m_datePickerExpiry.EnableWindow(bEditGeneral && IsTicked(m_checkExpiry));
		m_editAddress1.EnableWindow(bEditGeneral);
		m_editAddress2.EnableWindow(bEditGeneral);
		m_editAddress3.EnableWindow(bEditGeneral);
		m_editAddress4.EnableWindow(bEditGeneral);
		m_editAddress5.EnableWindow(bEditGeneral);
		m_editTel1.EnableWindow(bEditGeneral);
		m_editTel2.EnableWindow(bEditGeneral);
		m_editQRInfo.EnableWindow(bEditGeneral && m_bShowQR);
		m_editInfo1.EnableWindow(bEditGeneral && ( FALSE == m_bDateInfo1 ));
		m_editInfo2.EnableWindow(bEditGeneral && ( FALSE == m_bDateInfo2 ));
		m_editInfo3.EnableWindow(bEditGeneral && ( FALSE == m_bDateInfo3 ));
		m_editInfo4.EnableWindow(bEditGeneral && ( FALSE == m_bDateInfo4 ));
		m_checkInfo1.EnableWindow(bEditGeneral && ( TRUE == m_bDateInfo1));
		m_checkInfo2.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo2));
		m_checkInfo3.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo3));
		m_checkInfo4.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo4));
		m_datePickerInfo1.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo1) && IsTicked(m_checkInfo1));
		m_datePickerInfo2.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo2) && IsTicked(m_checkInfo2));
		m_datePickerInfo3.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo3) && IsTicked(m_checkInfo3));
		m_datePickerInfo4.EnableWindow(bEditGeneral && (TRUE == m_bDateInfo4) && IsTicked(m_checkInfo4));
		m_checkInactive.EnableWindow(bEditGeneral);
		m_checkDOB.EnableWindow(bEditGeneral);
		m_datePickerDOB.EnableWindow(bEditGeneral & IsTicked(m_checkDOB));
	}
	
	return TRUE;
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnPaint()
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

void CEditAccountPropertyPage1::InitStaticText(CStatic& staticText, int nField)
{
	CString strText = m_texts.GetText(nField, "");

	if (m_texts.GetCompulsoryFlag(nField) == TRUE)
	{
		strText += szCOMPULSORY_TEXT;
	}
	staticText.SetWindowText(strText);
}

//$$******************************************************************

void CEditAccountPropertyPage1::InitCheckBoxText(CButton& checkBox, int nField, CString strDefault)
{
	CString strText = m_texts.GetText(nField, strDefault);

	if (m_texts.GetCompulsoryFlag(nField) == TRUE)
	{
		strText += szCOMPULSORY_TEXT;
	}
	checkBox.SetWindowText(strText);
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnDoubleClickStaticUsername()
{
	CString strUsername = GetEditBoxText(m_editUsername);
	CString strForename = GetEditBoxText(m_editForename);
	CString strSurname = GetEditBoxText(m_editSurname);

	if (strUsername != "")
	{
		if ( (strForename == "") && (strSurname == ""))
		{
			strUsername.TrimLeft(' ');
			strUsername.TrimRight(' ');
			m_RowAccount.DivideCardName(strUsername, strForename, strSurname);		// assume Forename first
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

void CEditAccountPropertyPage1::OnToggleExpiry()
{
	m_datePickerExpiry.EnableWindow(IsTicked(m_checkExpiry));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnToggleDOB()
{
	m_datePickerDOB.EnableWindow(IsTicked(m_checkDOB));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnToggleInfo1()
{
	m_datePickerInfo1.EnableWindow(IsTicked(m_checkInfo1));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnToggleInfo2()
{
	m_datePickerInfo2.EnableWindow(IsTicked(m_checkInfo2));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnToggleInfo3()
{
	m_datePickerInfo3.EnableWindow(IsTicked(m_checkInfo3));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnToggleInfo4()
{
	m_datePickerInfo4.EnableWindow(IsTicked(m_checkInfo4));
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnButtonQR()
{
	bool bEditGeneral = Passwords.IsEditGeneralAllowed();

	if (TRUE == m_bAllowExtRef)
	{
		m_bShowQR = (FALSE == m_bShowQR);

		if (FALSE == m_bShowQR)
		{
			CUserTexts texts;
			texts.Read();
			m_staticExtRef.SetWindowText(texts.GetText(LOYALTYFIELD_EXTERNALREF, ""));
			ShowAndEnableWindow(&m_editExtRef, TRUE, m_bManualEditExtRef);
			ShowAndEnableWindow(&m_buttonExtRef, TRUE, Passwords.IsEditPointsAllowed());
			ShowAndEnableWindow(&m_editQRInfo, FALSE);
		}
		else
		{
			m_staticExtRef.SetWindowText("QR Info");
			ShowAndEnableWindow(&m_editExtRef, FALSE);
			ShowAndEnableWindow(&m_buttonExtRef, FALSE);
			ShowAndEnableWindow(&m_editQRInfo, TRUE, bEditGeneral);
			m_editQRInfo.SetSel(0, -1);
			m_editQRInfo.SetFocus();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnButtonExtRef()
{
	if (TRUE == m_bReaderMode)
	{
		m_editExtRef.SetWindowText(m_strCurrentExtRef);
		m_reader.Terminate();
	}
	else
	{
		m_editExtRef.EnableWindow(FALSE);

		m_bReaderMode = TRUE;
		m_buttonExtRef.SetWindowText("Cancel");

		if (m_reader.SelectReader(m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud) == TRUE)
		{
			m_reader.SetUIDFormat(m_terminal.m_nUIDFormat1);
			m_reader.SetDataFormat(m_terminal.GetDataFormat1());
			m_reader.m_pWnd = &m_editExtRef;						// NOTE: sets strExtRef variable to "Present card"
			m_reader.m_bWaitForCard = TRUE;

			while (TRUE)
			{
				int nReply;
				if ((nReply = m_reader.GetUID(TRUE, Server.GetLogCardReadsFlag())) == 0)	// 0=no error, 1=error, -1 = terminate
				{
					CString strMiFareNo = m_reader.GetCardNo();

					if (ValidateMiFareNo(strMiFareNo) == TRUE)
					{
						m_editExtRef.SetWindowText(strMiFareNo);
					}
					break;
				}

				else if (nReply == -1)								// terminate 
					break;

				else													// error
					Prompter.Error(m_reader.GetError(), "Reader Error");
			}

			m_reader.DeselectReader();
		}
		else
		{
			CString strError = m_reader.GetError();
			Prompter.Error(strError, "Hardware Error");
		}
	}

	m_bReaderMode = FALSE;
	m_buttonExtRef.SetWindowText("Read Card");
	UpdateData(FALSE);
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnButtonAlert()
{
	CSelectAlertDlg dlg{};
	dlg.m_strAlertCodes = m_strAlertCodes;

	if (dlg.DoModal() == IDOK)
	{
		m_strAlertCodes = dlg.m_strAlertCodes;
		m_editAlert.SetWindowText(m_alertText.GetDisplayText(m_strAlertCodes));
	}
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnButtonPhotoID()
{
	if (m_pPhotoIDFilenameStack != NULL)
	{
		m_pPhotoIDFilenameStack->RemoveFile(3);
		CPhotoIDEditDlg dlg( m_RowAccount, m_pPhotoIDFilenameStack, this);

		if (dlg.DoModal() == IDOK)
		{
			m_pPhotoIDFilenameStack->SetChangeFlag();
			UpdatePhotoID(2);
			Invalidate();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnSelectGroup()
{
	bool bShowStampOfferTab = FALSE;

	{
		int nGroupNo = ExtractGroupNo();
		
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			bShowStampOfferTab = RowGroup.GetStampOfferFlag();
		}

		int nSchemeNo = ExtractSchemeNo();
		CGroupComboHelpers::FillSchemeCombo(m_comboScheme, nGroupNo, nSchemeNo, TRUE, TRUE);
	}

	if (FALSE == bShowStampOfferTab)
	{
		CSQLRepositoryOfferStamp repoStamp;
		bShowStampOfferTab = (repoStamp.CountRowsByUserID(m_RowAccount.GetUserID(), NULL).GetSQLResult() != 0);
	}

	if (TRUE == bShowStampOfferTab)
	{
		((CEditAccountPropertySheet*)m_pParentSheet)->AddStampOfferTab();
	}
	else
	{
		((CEditAccountPropertySheet*)m_pParentSheet)->RemoveStampOfferTab();
	}
}

//$$******************************************************************

BOOL CEditAccountPropertyPage1::OnSetActive()
{
	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

BOOL CEditAccountPropertyPage1::OnKillActive()
{
	if ((m_RowAccount.IsCardHotlisted() == FALSE) && (Passwords.IsEditGeneralAllowed() == TRUE))
	{
		if (CheckCompulsoryField(FALSE, &m_editUsername, NULL, LOYALTYFIELD_FULLNAME, Account::Fullname.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editForename, NULL, LOYALTYFIELD_FORENAME, Account::Forename.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editSurname, NULL, LOYALTYFIELD_SURNAME, Account::Surname.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(TRUE, NULL, &m_checkDOB, LOYALTYFIELD_DOB, Account::DOB.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editAddress1, NULL, LOYALTYFIELD_ADDRESS1, Account::Address1.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editAddress2, NULL, LOYALTYFIELD_ADDRESS2, Account::Address2.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editAddress3, NULL, LOYALTYFIELD_ADDRESS3, Account::Address3.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editAddress4, NULL, LOYALTYFIELD_ADDRESS4, Account::Address4.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(FALSE, &m_editAddress5, NULL, LOYALTYFIELD_ADDRESS5, Account::Address5.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField( FALSE, &m_editTel1, NULL, LOYALTYFIELD_PHONE1, Account::Phone1.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField( FALSE, &m_editTel2, NULL, LOYALTYFIELD_PHONE2, Account::Phone2.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField( m_bDateInfo1, &m_editInfo1, &m_checkInfo1, LOYALTYFIELD_INFO1, Account::Info1.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(m_bDateInfo2, &m_editInfo2, &m_checkInfo2, LOYALTYFIELD_INFO2, Account::Info2.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(m_bDateInfo3, &m_editInfo3, &m_checkInfo3, LOYALTYFIELD_INFO3, Account::Info3.Label) == FALSE)
		{
			return FALSE;
		}

		if (CheckCompulsoryField(m_bDateInfo4, &m_editInfo4, &m_checkInfo4, LOYALTYFIELD_INFO4, Account::Info1.Label) == FALSE)
		{
			return FALSE;
		}

		{
			CSQLRowAccountFull RowTemp;
			RowTemp.SetUserID(m_RowAccount.GetUserID());
			RowTemp.SetGroupNo(ExtractGroupNo());
			RowTemp.SetFullname(GetEditBoxText(m_editUsername));
			RowTemp.SetHotlistCode(m_RowAccount.GetHotlistCode());
			RowTemp.SetExpiryDate(GetInfoString(TRUE, NULL, &m_checkExpiry, &m_datePickerExpiry));
			((CEditAccountPropertySheet*)m_pParentSheet)->UpdateTitle(RowTemp);
		}
	}

	m_pEditAccountBuffer->m_strCardName = GetEditBoxText(m_editUsername);
	m_pEditAccountBuffer->m_nGroupNo = ExtractGroupNo();
	
	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEditAccountPropertyPage1::UpdatePhotoID(int nLevel)
{
	if (m_pPhotoIDFilenameStack != NULL)
	{
		if ((HBITMAP)m_image != NULL)
		{
			m_image.Destroy();
		}

		CString strFilename = m_pPhotoIDFilenameStack->GetImageFilename(nLevel);

		if (strFilename != "")
		{
			m_image.Load(strFilename);
			m_staticNoPicture.ShowWindow(SW_HIDE);
		}
		else
		{
			m_staticNoPicture.ShowWindow(SW_SHOW);
		}
	}
}

//$$******************************************************************

int CEditAccountPropertyPage1::ExtractGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, Group::GroupNo.Min);
}

//$$******************************************************************

int CEditAccountPropertyPage1::ExtractSchemeNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, 0);
}

//$$******************************************************************

bool CEditAccountPropertyPage1::ValidateMiFareNo(const char* szMiFareNo)
{
	CString strMiFareNo = szMiFareNo;
	CString strErrorMsg = "";

	if (strMiFareNo != "")
	{
		CSQLRepositoryAccount repoAccount;
		__int64 nUserID = repoAccount.LookupMember(Account::ExternalRef.Label, strMiFareNo,NULL).GetSQLResult();

		if ((nUserID != 0) && (nUserID != m_RowAccount.GetUserID()))
		{
			strErrorMsg.Format("ATTENTION : Mifare card already in use!\n\nCard number %s is currently assigned to account %I64d!", 
				(const char*) strMiFareNo, 
				nUserID);
		}
	}

	if (strErrorMsg == "")
	{
		return TRUE;
	}

	strErrorMsg += "\n\nThe MiFare card reference will remain unchanged!";
	Prompter.Error(strErrorMsg, "MiFare Database Access");
	
	return FALSE;
}

//$$******************************************************************

bool CEditAccountPropertyPage1::CheckCompulsoryField(bool bIsDate, CEdit* pEdit, CButton* pCheck, int nField, CString strDefText)
{
	bool bResult = TRUE;
	if (TRUE == m_texts.GetCompulsoryFlag(nField))
	{
		if ( (FALSE == bIsDate) && ( pEdit != NULL ) )
		{
			CString strCurrent = "";
			pEdit->GetWindowText(strCurrent);

			if (strCurrent == "")
			{
				DisplayCompulsoryMessage(m_texts.GetText(nField, ""), strDefText);
				pEdit->SetFocus();
				bResult = FALSE;
			}
		}
		else if ( pCheck != NULL )
		{
			if (IsTicked(*pCheck) == FALSE)
			{
				DisplayCompulsoryMessage(m_texts.GetText(nField, ""), strDefText);
				pCheck->SetFocus();
				bResult = FALSE;
			}
		}
	}
	return bResult;
}

//$$******************************************************************

void CEditAccountPropertyPage1::DisplayCompulsoryMessage(CString strText, CString strDefText)
{
	SolutionGlobalFunctions::TrimSpacesFromString(strText, FALSE);

	if (strText == "")
	{
		strText = strDefText;
	}
	
	CString strMsg = "";
	strMsg += "No entry has been recorded against ";
	strMsg += strText;
	strMsg += "!";

	Prompter.Error(strMsg, "Mandatory Input");
}

//$$******************************************************************

BOOL CEditAccountPropertyPage1::OnQueryCancel()
{
	return CEditAccountPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditAccountPropertyPage1::OnCancel()
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

//$$******************************************************************

void CEditAccountPropertyPage1::UpdateDOBOptions()
{
	CString strDOB = "";
	if (IsTicked(m_checkDOB))
	{
		COleDateTime dateDOB;
		m_datePickerDOB.GetTime(dateDOB);

		if (CAccountFieldValidator::ValidateDOB(dateDOB) == TRUE)
		{
			strDOB = CAccountFieldValidator::OleDateToString(dateDOB);
		}
	}
	m_RowAccount.SetDOB(strDOB);
}

//$$******************************************************************

CString CEditAccountPropertyPage1::GetInfoString(bool bIsDate, CEdit* pEdit, CButton* pCheck, CDateTimeCtrl* pDatePicker )
{
	CString strResult = "";

	if (FALSE == bIsDate)
	{
		if (pEdit != NULL)
		{
			strResult = GetEditBoxText(*pEdit);
		}
	}
	else
	{
		if ((pCheck != NULL) && (pDatePicker != NULL))
		{
			if (IsTicked(*pCheck) == TRUE)
			{
				COleDateTime date;
				pDatePicker->GetTime(date);

				if (CAccountFieldValidator::ValidateExpiry(date) == TRUE)
				{
					strResult = CAccountFieldValidator::OleDateToString(date);
				}
			}
		}
	}

	return strResult;
}

//$$******************************************************************

void CEditAccountPropertyPage1::OnOK()
{
	if (TRUE == m_bReaderMode)
	{
		m_reader.Terminate();
		m_editExtRef.SetWindowText(m_strCurrentExtRef);
	}

	m_RowAccount.SetFullname(GetEditBoxText(m_editUsername));
	m_RowAccount.SetForename(GetEditBoxText(m_editForename));
	m_RowAccount.SetSurname(GetEditBoxText(m_editSurname));

	{
		CString strGender = "";
		m_comboGender.GetWindowText(strGender);
		m_RowAccount.SetGender(strGender);
	}
		
	m_RowAccount.SetAddress1(GetEditBoxText(m_editAddress1));
	m_RowAccount.SetAddress2(GetEditBoxText(m_editAddress2));
	m_RowAccount.SetAddress3(GetEditBoxText(m_editAddress3));
	m_RowAccount.SetAddress4(GetEditBoxText(m_editAddress4));
	m_RowAccount.SetAddress5(GetEditBoxText(m_editAddress5));
	m_RowAccount.SetPhone1(GetEditBoxText(m_editTel1));
	m_RowAccount.SetPhone2(GetEditBoxText(m_editTel2));
	m_RowAccount.SetInactive(IsTicked(m_checkInactive));
	m_RowAccount.SetGroupNo(ExtractGroupNo());
	m_RowAccount.SetSchemeNo(ExtractSchemeNo());
	m_RowAccount.SetAlertCodes(m_strAlertCodes);
	UpdateDOBOptions();

	m_RowAccount.SetExpiryDate(GetInfoString(TRUE, NULL, &m_checkExpiry, &m_datePickerExpiry));
	m_RowAccount.SetInfo1(GetInfoString(m_bDateInfo1, &m_editInfo1, &m_checkInfo1, &m_datePickerInfo1));
	m_RowAccount.SetInfo2(GetInfoString(m_bDateInfo2, &m_editInfo2, &m_checkInfo2, &m_datePickerInfo2));
	m_RowAccount.SetInfo3(GetInfoString(m_bDateInfo3, &m_editInfo3, &m_checkInfo3, &m_datePickerInfo3));
	m_RowAccount.SetInfo4(GetInfoString(m_bDateInfo4, &m_editInfo4, &m_checkInfo4, &m_datePickerInfo4));

	m_RowAccount.SetExternalRef(GetEditBoxText(m_editExtRef));
	
	CString strNewQRInfo = GetEditBoxText(m_editQRInfo);

	if ((strNewQRInfo != "") && (strNewQRInfo != m_strCurrentQRInfo))
	{
		CSQLRepositoryAccount repoAccount;
		if (repoAccount.RemoveQRInfo(strNewQRInfo, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			strNewQRInfo = m_strCurrentQRInfo;
		}
	}

	m_RowAccount.SetQRInfo(strNewQRInfo);

	CSSPropertyPage::OnOK();
}

//$$******************************************************************

