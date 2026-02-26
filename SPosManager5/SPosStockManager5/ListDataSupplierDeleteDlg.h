#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSupplierDelete : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSupplierDelete( CWordArray& arraySuppIdx );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CWordArray& m_arraySuppIdx;
};

/**********************************************************************/

class CListDataSupplierDeleteDlg : public CListDataDlg
{
public:
	CListDataSupplierDeleteDlg( CWordArray& arraySuppIdx, CWnd* pParent = NULL); 
	~CListDataSupplierDeleteDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON4; }
	virtual void HandleButton1();	//DELETE
	virtual void HandleButton4();	//CHECK
	virtual void HandleDefaultButton() { HandleButton4(); }

private:
	CWordArray& m_arraySuppIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
