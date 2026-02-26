#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PersistentSelectionEdit.h"
#include "PictCtrl.h"
#include "PluCSVArray.h"
/**********************************************************************/

class CPropPagePluInfo : public CSSPropertyPage
{
public:
	CPropPagePluInfo();
	~CPropPagePluInfo();

	//{{AFX_DATA(CPropPagePluInfo)
	enum { IDD = IDD_PROPPAGE_PLU_INFO };
	CTabCtrl m_TabInfoType;
	CPersistentSelectionEdit m_editText;
	CSSListCtrlVirtual m_listFiles;
	CPictureCtrl m_staticImage;
	CStatic m_staticWidth;
	CStatic m_staticHeight;
	CEdit m_editWidth;
	CEdit m_editHeight;
	CButton m_checkModifier;
	CButton m_buttonSelect;
	CButton m_buttonDelete;
	CStatic m_staticTooLarge;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePluKpInfo)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectResource(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonDelete();

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void Refresh();
	bool UpdateRecord();
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	
private:
	void ShowTextTab();
	void ShowImageTab();
	void ShowHTMLTab();
	void ShowResourceTab();
	void ShowSelectedResource();
	void SelectResourceListItem( int nSel );
	bool LoadTextField( const char* szFilename );
	
private:
	void SaveTextTabInfo();
	void SaveHTMLTabInfo();
	
private:
	void SelectTextFile();
	void SelectImageFile();
	void SelectHTMLFile();
	void SelectResourceFile();
	void DeleteTextFile();
	void DeleteImageFile();
	void DeleteHTMLFile();
	void DeleteResourceFile();
	
protected:
	//{{AFX_MSG(CPropPagePluInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluCSVRecord* m_pPluRecord;
	CString m_strInitialDir;
	bool m_bReadOnly;
	
private:
	int m_nTabType;
	bool m_bAllowChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
