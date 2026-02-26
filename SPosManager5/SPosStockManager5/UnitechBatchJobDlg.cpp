/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "Password.h"
#include "UnitechBatchJob.h"
#include "UnitechBatchJobResultsDlg.h"
#include "UnitechOptions.h"
/**********************************************************************/
#include "UnitechBatchJobDlg.h"
/**********************************************************************/

CUnitechBatchJobDlg::CUnitechBatchJobDlg( int nAction, int nIndex, CWnd* pParent )
	: CDialog("IDD_UNITECH_BATCHJOB", pParent)
{
	//{{AFX_DATA_INIT(CUnitechBatchJobDlg)
	//}}AFX_DATA_INIT

	switch( nAction )
	{
	case NODE_UNITECH_PLU:
		m_nAction = NODE_UNITECH_PLU;
		m_nSpIdx = 0;
		m_nDbIdx = nIndex;
		break;

	case NODE_UNITECH_STOCK:
	default:
		{
			m_nAction = NODE_UNITECH_STOCK;

			m_nSpIdx = nIndex;
	
			int nDbNo = dbStockpoint.GetDbNo( m_nSpIdx );
			dbDatabase.FindDatabaseByNumber( nDbNo, m_nDbIdx );
		}
		break;
	}	
}

/**********************************************************************/

void CUnitechBatchJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnitechBatchJobDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC2_CHECK_DOWNLOAD_PLU, m_checkDownloadPlu);
	DDX_Control(pDX, IDC2_CHECK_UPLOAD_PLU, m_checkUploadPlu);
	DDX_Control(pDX, IDC2_CHECK_UPLOAD_STOCK, m_checkUploadStock);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CUnitechBatchJobDlg, CDialog)
	//{{AFX_MSG_MAP(CUnitechBatchJobDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CUnitechBatchJobDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;

	if ( NODE_UNITECH_PLU == m_nAction )
	{
		strTitle.Format( "Unitech Download (%s)", dbDatabase.GetName( m_nDbIdx ) );
		m_checkUploadPlu.SetCheck( TRUE );
		m_checkDownloadPlu.SetCheck( TRUE );
		m_checkUploadStock.EnableWindow( FALSE );
	}
	else
	{
		strTitle.Format( "Unitech Import (%s)", dbStockpoint.GetName( m_nSpIdx ) );
		m_checkUploadPlu.SetCheck( TRUE );
		m_checkDownloadPlu.SetCheck( TRUE );
		m_checkUploadStock.SetCheck( TRUE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::UnitechPluDownload ) == FALSE )
	{
		m_checkDownloadPlu.SetCheck( FALSE );
		m_checkDownloadPlu.EnableWindow( FALSE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::UnitechPluUpload ) == FALSE )
	{
		m_checkUploadPlu.SetCheck( FALSE );
		m_checkUploadPlu.EnableWindow( FALSE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::UnitechStockUpload ) == FALSE )
	{
		m_checkUploadStock.SetCheck( FALSE );
		m_checkUploadStock.EnableWindow( FALSE );
	}

	m_checkUploadPlu.SetCheck( FALSE );
	m_checkUploadPlu.ShowWindow( SW_HIDE );
	m_checkUploadPlu.EnableWindow( FALSE );

	SetWindowText( strTitle );

	return TRUE;  
}

/**********************************************************************/

void CUnitechBatchJobDlg::OnOK() 
{
	bool bUploadStock = FALSE;
	bool bUploadPlu = ( m_checkUploadPlu.GetCheck() != 0 );
	bool bDownloadPlu = ( m_checkDownloadPlu.GetCheck() != 0 );
	
	if ( NODE_UNITECH_STOCK == m_nAction )
		bUploadStock = ( m_checkUploadStock.GetCheck() != 0 );

	bUploadPlu = FALSE;
	
	if ( ( FALSE == bUploadStock ) && ( FALSE == bUploadPlu ) && ( FALSE == bDownloadPlu ) )
	{
		Prompter.Error ( "You must select at least one action before\nstarting the Unitech import program." );
	}
	else
	{
		CUnitechBatchJobResultsDlg dlgResults( this );
					
		CUnitechBatchJob BatchJob( m_nDbIdx, m_nSpIdx );

		switch( BatchJob.RunBatch( bUploadPlu, bDownloadPlu, bUploadStock, dlgResults.m_Results, this ) )
		{
		case UNITECH_BATCH_ERROR_PROGRAM:
			{
				CString str = "";
				str += "Unable to find Unitech import program\n\n";
				str += "Folder : ";
				str += UnitechOptions.GetProgramFolder();
				str += "\n\n";
				str += "Program : ";
				str += UnitechOptions.GetProgramFile();
				Prompter.Error( str );
			}
			break;

		case UNITECH_BATCH_ERROR_JOBFILE:
			Prompter.Error( "Unable to create batch file for Unitech import program." );
			break;

		case UNITECH_BATCH_ERROR_CREATEFOLDER:
			Prompter.Error( "Unable to create folder for Unitech CSV files." );
			break;

		case UNITECH_BATCH_ERROR_LOCK:
			Prompter.Error( "Unable to access Unitech data files.\n\nAnother user is already running this job." );
			break;

		case UNITECH_BATCH_ERROR_SPAWN:
			Prompter.Error( "Unable to start the Unitech import program." );
			break;

		case UNITECH_BATCH_ERROR_NONE:
			{
				CString strMessage;
				bool bSuccess = BatchJob.GetStatusMessage( strMessage );

				if ( ( FALSE == bUploadStock ) || ( FALSE == bSuccess ) )
					Prompter.Info( strMessage );
				else
					dlgResults.DoModal();
			}
			break;
		}
	}
}

/**********************************************************************/
