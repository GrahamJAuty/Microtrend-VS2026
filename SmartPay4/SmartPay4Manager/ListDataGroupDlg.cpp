/**********************************************************************/
#include "GroupNoDlg.h"
#include "GroupPropertySheet.h"
#include "SQLTranSaveGroup.h"
/**********************************************************************/
#include "..\SmartPay4Shared\Prompter.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
/**********************************************************************/
#include "ListDataGroupDlg.h"
/**********************************************************************/

CSingleListContentHelpersGroup::CSingleListContentHelpersGroup(CSQLRowSetGroupFull& RowSet, CUserTypeText& userType )
: CSingleListContentHelpers(), m_RowSet(RowSet), m_userType( userType )
{
}

/**********************************************************************/

int CSingleListContentHelpersGroup::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersGroup::PrepareLine( int nIndex, CString& strLine )
{
	CSQLRowGroupFull RowGroup;
	m_RowSet.GetRow(nIndex, RowGroup);

	strLine.Format( "%d\t%s\t%s\t%s",
		RowGroup.GetGroupNo(),
		(const char*) RowGroup.GetGroupName(),
		(const char*) RowGroup.GetRefreshTypeText(),
		(const char*) m_userType.GetText(RowGroup.GetUserType() ) );

	if ( System.GetAllowGroupSetsFlag() )
	{
		CString strExtra;
		strExtra.Format( "\t%d", RowGroup.GetSetNo() );
		strLine += strExtra;
	}
	
	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersGroup::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_RowSet.GetSize() ) )
	{
		m_RowSet.RemoveRow(nIndex);
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
	
	if ( System.GetAllowGroupSetsFlag() )
	{
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 220 );
		list.InsertColumn ( 2, "Refresh", LVCFMT_LEFT, 80 );
		list.InsertColumn ( 3, "Type", LVCFMT_LEFT, 80 );
		list.InsertColumn ( 4, "Set", LVCFMT_LEFT, 40 );
	}
	else
	{
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 260 );
		list.InsertColumn ( 2, "Refresh", LVCFMT_LEFT, 80 );
		list.InsertColumn ( 3, "Type", LVCFMT_LEFT, 80 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataGroupDlg::CListDataGroupDlg(CWnd* pParent) : CListDataDlg(pParent)
{
	m_RowSet.LoadDatabaseGroups();

	m_userType.Open(Filenames.GetUserTypeFilename());
	
	m_pContentHelpers = new CSingleListContentHelpersGroup(m_RowSet, m_userType);
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
	
	if (m_RowSet.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton1()
{
	CString strTitle = "Add Group Database Record";

	int nSize = m_RowSet.GetSize();

	if (m_nNextDefaultGroupNo == 0)
	{
		if ( nSize > 0)
		{
			CSQLRowGroupFull RowGroup;
			m_RowSet.GetRow(nSize - 1, RowGroup);
			m_nNextDefaultGroupNo = RowGroup.GetGroupNo();
		}
	}
	else					
	{
		int nPos = 0;
		m_RowSet.FindGroup(m_nNextDefaultGroupNo, nPos);
		
		if (nPos < m_RowSet.GetSize())
		{
			CSQLRowGroupFull RowGroup;
			m_RowSet.GetRow(nPos, RowGroup);
			m_nNextDefaultGroupNo = RowGroup.GetGroupNo();
		}
	}

	int nGroupNo = m_nNextDefaultGroupNo;
	if (nGroupNo < Group::GroupNo.Max)
	{
		nGroupNo++;
	}

	CGroupNoDlg dlg( nGroupNo );

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	
	nGroupNo = dlg.m_nGroupNo;
	
	if (nGroupNo > 0)
	{
		int nPos = 0;
		if ( m_RowSet.FindGroup( nGroupNo, nPos ) == TRUE )
		{
			CString strMsg;
			strMsg.Format("Database record for Group %d already exists", nGroupNo);
			Prompter.Error(strMsg, strTitle);
			return;
		}

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nGroupNo);
		RowGroup.SetNewRecord();
		m_RowSet.InsertRow(nPos, RowGroup);
		
		CGroupPropertySheet dlgEdit(RowGroup, this);

		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSet.UpdateRow(nPos, RowGroup);
			m_listData.SetItemCountEx(GetArraySize());
			SelectLine(nPos);
			m_nNextDefaultGroupNo = nGroupNo;
			m_WarnCancelAfterChange.SetFlag();
		}
		else
		{
			m_RowSet.RemoveRow(nPos);
			SelectLine(nPos);
		}
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( ( nIndex >= 0 ) && ( nIndex < m_RowSet.GetSize() ) )
	{
		CSQLRowGroupFull RowGroup;
		m_RowSet.GetRow(nIndex, RowGroup);

		CGroupPropertySheet dlgEdit(RowGroup);
		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSet.UpdateRow(nIndex, RowGroup);
			m_WarnCancelAfterChange.SetFlag();
		}
		
		SelectLine( nIndex );
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

bool CListDataGroupDlg::HandleOK()
{
	CSQLRowGroupFull RowGroup;
	for (int i = 0; i < m_RowSet.GetSize(); i++)
	{
		m_RowSet.GetRow(i, RowGroup);

		if ((RowGroup.GetRefreshType() == nREFRESHTYPE_PERIOD) && (RowGroup.HavePeriodRefresh() == FALSE))
		{
			CString strMsg = "";
			strMsg.Format("Period Refresh values for Group %d as specified in Daily Spend Limits are all set to 0!", RowGroup.GetGroupNo());
			Prompter.Error(strMsg, "Invalid Entry");
			return FALSE;
		}
	}

	CSQLTranSaveGroup Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_RowSet);

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
	if ( CListDataDlg::HandleCancel() == TRUE )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

