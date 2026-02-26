#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersDelUnit : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersDelUnit();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataDelUnitDlg : public CListDataDlg
{
public:
	CListDataDelUnitDlg( const char* szText, int nDUItems, double dDUItemSize, CWnd* pParent = NULL); 
	~CListDataDelUnitDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	CString m_strInitialText;
	int m_nInitialDUItems;
	double m_dInitialDUItemSize;
};

/**********************************************************************/
#endif
/**********************************************************************/

