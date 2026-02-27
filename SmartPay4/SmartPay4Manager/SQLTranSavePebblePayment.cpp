/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRepositoryPebblePaymentType.h"
/**********************************************************************/
#include "SQLTranSavePebblePayment.h"
/**********************************************************************/

void CSQLTranSavePebblePayment::DoWork(CSQLRowSetPebblePayment& RowSet)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		{
			CString strSQL = "";
			strSQL.Format("DELETE FROM %s",
				SQLTableNames::PebblePaymentTypes);

			if (CSQLHelpers::ExecuteSQL(m_pDatabase, strSQL) != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		for (int n = 0; n < RowSet.GetSize(); n++)
		{
			CSQLRowPebblePaymentType RowPayment;
			RowSet.GetRow(n, RowPayment);

			CSQLRepositoryPebblePaymentType PrepStat;
			if ( PrepStat.InsertRow(RowPayment,m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
