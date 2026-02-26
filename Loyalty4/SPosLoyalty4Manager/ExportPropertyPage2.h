#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "AlertText.h"
#include "DbExportHandler.h"
//$$******************************************************************

class CExportPropertyPage2 : public CSSPropertyPage
{
public:
	CExportPropertyPage2();
	virtual ~CExportPropertyPage2();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

	enum { IDD = IDD_PROPPAGE_EXPORT2 };
	CStatic m_staticInclude;
	CButton m_checkValid;
	CButton m_checkExpired;
	CButton m_checkHotlisted;
	CButton m_checkInactive;
	CButton m_checkUnknown;
	/****/
	CStatic m_staticOrderBy;
	CSSComboBox m_comboOrderBy;
	CButton m_checkSkipBlanks;
	/****/
	CButton m_radioRange;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CSSTabbedComboBox m_comboGroup;
	CButton m_checkSortByGroup;
	CButton m_checkGroupSummary;
	/****/
	CSSComboBox m_comboGender;
	CStatic m_staticAlert[9];
	CSSComboBox m_comboAlert[9];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnSelectGroup();
	afx_msg void OnSelectOrderBy();
	
private:
	void SetSkipText();
	void SetReportingRange();
	void AddOrderByLabel(CString strComboLabel, CString strArrayLabel, CString strCheckLabel, int& nOrderByIndex, int& nOrderByCurSel);

public:
	int m_nSingleGroupNo;

private:
	int m_nCardRangeType;
	int m_nMaxCardNoLength;

private:
	CDbExportHandler* m_pDbExportHandler;

	CUserTexts m_Texts;
	CAlertText m_alertText;

	CStringArray m_arrayOrderByLabels;
};

//$$******************************************************************
