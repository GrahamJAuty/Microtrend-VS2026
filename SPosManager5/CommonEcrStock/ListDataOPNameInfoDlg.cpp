/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefEditOPNameDlg.h"
#include "DefEditOPNameOverrideDlg.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ListDataOPNameInfoDlg.h"
/**********************************************************************/

CSingleListContentHelpersOPNameInfo::CSingleListContentHelpersOPNameInfo( int nConLevel ) 
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

int CSingleListContentHelpersOPNameInfo::GetArraySize()
{
	return DataManagerNonDb.OPNameSystem.GetOPNameCount();
}

/**********************************************************************/

bool CSingleListContentHelpersOPNameInfo::PrepareLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		for ( int nList = 0; nList < m_arrayConLevels.GetSize(); nList++ )
		{
			COPNameMiniInfo infoMini;

			switch ( m_arrayConLevels.GetAt(nList ) )
			{
			case NODE_LOCATION:	
				DataManagerNonDb.OPNameLocation.GetOPName( nIndex, infoMini );	
				break;

			case NODE_DATABASE:	
				DataManagerNonDb.OPNameDatabase.GetOPName( nIndex, infoMini );
				break;

			case NODE_SYSTEM:	
				DataManagerNonDb.OPNameSystem.GetOPName( nIndex, infoMini );	
				break;
			}

			if ( ( FALSE == infoMini.m_bDefer) || ( nList >= m_arrayConLevels.GetSize() - 1 ) )
			{
				CString strPrinterNo = "";
				strPrinterNo.Format( "%2.2d", infoMini.m_nPrinterNo );

				COPNameInfo OPNameInfo;
				OPNameInfo.SetNameSystem( infoMini.m_strName );

				CCSV csv( '\t' );
				csv.Add( strPrinterNo );

				if ( NODE_SYSTEM != m_nConLevel )
				{
					switch( m_arrayConLevels.GetAt(nList) )
					{
					case NODE_LOCATION:	
						csv.Add( "LOC" );
						break;

					case NODE_DATABASE:	
						csv.Add( "DB" );
						break;

					case NODE_SYSTEM:	
						csv.Add( "SYS" );
						break;

					default:			
						csv.Add( "??" );	
						break;
					}
				}

				csv.Add( infoMini.m_strName );
				strLine = csv.GetLine();

				bResult = TRUE;

				break;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersOPNameInfo::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( NODE_SYSTEM == m_nConLevel )
	{
		list.InsertColumn ( 0, "Printer No", LVCFMT_LEFT, 100 );
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 380 );
	}
	else
	{
		list.InsertColumn ( 0, "Printer No", LVCFMT_LEFT, 100 );
		list.InsertColumn ( 1, "Level", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 320 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataOPNameInfoDlg::CListDataOPNameInfoDlg( int nConLevel, int nEntityIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nConLevel = nConLevel;
	m_nEntityIdx = nEntityIdx;
	m_pContentHelpers = new CSingleListContentHelpersOPNameInfo( m_nConLevel );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataOPNameInfoDlg::~CListDataOPNameInfoDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataOPNameInfoDlg::SetTitle()
{
	CString strTitle = "Configure Order Printer Names ";

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

void CListDataOPNameInfoDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Reset" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataOPNameInfoDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataOPNameInfoDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataOPNameInfoDlg::HandleButton1()
{
	int nSel = GetSafeSelection();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.OPNameSystem.GetOPNameCount() ) )
	{
		if ( NODE_SYSTEM == m_nConLevel )
		{
			CDefEditOPNameDlg dlgOPName(nSel, m_WarnCancelAfterChange, this);
			dlgOPName.SetListDataDlg(this);
			dlgOPName.DoModal();
		}
		else
		{
			CDefEditOPNameOverrideDlg dlgOPName(nSel, m_nConLevel, m_WarnCancelAfterChange, this);
			dlgOPName.SetListDataDlg(this);
			dlgOPName.DoModal();
		}
	}
}

/**********************************************************************/

void CListDataOPNameInfoDlg::HandleButton3()
{
	switch( m_nConLevel )
	{
	case NODE_LOCATION:	
		if ( Prompter.YesNo( "Reset location printer names to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPNameLocation.ResetOPNameList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_DATABASE:	
		if ( Prompter.YesNo( "Reset database printer names to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPNameDatabase.ResetOPNameList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_SYSTEM:	
		if ( Prompter.YesNo( "Reset system printer names to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPNameSystem.ResetOPNameList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;
	}
}

/**********************************************************************/
