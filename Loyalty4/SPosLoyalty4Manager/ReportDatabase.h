#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//*******************************************************************
#include "DbExportEditor.h"
#include "DbReportHandler.h"
//*******************************************************************

class CDatabaseReport
{
public:
	CDatabaseReport ( const char* szLabel, CWnd* pParent );
	~CDatabaseReport();

public:
	bool Show ( bool bEditReqd = TRUE );

private:
	int Create ( const char* szReportFile );
	void ClearTotals ( int nIndex );
	void AddColumnText();
	bool SaveLine( CArray<int,int>& arrayHeader, CSQLRowAccountFull& RowAccount );
	void SaveTotals (CArray<int, int>& arrayHeader, int nIndex, int nGroupNo = 0 );

private:
	CWnd* m_pParent;
	CDbReportHandler m_DbReporter;

	CReportFile m_Report;
	CString m_strReportLabel;
	CString m_strReportTitle;

	int m_nCount[2];
	int m_nWarnings[2];
};

/**********************************************************************/
