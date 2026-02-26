//*********************************************************************
#include <math.h>
//*********************************************************************
#include "TLogRecord.h"
//*********************************************************************

CTLogRecord::CTLogRecord() : CTLogRecord_base()
{
}

//*********************************************************************

CTLogRecord::CTLogRecord(CString strLine) : CTLogRecord_base(strLine)
{
}

//*********************************************************************

CString CTLogRecord::MakeLabel(int nNo)
{
	CString strLabel = "";
	strLabel.Format("col%d", nNo);
	return strLabel;
}

//*********************************************************************
// convert a TlogRecord using implied header database record ( col1,....,colnn )

bool CTLogRecord::ConvertRecord(CTLogDatabase* pDb)
{
	bool bReply;
	m_date.SetDate(pDb->GetString("col1"));			// date

	if ((bReply = m_date.IsSet()) == TRUE)
	{
		m_time.SetTime(pDb->GetString("col2"));		// time
		int nVersion = pDb->GetInt("col3");			// version number

		switch (nVersion)
		{
		case 1:
			ConvertDataV1(pDb);
			break;

		case 2:
			ConvertDataV2(pDb);
			break;

		case 3:
			ConvertDataV3(pDb);
			break;

		case 4:
			ConvertDataV4(pDb);
			break;

		case 5:
			ConvertDataV5(pDb);
			break;

		case 6:
			ConvertDataV6(pDb);
			break;

		case 7:
			ConvertDataV7(pDb);
			break;

		default:
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*******************************************************************

void CTLogRecord::ConvertDataV1(CTLogDatabase* pDb)
{
	m_nCCNo = pDb->GetInt("col4");		
	m_dPurse1Spend = pDb->GetDouble("col5");	
	int nCount = pDb->GetInt("col6");		

	ConvertItemData(pDb, nCount, 7, nV1_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord::ConvertDataV2(CTLogDatabase* pDb)
{
	m_nCCNo = pDb->GetInt("col4");		
	m_dPurse1Spend = pDb->GetDouble("col5");		
	int nCount = pDb->GetInt("col6");		

	ConvertItemData(pDb, nCount, 7, nV2_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord::ConvertDataV3(CTLogDatabase* pDb)
{
	m_nTerminalNo = pDb->GetInt("col4");		
	m_nCCNo = pDb->GetInt("col5");		
	m_dPurse1Spend = pDb->GetDouble("col6");		
	int nCount = pDb->GetInt("col7");		

	ConvertItemData(pDb, nCount, 8, nV3_ITEMFIELDS);
}

//*******************************************************************

void CTLogRecord::ConvertDataV4(CTLogDatabase* pDb)
{
	m_nTerminalNo = pDb->GetInt("col4");		
	m_nCCNo = pDb->GetInt("col5");		
	m_dPurse1Spend = pDb->GetDouble("col6");		
	m_dPurse2Spend = pDb->GetDouble("col7");		
	m_dPurse1 = pDb->GetDouble("col8");		
	m_dPurse2 = pDb->GetDouble("col9");		
	int nCount = pDb->GetInt("col10");		

	ConvertItemData(pDb, nCount, 11, nV4_ITEMFIELDS);
}

//********************************************************************

void CTLogRecord::ConvertDataV5(CTLogDatabase* pDb)
{
	m_nTerminalNo = pDb->GetInt("col4");		
	m_nCCNo = pDb->GetInt("col5");		
	m_dPurse1Spend = pDb->GetDouble("col6");		
	m_dPurse2Spend = pDb->GetDouble("col7");		
	m_dPurse1 = pDb->GetDouble("col8");		
	m_dPurse2 = pDb->GetDouble("col9");		
	int nCount = pDb->GetInt("col10");		

	ConvertItemData(pDb, nCount, 11, nV5_ITEMFIELDS);
}

//********************************************************************

void CTLogRecord::ConvertDataV6(CTLogDatabase* pDb)
{
	m_nDbNo = pDb->GetInt("col4");		
	m_nTerminalNo = pDb->GetInt("col5");		
	m_nCCNo = pDb->GetInt("col6");		
	m_dPurse1Spend = pDb->GetDouble("col7");		
	m_dPurse2Spend = pDb->GetDouble("col8");		
	m_dPurse1 = pDb->GetDouble("col9");		
	m_dPurse2 = pDb->GetDouble("col10");		
	int nCount = pDb->GetInt("col11");	

	ConvertItemData(pDb, nCount, 12, nV6_ITEMFIELDS);
}

//********************************************************************

void CTLogRecord::ConvertDataV7(CTLogDatabase* pDb)
{
	m_nDbNo = pDb->GetInt("col4");
	m_nFolderSet = pDb->GetInt("col5");
	m_nTerminalNo = pDb->GetInt("col6");
	m_nCCNo = pDb->GetInt("col7");
	m_dPurse1Spend = pDb->GetDouble("col8");
	m_dPurse2Spend = pDb->GetDouble("col9");
	m_dPurse1 = pDb->GetDouble("col10");
	m_dPurse2 = pDb->GetDouble("col11");
	int nCount = pDb->GetInt("col12");

	ConvertItemData(pDb, nCount, 13, nV7_ITEMFIELDS);
}

//********************************************************************

void CTLogRecord::ConvertItemData(CTLogDatabase* pDb, int nCount, int nBaseField, int nItemFields)
{
	for (int n = 0; n < nCount; n++)
	{
		for (int i = 0; i < nItemFields; i++)
		{
			m_csvItemData.Add(pDb->GetString(MakeLabel(nBaseField + i)));
		}
		nBaseField += nItemFields;
	}

	if (nItemFields < 6)
	{
		m_csvItemData.Add("0");
	}

	if (nItemFields < 7)
	{
		m_csvItemData.Add("-1");
	}
}

//********************************************************************

bool CTLogRecord::GetCombinedValues(CString& strSalesText, double& dSalesValue)
{
	strSalesText = "";
	dSalesValue = 0.0;

	for (int nIndex = 0; nIndex < GetItemCounter(); nIndex++)
	{
		ExtractItemData(nIndex);

		if (m_dItemLineVal != 0.0)
		{
			dSalesValue += m_dItemLineVal;

			switch (m_nItemType)
			{
			case nTLOGTYPE_PLU:
			case nTLOGTYPE_DEPT:
			{
				if (strSalesText.GetLength() > 0)
				{
					strSalesText += ", ";				// add separator
				}

				double dQty = m_dItemLineQty;
				if (dQty > 1)
				{
					double n;
					int nDps = (modf(dQty, &n) != 0.0) ? 1 : 0;

					CString strQtyText;
					strQtyText.Format("%.*lfx", nDps, dQty);		// 2xChips etc..
					strSalesText += strQtyText;
				}
				strSalesText += m_strItemText;						// add to combined description
			}
			break;

			default:
				break;
			}
		}
	}

	if (dSalesValue != 0.0)
	{
		if (strSalesText.GetLength() > 255)
		{
			strSalesText.SetAt(252, '.');
			strSalesText.SetAt(253, '.');
			strSalesText.SetAt(254, '\x00');
		}

		return TRUE;
	}

	return FALSE;
}

//**********************************************************************
