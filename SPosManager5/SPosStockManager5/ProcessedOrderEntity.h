#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

struct CProcessedOrderEntity
{
public:
	CProcessedOrderEntity();
	void Reset();

public:
	int Compare ( CProcessedOrderEntity& source, int nHint = 0 );
	void Add	( CProcessedOrderEntity& source );

public:
	const char* GetComboText();

public:
	int m_nOrderNo;
	CString m_strDate;
	CString m_strTime;
	int m_nSearchType;
	
public:
	CString m_strDetail;
	CString m_strComboText;
};

/**********************************************************************/
#endif
/**********************************************************************/
