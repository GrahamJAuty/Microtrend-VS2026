#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "PMSAccountFindDlg.h"
/**********************************************************************/

class CPMSBatchInvoiceDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBatchInvoiceDlg)

public:
	CPMSBatchInvoiceDlg( CWnd* pParent = NULL);   
	virtual ~CPMSBatchInvoiceDlg();

public:
	CSSListCtrl m_listBookings;
	CSSTabbedComboBox m_comboCheckOut;

	enum { IDD = IDD_PMS_BATCH_INVOICE };

public:
	virtual BOOL OnInitDialog();
	virtual void OnButtonPrintInvoice();
	virtual void OnButtonPrintList();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);

public:
	afx_msg void OnChangeDateCheckOut();
	afx_msg void OnOpenCheckOut();

private:
	void FillCheckOutCombo();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo> m_arrayBookingInfo;
	int m_nSearchCheckOutBase;
	int m_nSearchCheckOutDay;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
