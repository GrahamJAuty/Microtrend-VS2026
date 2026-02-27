#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\RedeemPoints.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CRedeemPointsDlg : public CSSAutoShutdownDialog
{
public:
	CRedeemPointsDlg (CWnd* pParent = NULL);  
	void DisplayNoShowReason( int nReason );

	enum { IDD = IDD_REDEEMPOINTS };
	int m_nRedeemPoints;
	double m_dRedeemValue;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnClickStaticComment();
	afx_msg void OnSpinRedeemPoints(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusPoints();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPoints() { return GetEdit(IDC_EDIT_POINTS); }

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void AddComments();

public:
	int m_nTripFactor;
	double m_dTripValue;
	bool m_bDisableRedeem;
	int m_nRedeemPurse;

private:
	CEdit m_editTrip;
	CEdit m_editAvailable;
	CStatic m_staticBonus;

private:
	CCommentHandler m_Comments;
	int m_nMaxRedeemPoints;
};

//$$******************************************************************
