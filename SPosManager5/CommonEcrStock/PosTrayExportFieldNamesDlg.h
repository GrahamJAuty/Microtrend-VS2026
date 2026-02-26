#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/

class CPosTrayExportFieldNamesDlg : public CSSDialog
{
public:
	CPosTrayExportFieldNamesDlg( int nTaskFileType, CWnd* pParent = NULL);
	virtual ~CPosTrayExportFieldNamesDlg();

	enum { IDD = IDD_POSTRAY_FIELDNAMES };

private:
	void Init();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	
private:
	void LoadTypeArray( int nSel );

private:
	CSSListCtrlVirtual m_listType;
	
private:
	CArray<int,int> m_arrayTypes;
	CArray<int,int> m_arrayFields;
	int m_nTypeSel;

private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listFields;
	
public:
	int m_nTaskFileType;
};

/**********************************************************************/
#endif
/**********************************************************************/
