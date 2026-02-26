//$$******************************************************************
#include "DbExportEditor.h"
#include "DbExportHandler.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SystemPropertyPage3.h"
//$$******************************************************************

CSystemPropertyPage3::CSystemPropertyPage3() : CSSPropertyPage(CSystemPropertyPage3::IDD)
{
}

//$$******************************************************************

CSystemPropertyPage3::~CSystemPropertyPage3()
{
}

//$$******************************************************************

void CSystemPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_EXPORTONEXIT, m_checkExportOnExit);
	DDX_Control(pDX, IDC_CHECK_EXPORTONDELETE, m_checkExportOnDelete);
	DDX_Control(pDX, IDC_BUTTON_ONEXIT1, m_buttonOnExit1);
	DDX_Control(pDX, IDC_BUTTON_ONEXIT2, m_buttonOnExit2);
	DDX_Control(pDX, IDC_BUTTON_ONEXIT3, m_buttonOnExit3);
	DDX_Control(pDX, IDC_BUTTON_ONDELETE, m_buttonOnDelete);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage3, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_EXPORTONEXIT, OnToggleExportOnExit)
	ON_BN_CLICKED(IDC_BUTTON_ONEXIT1, OnButtonOnExit1)
	ON_BN_CLICKED(IDC_BUTTON_ONEXIT2, OnButtonOnExit2)
	ON_BN_CLICKED(IDC_BUTTON_ONEXIT3, OnButtonOnExit3)
	ON_BN_CLICKED(IDC_CHECK_EXPORTONDELETE, OnToggleExportOnDelete)
	ON_BN_CLICKED(IDC_BUTTON_ONDELETE, OnButtonOnDelete)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_checkExportOnExit.SetCheck(System.GetExportOnExitFlag());
	m_checkExportOnDelete.SetCheck(System.GetExportOnDeleteFlag());

	OnToggleExportOnExit();
	OnToggleExportOnDelete();
	SetButtonTexts();

	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage3::OnKillActive()
{
	System.SetExportOnExitFlag(IsTicked(m_checkExportOnExit));
	System.SetExportOnDeleteFlag(IsTicked(m_checkExportOnDelete));
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage3::OnToggleExportOnDelete()
{
	m_buttonOnDelete.EnableWindow(IsTicked(m_checkExportOnDelete));
}

//$$******************************************************************

void CSystemPropertyPage3::OnToggleExportOnExit()
{
	bool bEnable = IsTicked(m_checkExportOnExit);
	m_buttonOnExit1.EnableWindow(bEnable);
	m_buttonOnExit2.EnableWindow(bEnable);
	m_buttonOnExit3.EnableWindow(bEnable);
}

//$$******************************************************************

void CSystemPropertyPage3::OnButtonOnExit1()	
{	
	EditLabel ( "DB1" ); 
}

//$$******************************************************************

void CSystemPropertyPage3::OnButtonOnExit2()
{
	EditLabel("DB2");
}

//$$******************************************************************

void CSystemPropertyPage3::OnButtonOnExit3()
{
	EditLabel("DB3");
}

//$$******************************************************************

void CSystemPropertyPage3::OnButtonOnDelete()
{
	EditLabel("DEL");
}

//$$******************************************************************

void CSystemPropertyPage3::EditLabel ( const char* szLabel )
{
	CDbExportEditor editor ( FALSE, this );
	editor.EditLabel ( szLabel );
	SetButtonTexts();
}

//$$******************************************************************

void CSystemPropertyPage3::SetButtonTexts()
{
	CDbExportHandler exportHandler{};

	if (exportHandler.Read("DB1") == FALSE)
	{
		m_buttonOnExit1.SetWindowText("Setup 1");
	}
	else
	{
		m_buttonOnExit1.SetWindowText("Modify 1");
	}

	if (exportHandler.Read("DB2") == FALSE)
	{
		m_buttonOnExit2.SetWindowText("Setup 2");
	}
	else
	{
		m_buttonOnExit2.SetWindowText("Modify 2");
	}

	if (exportHandler.Read("DB3") == FALSE)
	{
		m_buttonOnExit3.SetWindowText("Setup 3");
	}
	else
	{
		m_buttonOnExit3.SetWindowText("Modify 3");
	}

	if (exportHandler.Read("DEL") == FALSE)
	{
		m_buttonOnDelete.SetWindowText("Setup");
	}
	else
	{
		m_buttonOnDelete.SetWindowText("Modify");
	}
}

//$$******************************************************************
