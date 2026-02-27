#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************

class CGroupPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CGroupPropertyPage1(CSQLRowGroupFull& RowGroup);
	virtual ~CGroupPropertyPage1();

	enum { IDD = IDD_PROPPAGE_GROUP1 };

	CEdit m_editName;
	CSSComboBox m_comboUserType;
	CButton m_checkPurchaseControl;
	CButton m_checkKioskReval;
	CStatic m_staticSetNo;
	CSpinButtonCtrl m_spinSetNo;
	CSpinButtonCtrl m_spinPriceLevel;
	CButton m_checkPriceLevel;
	CButton m_checkVATShift;
	CStatic m_static2;
	CStatic m_static3;
	CStatic m_static4;
	CStatic m_static5;
	CEdit m_editDeptsInhibited;
	CStatic m_staticShiftGroup;
	CEdit m_editShiftGroup;
	
	int m_nUserTypeIndex;
	int m_nSetNo;
	int m_nPriceLevel;
	BOOL m_bVatShift;
	int m_nPointsAchievement;
	CString m_strDeptInhibitList;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSpinPriceLevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinGroupSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTogglePriceLevel();

	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditSetNo();
	CEdit* GetEditPriceLevel();
	CEdit* GetEditPointsAchievement();

private:
	bool m_bEnablePriceLevel;
	CSQLRowGroupFull& m_RowGroup;
};

//$$******************************************************************
