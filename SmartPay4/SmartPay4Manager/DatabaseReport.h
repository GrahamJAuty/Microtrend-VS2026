#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
/**********************************************************************/
#include "DbExportEditor.h"
/**********************************************************************/

class CDatabaseReport
{
public:
	CDatabaseReport ( const char* szLabel, CWnd* pParent );
	bool Show( bool bEditReqd = TRUE );
	bool Show( CDatabaseReportInfo& info );

private:
	int Create ( const char* szReportFile );
	void ClearTotals ( int nIndex );
	void AddColumnText();
	bool SaveLine( CSQLRowAccountFull& AccountRow);
	void SaveSortOnTotal ( const char* szSortOnText );
	void SaveGroupTotal ( int nGroupNo );
	void SaveReportTotal();
	void SaveTotals ( int nIndex, const char* szText );

private:
	CWnd* m_pParent;
	CDbReportHandler m_DbReporter;

	CReportFileWithEndText m_Report;
	CString m_strReportLabel;
	CString m_strReportTitle;

	int m_nCount[nREPORT_MAX];

public:
	CString m_strSaveAsFilename;
};

/**********************************************************************/
