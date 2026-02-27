//*******************************************************************************
// CAUTION : Changes to the format will required the ConvertRecord of Derived in 
// TLogRecord to be also changed *
//*******************************************************************************
#include "TLogRecord_base.h"
//*******************************************************************************
#define	nOFFSET_TYPE		0
#define nOFFSET_PLUNO		1
#define nOFFSET_TEXT		2
#define nOFFSET_LINEQTY		3
#define nOFFSET_LINEVAL		4
#define nOFFSET_POINTS		5
#define nOFFSET_TAXCODE		6
#define nOFFSET_ITEMVAL		7
//*******************************************************************************

CTLogRecord_base::CTLogRecord_base() : m_csvItemData ( ',', '"', TRUE, TRUE, TRUE )
{
	Reset();
	Init();
}

//*******************************************************************************

CTLogRecord_base::CTLogRecord_base ( const char* szLine )
{
	Init();
	CCSV csv ( szLine );
	m_date.SetDate ( csv.GetString(0) );				// date
	m_time.SetTime ( csv.GetString(1) );				// time
}

//*******************************************************************************

void CTLogRecord_base::Reset()
{
	m_date.SetDate("");
	m_time.SetTime("");
	m_nCCNo = 0;
	m_nFolderSet = 0;
	m_nTerminalNo = 0;
	m_strReceiptNo = "";
	m_dPurse1Spend = 0.0;
	m_dPurse2Spend = 0.0;
	m_dPurse3Spend = 0.0;
	m_dPurse1 = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse3 = 0.0;

	m_csvItemData.RemoveAll();
}

//*******************************************************************************

void CTLogRecord_base::Init()
{
	m_nFolderSet = 0;
	m_nTerminalNo = 0;
	m_nItemType = 0;
	m_nItemPoints = 0;
	m_nItemDeptNo = 0;
	m_nCCNo = 0;
	m_dPurse1 = 0.0;
	m_dPurse1Spend = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse2Spend = 0.0;
	m_dPurse3 = 0.0;
	m_dPurse3Spend = 0.0;
	m_dLineQty = 0.0;
	m_nPerItemVal = 0;
	m_dLineVal = 0.0;
}

//*******************************************************************************
// calc number of individual plu sale items in a line

int CTLogRecord_base::GetItemCounter()
{
	 return ( m_csvItemData.GetSize() / nTLOGREC_ITEMFIELDS );
}

//*******************************************************************************
// CAUTION : Changes to the format will required the ConvertRecord of Derived in 
// TLogRecord to be also changed 
//*******************************************************************************

bool CTLogRecord_base::WriteLine ( CSSFile* file )
{
	CCSV csv;
	csv.Add ( m_date.GetCSVDate() );
	csv.Add ( m_time.GetCSVTime() );
	csv.Add ( nTLOGREC_VERSION );						// save in latest version
	csv.Add ( m_nFolderSet);							// added in v6
	csv.Add ( m_nTerminalNo );							// added in v2
	csv.Add ( m_nCCNo );
	csv.Add ( m_strReceiptNo );							// added in v4
	csv.Add ( m_dPurse1Spend, nTLOGREC_DPVALUE );
	csv.Add ( m_dPurse2Spend, nTLOGREC_DPVALUE );		// added in v2
	csv.Add ( m_dPurse3Spend, nTLOGREC_DPVALUE );		// added in v3
	csv.Add ( m_dPurse1, nTLOGREC_DPVALUE );			// added in v2
	csv.Add ( m_dPurse2, nTLOGREC_DPVALUE );			// added in v2
	csv.Add ( m_dPurse3, nTLOGREC_DPVALUE );			// added in v3
	csv.Add ( GetItemCounter() );						// number of transaction items

	for (int nIndex = 0; nIndex < m_csvItemData.GetSize(); nIndex++)
	{
		csv.Add(m_csvItemData.GetString(nIndex));
	}

	return file->Write ( csv );							// audit record line
}

//*******************************************************************************
// CAUTION : Changes to the format will required the ConvertRecord of Derived in 
// TLogRecord to be also changed 
//*******************************************************************************

