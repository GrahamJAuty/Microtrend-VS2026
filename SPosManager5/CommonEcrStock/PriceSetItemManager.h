#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
#include "..\SPosTray5\PluMonitorPosTray.h"
#include "..\SPosStockManager5\PluMonitorStockman.h"
#include "PriceSetIncludes.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPriceSetItemManager
{
public:
	CPriceSetItemManager();
	void Refresh() { SelectPriceSet ( m_nLocIdx, m_nPriceSet ); }
	void SelectPriceSet ( int nLocIdx, int nSet );
	void SetPluIdx ( int nPluIdx ) { m_nPluIdx = nPluIdx; }
	
	const char* GetFieldFilename() { return m_strFieldFilename; }
	const char* GetLockFilename() { return m_strLockFilename; }
	const char* GetExcludeFilename() { return m_strExcludeFilename; }
	const char* GetIncludeFilename() { return m_strIncludeFilename; }
	const char* GetTempFilename() { return m_strTempFilename; }

	int GetPriceSetNumber() { return m_nPriceSet; }
	const char* GetPriceSetNameAndNumber() { return m_strPriceSetNameAndNumber; }
	
	const char* GetSavedProductLabelsFilename();
	const char* GetSavedShelfEdgeLabelsFilename();
	
#if defined (STOCKMAN_SYSTEM)
	void ReadPriceSetFile( bool bForDownload, CPluMonitorStockman& PluMonitor );
#elif defined (POSTRAY_UTILITY)
	void ReadPriceSetFile( bool bForDownload, CPluMonitorPosTray& PluMonitor );
#else
	void ReadPriceSetFile( bool bForDownload, CPluMonitorEcrman& PluMonitor );
#endif

	void WritePriceSetFile();

	BYTE GetPluStatus ( int nPluIdx );
	void SetPluStatus ( int nPluIdx, BYTE byStatus );

	BYTE GetPluStatus() { return GetPluStatus ( m_nPluIdx ); }
	void SetPluStatus( BYTE byStatus ) { SetPluStatus ( m_nPluIdx, byStatus ); }

	bool CheckField ( const char* szField );
	bool IsPluMaster();
	bool HasLocalFields();

	bool GetPluStatusModifiedFlag();

	void Reset();
	bool IsActive() { return m_bIsActive; }

	void ConvertIncludeExcludeFiles();

	void GetPriceSetExportInfo( CPriceSetExportInfo& info );

public:
	void RememberMasterRecord();
	void RestoreMasterRecord();
	void ReloadMasterRecord( CPluCSVRecord& PluRecord );
	void RememberMasterRecord( int nPluIdx );
	void RestoreMasterRecord( int nPluIdx );
	
private:
	CReportConsolidationArray<CSortedStringItem> m_arrayFields;
	CString m_strPriceSetHeader;
	int m_nLocIdx;
	int m_nPriceSet;
	int m_nPluIdx;
	bool m_bPluStatusModified;
	bool m_bIsActive;
	CString m_strFieldFilename;
	CString m_strLockFilename;
	CString m_strExcludeFilename;
	CString m_strIncludeFilename;
	CString m_strTempFilename;
	CByteArray m_arrayPluStatus;
	CStringArray m_arrayMasterRecord;
	CString m_strPriceSetNameAndNumber;
	CString m_strShelfListFilename;
	CString m_strBarcodeListFilename;
};

/**********************************************************************/
extern CPriceSetItemManager PriceSetItemManager;
/**********************************************************************/
#endif
/**********************************************************************/
