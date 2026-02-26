#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SubUnitEdit.h"
#include "DelUnitSelector.h"
//include "MySSDialog.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockEditReorderRandomDlg : public CSSDialog
{
public:
	CStockEditReorderRandomDlg( CStockFilterArray* pStockFilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockEditReorderRandomDlg)
	enum { IDD = IDD_STOCK_EDIT_REORDER_RANDOM };
	CStatic m_staticLevels;
	CStatic m_staticReorder;
	CStatic m_staticMaximum;
	CButton	m_radioStockCode;
	CButton	m_radioPluNumber;
	CButton	m_buttonUndo;
	CEdit	m_editDescription;
	CButton	m_buttonAdd;
	CEdit	m_editStockCode;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockEditReorderRandomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	bool m_bModified;

protected:
	//{{AFX_MSG(CStockEditReorderRandomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonAdd();
	virtual void OnCancel();
	afx_msg void OnKillFocusPluNo();
	virtual void OnOK();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnButtonUndo();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FindStockCodeFromPlu();
	void FindPluFromStockCode();
	void CheckStockCode();
	void Undo( bool bUpdateList );
	void PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos );
	int HandleAdd();

private:
	CEdit* GetEditStockQtyMinimum();
	CEdit* GetEditStockQtyReorder();
	CEdit* GetEditStockQtyMaximum();
	CEdit* GetEditPluNo();

private:
	CSubUnitEdit m_editStockSubMinimum;
	CSubUnitEdit m_editStockSubReorder;
	CSubUnitEdit m_editStockSubMaximum;
	
private:
	double m_dQuantityMinimum;
	__int64 m_nTotalSubUnitsMinimum;
	double m_dQuantityReorder;
	__int64 m_nTotalSubUnitsReorder;
	double m_dQuantityMaximum;
	__int64 m_nTotalSubUnitsMaximum;

private:
	bool m_bAllowReorder;
	CStockFilterArray m_StockFilterArray;
	CPluFilterArray m_PluFilterArray;
	CString m_strStockCode;
	CString m_strDescription;
	CSSListCtrl m_listReorder;
	CStringArray m_ReorderArray;
	CString m_strPluNo;
	int m_nEntryMode;
	int m_nStockIdx;
	int m_nDbNo;
	int m_nSpNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
