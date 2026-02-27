//$$******************************************************************
// used by:-
//			ParentMail - sends transactions & EOD if required ( no send balances)
//			Schoolcomms - sends transactions followed by balances 
//			Tucasi - send Atc purchases followed by balances
//			ParentPay - doesn't send transactions ( yet ) 
//			sQuid - doesn't do End of Day
//			Buckingham - doesn't do End of Day
//
//$$******************************************************************
#include "SetupWebPaymentOptionsDlg.h"
//$$******************************************************************

CSetupWebPaymentOptionsDlg::CSetupWebPaymentOptionsDlg(int nWebPaymentType, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupWebPaymentOptionsDlg::IDD, pParent)
{
	m_nWebPaymentType = nWebPaymentType;
	m_options.LoadRow(NULL);
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_ACCOUNT1, m_staticAccount1);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT2, m_staticAccount2);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT3, m_staticAccount3);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT4, m_staticAccount4);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT5, m_staticAccount5);
	DDX_Control(pDX, IDC_CHECK_ACC_MON, m_checkAccountMon);
	DDX_Control(pDX, IDC_CHECK_ACC_TUE, m_checkAccountTue);
	DDX_Control(pDX, IDC_CHECK_ACC_WED, m_checkAccountWed);
	DDX_Control(pDX, IDC_CHECK_ACC_THU, m_checkAccountThu);
	DDX_Control(pDX, IDC_CHECK_ACC_FRI, m_checkAccountFri);
	DDX_Control(pDX, IDC_CHECK_ACC_SAT, m_checkAccountSat);
	DDX_Control(pDX, IDC_CHECK_ACC_SUN, m_checkAccountSun);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ACC1, m_dtcAccount1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ACC2, m_dtcAccount2);	
	DDX_Control(pDX, IDC_EDIT_DATE4, m_editDate4);
	DDX_Control(pDX, IDC_EDIT_TIME4, m_editTime4);
	/*****/
	DDX_Control(pDX, IDC_STATIC_IMPORT1, m_staticImport1);
	DDX_Control(pDX, IDC_STATIC_IMPORT2, m_staticImport2);
	DDX_Control(pDX, IDC_STATIC_IMPORT3, m_staticImport3);
	DDX_Control(pDX, IDC_STATIC_IMPORT4, m_staticImport4);
	DDX_Control(pDX, IDC_STATIC_IMPORT5, m_staticImport5);
	DDX_Control(pDX, IDC_STATIC_IMPORT6, m_staticImport6);
	DDX_Control(pDX, IDC_STATIC_IMPORT7, m_staticImport7);
	DDX_Control(pDX, IDC_STATIC_IMPORT8, m_staticImport8);
	DDX_Control(pDX, IDC_STATIC_IMPORT9, m_staticImport9);
	DDX_Control(pDX, IDC_CHECK_IMP_MON, m_checkImportMon);
	DDX_Control(pDX, IDC_CHECK_IMP_TUE, m_checkImportTue);
	DDX_Control(pDX, IDC_CHECK_IMP_WED, m_checkImportWed);
	DDX_Control(pDX, IDC_CHECK_IMP_THU, m_checkImportThu);
	DDX_Control(pDX, IDC_CHECK_IMP_FRI, m_checkImportFri);
	DDX_Control(pDX, IDC_CHECK_IMP_SAT, m_checkImportSat);
	DDX_Control(pDX, IDC_CHECK_IMP_SUN, m_checkImportSun);
	DDX_Control(pDX, IDC_EDIT_DATE1, m_editDate1);
	DDX_Control(pDX, IDC_EDIT_TIME1, m_editTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcTime1);			// payments start
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtcTime2);			// payments end
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_dtcTime5);			// exclude start
	DDX_Control(pDX, IDC_DATETIMEPICKER6, m_dtcTime6);			// exclude end
	DDX_Control(pDX, IDC_DATETIMEPICKER7, m_dtcTime7);			// exclude2 start
	DDX_Control(pDX, IDC_DATETIMEPICKER8, m_dtcTime8);			// exclude2 end
	DDX_Control(pDX, IDC_DATETIMEPICKER9, m_dtcTime9);			// exclude3 start
	DDX_Control(pDX, IDC_DATETIMEPICKER10, m_dtcTime10);		// exclude3 end
	DDX_Control(pDX, IDC_DATETIMEPICKER11, m_dtcTime11);		// exclude4 start
	DDX_Control(pDX, IDC_DATETIMEPICKER12, m_dtcTime12);		// exclude4 end
	DDX_Control(pDX, IDC_CHECK_EXCLUDE1, m_checkExclude1Reqd);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE2, m_checkExclude2Reqd);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE3, m_checkExclude3Reqd);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE4, m_checkExclude4Reqd);
	/*****/
	DDX_Control(pDX, IDC_STATIC_EXPORTS1, m_staticExports1);
	DDX_Control(pDX, IDC_STATIC_EXPORTS2, m_staticExports2);
	DDX_Control(pDX, IDC_STATIC_EXPORTS3, m_staticExports3);
	DDX_Control(pDX, IDC_STATIC_EXPORTS4, m_staticExports4);
	DDX_Control(pDX, IDC_STATIC_EXPORTS5, m_staticExports5);
	DDX_Control(pDX, IDC_STATIC_EXPORTS6, m_staticExports6);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dtcTime3);			// End Of Day ( transactions )p
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dtcTime4);			// exclude updates start
	DDX_Control(pDX, IDC_DATETIMEPICKER13, m_dtcTime13);		// exclude update end
	DDX_Control(pDX, IDC_EDIT_DATE2, m_editDate2);
	DDX_Control(pDX, IDC_EDIT_DATE3, m_editDate3);
	DDX_Control(pDX, IDC_EDIT_TIME2, m_editTime2);
	DDX_Control(pDX, IDC_EDIT_TIME3, m_editTime3);
	DDX_Control(pDX, IDC_CHECK_EODAFTERPAYMENTS, m_checkEODAfterPayments);
	/*****/
	DDX_Control(pDX, IDC_CHECK_PROMPTBEFOREACCESS, m_checkPromptBeforeAccess);
	DDX_Control(pDX, IDC_CHECK_IMPORTACCOUNTS, m_checkImportAccounts);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupWebPaymentOptionsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE1, OnToggleExclude)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE2, OnToggleExclude2)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE3, OnToggleExclude3)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE4, OnToggleExclude4)
	ON_STN_CLICKED(IDC_STATIC_EXPORTS5, OnClickStaticLastEndOfDay)
	ON_STN_CLICKED(IDC_STATIC_EXPORTS6, OnClickStaticLastBalance)
