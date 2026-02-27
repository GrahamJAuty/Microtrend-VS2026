#pragma once

#include <winsubs.hpp>
#include "cardlink.h"

// CCashAddedDlg dialog

class CCashAddedDlg : public CSSDialog
{
//	DECLARE_DYNAMIC(CCashAddedDlg)

public:
	CCashAddedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCashAddedDlg();

// Dialog Data
	enum { IDD = IDD_CASHADDEDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);

private:
	void StartTimer();
	void StopTimer();
	void CheckForCash();

private:
	UINT m_hTimer;
	bool m_bCancelRequested;
	double m_dCashAdded;

	DECLARE_MESSAGE_MAP()
};