bool CTLogRecord_base::ReadLine(CSSFile* file, bool bFullRead)
{
	CCSV csv{};
	bool bReply = file->Read(csv);

	if (TRUE == bReply)
	{
		m_date.SetDate(csv.GetString(0));					// date
		m_time.SetTime(csv.GetString(1));					// time

		if (bFullRead == TRUE)								// see if full read of line required
		{
			int nVersion = csv.GetInt(2);

			switch (nVersion)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				ReadDataV1to6(csv, nVersion);
				break;

			default:
				bReply = FALSE;
				break;
			}
		}
	}

	return bReply;
}

//*******************************************************************************

void CTLogRecord_base::ReadDataV1to6(CCSV& csvLine, int nVer)
{
	int nPos = 3;

	if (nVer >= 6)
	{
		m_nFolderSet = csvLine.GetInt(nPos++);
	}

	if (nVer >= 2)
	{
		m_nTerminalNo = csvLine.GetInt(nPos++);
	}

	m_nCCNo = csvLine.GetInt(nPos++);

	if (nVer >= 4)
	{
		m_strReceiptNo = csvLine.GetString(nPos++);
	}

	m_dPurse1Spend = csvLine.GetDouble(nPos++);

	if (nVer >= 2)
	{
		m_dPurse2Spend = csvLine.GetDouble(nPos++);
	}

	if (nVer >= 3)
	{
		m_dPurse3Spend = csvLine.GetDouble(nPos++);
	}

	if (nVer >= 2)
	{
		m_dPurse1 = csvLine.GetDouble(nPos++);
		m_dPurse2 = csvLine.GetDouble(nPos++);
	}

	if (nVer >= 3)
	{
		m_dPurse3 = csvLine.GetDouble(nPos++);
	}

	int nCount = csvLine.GetInt(nPos++);

	int nItemFields = 0;
	switch (nVer)
	{
	case 1:
	case 2:
		nItemFields = nTLOGREC_ITEMFIELDSv2;
		break;

	case 3:
		nItemFields = nTLOGREC_ITEMFIELDSv3;
		break;

	case 4:
		nItemFields = nTLOGREC_ITEMFIELDSv4;
		break;

	case 5:
		nItemFields = nTLOGREC_ITEMFIELDSv5;
		break;

	case 6:
		nItemFields = nTLOGREC_ITEMFIELDS;
		break;
	}

	for (int n = 0; n < nCount; n++)
	{
		for (int i = 0; i < nItemFields; i++)
		{
			m_csvItemData.Add(csvLine.GetString(nPos++));
		}

		for (int i = nItemFields; i < nTLOGREC_ITEMFIELDS; i++)
		{
			m_csvItemData.Add("");
		}
	}
}

//*******************************************************************************

void CTLogRecord_base::SaveItemData ( int nType, const char* szNo, const char* szText, double dLineQty, int nPerItemVal, double dLineVal, int nPoints, const char* szTaxCode )
{
	if ( IsVoidItem ( nType, szNo, szText, dLineQty, dLineVal ) == FALSE )		// see if line is a void of previous item data in this sale
	{
		if ( IsDuplicate ( nType, szNo, szText, dLineQty, nPerItemVal, dLineVal, nPoints, szTaxCode ) == FALSE )	// see if line already exists, if does just inc qty & value
		{
			m_csvItemData.Add ( nType );						// 0-plu, 1=dept, 2=discount
			m_csvItemData.Add ( szNo );							// plu/dep no
			m_csvItemData.Add ( szText );						// description
			m_csvItemData.Add ( dLineQty, nTLOGREC_DPQTY );		// qty
			m_csvItemData.Add ( dLineVal, nTLOGREC_DPVALUE );	// value
			m_csvItemData.Add ( nPoints );						// Healthy Eating Points
			m_csvItemData.Add ( szTaxCode );					// tax code
			m_csvItemData.Add ( nPerItemVal );					// tax code
		}
	}
}

//*******************************************************************************
// extract data from individual sale items

