#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluImportTypeDlgStockman : public CDialog
{
public:
	CPluImportTypeDlgStockman( CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluImportTypeDlgStockman)
	enum { IDD = IDD_PLU_IMPORTTYPE_STOCKMAN };
	BOOL	m_bCreateStock;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluImportTypeDlgStockman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluImportTypeDlgStockman)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nImportMethod;
};

/**********************************************************************/
#endif				//__PLUPREFSDLG_H__
/**********************************************************************/
