#pragma once
//$$******************************************************************
#include "TerminalListHandler.h"
//$$******************************************************************

class CReportLocationControls
{
public:
	CReportLocationControls(CSSTabbedComboBox& comboFolderSet, CButton& checkTerminals, CComboBox& comboTerminals, CTerminalListHandler* pTerminalListHandler)
		: m_comboFolderSet(comboFolderSet)
		, m_checkTerminals(checkTerminals)
		, m_comboTerminals(comboTerminals)
	{
		m_pTerminalListHandler = pTerminalListHandler;	
	}

public:
	void SetTerminalListHandler(CTerminalListHandler* pTerminalListHandler) { m_pTerminalListHandler = pTerminalListHandler; }	
	void Init(int nFolderSet, int nTerminalSelectType);



	void AddTerminals();
	void SetTerminalOptions();
	void DeleteSelectedTerminalList();
	void SelectFolderSet();
	void ToggleTerminals();	
	void GetSettings(int& nFolderSet, bool& bWantTerminals, CString& strTerminalList);

private:
	CSSTabbedComboBox& m_comboFolderSet;
	CButton& m_checkTerminals;
	CComboBox& m_comboTerminals;
	CTerminalListHandler* m_pTerminalListHandler;
};

//$$******************************************************************