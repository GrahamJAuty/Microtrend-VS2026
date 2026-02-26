#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

struct CUpgradeStockInfo
{
	int m_nDbIdx;
	int m_nPluSize;
};

/**********************************************************************/

class CSingleListContentHelpersUpgradeStock : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersUpgradeStock( CArray<CUpgradeStockInfo,CUpgradeStockInfo>& arrayUpgrade );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CArray<CUpgradeStockInfo,CUpgradeStockInfo>& m_arrayUpgrade;
};

/**********************************************************************/

class CListDataUpgradeStockDlg : public CListDataDlg
{
public:
	CListDataUpgradeStockDlg( CWnd* pParent = NULL); 
	~CListDataUpgradeStockDlg();

public:
	int IsRelevant() { return ( GetArraySize() > 0 ); }

private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//UPGRADE
	virtual void HandleButton4();	//VIEW
	virtual void HandleDefaultButton() { HandleButton4(); }

	int UpgradeSystemInternal( int nDbIdx, CDataManagerInfo& info );

private:
	CArray<CUpgradeStockInfo,CUpgradeStockInfo> m_arrayUpgrade;
};

/**********************************************************************/
#endif
/**********************************************************************/
