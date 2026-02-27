#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************

class CServerPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CServerPropertyPage3();
	
	enum { IDD = IDD_PROPPAGE_SERVER3 };

private:
	CStatic m_staticPurseRevalue;
	CEdit m_editPluPurse1;
	CEdit m_editPluPurse3;
	/*****/
	CStatic m_staticPurseBonus;
	CStatic m_staticPurse1Trip;
	CStatic m_staticPurse3Trip;
	CStatic m_staticBonusComment;
	CButton m_radioRevaluePoints;
	CButton m_radioRevalueValue;
	CEdit m_editComment;
	/*****/
	CButton m_checkHistory;
	CButton m_checkHistoryDisableACZero;
	CStatic m_staticEcrmanImportFolder;
	CEdit m_editEcrmanImportFolder;
	CButton m_buttonBrowse;
	/*****/
	int m_nRevaluationType;

private:
	CEdit* GetEditTripPurse1();
	CEdit* GetEditTripPurse3();
	CEdit* GetEditRevaluePoints();
	CEdit* GetEditRevalueValue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleHistory();
	afx_msg void OnRadioRevaluePoints();
	afx_msg void OnRadioRevalueValue();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonTestPlu();
	afx_msg void OnButtonTestRule();
	afx_msg void OnButtonTestTermName();

	DECLARE_MESSAGE_MAP()

private:
	void SetRevaluationDisplay();
	void HandleTestButton(CString strFilename, CString strText, CString strReportType);
};

//$$******************************************************************
