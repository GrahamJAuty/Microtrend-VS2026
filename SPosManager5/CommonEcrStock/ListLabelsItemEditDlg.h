#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LabelListCSVArray.h"
//include "MySSDialog.h"
/**********************************************************************/

class CListLabelsItemEditDlg : public CSSDialog
{
public:
	CListLabelsItemEditDlg( CLabelListCSVRecord& LabelListRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CListLabelsItemEditDlg)
	enum { IDD = IDD_LIST_LABELS_ITEM_EDIT };
	CStatic m_staticBarcode;
	CEdit m_editBarcode;
	CStatic m_staticPlu;
	CEdit m_editPlu;
	CStatic m_staticDescription;
	CEdit m_editDescription;
	CStatic m_staticModifier;
	CEdit m_editModifier;
	CStatic m_staticQty;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListLabelsItemEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListLabelsItemEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditQty();
	
private:
	int m_nBarcodeQty;
	
private:
	CLabelListCSVRecord& m_LabelListRecord;
};

/**********************************************************************/

