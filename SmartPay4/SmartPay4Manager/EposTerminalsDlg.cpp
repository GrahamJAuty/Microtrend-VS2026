/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRecordSetEposTerminal.h"
/**********************************************************************/
#include "SQLTranSaveEposTerminal.h"
/**********************************************************************/
#include "EposTerminalsDlg.h"
/**********************************************************************/
#define COLOUR_EPOS				0x00		// black
#define COLOUR_KIOSK			0x01		// dark blue
//#define COLOUR_UNISSUED		0x02		// dark green
//#define COLOUR_EXPIRED		0x09		// light blue
#define COLOUR_DISABLED			0x0c		// light red
/**********************************************************************/

CEposTerminalsDlg::CEposTerminalsDlg(CWnd* pParent) : CSSAutoShutdownDialog(AutoShutdownHelper, CEposTerminalsDlg::IDD, pParent)
{
	m_bModified = FALSE;
}

/**********************************************************************/

CEposTerminalsDlg::~CEposTerminalsDlg()
{
}

/**********************************************************************/

void CEposTerminalsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposTerminalsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, OnButtonDisable)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposTerminalsDlg::CSSAutoShutdownPostInitDialog()
{
	m_list.SubclassDlgItem(IDC_LIST1, this);
	m_list.InsertColumn(0, "TNo", LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, "Type", LVCFMT_LEFT, 60);
	m_list.InsertColumn(2, "Name", LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, "Last Used", LVCFMT_LEFT, 110);
	m_list.InsertColumn(4, "Status", LVCFMT_RIGHT, 70);
	m_list.EnableColour();

	m_RowSetEP.LoadDatabaseEposTerminals();

	DisplayTerminals();
	m_bModified = FALSE;

	return TRUE;
}

/**********************************************************************/

void CEposTerminalsDlg::DisplayTerminals()
{
	m_list.DeleteAllItems();

	CString strBuf = "";

	for (int nIndex = 0; nIndex < m_RowSetEP.GetSize(); nIndex++)
	{
		CSQLRowEposTerminal RowEP;
		m_RowSetEP.GetRow(nIndex, RowEP);

		CCSV csv('\t');
		csv.Add(RowEP.GetTerminalNo());
		csv.Add(RowEP.GetTerminalText());
		csv.Add(RowEP.GetTerminalName());

		CString strDateLastUsed = "";
		if (RowEP.IsDateSet() == TRUE)
		{
			strDateLastUsed.Format("%s @ %s",
				(const char*)RowEP.GetLastUsedDate(),
				(const char*)RowEP.GetLastUsedTime());
		}
		else
		{
			strDateLastUsed = "";
		}

		csv.Add(strDateLastUsed);

		int nColour = COLOUR_EPOS;
		CString strStatus = "";

		if (RowEP.GetTerminalType() == nRECORDTYPE_KIOSK)
		{
			nColour = COLOUR_KIOSK;

			if (RowEP.GetDisableFlag() == TRUE)
			{
				nColour = COLOUR_DISABLED;
				strStatus = "Disabled";
			}
			else
			{
				strStatus = "Enabled";
			}

			csv.Add(strStatus);
		}

		m_list.AddString(csv.GetLine(), nColour);
	}

	m_list.SetCurSel(0);
	UpdateData(FALSE);
}

/**********************************************************************/

void CEposTerminalsDlg::OnButtonDelete()
{
	int nIndex = m_list.GetCurSel();

	if ((nIndex < 0) || (nIndex >= m_RowSetEP.GetSize()))
	{
		return;
	}

	CSQLRowEposTerminal RowEP;
	m_RowSetEP.GetRow(nIndex, RowEP);

	CString strMsg = "";
	strMsg.Format("Are you sure you wish to delete Terminal %d (%s) for the SmartPay Server List",
		RowEP.GetTerminalNo(),
		(const char*) RowEP.GetTerminalText());

	if (Prompter.YesNo(strMsg, "Please confirm") == IDYES)
	{
		m_arrayDeletedEP.Add(RowEP.GetTerminalNo());
		m_RowSetEP.RemoveRow(nIndex);
		DisplayTerminals();
		m_list.SetCurSel(nIndex);
		m_bModified = TRUE;
	}
}

/**********************************************************************/

void CEposTerminalsDlg::OnButtonDisable()
{
	int nIndex = m_list.GetCurSel();

	if ((nIndex < 0) || (nIndex >= m_RowSetEP.GetSize()))
	{
		return;
	}

	CSQLRowEposTerminal RowEP;
	m_RowSetEP.GetRow(nIndex, RowEP);

	if ( RowEP.GetTerminalType() == nRECORDTYPE_KIOSK )
	{
		RowEP.SetDisableFlag(!RowEP.GetDisableFlag());
		m_RowSetEP.UpdateRow(nIndex, RowEP);
		m_bModified = TRUE;
		DisplayTerminals();
	}
	else
	{
		Prompter.Error("This option only applies to Kiosk Terminals");
	}


	m_list.SetCurSel(nIndex);
}

/**********************************************************************/

void CEposTerminalsDlg::OnOK()
{
	if ( TRUE == m_bModified )
	{
		if (Prompter.YesNo("Save changes to SmartPay Server Terminal List", "Please confirm") == IDNO)
		{
			EndDialog(IDCANCEL);
			return;
		}
	}

	CSQLTranSaveEposTerminal Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_RowSetEP, m_arrayDeletedEP);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		break;

	default:
		Tran.ShowErrorMessage("Save Epos Terminals");
		break;
	}

	EndDialog(IDOK);
}

/**********************************************************************/

void CEposTerminalsDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

/**********************************************************************/

void CEposTerminalsDlg::CSSAutoShutdownLogOff()
{
	OnCancel();
}

//*******************************************************************
