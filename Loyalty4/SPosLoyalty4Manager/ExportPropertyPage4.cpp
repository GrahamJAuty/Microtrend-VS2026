//$$******************************************************************
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
//$$******************************************************************
#include "ExportPropertyPage4.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CExportPropertyPage4::CExportPropertyPage4() : CSSPropertyPage(CExportPropertyPage4::IDD)
{
	m_nSaveAsType = 0;
}

//$$******************************************************************

CExportPropertyPage4::~CExportPropertyPage4()
{
}

//$$******************************************************************

void CExportPropertyPage4::SetPointer ( CDbExportHandler* pDbExportHandler )
{
	m_pDbExportHandler	= pDbExportHandler;
}

//$$******************************************************************

void CExportPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFilename);
	DDX_Control(pDX, IDC_RADIO_REPORTOUTPUT1, m_radioReportOutput1);
	DDX_Control(pDX, IDC_RADIO_REPORTOUTPUT2, m_radioReportOutput2);
	DDX_Control(pDX, IDC_RADIO_REPORTOUTPUT3, m_radioReportOutput3);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage4, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_REPORTOUTPUT1, OnRadioReportOutput1)
	ON_BN_CLICKED(IDC_RADIO_REPORTOUTPUT2, OnRadioReportOutput2)
	ON_BN_CLICKED(IDC_RADIO_REPORTOUTPUT3, OnRadioReportOutput3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CExportPropertyPage4::OnInitDialog() 
{
	m_nSaveAsType 	= m_pDbExportHandler->GetSaveAsType();
	
	CPropertyPage::OnInitDialog();
	m_editFilename.SetWindowText(m_pDbExportHandler->GetSaveAsFile());
	
	switch (m_nSaveAsType)
	{
	case 1:	
		OnRadioReportOutput2();
		break;

	case 2:
		OnRadioReportOutput3();
		break;

	case 0:
	default:
		OnRadioReportOutput1();
		break;
	}
	
	SetDisplay();

	return TRUE;
}

//$$******************************************************************

BOOL CExportPropertyPage4::OnKillActive()
{
	m_pDbExportHandler->SetOutputMethod(m_nSaveAsType);
	m_pDbExportHandler->SetOutputFile(GetEditBoxText(m_editFilename));
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CExportPropertyPage4::OnRadioReportOutput1()	
{ 
	m_nSaveAsType = 0;
	SetDisplay();	
}

//$$******************************************************************

void CExportPropertyPage4::OnRadioReportOutput2()
{
	m_nSaveAsType = 1;
	SetDisplay();
}

//$$******************************************************************

void CExportPropertyPage4::OnRadioReportOutput3()
{
	m_nSaveAsType = 2;
	SetDisplay();
}

#//$$******************************************************************

void CExportPropertyPage4::SetDisplay()
{
	if ((m_nSaveAsType < 0) || (m_nSaveAsType > 2))
	{
		m_nSaveAsType = 0;
	}

	m_radioReportOutput1.SetCheck(0 == m_nSaveAsType);
	m_radioReportOutput2.SetCheck(1 == m_nSaveAsType);
	m_radioReportOutput3.SetCheck(2 == m_nSaveAsType);

	bool bFilenameAllowed = FALSE;

	switch (m_nSaveAsType)
	{
	case nSAVEAS_CSV:
	case nSAVEAS_HTML:
		bFilenameAllowed = TRUE;
		break;
	}

	m_editFilename.EnableWindow(bFilenameAllowed);

	if (TRUE == bFilenameAllowed)
	{
		m_editFilename.SetFocus();
	}
	else
	{
		m_radioReportOutput1.SetFocus();
	}
}

//$$******************************************************************

void CExportPropertyPage4::OnButtonBrowse()
{
	CString strExt = "";
	CString strFilter = "";

	if (m_nSaveAsType == nSAVEAS_CSV)
	{
		strExt = "csv";
		strFilter = "CSV File (*.csv)|*.csv";
	}
	else
	{
		strExt = "htm";
		strFilter = "HTML File (*.htm)|*.htm";
	}
	strFilter += "|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, strExt, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, strFilter, this);
	dlg.m_ofn.lpstrTitle = "Create additional output file ";
	dlg.m_ofn.lpstrInitialDir = Sysset.GetDataPath();

	if (dlg.DoModal() == IDOK)
	{
		m_editFilename.SetWindowText(dlg.GetPathName());
	}
}

//$$******************************************************************
