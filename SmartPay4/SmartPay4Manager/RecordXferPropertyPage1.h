#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "CommentHandler.h"
#include "RecordXferData.h"
//$$******************************************************************

class CRecordXferPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CRecordXferPropertyPage1( CSQLRowAccountFull& RowAccountFrom, CRecordXferData& RecordXferData );
	void SetPointer ( const char* szCardTo, bool bHotlistReplace );
	virtual ~CRecordXferPropertyPage1();

	bool StartAllowed();

	enum { IDD = IDD_PROPPAGE_XFER1 };

	CButton m_checkUserName;
	CButton m_checkForename;
	CButton m_checkSurname;
	CButton m_checkGroup;
	CButton m_checkMemberID;
	CButton m_checkExpiry;
	CButton m_checkCopyPurchaseHistory;
	/*****/
	CEdit m_editUserName;
	CEdit m_editForename;
	CEdit m_editSurname;
	CSSTabbedComboBox m_tabcomboGroup;
	CEdit m_editMemberID;
	CDateTimeCtrl m_DateTimePicker;
	CButton m_buttonSelectAll;
	CSSComboBox m_comboComments;
	/*****/
	CButton m_checkPoints;
	CButton m_checkPointsTD;
	CButton m_checkPurse1Liability;
	CButton m_checkPurse1Credit;
	CButton m_checkPurse1SpendTD;
	CButton m_checkPurse2;
	CButton m_checkPurse2SpendTD;
	CButton m_checkPurse3Liability;
	CButton m_checkPurse3Credit;
	CButton m_checkPurse3SpendTD;
	CButton m_checkPurchaseControl;
	/*****/
	CButton m_buttonSelectBalances;
	/*****/
	CButton m_checkMaxSpendCopy;
	CButton m_checkMaxSpendModify;
	CStatic m_staticMaxSpendP1;
	CStatic m_staticOverSpendP1;
	CStatic m_staticMaxSpendP3;
	CStatic m_staticOverSpendP3;

private:
	int m_nPoints;
	int m_nPointsTD;
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	double m_dPurse1SpendTD;
	double m_dPurse2;
	double m_dPurse2SpendTD;
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	double m_dPurse3SpendTD;
	double m_dMaxSpendP1;
	double m_dOverSpendP1;
	double m_dMaxSpendP3;
	double m_dOverSpendP3;

private:
	bool m_bCanModifyMaxSpend;
	int m_nOriginalGroupNo;
	int m_nOriginalGroupMaxSpendType;
	double m_dOriginalMaxSpendP1;
	double m_dOriginalMaxSpendP3;
	double m_dOriginalOverSpendP1;
	double m_dOriginalOverSpendP3;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickStaticComment();
	afx_msg void OnToggleUsername();
	afx_msg void OnToggleForename();
	afx_msg void OnToggleSurname();
	afx_msg void OnToggleGroupNo();
	afx_msg void OnSelectGroup();
	afx_msg void OnToggleMemberID();
	afx_msg void OnToggleExpiry();
	afx_msg void OnTogglePoints();
	afx_msg void OnTogglePointsTD();
	afx_msg void OnTogglePurse1Liability();
	afx_msg void OnTogglePurse1Credit();
	afx_msg void OnTogglePurse1SpendTD();
	afx_msg void OnTogglePurse2();
	afx_msg void OnTogglePurse2SpendTD();
	afx_msg void OnTogglePurse3Liability();
	afx_msg void OnTogglePurse3Credit();
	afx_msg void OnTogglePurse3SpendTD();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonSelectBalances();
	afx_msg void OnToggleCopyMaxSpend();
	afx_msg void OnToggleModifyMaxSpend();
	afx_msg void OnToggleCopyPurchases();

	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPoints();
	CEdit* GetEditPointsTD();
	CEdit* GetEditPurse1Liability();
	CEdit* GetEditPurse1Credit();
	CEdit* GetEditPurse1SpendTD();
	CEdit* GetEditPurse2();
	CEdit* GetEditPurse2SpendTD();
	CEdit* GetEditPurse3Liability();
	CEdit* GetEditPurse3Credit();
	CEdit* GetEditPurse3SpendTD();
	CEdit* GetEditMaxSpendP1();
	CEdit* GetEditOverSpendP1();
	CEdit* GetEditMaxSpendP3();
	CEdit* GetEditOverSpendP3();

private:
	void SetDisplay();
	void SetDisplayPurse2();
	void AddComments();
	int ExtractGroupNo();
	CString GetExpiryDate();
	void SelectGroupFromCombo( int nGroupNo );
	void ShowOriginalMaxSpend();

public:
	CSQLRowAccountFull& m_RowAccountFrom;
	
private:
	CRecordXferData& m_RecordXferData;
	CString m_strToCardNo;
	bool m_bHotlistReplace;

	CCommentHandler m_Comments;

	CSSDate m_dateToday;
	CSSDate m_dateExpiry; 

	int m_nMaxPoints;
	int m_nMaxPointsTD;
	double m_dMaxPurse1Liability;
	double m_dMaxPurse1Credit;
	double m_dMaxPurse2;
	double m_dMaxPurse3Liability;
	double m_dMaxPurse3Credit;
	double m_dMaxPurse1SpendTD;
	double m_dMaxPurse2SpendTD;
	double m_dMaxPurse3SpendTD;
	int m_nMinPoints;
	int m_nMinPointsTD;
	double m_dMinPurse1Liability;
	double m_dMinPurse1Credit;
	double m_dMinPurse2;
	double m_dMinPurse3Liability;
	double m_dMinPurse3Credit;
	double m_dMinPurse1SpendTD;
	double m_dMinPurse2SpendTD;
	double m_dMinPurse3SpendTD;
	int m_nActiveBalances;

	bool m_bSelectAllDetails;
	bool m_bSelectAllBalances;

	bool m_bAllowOverSpend;
	bool m_bAllowPurse3;
	bool m_bAllowCreditPurse1;
	bool m_bAllowCreditPurse3;
};

//$$******************************************************************
