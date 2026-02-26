/**********************************************************************/
 
/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayEposConsolParamsInfo.h"
/**********************************************************************/

CPosTrayEposConsolParamsInfo::CPosTrayEposConsolParamsInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayEposConsolParamsInfo::Reset()
{
	m_bOverride = FALSE;
	m_nConsolPrefsType = POSTRAY_EPOS_CONSOL_DEFAULT;
	m_bCustomPrefs = FALSE;
	m_strCustomPrefs = "";
}

/**********************************************************************/
