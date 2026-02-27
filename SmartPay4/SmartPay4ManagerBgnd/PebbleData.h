#pragma once
//*******************************************************************
#define nPEBBLE_LOCATEBY_UPN			1		// by UPN
#define nPEBBLE_LOCATEBY_MIS			2		// by Person_id
//*******************************************************************

namespace PebbleDataFields
{
	const char* const InstallationID = "InstallationID";
	const char* const OrganisationID = "OrganisationID";
	const char* const MISInfoIndex = "MISInfoIndex";
	const char* const UPNInfoIndex = "UPNInfoIndex";
};

//*******************************************************************

class CPebbleData
{
public:
	CPebbleData();
	void Read();
	void Save();
	bool IsSet();

public:
	CString GetInstallationID() { return m_strInstallationID; }
	CString GetOrganisationID() { return m_strOrganisationID; }
	int GetMISInfoIndex() { return m_nMISInfoIndex; }
	int GetUPNInfoIndex() { return m_nUPNInfoIndex; }

public:
	void SetInstallationID(CString str) { m_strInstallationID = str; }
	void SetOrganisationID(CString str) { m_strOrganisationID = str; }
	void SetMISInfoIndex(int n);
	void SetUPNInfoIndex(int n);

public:
	CString GetLocateByLabelMisID();
	CString GetLocateByLabelUPN();
	CString GetLocateByText(int n);

public:
	CString GetWebPaymentListFilename() { return System.GetWebPaymentPath("Pblist.dat", nWEBPAYMENT_PEBBLE); }
	CString GetImportDataFilename() { return System.GetWebPaymentPath("PbFile01.dat", nWEBPAYMENT_PEBBLE); }
	CString GetWebPaymentExceptionFilename() { return System.GetWebPaymentPath("PbFile02.dat", nWEBPAYMENT_PEBBLE); }
	CString GetUpdatesFilename() { return System.GetWebPaymentPath("PbFile03.dat", nWEBPAYMENT_PEBBLE); }
	CString GetBalanceDBListFilename() { return System.GetWebPaymentPath("PbFile04.dat", nWEBPAYMENT_PEBBLE); }

private:
	CString GetTextLabel(int n);

private:
	CString m_strInstallationID;
	CString m_strOrganisationID;
	int m_nMISInfoIndex;
	int m_nUPNInfoIndex;

private:
	CString m_strFilename;
};

//*******************************************************************
