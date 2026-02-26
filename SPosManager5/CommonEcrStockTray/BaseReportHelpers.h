#pragma once
/**********************************************************************/
#define FILESELECT_NORMAL 0
#define FILESELECT_LATEST 1
#define FILESELECT_BYDATE 2
#define FILESELECT_LITERAL 3
/**********************************************************************/

struct CEposReportSourceFileInfo
{
	int m_nSelectArrayIdx;		//LOC AND ECR THAT RELATES TO THIS FILE
	CString m_strFilename;		//FULL FILENAME AND PATH
	CString m_strJobType;		//X1 ETC
	CString m_strExtractDateTime;	//EXTRACTION TIME OF FILE
	CString m_strReportDate;		//DATE FOR REPORTING PURPOSES
};

/**********************************************************************/

struct CEposReportConfig
{
public:
	CEposReportConfig(){ m_bAppend = FALSE; }

public:
	int m_nRepType;			//REP_PLU_SALES ETC
	int m_nLevel;				//1 OR 2
	int m_nTaskNo;
	int m_nDbIdx;
	int m_nDeptFilter;
	int m_nFileSelectMode;		//NORMAL, LATEST, BY DATE OR LITERAL
	int m_nConLevel;
	bool m_bAppend;
	COleDateTime m_dateFrom;
	COleDateTime m_dateTo;
	CString m_strConTitle;
	CString m_strReportTitle;	//CREATED BY REPORT HANDLER
	CString m_strSourceFolder;	//FOR LITERAL FILE MODE
	CString m_strSourceFilename;	//FOR LATEST AND LITERAL FILE MODES
};

/**********************************************************************/
