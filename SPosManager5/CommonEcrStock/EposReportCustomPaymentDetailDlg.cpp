/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomPaymentDetailDlg.h"
/**********************************************************************/

CEposReportCustomPaymentDetailDlg::CEposReportCustomPaymentDetailDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CSSDialog(CEposReportCustomPaymentDetailDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
	m_checkPayTypeID[0] = IDC_CHECK_PAYTYPE1;
	m_checkPayTypeID[1] = IDC_CHECK_PAYTYPE2;
	m_checkPayTypeID[2] = IDC_CHECK_PAYTYPE3;
	m_checkPayTypeID[3] = IDC_CHECK_PAYTYPE4;
	m_checkPayTypeID[4] = IDC_CHECK_PAYTYPE5;
	m_checkPayTypeID[5] = IDC_CHECK_PAYTYPE6;
	m_checkPayTypeID[6] = IDC_CHECK_PAYTYPE7;
	m_checkPayTypeID[7] = IDC_CHECK_PAYTYPE8;
	m_checkPayTypeID[8] = IDC_CHECK_PAYTYPE9;
	m_checkPayTypeID[9] = IDC_CHECK_PAYTYPE10;
	m_checkPayTypeID[10] = IDC_CHECK_PAYTYPE11;
	m_checkPayTypeID[11] = IDC_CHECK_PAYTYPE12;
	m_checkPayTypeID[12] = IDC_CHECK_PAYTYPE13;
	m_checkPayTypeID[13] = IDC_CHECK_PAYTYPE14;
	m_checkPayTypeID[14] = IDC_CHECK_PAYTYPE15;
	m_checkPayTypeID[15] = IDC_CHECK_PAYTYPE16;
	m_checkPayTypeID[16] = IDC_CHECK_PAYTYPE17;
	m_checkPayTypeID[17] = IDC_CHECK_PAYTYPE18;
}

/**********************************************************************/

CEposReportCustomPaymentDetailDlg::~CEposReportCustomPaymentDetailDlg()
{
}

/**********************************************************************/

void CEposReportCustomPaymentDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_TABLENO, m_checkTableNo);
	DDX_Control(pDX, IDC_CHECK_TABLENAME, m_checkTableName);
	DDX_Control(pDX, IDC_CHECK_TABLEFILTER, m_checkTableFilter);
	DDX_Control(pDX, IDC_CHECK_PAYNUMFILTER, m_checkPayNumFilter);
	DDX_Control(pDX, IDC_EDIT_PAYNUM, m_editPayNum);
	DDX_Control(pDX, IDC_CHECK_SEQNO, m_checkSEQNo);
	DDX_Control(pDX, IDC_CHECK_SERVER, m_checkServer);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);

	for (int i = 0; i < 18; i++)
	{
		DDX_Control(pDX, m_checkPayTypeID[i], m_checkPayType[i]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomPaymentDetailDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE,OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_TABLEFILTER, OnToggleTableFilter)
	ON_BN_CLICKED(IDC_CHECK_PAYNUMFILTER, OnTogglePayNumFilter)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomPaymentDetailDlg::OnInitDialog()
{
	CEposReportCustomSettingsPaymentDetail Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine(strParams);

	CDialog::OnInitDialog();

	SubclassEdit(IDC_EDIT_TABLEFROM, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_TABLETO, SS_NUM, 3, "%d");
	m_editName.LimitText(EPOS_CUSTOM_MAXLEN_NAME);
	m_editPayNum.LimitText(EPOS_CUSTOM_MAXLEN_PAYNOFILTER);

	m_editName.SetWindowText(m_infoReport.GetCustomReportName());

	m_checkAdhoc.SetCheck(Settings.GetAllowAdhocFlag());

	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenu);
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenuTexts);

	m_checkTableNo.SetCheck(Settings.GetShowTableNoFlag());
	m_checkTableName.SetCheck(Settings.GetShowTableNameFlag());
	m_checkTableFilter.SetCheck(Settings.GetTableFilterFlag());
	m_checkPayNumFilter.SetCheck(Settings.GetPayNumFilterFlag());
	m_checkSEQNo.SetCheck(Settings.GetShowSEQNoFlag());
	m_checkServer.SetCheck(Settings.GetShowServerFlag());

	SetEditBoxInt(*GetEditTableFrom(), Settings.GetTableNoFrom());
	SetEditBoxInt(*GetEditTableTo(), Settings.GetTableNoTo());
	m_editPayNum.SetWindowText(Settings.GetPayNumFilter());

	OnToggleTableFilter();
	OnTogglePayNumFilter();

	{
		int nMask = 1;
		int nFilter = Settings.GetPaymentTypeFilter();

		for (int i = 0; i < 18; i++)
		{
			m_checkPayType[i].SetCheck((nFilter & nMask) != 0);
			nMask <<= 1;
		}
	}

	if (TRUE == m_bAdhoc)
	{
		m_editName.EnableWindow(FALSE);
		m_checkAdhoc.ShowWindow(SW_HIDE);
		m_checkAdhoc.EnableWindow(FALSE);
		SetWindowText("Adhoc Payment Detail Report Settings");
	}
	else
	{
		SetWindowText("Setup Custom Payment Detail Report");
	}

	if ((FALSE == m_bAdhoc) || (FALSE == bEnableSave))
	{
		m_buttonSave.ShowWindow(SW_HIDE);
		m_buttonSave.EnableWindow(FALSE);
	}

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomPaymentDetailDlg::OnToggleTableFilter()
{
	bool bEnable = IsTicked(m_checkTableFilter);
	GetEditTableFrom()->EnableWindow(bEnable);
	GetEditTableTo()->EnableWindow(bEnable);
}

