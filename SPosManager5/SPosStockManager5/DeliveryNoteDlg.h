#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

struct CSavedSessionInfo
{
	int m_nSuppNo;
	CString m_strFilename;
	CString m_strReference1;
	CString m_strReference2;
	CString m_strComboText;
};

/**********************************************************************/

class CDeliveryNoteDlg : public CDialog
{
public:
	CDeliveryNoteDlg( int nSpIdx, int nType, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDeliveryNoteDlg)
	enum { IDD = IDD_DELIVERY_NOTE };
	CStatic	m_staticSupplier;
	CEdit	m_editReference1;
	CSSComboBox m_comboSession;
	CSSComboBox m_comboSupplier;
	CString	m_strReference1;
	CString	m_strReference2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDeliveryNoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDeliveryNoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnSelectSession();
	DECLARE_MESSAGE_MAP()

public:
	void CreateSessionList();
	int GetSessionListSize(){ return m_arraySessions.GetSize(); }

private:
	void FillSessionCombo();

private:
	int m_nSpIdx;
	int m_nType;
	bool m_bAutoNum;
	int m_nSession;

private:
	CString m_strNewReference1;
	CString m_strNewReference2;

	CArray<CSavedSessionInfo,CSavedSessionInfo> m_arraySessions;

	CStocktakeCSVRecord* m_pStocktake;
};

/**********************************************************************/
#endif
/**********************************************************************/
