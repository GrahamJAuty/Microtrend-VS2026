//$$******************************************************************
#include "CkJsonObject.h"
#include "CkJsonArray.h"
#include "CkStringBuilder.h"
//$$******************************************************************
#include "PebbleHelpers.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRepositoryPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleRecentTransaction\SQLRepositoryPebbleRecentTransaction.h"
//$$******************************************************************

CString CPebbleHelpers::GetTransactions(CSQLRepositoryPebbleConfig& PebbleConfig, bool bSales, CDatabase* pDatabase, CWorkingDlg* pWorkingDlg, int& nTranCountTotal, CString& strLatestDateTime )
{
	nTranCountTotal = 0;
	strLatestDateTime = "";
	int nTranCountNew = 0;
	int nTranCountDuplicate = 0;
	int nTranCountLoopback = 0;
	int nTranCountOther = 0;
	bool bFailInsert = FALSE;
	
	CString strError = "";

	CReportConsolidationArray<CSortedStringItem> arrayKeys;

	if (UpdateRestConnect(strError) == FALSE)
	{
		return strError;
	}

	if (FALSE == m_bDoneFirstAPICall)
	{
		CString strCaption1 = pWorkingDlg->GetCaption1();
		pWorkingDlg->SetCaption1("Initialising Pebble API link");
		DummyFirstAPICall(PebbleConfig, pWorkingDlg);
		pWorkingDlg->SetCaption1(strCaption1);
	}

	CString strLastKey = "";

	CString strDiagnosticCaption = "";
	strDiagnosticCaption += "Get ";
	strDiagnosticCaption += bSales ? "Sales" : "Top-up";
	strDiagnosticCaption += " Transactions";

	int nKeyCount = 0;

	while (TRUE)
	{
		ClearRest();

		m_Rest.ClearAllQueryParams();
		m_Rest.AddQueryParam("per_page", "1000");

		if (strLastKey != "")
		{
			m_Rest.AddQueryParam("start_key", strLastKey);
		}

		CString strImportDate = PebbleConfig.GetSQLFilterDateTime(bSales);

		m_Rest.AddQueryParam("member_txns", "true");
		m_Rest.AddQueryParam("member_txns_sales", bSales ? "true" : "false");
		m_Rest.AddQueryParam("meta.createdAt.ge", strImportDate);
		m_Rest.AddQueryParam("core.state.eq", "processed");

		if (UpdateIAT(PebbleConfig, strError, pWorkingDlg) == FALSE)
		{
			return strError;
		}

		m_Rest.ClearAllHeaders();
		m_Rest.AddHeader("accept", "application/json");
		m_Rest.AddHeader("authorization", GetIATAuth());
		
		CString strURL = "";
		strURL.Format("/api/v1/orgs/%s/transactions",
			(const char*)PebbleConfig.m_Row.GetActiveOrganisationID());

		CkStringBuilder sbResponseBody;
		bool success = m_Rest.FullRequestNoBodySb("GET", strURL, sbResponseBody);
		
		if (success != true)
		{
			LogRestError();
			strError = "Pebble API Get Transactions Fail";
			Diagnostic(strError, TRUE, strDiagnosticCaption);
			return strError;
		}

		if (m_Rest.get_ResponseStatusCode() >= 400)
		{
			strError.Format("Pebble API Get Transaction Error %d", m_Rest.get_ResponseStatusCode());
			Diagnostic(strError, TRUE, strDiagnosticCaption);
			return strError;
		}

		m_bDoneFirstAPICall = TRUE;

		CkJsonObject jsonResponse;
		jsonResponse.LoadSb(sbResponseBody);
		
		CkJsonArray* pJsonTransactions = jsonResponse.ArrayOf("transactions");

		if (pJsonTransactions != NULL)
		{
			if (pJsonTransactions->get_Size() == 0)
			{
				nTranCountOther++;
				UpdatePendingTransactionCount(pWorkingDlg, bSales, nTranCountNew, nTranCountDuplicate, nTranCountLoopback, nTranCountOther);
			}
			else
			{
				for (int n = 0; n < pJsonTransactions->get_Size(); n++)
				{
					CkJsonObject* pTransaction = pJsonTransactions->ObjectAt(n);

					if (pTransaction != NULL)
					{
						Diagnostic(pTransaction, TRUE, strDiagnosticCaption);
						bool bIgnoreThisTransaction = FALSE;

						if (FALSE == bSales)
						{
							CString strPurseID = pTransaction->stringOf("meta.purseId");
							if (strPurseID != "default")
							{
								nTranCountOther++;
								UpdatePendingTransactionCount(pWorkingDlg, bSales, nTranCountNew, nTranCountDuplicate, nTranCountLoopback, nTranCountOther);
								bIgnoreThisTransaction = TRUE;
							}
						}

						if (FALSE == bIgnoreThisTransaction)
						{
							CString strMIS = pTransaction->stringOf("meta.memberDetails.mis.misId");

							if (strMIS == "")
							{
								CString strMemberType = pTransaction->stringOf("meta.memberDetails.memberType");

								strMemberType.MakeLower();
								if (strMemberType == "hospitality")
								{
									nTranCountOther++;
									UpdatePendingTransactionCount(pWorkingDlg, bSales, nTranCountNew, nTranCountDuplicate, nTranCountLoopback, nTranCountOther);
									bIgnoreThisTransaction = TRUE;
								}
							}
						}

						if (FALSE == bIgnoreThisTransaction)
						{
							CString strFirstName = pTransaction->stringOf("meta.memberDetails.mis.firstName");
							CString strLastName = pTransaction->stringOf("meta.memberDetails.mis.lastName");

							CString strName = strFirstName;
							if (strLastName != "")
							{
								if (strName != "")
								{
									strName += " ";
								}

								strName += strLastName;
							}

							CString strDateTime = "";
							{
								CString strSQLDateTime = pTransaction->stringOf("meta.createdAt");
								strDateTime = ::ConvertISO8601ToSimpleDateTime(strSQLDateTime);
							}

							CString strTranId = pTransaction->stringOf("meta.transactionId");
							CString strMemberId = pTransaction->stringOf("meta.memberId");
							CString strTag = pTransaction->stringOf("meta.tags.SmartPay");
							int nAmount = pTransaction->IntOf("amountAsInteger");

							if (strTag == "")
							{
								CSQLRepositoryPebblePendingTransaction PrepStatPending(FALSE);
								CSQLRepositoryPebbleRecentTransaction PrepStatRecent;

								CSQLRowPebblePendingTransaction RowPending;
								CSQLRowPebbleRecentTransaction RowRecent;
								RowPending.SetTransactionID(strTranId);
								RowRecent.SetTransactionID(strTranId);

								if (PrepStatPending.SelectRow(RowPending, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
								{
									nTranCountTotal++;
									nTranCountDuplicate++;
									DiagnosticTranDump(strTranId, strMemberId, strDateTime, strTag, nAmount, bSales, "Duplicate");
								}
								else if (PrepStatRecent.SelectRow(RowRecent, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
								{
									nTranCountTotal++;
									nTranCountDuplicate++;
									DiagnosticTranDump(strTranId, strMemberId, strDateTime, strTag, nAmount, bSales, "Duplicate");
								}
								else
								{
									RowPending.SetTransactionID(strTranId);
									RowPending.SetMemberID(strMemberId);
									RowPending.SetDateTime(strDateTime);
									RowPending.SetName(strName);
									RowPending.SetAmount(nAmount);
									RowPending.SetIsSaleFlag(bSales);

									if (PrepStatPending.InsertRow(RowPending, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
									{
										bFailInsert = TRUE;
									}
									else
									{
										if (strDateTime > strLatestDateTime)
										{
											strLatestDateTime = strDateTime;
										}

										nTranCountNew++;
										nTranCountTotal++;
										DiagnosticTranDump(strTranId, strMemberId, strDateTime, strTag, nAmount, bSales, "New");
									}
								}
							}
							else
							{
								if (strDateTime > strLatestDateTime)
								{
									strLatestDateTime = strDateTime;
								}

								nTranCountTotal++;
								nTranCountLoopback++;
								DiagnosticTranDump(strTranId, strMemberId, strDateTime, strTag, nAmount, bSales, "Loopback");
							}

							UpdatePendingTransactionCount(pWorkingDlg, bSales, nTranCountNew, nTranCountDuplicate, nTranCountLoopback, nTranCountOther);
						}
						
						delete pTransaction;
					}
				}
				delete pJsonTransactions;
			}
		}
			
		if (TRUE == bFailInsert)
		{
			strError = "Unable to insert new transaction.";
			Diagnostic(strError, TRUE, strDiagnosticCaption);
			break;
		}

		CkString lastkey;
		jsonResponse.StringOf("lastKey", lastkey);
		strLastKey = lastkey.getStringUtf8();

		if ( strLastKey == "")
		{
			break;
		}

		CString strCount = "";
		strCount.Format("Key# %d", ++nKeyCount);
		Diagnostic(strLastKey, TRUE, strCount);
	}

	return strError;
}

//$$******************************************************************

void CPebbleHelpers::DummyFirstAPICall(CSQLRepositoryPebbleConfig& PebbleConfig, CWorkingDlg* pWorkingDlg)
{
	CString strError = "";

	if (UpdateRestConnect(strError) == FALSE)
	{
		return;
	}

	ClearRest();

	m_Rest.ClearAllQueryParams();
	m_Rest.AddQueryParam("per_page", "1");
	m_Rest.AddQueryParam("member_txns", "true");
	m_Rest.AddQueryParam("member_txns_sales", "false");

	m_Rest.AddQueryParam("meta.createdAt.ge", "2500-01-01T00:00:00");

	if (UpdateIAT(PebbleConfig, strError, pWorkingDlg) == FALSE)
	{
		return;
	}

	m_Rest.ClearAllHeaders();
	m_Rest.AddHeader("accept", "application/json");
	m_Rest.AddHeader("authorization", GetIATAuth());

	CString strURL = "";
	strURL.Format("/api/v1/orgs/%s/transactions",
		(const char*)PebbleConfig.m_Row.GetActiveOrganisationID());

	CkStringBuilder sbResponseBody;
	m_Rest.FullRequestNoBodySb("GET", strURL, sbResponseBody);
	m_bDoneFirstAPICall = TRUE;

	return;
}

//$$******************************************************************

void CPebbleHelpers::UpdatePendingTransactionCount(CWorkingDlg* pWorkingDlg, bool bSales, int nNew, int nDuplicate, int nLoopback, int nOther)
{
#ifndef SYSTEMTYPE_BACKGOUND

	if (pWorkingDlg != NULL)
	{
		CString strMsg = "";
		strMsg.Format("New %s %d (Duplicate %d, Loopback %d, Other Type %d)",
			bSales ? "Sales" : "Top-ups",
			nNew,
			nDuplicate,
			nLoopback,
			nOther);

		pWorkingDlg->SetCaption2Message(strMsg, FALSE);
		
		Sleep(50);
	}

#endif
}

//$$******************************************************************

void CPebbleHelpers::DiagnosticTranDump(CString strTranId, CString strMemberId, CString strDateTime, CString strTag, int nAmount, bool bSales, CString strResult)
{
	if (Server.GetPebbleDiagnosticType() != 0)
	{
		CSSFile fileDump;
		if (fileDump.Open(Filenames.GetPebbleTranDumpFilename(), "ab") == TRUE)
		{
			CCSV csv;
			csv.Add(strTranId);
			csv.Add(strMemberId);
			csv.Add(strDateTime);
			csv.Add(strTag);
			csv.Add(nAmount);
			csv.Add(bSales ? "Sale" : "Top-up");
			csv.Add(strResult);
			fileDump.WriteLine(csv.GetLine());
		}
	}
}

//$$******************************************************************

