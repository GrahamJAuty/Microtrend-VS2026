#pragma once
/*********************************************************************/
#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
/*********************************************************************/
 
/**********************************************************************/
#define MAX_CATEGORY 999
/*********************************************************************/

class CDefTextCategory : public CSharedStringArray 
{
public:
	int Open( const char* szFilename, int nMode );

public:
	const char* GetDefaultText ( int nCategory );
	const char* GetText ( int nCategory );
	bool GetReportEnable ( int nCategory );
	int GetTaxBand ( int nCategory );
	void SetCategory ( int nCategory, const char* szText, int nTaxBand, bool bEnable );

public:
	void CopyFrom( CDefTextCategory& Source );

private:
	CString m_strDefText;
	CString m_strText;
};

/**********************************************************************/
#endif		//__DEFTEXTCATEGORY_H__
/**********************************************************************/
