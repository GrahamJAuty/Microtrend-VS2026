#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

class CAddLoyaltyDlg : public CSSDialog
{
public:
	CAddLoyaltyDlg(int nGroupNo, CWnd* pParent = NULL);  
	enum { IDD = IDD_ADDLOYALTY };

public:
	bool CanShowDialog(){ return ( 0 == m_nNoShowReason ); }
	void DisplayNoShowReason();

private:
	CEdit m_editSpendTD;
	CEdit m_staticCurrent;
	CEdit m_editCurrent;
	CEdit m_editTrip;
	CStatic m_staticPurseSpend;
	CSSComboBox m_comboComments;
	CEdit m_editReward;
	
public:
	void SetSpendToDate( double d ){ m_dSpendToDate = d; }
	void SetCurrentPurse( double d ){ m_dCurrentPurse = d; }
	void SetCurrentPoints( int n ){ m_nCurrentPoints = n; }

public:
	int GetLoyaltyType(){ return m_nLoyaltyType; }

public:
	double GetNewCashSpend(){ return m_dNewCashSpend; }
	double GetNewPurseSpend(){ return m_dNewPurseSpend; }
	double GetNewCashReward(){ return m_dNewCashReward; }
	int GetNewPointsReward(){ return m_nNewPointsReward; }
	const char* GetComment(){ return m_strComment; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusSpend();
	afx_msg void OnClickStaticComment();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPurseSpend();
	CEdit* GetEditCashSpend();

private:
	void AddComments();
	int CalculateLoyaltyPoints();
	double CalculateLoyaltyValue();

private:
	double m_dSpendToDate;
	double m_dCurrentPurse;
	int m_nCurrentPoints;

private:
	double m_dLoyaltyTripValue;
	int m_nLoyaltyType;
	double m_dCashRewardPerTrip;
	int m_nPointsRewardPerTrip;

private:
	CString m_strComment;
	double m_dNewCashSpend;
	double m_dNewPurseSpend;
	double m_dNewCashReward;
	int m_nNewPointsReward;

private:
	CCommentHandler m_Comments;
	
private:
	int m_nNoShowReason;
	bool m_bAllowPurseSpend;
	bool m_bAllowCashSpend;
	CString m_strLabelPurseSpend;
};

//*******************************************************************
