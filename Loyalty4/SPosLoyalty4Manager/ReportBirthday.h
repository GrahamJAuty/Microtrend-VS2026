#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPOSLoyalty4Shared\Consolidation.h"
#include "..\SPOSLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "BirthdaysReportDlg.h"
#include "DbReportHandler.h"
#include "recsort.h"
#include "ReportInfoBirthday.h"
//*******************************************************************

class CBirthdayReport
{
public:
	CBirthdayReport ( CWnd* pParent );
	bool Show();

private:
	int Create ( const char* szReportFile );
	void AddColumnText ( CReportFile* pReport );
	bool SaveLine( CArray<int,int>& arrayHeader, CSQLRowAccountFull& RowAccount);
	void AddToSorter ( CSQLRowAccountFull& RowAccount, bool bExpiry );
	void ShowMonthTotal (CArray<int, int>& arrayHeader, int nCount );
	bool SaveBanner ( int nMonth );

private:
	CWnd* m_pParent;
	CReportInfoBirthday m_ReportInfo;
	CBirthdaysReportDlg m_dlgBirthday;
	CDbReportHandler m_DbReporter;

	CReportFile m_Report;
	CReportConsolidationArray<CBirthdayReportItem> m_ConsolArray;
};

/**********************************************************************/
