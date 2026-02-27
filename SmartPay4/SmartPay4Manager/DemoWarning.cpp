/**********************************************************************/
#include "..\SmartPay4Shared\PresentationOptions.h"
/**********************************************************************/
#include "DemoWarning.h"
/**********************************************************************/

CDemoWarning::CDemoWarning()
{
	int nSerialNo = Sysset.GetSerialNo();
	int nExternal = Sysset.GetExternalLink();

	if ( ( nSerialNo >= 9001 ) && ( nSerialNo <= 9099 ) && ( ( nExternal & 1024 ) == 1024 ) )
	{
		CPresentationOptions RememberOptions;
		RememberOptions.CopyFrom(PresentationOptions);

		PresentationOptions.SetCustomMessageBoxFlag(TRUE);
		PresentationOptions.SetCustomMessageBoxFont(12);
		PresentationOptions.SetCustomMessageBoxLineGap(10);
		PresentationOptions.SetCustomMessageBoxColourBackInt( 0xFFFF00 );
		PresentationOptions.SetCustomMessageBoxColourTextInt( 0x0 );

		CString strInfo = ""; 
		
		strInfo += "This is a dealer demo of Microtrend ";
		strInfo += AfxGetAppName();
		strInfo += ".\n\n";

		strInfo += "This software is NOT licensed for commercial use.\n\n";
		
		if ( Sysset.IsNobbledDemo() == TRUE )
		{
			strInfo += "Please note that some transactions may be intentionally\n";
			strInfo += "deleted or changed to discourage unauthorised use.";
		}

		Prompter.Info(strInfo);

		PresentationOptions.CopyFrom(RememberOptions);
	}
}

/**********************************************************************/
