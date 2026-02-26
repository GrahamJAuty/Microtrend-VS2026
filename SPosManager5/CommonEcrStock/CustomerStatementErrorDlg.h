#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
//include "MySSListCtrl.h"
/**********************************************************************/
#define CUSTOMER_STATEMENT_DISPLAY 0
#define CUSTOMER_STATEMENT_PRINT 1
#define CUSTOMER_STATEMENT_EMAIL 2
/**********************************************************************/

class CCustomerStatementErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomerStatementErrorDlg)

public:
	CCustomerStatementErrorDlg( int nType, int Attempted, CArray<CCustomerStatementErrorInfo,CCustomerStatementErrorInfo>& arrayErrors, CWnd* pParent = NULL ); 
	virtual ~CCustomerStatementErrorDlg();

	enum { IDD = IDD_CUSTOMER_STATEMENT_ERROR };
	CSSListCtrl m_listErrors;
	CStatic m_staticText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CArray<CCustomerStatementErrorInfo,CCustomerStatementErrorInfo>& m_arrayErrors;
	int m_nType;
	int m_nAttempted;
};

/**********************************************************************/
#endif
/**********************************************************************/
