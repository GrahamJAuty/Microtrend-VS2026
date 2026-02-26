//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SetupUserTextsDlg.h"
//$$******************************************************************

CSetupUserTextsDlg::CSetupUserTextsDlg( CWnd* pParent ) : CSSDialog(CSetupUserTextsDlg::IDD, pParent)
{
}

//$$******************************************************************

CSetupUserTextsDlg::~CSetupUserTextsDlg()
{
}

//$$******************************************************************

void CSetupUserTextsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EXTREF, m_staticExtRef);
	DDX_Control(pDX, IDC_EDIT_EXTREF, m_editExtRef);
	DDX_Control(pDX, IDC_EDIT_ADD1, m_editAdd[0]);
	DDX_Control(pDX, IDC_EDIT_ADD2, m_editAdd[1]);
	DDX_Control(pDX, IDC_EDIT_ADD3, m_editAdd[2]);
	DDX_Control(pDX, IDC_EDIT_ADD4, m_editAdd[3]);
	DDX_Control(pDX, IDC_EDIT_ADD5, m_editAdd[4]);
	DDX_Control(pDX, IDC_EDIT_TEL1, m_editTel[0]);
	DDX_Control(pDX, IDC_EDIT_TEL2, m_editTel[1]);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo[0]);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo[1]);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo[2]);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo[3]);
	DDX_Control(pDX, IDC_EDIT_DOB, m_editDob);
	DDX_Control(pDX, IDC_CHECK_COMPADD1, m_checkAddCompulsory[0]);
	DDX_Control(pDX, IDC_CHECK_COMPADD2, m_checkAddCompulsory[1]);
	DDX_Control(pDX, IDC_CHECK_COMPADD3, m_checkAddCompulsory[2]);
	DDX_Control(pDX, IDC_CHECK_COMPADD4, m_checkAddCompulsory[3]);
	DDX_Control(pDX, IDC_CHECK_COMPADD5, m_checkAddCompulsory[4]);
	DDX_Control(pDX, IDC_CHECK_COMPTEL1, m_checkTelCompulsory[0]);
	DDX_Control(pDX, IDC_CHECK_COMPTEL2, m_checkTelCompulsory[1]);
	DDX_Control(pDX, IDC_CHECK_COMPINFO1, m_checkInfoCompulsory[0]);
	DDX_Control(pDX, IDC_CHECK_COMPINFO2, m_checkInfoCompulsory[1]);
	DDX_Control(pDX, IDC_CHECK_COMPINFO3, m_checkInfoCompulsory[2]);
	DDX_Control(pDX, IDC_CHECK_COMPINFO4, m_checkInfoCompulsory[3]);
	DDX_Control(pDX, IDC_CHECK_DATEINFO1, m_checkInfoDate[0]);
	DDX_Control(pDX, IDC_CHECK_DATEINFO2, m_checkInfoDate[1]);
	DDX_Control(pDX, IDC_CHECK_DATEINFO3, m_checkInfoDate[2]);
	DDX_Control(pDX, IDC_CHECK_DATEINFO4, m_checkInfoDate[3]);
	DDX_Control(pDX, IDC_CHECK_COMPDOB, m_checkDobCompulsory);
	DDX_Control(pDX, IDC_CHECK_COMPFORENAME, m_checkForenameCompulsory);
	DDX_Control(pDX, IDC_CHECK_COMPSURNAME, m_checkSurnameCompulsory);
	DDX_Control(pDX, IDC_CHECK_COMPUSERNAME, m_checkUsernameCompulsory);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupUserTextsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSetupUserTextsDlg::OnInitDialog()
{
	CUserTexts texts;
	texts.Read();

	CDialog::OnInitDialog();
	
	m_editDob.SetWindowText(texts.GetText(LOYALTYFIELD_DOB, ""));
	m_editExtRef.SetWindowText(texts.GetText(LOYALTYFIELD_EXTERNALREF, ""));

	m_checkDobCompulsory.SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_DOB));
	m_checkForenameCompulsory.SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_FORENAME));
	m_checkSurnameCompulsory.SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_SURNAME));
	m_checkUsernameCompulsory.SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_FULLNAME));
	
	m_editAdd[0].SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS1, ""));
	m_editAdd[1].SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS2, ""));
	m_editAdd[2].SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS3, ""));
	m_editAdd[3].SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS4, ""));
	m_editAdd[4].SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS5, ""));

	m_checkAddCompulsory[0].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_ADDRESS1));
	m_checkAddCompulsory[1].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_ADDRESS2));
	m_checkAddCompulsory[2].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_ADDRESS3));
	m_checkAddCompulsory[3].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_ADDRESS4));
	m_checkAddCompulsory[4].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_ADDRESS5));
	
	m_editTel[0].SetWindowText(texts.GetText(LOYALTYFIELD_PHONE1, ""));
	m_editTel[1].SetWindowText(texts.GetText(LOYALTYFIELD_PHONE2, ""));

	m_checkTelCompulsory[0].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_PHONE1));
	m_checkTelCompulsory[1].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_PHONE2));

	m_editInfo[0].SetWindowText(texts.GetText(LOYALTYFIELD_INFO1, ""));
	m_editInfo[1].SetWindowText(texts.GetText(LOYALTYFIELD_INFO2, ""));
	m_editInfo[2].SetWindowText(texts.GetText(LOYALTYFIELD_INFO3, ""));
	m_editInfo[3].SetWindowText(texts.GetText(LOYALTYFIELD_INFO4, ""));

	m_checkInfoCompulsory[0].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_INFO1));
	m_checkInfoCompulsory[1].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_INFO2));
	m_checkInfoCompulsory[2].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_INFO3));
	m_checkInfoCompulsory[3].SetCheck(texts.GetCompulsoryFlag(LOYALTYFIELD_INFO4));

	m_checkInfoDate[0].SetCheck(texts.GetDateFlag(LOYALTYFIELD_INFO1));
	m_checkInfoDate[1].SetCheck(texts.GetDateFlag(LOYALTYFIELD_INFO2));
	m_checkInfoDate[2].SetCheck(texts.GetDateFlag(LOYALTYFIELD_INFO3));
	m_checkInfoDate[3].SetCheck(texts.GetDateFlag(LOYALTYFIELD_INFO4));

	switch( System.GetInterfaceType() )
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		break;

	default:
		m_staticExtRef.ShowWindow(SW_HIDE);
		m_editExtRef.ShowWindow(SW_HIDE);
		break;
	}
	
	return TRUE;
}

