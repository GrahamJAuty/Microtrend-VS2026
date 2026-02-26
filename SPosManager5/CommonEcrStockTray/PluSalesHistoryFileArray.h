#pragma once
/**********************************************************************/
#include "EposSelectArray.h"
#include "ReportConsolidationArray.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CPluSalesHistoryFileArray
{
public:
	CPluSalesHistoryFileArray( CEposSelectArray& SelectArray );

public:
	void SetEposSalesFlag( bool b ){ m_bEposSales = b; }
	void SetEODModeFlag( bool b ){ m_bEODMode = b; }
	void SetGuestToTerminalFlag( bool b ){ m_bGuestToTerminal = b; }
	void SetManualSalesFlag( bool b ){ m_bManualSales = b; }
	void SetImportSalesFlag( bool b ){ m_bImportSales = b; }
	void SetSaleTimeFlag( bool b ){ m_bSaleTime = b; }
	void BuildFileList();

public:
	int GetTotalFileSize(){ return m_nTotalFileSize; }
	int GetFileCount(){ return m_arrayFileInfo.GetSize(); }
	void GetFileInfo( int nIdx, CTermFileListInfo& info );

private:
	int m_nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> m_arrayFileInfo;

private:
	CEposSelectArray& m_SelectArray;
	bool m_bEposSales;
	bool m_bEODMode;
	bool m_bGuestToTerminal;
	bool m_bManualSales;
	bool m_bImportSales;
	bool m_bSaleTime;
};

/**********************************************************************/
