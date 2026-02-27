#pragma once
//*******************************************************************
#include "Cardlink.h"
#include "PaylinkInterface.h"
#include "UserIDRecord.h"
//*******************************************************************

class CPhotoIDEnquiryDlg : public CSSColourDialog
{
public:
	CPhotoIDEnquiryDlg( CUserIDRecord* pRecord, int nTopupPurse, CWnd* pParent);
	~CPhotoIDEnquiryDlg();

	enum { IDD = IDD_PHOTOENQUIRYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	void StartTimer();
	void StopTimer();
	void HandleTimer();
	void OpenPayLinkInterface();
	void LogPayLinkAddition ( long lCoinageAdded );
	void LogPaylinkCurrentLevel();
	void HideCashAddedBox();
	void DisplayBalances();

public:
	bool m_bHaveInputFile = FALSE;

private:
	CPaylinkInterface m_paylink;
	CUserIDRecord* m_pRecord = nullptr;
	UINT m_hTimer = 0;
	LONGLONG m_nLastTimeoutTime = 0;
	LONGLONG m_nLastInputTime = 0;	
	int m_nTimeoutInterval = 2;
	int m_nInputInterval = nINTERVAL_INPUT;
	
	CString m_strCardNo = "";
	CString m_strCardName = "";
	CString m_strDateLastUsed = "";
	int m_nTopupPurse = 1;

	CSSColourButton m_buttonOK;
	CSSColourStatic m_staticTextCardNo;
	CSSColourStatic m_staticSurname;
	CSSColourStatic m_staticTextGroupNo;
	CSSColourStatic m_staticGroupNo;
	CSSColourStatic m_staticIssueNo;
	CSSColourStatic m_staticTextIssueNo;
	CSSColourStatic m_staticNotAccepted;

	CSSColourStatic m_staticTextBalance;
	CSSColourStatic m_staticBalance;
	CSSColourStatic m_staticTextAvailableSpend;
	CSSColourStatic m_staticAvailableSpend;
	CSSColourStatic m_staticTextLastUsed;
	CSSColourStatic m_staticLastUsed;

	CSSColourStatic m_staticTextPoints;
	CSSColourStatic m_staticPoints;

	CSSColourStatic m_staticTextLastRefresh;
	CSSColourStatic m_staticLastRefresh;

	CSSColourStatic m_staticNoImage;
	CSSColourStatic m_staticPicture;

	CSSColourStatic m_staticTextAdded;
	CSSColourStatic m_staticCashAdded;

	int m_nPayLinkRunning = nSTART_REQD;
	long m_lPayLinkCurrentValue = 0;
	long m_lPayLinkTTLAdded = 0;

	bool m_bInTimer = FALSE;
};

//*******************************************************************
