#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage8 : public CSSPropertyPage
{
public:
	CServerPropertyPage8();

	enum { IDD = IDD_PROPPAGE_SERVER8 };

	CSSComboBox m_comboType[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	CSSComboBox m_comboRevalBonus1;
	CSSComboBox m_comboRevalBonus2;
	CEdit m_editRevalComment1;
	CEdit m_editRevalComment2;
	CStatic m_staticP2Topup1;
	CStatic m_staticP2Topup2;
	CStatic m_staticP2Topup3;
	CStatic m_staticP2Topup4;
	CStatic m_staticP2Topup5;
	CStatic m_staticP2Bonus;
	CStatic m_staticP2Comment;
	CStatic m_staticP1Bonus;
	CStatic m_staticP1Comment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
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

private:
	CEdit* GetEditRevalPlu(int p, int t);
	CEdit* GetEditRevalTrip(int p, int t);
	CEdit* GetEditRevalPoints(int p, int t);
	CEdit* GetEditRevalValue(int p, int t);
	CEdit* GetEditRevalPercent(int p, int t);

private:
	bool m_bShowPurse2;
};

//$$******************************************************************