//$$******************************************************************

void CSetupUserTextsDlg::OnButtonDefault()
{
	if ( Prompter.YesNo( "Set user defined database field names to standard default text", "Please confirm" ) == IDYES )
	{
		DeleteFile();
		EndDialog(IDOK);
	}
}

//$$******************************************************************

void CSetupUserTextsDlg::DeleteFile()
{
	::SendToRecycleBin ( Filenames.GetUserTextsFilename() );
}

//$$******************************************************************

void CSetupUserTextsDlg::SaveText( CUserTexts& texts, CEdit& edit, int nField)
{
	CString str = GetEditBoxText(edit);

	if (str != "")
	{
		texts.SetText(nField, str);
	}
}

//$$******************************************************************

void CSetupUserTextsDlg::OnOK()
{
	DeleteFile();

	CUserTexts texts{};
	SaveText(texts, m_editAdd[0], LOYALTYFIELD_ADDRESS1);
	SaveText(texts, m_editAdd[1], LOYALTYFIELD_ADDRESS2);
	SaveText(texts, m_editAdd[2], LOYALTYFIELD_ADDRESS3);
	SaveText(texts, m_editAdd[3], LOYALTYFIELD_ADDRESS4);
	SaveText(texts, m_editAdd[4], LOYALTYFIELD_ADDRESS5);
	SaveText(texts, m_editTel[0], LOYALTYFIELD_PHONE1);
	SaveText(texts, m_editTel[1], LOYALTYFIELD_PHONE2);
	SaveText(texts, m_editInfo[0], LOYALTYFIELD_INFO1);
	SaveText(texts, m_editInfo[1], LOYALTYFIELD_INFO2);
	SaveText(texts, m_editInfo[2], LOYALTYFIELD_INFO3);
	SaveText(texts, m_editInfo[3], LOYALTYFIELD_INFO4);
	SaveText(texts, m_editDob, LOYALTYFIELD_DOB);
	SaveText(texts, m_editExtRef, LOYALTYFIELD_EXTERNALREF);

	texts.SetCompulsoryFlag(LOYALTYFIELD_ADDRESS1, IsTicked(m_checkAddCompulsory[0]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_ADDRESS2, IsTicked(m_checkAddCompulsory[1]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_ADDRESS3, IsTicked(m_checkAddCompulsory[2]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_ADDRESS4, IsTicked(m_checkAddCompulsory[3]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_ADDRESS5, IsTicked(m_checkAddCompulsory[4]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_PHONE1, IsTicked(m_checkTelCompulsory[0]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_PHONE2, IsTicked(m_checkTelCompulsory[1]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_INFO1, IsTicked(m_checkInfoCompulsory[0]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_INFO2, IsTicked(m_checkInfoCompulsory[1]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_INFO3, IsTicked(m_checkInfoCompulsory[2]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_INFO4, IsTicked(m_checkInfoCompulsory[3]));
	texts.SetCompulsoryFlag(LOYALTYFIELD_DOB, IsTicked(m_checkDobCompulsory));
	texts.SetCompulsoryFlag(LOYALTYFIELD_FULLNAME, IsTicked(m_checkUsernameCompulsory));
	texts.SetCompulsoryFlag(LOYALTYFIELD_FORENAME, IsTicked(m_checkForenameCompulsory));
	texts.SetCompulsoryFlag(LOYALTYFIELD_SURNAME, IsTicked(m_checkSurnameCompulsory));

	texts.SetDateFlag(LOYALTYFIELD_INFO1, IsTicked(m_checkInfoDate[0]));
	texts.SetDateFlag(LOYALTYFIELD_INFO2, IsTicked(m_checkInfoDate[1]));
	texts.SetDateFlag(LOYALTYFIELD_INFO3, IsTicked(m_checkInfoDate[2]));
	texts.SetDateFlag(LOYALTYFIELD_INFO4, IsTicked(m_checkInfoDate[3]));

	texts.Write();
	
	EndDialog(IDOK);
}

//$$******************************************************************
