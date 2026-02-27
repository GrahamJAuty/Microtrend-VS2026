//**********************************************************************
#include <math.h>
//**********************************************************************
#include "TLogRecord.h"
//**********************************************************************

CTLogRecord::CTLogRecord() : CTLogRecord_base()
{
}

//**********************************************************************

CTLogRecord::CTLogRecord ( const char* szLine ) : CTLogRecord_base(szLine)
{
}

//**********************************************************************

CString CTLogRecord::MakeLabel ( int nNo )
{
	CString strLabel = "";
	strLabel.Format ( "col%d", nNo );
	return strLabel;
}

//**********************************************************************
// See Base class definition for field layout
// convert a TlogFile record using implied header database record ( col1,....,colnn )

bool CTLogRecord::ConvertRecord(CTLogDatabase* pDb, bool bFullConvert)
{
	m_date.SetDate(pDb->GetString("col1"));			

	bool bReply = m_date.IsSet();

	if ( TRUE == bReply )
	{
		m_time.SetTime(pDb->GetString("col2"));		
		
		//col 3 = version
		switch (pDb->GetInt("col3"))
		{
		case 1:
			ConvertDataV1(pDb, bFullConvert);
			break;

		case 2:
			ConvertDataV2(pDb, bFullConvert);
			break;

		case 3:
			ConvertDataV3(pDb, bFullConvert);
			break;

		case 4:
			ConvertDataV4(pDb, bFullConvert);
			break;

		case 5:
			ConvertDataV5(pDb, bFullConvert);
			break;

		case 6:
			ConvertDataV6(pDb, bFullConvert);
			break;

		default:
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*******************************************************************

void CTLogRecord::ConvertDataV1(CTLogDatabase* pDb, bool bFullConvert)
{
	m_nCCNo = pDb->GetInt("col4");					// CCNo
	m_dPurse1Spend = pDb->GetDouble("col5");		// card spend
	int nCount = pDb->GetInt("col6");				// item count

	if (TRUE == bFullConvert)
	{
		int nNo = 7;											// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDSv2; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 7+i );
			}

			m_csvItemData.Add("");							// tax code
		}
	}
}

//*******************************************************************

void CTLogRecord::ConvertDataV2(CTLogDatabase* pDb, bool bFullConvert)
{
	m_nTerminalNo = pDb->GetInt("col4");		// Terminal no
	m_nCCNo = pDb->GetInt("col5");		// CCNo
	m_dPurse1Spend = pDb->GetDouble("col6");		// card spend
	m_dPurse2Spend = pDb->GetDouble("col7");		// Purse1 Spend
	m_dPurse1 = pDb->GetDouble("col8");		// Purse1 Spend
	m_dPurse2 = pDb->GetDouble("col9");		// Purse1 Spend
	int nCount = pDb->GetInt("col10");		// item count

	if (TRUE == bFullConvert)
	{
		int nNo = 11;														// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDSv2; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 8+i );
			}

			m_csvItemData.Add("");										// tax code
		}
	}
}

//*******************************************************************

void CTLogRecord::ConvertDataV3(CTLogDatabase* pDb, bool bFullConvert)
{
	m_nTerminalNo = pDb->GetInt("col4");		// Terminal no
	m_nCCNo = pDb->GetInt("col5");		// CCNo
	m_dPurse1Spend = pDb->GetDouble("col6");		// Purse1 spend
	m_dPurse2Spend = pDb->GetDouble("col7");		// Purse2 Spend
	m_dPurse3Spend = pDb->GetDouble("col8");		// Purse3 Spend
	m_dPurse1 = pDb->GetDouble("col9");		// Purse1 balance
	m_dPurse2 = pDb->GetDouble("col10");		// Purse2 balance
	m_dPurse3 = pDb->GetDouble("col11");		// Purse3 balance
	int nCount = pDb->GetInt("col12");		// item count

	if (TRUE == bFullConvert)
	{
		int nNo = 13;										// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDSv3; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 8+i );
			}
		}
	}
}

//*******************************************************************

void CTLogRecord::ConvertDataV4 ( CTLogDatabase* pDb, bool bFullConvert )
{
	m_nTerminalNo	= pDb->GetInt	 ( "col4" );		// Terminal no
	m_nCCNo			= pDb->GetInt	 ( "col5" );		// CCNo
	m_strReceiptNo	= pDb->GetString ( "col6" );		// ReceiptNo
	m_dPurse1Spend	= pDb->GetDouble ( "col7" );		// Purse1 spend
	m_dPurse2Spend	= pDb->GetDouble ( "col8" );		// Purse2 Spend
	m_dPurse3Spend	= pDb->GetDouble ( "col9" );		// Purse3 Spend
	m_dPurse1		= pDb->GetDouble ( "col10" );		// Purse1 balance
	m_dPurse2		= pDb->GetDouble ( "col11" );		// Purse2 balance
	m_dPurse3		= pDb->GetDouble ( "col12" );		// Purse3 balance
	int nCount		= pDb->GetInt	 ( "col13" );		// item count

	if ( TRUE == bFullConvert )
	{
		int nNo = 14;										// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDSv4; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 8+i );
			}
		}
	}
}

