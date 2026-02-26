#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluDeleteSingleDlg : public CDialog
{
public:
	CPluDeleteSingleDlg( CPluCSVRecord& PluRecord, CWnd* pParent = NULL); 

	//{{AFX_DATA(CPluDeleteSingleDlg)
	enum { IDD = IDD_PLU_DELETE_SINGLE };
	CStatic	m_staticPlu;
	CStatic	m_staticDescription;
	CStatic	m_staticDept;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeleteSingleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluDeleteSingleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluCSVRecord& m_PluRecord;
};


//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
