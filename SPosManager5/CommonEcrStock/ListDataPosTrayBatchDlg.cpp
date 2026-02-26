/**********************************************************************/
#include "..\SPosEcrManager5\PosTrayBatchReportStkLevelEcrDlg.h"
/**********************************************************************/
#include "FileRemover.h"
#include "PosTrayBatchExportDiscountDlg.h"
#include "PosTrayBatchExportPaymentDlg.h"
#include "PosTrayBatchExportSalesDlg.h"
#include "PosTrayBatchExportVoidDlg.h"
#include "PosTrayBatchReportEposDlg.h"
#include "PosTrayBatchReportHistoryDlg.h"
#include "PosTrayBatchReportTimeAttendDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataPosTrayBatchDlg.h"
/**********************************************************************/

CSingleListContentHelpersPosTrayBatch::CSingleListContentHelpersPosTrayBatch( CPosTrayFileSet* pFileSet )
{
	m_pPosTrayFileSet = pFileSet;
	m_pArrayBatchInfo = NULL;
}

/**********************************************************************/

int CSingleListContentHelpersPosTrayBatch::GetArraySize()
{
	return m_pArrayBatchInfo-> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPosTrayBatch::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CPosTrayBatchInfo info;
		m_pArrayBatchInfo -> GetAt( nIndex, info );
		strLine = info.m_strBatchName;
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPosTrayBatch::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pArrayBatchInfo -> GetSize() ) )
	{
		CPosTrayBatchInfo info;
		m_pArrayBatchInfo -> GetAt( nIndex, info );

		switch( m_pPosTrayFileSet -> m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_HIST_TASK:
			{
				CFilenameUpdater FnUp( SysFiles::PosTrayBatchHistTemp, 0, info.m_nBatchNo );
				CFileRemover FileRemover( FnUp.GetFilenameToUse() );
			}
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
			{
				CFilenameUpdater FnUp( SysFiles::PosTrayBatchExportSalesTemp, 0, info.m_nBatchNo );
				CFileRemover FileRemover( FnUp.GetFilenameToUse() );
			}
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
			{
				CFilenameUpdater FnUp( SysFiles::PosTrayBatchExportPaymentTemp, 0, info.m_nBatchNo );
				CFileRemover FileRemover( FnUp.GetFilenameToUse() );
			}
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
			{
				CFilenameUpdater FnUp( SysFiles::PosTrayBatchExportVoidTemp, 0, info.m_nBatchNo );
				CFileRemover FileRemover( FnUp.GetFilenameToUse() );
			}
			break;

		case POSTRAY_TASKFILE_STKLEVEL_TASK:
			{
				if ( SysInfo.IsStockSystem() == TRUE )
				{
					CFilenameUpdater FnUp( SysFiles::PosTrayBatchStkLevelStkTemp, 0, info.m_nBatchNo );
					CFileRemover FileRemover( FnUp.GetFilenameToUse() );
				}
				else
				{
					CFilenameUpdater FnUp( SysFiles::PosTrayBatchStkLevelEcrTemp, 0, info.m_nBatchNo );
					CFileRemover FileRemover( FnUp.GetFilenameToUse() );
				}
			}
			break;

		case POSTRAY_TASKFILE_EPOS_TASK:
		default:
			{
				CFilenameUpdater FnUp( SysFiles::PosTrayBatchEposTemp, 0, info.m_nBatchNo );
				CFileRemover FileRemover( FnUp.GetFilenameToUse() );
			}
			break;
		}

		m_pArrayBatchInfo -> RemoveAt( nIndex );	
	}
}

/**********************************************************************/

void CSingleListContentHelpersPosTrayBatch::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Name", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPosTrayBatchDlg::CListDataPosTrayBatchDlg( CPosTrayFileSet* pFileSet, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pPosTrayFileSet = pFileSet;
	m_nBatchNo = 0;
	m_pArrayBatchInfo = NULL;

	m_pContentHelpers = new CSingleListContentHelpersPosTrayBatch( m_pPosTrayFileSet );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	switch( m_pPosTrayFileSet -> m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "CSV Sales Line Format" );
			m_pDisplayHelpers -> SetTopPluralText( "CSV Sales Line Formats" );
			m_pDisplayHelpers -> SetBottomSingleText( "format" );
			m_pDisplayHelpers -> SetBottomPluralText( "formats" );
		}
		break;
	
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "CSV Payment Line Format" );
			m_pDisplayHelpers -> SetTopPluralText( "CSV Payment Line Formats" );
			m_pDisplayHelpers -> SetBottomSingleText( "format" );
			m_pDisplayHelpers -> SetBottomPluralText( "formats" );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "CSV Void Line Format" );
			m_pDisplayHelpers -> SetTopPluralText( "CSV Voids Line Formats" );
			m_pDisplayHelpers -> SetBottomSingleText( "format" );
			m_pDisplayHelpers -> SetBottomPluralText( "formats" );
		}
		break;

	case POSTRAY_TASKFILE_HIST_TASK:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "Sales History Report Batch" );
			m_pDisplayHelpers -> SetTopPluralText( "Sales History Report Batches" );
			m_pDisplayHelpers -> SetBottomSingleText( "batch" );
			m_pDisplayHelpers -> SetBottomPluralText( "batches" );
		}
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "Stock Level Report Batch" );
			m_pDisplayHelpers -> SetTopPluralText( "Stock Level Report Batches" );
			m_pDisplayHelpers -> SetBottomSingleText( "batch" );
			m_pDisplayHelpers -> SetBottomPluralText( "batches" );
		}
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		{
			m_pDisplayHelpers -> SetTopSingleText(  "EPOS Report Batch" );
			m_pDisplayHelpers -> SetTopPluralText( "EPOS Report Batches" );
			m_pDisplayHelpers -> SetBottomSingleText( "batch" );
			m_pDisplayHelpers -> SetBottomPluralText( "batches" );
		}
		break;
	}
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::SetBatchInfoArray( CReportConsolidationArray<CPosTrayBatchInfo>* pArray )
{ 
	m_pArrayBatchInfo = pArray;
	CSingleListContentHelpersPosTrayBatch* pHelpers = (CSingleListContentHelpersPosTrayBatch*) m_pContentHelpers;
	pHelpers -> SetBatchInfoArray( pArray );
}

