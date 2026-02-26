#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM 
/**********************************************************************/
#include "MyCSSLockFile.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CSPOSOfferGroupPluEditStockman
{
public:
	CSPOSOfferGroupPluEditStockman( __int64 nPluNo, CWnd* pParent );
	~CSPOSOfferGroupPluEditStockman();

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
