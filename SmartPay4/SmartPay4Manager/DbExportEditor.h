//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************

class CDbExportEditor  
{
public:
	CDbExportEditor ( CWnd* pParent = NULL );
	
public:
	bool EditExportLabel(const char* szLabel, const char* szTitle = NULL);
	bool EditExportLabelFromReportInfo( CDatabaseReportInfo& info);

private:
	CWnd* m_pParent;
};

//$$******************************************************************