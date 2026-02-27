#pragma once
/**********************************************************************/
#include "..\SQLTranBase.h"
/**********************************************************************/

class CSQLTranMigrateSchema : public CSQLTranBase
{
public:
	void DoWork(int nTargetVersion);

private:
	void MigrateV2();
	void MigrateV3();
	void MigrateV4();
	void MigrateV5();
	void MigrateV6();
	void MigrateV7();
	void MigrateV8();
	void MigrateV9();
	void MigrateV10();
	void MigrateV11();
	void MigrateV12();
	void MigrateV13();
	void MigrateV14();
	void MigrateV15();
	void MigrateV16();
	void MigrateV17();
	void MigrateV18();
	void MigrateV19();
	
private:
	CString GetPrimaryKeyName(CString strTableName);
	CString GetDropConstraintCommand(CString strTableName, CString strConstraint);
	static void AddAlterTableAddColumnCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, bool bNotNull, CString strDefault = "");
	bool ColumnExists(CString strTableName, CString strColumnName);

private:
	CStringArray m_arrayCommands;
};

/**********************************************************************/
