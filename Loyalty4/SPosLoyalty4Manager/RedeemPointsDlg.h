#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\RedeemPoints.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CRedeemPointsDlg : public CSSDialog
{
public:
	CRedeemPointsDlg (CWnd* pParent = NULL);  
	virtual ~CRedeemPointsDlg ();
	void DisplayNoShowReason( int nReason );

// Dialog Data
	enum { IDD = IDD_REDEEMPOINTS };
	int m_nRedeemPoints;
	double m_dRedeemValue;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickStaticComment();
	afx_msg void OnSpinRedeemPoints(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusEditPoints();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPoints();

public:
	int m_nTripFactor;
	double m_dTripValue;
	bool m_bDisableRedeem;

private:
	CEdit m_editTrip;
	CEdit m_editAvailable;

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
	int m_nMaxRedeemPoints;
};

//$$******************************************************************
