#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define VIEWMODE_LIST_ITEM_STANDARD 0
#define VIEWMODE_LIST_ITEM_LARGE 1
#define VIEWMODE_LIST_FUNCTION_STANDARD 2
#define VIEWMODE_LIST_FUNCTION_LARGE 3
#define VIEWMODE_LIST_FUNCTION_BOTTOM 4
#define VIEWMODE_SCREEN 5
/**********************************************************************/
#include "SPOSKeyboardKeyRecord.h"
#include "..\SPosEcrManager5\SPOSKeyboardPluEditEcrman.h"
#include "..\SPosStockManager5\SPOSKeyboardPluEditStockman.h"
/**********************************************************************/

class CSPOSKeyboardView : public CScrollView
{
protected: 
	CSPOSKeyboardView();
	DECLARE_DYNCREATE(CSPOSKeyboardView)
	
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardView)
	public:
	virtual void OnDraw(CDC* pDC);  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

public:
	virtual ~CSPOSKeyboardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CSPOSKeyboardView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditKey();
	afx_msg void OnEditPluMaster();
	afx_msg void OnEditPluPriceSet1();
	afx_msg void OnEditPluPriceSet2();
	afx_msg void OnEditPluPriceSet3();
	afx_msg void OnCreatePlu();
	afx_msg void OnLinkPlu();
	afx_msg void OnCopyStyle();
	afx_msg void OnCopyImage();
	afx_msg void OnCopyKey();
	afx_msg void OnSplitKey();
	afx_msg void OnGroupKey();
	afx_msg void OnTagKey();
	afx_msg void OnUntagKey();
	afx_msg void OnPasteKey();
	afx_msg long OnPluSortPromptMessage( WPARAM wIndex, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void InitialUpdate();
	void HandleResize();

public:
	void SaveKeyboard();
	void ImportWizardData();
	void ImportImages();
	void ZoomSmall();
	void ZoomNormal();
	void ZoomBig();
	void EditKeyTypes();
	void GlobalSettings();
	void EditMacros();
	void SetOptions();

public:
	void SelectViewMode();
	void CheckInvisible();
	void CheckImages();
	void SelectList();
	void ButtonAdd();
	void ButtonEdit();
	void ButtonDelete();
	void ButtonPrintList();
	void SelectPage();
	void ButtonAddPage();
	void ButtonEditPage();
	void ButtonDeletePage();
	void PluSort();

public:
	void SelectPage( int nPageNo );

private:
	void SetKeyboardDialogTitle();
	void CreateScreen(CDC* pDC, __int64 nListNo, int nPageNo );
	void DrawViewKeyText ( CDC* pDC, CStringArray& Texts, CRect rect, CSPOSKeyboardKeyRecord& KeyRecord, int nTextColour );
	void DropKey ( CPoint point );
	void RedrawKey( int nSharpKeyPos );
	void ShowPicture( CDC* pDC, CRect& rect, CSPOSKeyboardKeyRecord& KeyRecord );
	void EditPlu( int nMode );

public:
	void RedrawEditedKey();

private:
	void RefreshListCombo();
	void RefreshPageCombo();
	void GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount );
	void CancelKeyGrouper();

private:
	void AddList( __int64 nListNo, int nListType );
	void AddScreen( int nScreenNo );
	void EditList( __int64 nListNo );
	void EditScreen( int nScreenNo );
	void DeleteList( __int64 nListNo );
	void DeleteScreen( int nScreenNo );
	
protected:
	int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );

private:
	void RebuildKeymap();

private:
#ifdef STOCKMAN_SYSTEM
	void CreatePluKey( int nKeyIdx, CSPOSKeyboardPluEditStockman& PluEdit, bool bUseDefaultText, bool bUseDefaultImage, __int64 nPluNoSave);
#else
	void CreatePluKey( int nKeyIdx, CSPOSKeyboardPluEditEcrman& PluEdit, bool bUseDefaultText, bool bUseDefaultImage, __int64 nPluNoSave);
#endif

private:
	int m_nEditSharpKeyPos;
	int m_nMenuSharpKeyPos;
	bool m_bMenuIsPluKey;
	bool m_bMenuPluExists;
	__int64 m_nMenuPluNo;
	bool m_bMenuVisible;
	bool m_bSortPreviewMode;
	int m_nZoomType;
	int m_nNewPluDeptNo;

private:
	CBrush m_KeyBrush;

private:
	friend class CSPOSKeyboardModelessPointers;
};

/**********************************************************************/
#endif
/**********************************************************************/
