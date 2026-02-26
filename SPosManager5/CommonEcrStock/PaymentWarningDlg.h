#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPaymentWarningDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaymentWarningDlg)

public:
	CPaymentWarningDlg(CWnd* pParent = NULL); 
	virtual ~CPaymentWarningDlg();

	enum { IDD = IDD_PAYMENT_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
