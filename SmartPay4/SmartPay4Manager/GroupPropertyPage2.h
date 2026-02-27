#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************

class CGroupPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CGroupPropertyPage2(CSQLRowGroupFull& RowGroup);
	virtual ~CGroupPropertyPage2();
	void SetPointer ( bool bShowCash2, bool bRevaluationAllowed, bool bSpendAllowed );

	enum { IDD = IDD_PROPPAGE_GROUP2 };
	CButton m_checkUseGroupRevaluation;
	CButton m_checkRevaluationDisabled;
	CButton m_radioRevaluationPoints;
	CButton m_radioRevaluationValue;
	CStatic m_staticSecondBonusTrip;
	CButton m_buttonDefault1;
	CButton m_checkUseGroupReward;
	CButton m_checkRewardDisabled;
	CButton m_radioRewardPoints;
	CButton m_radioRewardValue;
	CButton m_buttonDefault2;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleRevaluationType();
	afx_msg void OnToggleRevalueDisabled();
	afx_msg void OnRadioRevaluationPoints();
	afx_msg void OnRadioRevaluationValue();
	afx_msg void OnButtonDefault1();
	afx_msg void OnToggleRewardType();
	afx_msg void OnToggleRewardDisabled();
	afx_msg void OnRadioRewardPoints();
	afx_msg void OnRadioRewardValue();
	afx_msg void OnButtonDefault2();
	DECLARE_MESSAGE_MAP()

private:
	void SetRevaluationDisplay();
	void SetRewardDisplay();

private:
	CEdit* GetEditRevalueTrip();
	CEdit* GetEditRevalueTrip2();
	CEdit* GetEditRevalueValue();
	CEdit* GetEditRevaluePoints();
	CEdit* GetEditRewardSpendTrip();
	CEdit* GetEditRewardValue();
	CEdit* GetEditRewardPoints();
	CEdit* GetEditUpperSpendLimit();
	CEdit* GetEditMultiplierTrigger();
	
public:
	int m_nRevaluationType;
	double m_dRevaluationTrip;
	double m_dRevaluationTrip2;
	int m_nRevaluationPoints;
	double m_dRevaluationValue;
	/*****/
	int m_nRewardType;
	double m_dRewardTrip;
	int m_nRewardPoints;
	double m_dRewardValue;
	double m_dRewardMultiplierTrigger;
	double m_dRewardUpperSpendLimit;

private:
	bool m_bShowCash2;
	bool m_bRevaluationAllowed;
	bool m_bSpendAllowed;	

private:
	CSQLRowGroupFull& m_RowGroup;
};

//$$******************************************************************
