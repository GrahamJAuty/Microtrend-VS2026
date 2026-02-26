#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSelectStockpointSurplusDlg : public CDialog
{
public:
	CSelectStockpointSurplusDlg( CArray<int,int>& arraySpIdx, int nSpIdx, CWnd* pParent = NULL);  

	//{{AFX_DATA(CSelectStockpointSurplusDlg)
	enum { IDD = IDD_SELECT_STOCKPOINT_SURPLUS };
	CSSComboBox	m_comboStockpoint;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectStockpointSurplusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSelectStockpointSurplusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nSpIdx;

private:
	CArray<int,int>& m_arraySpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/

