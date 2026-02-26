#pragma once
/**********************************************************************/

class CSPOSKeyboardSQLImporter  
{
public:
	CSPOSKeyboardSQLImporter();
	void ImportFile( const char* szFilename, bool bImport, bool bWantLayout, bool bWantMacros );

	bool GotFile() { return m_bGotFile; }
	int GetListHeaderCount() { return m_nTotalListHeader; }
	int GetListItemCount() { return m_nTotalListItem; }
	int GetScreenModeCount() { return m_nTotalScreenMode; }
	int GetSysValuesCount() { return m_nTotalSysValues; }
	int GetMacroCount() { return m_nTotalMacros; }

public:
	bool GotMacros(){ return m_MacroBuffer.GetSize() != 0; }
	void MergeMacroBufferKeyboard( bool bNewMacrosOnly );
	void MergeMacroBufferActive();

private:
	void CreateKeyboardMacro( CCascadingMacroCSVRecord& Macro, int nMacroNo, CString& strDetail );

private:
	void Reset();

private:
	void CheckListHeaders( CSSFile& fileSQL );
	void CheckListItems( CSSFile& fileSQL );
	void CheckScreenModes( CSSFile& fileSQL );
	void CheckSysValues( CSSFile& fileSQL );
	void CheckMacros( CSSFile& file );

private:
	void ImportListHeaders( CSSFile& fileSQL );
	void ImportListItems( CSSFile& fileSQL );
	void ImportScreenModes( CSSFile& fileSQL );
	void ImportSysValues( CSSFile& fileSQL );
	void ImportMacros( CSSFile& file );
		
private:
	bool m_bGotFile;
	int m_nTotalListHeader;
	int m_nTotalListItem;
	int m_nTotalScreenMode;
	int m_nTotalSysValues;
	int m_nTotalMacros;

private:
	CReportConsolidationArray<CSortedStringByInt> m_MacroBuffer;
};

/**********************************************************************/

