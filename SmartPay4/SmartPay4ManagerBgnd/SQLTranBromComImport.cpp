/**********************************************************************/
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLExceptionLogger.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingItem\SQLRepositoryBromComPendingItem.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRepositoryBromComPendingOrder.h"
/**********************************************************************/
#include "BromCom.hpp"
/**********************************************************************/
#include "SQLTranBromComImport.h"
/**********************************************************************/

CSQLTranBromComImport::CSQLTranBromComImport(CWorkingDlg& WorkingDlg, CBromComData& BromComData, bool bNewPayments, CWnd* pParent) : CSQLTranBase(),
	m_WorkingDlg(WorkingDlg),
	m_BromComData(BromComData)
{
	m_pParent = pParent;
	m_strFilenamePendingOrders = "";
	m_strFilenamePendingItems = "";

	m_csvHeaderOrder.Add("OrderID");
	m_csvHeaderOrder.Add("OrderStateDescription");
	m_csvHeaderOrder.Add("PaymentModifiedDate");

	m_csvHeaderOrderItem.Add("OrderID");
	m_csvHeaderOrderItem.Add("PersonID");
	m_csvHeaderOrderItem.Add("OrderItemPriceValue");

	m_bNewPayments = bNewPayments;
	m_bImportOK = ( FALSE == bNewPayments );
	m_nValidPaymentCount = 0;
	m_strBromComError = "";
}

/**********************************************************************/

void CSQLTranBromComImport::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CString strSQLException = "";
		m_bCanCommit = ImportNewPayments(strSQLException);

		if (strSQLException != "")
		{
			m_bCanCommit = FALSE;
			CSQLExceptionLogger::LogSQLExceptionMessage(strSQLException);
		}

		if (FALSE == m_bCanCommit)
		{
			if (m_strBromComError == "")
			{
				m_strBromComError = "API Data Error";
			}
		}

		m_bImportOK = m_bCanCommit;
	}	
}

/**********************************************************************/

bool CSQLTranBromComImport::ImportNewPayments(CString& strSQLException)
{
	m_strFilenamePendingOrders = GetPendingOrders();

	if (m_strBromComError != "")
	{
		return FALSE;
	}

	if (::FileExists(m_strFilenamePendingOrders) == FALSE)
	{
		return TRUE;
	}

	CReportConsolidationArray<CSortedIntItem> arrayOrderID;

	if (CopyPendingOrdersToDatabase(arrayOrderID) == FALSE)
	{
		return FALSE;
	}

	if (arrayOrderID.GetSize() == 0)
	{
		return TRUE;
	}

	bool bNoItemsButOK = FALSE;
	m_strFilenamePendingItems = GetPendingItems(arrayOrderID, bNoItemsButOK);

	if (m_strBromComError != "")
	{
		return FALSE;
	}

	if (TRUE == bNoItemsButOK)
	{
		DeletePendingItemsFromDatabase(arrayOrderID);
		return TRUE;
	}

	if ( ::FileExists(m_strFilenamePendingItems) == FALSE)
	{
		return FALSE;
	}

	DeletePendingItemsFromDatabase(arrayOrderID);
	CopyPendingItemsToDatabase();
	return TRUE;
}

/**********************************************************************/

CString CSQLTranBromComImport::GetPendingOrders()
{
	m_strBromComError = "";

	CString strLoginReply = Filenames.GetWebPaymentImportFilename("xml");
	CString strPaymentsFile = Filenames.GetWebPaymentImportFilename("dat");

	CSSBromCom BromCom(&m_BromComData);
	m_strBromComError = BromCom.GetBromComPaymentOrders(strLoginReply, strPaymentsFile, m_csvHeaderOrder.GetLine(), m_WorkingDlg);

	return strPaymentsFile;
}

//*******************************************************************