//********************************************************************

void CTLogRecord::ConvertDataV5(CTLogDatabase* pDb, bool bFullConvert)
{
	m_nTerminalNo = pDb->GetInt("col4");		// Terminal no
	m_nCCNo = pDb->GetInt("col5");		// CCNo
	m_strReceiptNo = pDb->GetString("col6");		// ReceiptNo
	m_dPurse1Spend = pDb->GetDouble("col7");		// Purse1 spend
	m_dPurse2Spend = pDb->GetDouble("col8");		// Purse2 Spend
	m_dPurse3Spend = pDb->GetDouble("col9");		// Purse3 Spend
	m_dPurse1 = pDb->GetDouble("col10");		// Purse1 balance
	m_dPurse2 = pDb->GetDouble("col11");		// Purse2 balance
	m_dPurse3 = pDb->GetDouble("col12");		// Purse3 balance
	int nCount = pDb->GetInt("col13");		// item count

	if (TRUE == bFullConvert)
	{
		int nNo = 14;										// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDS; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 8+i );
			}
		}
	}
}

//********************************************************************

void CTLogRecord::ConvertDataV6(CTLogDatabase* pDb, bool bFullConvert)
{
	m_nFolderSet = pDb->GetInt("col4");		// Folder set
	m_nTerminalNo = pDb->GetInt("col5");		// Terminal no
	m_nCCNo = pDb->GetInt("col6");		// CCNo
	m_strReceiptNo = pDb->GetString("col7");		// ReceiptNo
	m_dPurse1Spend = pDb->GetDouble("col8");		// Purse1 spend
	m_dPurse2Spend = pDb->GetDouble("col9");		// Purse2 Spend
	m_dPurse3Spend = pDb->GetDouble("col10");		// Purse3 Spend
	m_dPurse1 = pDb->GetDouble("col11");		// Purse1 balance
	m_dPurse2 = pDb->GetDouble("col12");		// Purse2 balance
	m_dPurse3 = pDb->GetDouble("col13");		// Purse3 balance
	int nCount = pDb->GetInt("col14");		// item count

	if (TRUE == bFullConvert)
	{
		int nNo = 15;										// start of item data
		for (int n = 0; n < nCount; n++)
		{
			for (int i = 0; i < nTLOGREC_ITEMFIELDS; i++)
			{
				m_csvItemData.Add(pDb->GetString(MakeLabel(nNo++)));	// strLabel.Format ( "col%d", 8+i );
			}
		}
	}
}

//********************************************************************

void CTLogRecord::GetCombinedValues ( CString& strSalesText, double& dSalesValue )
{
	strSalesText = "";
	dSalesValue  = 0.0;

	for ( int nIndex = 0; nIndex < GetItemCounter(); nIndex++ )
	{
		ExtractItemData ( nIndex );

		if ( m_dLineQty != 0 )							// stopped 0 priced items from being display //29/09/2016 - v1.13a
		{
			dSalesValue += m_dLineVal;

			switch (m_nItemType)
			{
			case nTLOGTYPE_PLU:
			case nTLOGTYPE_DEPT:
			case nTLOGTYPE_PLU2:
			case nTLOGTYPE_DISC_ITEM:
			{
				if (strSalesText.GetLength() > 0)
				{
					strSalesText += ", ";				// add separator
				}

				double dQty = m_dLineQty;
				CString strRefund = "";

				if (dQty < 0)							// see if refund
				{
					strRefund = "(R)";
					dQty = -(dQty);						// make the qty value POSITIVE
				}

				if (dQty > 1)
				{
					double n = 0.0;
					int nDps = (modf(dQty, &n) != 0.0) ? 1 : 0;

					CString strQtyText = "";
					strQtyText.Format("%.*lfx", nDps, dQty);		// 2xChips etc..
					strSalesText += strQtyText;
				}

				strSalesText += m_strItemText;						// add to combined description
				strSalesText += strRefund;							// refund character if required
			}
			break;
			}
		}
	}

	if ( strSalesText.GetLength() > 255 )
	{
		strSalesText.SetAt(252, '.');
		strSalesText.SetAt(253, '.');
		strSalesText.SetAt(254, '\x00' );
	}
}

//********************************************************************

CString CTLogRecord::GetXmlDate ( bool bMilliSecondsReqd )
{
	CString strXmlDate;
	strXmlDate.Format ( "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:00", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(), m_time.GetHour(), m_time.GetMinute() );

	if (bMilliSecondsReqd == TRUE)
	{
		strXmlDate += ":000";
	}

	return strXmlDate;
}

//********************************************************************

