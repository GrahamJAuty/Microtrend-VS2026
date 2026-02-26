#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/

struct CPaymentMapEditInfo
{
public:
	int m_nSportsBookerPayNo;
	int m_nCurrentSharpPosNo;
	bool m_bUnknownSharpPosNo;
	CString m_strCurrentSharpPosCellName;
	CString m_strCurrentSharpPosListName;
};

/**********************************************************************/

class CSportsBookerPaymentMapDlg : public CSSDialog
{
public:
	CSportsBookerPaymentMapDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CSportsBookerPaymentMapDlg)
	enum { IDD = IDD_SPTBOOK_PAYMENT_MAP };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSportsBookerPaymentMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CTabCtrl m_TabPayments;
	CComboListCtrl m_listPayments;
	
protected:
	//{{AFX_MSG(CSportsBookerPaymentMapDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	void CreateHelperArrays();
	void AddList();
	void AddField( CCSV& csv );
	void SaveTabChanges();

private:
	void GetTabItemCount( int& nStartIdx, int& nItemsOnPage );

private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CArray<CPaymentMapEditInfo,CPaymentMapEditInfo> m_arrayEditInfo;

private:
	int m_nTabPage;
	
private:
	CellTypeInfoTextBuffer m_PaymentTextBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
