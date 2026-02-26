#pragma once
/*********************************************************************/
#ifdef STOCKMAN_SYSTEM
/*********************************************************************/
#include "SharedResources.h"
/*********************************************************************/
//include "MyComboBox.h"
//include "MyComboBoxEx.h"
/*********************************************************************/

class CSetupWarehouseDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupWarehouseDlg)

public:
	CSetupWarehouseDlg( int nDbIdx, CWnd* pParent = NULL);   
	virtual ~CSetupWarehouseDlg();

	CStatic m_static1;
	CStatic m_static2;
	CSSComboBoxEx m_comboStockpoint;
	CSSComboBox m_comboTransfer1;
	CSSComboBox m_comboTransfer2;

	enum { IDD = IDD_SETUP_WAREHOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	void AddStockpointToCombo ( const char* szText, int nImage );

private:
	CImageList m_ImageList;
	int m_nDbIdx;

public:
	int m_nSpIdx;
	int m_nTransfer1;
	int m_nTransfer2;
};

/*********************************************************************/
#endif
/*********************************************************************/
