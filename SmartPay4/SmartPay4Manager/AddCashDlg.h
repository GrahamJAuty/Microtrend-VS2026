#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CAddCashDlg : public CSSAutoShutdownDialog
{
public:
	CAddCashDlg( int nPurse, CWnd* pParent = NULL);
	virtual ~CAddCashDlg();

// Dialog Data
	enum { IDD = IDD_ADDCASH };
	double m_dValue;
	double m_dPurseBalance;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClickStaticComment();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	void SetPebbleAdjustMode() { m_bPebbleAdjust = TRUE; }
	void SetPendingPebbleAdjust(double dValue) { m_dPendingPebbleAdjust = dValue; }

private:
	void AddComments();

private:
	int m_nPurse;
	bool m_bPebbleAdjust;
	double m_dPendingPebbleAdjust;
	CCommentHandler m_Comments;
};

//$$******************************************************************
