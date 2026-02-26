#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpersCustomer.h"
#include "DataAccessHelpersEcrData.h"
#include "DataAccessHelpersEcrTexts.h"
#include "DataAccessHelpersKeyboard.h"
#include "DataAccessHelpersLoyalty.h"
#include "DataAccessHelpersMixMatch.h"
#include "DataAccessHelpersPlu.h"
#include "DataAccessHelpersReports.h"
#include "DataAccessHelpersReportSets.h"
#include "DataAccessHelpersServer.h"
#include "DataAccessHelpersSetup.h"
#include "..\SPosStockManager5\DataAccessHelpersStockData.h"
#include "..\SPosStockManager5\DataAccessHelpersStockTexts.h"
#include "DataAccessHelpersTask.h"
/**********************************************************************/

class CDataAccessHelpersSet
{
public:
	CDataAccessHelpersSet();
	void SetParentWnd( CWnd* pParentWnd );
	CWnd* GetParentWnd(){ return m_Plu.GetParentWnd(); }
	
public:
	CDataAccessHelpersCustomer m_Customer;
	CDataAccessHelpersEcrData m_EcrData;
	CDataAccessHelpersEcrTexts m_EcrText;
	CDataAccessHelpersKeyboard m_Keyboard;
	CDataAccessHelpersMixMatch m_MixMatch;
	CDataAccessHelpersPlu m_Plu;
	CDataAccessHelpersReports m_Reports;
	CDataAccessHelpersReportSets m_ReportSet;
	CDataAccessHelpersServer m_Server;
	CDataAccessHelpersSetup m_Setup;
	CDataAccessHelpersLoyalty m_Loyalty;
	CDataAccessHelpersTask m_Task;

#ifdef STOCKMAN_SYSTEM
	CDataAccessHelpersStockData m_StockData;
	CDataAccessHelpersStockTexts m_StockText;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
