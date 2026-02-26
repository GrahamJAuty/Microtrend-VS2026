//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CAddCashDlg : public CSSDialog
{
public:
	CAddCashDlg(bool bSpend, bool bPurse2, CWnd* pParent = NULL);   
	
// Dialog Data
	enum { IDD = IDD_ADDCASH };
	double m_dValue;
	double m_dPurseBalance;
	CStatic m_staticAction;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedStaticComment();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
	bool m_bSpend;
	bool m_bPurse2;
};

//$$******************************************************************