#pragma once
/**********************************************************************/
#ifndef BACKGROUND_SCHEDULER
/**********************************************************************/
#define BACKUPFILTER_RULE_ALL 0
/**********************************************************************/
#define BACKUPFILTER_RULE_FILEIS 1
#define BACKUPFILTER_RULE_FILECONTAINS 2
/**********************************************************************/
#define BACKUPFILTER_RULE_FOLDERCONTAINS 101
/**********************************************************************/

class CBackupFilterRule
{
public:
	CBackupFilterRule();

public:
	bool m_bInclude;
	int m_nRuleType;
	CString m_strRuleString;
};

/**********************************************************************/

class CBackupFilter
{
public:
	CBackupFilter(void);
	
public:
	void LoadRules();
	bool CheckFile( CString& strPath, CString& strFile );
	bool GotRules(){ return m_arrayRules.GetSize() != 0; }

private:
	void AddStandardRules1();

private:
	CArray<CBackupFilterRule,CBackupFilterRule> m_arrayRules;
	bool m_bGotStandardRules1;
};

/**********************************************************************/
#endif
/**********************************************************************/