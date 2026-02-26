#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "PictCtrl.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardImageImportDlg : public CDialog
{
public:
	CSPOSKeyboardImageImportDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CSPOSKeyboardImageImportDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_IMAGE_IMPORT };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardImageImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardImageImportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFile();
	afx_msg void OnButtonFolder();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetNewSelectedImage(){ return m_strNewSelectedImage; }

private:
	void AddFileToList( int nPos, const char* szText );
	void SelectListItem();
	
private:
	CSSListTaggedSelectCtrl m_listFiles;
	
private:
	CReportConsolidationArray<CSortedImageInfo> m_arrayImageInfo;
	CString m_strInitialDir;

private:
	CPictureCtrl m_staticImage;
	CString m_strNewSelectedImage;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
