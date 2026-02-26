#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CTransferNoteDlg : public CDialog
{
public:
	CTransferNoteDlg( int nSpIdx, CWnd* pParent = NULL);
	bool IsRelevant() { return m_bIsRelevant; }
	int GetDestSpIdx() { return m_nDestSpIdx; }
	const char* GetReference1() { return m_strReference1; }
	const char* GetReference2() { return m_strReference2; }

	//{{AFX_DATA(CTransferNoteDlg)
	enum { IDD = IDD_TRANSFER_NOTE };
	CEdit	m_editReference1;
	CSSComboBoxEx	m_comboStockpoint;
	CString	m_strReference1;
	CString	m_strReference2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransferNoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTransferNoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;
	int m_nSourceSpIdx;
	int m_nDestSpIdx;
	bool m_bAutoNum;
	bool m_bIsRelevant;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/


