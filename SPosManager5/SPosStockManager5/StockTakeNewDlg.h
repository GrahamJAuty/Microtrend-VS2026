#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "StockTakeCSVArray.h"
/**********************************************************************/

class CStocktakeNewDlg : public CSSDialog
{
public:
	CStocktakeNewDlg( bool bNewStocktake, CCategorySetCSVRecord& CategorySet, CWordArray& array, CWnd* pParent = NULL );   

	//{{AFX_DATA(CStocktakeNewDlg)
	enum { IDD = IDD_STOCKTAKE_NEW };
	CSSComboBox	m_comboSupplier;
	CButton	m_buttonSupplier;
	CButton	m_buttonCategory;
	CSSComboBox m_comboActive; 
	CSSComboBox	m_comboCategory;
	CButton	m_buttonImportSheet;
	CButton	m_checkClosingZero;
	CString	m_strComment;
	int		m_nStocktakeType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStocktakeNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetImportSheetHelpers(int nSpIdx, CStocktakeCSVArray* pArray, CString strDate, int nStocktakeNo)
	{
		m_nSpIdx = nSpIdx;
		m_pStocktakeArray = pArray;
		m_strDate = strDate;
		m_nExistingStocktakeNo = nStocktakeNo;
	}

protected:
	//{{AFX_MSG(CStocktakeNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonCategory();
	afx_msg void OnButtonSupplier();
	afx_msg void OnRadioPeriod();
	//}}AFX_MSG
	afx_msg void OnRadioYTD();
	afx_msg void OnRadioApparent();
	afx_msg void OnButtonImportSheet();
	DECLARE_MESSAGE_MAP()

private:
	void FillCategoryCombo();
	void FillSupplierCombo();
	void UpdateSupplierArray( CWordArray& array );
	void UpdateCategorySet( CCategorySetCSVRecord& CategorySet );
	void UpdateSettings();
	void CreateImportSheetNewStocktake();
	void CreateImportSheetEditStocktake();

public:
	CString m_strCategory;
	CString m_strSupplier;
	bool m_bActive;
	bool m_bClosingZero;

private:
	CCategorySelector m_CategorySelector;
	CCategorySetCSVRecord m_CategorySetCustom;
	CWordArray m_arraySuppIdxCustom;

private:
	CCategorySetCSVRecord& m_CategorySetOut;
	CWordArray& m_arraySuppIdxOut;

private:
	bool m_bNewStocktake;

private:
	CStocktakeCSVArray* m_pStocktakeArray;
	int m_nSpIdx;
	CString m_strDate;
	int m_nExistingStocktakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
