//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************
#include "DbExportEditor.h"
#include "SetupExportSalesDlg.h"
#include "SystemPropertyPage5.h"
//$$******************************************************************

CSystemPropertyPage5::CSystemPropertyPage5()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage5::IDD)
{
}

//$$******************************************************************

void CSystemPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_EXPORT_EXIT, m_checkExportExit );
	DDX_Control( pDX, IDC_BUTTON_SETUP_EXIT1, m_buttonSetupExit1 );
	DDX_Control( pDX, IDC_BUTTON_SETUP_EXIT2, m_buttonSetupExit2 );
	DDX_Control( pDX, IDC_BUTTON_SETUP_EXIT3, m_buttonSetupExit3 );
	/*****/
	DDX_Control( pDX, IDC_CHECK_EXPORT_DELETE, m_checkExportDelete );
	DDX_Control( pDX, IDC_BUTTON_SETUP_DELETE, m_buttonSetupDelete );
	/*****/
	DDX_Control( pDX, IDC_CHECK_EXPORT_SALES, m_checkExportSales );
	DDX_Control( pDX, IDC_BUTTON_SETUP_SALES, m_buttonSetupSales );
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage5, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_EXPORT_EXIT, OnToggleExportExit)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_EXIT1, OnButtonSetupExit1)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_EXIT2, OnButtonSetupExit2)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_EXIT3, OnButtonSetupExit3)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_EXPORT_DELETE, OnToggleExportDelete)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_DELETE, OnButtonSetupDelete)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_EXPORT_SALES, OnToggleExportSales)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_SALES, OnButtonSetupSales)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage5::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_checkExportExit.SetCheck( System.GetExportOnExitFlag() );
	m_checkExportDelete.SetCheck( System.GetExportOnDeleteFlag() );
	m_checkExportSales.SetCheck( System.GetEnableExportSalesFlag() );

	SetDisplay();
	SetButtonTexts();
	
	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage5::OnKillActive()
{
	System.SetExportOnExitFlag(IsTicked(m_checkExportExit));
	System.SetExportOnDeleteFlag(IsTicked(m_checkExportDelete));
	System.SetEnableExportSalesFlag(IsTicked(m_checkExportSales));
	
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage5::SetButtonTexts()
{
	CDbExportHandler exportHandler{};

	if (exportHandler.Read("DB1") == FALSE)
	{
		m_buttonSetupExit1.SetWindowText("Setup 1");
	}
	else
	{
		m_buttonSetupExit1.SetWindowText("Modify 1");
	}

	if (exportHandler.Read("DB2") == FALSE)
	{
		m_buttonSetupExit2.SetWindowText("Setup 2");
	}
	else
	{
		m_buttonSetupExit2.SetWindowText("Modify 2");
	}

	if (exportHandler.Read("DB3") == FALSE)
	{
		m_buttonSetupExit3.SetWindowText("Setup 3");
	}
	else
	{
		m_buttonSetupExit3.SetWindowText("Modify 3");
	}

	if (exportHandler.Read("DEL") == FALSE)
	{
		m_buttonSetupDelete.SetWindowText("Setup");
	}
	else
	{
		m_buttonSetupDelete.SetWindowText("Modify");
	}
}

//$$******************************************************************

void CSystemPropertyPage5::SetDisplay()
{
	bool bExit = ( m_checkExportExit.GetCheck() != 0 );
	bool bDelete = ( m_checkExportDelete.GetCheck() != 0 );
	bool bSales = ( m_checkExportSales.GetCheck() != 0 );
	
	m_buttonSetupExit1.EnableWindow(bExit);
	m_buttonSetupExit2.EnableWindow(bExit);
	m_buttonSetupExit3.EnableWindow(bExit);
	m_buttonSetupDelete.EnableWindow(bDelete);
	m_buttonSetupSales.EnableWindow(bSales);
}

//$$******************************************************************

void CSystemPropertyPage5::EditLabel ( const char* szLabel )
{
	CDbExportEditor editor ( this );
	editor.EditExportLabel ( szLabel );
	SetButtonTexts();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage5::OnToggleExportExit()	
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CSystemPropertyPage5::OnButtonSetupExit1()	
{ 
	EditLabel ( "DB1" ); 
}

//$$******************************************************************

void CSystemPropertyPage5::OnButtonSetupExit2()	
{ 
	EditLabel ( "DB2" ); 

}

//$$******************************************************************

void CSystemPropertyPage5::OnButtonSetupExit3()	
{ 
	EditLabel ( "DB3" ); 
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage5::OnToggleExportDelete()	
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CSystemPropertyPage5::OnButtonSetupDelete()	
{	
	EditLabel ( "DEL" ); 
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage5::OnToggleExportSales()	
{
	SetDisplay(); 
}

//$$******************************************************************

void CSystemPropertyPage5::OnButtonSetupSales()	
{ 
	CSetupExportSalesDlg dlg ( this );
	dlg.DoModal();
}

//$$******************************************************************

