#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "HistoricalTaxRates.h"
/**********************************************************************/

class CEditHistoricalTaxDlg : public CSSDialog
{
public:
	CEditHistoricalTaxDlg( CTaxDateInfo& info, bool bAdd, CWnd* pParent = NULL);   

	//{{AFX_DATA(CEditHistoricalTaxDlg)
	enum { IDD = IDD_EDIT_HISTORICAL_TAX };
	CDateTimeCtrl	m_DatePicker;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEditHistoricalTaxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEditHistoricalTaxDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//ARRAYS OF CONTROL IDS
	WORD m_IDEditRate[MAX_TAX_BANDS];
	
	//ARRAYS OF VALUES
	double m_dRate[MAX_TAX_BANDS];

private:
	CEdit* GetEditRate( int n );
	
private:
	CTaxDateInfo& m_TaxDateInfo;
	bool m_bAdd;
	int m_nMaxLen;
	int m_nDPQty;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif 
/**********************************************************************/
