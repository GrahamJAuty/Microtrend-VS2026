/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "SQLTranCreatePINNumbers.h"
/**********************************************************************/

void CSQLTranCreatePINNumbers::DoWork(bool bDeleteExisting, bool bExcludeWeak)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = 2;

		if ( TRUE == bDeleteExisting )
		{
			CString strSQL = "";
			strSQL.Format("DELETE FROM %s", 
				(const char*) SQLTableNames::PINNumber);

			if ( CSQLHelpers::ExecuteSQL(m_pDatabase, strSQL) != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		CSQLRepositoryPINNumber RepoPINNumber;
		CSQLResultInfo result = RepoPINNumber.CountRows(m_pDatabase);

		if ( result.GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if ( result.GetSQLResult() == 0)
			{
				CSQLRowPINNumber RowPIN;
				RowPIN.SetUserID("");
				RowPIN.SetUsername("");
				RowPIN.SetFirstDate("");
				RowPIN.SetFirstTime("");
				RowPIN.SetLastDate("");
				RowPIN.SetLastTime("");
				RowPIN.SetUseCounter(0);

				CWorkingDlg dlgWorking("Resetting PIN Numbers", FALSE);
				dlgWorking.Create(NULL);

				for (int nPINNumber = 0; nPINNumber <= 9999; nPINNumber++)
				{
					if (nPINNumber % 10 == 0)
					{
						dlgWorking.SetCaption2RecordsOfTotal(nPINNumber, 10000, FALSE);
					}

					bool bAddPINNumber = TRUE;
					if (TRUE == bExcludeWeak)
					{
						bAddPINNumber = (WeakPIN(nPINNumber) == FALSE);
					}

					if (TRUE == bAddPINNumber)
					{
						CString strPINNumber = "";
						strPINNumber.Format("%4.4d", nPINNumber);
						RowPIN.SetPINNumber(strPINNumber);
						RepoPINNumber.InsertRow(RowPIN, m_pDatabase);
					}
				}

				dlgWorking.DestroyWindow();
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

bool CSQLTranCreatePINNumbers::WeakPIN(int nPINNumber)
{
	//CATCH XXXX and XYXY 
	if ( (nPINNumber / 100) == (nPINNumber % 100))
	{
		return TRUE;
	}

	//CATCH BIRTHDAY YEARS - 1950 to 2030
	if ((nPINNumber >= 1950) && (nPINNumber <= 2030))
	{
		return TRUE;
	}

	CString strPINNumber = "";
	strPINNumber.Format("%4d", nPINNumber);
	int nDigit1 = strPINNumber.GetAt(0);
	int nDigit2 = strPINNumber.GetAt(1);
	int nDigit3 = strPINNumber.GetAt(2);
	int nDigit4 = strPINNumber.GetAt(3);

	//CATCH 1234, 2345, 3456, 4567, 5678, 6789
	if ( (nDigit2 == nDigit1 + 1) && (nDigit3 == nDigit2 + 1) && (nDigit4 == nDigit3 + 1))
	{
		return TRUE;
	}

	//CATCH 4321, 5432, 6543, 7654, 8765, 9876
	if ((nDigit2 == nDigit1 - 1) && (nDigit3 == nDigit2 - 1) && (nDigit4 == nDigit3 - 1))
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
