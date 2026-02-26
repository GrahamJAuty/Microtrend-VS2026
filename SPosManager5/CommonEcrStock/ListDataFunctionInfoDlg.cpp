/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefEditFunctionDlg.h"
#include "DefEditFunctionOverrideDlg.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ListDataFunctionInfoDlg.h"
/**********************************************************************/

CSingleListContentHelpersFunctionInfo::CSingleListContentHelpersFunctionInfo( int nConLevel ) 
{
	m_nConLevel = nConLevel;

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		m_arrayConLevels.Add( NODE_DATABASE );
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;

	case NODE_LOCATION:
		m_arrayConLevels.Add( NODE_LOCATION );
		m_arrayConLevels.Add( NODE_DATABASE );
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;

	case NODE_SYSTEM:
	default:
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;
	}
}

/**********************************************************************/

int CSingleListContentHelpersFunctionInfo::GetArraySize()
{
	return DataManagerNonDb.FunctionSystem.GetFunctionCount();
}

/**********************************************************************/

bool CSingleListContentHelpersFunctionInfo::PrepareLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		for ( int nList = 0; nList < m_arrayConLevels.GetSize(); nList++ )
		{
			CFunctionAuthMiniInfo infoMini;

			switch ( m_arrayConLevels.GetAt(nList ) )
			{
			case NODE_LOCATION:	DataManagerNonDb.FunctionLocation.GetFunction( nIndex, infoMini );	break;
			case NODE_DATABASE:	DataManagerNonDb.FunctionDatabase.GetFunction( nIndex, infoMini );	break;
			case NODE_SYSTEM:	DataManagerNonDb.FunctionSystem.GetFunction( nIndex, infoMini );	break;
			}

			if ( ( infoMini.m_nAuthLevel >= 0 ) || ( nList >= m_arrayConLevels.GetSize() - 1 ) )
			{
				CString strFuncNo;
				strFuncNo.Format( "%4.4d", infoMini.m_nFuncNo );

				CCSV csv( '\t' );
				csv.Add( strFuncNo );

				if ( NODE_SYSTEM != m_nConLevel )
				{
					switch( m_arrayConLevels.GetAt(nList) )
					{
					case NODE_LOCATION:	csv.Add( "LOC" );	break;
					case NODE_DATABASE:	csv.Add( "DB" );	break;
					case NODE_SYSTEM:	csv.Add( "SYS" );	break;
					default:			csv.Add( "??" );	break;
					}
				}

				csv.Add( infoMini.m_strName );
				csv.Add( infoMini.m_nAuthLevel );
				csv.Add( infoMini.m_bPassword ? "Y" : "" );
				csv.Add( infoMini.m_bAudit ? "Y" : "" );
				csv.Add( infoMini.m_bLog ? "Y" : "" );
				strLine = csv.GetLine();

				bResult = TRUE;

				break;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersFunctionInfo::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( NODE_SYSTEM == m_nConLevel )
	{
		list.InsertColumn ( 0, "F.No", LVCFMT_LEFT, 50 );
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 230 );
		list.InsertColumn ( 2, "Auth", LVCFMT_LEFT, 50);
		list.InsertColumn ( 3, "Pass", LVCFMT_LEFT, 50 );
		list.InsertColumn ( 4, "Audit", LVCFMT_LEFT, 50 );
		list.InsertColumn ( 5, "Log", LVCFMT_LEFT, 50 );
	}
	else
	{
		list.InsertColumn ( 0, "F.No", LVCFMT_LEFT, 50 );
		list.InsertColumn ( 1, "Level", LVCFMT_LEFT, 50 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 220 );
		list.InsertColumn ( 3, "Auth", LVCFMT_LEFT, 40 );
		list.InsertColumn ( 4, "Pass", LVCFMT_LEFT, 40 );
		list.InsertColumn ( 5, "Audit", LVCFMT_LEFT, 40 );
		list.InsertColumn ( 6, "Log", LVCFMT_LEFT, 40 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataFunctionInfoDlg::CListDataFunctionInfoDlg( int nConLevel, int nEntityIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nConLevel = nConLevel;
	m_nEntityIdx = nEntityIdx;
	
	m_pContentHelpers = new CSingleListContentHelpersFunctionInfo( m_nConLevel );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataFunctionInfoDlg::~CListDataFunctionInfoDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataFunctionInfoDlg::SetTitle()
{
	CString strTitle = "Configure Function Settings ";

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		{
			CString strExtra;
			strExtra.Format( "(%s)", dbDatabase.GetName( m_nEntityIdx ) );
			SetWindowText ( strTitle + strExtra );
		}
		break;

	case NODE_LOCATION:
		{
			CString strExtra;
			strExtra.Format( "(%s)", dbLocation.GetName( m_nEntityIdx ) );
			SetWindowText ( strTitle + strExtra );
		}
		break;

	case NODE_SYSTEM:
	default:
		SetWindowText ( strTitle + "(System)" );
		break;
	}
}

/**********************************************************************/

void CListDataFunctionInfoDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Reset" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataFunctionInfoDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataFunctionInfoDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataFunctionInfoDlg::HandleButton1()
{
	int nSel = GetSafeSelection();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.FunctionSystem.GetFunctionCount() ) )
	{
		if ( NODE_SYSTEM == m_nConLevel )
		{
			CDefEditFunctionDlg dlgFunction( nSel, m_WarnCancelAfterChange, this );
			dlgFunction.SetListDataDlg( this );
			dlgFunction.DoModal();
		}
		else
		{
			CDefEditFunctionOverrideDlg dlgFunction( nSel, m_nConLevel, m_WarnCancelAfterChange, this );
			dlgFunction.SetListDataDlg( this );
			dlgFunction.DoModal();
		}
	}
}

/**********************************************************************/

void CListDataFunctionInfoDlg::HandleButton3()
{
	switch( m_nConLevel )
	{
	case NODE_LOCATION:	
		if ( Prompter.YesNo( "Reset location functions to defaults?" ) == IDYES )
		{
			DataManagerNonDb.FunctionLocation.ResetFunctionList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_DATABASE:	
		if ( Prompter.YesNo( "Reset database functions to defaults?" ) == IDYES )
		{
			DataManagerNonDb.FunctionDatabase.ResetFunctionList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_SYSTEM:	
		if ( Prompter.YesNo( "Reset system functions to defaults?" ) == IDYES )
		{
			DataManagerNonDb.FunctionSystem.ResetFunctionList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;
	}
}

/**********************************************************************/
