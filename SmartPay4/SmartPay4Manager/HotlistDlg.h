#pragma once
//*********************************************************************
#include "resource.h"
//*********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*********************************************************************
#include "CommentHandler.h"
//*********************************************************************

class CHotlistDlg : public CSSAutoShutdownDialog
{
public:
	CHotlistDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent = NULL);   
	
// Dialog Data
	enum { IDD = IDD_HOTLIST };
	CStatic m_staticMemberID;
	CEdit m_editMemberID;
	CEdit m_editCardNo;
	CEdit m_editSurname;
	CEdit m_editForename;
	CEdit m_editGroupName;
	CEdit m_editPurse1Date;
	CEdit m_editPurse2Date;
	CEdit m_editPurse3Date;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnClickStaticComment();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	int GetReason() { return m_nReasonIndex+1; }

private:
	void AddComments();

private:
	CSQLRowAccountFull& m_RowAccount;
	CCommentHandler m_Comments;
	CSSComboBox m_comboReason;
	int m_nReasonIndex;
};

//*********************************************************************
