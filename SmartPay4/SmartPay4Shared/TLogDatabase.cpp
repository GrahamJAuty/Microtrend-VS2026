// Open a TLogFile purchase history as a Unsorted database
// & find the purchases made on a particular Date \ Time & Ecr CCNo
// 
// Used to add purchase items text to an Ecr transaction line in atc audit report line
//
//*****************************************************************************
#include "..\SmartPay4Shared\TLogFile.h"
//*****************************************************************************
#include "TLogDatabase.h"
//*****************************************************************************

CTLogDatabase::CTLogDatabase()
{
	m_strSalesText = "";

	for (int i = 0; i < nMAX_TAXCODES; i++)
	{
		m_dSales[i] = 0;
	}

	m_dSalesUnknown = 0;									// sales without a VAT code
}

//******************************************************************************
// as TLogFile does not have a header - uses implied oolumn field names ( col1, col2 .. colnn )
// see TlogRecord for colnn labels

int CTLogDatabase::Open ( const char* szFilename )
{
	return OpenSSDatabaseReadOnlyImplied ( szFilename, "<col>", 10 );
}	

//******************************************************************

CString CTLogDatabase::MakeKey ( const char* szDate, const char* szTime )
{
	CSSDate date ( szDate );
	CSSTime time ( szTime );

	CString strKey ="";
	strKey.Format ( "%4.4d%2.2d%2.2d%2.2d%2.2d", date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute() );
	return strKey;
}
 
//******************************************************************

bool CTLogDatabase::FindSalesText(const char* szDate, const char* szTime, int nCCNo)
{
	bool bReply = FALSE;
	CString strKey = MakeKey(szDate, szTime);

	int nLow = 0;
	int nLowest = GetRecordCount();
	int nHigh = nLowest - 1;

	CTLogRecord tlog;
	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh) / 2;
		MoveTo(nMid);											// get record

		tlog.ConvertRecord(this, FALSE);							// convert database line to a tlog record

		CString strTlogKey = MakeKey(tlog.GetDate(), tlog.GetTime());

		if (strKey < strTlogKey)
		{
			nHigh = nMid - 1;
		}
		else
		{
			if (strKey > strTlogKey)
			{
				nLow = nMid + 1;
			}
			else													// match on date & time
			{
				if (tlog.GetCCNo() == nCCNo)
				{
					bReply = TRUE;									// found match on first try
				}
				else
				{
					bReply = LocateSale(nCCNo, tlog.GetTime());	// try & find relevant record with matching CCNo
				}

				if (bReply == TRUE)
				{
					tlog.ConvertRecord(this, TRUE);						// convert database line to a tlog record

					double dSalesValue = 0;										// sales value not used
					tlog.GetCombinedValues(m_strSalesText, dSalesValue);	// just extract purchases texts
				}
				break;
			}
		}
	}

	return bReply;
}
 
//******************************************************************

bool CTLogDatabase::FindSalesTaxes(const char* szDate, const char* szTime, int nCCNo)
{
	bool bReply = FALSE;
	CString strKey = MakeKey(szDate, szTime);

	int nLow = 0;
	int nLowest = GetRecordCount();
	int nHigh = nLowest - 1;

	CTLogRecord tlog;
	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh) / 2;
		MoveTo(nMid);											// get record

		tlog.ConvertRecord(this, FALSE);							// convert database line to a tlog partial record

		CString strTlogKey = MakeKey(tlog.GetDate(), tlog.GetTime());

		if (strKey < strTlogKey)
		{
			nHigh = nMid - 1;
		}
		else
		{
			if (strKey > strTlogKey)
			{
				nLow = nMid + 1;
			}
			else													// match on date & time;
			{
				int n = tlog.GetCCNo();
				if (tlog.GetCCNo() == nCCNo)
				{
					bReply = TRUE;									// found match on first try
				}
				else
				{
					bReply = LocateSale(nCCNo, tlog.GetTime());	// try & find relevant record with matching CCNo
				}

				if (bReply == TRUE)
				{
					tlog.ConvertRecord(this, TRUE);				// convert database line to a full tlog record

					for (int nIndex = 0; nIndex < tlog.GetItemCounter(); nIndex++)
					{
						tlog.ExtractItemData(nIndex);

						double dValue = tlog.GetItemLineVal();

						CString strTaxcode = tlog.GetItemTax();
						if (strTaxcode != "")
						{
							char cTaxcode = strTaxcode.GetAt(0);
							int nTaxCode = cTaxcode - 'A';

							if (nTaxCode >= 0 && nTaxCode < nMAX_TAXCODES)
							{
								m_dSales[nTaxCode] += dValue;
							}
							else
							{
								m_dSalesUnknown += dValue;
							}
						}
						else
						{
							m_dSalesUnknown += dValue;
						}
					}
				}
				break;
			}
		}
	}

	return bReply;
}
 