/**********************************************************************/

CListDataPosTrayBatchDlg::~CListDataPosTrayBatchDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::SetTitle()
{
	switch( m_pPosTrayFileSet -> m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_HIST_TASK:
		SetWindowText ( "Configure Sales History Report Batches for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		SetWindowText ( "Configure CSV Sales Line Formats for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		SetWindowText ( "Configure CSV Payment Line Formats for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		SetWindowText ( "Configure CSV Void Line Formats for PosTray" );
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		SetWindowText ( "Configure Stock Level Report Batches for PosTray" );
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		SetWindowText ( "Configure EPOS Report Batches for PosTray" );
		break;
	}
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Copy" );
	m_button3.SetWindowText ( "&Edit" );
	m_button4.SetWindowText ( "&Delete" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 127 );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	for ( int n = 0; n < m_pArrayBatchInfo -> GetSize(); n++ )
	{
		CPosTrayBatchInfo info;
		m_pArrayBatchInfo -> GetAt( n, info );

		if ( info.m_nBatchNo == m_nBatchNo )
		{
			SelectLine( n );
			break;
		}
	}
}

/**********************************************************************/

bool CListDataPosTrayBatchDlg::GetNextBatchNo( int& nBatchNo )
{
	nBatchNo = 1;

	for ( int n = 0; n < m_pArrayBatchInfo -> GetSize(); n++ )
	{
		CPosTrayBatchInfo info;
		m_pArrayBatchInfo -> GetAt( n, info );

		if ( info.m_nBatchNo > nBatchNo )
			break;

		nBatchNo++;
	}

	return ( nBatchNo <= POSTRAY_MAX_BATCH );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::NewBatchError()
{
	if ( m_pPosTrayFileSet -> IsExportTask() == TRUE )
		Prompter.Error( "You have already created the maximum\nnumber of CSV line formats" );
	else
		Prompter.Error( "You have already created the maximum\nnumber of report batches" );
		
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::HandleButton1() 
{
	int nBatchNo = 0;
	if ( GetNextBatchNo( nBatchNo ) == FALSE )
	{
		NewBatchError();
		return;
	}

	CPosTrayTask batch( m_pPosTrayFileSet -> m_nBatchFileType );
	
	AddBatch( nBatchNo, batch );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
	{
		return;
	}

	int nBatchNo = 0;
	if ( GetNextBatchNo( nBatchNo ) == FALSE )
	{
		NewBatchError();
		return;
	}

	CPosTrayTask BatchToCopy( m_pPosTrayFileSet -> m_nBatchFileType );
	
	CPosTrayBatchInfo infoBatch;
	m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

	BatchToCopy.SetBatchNo( infoBatch.m_nBatchNo );
	BatchToCopy.Read( infoBatch.m_bModified );

	AddBatch( nBatchNo, BatchToCopy );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::AddBatch( int nBatchNo, CPosTrayTask& BatchToCopy )
{
	CPosTrayBatchInfo info;
	info.m_nBatchNo = nBatchNo;
	info.m_strBatchName.Format( "%s %d", 
			m_pPosTrayFileSet -> IsExportTask() ? "Line Format" : "Batch",
			nBatchNo );
	info.m_bModified = TRUE;

	int nPos;
	if ( m_pArrayBatchInfo -> Find( info, nPos ) == FALSE )
	{
		m_pArrayBatchInfo -> InsertAt( nPos, info );
		m_listData.SetItemCountEx ( m_pArrayBatchInfo -> GetSize() );
		m_listData.Invalidate();

		BatchToCopy.SetBatchNo( info.m_nBatchNo );
		BatchToCopy.SetBatchName( info.m_strBatchName );
		BatchToCopy.SetBatchSaleTimeFlag( EcrmanOptions.GetReportsSalesTimeType() == 1 );
		BatchToCopy.Write( TRUE );

		EditItem( nPos );
	}
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::HandleButton3() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	EditItem( nIndex );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::EditItem( int nIndex )
{
	SelectLine( nIndex );

	switch( m_pPosTrayFileSet -> m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_HIST_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
			DataManagerNonDb.OpenSalesHistoryPlu( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_HIST_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchReportHistoryDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}	

			dbLocation.RemovePMSPseudoLocation();

			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseTimeSliceList( info );
			DataManagerNonDb.CloseSalesHistoryPlu( info );
			DataManagerNonDb.CloseSystemPluFilterNames( info );
			DataManagerNonDb.CloseSystemPluFilterItems( info );
			DataManagerNonDb.CloseSystemDepartment( info );
			DataManagerNonDb.CloseSystemDepartmentSet( info );
		}
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession(DB_READONLY, info);
		
		CPosTrayBatchInfo infoBatch;
		m_pArrayBatchInfo->GetAt(nIndex, infoBatch);

		CPosTrayTask batch(POSTRAY_TASKFILE_TIMEATTEND_BATCH);
		batch.SetBatchNo(infoBatch.m_nBatchNo);
		batch.Read(infoBatch.m_bModified);

		CPosTrayBatchReportTimeAttendDlg dlg(batch, this);

		if (dlg.DoModal() == IDOK)
		{
			batch.Write(TRUE);
			infoBatch.m_strBatchName = batch.GetBatchName();
			infoBatch.m_bModified = TRUE;
			m_pArrayBatchInfo->SetAt(nIndex, infoBatch);
		}

		DataManagerNonDb.CloseReportSession(info);
	}
	break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
			DataManagerNonDb.OpenSalesHistoryPlu( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_EXPORT_SALES_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchExportSalesDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}	

			dbLocation.RemovePMSPseudoLocation();

			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseTimeSliceList( info );
			DataManagerNonDb.CloseSalesHistoryPlu( info );
			DataManagerNonDb.CloseSystemPluFilterNames( info );
			DataManagerNonDb.CloseSystemPluFilterItems( info );
			DataManagerNonDb.CloseSystemDepartment( info );
			DataManagerNonDb.CloseSystemDepartmentSet( info );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
			DataManagerNonDb.OpenSalesHistoryPlu( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_EXPORT_DISCOUNT_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchExportDiscountDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}	

			dbLocation.RemovePMSPseudoLocation();

			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseTimeSliceList( info );
			DataManagerNonDb.CloseSalesHistoryPlu( info );
			DataManagerNonDb.CloseSystemPluFilterNames( info );
			DataManagerNonDb.CloseSystemPluFilterItems( info );
			DataManagerNonDb.CloseSystemDepartment( info );
			DataManagerNonDb.CloseSystemDepartmentSet( info );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
			
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_EXPORT_PAYMENT_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchExportPaymentDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}	

			dbLocation.RemovePMSPseudoLocation();

			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseTimeSliceList( info );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
			
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_EXPORT_VOID_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			//dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchExportVoidDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}

			//dbLocation.RemovePMSPseudoLocation();

			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseTimeSliceList( info );
		}
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:

#ifndef STOCKMAN_SYSTEM

		{
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_STKLEVEL_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			CPosTrayBatchReportStkLevelEcrDlg dlg( batch, this );
				
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
			}
		}

#else

		{
		}

#endif

		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

			CPosTrayTask batch( POSTRAY_TASKFILE_EPOS_BATCH );
			batch.SetBatchNo( infoBatch.m_nBatchNo );
			batch.Read( infoBatch.m_bModified );

			dbLocation.CreatePMSPseudoLocation();

			CPosTrayBatchReportEposDlg dlg( batch, infoBatch.m_bModified, this );
			
			if ( dlg.DoModal() == IDOK )
			{
				batch.Write( TRUE );
				infoBatch.m_strBatchName = batch.GetBatchName();
				infoBatch.m_bModified = TRUE;
				m_pArrayBatchInfo -> SetAt( nIndex, infoBatch );
				m_listData.Invalidate();
			}

			dbLocation.RemovePMSPseudoLocation();
			
			DataManagerNonDb.CloseReportSession( info );
			DataManagerNonDb.CloseSystemPluFilterNames( info );
			DataManagerNonDb.CloseSystemPluFilterItems( info );
			DataManagerNonDb.CloseSystemDepartment( info );
			DataManagerNonDb.CloseSystemDepartmentSet( info );
		}
		break;
	}

	SelectLine ( nIndex );
}

/**********************************************************************/

void CListDataPosTrayBatchDlg::HandleButton4() 
{
	m_pDisplayHelpers -> HandleDelete( this );
}

/**********************************************************************/

bool CListDataPosTrayBatchDlg::HandleCancel()
{
	return HandleOK();
}

/**********************************************************************/

bool CListDataPosTrayBatchDlg::HandleOK()
{
	m_nBatchNo = 0;

	int nIndex = GetSafeSelection();
	if ( nIndex != -1 )
	{
		CPosTrayBatchInfo info;
		m_pArrayBatchInfo -> GetAt( nIndex, info );
		m_nBatchNo = info.m_nBatchNo;
	}
	return TRUE;
}

/**********************************************************************/