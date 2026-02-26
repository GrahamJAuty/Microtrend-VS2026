#pragma once
//$$******************************************************************
#define BONUS_MON 0
#define BONUS_TUE 1
#define BONUS_WED 2
#define BONUS_THU 3
#define BONUS_FRI 4
#define BONUS_SAT 5
#define BONUS_SUN 6
//$$******************************************************************

class CServerPropertyPage5 : public CSSPropertyPage
{
public:
	CServerPropertyPage5();

	enum { IDD = IDD_PROPPAGE_SERVER5 };
	CButton m_checkPurse1;
	CButton m_checkPurse2;
	CButton m_checkCash;
	CButton m_checkManual;
	CButton m_checkNoRewardPluPts;
	double m_dSpendLimit;
	/*****/
	double m_dRewardTrip;
	CButton m_radioRewardPoints;
	CButton m_radioRewardValue;
	int m_nRewardPoints;
	double m_dRewardValue;
	CEdit m_editRewardComment;
	/*****/
	int m_nRewardMultiplierFactor[7];
	CDateTimeCtrl m_dtcStart[7];
	CDateTimeCtrl m_dtcEnd[7];
	double m_dRewardMultiplierTrigger;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnTogglePurse1();
	afx_msg void OnTogglePurse2();
	afx_msg void OnToggleCash();
	afx_msg void OnToggleManual();
	/*****/
	afx_msg void OnRadioRewardPoints();
	afx_msg void OnRadioRewardValue();
	/*****/
	afx_msg void OnSpinMultiplierMon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierTue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierWed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierThu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierFri(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierSat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpinMultiplierSun(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditSpendLimit();
	CEdit* GetEditRewardTrip();
	CEdit* GetEditRewardPoints();
	CEdit* GetEditRewardValue();
	CEdit* GetEditMultiplierTrigger();

private:
	void HandleMultiplierSpin( NMHDR *pNMHDR, LRESULT *pResult, int nDay );
	void SetBonusEnables( int nDay );
	void CheckQualifyingSpendTypes();
	void SetRewardDisplay( int nType );

private:
	int m_nRewardType;
};

//$$******************************************************************

