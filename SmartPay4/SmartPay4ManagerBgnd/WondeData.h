#pragma once

#define nWONDE_NEWINTAKE_MIS_INDEX		0		// create account by using MIS _id
#define nWONDE_NEWINTAKE_ADNO_INDEX		1		// create account by using Adno
#define nWONDE_NEWINTAKE_NEXTFREE_INDEX	2		// next free record

#define nWONDE_WEBPAYMENT_NONE_INDEX	0
#define nWONDE_WEBPAYMENT_MIS_INDEX		1
#define nWONDE_WEBPAYMENT_UPN_INDEX		2
#define nWONDE_WEBPAYMENT_ADNO_INDEX	3

#define nWONDE_LOCATEON_MIS_INDEX		0
#define nWONDE_LOCATEON_ADNO_INDEX		1
#define nWONDE_LOCATEON_UPN_INDEX		2

//***************************************************************

class CWondeData
{
public:
	CWondeData();
	bool Read();
	bool Save();

	const char* GetLocateOnLabel();
	const char* GetLocateOnText ( int nIndex );
	const char* GetWebPaymentText ( int nIndex );
	const char* GetNewIntakeText ( int nIndex );

public:
	CString m_strImportFilename;
	bool m_bDeleteImportFile;
	int m_nImportMethod;
	int m_nWebPaymentIndex1;
	int m_nWebPaymentIndex2;
	int m_nLocateOnIndex;
	bool m_bTestMode;
	int m_nFSMGroupNo;
	int m_nFSMRevertGroupNo;
	bool m_bDisableStaffImport;
	int m_nStaffGroupNo;
	int m_nMISIDUserTextNo;
	int m_nAdNoUserTextNo;
	int m_nUPNUserTextNo;
	int m_nYearUserTextNo;
	int m_nRegUserTextNo;
	int m_nHouseUserTextNo;
	bool m_bSavePupilEmail;
	int m_nEmailUserTextNo;
	bool m_bSaveStaffEmail;
	int m_nEmail2UserTextNo;
	CString m_strPhotoFolder;
	bool m_bDeletePhotoFile;
	int m_nNewPupilIntakeIndex;
	int m_nNewStaffIntakeIndex;
	CString m_strPupilStartFrom;
	CString m_strStaffStartFrom;

private:
	const char* GetMISIDTextLabel();
	const char* GetAdminNoTextLabel();
	const char* GetUPNTextLabel();
	const char* GetTextLabel ( int nIndex );

private:
	CString m_strFilename;
};
