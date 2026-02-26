/**********************************************************************/
#include "BackupFilter.h"
/**********************************************************************/

CBackupFilterRule::CBackupFilterRule()
{
	m_bInclude = TRUE;
	m_nRuleType = BACKUPFILTER_RULE_ALL;
	m_strRuleString = "";
}

/**********************************************************************/

CBackupFilter::CBackupFilter(void)
{
	m_bGotStandardRules1 = FALSE;
}

/**********************************************************************/

void CBackupFilter::LoadRules()
{
	m_arrayRules.RemoveAll();
	m_bGotStandardRules1 = FALSE;

	COleDateTime dateNow = COleDateTime::GetCurrentTime();

	CString strFileName;
	strFileName.Format( "BF%4.4d%2.2d%2.2d.dat",
		dateNow.GetYear(),
		dateNow.GetMonth(),
		dateNow.GetDay() );

	GetDataProgramPath( strFileName );

	CSSFile fileRules;
	if ( fileRules.Open( strFileName, "rb" ) == TRUE )
	{
		CString strBuffer;
		while( fileRules.ReadString( strBuffer ) == TRUE )
		{
			if (strBuffer == "#1")
			{
				AddStandardRules1();
			}
			else
			{
				CCSV csv( strBuffer );
				
				if ( csv.GetSize() <= 3 )
				{
					CBackupFilterRule Rule;
					Rule.m_bInclude = csv.GetBool(0);
					Rule.m_nRuleType = csv.GetInt(1);
					Rule.m_strRuleString = csv.GetString(2);
					m_arrayRules.Add( Rule );
				}
			}
		}

		if (m_arrayRules.GetSize() == 0)
		{
			AddStandardRules1();
		}
	}
}

/**********************************************************************/

void CBackupFilter::AddStandardRules1()
{
	if ( FALSE == m_bGotStandardRules1 )
	{
		CBackupFilterRule Rule;
		Rule.m_bInclude = FALSE;
		Rule.m_nRuleType = BACKUPFILTER_RULE_FILEIS;

		//EXAMPLE RULE
		//Rule.m_strRuleString = "FILENAMETOEXCLUE";
		//m_arrayRules.Add(Rule);

		m_bGotStandardRules1 = TRUE;
	}
}

/**********************************************************************/

bool CBackupFilter::CheckFile( CString& strPath, CString& strFile )
{
	for ( int n = 0; n < m_arrayRules.GetSize(); n++ )
	{
		bool bMatch = FALSE;

		CString strRuleString = m_arrayRules[n].m_strRuleString;
		strRuleString.MakeUpper();

		switch( m_arrayRules[n].m_nRuleType )
		{
		case BACKUPFILTER_RULE_ALL:
			bMatch = TRUE;
			break;

		case BACKUPFILTER_RULE_FILEIS:
			bMatch = ( strFile == strRuleString );
			break;

		case BACKUPFILTER_RULE_FILECONTAINS:
			bMatch = ( strFile.Find( strRuleString ) != -1 );
			break;

		case BACKUPFILTER_RULE_FOLDERCONTAINS:
			bMatch = ( strPath.Find( strRuleString ) != -1 );
			break;
		}

		if ( TRUE == bMatch )
			return ( TRUE == m_arrayRules[n].m_bInclude );
	}

	return TRUE;
}

/**********************************************************************/
