#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPosTrayExportFieldDlg : public CSSDialog
{
public:
	CPosTrayExportFieldDlg( int nTaskFileType, int nFieldType, CWnd* pParent = NULL);
	CPosTrayExportFieldDlg( int nTaskFileType, CPosTrayExportFieldDlg** pParentToThis, CWnd* pParent = NULL);
	virtual ~CPosTrayExportFieldDlg();

	enum { IDD = IDD_POSTRAY_EXPORT_FIELDS };

private:
	void Init();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListField(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListField(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectType(NMHDR* pNMHDR, LRESULT* pResult);
	void OnButtonAdd();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	
private:
	void LoadTypeArray( int nSel );

private:
	CSSListCtrlVirtual m_listType;
	CSSListCtrlVirtual m_listField;
	CButton m_buttonOK;
	CButton m_buttonAdd;

private:
	CArray<int,int> m_arrayTypes;
	CArray<int,int> m_arrayFields;
	int m_nTypeSel;
	
public:
	int m_nTaskFileType;
	int m_nFieldType;
	bool m_bAdd;

private:
	CPosTrayExportFieldDlg** m_pParentToThis;
};

/**********************************************************************/
#endif
/**********************************************************************/
