/**********************************************************************/
#pragma once
/**********************************************************************/

#define DB_SORT_ASCENDING	0	// ascending				sql: ASC
#define DB_SORT_DESCENDING	1	// descending				sql: DESC

#define DB_SORT_TEXT		0	// text (case sensitive)		sql: text
#define DB_SORT_TEXT_NOCASE	1	// text (case insensitive)	sql: texti
#define DB_SORT_INT			2	// int64					sql: int
#define DB_SORT_FLOAT		3	// double					sql:	float
#define DB_SORT_DATE		4	// DD*MM*YYyy				sql: date
#define DB_SORT_TIME		5	// HH*MM*ss*mmm			sql: time
#define DB_SORT_IPV4		6	// n.n.n.n				sql: ipv4

/**********************************************************************/
