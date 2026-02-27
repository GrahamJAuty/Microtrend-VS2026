//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "EditStampsDlg.h"
//$$******************************************************************

CEditStampsDlg::CEditStampsDlg(CSQLRowPurchaseControlStamp& RowStampOld, CSQLRowPurchaseControlStamp& RowStampNew, CSQLRowPurchaseControlRule& RowRule, bool bAdd, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CEditStampsDlg::IDD, pParent),
	m_RowStampOld(RowStampOld),
	m_RowStampNew(RowStampNew),
	m_RowRule(RowRule)
{
	RowStampNew = RowStampOld;
	m_bAdd = bAdd;
}

//$$******************************************************************

void CEditStampsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATEPICKER_AWARD, m_DatePickerAward);
	DDX_Control(pDX, IDC_DATEPICKER_EXPIRY, m_DatePickerExpiry);
	DDX_Control(pDX, IDC_EDIT_NEVER, m_editNever);
	DDX_Control(pDX, IDC_CHECK_NOEXPIRY, m_checkNoExpiry);
	DDX_Control(pDX, IDC_STATIC_STAMPS, m_staticStamps);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditStampsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_NOEXPIRY, OnToggleNoExpiry)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEditStampsDlg::CSSAutoShutdownPostInitDialog()
{
	if ( FALSE == m_bAdd )
	{
		CString strTitle = "";
		strTitle.Format("Edit Purchase Control Stamps (%s)",
			(const char*) m_RowRule.GetDisplayDescription());
		SetWindowText(strTitle);

		m_staticStamps.SetWindowText("Stamps left");
	}
	else
	{
		CString strTitle = "";
		strTitle.Format("Add Purchase Control Stamps (%s)",
			(const char*) m_RowRule.GetDisplayDescription());
		SetWindowText(strTitle);

		m_staticStamps.SetWindowText("New stamps");
	}

	SubclassEdit(IDC_EDIT_STAMPS, SS_NUM, 6, "%d");

	CString str = "";
	str.Format("%d", m_RowStampOld.GetStampCount());
	GetEditStamps()->SetWindowText(str);

	m_DatePickerAward.SetFormat("ddd dd MMM yyy");
	m_DatePickerExpiry.SetFormat("ddd dd MMM yyy");

	m_editNever.SetWindowText("Never");

	COleDateTime dateAward;
	{
		bool bGotAwardDate = FALSE;

		CString strAwardDate = m_RowStampOld.GetAwardDate();
		if ((strAwardDate.GetLength() == 8) && (::TestNumeric(strAwardDate) == TRUE))
		{
			dateAward = COleDateTime(
				atoi(strAwardDate.Left(4)),
				atoi(strAwardDate.Mid(4, 2)),
				atoi(strAwardDate.Mid(6, 2)),
				0, 0, 0);

			if (dateAward.GetStatus() == COleDateTime::valid)
			{
				bGotAwardDate = TRUE;
			}
		}

		if ( FALSE == bGotAwardDate )
		{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();
				dateAward = COleDateTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0);	
		}

		m_DatePickerAward.SetTime(dateAward);
	}

	COleDateTime dateMinExpire, dateMaxExpire;
	dateMinExpire = dateAward;
	dateMaxExpire = dateAward + COleDateTimeSpan(PurchaseControlRule::Expiry.Max - 1, 0, 0, 0);
	m_DatePickerExpiry.SetRange(&dateMinExpire, &dateMaxExpire);

	COleDateTime dateExpire;
	{
		bool bGotExpireDate = FALSE;

		CString strExpireDate = m_RowStampOld.GetExpireDate();
		if ((strExpireDate.GetLength() == 8) && (::TestNumeric(strExpireDate) == TRUE))
		{
			dateExpire = COleDateTime(
				atoi(strExpireDate.Left(4)),
				atoi(strExpireDate.Mid(4, 2)),
				atoi(strExpireDate.Mid(6, 2)),
				0, 0, 0);

			if ( (dateExpire.GetStatus() == COleDateTime::valid) && ( dateExpire >= dateMinExpire) &&( dateExpire <= dateMaxExpire ) )
			{
				bGotExpireDate = TRUE;
			}
		}

		if (FALSE == bGotExpireDate)
		{
			int nExpireDays = m_RowRule.GetExpiryDays();
			if ((nExpireDays < 1) || (nExpireDays > PurchaseControlRule::Expiry.Max))
			{
				nExpireDays = 1;
			}

			COleDateTime dateExpire = dateAward;
			if ( nExpireDays > 1 )
			{
				dateExpire = dateAward + COleDateTimeSpan(nExpireDays - 1, 0, 0, 0);
			}

			m_DatePickerExpiry.SetTime(dateExpire);
			m_checkNoExpiry.SetCheck(TRUE);
			m_editNever.ShowWindow(SW_SHOW);
			ShowAndEnableWindow(&m_DatePickerExpiry, FALSE);
		}
		else
		{
			m_DatePickerExpiry.SetTime(dateExpire);
			m_DatePickerExpiry.EnableWindow(TRUE);
			m_checkNoExpiry.SetCheck(FALSE);
			m_editNever.ShowWindow(SW_HIDE);
			ShowAndEnableWindow(&m_DatePickerExpiry, TRUE);
		}
	}

	return TRUE;
}

//$$******************************************************************

void CEditStampsDlg::OnToggleNoExpiry()
{
	if (IsTicked(m_checkNoExpiry) == TRUE)
	{
		m_editNever.ShowWindow(SW_SHOW);
		ShowAndEnableWindow(&m_DatePickerExpiry, FALSE);
	}
	else
	{
		m_editNever.ShowWindow(SW_HIDE);
		ShowAndEnableWindow(&m_DatePickerExpiry, TRUE);
	}
}

//$$******************************************************************

void CEditStampsDlg::OnOK()
{
	int nStamps = GetEditBoxInt(*GetEditStamps());

	if ((nStamps < 1) || (nStamps > 999999))
	{
		Prompter.Error("Please select between 1 and 999 stamps");
		GetEditStamps()->SetFocus();
		GetEditStamps()->SetSel(0, -1);
		return;
	}

	m_RowStampNew.SetStampCount(nStamps);

	if (IsTicked(m_checkNoExpiry) == TRUE)
	{
		m_RowStampNew.SetNeverExpire();
	}
	else
	{
		COleDateTime dateExpiry;
		m_DatePickerExpiry.GetTime(dateExpiry);

		if (dateExpiry.m_status == COleDateTime::valid)
		{
			CString strDate = "";
			strDate.Format("%4.4d%2.2d%2.2d",
				dateExpiry.GetYear(),
				dateExpiry.GetMonth(),
				dateExpiry.GetDay());

			m_RowStampNew.SetExpireDate(strDate);
		}
	}

	EndDialog(IDOK);
}

//$$******************************************************************