//*******************************************************************
// to get here have found a match on date & time 

bool CTLogDatabase::LocateSale ( int nCCNo, const char* szTime )
{
	bool bFound = FALSE;

	CTLogRecord tlog;
	CSSTime timeReqd ( szTime );						// time to match on

	int nCurrentRow = GetCurrentRow();					// starting point in TLog file

// start looking forward from starting point

	int nOffset = 1;

	while ( MoveTo ( nCurrentRow + nOffset ) == TRUE )	// move forward from starting point
	{
		tlog.ConvertRecord ( this, FALSE );				// convert database line to a partila tlog record

		CSSTime time ( tlog.GetTime() ); 
		if (time != timeReqd)							// gone passed reqd time
		{
			break;
		}

		if ( tlog.GetCCNo() == nCCNo )
		{
			bFound = TRUE;								// match found - so exit
			break;
		}
		++nOffset;										// keep going to next record
	}  

	if ( bFound == FALSE )								// not found so work back from starting point
	{
		nOffset = 1;
		while ( MoveTo ( nCurrentRow - nOffset ) == TRUE )		// move backwards from starting point
		{
			tlog.ConvertRecord ( this, FALSE );					// convert database line to a partial tlog record

			CSSTime time ( tlog.GetTime() );	
			if (time != timeReqd)								// gone passed reqd time
			{
				break;
			}

			if ( tlog.GetCCNo() == nCCNo )
			{
				bFound = TRUE;
				break;
			}
			++nOffset;
		}  
	}

	return bFound;
}

//*******************************************************************
#ifndef SYSTEMTYPE_SERVER
//*******************************************************************

void CTLogDatabase::GetPebbleTransactionItems(const char* szDate, const char* szTime, int nCCNo, CArray<CPebbleTransactionItem, CPebbleTransactionItem>& arrayItems)
{
	arrayItems.RemoveAll();

	CString strKey = MakeKey(szDate, szTime);

	int nLow = 0;
	int nLowest = GetRecordCount();
	int nHigh = nLowest - 1;

	bool bFoundTran = FALSE;

	CTLogRecord tlog;
	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh) / 2;
		MoveTo(nMid);											// get record

		tlog.ConvertRecord(this, FALSE);							// convert database line to a tlog partial record

		CString strTlogKey = MakeKey(tlog.GetDate(), tlog.GetTime());

		if (strKey < strTlogKey)
		{
			nHigh = nMid - 1;
		}
		else
		{
			if (strKey > strTlogKey)
			{
				nLow = nMid + 1;
			}
			else													// match on date & time;
			{
				int n = tlog.GetCCNo();
				if (tlog.GetCCNo() == nCCNo)
				{
					bFoundTran = TRUE;									// found match on first try
				}
				else
				{
					bFoundTran = LocateSale(nCCNo, tlog.GetTime());	// try & find relevant record with matching CCNo
				}

				if ( TRUE == bFoundTran)
				{
					tlog.ConvertRecord(this, TRUE);				// convert database line to a full tlog record

					for (int nIndex = 0; nIndex < tlog.GetItemCounter(); nIndex++)
					{
						tlog.ExtractItemData(nIndex);

						CPebbleTransactionItem TransactionItem;
						TransactionItem.m_nPluNo = _atoi64(tlog.GetItemNo());
						TransactionItem.m_dItemQty = tlog.GetItemLineQty();
						TransactionItem.m_dItemCost = ((double)tlog.GetPerItemVal()) / 100.0;
						TransactionItem.m_strDescription = tlog.GetItemText();
						TransactionItem.m_strVATBand = tlog.GetItemTax();
						arrayItems.Add(TransactionItem);
					}
				}
				break;
			}
		}
	}
}

//*******************************************************************
#endif
//*******************************************************************
