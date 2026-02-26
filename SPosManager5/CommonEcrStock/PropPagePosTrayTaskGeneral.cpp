/**********************************************************************/
#include "..\SPosEcrManager5\PosTrayBatchReportStkLevelEcrDlg.h"
/**********************************************************************/
#include "ListDataEmailAddressDlg.h"
#include "ListDataPosTrayBatchDlg.h"
#include "PosTrayBatchExportDiscountDlg.h"
#include "PosTrayBatchExportSalesDlg.h"
#include "PosTrayBatchExportPaymentDlg.h"
#include "PosTrayBatchExportVoidDlg.h"
#include "PosTrayBatchReportEposDlg.h"
#include "PosTrayBatchReportHistoryDlg.h"
#include "PosTrayBatchReportTimeAttendDlg.h"
#include "PosTrayEmailJobsDlg.h"
#include "PosTrayOptions.h"
#include "PosTrayTask.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePosTrayTaskGeneral.h"
/**********************************************************************/

CPropPagePosTrayTaskGeneral::CPropPagePosTrayTaskGeneral() : CSSPropertyPage(CPropPagePosTrayTaskGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayTaskGeneral)
	//}}AFX_DATA_INIT
	m_pPosTrayFileSet = NULL;
	m_strEmailByReport = "";
}

/**********************************************************************/