void CTLogRecord_base::ExtractItemData(int nIndex)
{
	int n = (nIndex * nTLOGREC_ITEMFIELDS);							// index to description

	m_nItemType = m_csvItemData.GetInt(n);
	CString strBuf = m_csvItemData.GetString(n + 1);
	m_strItemText = m_csvItemData.GetString(n + 2);
	m_dLineQty = m_csvItemData.GetDouble(n + 3);
	m_dLineVal = m_csvItemData.GetDouble(n + 4);
	m_nItemPoints = m_csvItemData.GetInt(n + 5);
	m_strItemTax = m_csvItemData.GetString(n + 6);
	m_nPerItemVal = m_csvItemData.GetInt(n + 7);

	m_strItemNo = "";
	m_nItemDeptNo = 0;

	switch (m_nItemType)
	{
	case nTLOGTYPE_PLU:
	case nTLOGTYPE_DISC_ITEM:
		m_strItemNo = strBuf;
		break;

	case nTLOGTYPE_DEPT:
		m_nItemDeptNo = atoi(strBuf);
		break;

	case nTLOGTYPE_DISC_SUBT:
	case nTLOGTYPE_PROMOTION:
		break;

	case nTLOGTYPE_PLU2:
		m_nItemDeptNo = atoi(strBuf.Left(4));		// DeptNo ( fixed 4 digits ) followed by PluNumber 1- 13						
		m_strItemNo = strBuf.Mid(4);
		break;

	case nTLOGTYPE_SURCHARGE:
		m_strItemNo = strBuf;							// Group% discount					
		break;
	}
}

//*******************************************************************************
// see if can remove void items out of current transaction
// no checking on Points as that doesn't come from the till

bool CTLogRecord_base::IsVoidItem ( int nType, const char* szNo, const char* szText, double dQty, double dValue )
{
	for ( int n = 1 ; n <= GetItemCounter() ; n++ )
	{
		int nIndex = ( (n-1) * nTLOGREC_ITEMFIELDS );			// index to type

		if ( nType == m_csvItemData.GetInt(nIndex) )	// type same
		{
			if ( strcmp ( szNo, m_csvItemData.GetString(nIndex+nOFFSET_PLUNO) ) == 0 )			// same plu/dept no
			{
				if ( strcmp ( szText, m_csvItemData.GetString(nIndex+nOFFSET_TEXT) ) == 0 )		// text same
				{
					if ( dQty == -(m_csvItemData.GetDouble(nIndex+nOFFSET_LINEQTY )) )				// opposite qty
					{
						if ( dValue == -(m_csvItemData.GetDouble(nIndex+nOFFSET_LINEVAL )) )		// opposite value
						{
							m_csvItemData.RemoveAt(nIndex, nTLOGREC_ITEMFIELDS );
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

//*******************************************************************************
// see if item already exists, if it does just inc existing qty & value
// save having multiple lines in the transaction

bool CTLogRecord_base::IsDuplicate ( int nType, const char* szNo, const char* szText, double dLineQty, int nPerItemVal, double dLineVal, int nPoints, const char* szTaxCode )
{
	for ( int n = 1 ; n <= GetItemCounter() ; n++ )
	{
		int nIndex = ( (n-1) * nTLOGREC_ITEMFIELDS );			// index to type

		if ( nType == m_csvItemData.GetInt(nIndex) )			// type same
		{
			if ( strcmp ( szNo, m_csvItemData.GetString(nIndex+nOFFSET_PLUNO) ) == 0 )				// same plu/dept no
			{
				if ( strcmp ( szText, m_csvItemData.GetString(nIndex+nOFFSET_TEXT) ) == 0 )			// text same
				{
					if ( strcmp ( szTaxCode, m_csvItemData.GetString(nIndex+nOFFSET_TAXCODE) ) == 0 )// taxcode same
					{
						if (nPerItemVal == m_csvItemData.GetInt(nIndex + nOFFSET_ITEMVAL))
						{
							dLineQty += m_csvItemData.GetDouble(nIndex + nOFFSET_LINEQTY);		// existing qty
							m_csvItemData.SetAt(nIndex + nOFFSET_LINEQTY, dLineQty, nTLOGREC_DPQTY);

							dLineVal += m_csvItemData.GetDouble(nIndex + nOFFSET_LINEVAL);	// existing value
							m_csvItemData.SetAt(nIndex + nOFFSET_LINEVAL, dLineVal, nTLOGREC_DPVALUE);

							nPoints += m_csvItemData.GetInt(nIndex + nOFFSET_POINTS);		// existing value
							m_csvItemData.SetAt(nIndex + nOFFSET_POINTS, nPoints);

							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

//********************************************************************

