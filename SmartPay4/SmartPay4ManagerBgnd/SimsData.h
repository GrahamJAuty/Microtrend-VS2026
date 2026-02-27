#pragma once

#define nSIMS_LOCATEBY_ADNO_INDEX		0		// modify exiting account by SIMS Adno
#define nSIMS_LOCATEBY_UPN_INDEX		1		// by SIMS UPN
#define nSIMS_LOCATEBY_PID_INDEX		2		// by SIMS Person_id

#define nSIMS_LOCATEPHOTOBY_ADNO_INDEX	0		// photo file uses adno
#define nSIMS_LOCATEPHOTOBY_UPN_INDEX	1		// photo file uses Person_id
#define nSIMS_LOCATEPHOTOBY_PID_INDEX	2		// photo file uses Person_id

#define nSIMS_NEWINTAKE_ADNO_INDEX		0		// create account by using SIMS Adno
#define nSIMS_NEWINTAKE_PID_INDEX		1		// create account by using SIMS Person_id
#define nSIMS_NEWINTAKE_NEXTFREE_INDEX	2		// next free record

class CSimsData
{
public:
	CSimsData();
	bool Read();
	bool Save();

	bool SIMSRequired();
	bool UPNRequired();
	bool PIDRequired();

	const char* GetLocateByText ( int nIndex );
	const char* GetLocateByLabel();

	const char* GetLocatePhotoByLabel();
	const char* GetLocatePhotoByText ( int nIndex = -1 );

	const char* GetNewIntakeAddedByText ( int nIndex = -1 );

public:
	bool m_bDeleteImportFile;
	bool  m_bDeletePhotoFile;
	int m_nImportMethod;
	CString m_strImportFilename;
	CString m_strSimsPhotoFolder;
	int m_nFSMGroupNo;
	int m_nFSMRevertGroupNo;
	int m_nSIMSUserTextNo;
	int m_nUPNUserTextNo;
	int m_nPIDUserTextNo;
	bool m_bPrsServerPhotoList;
	bool m_bPendingUpdateName;
	bool m_bPendingUpdateGroup;
	bool m_bPendingUpdateDob;
	int m_nYearContactNo;
	int m_nRegContactNo;
	bool m_bSaveEmail;				// now have specific email field
	int m_nEmailContactNo;			// 0=email field, else info 1-8
	int m_nDobNo;
	int m_nNewIntakeAccount;
	int m_nLocateBy;
	bool m_bAddUnknowns;
	int m_nLocatePhotoBy;
	CString m_strStartFromUserID;

private:
	const char* GetTextLabel ( int nInfo );
	const char* GetSIMSTextLabel();
	const char* GetUPNTextLabel();
	const char* GetPIDTextLabel();

private:
	CString m_strFilename;
};
