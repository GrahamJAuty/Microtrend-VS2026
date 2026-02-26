// DatabaseReportsDlg.cpp : implementation file
//

#include "LoyaltyManager.h"
#include "DbExportHandler.h"
#include "PasswordData.h"
#include "DatabaseReportsDlg.h"

// CDatabaseReportsDlg dialog

CDatabaseReportsDlg::CDatabaseReportsDlg( CWnd* pParent /*=NULL*/)
	: CDialog(CDatabaseReportsDlg::IDD, pParent)
{
	m_strReportName		= _T("");
	m_strReportLabel	= "";
	m_nDefaultOption	= 0;
	m_bEditReqd			= FALSE;
}

CDatabaseReportsDlg::~CDatabaseReportsDlg()
{
}

void CDatabaseReportsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_LIST_DBREPORTS, m_listboxReports);
	DDX_LBString(pDX, IDC_LIST_DBREPORTS, m_strReportName);
}


BEGIN_MESSAGE_MAP(CDatabaseReportsDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_DBREPORTS, OnDblclkListDbReports)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
END_MESSAGE_MAP()


// CDatabaseReportsDlg message handlers

BOOL CDatabaseReportsDlg::OnInitDialog()
{
	m_csvReportList.RemoveAll();
	m_export.GetCustomList ( m_csvReportList );				// URnn,repname,URnn,repame etc

	if ( m_csvReportList.GetSize() == 0 )					// no custom reports
	{
		m_strReportLabel = "UR01";
		m_export.Read ( m_strReportLabel );					// create default settings
		m_export.Save ( m_strReportLabel );					// save default:
		m_export.GetCustomList ( m_csvReportList );			// create custom list
	}

	CDialog::OnInitDialog();

	for ( int nIndex = 1 ; nIndex < m_csvReportList.GetSize() ; nIndex += 2 )
	{
		CString strName = m_csvReportList.GetString(nIndex);
		m_listboxReports.AddString ( m_csvReportList.GetString ( nIndex ) );
	}

	m_listboxReports.SetCurSel(m_nDefaultOption);
	m_bEditReqd = FALSE;
	
	if ( Passwords.IsEditRecordAllowed() == FALSE )
	{
		m_buttonAdd.EnableWindow(FALSE);
		m_buttonEdit.EnableWindow(FALSE);
		m_buttonDelete.EnableWindow(FALSE);
	}

	return TRUE;
}

void CDatabaseReportsDlg::OnDblclkListDbReports()
{
	OnOK();
}

void CDatabaseReportsDlg::OnButtonEdit() 
{
	m_bEditReqd = TRUE;
	OnOK();
}

void CDatabaseReportsDlg::OnOK() 
{
	if ( UpdateData() == TRUE )
	{
		GetLabel();
		m_nDefaultOption = m_listboxReports.FindString (-1, m_strReportName );
		CDialog::OnOK();
	}
}

//*******************************************************************
// get associated report label for report title

int CDatabaseReportsDlg::GetLabel()
{
	m_strReportLabel = "";

	int nIndex;
	for ( nIndex = 0 ; nIndex < m_csvReportList.GetSize() ; nIndex += 2 )	// URnn,repname,URnn,repame etc
	{
		CString strName = m_csvReportList.GetString(nIndex+1);
		if ( m_strReportName == strName )
		{
			m_strReportLabel = m_csvReportList.GetString(nIndex);
			break;
		}
	}

	return nIndex;
}

void CDatabaseReportsDlg::OnButtonAdd() 
{
	if ( UpdateData() == TRUE )
	{
		if ( m_export.GetNextCustomLabel( m_strReportLabel ) == TRUE )
		{
			m_bEditReqd = TRUE;
			CDialog::OnOK();
		}
	}
}

void CDatabaseReportsDlg::OnButtonDelete() 
{
	if ( UpdateData() == TRUE )
	{
		GetLabel();											// get report label from list box

		CString strMsg;
		strMsg.Format ( "Delete database report ' %s '", m_strReportName );

		if ( Prompter.YesNo( strMsg, "Please confirm" ) == IDYES )
		{
			m_export.Delete ( m_strReportLabel );			// delete report label for default file

			int nListIndex = m_listboxReports.FindString(-1, m_strReportName );
			m_listboxReports.DeleteString(nListIndex);			// delete report from list box

			m_csvReportList.RemoveAll();						// rebuild list
			m_export.GetCustomList ( m_csvReportList );			// URnn,repname,URnn,repame etc
		}
	}	
}

//*******************************************************************

