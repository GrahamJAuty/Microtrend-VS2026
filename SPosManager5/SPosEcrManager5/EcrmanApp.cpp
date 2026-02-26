/**********************************************************************/
#include "..\CommonEcrStock\CommonDoc.h"
#include "DemoWarning.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EcrmanApp.h"
/**********************************************************************/

BEGIN_MESSAGE_MAP(CEcrmanApp, CWinApp)
	//{{AFX_MSG_MAP(CEcrmanApp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/**********************************************************************/

CEcrmanApp theApp;

/**********************************************************************/