bool CSQLTranBromComImport::CopyPendingOrdersToDatabase(CReportConsolidationArray<CSortedIntItem>& arrayOrderID)
{
	arrayOrderID.RemoveAll();

	CString strCommand = "";
	strCommand.Format("UPDATE %s SET %s = 'false'",
		SQLTableNames::BromComPendingOrders,
		BromComPendingOrder::NewOrder.Label);

	try
	{
		m_pDatabase -> ExecuteSQL(strCommand);
	}
	catch (CDBException* pe)
	{
		CSQLExceptionLogger::LogSQLExceptionMessage(pe);
		pe->Delete();
		m_strBromComError = "Unable to clear new order flags";
		return FALSE;
	}

	CSSUnsortedDatabase dbOrders;
	if (dbOrders.OpenSSDatabaseReadOnly(m_strFilenamePendingOrders, 10) != DB_ERR_NONE)
	{
		m_strBromComError.Format("Error in opening Import file ' %s '", (const char*)m_strFilenamePendingOrders);
		return FALSE;
	}

	bool bGotNewDate = FALSE;
	int nLastOrderID = m_BromComData.m_nLastOrderID;
	CString strLastOrderSQLDateTime = m_BromComData.GetLastOrderSQLDateTime();
	CString strLastPaymentDateTime = m_BromComData.GetSQLFilterDateTime();

	if (dbOrders.MoveFirst() == TRUE)
	{
		int nProcessedSoFar = 0;
		m_WorkingDlg.SetCaption1("Adding Pending Orders to Database");

		do
		{
			m_WorkingDlg.SetCaption2RecordsChecked(++nProcessedSoFar, FALSE);

			CSQLRowBromComPendingOrder RowOrder;
			RowOrder.SetOrderID(dbOrders.GetInt(m_csvHeaderOrder.GetString(0)));
			RowOrder.SetStatus(dbOrders.GetString(m_csvHeaderOrder.GetString(1)));
			RowOrder.SetLastModified(dbOrders.GetString(m_csvHeaderOrder.GetString(2)));

			RowOrder.SetNewOrderFlag(
				(RowOrder.GetOrderID() != m_BromComData.m_nLastOrderID) ||
				(RowOrder.GetLastModified() != m_BromComData.GetLastOrderSQLDateTime()));

			{
				CString strLastModified = RowOrder.GetLastModified();

				if ( ::Compare8601Times(strLastModified, strLastPaymentDateTime) == 1 )
				{
					strLastPaymentDateTime = strLastModified;
					strLastOrderSQLDateTime = strLastModified;
					nLastOrderID = RowOrder.GetOrderID();
					bGotNewDate = TRUE;
				}
			}

			CSortedIntItem item;
			item.m_nItem = RowOrder.GetOrderID();
			arrayOrderID.Consolidate(item);

			CSQLRepositoryBromComPendingOrder PrepStat{};
			PrepStat.UpsertRow(RowOrder, m_pDatabase);

		} while (dbOrders.MoveNext() == TRUE);
	}

	if (TRUE == bGotNewDate)
	{
		if (strLastPaymentDateTime.GetLength() >= 19)
		{
			CString strUKTime = "";
			::Convert8601TimeToUKTime(strLastPaymentDateTime, strUKTime);

			m_BromComData.m_nLastOrderID = nLastOrderID;
			m_BromComData.SetLastOrderSQLDateTime(strLastOrderSQLDateTime);
			
			if (strUKTime.GetLength() >= 19)
			{
				m_BromComData.SetBromComLastDate(strUKTime.Left(4) + strUKTime.Mid(5, 2) + strUKTime.Mid(8, 2));
				m_BromComData.SetBromComLastTime(strUKTime.Mid(11, 2) + strUKTime.Mid(14, 2) + strUKTime.Mid(17, 2));
			}

			m_BromComData.Save();
		}
	}

	return TRUE;
}

//*******************************************************************

void CSQLTranBromComImport::DeletePendingItemsFromDatabase(CReportConsolidationArray<CSortedIntItem>& arrayOrderID)
{
	m_WorkingDlg.SetCaption1("Resetting Pending Items for Changed Orders");

	//DELETE EXISTING PENDING ITEMS
	CSQLRepositoryBromComPendingItem PrepStat{};
	for (int n = 0; n < arrayOrderID.GetSize(); n++)
	{
		m_WorkingDlg.SetCaption2RecordsOfTotal(n + 1, arrayOrderID.GetSize(), FALSE);

		CSortedIntItem item{};
		arrayOrderID.GetAt(n, item);

		PrepStat.DeletePendingItemOrder(item.m_nItem,m_pDatabase);
	}
}

//*******************************************************************

bool CSQLTranBromComImport::CopyPendingItemsToDatabase()
{
	CSSUnsortedDatabase dbItems;
	if (dbItems.OpenSSDatabaseReadOnly(m_strFilenamePendingItems, 10) != DB_ERR_NONE)
	{
		m_strBromComError.Format("Error in opening Import file ' %s '", (const char*)m_strFilenamePendingItems);
		return FALSE;
	}

	if (dbItems.MoveFirst() == TRUE)
	{
		int nProcessedSoFar = 0;
		m_WorkingDlg.SetCaption1("Adding Pending Items to Database");

		do
		{
			m_WorkingDlg.SetCaption2RecordsChecked(++nProcessedSoFar, FALSE);

			CSQLRowBromComPendingItem RowItem;
			RowItem.SetOrderID(dbItems.GetInt(m_csvHeaderOrderItem.GetString(0)));
			RowItem.SetPersonID(dbItems.GetInt(m_csvHeaderOrderItem.GetString(1)));
			double dAmount = dbItems.GetDouble(m_csvHeaderOrderItem.GetString(2));

			CSQLRepositoryBromComPendingItem PrepStat{};

			if ( PrepStat.SelectRow(RowItem,m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				RowItem.SetAmount(RowItem.GetAmount() + dAmount);
				PrepStat.UpdateRow(RowItem,m_pDatabase);
			}
			else
			{
				RowItem.SetAmount(dAmount);
				PrepStat.InsertRow(RowItem,m_pDatabase);
			}
		} while (dbItems.MoveNext() == TRUE);
	}

	return TRUE;
}

//*******************************************************************

CString CSQLTranBromComImport::GetPendingItems(CReportConsolidationArray<CSortedIntItem>& arrayOrderID, bool& bNoItemsButOK)
{
	m_strBromComError = "";

	CString strLoginReply = Filenames.GetWebPaymentImportFilenameNum("xml", 2);
	CString strPaymentsFile = Filenames.GetWebPaymentImportFilenameNum("dat", 2);

	CSSBromCom BromCom(&m_BromComData);
	m_strBromComError = BromCom.GetBromComPaymentOrderItems(strLoginReply, strPaymentsFile, m_csvHeaderOrderItem.GetLine(), arrayOrderID, m_WorkingDlg, bNoItemsButOK);

	return strPaymentsFile;
}

//*******************************************************************
