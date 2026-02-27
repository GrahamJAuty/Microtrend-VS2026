//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "Standardcombos.h"
//$$******************************************************************
#include "ReportLocationControls.h"
//$$******************************************************************

void CReportLocationControls::Init(int nFolderSet, int nTerminalSelectType )
{
	if ((nFolderSet < 0) || (nFolderSet > EPOSREQPATH_COUNT))
	{
		nFolderSet = 0;
	}
	if ((nTerminalSelectType < 0) || (nTerminalSelectType > 1) || (0 == nFolderSet))
	{
		nTerminalSelectType = 0;
	}

	m_checkTerminals.SetCheck(1 == nTerminalSelectType);
	CStandardCombos::FillFolderSetCombo(m_comboFolderSet, nFolderSet);

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

void CReportLocationControls::GetSettings(int& nFolderSet, bool& bWantTerminals, CString & strTerminalList)
{
	nFolderSet = m_comboFolderSet.GetCurSel();
	bWantTerminals = (m_checkTerminals.GetCheck() != 0);
	strTerminalList = "";

	if ((nFolderSet < 0) || (nFolderSet > EPOSREQPATH_COUNT) )
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
