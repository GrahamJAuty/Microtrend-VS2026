#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CSPOSKeyboardSQLExporter  
{
public:
	CSPOSKeyboardSQLExporter( int nEPOSModel );
	
public:
	bool GotFile() { return m_bGotFile; }
	int GetListHeaderCount() { return m_nTotalListHeader; }
	int GetListItemCount() { return m_nTotalListItem; }
	int GetScreenModeCount() { return m_nTotalScreenMode; }
	int GetSysValuesCount() { return m_nTotalSysValues; }
	int GetMacrosCount() { return m_nTotalMacros; }

private:
	void Reset();

public:
	void ExportFileForDownload( const char* szFilename, bool bClearMacros );
	void ExportFileForSave( const char* szFilename, bool bIncludeMacros );

private:
	void ExportFile( const char* szFilename, bool bForDownload, bool bIncludeMacros, bool bClearMacros );
	void ExportListHeaders( CSSFile& fileSQL, bool bForDownload);
	void ExportListItems( CSSFile& fileSQL, bool bForDownload );
	void ExportScreenModes( CSSFile& fileSQL );
	void ExportSysValues( CSSFile& fileSQL );
	void ExportMacros( CSSFile& fileSQL, bool bClear );
		
private:
	bool m_bGotFile;
	int m_nTotalListHeader;
	int m_nTotalListItem;
	int m_nTotalScreenMode;
	int m_nTotalSysValues;
	int m_nTotalMacros;
	int m_nSPOSVersion;

private:
	CReportConsolidationArray<CSortedInt64Item> m_arrayPluList;
};

/**********************************************************************/

