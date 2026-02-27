#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************
#define nBATCH_RANGE	0
#define nBATCH_LIST		1
//$$******************************************************************

class CBatchPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CBatchPropertyPage1();
	virtual ~CBatchPropertyPage1();
	void SetPointer ( int nUpdateType );

	__int64 RecordsInRange();
	bool IsValid(CSQLRowAccountFull& RowAccount);
	bool HaveRecords();
	const char* GetComment() { return m_strComment; }

	enum { IDD = IDD_PROPPAGE_BATCH1 };
	
	CStatic m_staticInclude;
	CButton m_checkAlert;
	
	CStatic m_staticUser;
	CSSTabbedComboBox m_tabcomboGroup;
	CEdit m_editFromCardNo;
	CEdit m_editToCardNo;
	CButton m_buttonList;
	
	CStatic m_staticAudit;
	CStatic m_staticComments;
	CSSComboBox m_comboComments;

	CStatic m_staticTickbox;
	CStatic m_staticTick1;
	CButton m_radioTick1;
	CButton m_radioTickYes1;
	CButton m_radioTickNo1;
	CStatic m_staticTick2;
	CButton m_radioTick2;
	CButton m_radioTickYes2;
	CButton m_radioTickNo2;
	CStatic m_staticTick3;
	CButton m_radioTick3;
	CButton m_radioTickYes3;
	CButton m_radioTickNo3;
	CStatic m_staticTick4;
	CButton m_radioTick4;
	CButton m_radioTickYes4;
	CButton m_radioTickNo4;
	
	BOOL m_bIncludeValid;
	BOOL m_bIncludeExpired;
	BOOL m_bIncludeInactive;
	BOOL m_bIncludeAlert;
	BOOL m_bIncludeUnissued;
	
	CString m_strComment;
	int m_nCardRange;
	CString m_strFromCardNo;
	CString m_strToCardNo;
	
	int m_nTick1Filter;
	int m_nTick2Filter;
	int m_nTick3Filter;
	int m_nTick4Filter;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticComment();
	afx_msg void OnBnClickedRadioRange();
	afx_msg void OnBnClickedRadioList();
	afx_msg void OnBnClickedButtonList();
	afx_msg void OnBnDoubleclickedRadioList();

	DECLARE_MESSAGE_MAP()

private:
	void AddComments();
	void SetReportingRange();
	bool CheckUserTicks(CSQLRowAccountFull& RowAccount);
	bool ValidateTickReqd();

public:
	int m_nSingleGroupNo;

private:
	CCommentHandler m_Comments;

	int m_nUpdateType;
	__int64 m_nRangeFromCardNo;
	__int64 m_nRangeToCardNo;
	CString m_strLabel;
	CCardPicker m_Picker;

	int m_nGroupIndex;
};

//$$******************************************************************

