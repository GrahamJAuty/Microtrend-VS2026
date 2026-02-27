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
#define COLUMN_USERID 2
#define COLUMN_USERNAME 3
#define COLUMN_FORENAME 4
#define COLUMN_GROUP 5
#define COLUMN_PURSE1 6
#define COLUMN_PURSE2 7
#define COLUMN_PURSE3 8
#define COLUMN_POINTS 9
#define COLUMN_BALANCE 10
#define COLUMN_SURNAME 11
#define COLUMN_STATUS 12
/**********************************************************************/

namespace ColumnNames
{
	const ColumnName Height = { "HEIGHT", COLUMN_HEIGHT };
	const ColumnName UserID = { "USERID", COLUMN_USERID };
	const ColumnName UserName = { "USERNAME", COLUMN_USERNAME };
	const ColumnName Forename = { "FORENAME", COLUMN_FORENAME };
	const ColumnName Group = { "GROUP", COLUMN_GROUP };
	const ColumnName Purse1 = { "PURSE1", COLUMN_PURSE1 };
	const ColumnName Purse2 = { "PURSE2", COLUMN_PURSE2 };
	const ColumnName Purse3 = { "PURSE3", COLUMN_PURSE3 };
	const ColumnName Points = { "POINTS", COLUMN_POINTS };
	const ColumnName Balance = { "BALANCE", COLUMN_BALANCE };
	const ColumnName Surname = { "SURNAME", COLUMN_SURNAME };
	const ColumnName Status = { "STATUS", COLUMN_STATUS };
}

/**********************************************************************/
