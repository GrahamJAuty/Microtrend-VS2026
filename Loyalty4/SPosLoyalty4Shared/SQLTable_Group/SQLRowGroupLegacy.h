#pragma once
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************
#define LEGACY_REVALUE_PLUNO_COUNT 4
//**********************************************************************

class CSQLRowGroupLegacy
{
public:
	CSQLRowGroupLegacy();
	
public:
	void Reset();
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	
	/*****/
	
	bool GetRevalueFlag(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_bRevalueFlag[n];
		}
		else
		{
			return FALSE;
		}
	}

	bool GetRevalueDisable(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_bRevalueDisable[n];
		}
		else
		{
			return FALSE;
		}
	}

	int GetRevalueType(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_nRevalueType[n];
		}
		else
		{
			return FALSE;
		}
	}

	double GetRevalueTrip(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_dRevalueTrip[n];
		}
		else
		{
			return FALSE;
		}
	}
	
	int GetRevaluePoints(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_nRevaluePoints[n];
		}
		else
		{
			return FALSE;
		}
	}
	
	double GetRevalueBonus(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_dRevalueBonus[n];
		}
		else
		{
			return FALSE;
		}
	}
	
	int GetRevaluePercent(int n)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			return m_nRevaluePercent[n];
		}
		else
		{
			return FALSE;
		}
	}
	
public:
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	
	/*****/
	
	void SetRevalueFlag(int n, bool b)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			m_bRevalueFlag[n] = b;
		}
	}

	void SetRevalueDisable(int n, bool b)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			m_bRevalueDisable[n] = b;
		}
	}

	void SetRevalueType(int n, int nType)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			vdtr.SetRevalueType(nType, m_nRevalueType[n]);
		}
	}

	void SetRevalueTrip(int n, double d)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			vdtr.SetRevalueTrip(d, m_dRevalueTrip[n]);
		}
	}
	
	void SetRevaluePoints(int n, int nPoints)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			vdtr.SetRevaluePoints(nPoints, m_nRevaluePoints[n]);
		}
	}
	
	void SetRevalueBonus(int n, double d)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			vdtr.SetRevalueBonus(d, m_dRevalueBonus[n]);
		}
	}

	void SetRevaluePercent(int n, int nPercent)
	{
		if ((n >= 1) && (n <= LEGACY_REVALUE_PLUNO_COUNT))
		{
			vdtr.SetRevaluePercent(nPercent, m_nRevaluePercent[n]);
		}
	}

private:
	int m_nGroupNo;
	/*****/
	bool m_bRevalueFlag[LEGACY_REVALUE_PLUNO_COUNT + 1];
	bool m_bRevalueDisable[LEGACY_REVALUE_PLUNO_COUNT + 1];
	int m_nRevalueType[LEGACY_REVALUE_PLUNO_COUNT + 1 ];
	double m_dRevalueTrip[LEGACY_REVALUE_PLUNO_COUNT + 1 ];
	int m_nRevaluePoints[LEGACY_REVALUE_PLUNO_COUNT + 1 ];
	double m_dRevalueBonus[LEGACY_REVALUE_PLUNO_COUNT + 1];
	int m_nRevaluePercent[LEGACY_REVALUE_PLUNO_COUNT + 1];
	
private:
	CSQLFieldValidatorGroup vdtr;
};

//**********************************************************************

