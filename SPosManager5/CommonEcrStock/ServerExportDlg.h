#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CServerExportDlg : public CSSDialog
{
public:
	CServerExportDlg( int nServerDbIdx, int nServerLocIdx, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CServerExportDlg)
	enum { IDD = IDD_SERVER_EXPORT };
	CEdit	m_editDestination;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CServerExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void CreateAvailableFieldsArray( CStringArray& arrayFields, CReportConsolidationArray<CSortedStringItem>& arrayAvailableFields );
	void AddFieldToList( const char* szText, int nTagState );

private:
	void ConsolidateString( CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField );
	void ConsolidateString( CStringArray& arrayStrings, CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField );

protected:
	//{{AFX_MSG(CServerExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnStaticFrom();
	afx_msg void OnStaticTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetServerHeaderLine() { return m_strServerHeaderLine; }

private:
	void AppendServerHeader ( CCSV& csv );
	void CreateServerHeaderLine();
	void CreateSavedSettingsLine();

private:
	CEdit* GetEditServerFrom();
	CEdit* GetEditServerTo();
	
public:
	CString m_strServerFrom;
	CString m_strServerTo;
	
private:
	CSSListTaggedSelectCtrl m_listFields;
	CString m_strServerHeaderLine;
	CString m_strSavedSettingsLine;
	int m_nServerDbIdx;
	int m_nServerLocIdx;
};

/**********************************************************************/

