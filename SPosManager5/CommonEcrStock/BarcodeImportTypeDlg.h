#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CBarcodeImportTypeDlg : public CDialog
{
public:
	CBarcodeImportTypeDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CBarcodeImportTypeDlg)
	enum { IDD = IDD_BARCODE_IMPORTTYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeImportTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeImportTypeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nImportMethod;
};

/**********************************************************************/

