#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************

class CGroupPropertyPage8 : public CSSAutoShutdownPropertyPage
{
public:
	CGroupPropertyPage8(CSQLRowGroupFull& RowGroup);
	virtual ~CGroupPropertyPage8();
	
	enum { IDD = IDD_PROPPAGE_GROUP8 };

	CEdit m_editPurse1DeptInhibitList;
	/*****/
	CEdit m_editPurse2DeptInhibitList;
	CSSComboBox m_comboPurse2Type;
	CButton m_buttonPurse2Refresh;
	CButton m_checkPurse2AppendRefresh;
	CEdit m_editPurse2RefreshDays;
	CButton m_buttonPurse2Set;
	/*****/
	CEdit m_editPurse3DeptInhibitList;
	/*****/	
	CString m_strPurse1DeptInhibitList;
	double m_dPurse1LowBalance;
	/*****/
	CString m_strPurse2DeptInhibitList;
	double m_dPurse2LowBalance;
	int m_nPurse2RefreshType;
	CString m_strPurse2RefreshDays;
	double m_dPurse2RefreshValue;
	BOOL m_bPurse2AppendRefresh;
	/*****/
	CString m_strPurse3DeptInhibitList;
	double m_dPurse3LowBalance;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnButtonPurse2Set();
	afx_msg void OnButtonPurse2Refresh();
	afx_msg void OnSelectPurse2RefreshType();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPurse2LowBalance();
	CEdit* GetEditPurse2RefreshValue();
	CEdit* GetEditPurse3LowBalance();
	
private:
	void DisplayPurse2RefreshDays();

private:
	CSQLRowGroupFull& m_RowGroup;
};

//$$******************************************************************