END_MESSAGE_MAP()

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::SetTimeControl(CDateTimeCtrl& ctrl, CString strTime)
{
	CSSTime time;
	time.SetTime(strTime);
	COleDateTime oleTime = COleDateTime(2024, 1, 1, time.GetHour(), time.GetMinute(), 0);
	ctrl.SetTime(oleTime);
}

//$$******************************************************************

BOOL CSetupWebPaymentOptionsDlg::CSSAutoShutdownPostInitDialog()
{
	if (m_options.m_Row.AccountLinkAllowed(m_nWebPaymentType) == TRUE)
	{
		m_dtcAccount1.SetFormat("HH:mm");
		m_dtcAccount2.SetFormat("HH:mm");
		SetTimeControl(m_dtcAccount1, m_options.m_Row.GetAccountLinkStart());
		SetTimeControl(m_dtcAccount2, m_options.m_Row.GetAccountLinkEnd());
		m_checkAccountMon.SetCheck(m_options.m_Row.IsAccountLinkDay(1));
		m_checkAccountTue.SetCheck(m_options.m_Row.IsAccountLinkDay(2));
		m_checkAccountWed.SetCheck(m_options.m_Row.IsAccountLinkDay(3));
		m_checkAccountThu.SetCheck(m_options.m_Row.IsAccountLinkDay(4));
		m_checkAccountFri.SetCheck(m_options.m_Row.IsAccountLinkDay(5));
		m_checkAccountSat.SetCheck(m_options.m_Row.IsAccountLinkDay(6));
		m_checkAccountSun.SetCheck(m_options.m_Row.IsAccountLinkDay(0));
		m_editDate4.SetWindowText(m_options.m_Row.GetAccountLinkLastDate());
		m_editTime4.SetWindowText(m_options.m_Row.GetAccountLinkLastTime());
	}
	else
	{
		ShowAndEnableWindow(&m_staticAccount1, FALSE);
		ShowAndEnableWindow(&m_staticAccount2, FALSE);
		ShowAndEnableWindow(&m_staticAccount3, FALSE);
		ShowAndEnableWindow(&m_staticAccount4, FALSE);
		ShowAndEnableWindow(&m_staticAccount5, FALSE);
		ShowAndEnableWindow(&m_checkAccountMon, FALSE);
		ShowAndEnableWindow(&m_checkAccountTue, FALSE);
		ShowAndEnableWindow(&m_checkAccountWed, FALSE);
		ShowAndEnableWindow(&m_checkAccountThu, FALSE);
		ShowAndEnableWindow(&m_checkAccountFri, FALSE);
		ShowAndEnableWindow(&m_checkAccountSat, FALSE);
		ShowAndEnableWindow(&m_checkAccountSun, FALSE);
		ShowAndEnableWindow(&m_dtcAccount1, FALSE);
		ShowAndEnableWindow(&m_dtcAccount2, FALSE);
		ShowAndEnableWindow(&m_editDate4, FALSE);
		ShowAndEnableWindow(&m_editTime4, FALSE);
		/*****/
		MoveControl(&m_staticImport1, 7, 7);
		MoveControl(&m_staticImport2, 18, 24);
		MoveControl(&m_staticImport3, 76, 45);
		MoveControl(&m_staticImport4, 172, 45);
		MoveControl(&m_staticImport5, 249, 45);
		MoveControl(&m_staticImport6, 172, 67);
		MoveControl(&m_staticImport7, 340, 67);
		MoveControl(&m_staticImport8, 172, 88);
		MoveControl(&m_staticImport9, 340, 88);
		MoveControl(&m_checkImportMon, 130, 24);
		MoveControl(&m_checkImportTue, 169, 24);
		MoveControl(&m_checkImportWed, 208, 24);
		MoveControl(&m_checkImportThu, 247, 24);
		MoveControl(&m_checkImportFri, 286, 24);
		MoveControl(&m_checkImportSat, 325, 24);
		MoveControl(&m_checkImportSun, 364, 24);
		MoveControl(&m_dtcTime1, 130, 42);
		MoveControl(&m_dtcTime2, 190, 42);
		MoveControl(&m_editDate1, 299, 42);
		MoveControl(&m_editTime1, 337, 42);
		MoveControl(&m_dtcTime5, 130, 64);
		MoveControl(&m_dtcTime6, 190, 64);
		MoveControl(&m_dtcTime7, 130, 85);
		MoveControl(&m_dtcTime8, 190, 85);
		MoveControl(&m_dtcTime9, 298, 64);
		MoveControl(&m_dtcTime10, 358, 64);
		MoveControl(&m_dtcTime11, 298, 85);
		MoveControl(&m_dtcTime12, 358, 85);
		MoveControl(&m_checkExclude1Reqd, 76, 64);
		MoveControl(&m_checkExclude2Reqd, 76, 85);
		MoveControl(&m_checkExclude3Reqd, 248, 64);
		MoveControl(&m_checkExclude4Reqd, 248, 85);
		/*****/
		MoveControl(&m_staticExports1, 8, 125);
		MoveControl(&m_staticExports2, 18, 141);
		MoveControl(&m_staticExports3, 212, 141);
		MoveControl(&m_staticExports4, 18, 162);
		MoveControl(&m_staticExports5, 223, 162);
		MoveControl(&m_staticExports6, 223, 182);
		MoveControl(&m_dtcTime3, 167, 158);
		MoveControl(&m_dtcTime4, 167, 138);
		MoveControl(&m_dtcTime13, 230, 138);
		MoveControl(&m_editDate2, 343, 158);
		MoveControl(&m_editDate3, 343, 179);
		MoveControl(&m_editTime2, 302, 158);
		MoveControl(&m_editTime3, 302, 179);
		MoveControl(&m_checkEODAfterPayments, 18, 181);
		/*****/
		MoveControl(&m_checkImportAccounts, 18, 208);
		MoveControl(&m_checkPromptBeforeAccess, 18, 226);
		MoveControl(&m_buttonOK, 297, 229);
		MoveControl(&m_buttonCancel, 356, 229);
		ResizeDialog(413, 250);
	}

	SetTimeControl(m_dtcTime1, m_options.m_Row.GetPaymentStart());
	SetTimeControl(m_dtcTime2, m_options.m_Row.GetPaymentEnd());
	SetTimeControl(m_dtcTime3, m_options.m_Row.GetEODAfterTime());
	SetTimeControl(m_dtcTime4, m_options.m_Row.GetExcludeUploadStart());
	SetTimeControl(m_dtcTime5, m_options.m_Row.GetExclude1Start());
	SetTimeControl(m_dtcTime6, m_options.m_Row.GetExclude1End());
	SetTimeControl(m_dtcTime7, m_options.m_Row.GetExclude2Start());
	SetTimeControl(m_dtcTime8, m_options.m_Row.GetExclude2End());
	SetTimeControl(m_dtcTime9, m_options.m_Row.GetExclude3Start());
	SetTimeControl(m_dtcTime10, m_options.m_Row.GetExclude3End());
	SetTimeControl(m_dtcTime11, m_options.m_Row.GetExclude4Start());
	SetTimeControl(m_dtcTime12, m_options.m_Row.GetExclude4End());
	SetTimeControl(m_dtcTime13, m_options.m_Row.GetExcludeUploadEnd());

	m_editDate1.SetWindowText(m_options.m_Row.GetPaymentLastDate());		// last payments
	m_editTime1.SetWindowText(m_options.m_Row.GetPaymentLastTime());
	m_editDate2.SetWindowText(m_options.m_Row.GetTransactionLastDate());	// last End of day ( transactions )
	m_editTime2.SetWindowText(m_options.m_Row.GetTransactionLastTime());
	m_editDate3.SetWindowText(m_options.m_Row.GetBalanceLastDate());		// last balance
	m_editTime3.SetWindowText(m_options.m_Row.GetBalanceLastTime());

	m_checkImportMon.SetCheck(m_options.m_Row.IsPaymentDay(1));
	m_checkImportTue.SetCheck(m_options.m_Row.IsPaymentDay(2));
	m_checkImportWed.SetCheck(m_options.m_Row.IsPaymentDay(3));
	m_checkImportThu.SetCheck(m_options.m_Row.IsPaymentDay(4));
	m_checkImportFri.SetCheck(m_options.m_Row.IsPaymentDay(5));
	m_checkImportSat.SetCheck(m_options.m_Row.IsPaymentDay(6));
	m_checkImportSun.SetCheck(m_options.m_Row.IsPaymentDay(0));

	m_checkExclude1Reqd.SetCheck(m_options.m_Row.GetExclude1Flag());
	m_checkExclude2Reqd.SetCheck(m_options.m_Row.GetExclude2Flag());
	m_checkExclude3Reqd.SetCheck(m_options.m_Row.GetExclude3Flag());
	m_checkExclude4Reqd.SetCheck(m_options.m_Row.GetExclude4Flag());

	m_checkPromptBeforeAccess.SetCheck(m_options.m_Row.GetPromptBeforeAccessFlag());
	m_checkImportAccounts.SetCheck(m_options.m_Row.GetImportAccountsFlag());
	m_checkEODAfterPayments.SetCheck((m_nWebPaymentType == nWEBPAYMENT_PARENTMAIL) ? TRUE : m_options.m_Row.GetEODAfterPaymentFlag());

	m_dtcTime1.SetFormat("HH:mm");
	m_dtcTime2.SetFormat("HH:mm");
	m_dtcTime3.SetFormat("HH:mm");
	m_dtcTime4.SetFormat("HH:mm");
	m_dtcTime5.SetFormat("HH:mm");
	m_dtcTime6.SetFormat("HH:mm");
	m_dtcTime7.SetFormat("HH:mm");
	m_dtcTime8.SetFormat("HH:mm");
	m_dtcTime9.SetFormat("HH:mm");
	m_dtcTime10.SetFormat("HH:mm");
	m_dtcTime11.SetFormat("HH:mm");
	m_dtcTime12.SetFormat("HH:mm");
	m_dtcTime13.SetFormat("HH:mm");

	m_dtcTime5.EnableWindow(m_options.m_Row.GetExclude1Flag());			// exclude start
	m_dtcTime6.EnableWindow(m_options.m_Row.GetExclude1Flag());			// exclude end
	m_dtcTime7.EnableWindow(m_options.m_Row.GetExclude2Flag());			// exclude2 start
	m_dtcTime8.EnableWindow(m_options.m_Row.GetExclude2Flag());			// exclude2 end
	m_dtcTime9.EnableWindow(m_options.m_Row.GetExclude3Flag());			// exclude3 start
	m_dtcTime10.EnableWindow(m_options.m_Row.GetExclude3Flag());		// exclude3 end
	m_dtcTime11.EnableWindow(m_options.m_Row.GetExclude4Flag());		// exclude4 end
	m_dtcTime12.EnableWindow(m_options.m_Row.GetExclude4Flag());		// exclude4 end

	CString strText = "";
	strText.Format("Enable importing of account details from %s",
		(const char*)System.GetWebPaymentDisplayName(m_nWebPaymentType));

	m_checkImportAccounts.SetWindowText(strText);
	m_checkImportAccounts.EnableWindow(m_options.m_Row.ImportAccountsService());		// see if have relevant web service available

	int nShowEODAfterPayment = SW_SHOW;

	if ( nWEBPAYMENT_PARENTPAY == m_nWebPaymentType)
	{
		m_dtcTime4.EnableWindow(FALSE);									// cannot stop parentpay updates
		m_dtcTime13.EnableWindow(FALSE);								// 
		m_staticExports5.ShowWindow(SW_HIDE);
		m_editDate2.ShowWindow(SW_HIDE);
		m_editTime2.ShowWindow(SW_HIDE);
	}

	if (m_options.m_Row.EODAllowed(m_nWebPaymentType) == FALSE)				//16/03/2016
	{
		m_staticExports4.ShowWindow(SW_HIDE);
		m_dtcTime3.ShowWindow(SW_HIDE);

		if (m_nWebPaymentType != nWEBPAYMENT_SCHOOLCOMMS)
		{
			m_staticExports6.ShowWindow(SW_HIDE);
			m_editDate3.ShowWindow(SW_HIDE);
			m_editTime3.ShowWindow(SW_HIDE);
		}
		nShowEODAfterPayment = SW_HIDE;
	}
	else
	{
		if (nWEBPAYMENT_PARENTMAIL == m_nWebPaymentType)
		{
			nShowEODAfterPayment = SW_HIDE;
		}
		else
		{
			m_staticExports4.SetWindowText("Allow End of Day balances to be sent after");
		}
	}

	m_checkEODAfterPayments.ShowWindow(nShowEODAfterPayment);

	return TRUE;
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnClickStaticLastEndOfDay()
{
	CString strDate2 = GetEditBoxText(m_editDate2);

	if (strDate2 == "")
	{
		m_options.m_Row.SetLastTransactionDone();
		m_editDate2.SetWindowText(m_options.m_Row.GetTransactionLastDate());
		m_editTime2.SetWindowText(m_options.m_Row.GetTransactionLastTime());
	}
	else
	{
		m_editDate2.SetWindowText("");
		m_editTime2.SetWindowText("");
		m_options.m_Row.ClearLastTransactionDone();
	}
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnClickStaticLastBalance()
{
	CString strDate3 = GetEditBoxText(m_editDate2);

	if (strDate3 == "")
	{
		m_options.m_Row.SetLastBalanceDone();
		m_editDate3.SetWindowText(m_options.m_Row.GetBalanceLastDate());
		m_editTime3.SetWindowText(m_options.m_Row.GetBalanceLastTime());
	}
	else
	{
		m_editDate3.SetWindowText("");
		m_editTime3.SetWindowText("");
		m_options.m_Row.ClearLastBalanceDone();
	}
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnToggleExclude()
{
	SetDisplay();
	m_dtcTime5.SetFocus();
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnToggleExclude2()
{
	SetDisplay();
	m_dtcTime7.SetFocus();
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnToggleExclude3()
{
	SetDisplay();
	m_dtcTime9.SetFocus();
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnToggleExclude4()
{
	SetDisplay();
	m_dtcTime11.SetFocus();
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::SetDisplay()
{
	m_dtcTime5.EnableWindow(IsTicked(m_checkExclude1Reqd));			// exclude start
	m_dtcTime6.EnableWindow(IsTicked(m_checkExclude1Reqd));			// exclude end
	m_dtcTime7.EnableWindow(IsTicked(m_checkExclude2Reqd));			// exclude2 start
	m_dtcTime8.EnableWindow(IsTicked(m_checkExclude2Reqd));			// exclude2 end
	m_dtcTime9.EnableWindow(IsTicked(m_checkExclude3Reqd));			// exclude3 start
	m_dtcTime10.EnableWindow(IsTicked(m_checkExclude3Reqd));		// exclude3 end
	m_dtcTime11.EnableWindow(IsTicked(m_checkExclude4Reqd));		// exclude3 end
	m_dtcTime12.EnableWindow(IsTicked(m_checkExclude4Reqd));		// exclude3 end
}

//$$******************************************************************

void CSetupWebPaymentOptionsDlg::OnOK()
{
	COleDateTime time;

	m_dtcTime1.GetTime(time);
	m_options.m_Row.SetPaymentStart(time.Format("%H:%M"));

	m_dtcTime2.GetTime(time);
	m_options.m_Row.SetPaymentEnd(time.Format("%H:%M"));

	m_dtcTime3.GetTime(time);
	m_options.m_Row.SetEODAfterTime(time.Format("%H:%M"));

	m_dtcTime5.GetTime(time);
	m_options.m_Row.SetExclude1Start(time.Format("%H:%M"));
	m_dtcTime6.GetTime(time);
	m_options.m_Row.SetExclude1End(time.Format("%H:%M"));

	m_dtcTime7.GetTime(time);
	m_options.m_Row.SetExclude2Start(time.Format("%H:%M"));
	m_dtcTime8.GetTime(time);
	m_options.m_Row.SetExclude2End(time.Format("%H:%M"));

	m_dtcTime9.GetTime(time);
	m_options.m_Row.SetExclude3Start(time.Format("%H:%M"));
	m_dtcTime10.GetTime(time);
	m_options.m_Row.SetExclude3End(time.Format("%H:%M"));

	m_dtcTime11.GetTime(time);
	m_options.m_Row.SetExclude4Start(time.Format("%H:%M"));
	m_dtcTime12.GetTime(time);
	m_options.m_Row.SetExclude4End(time.Format("%H:%M"));

	m_dtcTime4.GetTime(time);
	m_options.m_Row.SetExcludeUploadStart(time.Format("%H:%M"));
	m_dtcTime13.GetTime(time);
	m_options.m_Row.SetExcludeUploadEnd(time.Format("%H:%M"));

	m_options.m_Row.SetExclude1Flag(IsTicked(m_checkExclude1Reqd));
	m_options.m_Row.SetExclude2Flag(IsTicked(m_checkExclude2Reqd));
	m_options.m_Row.SetExclude3Flag(IsTicked(m_checkExclude3Reqd));
	m_options.m_Row.SetExclude4Flag(IsTicked(m_checkExclude4Reqd));
	m_options.m_Row.SetPromptBeforeAccessFlag(IsTicked(m_checkPromptBeforeAccess));
	m_options.m_Row.SetEODAfterPaymentFlag(IsTicked(m_checkEODAfterPayments));
	m_options.m_Row.SetImportAccountsFlag(IsTicked(m_checkImportAccounts));

	m_options.m_Row.SetPaymentDay(1, IsTicked(m_checkImportMon));
	m_options.m_Row.SetPaymentDay(2, IsTicked(m_checkImportTue));
	m_options.m_Row.SetPaymentDay(3, IsTicked(m_checkImportWed));
	m_options.m_Row.SetPaymentDay(4, IsTicked(m_checkImportThu));
	m_options.m_Row.SetPaymentDay(5, IsTicked(m_checkImportFri));
	m_options.m_Row.SetPaymentDay(6, IsTicked(m_checkImportSat));
	m_options.m_Row.SetPaymentDay(0, IsTicked(m_checkImportSun));

	if (m_options.m_Row.AccountLinkAllowed(m_nWebPaymentType) == TRUE)
	{
		m_dtcAccount1.GetTime(time);
		m_options.m_Row.SetAccountLinkStart(time.Format("%H:%M"));
		m_dtcAccount2.GetTime(time);
		m_options.m_Row.SetAccountLinkEnd(time.Format("%H:%M"));
		m_options.m_Row.SetAccountLinkDay(1, IsTicked(m_checkAccountMon));
		m_options.m_Row.SetAccountLinkDay(2, IsTicked(m_checkAccountTue));
		m_options.m_Row.SetAccountLinkDay(3, IsTicked(m_checkAccountWed));
		m_options.m_Row.SetAccountLinkDay(4, IsTicked(m_checkAccountThu));
		m_options.m_Row.SetAccountLinkDay(5, IsTicked(m_checkAccountFri));
		m_options.m_Row.SetAccountLinkDay(6, IsTicked(m_checkAccountSat));
		m_options.m_Row.SetAccountLinkDay(0, IsTicked(m_checkAccountSun));
	}

	switch (m_options.SaveRow(NULL, TRUE))
	{
	case SQLCRUD_ERR_UPDATE:
		Prompter.Error("Unable to save changes.\n\nSettings may have been modified by another process.");
		break;

	case SQLCRUD_ERR_NONE:
		break;

	default:
		Prompter.Error("Unable to save changes");
		break;
	}

	EndDialog(IDOK);
}

//$$******************************************************************
