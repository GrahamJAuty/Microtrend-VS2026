#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//**********************************************************************
#include "SetupCsvImportPropertyPage1.h"
#include "SetupCsvImportPropertyPage2.h"
#include "SetupCsvImportPropertyPage3.h"
#include "SetupCsvImportPropertyPage4.h"
//**********************************************************************

class CSetupCsvImportPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CSetupCsvImportPropertySheet( CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSetupCsvImportPropertySheet();
	void Save();

	CSetupCsvImportPropertyPage1 m_Page1;
	CSetupCsvImportPropertyPage2 m_Page2;
	CSetupCsvImportPropertyPage3 m_Page3;
	CSetupCsvImportPropertyPage4 m_Page4;

protected:
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	CCsvImportData m_data;
};

//**********************************************************************
