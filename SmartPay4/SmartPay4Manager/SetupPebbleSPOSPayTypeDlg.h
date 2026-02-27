#pragma once
/******************************************************************************/
#include "resource.h"
/******************************************************************************/

class CSetupPebbleSPOSPayTypeDlg : public CSSAutoShutdownDialog
{
public:
	CSetupPebbleSPOSPayTypeDlg( CWnd* pParent = NULL);
	virtual ~CSetupPebbleSPOSPayTypeDlg();

	// Dialog Data
	enum { IDD = IDD_SETUP_PEBBLE_SPOSPAYTYPE };
	CSSComboBox m_comboSPOSPayType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	int m_nPaymentType;
};

/******************************************************************************/
