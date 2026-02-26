//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//*******************************************************************
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
//*******************************************************************
#include "SetupPhotoIDDlg.h"
//*******************************************************************

CSetupPhotoIDDlg::CSetupPhotoIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupPhotoIDDlg::IDD, pParent)
{
	m_strLabelName	= m_data.m_strLabelName;		// default label  read when class constructed
	SetLabelData();									// set field data

	m_nField1Opt = 0;
	m_nField2Opt = 0;
	m_nField3Opt = 0;
	m_nField4Opt = 0;
	m_nField5Opt = 0;
	m_nField6Opt = 0;
}

//*******************************************************************

void CSetupPhotoIDDlg::SetLabelData()
{
	m_strField1 = m_data.m_strField1;
	m_strField2 = m_data.m_strField2;
	m_strField3 = m_data.m_strField3;
	m_strField4 = m_data.m_strField4;
	m_strField5 = m_data.m_strField5;
	m_strField6 = m_data.m_strField6;
	m_strFieldText1 = m_data.m_strFieldText1;
	m_strFieldText2 = m_data.m_strFieldText2;
	m_strFieldText3 = m_data.m_strFieldText3;
	m_strFieldText4 = m_data.m_strFieldText4;
	m_strFieldText5 = m_data.m_strFieldText5;
	m_strFieldText6 = m_data.m_strFieldText6;
}

//*******************************************************************

void CSetupPhotoIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LABELS, m_comboLabel);
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboField1);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboField2);
	DDX_Control(pDX, IDC_COMBO_FIELD3, m_comboField3);
	DDX_Control(pDX, IDC_COMBO_FIELD4, m_comboField4);
	DDX_Control(pDX, IDC_COMBO_FIELD5, m_comboField5);
	DDX_Control(pDX, IDC_COMBO_FIELD6, m_comboField6);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT1, m_strFieldText1);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT2, m_strFieldText2);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT3, m_strFieldText3);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT4, m_strFieldText4);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT5, m_strFieldText5);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT6, m_strFieldText6);
	DDX_CBString(pDX, IDC_COMBO_FIELD1, m_strField1);
	DDX_CBString(pDX, IDC_COMBO_FIELD2, m_strField2);
	DDX_CBString(pDX, IDC_COMBO_FIELD3, m_strField3);
	DDX_CBString(pDX, IDC_COMBO_FIELD4, m_strField4);
	DDX_CBString(pDX, IDC_COMBO_FIELD5, m_strField5);
	DDX_CBString(pDX, IDC_COMBO_FIELD6, m_strField6);
	DDX_CBString(pDX, IDC_COMBO_LABELS, m_strLabelName);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupPhotoIDDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_CBN_SELCHANGE(IDC_COMBO_LABELS, OnSelchangeComboLabels)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSetupPhotoIDDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_data.CompileLabelCombo ( &m_comboLabel );			// full barman label list

	int n = 0;
	AddFieldName ( n++, Account::Blank.Label );
	AddFieldName ( n++, Account::Text.Label );
	AddFieldName ( n++, Account::UserID.Label );

	if (System.GetInterfaceType() == nINTERFACE_SWIPEv2)		// prefix + number + c 
	{
		AddFieldName(n++, Account::FullUserID.Label);
	}

	AddFieldName ( n++, Account::Fullname.Label );

	AddFieldName ( n++, Account::GroupNo.Label );
	AddFieldName ( n++, Account::GroupUserID.Label );

	if (System.GetInterfaceType() == nINTERFACE_SWIPEv2)		// prefix + number + c 
	{
		AddFieldName(n++, Account::GroupFullUserID.Label);
	}

	AddFieldName ( n++, Account::ExpiryDate.Label );
	AddFieldName ( n++, Account::DOB.Label );
	AddFieldName ( n++, Account::Address1.Label );
	AddFieldName ( n++, Account::Address2.Label );
	AddFieldName ( n++, Account::Address3.Label );
	AddFieldName ( n++, Account::Address4.Label );
	AddFieldName ( n++, Account::Address5.Label );
	AddFieldName ( n++, Account::Phone1.Label );
	AddFieldName ( n++, Account::Phone2.Label );
	AddFieldName ( n++, Account::Info1.Label );
	AddFieldName ( n++, Account::Info2.Label );
	AddFieldName ( n++, Account::Info3.Label );
	AddFieldName ( n++, Account::Info4.Label );

	m_comboField1.SetCurSel ( m_nField1Opt );
	m_comboField2.SetCurSel ( m_nField2Opt );
	m_comboField3.SetCurSel ( m_nField3Opt );
	m_comboField4.SetCurSel ( m_nField4Opt );
	m_comboField5.SetCurSel ( m_nField5Opt );
	m_comboField6.SetCurSel ( m_nField6Opt );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//*******************************************************************

