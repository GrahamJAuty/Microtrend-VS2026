//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRecordSetOfferStamp.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
//*******************************************************************
#include "SQLTranStampManualEdit.h"
//*******************************************************************
#include "StampOfferModifyDlg.h"
//*******************************************************************

CStampOfferModifyDlg::CStampOfferModifyDlg(bool bAllAccounts, __int64 nUserID, int nOfferID, CWnd* pParent /*=NULL*/)
	: CSSDialog(CStampOfferModifyDlg::IDD, pParent)
{
	m_bAllAccounts = bAllAccounts;
	m_nUserID = nUserID;
	m_nOfferID = nOfferID;
}

//*******************************************************************

void CStampOfferModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ACTION1, m_checkAction1);
	DDX_Control(pDX, IDC_CHECK_ACTION2, m_checkAction2);
	DDX_Control(pDX, IDC_DATEPICKER_CURRENT_FROM, m_DatePickerCurrentFrom);
	DDX_Control(pDX, IDC_DATEPICKER_CURRENT_TO, m_DatePickerCurrentTo);
	DDX_Control(pDX, IDC_CHECK_CURRENT_NOEXPIRE, m_checkCurrentNoExpire);
	DDX_Control(pDX, IDC_DATEPICKER_NEW, m_DatePickerNew);
	DDX_Control(pDX, IDC_COMBO_NEWEXPIRY, m_comboNewExpiry);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CStampOfferModifyDlg, CSSDialog)
	ON_BN_CLICKED(IDC_CHECK_ACTION1, OnToggleAction1)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWEXPIRY, OnSelectNewExpiry)
	ON_STN_CLICKED(IDC_STATIC_TO, OnClickStaticTo)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CStampOfferModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strText = "";
	strText.Format("Modify Expired Stamps for Offer %d", m_nOfferID);
	
	if (TRUE == m_bAllAccounts)
	{
		strText += ", All Accounts";
	}
	else
	{
		CString strAccount = "";
		strAccount.Format(", Account %I64d", m_nUserID);
		strText += strAccount;
	}

	m_DatePickerCurrentFrom.SetFormat("ddd dd MMM yyy");
	m_DatePickerCurrentTo.SetFormat("ddd dd MMM yyy");
	m_DatePickerNew.SetFormat("ddd dd MMM yyy");

	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	COleDateTime dateMinExpire = dateNow;;
	COleDateTime dateMaxExpire = dateNow;
	dateMinExpire -= COleDateTimeSpan(1, 0, 0, 0);
	dateMaxExpire += COleDateTimeSpan(StampOfferInfo::Expiry.Max, 0, 0, 0);

	m_DatePickerNew.SetRange(&dateMinExpire,&dateMaxExpire);
	m_DatePickerNew.SetTime(dateNow);

	COleDateTime dateMin = COleDateTime(2024, 1, 1, 0, 0, 0);
	m_DatePickerCurrentFrom.SetRange(&dateMin, &dateMaxExpire);
	m_DatePickerCurrentTo.SetRange(&dateMin, &dateMaxExpire);

	m_comboNewExpiry.AddString("Default expiry for offer");
	m_comboNewExpiry.AddString("Fixed expiry date");
	m_comboNewExpiry.AddString("Never expire");
	m_comboNewExpiry.SetCurSel(0);

	SetWindowText(strText);
	OnToggleAction1();

	return TRUE;
}

//*******************************************************************

CString CStampOfferModifyDlg::GetAccountFilter()
{
	CString strFilter = "";

	if (FALSE == m_bAllAccounts)
	{
		strFilter += OfferStamp::UserID.Label;
		strFilter += " = ";

		CString strUserID = "";
		strUserID.Format("%I64d", m_nUserID);
		strFilter += strUserID;

		strFilter += " AND ";
	}

	return strFilter;
}

//*******************************************************************

void CStampOfferModifyDlg::OnToggleAction1()
{
	bool bEnable = IsTicked(m_checkAction1);
	m_DatePickerCurrentFrom.EnableWindow(bEnable);
	m_DatePickerCurrentTo.EnableWindow(bEnable);
	m_checkCurrentNoExpire.EnableWindow(bEnable);
	m_comboNewExpiry.EnableWindow(bEnable);
	m_DatePickerNew.EnableWindow(bEnable && ( m_comboNewExpiry.GetCurSel() == 1));
}

