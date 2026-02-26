#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CDataAccessHelpers
{
public:
	CDataAccessHelpers();

public:
	void SetParentWnd( CWnd* pParentWnd ){ m_pParentWnd = pParentWnd; }
	CWnd* GetParentWnd(){ return m_pParentWnd; }
	int GetDbIdx( int nDbNo );

protected:
	CWnd* m_pParentWnd;
};

/**********************************************************************/
#endif
/**********************************************************************/