void CSetupPhotoIDDlg::AddFieldName ( int nOpt, const char* szText ) 
{
	m_comboField1.AddString ( szText );
	m_comboField2.AddString ( szText );
	m_comboField3.AddString ( szText );
	m_comboField4.AddString ( szText );
	m_comboField5.AddString ( szText );
	m_comboField6.AddString ( szText );
	if ( m_data.m_strField1 == szText )		m_nField1Opt = nOpt;
	if ( m_data.m_strField2 == szText )		m_nField2Opt = nOpt;
	if ( m_data.m_strField3 == szText )		m_nField3Opt = nOpt;
	if ( m_data.m_strField4 == szText )		m_nField4Opt = nOpt;
	if ( m_data.m_strField5 == szText )		m_nField5Opt = nOpt;
	if ( m_data.m_strField6 == szText )		m_nField6Opt = nOpt;
}

//***********************************************************************

void CSetupPhotoIDDlg::OnSelchangeComboLabels() 
{
	if ( UpdateData() == TRUE )
	{
		m_data.Load ( m_strLabelName );
		SetLabelData();
		UpdateData(FALSE);
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnBnClickedButtonClear()
{
	if ( UpdateData() == TRUE )
	{
		if ( Prompter.YesNo( "Are you sure you wish to clear these label settings", "Please confirm" ) == IDYES )
		{
			m_data.RemoveLabel ( m_strLabelName );			// remove from label data file
			m_data.Load (  m_strLabelName );				// load none existant file 
			SetLabelData();									// set data to default values
			UpdateData(FALSE);
		}
	}
}

//**********************************************************************

bool CSetupPhotoIDDlg::Save() 
{
	if ( UpdateData() == TRUE )
	{
		m_data.m_strLabelName		= m_strLabelName;
		m_data.m_strField1			= m_strField1;
		m_data.m_strField2			= m_strField2;
		m_data.m_strField3			= m_strField3;
		m_data.m_strField4			= m_strField4;
		m_data.m_strField5			= m_strField5;
		m_data.m_strField6			= m_strField6;
		m_data.m_strFieldText1		= m_strFieldText1;
		m_data.m_strFieldText2		= m_strFieldText2;
		m_data.m_strFieldText3		= m_strFieldText3;
		m_data.m_strFieldText4		= m_strFieldText4;
		m_data.m_strFieldText5		= m_strFieldText5;
		m_data.m_strFieldText6		= m_strFieldText6;
		return m_data.SaveDefault();
	}

	return FALSE;
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonSave()
{
	if (Save() == TRUE)
	{
		CDialog::OnOK();
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonTest()
{
	if (Save() == TRUE)
	{
		CString strFilename;
		strFilename = Filenames.GetReportFilename("R999");		// temp filename

		CSSFile fileOut;
		if (fileOut.Open(strFilename, "wb") == TRUE)
		{
			for (int i = 0; i < 20; i++)
			{
				fileOut.WriteLine(m_data.GetTestReportData());
			}

			fileOut.Close();

			CRepmanHandler repman("PhotoID", this);
			repman.ShowBarcode(strFilename, m_data.m_strLabelName);

			m_data.CompileLabelCombo(&m_comboLabel);
		}
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonSetup() 
{
	if ( UpdateData() == TRUE )
	{
		CRepmanHandler repman ( "PhotoID", this );
		repman.ShowBarcode ( "SETUP", m_data.m_strLabelName );
		m_data.CompileLabelCombo ( &m_comboLabel );
	}
}

//**********************************************************************


