#pragma once
/******************************************************************************/
#include "resource.h"
/******************************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRowPebblePaymentType.h"
/******************************************************************************/

class CSetupPebblePaymentDlg : public CSSAutoShutdownDialog
{
public:
	CSetupPebblePaymentDlg(bool bAdd, CSQLRowPebblePaymentType& RowPayment, CWnd* pParent = NULL);   
	
// Dialog Data
	enum { IDD = IDD_SETUP_PEBBLE_PAYMENT };
	CEdit m_editSPOS;
	CEdit m_editSaleName;
	CStatic m_staticTopupType;
	CSSComboBox m_comboTopupType;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CSQLRowPebblePaymentType& m_RowPayment;
	bool m_bAdd;
};

/******************************************************************************/
