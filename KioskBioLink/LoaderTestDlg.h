//*******************************************************************
#include "Cardlink.h"
//*******************************************************************
#include "PaylinkInterface.h"
//*******************************************************************

class CLoaderTestDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CLoaderTestDlg)

public:
	CLoaderTestDlg(CWnd* pParent);
	virtual ~CLoaderTestDlg();

	enum { IDD = IDD_LOADERTEST };

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
	void SetDisplay();
	void OpenPayLinkInterface();
	void LogPaylinkCurrentLevel();

private:
	CPaylinkInterface m_paylink;
	UINT m_hTimer = 0;

	CSSColourButton m_buttonOK;

	CSSColourStatic m_staticText2000;
	CSSColourStatic m_staticText1000;
	CSSColourStatic m_staticText500;
	CSSColourStatic m_staticText200;
	CSSColourStatic m_staticText100;
	CSSColourStatic m_staticText50;
	CSSColourStatic m_staticText20;
	CSSColourStatic m_staticText10;

	CSSColourStatic m_staticQty2000;
	CSSColourStatic m_staticQty1000;
	CSSColourStatic m_staticQty500;
	CSSColourStatic m_staticQty200;
	CSSColourStatic m_staticQty100;
	CSSColourStatic m_staticQty50;
	CSSColourStatic m_staticQty20;
	CSSColourStatic m_staticQty10;

	CSSColourStatic m_static2000;
	CSSColourStatic m_static1000;
	CSSColourStatic m_static500;
	CSSColourStatic m_static200;
	CSSColourStatic m_static100;
	CSSColourStatic m_static50;
	CSSColourStatic m_static20;
	CSSColourStatic m_static10;
	CSSColourStatic m_staticTotalValue;

	int m_nPayLinkRunning = nSTART_REQD;
	long m_lPayLinkCurrentValue = 0;
	long m_lPayLinkTTLAdded = 0;

	int m_nQty2000 = 0;
	int m_nQty1000 = 0;
	int m_nQty500 = 0;
	int m_nQty200 = 0;
	int m_nQty100 = 0;
	int m_nQty50 = 0;
	int m_nQty20 = 0;
	int m_nQty10 = 0;

	int m_nTotal2000 = 0;
	int m_nTotal1000 = 0;
	int m_nTotal500 = 0;
	int m_nTotal200 = 0;
	int m_nTotal100 = 0;
	int m_nTotal50 = 0;
	int m_nTotal20 = 0;
	int m_nTotal10 = 0;

	int m_nTotalValue = 0;

	bool m_bInTimer = FALSE;
};

//*******************************************************************