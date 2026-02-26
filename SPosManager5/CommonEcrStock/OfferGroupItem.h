#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct COfferGroupItem
{
public:
	COfferGroupItem();
	void Reset();
	
public:
	int Compare ( COfferGroupItem& source, int nHint = 0 );
	void Add	( COfferGroupItem& source );
	
public:
	int m_nItemNo;
	CString m_strPluNo;
	int m_nModifiers;
};

/**********************************************************************/
#endif
/**********************************************************************/
