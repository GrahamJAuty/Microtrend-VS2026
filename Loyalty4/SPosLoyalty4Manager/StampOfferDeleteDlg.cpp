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
#include "StampOfferDeleteDlg.h"
//*******************************************************************

CStampOfferDeleteDlg::CStampOfferDeleteDlg(bool bAllAccounts, __int64 nUserID, int nOfferID, CWnd* pParent)
	: CSSDialog(CStampOfferDeleteDlg::IDD, pParent)
{
	m_bAllAccounts = bAllAccounts;
	m_nUserID = nUserID;
	m_nOfferID = nOfferID;
}

//*******************************************************************

void CStampOfferDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ACTION1, m_checkAction1);
	DDX_Control(pDX, IDC_CHECK_ACTION2, m_checkAction2);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CStampOfferDeleteDlg, CSSDialog)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CStampOfferDeleteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	
	CString strText = "";
	strText.Format("Delete Expired Stamps for Offer %d", m_nOfferID);

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

	SetWindowText(strText);

	return TRUE;
}

//*******************************************************************

CString CStampOfferDeleteDlg::GetAccountFilter()
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

void CStampOfferDeleteDlg::OnOK()
{
	if ((IsTicked(m_checkAction1) == FALSE) && (IsTicked(m_checkAction2) == FALSE))
	{
		Prompter.Error("Please select at least one action to perform.");
		return;
	}

	if (Prompter.YesNo("Are you sure you wish to delete the selected stamps?") != IDYES)
	{
		return;
	}

	m_strResults = "";

	if (IsTicked(m_checkAction1))
	{
		DeleteExpiredStamps();
	}

	if (IsTicked(m_checkAction2))
	{
		DeletePreviousEditionStamps();
	}

	if (m_strResults != "")
	{
		Prompter.Info(m_strResults);
		EndDialog(IDOK);
	}
	else
	{
		Prompter.Info("No stamps were deleted.");
		EndDialog(IDCANCEL);
	}
}

//*******************************************************************

void CStampOfferDeleteDlg::DeleteExpiredStamps()
{
	COleDateTime dateToday = COleDateTime::GetCurrentTime();

	CString strDateToday = "";
	strDateToday.Format("%4.4d%2.2d%2.2d",
		dateToday.GetYear(),
		dateToday.GetMonth(),
		dateToday.GetDay());

	CSQLRecordSetOfferStamp RecordSet(NULL, RSParams_OfferStamp_NormalNoParams{});

	RecordSet.m_strFilter.Format("%s%s < %s AND LEN (%s) = 8 AND %s = %d",
		(const char*) GetAccountFilter(),
		OfferStamp::ExpireDate.Label,
		(const char*) strDateToday,
		OfferStamp::ExpireDate.Label,
		OfferStamp::StampOfferID.Label,
		m_nOfferID);

	int nRows = 0;
	int nStamps = 0;
	DeleteRecords(RecordSet, APPNO_SO_MDEL_EXPIRE, nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
		strDetail.Format("Deleted %d expired stamp%s from %d row%s", 
			nStamps,
			( 1 == nStamps ) ? "" : "s",
			nRows,
			( 1 == nRows ) ? "" : "s" );

		m_strResults += strDetail;
	}
}

//*******************************************************************

void CStampOfferDeleteDlg::DeletePreviousEditionStamps()
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
	DeleteRecords(RecordSet, APPNO_SO_MDEL_EDITION, nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
		strDetail.Format("Deleted %d old edition stamp%s from %d row%s",
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

void CStampOfferDeleteDlg::DeleteRecords(CSQLRecordSetOfferStamp& RecordSet, int nAppNo, int& nRows, int& nStamps )
{
	nRows = 0;
	nStamps = 0;

	CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> arrayStamps;

	CSQLRowOfferStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}

	for (int i = 0; i < arrayStamps.GetSize(); i++)
	{
		CSQLRepositoryOfferStamp repoStamp;
		if (repoStamp.DeleteRow(arrayStamps[i], NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			SQLStampOfferBuffer.Reset();

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(arrayStamps[i].GetUserID());
			CSQLRepositoryAccount repoAccount;
			repoAccount.SelectRow(RowAccount, NULL);

			CSQLRepositoryStampOfferInfo repoInfo;

			SQLStampOfferBuffer.BufferPCAction(
				nAppNo,
				RowAccount,
				arrayStamps[i],
				repoInfo.GetOfferName(m_nOfferID, NULL),
				arrayStamps[i].GetStampCount() * (-1),
				0
			);

			SQLStampOfferBuffer.WritePCAuditLines(NULL);

			nRows++;
			nStamps += arrayStamps[i].GetStampCount();
		}
	}
}

//*******************************************************************

void CStampOfferDeleteDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************
