#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "AddressInfo.h"
/**********************************************************************/
 
/**********************************************************************/
#define SITE_VERSION 1
/**********************************************************************/
#define SITEIDX_SPECIAL_UNKNOWN 0x1000000
/**********************************************************************/

class CSiteCSVRecord : public CCSVRecord
{
public:
	CSiteCSVRecord(); 
	virtual ~CSiteCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
private:
	void SetSiteNo ( int nSiteNo ) { m_nSiteNo = nSiteNo; }
	void SetName ( const char* szName );
	void SetDbNo ( int nDbNo ) { m_nDbNo = nDbNo; }
	void SetLegacyAddress1 ( const char* szText ) { m_strLegacyAddress1 = szText; }
	void SetLegacyAddress2 ( const char* szText ) { m_strLegacyAddress2 = szText; }
	void SetLegacyAddress3 ( const char* szText ) { m_strLegacyAddress3 = szText; }
	void SetLegacyPostcode ( const char* szText ) { m_strLegacyPostcode = szText; }
	void SetLegacyPhone ( const char* szText ) { m_strLegacyPhone = szText; }
	void SetLegacyFax ( const char* szText ) { m_strLegacyFax = szText; }
	void SetLegacyEmail ( const char* szText ) { m_strLegacyEmail = szText; }

	const char* GetSystemName();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nSiteNo;
	int m_nDbNo;
	CString m_strName;
	CString m_strSystemName;
	CString m_strLegacyAddress1;
	CString m_strLegacyAddress2;
	CString m_strLegacyAddress3;
	CString m_strLegacyPostcode;
	CString m_strLegacyPhone;
	CString m_strLegacyFax;
	CString m_strLegacyEmail;

	friend class CSiteCSVArray;
};

/**********************************************************************/

class CSiteCSVArray : public CPtrArray
{
public:
	CSiteCSVArray();
	~CSiteCSVArray();

	bool RenameForDeletion( int nSiteIdx );
	void ClearSiteFolder( int nSiteIdx );
	void ClearDeletionFolder( int nSiteIdx );
	
	int AddSite ( int nDbNo );
	int AddSite ( int nDbNo, int nSiteNo, const char* szPath );
	bool WriteSite ( int nSiteIdx );

	bool FindSiteByNumber ( int nDbNo, int nSiteNo, int& nSiteIdx );
	void GetSiteIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx );

	void DeleteSiteByNumber ( int nDbNo, int nSiteNo );
	void DeleteSiteByIndex ( int nSiteIdx, bool bDeleteFiles );
	void PurgeDatabase( int nDbIdx );
	
	void RemoveAll();

	int GetSiteNo( int nSiteIdx );
	int GetDbNo ( int nSiteIdx );
	const char* GetName ( int nSiteIdx );
	const char* GetSystemName ( int nSiteIdx );
	const char* GetLegacyAddress1 ( int nSiteIdx );
	const char* GetLegacyAddress2 ( int nSiteIdx );
	const char* GetLegacyAddress3 ( int nSiteIdx );
	const char* GetLegacyPostcode ( int nSiteIdx );
	const char* GetLegacyPhone ( int nSiteIdx );
	const char* GetLegacyFax ( int nSiteIdx );
	const char* GetLegacyEmail ( int nSiteIdx );
	void SetSiteNo ( int nSiteIdx, int nSiteNo );
	void SetDbNo ( int nSiteIdx, int nDbNo );
	void SetName ( int nSiteIdx, const char* szName );
	void SetLegacyAddress1 ( int nSiteIdx, const char* szAddress1 );
	void SetLegacyAddress2 ( int nSiteIdx, const char* szAddress2 );
	void SetLegacyAddress3 ( int nSiteIdx, const char* szAddress3 );
	void SetLegacyPostcode ( int nSiteIdx, const char* szPostcode );
	void SetLegacyPhone ( int nSiteIdx, const char* szPhone );
	void SetLegacyFax ( int nSiteIdx, const char* szFax );
	void SetLegacyEmail ( int nSiteIdx, const char* szEmail );

	const char* GetFolderPath( int nSiteIdx );
	const char* GetFolderPathDelete( int nSiteIdx );
	const char* GetFilePathInfo( int nSiteIdx );
	
	const char* GetCSVLine( int nSiteIdx );

	void SortByName( CWordArray& arraySiteIdx, int nDbIdx = -1 );

	void GetAddressInfo( int nSiteIdx, CAddressInfo& info );
	void SetAddressInfo( int nSiteIdx, CAddressInfo& info );

public:
	void BuildSortIdxTables();
	int GetSortIdxFromSiteIdx( int nSiteIdx );
	int GetSiteIdxFromSortIdx( int nSortIdx );

private:
	CSiteCSVRecord* GetSite( int nSiteIdx ) { return ( CSiteCSVRecord*) GetAt ( nSiteIdx ); }
	
private:
	CString m_strName;
	CString m_strSystemName;
	CString m_strLegacyAddress1;
	CString m_strLegacyAddress2;
	CString m_strLegacyAddress3;
	CString m_strLegacyPostcode;
	CString m_strLegacyPhone;
	CString m_strLegacyFax;
	CString m_strLegacyEmail;
	CString m_strCSVLine;

	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	CString m_strFilePathInfo;

	CWordArray m_arraySortIdxToSiteIdx;
	CWordArray m_arraySiteIdxToSortIdx;
};

/**********************************************************************/
extern CSiteCSVArray dbSite;
/**********************************************************************/
#endif
/**********************************************************************/
