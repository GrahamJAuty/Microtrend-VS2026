#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CBarcodeTableDefaultsDlg : public CDialog
{
	DECLARE_DYNAMIC(CBarcodeTableDefaultsDlg)

public:
	CSSComboBox m_comboBarcode;
	CSSComboBox m_comboPlu;

public:
	CBarcodeTableDefaultsDlg(CWnd* pParent = NULL);   
	virtual ~CBarcodeTableDefaultsDlg();

// Dialog Data
	enum { IDD = IDD_BARCODETABLE_DEFAULTS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void OnOK();

public:
	int m_nBarcodeAction;
	int m_nPluAction;

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
