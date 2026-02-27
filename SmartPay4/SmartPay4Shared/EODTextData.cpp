/******************************************************************************/
#include "..\SmartPay4Shared\RepmanDefines.h"
/******************************************************************************/
#include "EODTextData.h"
/******************************************************************************/

CEODTextData::CEODTextData()
{
	m_strFilename = Filenames.GetEODTextsFilename();
}

//******************************************************************

void CEODTextData::Read()	
{
	bool bResetReqd = TRUE;
	if ( m_arrayTexts.Read ( m_strFilename ) == TRUE )
	{ 
		if (m_arrayTexts.GetCount() == nMAXLINES)
		{
			bResetReqd = FALSE;
		}
	}

	if (bResetReqd == TRUE)
	{
		m_arrayTexts.RemoveAll();
		m_arrayTexts.SetSize(nMAXLINES);

		SetZeroSkip(FALSE);
		SetSaveAsType(nSAVEAS_NONE);

		m_arrayTexts.SetAt(nFLAGS, m_csvFlags.GetLine());
		m_arrayTexts.SetAt(nPOSTOPUP, "Pos Topup");
		m_arrayTexts.SetAt(nPOSTOPUPREFUND, "Pos Topup Refund");
		m_arrayTexts.SetAt(nOFFICETOPUP, "Office Topup");
		m_arrayTexts.SetAt(nOFFICETOPUPREFUND, "Office Topup Refund");
		m_arrayTexts.SetAt(nIMPORTTOPUP, "Import Topup ");
		m_arrayTexts.SetAt(nIMPORTTOPUPREFUND, "Import Topup Refund");
		m_arrayTexts.SetAt(nOFFICEADJUSTMENT, "Office Adjustment");
		m_arrayTexts.SetAt(nKIOSKTOPUP, "Kiosk Topup");
		m_arrayTexts.SetAt(nWEBPAYMENTTOPUP, "Webpayment Topup");
		m_arrayTexts.SetAt(nWEBPAYMENTREFUND, "Webpayment Refund");
		m_arrayTexts.SetAt(nPOSSALEREFUND, "Pos Sale Refund");
		m_arrayTexts.SetAt(nOFFICESALEADJUSTMENT, "Office Sale Adjustment");
		m_arrayTexts.SetAt(nOFFICEDELETION, "Office Deletion");
		m_arrayTexts.SetAt(nPURSE1TEXT, "Purse1");
		m_arrayTexts.SetAt(nPURSE2TEXT, "Purse2");
		m_arrayTexts.SetAt(nPURSE3TEXT, "Purse3");
		m_arrayTexts.SetAt(nCASHTEXT, "Cash");
		m_arrayTexts.SetAt(nOFFICEMEAL, "Office Meal");
		m_arrayTexts.SetAt(nCREDITADJUSTPCUP, "Pc Credit Adjust (+)");
		m_arrayTexts.SetAt(nCREDITADJUSTPCDOWN, "Pc Credit Adjust (-)");
		m_arrayTexts.SetAt(nCREDITADJUSTECRUP, "Pos Credit Adjust (+)");
		m_arrayTexts.SetAt(nCREDITADJUSTECRDOWN, "Pos Credit Adjust (-)");
		m_arrayTexts.SetAt(nPEBBLEORDER, "Pebble Order");
		m_arrayTexts.SetAt(nPEBBLEORDERREFUND, "Pebble Order Refund");
	}
	else
	{
		CCSV csv(m_arrayTexts.GetAt(0));

		for (int i = 0; i < csv.GetSize(); i++)
		{
			m_csvFlags.SetAt(i, csv.GetString(i));
		}
	}
}

//*********************************************************************

void CEODTextData::Save()
{
	m_arrayTexts.SetAt ( nFLAGS, m_csvFlags.GetLine() );
	m_arrayTexts.Write ( m_strFilename );
}

//*********************************************************************

CString CEODTextData::GetText(int nIndex)
{
	CString strText = "";

	if ((nIndex > 0) && (nIndex < nMAXLINES))
	{
		strText = m_arrayTexts.GetAt(nIndex);
	}
	return strText;
}

//******************************************************************

CString CEODTextData::GetSaveAsFilename ( const char* szReportFile )
{
	CString strExt = "";
	switch ( GetSaveAsType() )
	{
	case nSAVEAS_NONE:
		break;

	case nSAVEAS_CSV:
		strExt = "csv";
		break;

	case nSAVEAS_HTML:	
		strExt = "htm";	
		break;
	}

	CString strFilename = "";
	strFilename.Format ( "%s.%s",
		(const char*) ::StripFileExtension(szReportFile),
		(const char*) strExt );
	
	return strFilename;
}

//******************************************************************
