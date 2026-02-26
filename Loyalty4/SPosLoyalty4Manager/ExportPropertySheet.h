#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "DbExportHandler.h"
#include "ExportPropertyPage1.h"
#include "ExportPropertyPage2.h"
#include "ExportPropertyPage3.h"
#include "ExportPropertyPage4.h"
//*******************************************************************

/////////////////////////////////////////////////////////////////////////////
// CExportPropertySheet

class CExportPropertySheet : public CSSPropertySheet
{
// Construction
public:
	CExportPropertySheet(CDbExportHandler* pDbExportHandler, bool bLockBasicFields, CWnd* pParentWnd, UINT iSelectPage);
	~CExportPropertySheet();

// Attributes
public:
	CExportPropertyPage1 m_Page1;
	CExportPropertyPage2 m_Page2;
	CExportPropertyPage3 m_Page3;
	CExportPropertyPage4 m_Page4;

// Operations
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CDbExportHandler* m_pDbExportHandler;
	bool m_bLockBasicFields;
};