//*******************************************************************

void CStampOfferModifyDlg::OnSelectNewExpiry()
{
	m_DatePickerNew.EnableWindow(m_comboNewExpiry.GetCurSel() == 1);
}

//*******************************************************************

void CStampOfferModifyDlg::OnClickStaticTo()
{
	COleDateTime dateFrom;
	m_DatePickerCurrentFrom.GetTime(dateFrom);
	m_DatePickerCurrentTo.SetTime(dateFrom);
}

//*******************************************************************

void CStampOfferModifyDlg::OnOK()
{
	if ((IsTicked(m_checkAction1) == FALSE) && (IsTicked(m_checkAction2) == FALSE))
	{
		Prompter.Error("Please select at least one action to perform.");
		return;
	}

	if (Prompter.YesNo("Are you sure you wish to modify the selected stamps ?") != IDYES)
	{
		return;
	}

	m_strResults = "";

	if (IsTicked(m_checkAction1))
	{
		ModifyStampExpiryDates();
	}

	if (IsTicked(m_checkAction2))
	{
		MakeAllStampsCurrentEdition();
	}

	if (m_strResults != "")
	{
		Prompter.Info(m_strResults);
		EndDialog(IDOK);
	}
	else
	{
		Prompter.Info("No stamps were modified.");
		EndDialog(IDCANCEL);
	}
}

//*******************************************************************

