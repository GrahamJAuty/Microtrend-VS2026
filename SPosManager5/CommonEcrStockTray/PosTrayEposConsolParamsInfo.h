#pragma once
/**********************************************************************/

struct CPosTrayEposConsolParamsInfo
{
public:
	CPosTrayEposConsolParamsInfo();

public:
	void Reset();

public:
	bool m_bOverride;
	int m_nConsolPrefsType;
	bool m_bCustomPrefs;
	CString m_strCustomPrefs;
};

/**********************************************************************/
