#pragma once
/**********************************************************************/

struct SimpleLabel
{
	const char* Label;
};

/**********************************************************************/

struct TableName
{
	const char* Label;
	int FileIdx;
};

/**********************************************************************/

struct ColumnName
{
	const char* szLabel;
	const int Type;
};

/**********************************************************************/

namespace TableNames
{
	const TableName AccountList =		{ "ACCOUNTLIST", 1 };
};

/**********************************************************************/
#define COLUMN_HEIGHT 1
#define COLUMN_ACCOUNTNO 2
#define COLUMN_USERNAME 3
#define COLUMN_GROUP 4
#define COLUMN_SPENDTD 5
#define COLUMN_POINTSTD 6
#define COLUMN_PURSE1 7
#define COLUMN_PURSE2 8
#define COLUMN_POINTS 9
#define COLUMN_BALANCE 10
#define COLUMN_INFO1 11
#define COLUMN_INFO2 12
#define COLUMN_INFO3 13
#define COLUMN_INFO4 14
#define COLUMN_ADDRESS1 15
#define COLUMN_ADDRESS5 16
#define COLUMN_STATUS 17
/**********************************************************************/

namespace ColumnNames
{
	const ColumnName Height = { "HEIGHT", COLUMN_HEIGHT };
	const ColumnName AccountNo = { "ACCOUNTNO",	COLUMN_ACCOUNTNO };
	const ColumnName UserName = { "USERNAME", COLUMN_USERNAME };
	const ColumnName Group = { "GROUP",	COLUMN_GROUP };
	const ColumnName SpendTD = { "SPENDTD",	COLUMN_SPENDTD };
	const ColumnName PointsTD = { "POINTSTD", COLUMN_POINTSTD };
	const ColumnName Purse1 = { "PURSE1", COLUMN_PURSE1 };
	const ColumnName Purse2 = { "PURSE2", COLUMN_PURSE2 };
	const ColumnName Points = { "POINTS", COLUMN_POINTS };
	const ColumnName Balance = { "BALANCE", COLUMN_BALANCE };
	const ColumnName Info1 = { "INFO1",	COLUMN_INFO1 };
	const ColumnName Info2 = { "INFO2",	COLUMN_INFO2 };
	const ColumnName Info3 = { "INFO3",	COLUMN_INFO3 };
	const ColumnName Info4 = { "INFO4",	COLUMN_INFO4 };
	const ColumnName Address1 = { "ADDRESS1", COLUMN_ADDRESS1 };
	const ColumnName Address5 = { "ADDRESS5", COLUMN_ADDRESS5 };
	const ColumnName Status = { "STATUS", COLUMN_STATUS };
}

/**********************************************************************/
