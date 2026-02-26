#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/

class CAdhocDeliveryUnitDlg : public CDialog
{
public:
	CAdhocDeliveryUnitDlg( CStockCSVRecord& StockRecord, int nStockSuppIdx, int nInvoiceSuppNo, CWnd* pParent = NULL);

	//{{AFX_DATA(CAdhocDeliveryUnitDlg)
	CButton	m_buttonPreferred;
	CButton	m_radioAction1;
	CButton	m_radioAction2;
	CButton	m_radioAction3;
	CButton	m_radioAction4;
	CStatic	m_staticDeliveryUnit;
	CStatic	m_staticSupplier;
	CStatic	m_staticStockcode;
	CStatic	m_staticDescription;
	CStatic	m_staticCost;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdhocDeliveryUnitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAdhocDeliveryUnitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioAction1();
	afx_msg void OnRadioAction2();
	afx_msg void OnRadioAction3();
	afx_msg void OnRadioAction4();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetStockSuppIdx() { return m_nStockSuppIdx; }

private:
	CStockCSVRecord& m_StockRecord;
	int m_nStockSuppIdx;
	int m_nInvoiceSuppNo;

private:
	int m_nActionType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
