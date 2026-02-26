#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRowGroupTopupBonus.h"
//$$******************************************************************

class CGroupPropertyPage4: public CSSPropertyPage
{
public:
	CGroupPropertyPage4(CSQLRowGroup& RowGroup, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopupBonus);
	virtual ~CGroupPropertyPage4();

	enum { IDD = IDD_PROPPAGE_GROUP4 };
	CSSComboBox m_comboSetting[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	CSSComboBox m_comboType[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	CStatic m_staticP2Topup1;
	CStatic m_staticP2Topup2;
	CStatic m_staticP2Topup3;
	CStatic m_staticP2Topup4;
	CStatic m_staticP2Topup5;
	/**********/
	int m_nLastSetting[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectSettingPurse1Topup1();
	afx_msg void OnSelectSettingPurse1Topup2();
	afx_msg void OnSelectSettingPurse1Topup3();
	afx_msg void OnSelectSettingPurse1Topup4();
	afx_msg void OnSelectSettingPurse1Topup5();
	afx_msg void OnSelectSettingPurse2Topup1();
	afx_msg void OnSelectSettingPurse2Topup2();
	afx_msg void OnSelectSettingPurse2Topup3();
	afx_msg void OnSelectSettingPurse2Topup4();
	afx_msg void OnSelectSettingPurse2Topup5();
	afx_msg void OnSelectPurse1BonusType1();
	afx_msg void OnSelectPurse1BonusType2();
	afx_msg void OnSelectPurse1BonusType3();
	afx_msg void OnSelectPurse1BonusType4();
	afx_msg void OnSelectPurse1BonusType5();
	afx_msg void OnSelectPurse2BonusType1();
	afx_msg void OnSelectPurse2BonusType2();
	afx_msg void OnSelectPurse2BonusType3();
	afx_msg void OnSelectPurse2BonusType4();
	afx_msg void OnSelectPurse2BonusType5();
	/**********/
	DECLARE_MESSAGE_MAP()

private:
	void SelectType(int p, int t);
	void SelectSetting(int p, int t);

private:
	CEdit* GetEditRevalTrip(int p, int t);
	CEdit* GetEditRevalPoints(int p, int t);
	CEdit* GetEditRevalValue(int p, int t);
	CEdit* GetEditRevalPercent(int p, int t);

private:
	CSQLRowGroup& m_RowGroup;
	CReportConsolidationArray<CSQLRowGroupTopupBonus>& m_arrayGroupTopupBonus;
	bool m_bShowPurse2;
};

//$$******************************************************************
