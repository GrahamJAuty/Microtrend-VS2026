#pragma once
//$$******************************************************************
#include "TerminalListHandler.h"
//$$******************************************************************

class CReportLocationControls
{
public:
	CReportLocationControls(CSSTabbedComboBox& comboDatabase, CSSTabbedComboBox& comboFolderSet, CButton& checkTerminals, CComboBox& comboTerminals, CTerminalListHandler* pTerminalListHandler)
		: m_comboDatabase(comboDatabase)
		, m_comboFolderSet(comboFolderSet)
		, m_checkTerminals(checkTerminals)
		, m_comboTerminals(comboTerminals)
	{
		m_pTerminalListHandler = pTerminalListHandler;	
		m_bAllowAnyDb = TRUE;
	}

public:
	void SetTerminalListHandler(CTerminalListHandler* pTerminalListHandler) { m_pTerminalListHandler = pTerminalListHandler; }	
	void Init(int nDbNo, int nFolderSet, int nTerminalSelectType, bool bAllowAnyDb);



	void AddTerminals();
	void SetTerminalOptions();
	void DeleteSelectedTerminalList();
	void SelectDatabase();
	void SelectFolderSet();
	void ToggleTerminals();	
	void GetSettings(int& nDbNo, int& nFolderSet, bool& bWantTerminals, CString& strTerminalList);

private:
	CSSTabbedComboBox& m_comboDatabase;
	CSSTabbedComboBox& m_comboFolderSet;
	CButton& m_checkTerminals;
	CComboBox& m_comboTerminals;
	CTerminalListHandler* m_pTerminalListHandler;
	bool m_bAllowAnyDb;
};

//$$******************************************************************