/**********************************************************************/
#include "GroupComboHelpers.h"
#include "GroupNoDlg.h"
#include "GroupPropertySheet.h"
#include "SQLTranSaveGroup.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLHelpers.h"
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "ListDataGroupDlg.h"
/**********************************************************************/

CSingleListContentHelpersGroup::CSingleListContentHelpersGroup(CSQLRowSetGroup& RowSetGroup, CSQLRowSetGroupTopupBonus& RowSetGroupTopupBonus)
	: CSingleListContentHelpers(), m_RowSetGroup(RowSetGroup), m_RowSetGroupTopupBonus(RowSetGroupTopupBonus)
{
}

/**********************************************************************/

int CSingleListContentHelpersGroup::GetArraySize()
{
	return m_RowSetGroup.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersGroup::PrepareLine( int nIndex, CString& strLine )
{
	CSQLRowGroup RowGroup;
	m_RowSetGroup.GetRow(nIndex, RowGroup);

	strLine.Format("%d\t%s",
		RowGroup.GetGroupNo(),
		(const char*) RowGroup.GetGroupName());

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersGroup::DeleteDatabaseEntry( int nIndex )
{
	if ((nIndex >= 0) && (nIndex < m_RowSetGroup.GetSize()))
	{
		CSQLRowGroup RowGroup;
		m_RowSetGroup.GetRow(nIndex, RowGroup);
		m_RowSetGroup.RemoveRow(nIndex);
		m_RowSetGroupTopupBonus.RemoveGroupTopups(RowGroup.GetGroupNo());
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersGroup::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersGroup::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 420 );		
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataGroupDlg::CListDataGroupDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_RowSetGroup.LoadDatabaseGroups();
	m_RowSetGroupTopupBonus.LoadDatabaseGroups();

	m_pContentHelpers = new CSingleListContentHelpersGroup(m_RowSetGroup,m_RowSetGroupTopupBonus);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	m_nNextDefaultGroupNo = 0;

	m_pDisplayHelpers->SetTopSingleText("group");
	m_pDisplayHelpers->SetTopPluralText("groups");
	m_pDisplayHelpers->SetBottomSingleText("group");
	m_pDisplayHelpers->SetBottomPluralText("groups");
}

/**********************************************************************/

CListDataGroupDlg::~CListDataGroupDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataGroupDlg::SetTitle()
{
	SetWindowText ( "Setup Group Details" );
}

/**********************************************************************/

void CListDataGroupDlg::SetButtonTexts()
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

void CListDataGroupDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataGroupDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_RowSetGroup.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton1()
{
	CString strTitle = "Add Group Database Record";

	int nSize = m_RowSetGroup.GetSize();

	if (m_nNextDefaultGroupNo == 0)
	{
		if (nSize > 0)
		{
			CSQLRowGroup RowGroup;
			m_RowSetGroup.GetRow(nSize - 1, RowGroup);
			m_nNextDefaultGroupNo = RowGroup.GetGroupNo();
		}
	}
	else
	{
		int nPos = 0;
		m_RowSetGroup.FindGroup(m_nNextDefaultGroupNo, nPos);

		if (nPos < m_RowSetGroup.GetSize())
		{
			CSQLRowGroup RowGroup;
			m_RowSetGroup.GetRow(nPos, RowGroup);
			m_nNextDefaultGroupNo = RowGroup.GetGroupNo();
		}
	}

	int nGroupNo = m_nNextDefaultGroupNo;
	if (nGroupNo < Group::GroupNo.Max)
	{
		nGroupNo++;
	}

	CGroupNoDlg dlg(FALSE, nGroupNo);

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	nGroupNo = dlg.m_nGroupNo;

	if (nGroupNo > 0)
	{
		int nPos = 0;
		if (m_RowSetGroup.FindGroup(nGroupNo, nPos) == TRUE)
		{
			CString strMsg;
			strMsg.Format("Database record for Group %d already exists", nGroupNo);
			Prompter.Error(strMsg, strTitle);
			return;
		}

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);
		m_RowSetGroup.InsertRow(nPos, RowGroup);
		m_RowSetGroupTopupBonus.RemoveGroupTopups(nGroupNo);

		CReportConsolidationArray<CSQLRowGroupTopupBonus> arrayGroupTopupBonus;

		CGroupPropertySheet dlgEdit(RowGroup, arrayGroupTopupBonus, this);

		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSetGroup.UpdateRow(nPos, RowGroup);
			m_RowSetGroupTopupBonus.SetGroupTopups(nGroupNo, arrayGroupTopupBonus);
			m_listData.SetItemCountEx(GetArraySize());
			SelectLine(nPos);
			m_nNextDefaultGroupNo = nGroupNo;
			m_WarnCancelAfterChange.SetFlag();
		}
		else
		{
			m_RowSetGroup.RemoveRow(nPos);
			m_RowSetGroupTopupBonus.RemoveGroupTopups(nGroupNo);	
			SelectLine(nPos);
		}
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSetGroup.GetSize()))
	{
		CSQLRowGroup RowGroup;
		m_RowSetGroup.GetRow(nIndex, RowGroup);

		CReportConsolidationArray<CSQLRowGroupTopupBonus> arrayGroupTopupBonus;
		m_RowSetGroupTopupBonus.GetGroupTopups(RowGroup.GetGroupNo(), arrayGroupTopupBonus);

		CGroupPropertySheet dlgEdit(RowGroup, arrayGroupTopupBonus, this);
		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSetGroup.UpdateRow(nIndex, RowGroup);
			m_RowSetGroupTopupBonus.SetGroupTopups(RowGroup.GetGroupNo(), arrayGroupTopupBonus);
			m_WarnCancelAfterChange.SetFlag();
		}

		SelectLine(nIndex);
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton3()
{
	m_pDisplayHelpers->HandleDelete(this);
	m_WarnCancelAfterChange.SetFlag(m_pContentHelpers->GetDeletionEffectFlag());
}

/**********************************************************************/

bool CListDataGroupDlg::HandleOK()
{
	CSQLTranSaveGroup Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_RowSetGroup,m_RowSetGroupTopupBonus);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		break;

	default:
		Tran.ShowErrorMessage("Save Groups");
		break;
	}

	return TRUE;
}

/**********************************************************************/

bool CListDataGroupDlg::HandleCancel()
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

