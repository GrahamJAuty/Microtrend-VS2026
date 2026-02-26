#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "AlertText.h"
#include "BatchUpdateHelpers.h"
#include "CommentHandler.h"
//*******************************************************************
#define nBATCH_RANGE	0
#define nBATCH_LIST		1
//******************************************************************
// CBatchPropertyPage1 dialog

class CBatchPropertyPage1 : public CSSPropertyPage
{
public:
	CBatchPropertyPage1();
	virtual ~CBatchPropertyPage1() {};
	void SetPointer ( CBatchUpdateHelpers* pUpdateHelpers, const char* szTitle, int nUpdateType );
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_BATCH1 };
	CButton m_checkValid;
	CButton m_checkExpired;
	CButton m_checkHotlisted;
	CButton m_checkInactive;
	/*****/
	CButton m_radioRange;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CSSTabbedComboBox m_comboGroup;
	CButton m_checkBirthday;
	/*****/
	CSSComboBox m_comboGender;
	CStatic m_staticAlert[9];
	CSSComboBox m_comboAlert[9];
	/*****/
	CStatic m_staticComment;
	CSSComboBox m_comboComment;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnStaticComment();	
	DECLARE_MESSAGE_MAP()

private:
	void SetReportingRange();
	bool HaveRecords();

private:
	void AddComments();

public:
	int m_nSingleGroupNo;
	int m_nCardRangeType;

private:
	int m_nUpdateType;
	CString m_strLabel;
	CString m_strTitle;

	int m_nMaxCardNoLength;

	CCommentHandler m_Comments;
	CBatchUpdateHelpers* m_pUpdateHelpers;
	CAlertText m_alertText;
};

//*******************************************************************
