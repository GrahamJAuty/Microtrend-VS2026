/**********************************************************************/
#include "SQLTranSaveStaffCard.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLHelpers.h"
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
/**********************************************************************/
#include "EditStaffCardDlg.h"
/**********************************************************************/
#include "ListDataStaffCardDlg.h"
/**********************************************************************/

CSingleListContentHelpersStaffCard::CSingleListContentHelpersStaffCard(CSQLRowSetStaffCard& RowSet) 
	: CSingleListContentHelpers(), m_RowSet(RowSet)
{
}

/**********************************************************************/

int CSingleListContentHelpersStaffCard::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersStaffCard::PrepareLine( int nIndex, CString& strLine )
{
	CSQLRowStaffCard RowSC;
	m_RowSet.GetRow(nIndex, RowSC);

	CCSV csvLine('\t');
	AppendNumberOrAny(csvLine, RowSC.GetFolderIndexDbNo());
	AppendNumberOrAny(csvLine, RowSC.GetFolderIndexSetNo());
	csvLine.Add(RowSC.GetServerNo());
	csvLine.Add(RowSC.GetDisplayUserID());
	csvLine.Add(RowSC.GetDisplayAccountName());

	strLine = csvLine.GetLine();

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersStaffCard::AppendNumberOrAny(CCSV& csv, int nNumber)
{
	if (0 == nNumber)
	{
		csv.Add("Any");
	}
	else
	{
		csv.Add(nNumber);
	}
}

/**********************************************************************/

void CSingleListContentHelpersStaffCard::DeleteDatabaseEntry( int nIndex )
{
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		m_RowSet.RemoveRow(nIndex);
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersStaffCard::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersStaffCard::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "DbNo", LVCFMT_LEFT, 60);
	list.InsertColumn(1, "Folder", LVCFMT_LEFT, 60);
	list.InsertColumn(2, "Server", LVCFMT_LEFT, 60);
	list.InsertColumn(3, "Account", LVCFMT_LEFT, 120);
	list.InsertColumn(4, "Name", LVCFMT_LEFT, 180);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataStaffCardDlg::CListDataStaffCardDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_RowSet.LoadDatabaseCards();

	m_pContentHelpers = new CSingleListContentHelpersStaffCard(m_RowSet);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("staff card");
	m_pDisplayHelpers->SetTopPluralText("staff cards");
	m_pDisplayHelpers->SetBottomSingleText("staff card");
	m_pDisplayHelpers->SetBottomPluralText("staff cards");
}

/**********************************************************************/

CListDataStaffCardDlg::~CListDataStaffCardDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataStaffCardDlg::SetTitle()
{
	SetWindowText ( "Link Staff Cards to SPOS Servers" );
}

/**********************************************************************/

void CListDataStaffCardDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataStaffCardDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataStaffCardDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_RowSet.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataStaffCardDlg::HandleButton1()
{
	CSQLRowStaffCard RowSC;

	Server.UpdateEcrmanDbInfo();
	CEditStaffCardDlg dlgEdit(RowSC, FALSE, this);
	if (dlgEdit.DoModal() == IDOK)
	{
		int nPos = m_RowSet.AddOrInsertRow(RowSC);
		m_listData.SetItemCountEx(GetArraySize());
		SelectLine(nPos);
	}
}

/**********************************************************************/

void CListDataStaffCardDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CSQLRowStaffCard RowSC;
		m_RowSet.GetRow(nIndex, RowSC);

		Server.UpdateEcrmanDbInfo();
		CEditStaffCardDlg dlgEdit(RowSC, TRUE, this);
		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSet.UpdateRow(nIndex, RowSC);
			m_WarnCancelAfterChange.SetFlag();
		}

		SelectLine(nIndex);
	}
}

/**********************************************************************/

void CListDataStaffCardDlg::HandleButton3()
{
	m_pDisplayHelpers->HandleDelete(this);
	m_WarnCancelAfterChange.SetFlag(m_pContentHelpers->GetDeletionEffectFlag());
}

/**********************************************************************/

bool CListDataStaffCardDlg::HandleOK()
{
	CSQLTranSaveStaffCard Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_RowSet);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		break;

	default:
		Tran.ShowErrorMessage("Save Staff Cards");
		break;
	}

	return TRUE;
}

/**********************************************************************/

bool CListDataStaffCardDlg::HandleCancel()
{
	if (CListDataDlg::HandleCancel() == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

