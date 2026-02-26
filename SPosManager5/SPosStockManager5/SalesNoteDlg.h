#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSalesNoteDlg : public CDialog
{
public:
	CSalesNoteDlg( int nSpIdx, int nType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSalesNoteDlg)
	enum { IDD = IDD_SALES_NOTE };
	CSSComboBox m_comboSession;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesNoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesNoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillSessionCombo();

public:
	int GetSessionCount(){ return m_arraySessions.GetSize(); }
	const char* GetFilename(){ return m_strFilename; }

private:
	int m_nType;
	int m_nSpIdx;
	CStringArray m_arraySessions;
	CString m_strFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/
