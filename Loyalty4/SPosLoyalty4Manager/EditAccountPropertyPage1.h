#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "AlertText.h"
#include "EditAccountBuffer.h"
#include "MyMifare.h"
#include "PhotoIDFilenameStack.h"
#include "TerminalData.h"
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
static const char* szCOMPULSORY_TEXT = "*";
//$$******************************************************************

class CRightClickStatic : public CStatic
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point) { GetParent()->PostMessage(WM_APP + 1); }
};

//$$******************************************************************

class CEditAccountPropertyPage1 : public CSSPropertyPage
{
public:
	CEditAccountPropertyPage1(CSQLRowAccountFull& RowAccount);
	virtual ~CEditAccountPropertyPage1();
	void SetPointer(void* pParentSheet, CEditAccountBuffer* pEditAccountBuffer);
	void SetPhotoIDFilenameStack(CPhotoIDFilenameStack* p) { m_pPhotoIDFilenameStack = p; }

public:
	enum { IDD = IDD_PROPPAGE_EDITACCOUNT1 };
	CStatic m_staticUsername;
	CEdit m_editUsername;
	CStatic m_staticForename;
	CEdit m_editForename;
	CStatic m_staticSurname;
	CEdit m_editSurname;
	CSSComboBox m_comboGender;
	CButton m_checkDOB;
	CDateTimeCtrl m_datePickerDOB;
	CStatic m_staticAddress1;
	CStatic m_staticAddress2;
	CStatic m_staticAddress3;
	CStatic m_staticAddress4;
	CStatic m_staticAddress5;
	CStatic m_staticTel1;
	CStatic m_staticTel2;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	CStatic m_staticInfo3;
	CStatic m_staticInfo4;
	CButton m_checkInfo1;
	CButton m_checkInfo2;
	CButton m_checkInfo3;
	CButton m_checkInfo4;
	CEdit m_editAddress1;
	CEdit m_editAddress2;
	CEdit m_editAddress3;
	CEdit m_editAddress4;
	CEdit m_editAddress5;
	CEdit m_editTel1;
	CEdit m_editTel2;
	CEdit m_editInfo1;
	CEdit m_editInfo2;
	CEdit m_editInfo3;
	CEdit m_editInfo4;
	CEdit m_editQRInfo;
	CDateTimeCtrl m_datePickerInfo1;
	CDateTimeCtrl m_datePickerInfo2;
	CDateTimeCtrl m_datePickerInfo3;
	CDateTimeCtrl m_datePickerInfo4;
	/****/
	CButton m_checkExpiry;
	CDateTimeCtrl m_datePickerExpiry;
	CButton m_checkInactive;
	CSSTabbedComboBox m_comboGroup;
	CSSTabbedComboBox m_comboScheme;
	CButton m_buttonQR;
	CRightClickStatic m_staticExtRef;
	CEdit m_editExtRef;
	CButton m_buttonExtRef;
	CButton m_buttonAlert;
	CEdit m_editAlert;
	CStatic m_staticHotlist;
	CEdit m_editHotlistDate;
	CEdit m_editHotlistTime;
	CStatic m_staticPrevious;
	CEdit m_editPrevious;
	CStatic m_staticNext;
	CEdit m_editNext;
	CStatic	m_staticPicture;
	CStatic	m_staticNoPicture;
	CImage m_image;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDoubleClickStaticUsername();
	afx_msg void OnToggleDOB();
	afx_msg void OnToggleExpiry();
	afx_msg void OnToggleInfo1();
	afx_msg void OnToggleInfo2();
	afx_msg void OnToggleInfo3();
	afx_msg void OnToggleInfo4();
	afx_msg void OnButtonQR();
	afx_msg void OnButtonExtRef();
	afx_msg void OnButtonPhotoID();
	afx_msg void OnButtonAlert();
	afx_msg void OnSelectGroup();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	LRESULT OnRightClickExtRef(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	int ExtractGroupNo();
	int ExtractSchemeNo();
	bool ValidateMiFareNo(const char* szMiFareNo);
	void LoadDate(CButton& checkDate, CDateTimeCtrl& datePicker, CString strDate, bool bDOB);
	void InitStaticText(CStatic& staticText, int nField);
	void InitCheckBoxText(CButton& checkBox, int nField, CString strDefault);
	bool CheckCompulsoryField(bool bIsDate, CEdit* pEdit, CButton* pCheck, int nField, CString strDefText);
	CString GetInfoString(bool bIsDate, CEdit* pEdit, CButton* pCheck, CDateTimeCtrl* pDatePicker);
	void DisplayCompulsoryMessage(CString strText, CString strDefText);
	void UpdatePhotoID(int nLevel);
	void UpdateDOBOptions();
	
private:
	bool m_bReaderMode;
	CString m_strCurrentExtRef;
	CString m_strCurrentQRInfo;
	CString m_strAlertCodes;

private:
	void* m_pParentSheet;
	CUserTexts m_texts;
	CTerminalData m_terminal;
	CMyMifare m_reader;
	CAlertText m_alertText;
	CPhotoIDFilenameStack* m_pPhotoIDFilenameStack;
	CEditAccountBuffer* m_pEditAccountBuffer;

private:
	bool m_bDateInfo1;
	bool m_bDateInfo2;
	bool m_bDateInfo3;
	bool m_bDateInfo4;

private:
	bool m_bShowQR;
	bool m_bAllowExtRef;
	bool m_bManualEditExtRef;;

private:
	CSQLRowAccountFull& m_RowAccount;
};

//$$******************************************************************
