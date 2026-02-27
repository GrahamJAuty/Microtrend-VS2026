/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "SQLTranImportPINNumbers.h"
/**********************************************************************/

void CSQLTranImportPINNumbers::DoWork(CReportConsolidationArray<CSortedIntByInt64>& arrayPINs, bool bRemoveExisting)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_strError = "PIN Link Import Failed\n\n";

		m_bCanCommit = FALSE;
		m_nState = 2;

		if ( TRUE == bRemoveExisting )
		{
			CString strSQL = "";
			strSQL.Format("UPDATE %s SET %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = 0",
				(const char*)SQLTableNames::PINNumber,
				(const char*)PINNumber::UserID.Label,
				(const char*)Account::Username.Label,
				(const char*)PINNumber::FirstDate.Label,
				(const char*)PINNumber::FirstTime.Label,
				(const char*)PINNumber::LastDate.Label,
				(const char*)PINNumber::LastTime.Label,
				(const char*)PINNumber::UseCounter.Label);

			if ( CSQLHelpers::ExecuteSQL(m_pDatabase, strSQL) != SQLCRUD_ERR_NONE)
			{
				m_strError += "Unable to remove existing links.";
				return;
			}
		}

		CWorkingDlg dlgWorking("Importing PIN Assignments", FALSE);
		dlgWorking.Create(NULL);

		bool bErrorQuit = FALSE;
		for (int i = 0; i < arrayPINs.GetSize(); i++)
		{
			dlgWorking.SetCaption2RecordsOfTotal(i + 1, arrayPINs.GetSize(), FALSE);

			CSortedIntByInt64 item;
			arrayPINs.GetAt(i, item);

			__int64 nUserID = item.m_nKey;
			int nPINNumber = item.m_nVal;

			CString strPINNumber = "";
			strPINNumber.Format("%4.4d", nPINNumber);
			if ( CSQLRowPINNumber::IsValidPINNumber(strPINNumber) == FALSE )
			{
				continue;
			}

			CSQLRepositoryPINNumber repoPINNumber;
			if ( repoPINNumber.LinkAccountIDToPINNumber(strPINNumber, nUserID, m_pDatabase) == FALSE )
			{
				m_strError.AppendFormat("Unable to link PIN number to account.\n\nUserID: %I64d\nPIN Number: %s", 
					nUserID, 
					(const char*) strPINNumber);

				bErrorQuit = TRUE;
				break;
			}
		}

		dlgWorking.DestroyWindow();

		if (FALSE == bErrorQuit)
		{
			m_bCanCommit = TRUE;
		}
	}
}

/**********************************************************************/