void CStampOfferModifyDlg::ModifyStampExpiryDates()
{
	CSQLRowStampOfferInfo RowOffer;
	RowOffer.SetStampOfferID(m_nOfferID);

	CSQLRepositoryStampOfferInfo repoInfo;
	if (repoInfo.SelectRow(RowOffer, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CSQLRecordSetOfferStamp RecordSet(NULL, RSParams_OfferStamp_NormalNoParams{});

	COleDateTime dateFrom, dateTo;
	m_DatePickerCurrentFrom.GetTime(dateFrom);
	m_DatePickerCurrentTo.GetTime(dateTo);

	CString strDateFrom = "";
	strDateFrom.Format("%4.4d%2.2d%2.2d",
				dateFrom.GetYear(),
				dateFrom.GetMonth(),
				dateFrom.GetDay());	

	CString strDateTo = "";
	strDateTo.Format("%4.4d%2.2d%2.2d",
						dateTo.GetYear(),
						dateTo.GetMonth(),
						dateTo.GetDay());

	CString strExpireDate = "00";
	if (IsTicked(m_checkCurrentNoExpire))
	{
		strExpireDate = CSQLRowOfferStamp::GetNeverExpireDate();
	}

	RecordSet.m_strFilter.Format("%s%s = %d AND LEN(%s) = 8 AND (( %s >= %s AND %s <= %s ) OR ( %s = %s ) )",
		(const char*)GetAccountFilter(),
		OfferStamp::StampOfferID.Label,
		m_nOfferID,
		OfferStamp::ExpireDate.Label,
		OfferStamp::ExpireDate.Label,
		(const char*)strDateFrom,
		OfferStamp::ExpireDate.Label,
		(const char*)strDateTo,
		OfferStamp::ExpireDate.Label,
		strExpireDate);

	CString strNewDate = "";
	switch (m_comboNewExpiry.GetCurSel())
	{
	case 1:
		{
			COleDateTime dateNew;
			m_DatePickerNew.GetTime(dateNew);

			strNewDate.Format("%4.4d%2.2d%2.2d",
				dateNew.GetYear(),
				dateNew.GetMonth(),
				dateNew.GetDay());
		}
		break;

	case 2:
		strNewDate = CSQLRowOfferStamp::GetNeverExpireDate();
		break;

	case 0:
	default:
		strNewDate = "";
		break;
	}

	int nRows = 0;
	int nStamps = 0;
	UpdateRecords(RecordSet, APPNO_SO_MFIX_EXPIRE, RowOffer.GetExpiry(), strNewDate, nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
	
		strDetail.Format("Modified expiry dates for %d stamp%s in %d row%s",
						nStamps,
						(1 == nStamps) ? "" : "s",
						nRows,
						(1 == nRows) ? "" : "s");

		m_strResults += strDetail;
	}
}

//*******************************************************************

void CStampOfferModifyDlg::MakeAllStampsCurrentEdition()
{
	CSQLRowStampOfferInfo RowOffer;
	RowOffer.SetStampOfferID(m_nOfferID);

	CSQLRepositoryStampOfferInfo repoInfo;
	if (repoInfo.SelectRow(RowOffer, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CSQLRecordSetOfferStamp RecordSet(NULL, RSParams_OfferStamp_NormalNoParams{});

	RecordSet.m_strFilter.Format("%s%s != %d AND %s = %d",
				(const char*) GetAccountFilter(),
				StampOfferInfo::Edition.Label,
				RowOffer.GetEdition(),
				OfferStamp::StampOfferID.Label,
				m_nOfferID);

	int nRows = 0;
	int nStamps = 0;
	UpdateRecords(RecordSet, APPNO_SO_MFIX_EDITION, RowOffer.GetEdition(), "", nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
		strDetail.Format("Made %d stamp%s current edition in %d row%s",
						nStamps,
						(1 == nStamps) ? "" : "s",
						nRows,
						(1 == nRows) ? "" : "s");

		if (m_strResults != "")
		{
			m_strResults += "\n\n";
		}

		m_strResults += strDetail;
	}
}

//*******************************************************************

void CStampOfferModifyDlg::UpdateRecords(CSQLRecordSetOfferStamp& RecordSet, int nAppNo, int nEditionOrExpiry, CString strNewDate, int& nRows, int& nStamps)
{
	nRows = 0;
	nStamps = 0;

	CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> arrayStamps;

	CSQLRowOfferStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}

	CSQLRowAccountFull RowAccount;
	__int64 nCurrentUserID = -1;

	for (int i = 0; i < arrayStamps.GetSize(); i++)
	{
		CSQLRowOfferStamp RowStampsOld = arrayStamps[i];
		CSQLRowOfferStamp RowStampsNew = arrayStamps[i];

		if (nAppNo == APPNO_SO_MFIX_EXPIRE)
		{
			if (strNewDate != "")
			{
				RowStampsNew.SetExpireDate(strNewDate);
			}
			else if (0 == nEditionOrExpiry)
			{
				RowStampsNew.SetExpireDate(CSQLRowOfferStamp::GetNeverExpireDate());
			}
			else
			{
				CString strAwardDate = RowStampsOld.GetAwardDate();

				if ((strAwardDate.GetLength() == 8) && (::TestNumeric(strAwardDate) == TRUE))
				{
					COleDateTime dateAward = COleDateTime(
						atoi(strAwardDate.Left(4)),
						atoi(strAwardDate.Mid(4, 2)),
						atoi(strAwardDate.Right(2)),
						0, 0, 0);

					COleDateTimeSpan spanExpiry = COleDateTimeSpan(nEditionOrExpiry, 0, 0, 0);
					COleDateTime dateExpiry = dateAward + spanExpiry;

					CString strExpiryDate = "";
					strExpiryDate.Format("%4.4d%2.2d%2.2d",
						dateExpiry.GetYear(),
						dateExpiry.GetMonth(),
						dateExpiry.GetDay());

					RowStampsNew.SetExpireDate(strExpiryDate);
				}
			}
		}
		else
		{
			RowStampsNew.SetEdition(nEditionOrExpiry);
		}

		SQLStampOfferBuffer.Reset();

		if (nCurrentUserID != RowStampsOld.GetUserID())
		{
			RowAccount.SetUserID(RowStampsOld.GetUserID());

			CSQLRepositoryAccount repoAccount;
			if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				nCurrentUserID = -1;
				continue;
			}
			else
			{
				nCurrentUserID = RowStampsOld.GetUserID();
			}
		}

		CSQLRepositoryStampOfferInfo repoInfo;

		CSQLTranStampManualEdit Tran(nAppNo);
		Tran.BeginTrans();
		Tran.DoWork(RowAccount, RowStampsOld, RowStampsNew,
			repoInfo.GetOfferName(RowStampsOld.GetStampOfferID(), NULL));

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			Tran.AfterTransaction();
			nRows++;
			nStamps += RowStampsOld.GetStampCount();
			break;

		default:
			break;
		}
	}
}

//*******************************************************************

void CStampOfferModifyDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************