/**********************************************************************/

void CEposReportCustomPaymentDetailDlg::OnTogglePayNumFilter()
{
	m_editPayNum.EnableWindow(IsTicked(m_checkPayNumFilter));
}

/**********************************************************************/

bool CEposReportCustomPaymentDetailDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsPaymentDetail Settings;
	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_PAYMENTDETAIL );

	{
		CString strText;
		m_editName.GetWindowText( strText );
		infoReport.SetCustomReportName( strText );
	}

	Settings.SetShowTableNoFlag(IsTicked(m_checkTableNo));
	Settings.SetShowTableNameFlag(IsTicked(m_checkTableName));
	Settings.SetTableFilterFlag(IsTicked(m_checkTableFilter));

	if (Settings.GetTableFilterFlag() == TRUE)
	{
		int nTableNoFrom = GetEditBoxInt(*GetEditTableFrom());
		int nTableNoTo = GetEditBoxInt(*GetEditTableTo());

		if (nTableNoFrom > nTableNoTo)
		{
			int nTemp = nTableNoFrom;
			nTableNoFrom = nTableNoTo;
			nTableNoTo = nTemp;
		}

		Settings.SetTableNoFrom(nTableNoFrom);
		Settings.SetTableNoTo(nTableNoTo);	
	}
	else
	{
		Settings.SetTableNoFrom(0);
		Settings.SetTableNoTo(999);
	}

	Settings.SetPayNumFilterFlag(IsTicked(m_checkPayNumFilter));
	Settings.SetPayNumFilter(GetEditBoxText(m_editPayNum));
	
	int nFilter = 0;
	{
		int nMask = 1;
		for (int i = 0; i < 18; i++)
		{
			if (m_checkPayType[i].GetCheck() != 0)
			{
				nFilter |= nMask;
			}
			nMask <<= 1;
		}
	}
	Settings.SetPaymentTypeFilter(nFilter);

	Settings.SetShowSEQNoFlag(IsTicked(m_checkSEQNo));
	Settings.SetShowServerFlag(IsTicked(m_checkServer));

	CString strLine = "";
	Settings.GetSettingsCSVLine( strLine );
	infoReport.SetCustomReportParams( strLine );
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomPaymentDetailDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomPaymentDetailDlg::OnOK()
{
	if (GetUpdatedSettings(m_infoReport) == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/
