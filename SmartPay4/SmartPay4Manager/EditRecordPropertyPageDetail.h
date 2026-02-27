#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "EditAccountBuffer.h"
#include "MyMifare.h"
#include "TerminalData.h"
//$$******************************************************************

class CEditRecordPropertyPageDetail : public CSSAutoShutdownPropertyPage
{
public:
	CEditRecordPropertyPageDetail(CSQLRowAccountFull& Row );
	virtual ~CEditRecordPropertyPageDetail();
	void SetPointer ( void* pParentSheet, CEditAccountBuffer* pEditAccountBuffer, int nMode, CGroupShiftValidator* pGroupShiftValidator, CLeaversHandler* pLeaversData = NULL );
	void SaveChanges (CSQLRowAccountFull& Row);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITRECORD_DETAIL };
	
	/*****/
	CStatic m_staticUsername;
	CEdit m_editUsername;
	CEdit m_editForename;
	CEdit m_editSurname;
	CSSComboBox m_comboGender;
	CButton m_checkDOB;
	CDateTimeCtrl m_DatePickerDOB;
	CEdit m_editEmail;
	CStatic m_staticInfo1;
	CEdit m_editInfo1;
	CStatic m_staticInfo2;
	CEdit m_editInfo2;
	CStatic m_staticInfo3;
	CEdit m_editInfo3;
	CStatic m_staticInfo4;
	CEdit m_editInfo4;
	CStatic m_staticInfo5;
	CEdit m_editInfo5;
	CStatic m_staticInfo6;
	CEdit m_editInfo6;
	CStatic m_staticInfo7;
	CEdit m_editInfo7;
	CStatic m_staticInfo8;
	CEdit m_editInfo8;
	CButton m_checkTick1;
	CButton m_checkTick2;
	CButton m_checkTick3;
	CButton m_checkTick4;
	CEdit m_editDietary;
	CButton m_buttonAllergies;
	CEdit m_editAllergies;
	CButton m_checkExpiry;
	CDateTimeCtrl m_DatePickerExpiry;
	CButton m_checkInactive;
	CStatic m_staticUserType1;
	CSSTabbedComboBox m_tabcomboGroup1;
	CButton m_buttonSetGroup;
	CStatic m_staticUserType2;
	CSSTabbedComboBox m_tabcomboGroup2;
	CStatic m_staticMemberID;
	CEdit m_editMemberID;
	CButton m_buttonExtRefToggle;
	CStatic m_staticExtRef;
	CEdit m_editExtRef;
	CButton m_buttonReadCard;
	CStatic m_staticExtRef2;
	CEdit m_editExtRef2;
	CButton m_buttonReadCard2;
	CStatic m_staticBioReg;
	CEdit m_editBioReg;
	CEdit m_editAlert;
	CButton m_buttonAlert;
	CButton m_buttonAltKey1;
	CStatic m_staticPreviousCardNo;
	CEdit m_editPreviousCardNo;
	CStatic m_staticNextCardNo;
	CEdit m_editNextCardNo;
	CButton m_checkOwnMaxSpend;
	CStatic m_staticNoImage;
	CButton m_buttonPhoto;
	CStatic	m_staticPicture;
	/*****/
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnQueryCancel();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnDoubleClickStaticUsername();
	afx_msg void OnButtonAllergies();
	afx_msg void OnToggleExpiry();
	afx_msg void OnToggleDOB();
	afx_msg void OnSelectGroup1();
	afx_msg void OnSelectGroup2();
	afx_msg void OnButtonSetGroup();
	afx_msg void OnDoubleClickStaticExtRef();
	afx_msg void OnDoubleClickStaticExtRef2();
	afx_msg void OnButtonExtRefToggle();
	afx_msg void OnButtonReader();
	afx_msg void OnButtonReader2();
	afx_msg void OnButtonAlert();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonPhoto();
	/*****/
	
	DECLARE_MESSAGE_MAP()

private:
	bool IsHotlisted();
	int ExtractUsualGroupNo();
	int ExtractShiftedGroupNo();
	bool ValidateRefreshDays();
	bool ValidateMiFareNo ( const char* szMiFareNo, int nMiFareDbNo );
	bool SetAlternateGroup();
	bool SetRefreshDays();
	void InitPhotoFilename();
	void ShowField(CString strText, CWnd* pLabelWnd, CWnd* pEditWnd);
	void UpdateAllergyEditBox();
	CString GetExpiryDate();
	void SelectCurrentGroup();
	void FillGroupShiftCombo(int nUsualGroup, int nShiftedGroup);
	void TidyReferenceControls();
	void HandleButtonReader2();
	void HandleButtonNewPIN();

private:
	void UpdateExpiryOptions();
	void UpdateDOBOptions();

private:
	void DivideCardName(const char* szCardName, CString& strFirstName, CString& strSecondName);

private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CSQLRowGroupFull m_SQLRowGroup;
	CLeaversHandler* m_pLeaversData;
	CTerminalData m_terminal;
	CEditAccountBuffer* m_pEditAccountBuffer;
	CGroupShiftValidator* m_pGroupShiftValidator;
	void* m_pParentSheet;
	
private:
	CString m_strOriginalExtRef = "";
	CString m_strOriginalExtRef2 = "";
	CString m_strOriginalMemberID = "";
	CString m_strNewPINNumber = "";

private:
	int m_nMode;
	CString m_strPhotoFilename;
	CString m_strNoPhotoFilename;
	int m_nAllergyCodes;

private:
	CAlertText m_alert;
	CAllergyText m_allergy;
	CImage m_image;

private:
	CMyMifare m_reader;
	bool m_bReaderMode;
	bool m_bEnableHotlistEdit;

private:
	bool m_bEnableExtRefToggle;
	int m_nVisibleExtRef;
};

//$$******************************************************************
