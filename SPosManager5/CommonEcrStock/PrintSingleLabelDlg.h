#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/

class CPrintSingleLabelDlg : public CSSDialog
{
public:
	CPrintSingleLabelDlg( bool bProduct, CPluCSVRecord& PluRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPrintSingleLabelDlg)
	enum { IDD = IDD_PRINT_SINGLE_LABEL };
	CStatic m_staticPlu;
	CStatic m_staticDescription;
	int m_nPos;
	int m_nQty;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrintSingleLabelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPrintSingleLabelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bProduct;
	CPluCSVRecord& m_PluRecord;
};

/**********************************************************************/
#endif 
/**********************************************************************/
