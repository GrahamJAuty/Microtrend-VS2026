#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSportsBookerPriceBandDlg : public CSSDialog
{
public:
	CSportsBookerPriceBandDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CSportsBookerPriceBandDlg)
	enum { IDD = IDD_SPTBOOK_PRICEBAND };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CSportsBookerPriceBandDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	CTabCtrl m_TabBands;
	
private:
	int m_staticBandID[10];
	int m_comboBandID[10];
	int m_editTextID[10];
	CStatic m_staticBand[10];
	CSSComboBox m_comboBand[10];
	CEdit m_editText[10];

protected:
	//{{AFX_MSG(CSportsBookerPriceBandDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectBand1();
	afx_msg void OnSelectBand2();
	afx_msg void OnSelectBand3();
	afx_msg void OnSelectBand4();
	afx_msg void OnSelectBand5();
	afx_msg void OnSelectBand6();
	afx_msg void OnSelectBand7();
	afx_msg void OnSelectBand8();
	afx_msg void OnSelectBand9();
	afx_msg void OnSelectBand10();

private:
	void GetTabItemCount( int& nStartIdx, int& nItemsOnPage );
	void LoadTabSettings();
	void SaveTabChanges();

private:
	void SelectSharpPosBand( int nRow );
	void UpdateTextField( int nRow, CSportsBookerPriceBandCSVRecord Record );

private:
	int m_nTabPage;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