CPropPagePosTrayTaskGeneral::~CPropPagePosTrayTaskGeneral()
{
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayTaskGeneral)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_STATIC_BATCH, m_staticBatch);
	DDX_Control(pDX, IDC_COMBO_BATCH, m_comboBatch);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_staticFormatEmail);
	DDX_Control(pDX, IDC_COMBO_FORMAT_EMAIL, m_comboFormatEmail);
	DDX_Control(pDX, IDC_BUTTON_EMAIL_JOBS, m_buttonEmailJobs);
	DDX_Control(pDX, IDC_BUTTON_EMAIL_LIST, m_buttonEmailList);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticFormatExport);
	DDX_Control(pDX, IDC_COMBO_FORMAT_EXPORT, m_comboFormatExport);
	DDX_Control(pDX, IDC_STATIC_PRINT, m_staticPrint);
	DDX_Control(pDX, IDC_COMBO_PRINT, m_comboPrint);
	DDX_Control(pDX, IDC_STATIC_FTP, m_staticFTP);
	DDX_Control(pDX, IDC_CHECK_FTP, m_checkFTP);
	DDX_Control(pDX, IDC_EDIT_FTP, m_editFTP);
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_COMBO_DATE, m_comboDate);
	DDX_Control(pDX, IDC_CHECK_FOLDER, m_checkFolder);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_editFolder);
	DDX_Control(pDX, IDC_BUTTON_FOLDER, m_buttonFolder);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFilename);
	DDX_Control(pDX, IDC_CHECK_FILENAME, m_checkFilename);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFilename);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFilename);
	DDX_Control(pDX, IDC_COMBO_FILENAME, m_comboFilename);
	DDX_Control(pDX, IDC_STATIC_LOCINFO, m_staticLocInfo);
	DDX_Control(pDX, IDC_COMBO_LOCINFO, m_comboLocInfo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayTaskGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayTaskGeneral)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EDIT, OnButtonBatchEdit)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_LIST, OnButtonBatchList)
	ON_BN_CLICKED(IDC_BUTTON_EMAIL_LIST, OnButtonEmailList)
	ON_BN_CLICKED(IDC_BUTTON_EMAIL_JOBS, OnButtonEmailJobs)
	ON_BN_CLICKED(IDC_CHECK_FTP, OnToggleFTP)
	ON_BN_CLICKED(IDC_CHECK_FOLDER, OnToggleFolder)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	ON_BN_CLICKED(IDC_CHECK_FILENAME, OnToggleFilename)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayTaskGeneral::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	MoveControl(&m_staticDate, 200, 173);
	MoveControl(&m_comboDate, 266, 173);
	MoveControl(&m_checkFTP, 21, 205);
	MoveControl(&m_staticFTP, 33, 206);
	MoveControl(&m_editFTP, 113, 205);
	ResizeControl(&m_editFTP, 233, 12);

	m_editName.LimitText(POSTRAY_MAXLENGTH_TASK_NAME);
	m_editName.SetWindowText(m_pTask->GetTaskName());

	FillBatchCombo(m_pTask->GetTaskBatchNo());
	FillEmailCombo();

	m_arrayEmailAddresses.RemoveAll();
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		m_arrayEmailAddresses.Add(m_pTask->GetTaskEmailNickname(n));
	}

	UpdateDefaultEmailCombo();

	m_strDefaultFilename = "";
	switch( m_pPosTrayFileSet -> m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		m_strDefaultFilename.Format(  "plusales_%3.3d.csv", m_pTask -> GetTaskNo() );
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		m_strDefaultFilename.Format(  "payment_%3.3d.csv", m_pTask -> GetTaskNo() );
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		m_strDefaultFilename.Format(  "void_%3.3d.csv", m_pTask -> GetTaskNo() );
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		m_strDefaultFilename.Format(  "discount_%3.3d.csv", m_pTask -> GetTaskNo() );
		break;
	}

	switch (m_pPosTrayFileSet->m_nTaskFileType)
	{
	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	{
		m_staticBatch.SetWindowText("Report batch");

		m_comboFormatEmail.AddString("None");
		m_comboFormatEmail.AddString("CSV");
		m_comboFormatEmail.AddString("HTML");

		m_comboFormatExport.AddString("None");
		m_comboFormatExport.AddString("CSV");
		m_comboFormatExport.AddString("HTML");

		m_comboPrint.AddString("No");
		m_comboPrint.AddString("Yes");
		m_comboPrint.SetCurSel(m_pTask->GetTaskAutoPrintFlag() ? 1 : 0);

		CString strFilename = m_pTask->GetTaskExportFilename();
		::TrimSpaces(strFilename, FALSE);
		bool bEnableFilename = (strFilename != "");

		m_checkFilename.SetCheck(bEnableFilename);
		m_editFilename.SetWindowText(strFilename);
		m_editFilename.SetReadOnly(FALSE == bEnableFilename);

		m_checkFilename.SetWindowText("Export filename prefix");

		int nSel = m_pTask->GetTaskExportFilenameFormat();
		if ((nSel < 0) || (nSel > 5))
		{
			nSel = 5;
		}

		FillFilenameFormatCombo(FALSE, bEnableFilename, nSel);

		ShowAndEnableWindow(&m_staticFTP, FALSE);
		ShowAndEnableWindow(&m_checkFTP, FALSE);
		ShowAndEnableWindow(&m_editFTP, FALSE);
		ShowAndEnableWindow(&m_staticDate, FALSE);
		ShowAndEnableWindow(&m_comboDate, FALSE);
		ShowAndEnableWindow(&m_buttonEmailJobs, TRUE);
		MoveControl(&m_buttonEmailList, 153, 121);
	}
	break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	{
		m_staticBatch.SetWindowText("CSV Line format");

		m_comboFormatEmail.AddString("None");
		m_comboFormatEmail.AddString("CSV");

		m_comboFormatExport.AddString("None");
		m_comboFormatExport.AddString("CSV");

		m_comboDate.AddString("Default");
		m_comboDate.AddString("DD/MM/YYYY");
		m_comboDate.AddString("YYYY/MM/DD");
		m_comboDate.AddString("DDMMYYYY");
		m_comboDate.AddString("YYYYMMDD");

		CString strFilename = m_pTask->GetTaskExportFilename();
		::TrimSpaces(strFilename, FALSE);
		bool bEnableFilename = (strFilename != "");

		if (strFilename == "")
		{
			strFilename = m_strDefaultFilename;
			bEnableFilename = FALSE;
		}

		m_checkFilename.SetCheck(bEnableFilename);
		m_editFilename.SetWindowText(strFilename);
		m_editFilename.SetReadOnly(FALSE == bEnableFilename);

		ShowAndEnableWindow(&m_staticPrint, FALSE);
		ShowAndEnableWindow(&m_comboPrint, FALSE);

		int nSel = m_pTask->GetTaskExportFilenameFormat();
		if ((nSel < 0) || (nSel > 5))
		{
			nSel = 5;
		}

		FillFilenameFormatCombo(TRUE, FALSE, nSel);

		if (PosTrayOptions.GotFTPSettings() == FALSE)
		{
			m_checkFTP.SetCheck(FALSE);
			m_editFTP.SetWindowText("<FTP Settings have not been configured>");
			m_checkFTP.EnableWindow(FALSE);
			m_editFTP.SetReadOnly(TRUE);
		}
		else if (m_pTask->GetTaskFTPFlag() == TRUE)
		{
			m_checkFTP.SetCheck(TRUE);
			m_editFTP.EnableWindow(TRUE);
			m_editFTP.SetWindowText(m_pTask->GetTaskFTPFolder());
		}
		else
		{
			m_checkFTP.SetCheck(FALSE);
			m_editFTP.EnableWindow(FALSE);
			m_editFTP.SetWindowText("");
		}

		{
			int nSel = 0;
			switch (m_pTask->GetTaskExportDateType())
			{
			case 0:	nSel = 0;	break;
			case 1:	nSel = 1;	break;
			case 2: nSel = 3;	break;
			case 3:	nSel = 4;	break;
			case 4:	nSel = 2;	break;
			}
			m_comboDate.SetCurSel(nSel);
		}
		ShowAndEnableWindow(&m_buttonEmailJobs, FALSE);
		MoveControl(&m_buttonEmailList, 113, 121);
	}
	break;

	default:
		ShowAndEnableWindow(&m_staticFilename, FALSE);
		ShowAndEnableWindow(&m_comboFilename, FALSE);
		ShowAndEnableWindow(&m_checkFilename, FALSE);
		ShowAndEnableWindow(&m_editFilename, FALSE);
		ShowAndEnableWindow(&m_staticPrint, FALSE);
		ShowAndEnableWindow(&m_comboPrint, FALSE);
		ShowAndEnableWindow(&m_staticFTP, FALSE);
		ShowAndEnableWindow(&m_checkFTP, FALSE);
		ShowAndEnableWindow(&m_editFTP, FALSE);
		ShowAndEnableWindow(&m_staticDate, FALSE);
		ShowAndEnableWindow(&m_comboDate, FALSE);
		ShowAndEnableWindow(&m_buttonEmailJobs, FALSE);
		MoveControl(&m_buttonEmailList, 113, 121);
		break;
	}

	switch (m_pPosTrayFileSet->m_nTaskFileType)
	{
	case POSTRAY_TASKFILE_EPOS_TASK:
		m_comboLocInfo.AddString("Automatically generated unique number");
		m_comboLocInfo.AddString("Database, Location or Terminal name");
		m_comboLocInfo.SetCurSel(m_pTask->GetTaskLocNameFlag() ? 1 : 0);
		break;

	default:
		ShowAndEnableWindow(&m_staticLocInfo, FALSE);
		ShowAndEnableWindow(&m_comboLocInfo, FALSE);
		break;
	}

	{
		bool bEnableFolder = TRUE;
	
		CString strFolder = m_pTask -> GetTaskExportFolder();
		::TrimSpaces( strFolder, FALSE );
		if ( strFolder == "" )
		{
			strFolder = "PosTray\\Exports";
			::GetSyssetProgramPath( strFolder );
			strFolder = strFolder.Right(26);
			bEnableFolder = FALSE;
		}
		
		m_checkFolder.SetCheck( bEnableFolder );
		m_editFolder.SetWindowText( strFolder );
		m_editFolder.SetReadOnly( FALSE == bEnableFolder );
		m_buttonFolder.EnableWindow( bEnableFolder );
	}

	{
		int nSel = m_pTask -> GetTaskFormatEmail() + 1;
		if ( ( nSel < 0 ) || ( nSel >= m_comboFormatEmail.GetCount() ) ) nSel = 0;
		m_comboFormatEmail.SetCurSel( nSel );
	}
	
	{
		int nSel = m_pTask -> GetTaskFormatExport() + 1;
		if ( ( nSel < 0 ) || ( nSel >= m_comboFormatExport.GetCount() ) ) nSel = 0;
		m_comboFormatExport.SetCurSel( nSel );
	}

	m_strEmailByReport = m_pTask->GetTaskEmailByReport();

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::FillFilenameFormatCombo(bool bExport, bool bPrefix, int nSel)
{
	CString strPrefix = "";
	CString strTaskType = "";
	CString strLocInfo = "";

	if (TRUE == bExport)
	{
		strTaskType = "<ExportFilename>";
	}
	else
	{
		strTaskType = "<ReportFilename>";

		if (TRUE == bPrefix)
		{
			strPrefix = "<Prefix>";
		}
	}

	if (POSTRAY_TASKFILE_EPOS_TASK == m_pPosTrayFileSet->m_nTaskFileType)
	{
		strLocInfo = "_<LocInfo>";
	}

	m_comboFilename.ResetContent();
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo);
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo + "_<Date>");
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo + "_<Date>_<Time>");
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo + "_<TaskNo>");
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo + "_<TaskNo>_<Date>");
	m_comboFilename.AddString(strPrefix + strTaskType + strLocInfo + "_<TaskNo>_<Date>_<Time>");
	m_comboFilename.SetCurSel(nSel);
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::FillBatchCombo( int nBatchNo )
{
	m_comboBatch.ResetContent();
	m_comboBatch.AddString( m_pPosTrayFileSet -> IsExportTask() ?  "<New Format>" : "<New Batch>" );

	int nSel = 0;
	for ( int nBatchIdx = 0; nBatchIdx < m_pArrayBatchInfo -> GetSize(); nBatchIdx++ )
	{
		CPosTrayBatchInfo infoBatch;
		m_pArrayBatchInfo -> GetAt( nBatchIdx, infoBatch );
		m_comboBatch.AddString( infoBatch.m_strBatchName );

		if ( infoBatch.m_nBatchNo == nBatchNo )
			nSel = nBatchIdx + 1;
	}

	m_comboBatch.SetCurSel( nSel );
	m_comboBatch.EnableWindow( m_comboBatch.GetCount() > 1 );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::FillEmailCombo()
{
	m_comboEmail.ResetContent();
	
	for ( int nIndex = 0; nIndex < DataManagerNonDb.EmailAddress.GetSize(); nIndex++ )
		m_comboEmail.AddString ( DataManagerNonDb.EmailAddress.GetComboText ( nIndex ) );
}

/**********************************************************************/

BOOL CPropPagePosTrayTaskGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayTaskGeneral::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnButtonBatchEdit()
{
	int nSel = m_comboBatch.GetCurSel();

	if ( 0 == nSel )
	{
		int nBatchNo = 1;

		for ( int nBatchIdx = 0; nBatchIdx < m_pArrayBatchInfo -> GetSize(); nBatchIdx++ )
		{
			CPosTrayBatchInfo infoBatch;
			m_pArrayBatchInfo -> GetAt( nBatchIdx, infoBatch );

			if ( infoBatch.m_nBatchNo > nBatchNo )
				break;

			nBatchNo++;
		}

		if ( nBatchNo > POSTRAY_MAX_BATCH )
		{
			if ( m_pPosTrayFileSet -> IsExportTask() == TRUE )
				Prompter.Error( "You have already created the maximum\nnumber of CSV line formats" );
			else
				Prompter.Error( "You have already created the maximum\nnumber of report batches" );

			return;
		}

		CPosTrayBatchInfo infoBatch;
		infoBatch.m_nBatchNo = nBatchNo;
		infoBatch.m_strBatchName.Format( "%s %d", 
				m_pPosTrayFileSet -> IsExportTask() ? "Line Format" : "Batch",
				nBatchNo );
		infoBatch.m_bModified = TRUE;

		int nPos;
		if ( m_pArrayBatchInfo -> Find( infoBatch, nPos ) == TRUE )
			return;

		m_pArrayBatchInfo -> InsertAt( nPos, infoBatch );
	
		CPosTrayTask batch( m_pPosTrayFileSet -> m_nBatchFileType );
		batch.SetBatchNo( infoBatch.m_nBatchNo );
		batch.SetBatchName( infoBatch.m_strBatchName );
		batch.SetBatchSaleTimeFlag( EcrmanOptions.GetReportsSalesTimeType() == 1 );
		batch.Write( TRUE );

		EditBatch( nPos );
	}
	else
		EditBatch( nSel - 1 );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::EditBatch( int nIndex )
{
	if ( ( nIndex < 0 ) || ( nIndex >= m_pArrayBatchInfo -> GetSize() ) )
		return;

	CPosTrayBatchInfo infoBatch;
	m_pArrayBatchInfo -> GetAt( nIndex, infoBatch );

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

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
#ifndef STOCKMAN_SYSTEM
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );

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

			DataManagerNonDb.CloseSystemPluFilterNames( info );
			DataManagerNonDb.CloseSystemPluFilterItems( info );
		}
#else
		{
		}
#endif
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession(DB_READONLY, info);
		
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

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenReportSession( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
			DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
			DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		
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

	FillBatchCombo( infoBatch.m_nBatchNo );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnButtonBatchList()
{
	int nBatchNo = 0;
	
	int nSel = m_comboBatch.GetCurSel();
	if ( ( nSel >= 1 ) && ( nSel <= m_pArrayBatchInfo -> GetSize() ) )
	{
		CPosTrayBatchInfo infoBatch;
		m_pArrayBatchInfo -> GetAt( nSel - 1, infoBatch );
		nBatchNo = infoBatch.m_nBatchNo;
	}
	
	CListDataPosTrayBatchDlg dlg( m_pPosTrayFileSet, this );
	dlg.SetBatchNo( nBatchNo );
	dlg.SetBatchInfoArray( m_pArrayBatchInfo );
	dlg.DoModal();

	FillBatchCombo( dlg.GetBatchNo() );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnButtonEmailList() 
{	
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenEmailAddressBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CString strOldEmail;
	m_comboEmail.GetWindowText( strOldEmail );
	
	CString strNewEmail = strOldEmail;

	strOldEmail = strOldEmail.SpanExcluding( "(" );
	::TrimSpaces( strOldEmail, FALSE );

	CListDataEmailAddressDlg dlg ( strOldEmail,  this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteEmailAddressBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
			strNewEmail = dlg.GetSelectedText();
	}
	
	DataManagerNonDb.CloseEmailAddressBuffer( info );

	FillEmailCombo();
	m_comboEmail.SetWindowText( strNewEmail );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::UpdateDefaultEmailCombo()
{
	CString strEmail = m_arrayEmailAddresses.GetAt(0);
	int nPos = DataManagerNonDb.EmailAddress.FindNameOrFirstAddress(strEmail);

	if (nPos != -1)
	{
		m_comboEmail.SetCurSel(nPos);
	}
	else
	{
		m_comboEmail.SetWindowText(strEmail);
	}
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::UpdateDefaultEmailAddress()
{
	CString strEmail = "";
	m_comboEmail.GetWindowText(strEmail);
	strEmail = strEmail.SpanExcluding("(");
	::TrimSpaces(strEmail, FALSE);
	m_arrayEmailAddresses.SetAt(0, strEmail);
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnButtonEmailJobs()
{
	int nBatchType = 0;
	switch (m_pPosTrayFileSet->m_nTaskFileType)
	{
	case POSTRAY_TASKFILE_EPOS_TASK:
		nBatchType = POSTRAY_TASKFILE_EPOS_BATCH;
		break;

	case POSTRAY_TASKFILE_HIST_TASK:
		nBatchType = POSTRAY_TASKFILE_HIST_BATCH;
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		nBatchType = POSTRAY_TASKFILE_TIMEATTEND_BATCH;
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		nBatchType = POSTRAY_TASKFILE_STKLEVEL_BATCH;
		break;

	default:
		return;
	}

	int nSel = m_comboBatch.GetCurSel();
	if ((nSel < 1) || (nSel > m_pArrayBatchInfo->GetSize()))
	{
		Prompter.Error("You must select a batch of reports before\nyou can use this option.");
		return;
	}

	CPosTrayBatchInfo infoBatch;
	m_pArrayBatchInfo->GetAt(nSel - 1, infoBatch);

	CPosTrayTask batch(nBatchType);
	batch.SetBatchNo(infoBatch.m_nBatchNo);
	batch.Read(infoBatch.m_bModified);

	UpdateDefaultEmailAddress();
	CPosTrayEmailJobsDlg dlgJobs( batch, m_arrayEmailAddresses, m_strEmailByReport, this);
	dlgJobs.DoModal();
	FillEmailCombo();
	UpdateDefaultEmailCombo();
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnToggleFTP()
{
	m_editFTP.EnableWindow( m_checkFTP.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnToggleFolder()
{
	bool bEnableFolder = ( m_checkFolder.GetCheck() != 0 );

	CString strFolder = "";
	if ( FALSE == bEnableFolder )
	{
		strFolder = "PosTray\\Exports";
		::GetSyssetProgramPath( strFolder );
		strFolder = strFolder.Right(26);
	}
		
	m_checkFolder.SetCheck( bEnableFolder );
	m_editFolder.SetWindowText( strFolder );
	m_editFolder.SetReadOnly( FALSE == bEnableFolder );
	m_buttonFolder.EnableWindow( bEnableFolder );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnButtonFolder()
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Export Folder Path", NULL, this ) == TRUE )
		m_editFolder.SetWindowText( strPathname );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::OnToggleFilename()
{
	bool bEnableFilename = ( m_checkFilename.GetCheck() != 0 );

	switch( m_pPosTrayFileSet -> m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		{
			CString strFilename = "";
			
			if ( FALSE == bEnableFilename )
				strFilename = m_strDefaultFilename;
		
			m_editFilename.SetWindowText( strFilename );
		}
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		FillFilenameFormatCombo( TRUE, bEnableFilename, m_comboFilename.GetCurSel() );
		break;
	}
		
	m_checkFilename.SetCheck( bEnableFilename );
	m_editFilename.SetReadOnly( FALSE == bEnableFilename );
}

/**********************************************************************/

void CPropPagePosTrayTaskGeneral::SaveRecord()
{
	CString strName = "";
	m_editName.GetWindowText(strName);
	m_pTask->SetTaskName(strName);

	int nSel = m_comboBatch.GetCurSel();
	if ((nSel >= 1) && (nSel <= m_pArrayBatchInfo->GetSize()))
	{
		CPosTrayBatchInfo infoBatch;
		m_pArrayBatchInfo->GetAt(nSel - 1, infoBatch);
		m_pTask->SetTaskBatchNo(infoBatch.m_nBatchNo);
	}
	else
	{
		m_pTask->SetTaskBatchNo(0);
	}

	UpdateDefaultEmailAddress();
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		m_pTask->SetTaskEmailNickname(n, m_arrayEmailAddresses.GetAt(n));
	}

	m_pTask->SetTaskFormatEmail(m_comboFormatEmail.GetCurSel() - 1);
	m_pTask->SetTaskFormatExport(m_comboFormatExport.GetCurSel() - 1);
	m_pTask->SetTaskAutoPrintFlag(m_comboPrint.GetCurSel() == 1);

	{
		CString strExportFolder = "";

		if (m_checkFolder.GetCheck() != 0)
			m_editFolder.GetWindowText(strExportFolder);

		::TrimSpaces(strExportFolder, FALSE);

		m_pTask->SetTaskExportFolder(strExportFolder);
	}

	{
		CString strExportFilename = "";

		if (m_checkFilename.GetCheck() != 0)
			m_editFilename.GetWindowText(strExportFilename);

		::TrimSpaces(strExportFilename, FALSE);

		m_pTask->SetTaskExportFilename(strExportFilename);
	}

	m_pTask->SetTaskExportFilenameFormat(m_comboFilename.GetCurSel());
	m_pTask->SetTaskLocNameFlag(m_comboLocInfo.GetCurSel() == 1);

	{
		if (m_checkFTP.GetCheck() != 0)
		{
			CString strFTPFolder = "";
			m_editFTP.GetWindowText(strFTPFolder);
			::TrimSpaces(strFTPFolder, FALSE);
			m_pTask->SetTaskFTPFlag(TRUE);
			m_pTask->SetTaskFTPFolder(strFTPFolder);
		}
		else
		{
			m_pTask->SetTaskFTPFlag(FALSE);
			m_pTask->SetTaskFTPFolder("");
		}
	}

	{
		int nType = 0;
		switch (m_comboDate.GetCurSel())
		{
		case 0:	nType = 0;	break;
		case 1:	nType = 1;	break;
		case 2: nType = 4;	break;
		case 3: nType = 2;	break;
		case 4: nType = 3;	break;
		}
		m_pTask->SetTaskExportDateType(nType);
	}

	m_pTask->SetTaskEmailByReport(m_strEmailByReport);
}

/**********************************************************************/
