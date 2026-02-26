/******************************************************************************/
#include "TLogRecord_base.h"
/******************************************************************************/
//#define nVERSION			1			// v1 = nType, szNo, szText, dQty, dValue
//#define nVERSION			2			// v2 = nType, szNo, szText, dQty, dValue, points
//#define nVERSION			3			// v3 = TerminalNo added before ItemData
//#define nVERSION			4			// v4 = Purse spend + purse balances added before ItemData
//#define nVERSION			5			// v5 = Item data includes flags for renewal and revalue
//#define nVERSION			6			// v6 = DbNo added before item data.
#define nVERSION			7			// v7 = FolderSet added before item data.
/******************************************************************************/
#define nITEMFIELDS			7			
/******************************************************************************/
#define	nOFFSET_TYPE		0
#define nOFFSET_PLUNO		1
#define nOFFSET_TEXT		2
#define nOFFSET_QTY			3
#define nOFFSET_VALUE		4
#define nOFFSET_POINTS		5
#define nOFFSET_FLAGS		6
/**********************************************************************/

CTLogRecord_base::CTLogRecord_base() : m_csvItemData(',', '"', TRUE, TRUE, TRUE)
{
	Reset();
}

/**********************************************************************/

CTLogRecord_base::CTLogRecord_base(CString strLine)
{
	Reset();
	CCSV csv(strLine);
	m_date.SetDate(csv.GetString(0));			// date
	m_time.SetTime(csv.GetString(1));			// time
}

/**********************************************************************/

void CTLogRecord_base::Reset()
{
	m_date.SetDate("");
	m_time.SetTime("");
	m_nDbNo = 1;
	m_nFolderSet = 0;
	m_nTerminalNo = 0;
	m_nCCNo = 0;
	m_dPurse1Spend = 0.0;
	m_dPurse2Spend = 0.0;
	m_dPurse1 = 0.0;
	m_dPurse2 = 0.0;
	m_csvItemData.RemoveAll();
}

//**********************************************************************
// calc number of individual plu sale items in a line

int CTLogRecord_base::GetItemCounter()
{
	return (m_csvItemData.GetSize() / nITEMFIELDS);
}

//**********************************************************************

bool CTLogRecord_base::WriteLine(CSSFile* file)
{
	CCSV csv;
	csv.Add(m_date.GetCSVDate());
	csv.Add(m_time.GetCSVTime());
	csv.Add(nVERSION);
	csv.Add(m_nDbNo);								// added in v6
	csv.Add(m_nFolderSet);							// added in v7
	csv.Add(m_nTerminalNo);							// added in v3
	csv.Add(m_nCCNo);
	csv.Add(m_dPurse1Spend, nTLOGFILE_DPVALUE);
	csv.Add(m_dPurse2Spend, nTLOGFILE_DPVALUE);		// added in v4
	csv.Add(m_dPurse1, nTLOGFILE_DPVALUE);			// added in v4
	csv.Add(m_dPurse2, nTLOGFILE_DPVALUE);			// added in v4

	csv.Add(GetItemCounter());						// number of plu sale items
	for (int nIndex = 0; nIndex < m_csvItemData.GetSize(); nIndex++)
	{
		csv.Add(m_csvItemData.GetString(nIndex));
	}

	return file->Write(csv);
}

//*********************************************************************
// read line & convert to latest version type ( if required )

bool CTLogRecord_base::ReadLine(CSSFile* file, bool bFullRead)
{
	CCSV csv;
	bool bReply = file->Read(csv);

	if (TRUE == bReply)
	{
		m_date.SetDate(csv.GetString(0));				// date
		m_time.SetTime(csv.GetString(1));				// time

		if (TRUE == bFullRead)							// see if full read of line required
		{
			int nVersionNo = csv.GetInt(2);				// get record version

			switch (nVersionNo)
			{
			case 1:
				ReadDataV1(csv);
				break;

			case 2:
				ReadDataV2(csv);
				break;

			case 3:
				ReadDataV3(csv);
				break;

			case 4:
				ReadDataV4(csv);
				break;

			case 5:
				ReadDataV5(csv);
				break;

			case 6:
				ReadDataV6(csv);
				break;

			case 7:
				ReadDataV7(csv);
				break;

			default:
				bReply = FALSE;
				break;
			}
		}
	}
	return bReply;
}

//*******************************************************************

