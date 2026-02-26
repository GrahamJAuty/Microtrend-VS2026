#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage3 : public CSSPropertyPage
{
public:
	CServerPropertyPage3();
	
	enum { IDD = IDD_PROPPAGE_SERVER3 };
	
	/**********/
	CEdit m_editEcrmanImportFolder;
	CButton m_checkSalesPluBonus;
	CButton m_checkSalesDeptBonus;
	CButton m_checkSalesNoPurseBonus;
	CButton m_checkPurse2Bonus;
	CButton m_checkPurse1Sales;
	CButton m_checkSalesTLog;
	CButton m_buttonSalesBrowse;
	CButton m_buttonTest;
	CButton m_checkGlobalDept;
	CButton m_checkGlobalOffer;
	/**********/
	CString m_strStaffGiftPlu;
	CString m_strStaffGiftRedirPlu;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	/**********/
	afx_msg void OnTogglePluOrDeptBonus();
	/**********/
	afx_msg void OnButtonSalesBrowse();
	afx_msg void OnButtonTest();
	
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditStaffGiftPlu();
	CEdit* GetEditStaffGiftRedirPlu();
};

//$$******************************************************************
