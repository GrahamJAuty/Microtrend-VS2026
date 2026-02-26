#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM 
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CSPOSOfferGroupPluEditEcrman
{
public:
	CSPOSOfferGroupPluEditEcrman( __int64 nPluNo, CWnd* pParent );
	~CSPOSOfferGroupPluEditEcrman();

public:
	bool LoadDatabase();
	bool EditPlu();
	void LoadReadOnlyDatabase();
	void SaveChanges();
	void Finalise();

private:
	bool EditPluInternal(int nPluIdx);
	
private:
	CPluFilterArray m_FilterArray;

private:
	__int64 m_nPluNo;

private:
	CWnd* m_pParent;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