void CTLogRecord_base::ReadDataV1(CCSV& csv)
{
	m_nCCNo = csv.GetInt(3);
	m_dPurse1Spend = csv.GetDouble(4);

	int nCount = csv.GetInt(5);						
	ReadItemFields(csv, 6, nCount, nV1_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord_base::ReadDataV2(CCSV& csv)
{
	m_nCCNo = csv.GetInt(3);
	m_dPurse1Spend = csv.GetDouble(4);

	int nCount = csv.GetInt(5);						
	ReadItemFields(csv, 6, nCount, nV2_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord_base::ReadDataV3(CCSV& csv)
{
	m_nTerminalNo = csv.GetInt(3);						
	m_nCCNo = csv.GetInt(4);
	m_dPurse1Spend = csv.GetDouble(5);

	int nCount = csv.GetInt(6);						
	ReadItemFields(csv, 7, nCount, nV3_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord_base::ReadDataV4(CCSV& csv)
{
	m_nTerminalNo = csv.GetInt(3);
	m_nCCNo = csv.GetInt(4);
	m_dPurse1Spend = csv.GetDouble(5);
	m_dPurse2Spend = csv.GetDouble(6);
	m_dPurse1 = csv.GetDouble(7);
	m_dPurse2 = csv.GetDouble(8);

	int nCount = csv.GetInt(9);
	ReadItemFields(csv, 10, nCount, nV4_ITEMFIELDS);
}

//*********************************************************************

void CTLogRecord_base::ReadDataV5(CCSV& csv)
{
	m_nTerminalNo = csv.GetInt(3);
	m_nCCNo = csv.GetInt(4);
	m_dPurse1Spend = csv.GetDouble(5);
	m_dPurse2Spend = csv.GetDouble(6);
	m_dPurse1 = csv.GetDouble(7);
	m_dPurse2 = csv.GetDouble(8);

	int nCount = csv.GetInt(9);
	ReadItemFields(csv, 10, nCount, nV5_ITEMFIELDS);
}

//*********************************************************************

void CTLogRecord_base::ReadDataV6(CCSV& csv)
{
	m_nDbNo = csv.GetInt(3);
	m_nTerminalNo = csv.GetInt(4);
	m_nCCNo = csv.GetInt(5);
	m_dPurse1Spend = csv.GetDouble(6);
	m_dPurse2Spend = csv.GetDouble(7);
	m_dPurse1 = csv.GetDouble(8);
	m_dPurse2 = csv.GetDouble(9);

	int nCount = csv.GetInt(10);
	ReadItemFields(csv, 11, nCount, nV6_ITEMFIELDS);
}

//*********************************************************************

void CTLogRecord_base::ReadDataV7(CCSV& csv)
{
	m_nDbNo = csv.GetInt(3);
	m_nFolderSet = csv.GetInt(4);
	m_nTerminalNo = csv.GetInt(5);
	m_nCCNo = csv.GetInt(6);
	m_dPurse1Spend = csv.GetDouble(7);
	m_dPurse2Spend = csv.GetDouble(8);
	m_dPurse1 = csv.GetDouble(9);
	m_dPurse2 = csv.GetDouble(10);

	int nCount = csv.GetInt(11);
	ReadItemFields(csv, 12, nCount, nV6_ITEMFIELDS);
}

//*********************************************************************

void CTLogRecord_base::ReadItemFields(CCSV& csv, int nFieldIdx, int nItemCount, int nItemFields)
{
	if ((nItemFields >= 5) && (nItemFields <= 7))
	{
		for (int n = 0; n < nItemCount; n++)
		{
			for (int i = 0; i < nItemFields; i++)
			{
				m_csvItemData.Add(csv.GetString(nFieldIdx++));
			}

			//points
			if (nItemFields < 6)
			{
				m_csvItemData.Add("0");
			}

			//renewal flag
			if (nItemFields < 7)
			{
				m_csvItemData.Add("-1");
			}
		}
	}
}

//*********************************************************************

void CTLogRecord_base::SaveItemData(int nType, CString strPluNo, CString strText, double dQty, double dValue, int nPoints, int nFlags)
{
	if (IsVoidItem(nType, strPluNo, strText, nFlags, dQty, dValue) == FALSE)	// see if line is a void of previous item data in this sale
	{
		if (IsDuplicate(nType, strPluNo, strText, nFlags, dQty, dValue, nPoints) == FALSE)	// see if line alreday exists, if does just inc qty & value
		{
			m_csvItemData.Add(nType);						// 0-plu, 1=dept, 2=discount
			m_csvItemData.Add(strPluNo);					// plu/dep no
			m_csvItemData.Add(strText);						// description
			m_csvItemData.Add(dQty, nTLOGFILE_DPQTY);		// qty
			m_csvItemData.Add(dValue, nTLOGFILE_DPVALUE);	// value
			m_csvItemData.Add(nPoints);						// points
			m_csvItemData.Add(nFlags);						// flags
		}
	}
}

//********************************************************************
// extract data from individual sale items
//
//void CTLogRecord::GetItemData ( int nIndex, int& nType, CString& strNo, CString& strText, double& dQty, double& dValue, int& nPoints )
//{
//	int n = ( nIndex * nITEMFIELDS );							// index to description
//
//	nType	= m_csvItemData.GetInt(n);
//	strNo	= m_csvItemData.GetString(n+1);
//	strText	= m_csvItemData.GetString(n+2);
//	dQty	= m_csvItemData.GetDouble(n+3);
//	dValue	= m_csvItemData.GetDouble(n+4);
//	nPoints = m_csvItemData.GetInt (n+5);
//}
//********************************************************************
// extract data from individual sale items

void CTLogRecord_base::ExtractItemData(int nIndex)
{
	int n = (nIndex * nITEMFIELDS);							

	m_nItemType = m_csvItemData.GetInt(n);
	m_strItemNo = m_csvItemData.GetString(n + 1);
	m_strItemText = m_csvItemData.GetString(n + 2);
	m_dItemLineQty = m_csvItemData.GetDouble(n + 3);
	m_dItemLineVal = m_csvItemData.GetDouble(n + 4);
	m_nItemPoints = m_csvItemData.GetInt(n + 5);
	m_nItemFlags = m_csvItemData.GetInt(n + 6);
}


//********************************************************************
// see if can remove void items out of current transaction

bool CTLogRecord_base::IsVoidItem(int nType, CString strPluNo, CString strText, int nFlags, double dQty, double dValue)
{
	for (int n = 0; n < GetItemCounter(); n++)
	{
		int nIndex = (n * nITEMFIELDS);									// index to type

		if (nType == m_csvItemData.GetInt(nIndex))		// type same
		{
			if (strcmp(strPluNo, m_csvItemData.GetString(nIndex + nOFFSET_PLUNO)) == 0)		// same plu/dept no
			{
				if (strcmp(strText, m_csvItemData.GetString(nIndex + nOFFSET_TEXT)) == 0)	// text same
				{
					if (nFlags == m_csvItemData.GetInt(nIndex + nOFFSET_FLAGS)) // flags same
					{
						if (dQty == -(m_csvItemData.GetDouble(nIndex + nOFFSET_QTY)))			// opposite qty
						{
							if (dValue == -(m_csvItemData.GetDouble(nIndex + nOFFSET_VALUE)))	// opposite value
							{
								m_csvItemData.RemoveAt(nIndex, nITEMFIELDS);
								return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

//********************************************************************
// see if items alreday exists, if it does just inc existing qty & value
// save having multiple lines in the transaction

bool CTLogRecord_base::IsDuplicate(int nType, CString strPluNo, CString strText, int nFlags, double dQty, double dValue, int nPoints)
{
	for (int n = 0; n < GetItemCounter(); n++)
	{
		int nIndex = (n * nITEMFIELDS);									// index to type

		if (nType == m_csvItemData.GetInt(nIndex))						// type same
		{
			if (strcmp(strPluNo, m_csvItemData.GetString(nIndex + nOFFSET_PLUNO)) == 0)	// same plu/dept no
			{
				if (strcmp(strText, m_csvItemData.GetString(nIndex + nOFFSET_TEXT)) == 0)// text same
				{
					if (nFlags == m_csvItemData.GetInt(nIndex + nOFFSET_FLAGS)) // flags same
					{
						dQty += m_csvItemData.GetDouble(nIndex + nOFFSET_QTY);		// existing qty
						m_csvItemData.SetAt(nIndex + 3, dQty, nTLOGFILE_DPQTY);

						dValue += m_csvItemData.GetDouble(nIndex + nOFFSET_VALUE);	// existing value
						m_csvItemData.SetAt(nIndex + 4, dValue, nTLOGFILE_DPVALUE);

						nPoints += m_csvItemData.GetInt(nIndex + nOFFSET_POINTS);		// existing value
						m_csvItemData.SetAt(nIndex + 5, nPoints);

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

//********************************************************************
// work through allocated points & apply bonus multiplier

void CTLogRecord_base::ApplyBonusPointsMultiplier(int nBonusMultiplier)
{
	if (nBonusMultiplier != 1)
	{
		for (int n = 0; n < GetItemCounter(); n++)
		{
			int nIndex = (n * nITEMFIELDS) + nOFFSET_POINTS;		// existing points

			int nPoints = m_csvItemData.GetInt(nIndex);
			nPoints *= nBonusMultiplier;
			m_csvItemData.SetAt(nIndex, nPoints);
		}
	}
}

//********************************************************************
