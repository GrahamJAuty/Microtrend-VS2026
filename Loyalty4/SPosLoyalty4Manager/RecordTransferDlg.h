#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

struct CRecordTransferInfo
{
public:
	CRecordTransferInfo();
	void Reset( bool bAll);

public:
	bool m_bSetGroup;
	bool m_bSetScheme;
	bool m_bSetDOB;
	bool m_bSetExpiry;
	bool m_bSetAlert;
	bool m_bSetInactive;
	bool m_bSetGender;
	bool m_bSetName;
	bool m_bSetAddress1;
	bool m_bSetAddress2;
	bool m_bSetAddress3;
	bool m_bSetAddress4;
	bool m_bSetAddress5;
	bool m_bSetPhone1;
	bool m_bSetPhone2;
	bool m_bSetInfo1;
	bool m_bSetInfo2;
	bool m_bSetInfo3;
	bool m_bSetInfo4;
	bool m_bSetPoints;
	bool m_bSetPointsTD;
	bool m_bSetPurse1;
	bool m_bSetPurse1TD;
	bool m_bSetPurse2;
	bool m_bSetPurse2TD;
	bool m_bSetCashTD;
	bool m_bSetMaxSpend;
	bool m_bSetOverSpend;
	bool m_bSetCopyPurchaseHistory;
	bool m_bSetOfferStamps;
	/*****/
	int m_nSchemeNo;
	CString m_strDOB;
	CString m_strExpiry;
	CString m_strName;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strComment;
	CString m_strGender;
	CString m_strAlertCodes;
	bool m_bInactive;
	int m_nGroupNo;
	/*****/
	int m_nPoints;
	int m_nPointsTD;
	double m_dPurse1;
	double m_dPurse1TD;
	double m_dPurse2;
	double m_dPurse2TD;
	double m_dCashTD;
	/*****/
	double m_dMaxSpend;
	double m_dOverSpend;
	/*****/
	double m_dPurse1LastSpend;
	CString m_strPurse1DateLastUsed;
	CString m_strPurse1TimeLastUsed;
	double m_dPurse2LastSpend;
	CString m_strPurse2DateLastUsed;
	CString m_strPurse2TimeLastUsed;
	CString m_strDateLastRefreshed;
	CString m_strTimeLastRefreshed;
};

//*******************************************************************

class CRecordTransferDlg : public CSSDialog
{
public:
	CRecordTransferDlg( CSQLRowAccountFull& RowFrom, CSQLRowAccountFull& RowTo, bool bHotlistReplace, CWnd* pParent = NULL);   // standard constructor
	~CRecordTransferDlg();
	
// Dialog Data
	enum { IDD = IDD_TRANSFER };

