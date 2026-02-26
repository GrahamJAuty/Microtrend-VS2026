#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

class CHotlistDlg : public CDialog
{
public:
	CHotlistDlg( CSQLRowAccountFull& RowAccount, CWnd* pParent = NULL);   
	~CHotlistDlg();

// Dialog Data
	enum { IDD = IDD_HOTLIST };
	CSSComboBox m_comboComments;
	CString m_strComment;
	CString m_strCardName;
	CString m_strDateLastUsed;
	CString m_strTimeLastUsed;
	CString m_strDateLastRefreshed;
	CString m_strTimeLastRefreshed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnClickStaticComment();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
	CSSTabbedComboBox m_comboGroup;
	CSQLRowAccountFull& m_RowAccount;
};

//*******************************************************************
