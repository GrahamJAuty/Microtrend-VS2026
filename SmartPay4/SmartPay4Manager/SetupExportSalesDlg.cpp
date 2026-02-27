/**********************************************************************/
#include "DbExportEditor.h"
#include "HeaderWarnDlg.h"
#include "WebPaymentImportLogDlg.h"
#include "ReportCSVFile.h"
/**********************************************************************/
#include "SetupExportSalesDlg.h"
/**********************************************************************/

CSetupExportSalesDlg::CSetupExportSalesDlg( CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupExportSalesDlg::IDD, pParent)
{
	m_strReportLabel = "EXP";
	m_pParent = pParent;
}

/**********************************************************************/

void CSetupExportSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEW1, m_listboxView1);
	DDX_Control(pDX, IDC_BUTTON_FIELDS, m_buttonFields);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSetupExportSalesDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonLog)
	ON_BN_CLICKED(IDC_BUTTON_FIELDS, OnButtonFields)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
END_MESSAGE_MAP()

/**********************************************************************/
// CSetupExportSalesDlg message handlers

BOOL CSetupExportSalesDlg::CSSAutoShutdownPostInitDialog()
{
	m_dbReporter.Read ( m_strReportLabel );
	CString strExtraFields = m_dbReporter.m_strHeader;

	DisplayFieldList ( strExtraFields );

	CString strExportHeader = m_data.GetUploadHeader ( strExtraFields );			// add extra dbase fields to standard header

	bool bViewAllowed = FALSE;

	CString strFileHeader = strExportHeader;

	CSSFile file;
	if ( ( bViewAllowed = file.Open ( m_data.GetInternalExportFilename(), "rb" ) ) == TRUE )
	{
		strFileHeader = "";
		file.ReadString( strFileHeader );
	}

	if ( strFileHeader != strExportHeader )
	{
		MessageBeep ( MB_ICONEXCLAMATION );
		
		CHeaderWarnDlg dlg(this);
		dlg.SetTitle("WARNING: File Format Mismatch");
		dlg.SetInfo1Text("The export file header format is not as expected.");
		dlg.SetInfo2Text("Click Continue to view the current header.");
		dlg.SetHeader1(strFileHeader);
		dlg.SetHeader2(strExportHeader);
		dlg.SetHeader1Description("Existing Header");
		dlg.SetHeader2Description("Expected Header");
		dlg.SetAllowCancelFlag(FALSE);
		dlg.DoModal();
	}

	m_buttonFields.EnableWindow(!bViewAllowed);
	m_buttonView.EnableWindow(bViewAllowed);

	return TRUE;
}

/**********************************************************************/

void CSetupExportSalesDlg::DisplayFieldList ( const char* szHeader )
{
	CCSV csv ( szHeader );
	for (int i = 0; i < csv.GetSize(); i++)
	{
		m_listboxView1.AddString(csv.GetString(i));
	}
}

/**********************************************************************/

void CSetupExportSalesDlg::OnButtonLog()
{
	CWebPaymentImportLogDlg dlg ( nWEBPAYMENT_EXPORTSALES, this );
	dlg.DoModal();
}

/**********************************************************************/

void CSetupExportSalesDlg::OnButtonFields()
{
	CDbExportEditor editor ( this );
	editor.EditExportLabel ( m_strReportLabel );

	if ( m_dbReporter.Read ( m_strReportLabel ) == TRUE )
	{
		m_listboxView1.ResetContent();
		DisplayFieldList ( m_dbReporter.m_strHeader );		
	}
}

/**********************************************************************/

void CSetupExportSalesDlg::OnButtonView()
{
	CCSVFileReport csv ( "R220", m_data.GetInternalExportFilename(), this );
	csv.Show();
}

/**********************************************************************/