	CEdit m_editCardNo;
	CButton m_checkName;
	CEdit m_editName;
	CButton m_checkGroup;
	CSSTabbedComboBox m_comboGroup;
	CButton m_checkScheme;
	CSSTabbedComboBox m_comboScheme;
	CButton m_checkSetExpiry;
	CButton m_checkDateExpiry;
	CDateTimeCtrl m_datePickerExpiry;
	CButton m_checkSetDOB;
	CButton m_checkDateDOB;
	CDateTimeCtrl m_datePickerDOB;
	CButton m_checkAlert;
	CButton m_checkInactive;
	CButton m_checkGender;
	CButton m_checkAddress1;
	CEdit m_editAddress1;
	CButton m_checkAddress2;
	CEdit m_editAddress2;
	CButton m_checkAddress3;
	CEdit m_editAddress3;
	CButton m_checkAddress4;
	CEdit m_editAddress4;
	CButton m_checkAddress5;
	CEdit m_editAddress5;
	CButton m_checkPhone1;
	CEdit m_editPhone1;
	CButton m_checkPhone2;
	CEdit m_editPhone2;
	CButton m_checkInfo1;
	CEdit m_editInfo1;
	CButton m_checkDateInfo1;
	CDateTimeCtrl m_datePickerInfo1;
	CButton m_checkInfo2;
	CEdit m_editInfo2;
	CButton m_checkDateInfo2;
	CDateTimeCtrl m_datePickerInfo2;
	CButton m_checkInfo3;
	CEdit m_editInfo3;
	CButton m_checkDateInfo3;
	CDateTimeCtrl m_datePickerInfo3;
	CButton m_checkInfo4;
	CEdit m_editInfo4;
	CButton m_checkDateInfo4;
	CDateTimeCtrl m_datePickerInfo4;
	CButton m_checkPoints;
	CButton m_checkPointsTD;
	CButton m_checkPurse1;
	CButton m_checkPurse1TD;
	CButton m_checkPurse2;
	CButton m_checkPurse2TD;
	CButton m_checkCashTD;
	CButton m_checkMaxSpend;
	CButton m_checkOverSpend;
	CButton m_checkCopyPurchaseHistory;
	CButton m_checkOfferStamp;
	CSSComboBox m_comboComments;
	CButton m_buttonSelectAll; 
	CButton m_buttonSelectBalances;
	CButton m_buttonOK;
	/****/
	int m_nPoints;
	int m_nPointsTD;
	double m_dPurse1;
	double m_dPurse1TD;
	double m_dPurse2;
	double m_dPurse2TD;
	double m_dCashTD;
	double m_dOverSpend;
	double m_dMaxSpend;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleDateExpiry();
	afx_msg void OnToggleDateDOB();
	afx_msg void OnToggleDateInfo1();
	afx_msg void OnToggleDateInfo2();
	afx_msg void OnToggleDateInfo3();
	afx_msg void OnToggleDateInfo4();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectBalances();
	afx_msg void OnClickStaticComment();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPoints();
	CEdit* GetEditPointsTD();
	CEdit* GetEditPurse1();
	CEdit* GetEditPurse2();
	CEdit* GetEditPurse1TD();
	CEdit* GetEditPurse2TD();
	CEdit* GetEditCashTD();
	CEdit* GetEditMaxSpend();
	CEdit* GetEditOverSpend();

public:
	void GetRecordTransferInfo(CRecordTransferInfo& info) { info = m_XferInfo; }

private:
	void AddComments();
	void SetDisplay();
	bool ValidateBalance ( const char* szTitle, int& nValue, int nMin, int nMax );
	bool ValidateBalance ( const char* szTitle, double& dValue, double dMin, double dMax );
	void TransferAdjusted ( const char* szTitle, int nErrorNo );
	bool StartAllowed();
	
private:
	void LoadDateInfo(bool bDOB, CString strDate, CButton& checkBox, CDateTimeCtrl& datePicker);
	void GetNewDateInfo(bool bDOB, bool& bSaveTick, CString& strSaveDate, CButton& checkBox, CDateTimeCtrl& datePicker);
	bool CheckTransferValue(CString strType, CButton& check, CEdit* pEdit, double dValue, double dMinValue, double dMaxValue, bool& bOutFlag, double& dOutValue);
	bool CheckTransferValue(CString strType, CButton& check, CEdit* pEdit, int nValue, int nMinValue, int nMaxValue, bool& bOutFlag, int& nOutValue);
	void CheckTransferValue(bool bUseDate, CButton& checkSet, CEdit& edit, CButton& checkDate, CDateTimeCtrl& dtc, bool& bOutFlag, CString& strOutValue);
	void SetInfoValue(bool bUseDate, CButton& checkSet, CEdit& edit, CButton& checkDate, CDateTimeCtrl& dtc, CString strValue);

private:
	CCommentHandler m_Comments;
	bool m_bHotlistReplace;

	int m_nActiveBalances;
	
	int m_nMaxPoints;
	int m_nMinPoints;
	int m_nMaxPointsTD;
	int m_nMinPointsTD;
	double m_dMaxPurse1;
	double m_dMinPurse1;
	double m_dMaxPurse1TD;
	double m_dMinPurse1TD;
	double m_dMaxPurse2;
	double m_dMinPurse2;
	double m_dMaxPurse2TD;
	double m_dMinPurse2TD;
	double m_dMaxCashTD;
	double m_dMinCashTD;

	bool m_bSelectAllDetails;
	bool m_bSelectAllBalances;

	double m_dPurse1LastSpend;
	double m_dPurse2LastSpend;
	CString m_strPurse1DateLastUsed;
	CString m_strPurse1TimeLastUsed;
	CString m_strPurse2DateLastUsed;
	CString m_strPurse2TimeLastUsed;
	CString m_strDateLastRefreshed;
	CString m_strTimeLastRefreshed;

private:
	bool m_bDateInfo1;
	bool m_bDateInfo2;
	bool m_bDateInfo3;
	bool m_bDateInfo4;
	bool m_bCanTransferMaxSpend;

private:
	CSQLRowAccountFull& m_RowFrom;
	CSQLRowAccountFull& m_RowTo;

private:
	CRecordTransferInfo m_XferInfo;
};

//*******************************************************************
