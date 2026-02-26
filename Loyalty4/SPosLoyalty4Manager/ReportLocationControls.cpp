//$$******************************************************************
#include "standardcombos.h"
//$$******************************************************************
#include "ReportLocationControls.h"
//$$******************************************************************

void CReportLocationControls::Init(int nDbNo, int nFolderSet, int nTerminalSelectType, bool bAllowAnyDb )
{
	if (System.GetMaxEcrmanDatabases() == 1)
	{
		bAllowAnyDb = FALSE;
	}

	m_bAllowAnyDb = bAllowAnyDb;
	
	if ((nDbNo < 0) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 0;
	}
	if (( 0 == nDbNo) && ( FALSE == m_bAllowAnyDb))
	{
		nDbNo = 1;
	}
	if ((nFolderSet < 0) || (nFolderSet > EPOSREQPATH_COUNT) || (0 == nDbNo))
	{
		nFolderSet = 0;
	}
	if ((nTerminalSelectType < 0) || (nTerminalSelectType > 1) || (0 == nFolderSet))
	{
		nTerminalSelectType = 0;
	}

	m_checkTerminals.SetCheck(1 == nTerminalSelectType);
	CStandardCombos::FillDatabaseCombo(m_comboDatabase, m_bAllowAnyDb, nDbNo);
	CStandardCombos::FillFolderSetCombo(m_comboFolderSet, nDbNo, nFolderSet);

	AddTerminals();
	SetTerminalOptions();
}


//$$******************************************************************

void CReportLocationControls::AddTerminals()
{
	for (int i = 0; i < m_pTerminalListHandler->TNoListGetSize(); i++)
	{
		m_comboTerminals.AddString(m_pTerminalListHandler->TNoListGetItem(i));
	}
	m_comboTerminals.SetCurSel(0);
}

//$$******************************************************************

void CReportLocationControls::SetTerminalOptions()
{
	bool bEnable = TRUE;
	m_comboDatabase.EnableWindow(bEnable);
	bEnable &= ((m_comboDatabase.GetCurSel() != 0) || (FALSE == m_bAllowAnyDb));
	m_comboFolderSet.EnableWindow(bEnable);
	bEnable &= (m_comboFolderSet.GetCurSel() != 0);
	m_checkTerminals.EnableWindow(bEnable);
	
	if (FALSE == bEnable)
	{
		m_checkTerminals.SetCheck(FALSE);
	}
	
	m_comboTerminals.EnableWindow(bEnable & (m_checkTerminals.GetCheck() != 0));
	m_comboTerminals.SetFocus();
}

//$$******************************************************************

void CReportLocationControls::SelectDatabase()
{
	int nDbNo = m_comboDatabase.GetCurSel();
	if (FALSE == m_bAllowAnyDb)
	{
		nDbNo++;
	}

	CStandardCombos::FillFolderSetCombo(m_comboFolderSet, nDbNo, m_comboFolderSet.GetCurSel());
	SetTerminalOptions();
}

//$$******************************************************************

void CReportLocationControls::SelectFolderSet()
{
	SetTerminalOptions();
}

//$$******************************************************************

void CReportLocationControls::ToggleTerminals()
{
	SetTerminalOptions();
}

//$$******************************************************************

void CReportLocationControls::DeleteSelectedTerminalList()
{
	if (m_checkTerminals.GetCheck() != 0)
	{
		CString strTerminalList = "";
		m_comboTerminals.GetWindowText(strTerminalList);

		if (strTerminalList != "")
		{
			m_pTerminalListHandler->TNoListDeleteItem(strTerminalList);		// delete comment
			m_comboTerminals.ResetContent();								// new list
			AddTerminals();
		}
	}
}

//$$******************************************************************

void CReportLocationControls::GetSettings(int& nDbNo, int& nFolderSet, bool& bWantTerminals, CString & strTerminalList)
{
	nDbNo = m_comboDatabase.GetCurSel();
	nFolderSet = m_comboFolderSet.GetCurSel();
	bWantTerminals = (m_checkTerminals.GetCheck() != 0);
	strTerminalList = "";

	int nMinDb = 0;
	if (FALSE == m_bAllowAnyDb)
	{
		nDbNo++;
		nMinDb = 1;
	}

	if ((nDbNo < nMinDb) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = nMinDb;
	}

	if ((nFolderSet < 0) || (nFolderSet > EPOSREQPATH_COUNT) || (0 == nDbNo))
	{
		nFolderSet = 0;
	}

	if (0 == nFolderSet)
	{
		bWantTerminals = FALSE;
	}

	if (TRUE == bWantTerminals)
	{
		m_comboTerminals.GetWindowText(strTerminalList);
	}
}

//$$******************************************************************
