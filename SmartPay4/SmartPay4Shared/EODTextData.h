#pragma once

/**********************************************************************/

#define nFLAGS					0			
#define nPOSTOPUP				1			// index into CStringArray holding texts
#define nPOSTOPUPREFUND			2
#define nOFFICETOPUP			3
#define nOFFICETOPUPREFUND		4
#define nIMPORTTOPUP			5
#define nIMPORTTOPUPREFUND		6
#define nOFFICEADJUSTMENT		7
#define nKIOSKTOPUP				8
#define nWEBPAYMENTTOPUP		9
#define nWEBPAYMENTREFUND		10
#define nPOSSALEREFUND			11
#define nOFFICESALEADJUSTMENT	12
#define nOFFICEDELETION			13
#define nPURSE1TEXT				14
#define nPURSE2TEXT				15
#define nPURSE3TEXT				16
#define nCASHTEXT				17
#define nOFFICEMEAL				18
#define nCREDITADJUSTPCUP		19
#define nCREDITADJUSTPCDOWN		20
#define nCREDITADJUSTECRUP		21
#define nCREDITADJUSTECRDOWN	22
#define nPEBBLEORDER			23
#define nPEBBLEORDERREFUND		24
#define nMAXLINES				25

/**********************************************************************/

class CEODTextData
{
public:
	CEODTextData();
	void Read();
	void Save();

	CString GetText(int nLabel);
	void SetText(int nIndex, const char* szText) { m_arrayTexts.SetAt(nIndex, szText); }

	bool IsZeroSkip() { return m_csvFlags.GetBool(0); }
	void SetZeroSkip(bool bSet) { m_csvFlags.SetAt(0, bSet); }

	int GetSaveAsType() { return m_csvFlags.GetInt(1); }
	void SetSaveAsType(int n) { m_csvFlags.SetAt(1, n); }

	bool ExcludePeriodSales() { return m_csvFlags.GetBool(2); }
	void SetExcludePeriodSales(bool bSet) { m_csvFlags.SetAt(2, bSet); }

	bool ExcludeEposPayments() { return m_csvFlags.GetBool(3); }
	void SetExcludeEposPayments(bool bSet) { m_csvFlags.SetAt(3, bSet); }

	bool ExcludeSalesBreakdown() { return m_csvFlags.GetBool(4); }
	void SetExcludeSalesBreakdown(bool bSet) { m_csvFlags.SetAt(4, bSet); }

	bool IsZeroSkipVAT() { return m_csvFlags.GetBool(5); }
	void SetZeroSkipVAT(bool bSet) { m_csvFlags.SetAt(5, bSet); }

	CString GetSaveAsFilename(const char* szReportFile);

private:
	CString m_strFilename = "";
	CFileStringArray m_arrayTexts;
	CCSV m_csvFlags;
};

/**********************************************************************/
