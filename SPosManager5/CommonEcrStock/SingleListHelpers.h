#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CSingleListContentHelpers
{
public:
	CSingleListContentHelpers();
	virtual ~CSingleListContentHelpers(void);

public:
	int GetListLineColour(){ return m_nListLineColour; }
	
public:
	virtual int GetArraySize() = 0;
	virtual bool PrepareLine( int nIndex, CString& strLine) = 0;
	virtual bool PrepareDeletionLine( int nIndex, CString& strLine){ return PrepareLine( nIndex, strLine ); }
	virtual void DeleteDatabaseEntry( int nIndex ) = 0;
	virtual void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ) = 0;
	
public:
	virtual void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected );
	virtual int GetArrayIdxFromListIdx( int nListIdx );

public:
	bool GetPreventDeletionFlag(){ return m_bPreventDeletion; }

public:
	void ClearDeletionEffectFlag(){ m_bDeletionEffect = FALSE; }
	bool GetDeletionEffectFlag(){ return m_bDeletionEffect; }

protected:
	bool m_bPreventDeletion;
	int m_nListLineColour;

protected:
	CString m_strDeletionText;
	bool m_bDeletionEffect;
};

/**********************************************************************/

class CSingleListDisplayHelpers
{
public:
	CSingleListDisplayHelpers( CSSListCtrlVirtual& list, CSingleListContentHelpers& Content );
	virtual ~CSingleListDisplayHelpers(void);

public:
	const char* GetTopSingleText(){ return m_strTopSingle; }
	const char* GetTopPluralText(){ return m_strTopPlural; }
	const char* GetBottomSingleText(){ return m_strBottomSingle; }
	const char* GetBottomPluralText(){ return m_strBottomPlural; }

public:
	void SetTopSingleText( const char* sz ){ m_strTopSingle = sz; }
	void SetTopPluralText( const char* sz ){ m_strTopPlural = sz; }
	void SetBottomSingleText( const char* sz ){ m_strBottomSingle = sz; }
	void SetBottomPluralText( const char* sz ){ m_strBottomPlural = sz; }

public:
	void SetDeletionList( CSSListCtrlVirtual* pList ){ m_pDeletionList = pList; }
	void PrepareDatabaseList( bool bMultiSelect, bool bEnableColour );
	void PrepareDeletionList( bool bEnableColour );
	void SelectDatabaseLine ( int nIndex );
	void SelectDeletionLine ( int nIndex );

public:
	void HandleDelete( CWnd* pParent );
	int GetFirstDeletedItem(){ return m_nFirstDeletedItem; }
	
public:
	int GetSelectedArraySize(){ return m_arraySelected.GetSize(); }
	int GetSelectedLine( int nIndex );

private:
	void UpdateSelectedItemArray();
	
private:
	CSSListCtrlVirtual& m_list;
	CSSListCtrlVirtual* m_pDeletionList;
	CReportConsolidationArray<CSortedIntItem> m_arraySelected;

protected:
	CString m_strTopSingle;
	CString m_strTopPlural;
	CString m_strBottomSingle;
	CString m_strBottomPlural;
	
protected:
	CSingleListContentHelpers& m_Content;
	int m_nFirstDeletedItem;
};

/**********************************************************************/
#endif
/**********************************************************************/
