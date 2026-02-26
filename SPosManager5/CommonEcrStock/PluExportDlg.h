#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluExportDlg : public CSSDialog
{
public:
	CPluExportDlg( CPluFilterArray& FilterArray, int nStartPos, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPluExportDlg)
	enum { IDD = IDD_PLU_EXPORT };
	CButton	m_checkChangesOnly;
	CEdit	m_editDestination;
	BOOL	m_bChangesOnly;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPluExportDlg)
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
	//{{AFX_MSG(CPluExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusPluNo();
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
	const char* GetPluHeaderLine() { return m_strPluHeaderLine; }

private:
	void AppendPluHeader ( CCSV& csv );
	void CreatePluHeaderLine();
	void CreateSavedSettingsLine();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	
public:
	CString m_strPluFrom;
	CString m_strPluTo;
	
private:
	CPluFilterArray& m_FilterArray;
	CSSListTaggedSelectCtrl m_listFields;
	CString m_strPluHeaderLine;
	CString m_strSavedSettingsLine;
};

/**********************************************************************/

