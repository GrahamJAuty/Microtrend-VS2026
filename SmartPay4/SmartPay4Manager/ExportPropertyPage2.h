#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************

class CExportPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CExportPropertyPage2();
	virtual ~CExportPropertyPage2();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

	enum { IDD = IDD_PROPPAGE_EXPORT2 };
	CStatic m_staticInclude;
	CButton m_checkIncludeValid;
	CButton m_checkIncludeUnissued;
	CButton m_checkIncludeAlert;
	CButton m_checkIncludeInactive;
	CButton m_checkIncludeExpired;
	CButton m_checkIncludeUnknown;
	CButton m_checkIncludeGroupShiftNone;
	CButton m_checkIncludeGroupShiftCurrent;
	CButton m_checkIncludeGroupShiftFuture;
	/*****/
	CStatic m_staticTickBox;
	CStatic m_staticTick1;
	CButton m_radioTick1Ignore;
	CButton m_radioTick1Yes;
	CButton m_radioTick1No;
	CStatic m_staticTick2;
	CButton m_radioTick2Ignore;
	CButton m_radioTick2Yes;
	CButton m_radioTick2No;
	CStatic m_staticTick3;
	CButton m_radioTick3Ignore;
	CButton m_radioTick3Yes;
	CButton m_radioTick3No;
	CStatic m_staticTick4;
	CButton m_radioTick4Ignore;
	CButton m_radioTick4Yes;
	CButton m_radioTick4No;
	/*****/
	CStatic m_staticUser;
	CButton m_radioRange;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CSSTabbedComboBox m_tabcomboGroup;
	CButton m_checkSortByGroup;
	CButton m_checkGroupSummary;
	/*****/
	CStatic m_staticMisc;
	CStatic m_staticOrderBy;
	CSSComboBox m_comboSortOn;
	CButton m_checkSkipBlanks;
	CButton m_checkTotalise;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/****/
	afx_msg void OnRadioTick1Ignore();
	afx_msg void OnRadioTick1Yes();
	afx_msg void OnRadioTick1No();
	afx_msg void OnRadioTick2Ignore();
	afx_msg void OnRadioTick2Yes();
	afx_msg void OnRadioTick2No();
	afx_msg void OnRadioTick3Ignore();
	afx_msg void OnRadioTick3Yes();
	afx_msg void OnRadioTick3No();
	afx_msg void OnRadioTick4Ignore();
	afx_msg void OnRadioTick4Yes();
	afx_msg void OnRadioTick4No();
	/****/
	afx_msg void OnRadioRange();
	afx_msg void OnRadioList();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnButtonList();
	/****/
	afx_msg void OnSelectGroup();
	/****/
	afx_msg void OnSelectOrderBy();
	DECLARE_MESSAGE_MAP()

private:
	void SetRadioTick1(int n);
	void SetRadioTick2(int n);
	void SetRadioTick3(int n);
	void SetRadioTick4(int n);
	
private:
	void SetSkipText();
	void SetReportingRange();
	void AddSortOnOption( CString strLabel, CString strComboText, CString& strSortOnLabel, int& nSortOnIndex, int& nSortOnCurSel);
	void CheckTicks( CString strText, CStatic& static1, CButton& radio1, CButton& radio2, CButton& radio3, int& nTickCount);

private:
	CDbExportHandler* m_pDbExportHandler;
	CStringArray m_arraySortOnLabels;

private:
	int m_nSingleGroupNo;
	int m_nTick1FilterType;
	int m_nTick2FilterType;
	int m_nTick3FilterType;
	int m_nTick4FilterType;
	int m_nCardRangeType;
};

//$$******************************************************************
