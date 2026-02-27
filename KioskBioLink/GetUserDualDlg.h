#pragma once
//*******************************************************************
#include "Defines.h"
#include "SystemData.h"
//*******************************************************************
#include "cardlink.h"
//*******************************************************************

class CGetUserDualDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CGetUserDualDlg)

public:
	CGetUserDualDlg(CWnd* pParent);
	virtual ~CGetUserDualDlg();
	int GetInterfaceType() { return m_nInterfaceReqd; }
	bool HaveInputFile() { return m_bHaveInputFile; }

	enum { IDD = IDD_GETDUALDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonFinger();
	afx_msg void OnButtonReadcard();

	DECLARE_MESSAGE_MAP()

private:
	void StartTimer();
	void StopTimer();
	void ShowDateTime();
	void HandleTimer();

private:
	UINT m_hTimer = 0;
	int m_nInterfaceReqd = nINTERFACE_BIOMETRIC;
	bool m_bHaveInputFile = FALSE;

	CSSColourStatic m_staticPrompt;
	CSSColourButton m_buttonBiometric;
	CSSColourButton m_buttonReadCard;

	LONGLONG m_nLastMessageTime = 0;
	int m_nMessageInterval = nINTERVAL_MESSAGE;

	CSSColourStatic m_staticDateTime;
	CString m_strStaticDateTimeText = "";
	CString m_strCurrentDateTimeText = "";

	bool m_bInTimer = FALSE;
};

//*******************************************************************

