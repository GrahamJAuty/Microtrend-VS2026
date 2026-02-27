#pragma once
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
/**********************************************************************/
#include "ExportPropertyPage1.h"
#include "ExportPropertyPage2.h"
#include "ExportPropertyPage3.h"
#include "ExportPropertyPage4.h"
/**********************************************************************/

class CExportPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CExportPropertySheet(CDbExportHandler* pDbExportHandler, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CExportPropertySheet() {}

public:
	CExportPropertyPage1 m_Page1;
	CExportPropertyPage2 m_Page2;
	CExportPropertyPage3 m_Page3;
	CExportPropertyPage4 m_Page4;

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CDbExportHandler* m_pDbExportHandler;
};

/**********************************************************************/
