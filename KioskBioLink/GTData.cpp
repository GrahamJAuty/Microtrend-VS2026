//****************************************************************************
#include <io.h>
//****************************************************************************
#include "Cardlink.h"
#include "SystemData.h"
#include "GlobalFunctions.h"
//****************************************************************************
#include "GTData.h"
//****************************************************************************

CGTData::CGTData()
{
}

//****************************************************************************

bool CGTData::Read()
{
	CSSIniFile file(SS_INI_AES);
	if (file.Read(System.m_strGTFilename) == TRUE)
	{
		m_bReadReqd = FALSE;
		m_nCCNo = file.GetInt("CCNO", m_nCCNo);
		m_lGT = file.GetLong("GT", m_lGT);
		m_lCID = file.GetLong("CID", m_lCID);
		m_nQty2000 = file.GetInt("Qty2000", m_nQty2000);
		m_nQty1000 = file.GetInt("Qty1000", m_nQty1000);
		m_nQty500 = file.GetInt("Qty500", m_nQty500);
		m_nQty200 = file.GetInt("Qty200", m_nQty200);
		m_nQty100 = file.GetInt("Qty100", m_nQty100);
		m_nQty50 = file.GetInt("Qty50", m_nQty50);
		m_nQty20 = file.GetInt("Qty20", m_nQty20);
		m_nQty10 = file.GetInt("Qty10", m_nQty10);
		m_lQtyOther = file.GetLong("QtyOther", m_lQtyOther);

		return TRUE;
	}
	return FALSE;
}

//****************************************************************************

bool CGTData::Write()
{
	CSSIniFile file(SS_INI_AES);

	file.Set("CCNO", m_nCCNo);
	file.Set("GT", m_lGT);
	file.Set("CID", m_lCID);
	file.Set("Qty2000", m_nQty2000);
	file.Set("Qty1000", m_nQty1000);
	file.Set("Qty500", m_nQty500);
	file.Set("Qty200", m_nQty200);
	file.Set("Qty100", m_nQty100);
	file.Set("Qty50", m_nQty50);
	file.Set("Qty20", m_nQty20);
	file.Set("Qty10", m_nQty10);
	file.Set("QtyOther", m_lQtyOther);

	return file.Write(System.m_strGTFilename);
}

//****************************************************************************

int CGTData::GetCCNo()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	return m_nCCNo;
}

//****************************************************************************

int CGTData::NextCCNo ( bool bSaveReqd )
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	int nCCNo = 1 + m_nCCNo;
	if (nCCNo > 9999)
	{
		nCCNo = 1;								// don't allow 0000
	}

	if ( TRUE == bSaveReqd )
	{
		m_nCCNo = nCCNo;
		Write();
	}

	return nCCNo;
}

//****************************************************************************

bool CGTData::SetCCNo(int nCCNo)
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	m_nCCNo = nCCNo;
	return Write();
}

//****************************************************************************

bool CGTData::SetGT ( double dValue )
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	CString strBuf = "";
	strBuf.Format ( "%.0lf", dValue * 100 );

	m_lGT = atol ( strBuf );
	return Write();
}

//****************************************************************************

long CGTData::GetGT()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	return m_lGT;
}

//****************************************************************************

double CGTData::GetGTValue()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	double dValue = (double)m_lGT / 100;
	return dValue;
}

//****************************************************************************

bool CGTData::UpdateGTTotals(long lValue)
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	m_lGT += lValue;
	m_lCID += lValue;

	switch (lValue)
	{
	case 2000:
		++m_nQty2000;
		break;

	case 1000:
		++m_nQty1000;
		break;

	case 500:
		++m_nQty500;
		break;

	case 200:
		++m_nQty200;
		break;

	case 100:	
		++m_nQty100;
		break;

	case 50:	
		++m_nQty50;	
		break;

	case 20:
		++m_nQty20;
		break;

	case 10:
		++m_nQty10;
		break;

	default:
		m_lQtyOther += lValue;
		break;
	}

	return Write();
}

//****************************************************************************

CString CGTData::GetValue2000()	{ return FormatValue ( (long)m_nQty2000 * 2000 );	}
CString CGTData::GetValue1000()	{ return FormatValue ( (long)m_nQty1000 * 1000 ); }
CString CGTData::GetValue500()	{ return FormatValue ( (long)m_nQty500 * 500 ); }
CString CGTData::GetValue200()	{ return FormatValue ( (long)m_nQty200 * 200 ); }
CString CGTData::GetValue100()	{ return FormatValue ( (long)m_nQty100 * 100 ); }
CString CGTData::GetValue50()	{ return FormatValue ( (long)m_nQty50 * 50 ); }
CString CGTData::GetValue20()	{ return FormatValue ( (long)m_nQty20 * 20 ); }
CString CGTData::GetValue10()	{ return FormatValue ( (long)m_nQty10 * 10 ); }

//****************************************************************************

long CGTData::GetCID()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	return m_lCID;
}

//****************************************************************************

CString CGTData::GetCIDValue()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	return FormatValue(m_lCID);
}

//****************************************************************************

bool CGTData::ClearCID()
{
	if (TRUE == m_bReadReqd)
	{
		Read();
	}

	ExportCID();							// export cid values to data file

	m_lCID = 0;
	m_nQty2000 = 0;
	m_nQty1000 = 0;
	m_nQty500 = 0;
	m_nQty200 = 0;
	m_nQty100 = 0;
	m_nQty50 = 0;
	m_nQty20 = 0;
	m_nQty10 = 0;
	m_lQtyOther = 0;

	return Write();
}

//****************************************************************************

bool CGTData::ExportCID()
{
	bool bReply = FALSE;

	CSSFile file;
	if ((bReply = file.Open(System.m_strGTExportFilename, "wb")) == TRUE)
	{
		CString strLine = "";

		strLine.Format("Date = %s", CSSDate::GetCurrentDate().GetDate());
		file.WriteLine(strLine);

		strLine.Format("Time = %s", CSSTime::GetCurrentTime().GetTime());
		file.WriteLine(strLine);

		strLine.Format("CID  = %s", (const char*)GetCIDValue());
		file.WriteLine(strLine);

		strLine.Format("£20  = %s", (const char*)GetValue2000());
		file.WriteLine(strLine);

		strLine.Format("£10  = %s", (const char*)GetValue1000());
		file.WriteLine(strLine);

		strLine.Format("£ 5  = %s", (const char*)GetValue500());
		file.WriteLine(strLine);

		strLine.Format("£ 2  = %s", (const char*)GetValue200());
		file.WriteLine(strLine);

		strLine.Format("£ 1  = %s", (const char*)GetValue100());
		file.WriteLine(strLine);

		strLine.Format("50p  = %s", (const char*)GetValue50());
		file.WriteLine(strLine);

		strLine.Format("20p  = %s", (const char*)GetValue20());
		file.WriteLine(strLine);

		strLine.Format("10p  = %s", (const char*)GetValue10());
		file.WriteLine(strLine);

		if (m_lQtyOther != 0)
		{
			strLine.Format("Other = %s", (const char*)FormatValue(m_lQtyOther));
			file.WriteLine(strLine);
		}

		file.Close();
	}

	return bReply;
}

//****************************************************************************

CString CGTData::FormatValue ( long lValue )
{
	CString strBuf = "";
	strBuf.Format ( "£ %.2lf", (double)lValue / 100 );
	return strBuf;
}

//****************************************************************************
