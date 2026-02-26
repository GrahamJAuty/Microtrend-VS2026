#pragma once
/**********************************************************************/

//key types for list and screen
#define KEY_TYPE_EMPTY 0
#define KEY_TYPE_FUNCTION 1
#define KEY_TYPE_PAYMENT 2
#define KEY_TYPE_MODIFIER 3
#define KEY_TYPE_CONTROL 6
#define KEY_TYPE_LIST 12
#define KEY_TYPE_SVRPRESET 13
#define KEY_TYPE_MACRO 20
#define KEY_TYPE_PLUNUMBER 21

//list formats (internal use)
//NOTE - 1x5 means 1 row of 5 columns
#define LIST_FORMAT_1x5 105		//L
#define LIST_FORMAT_1x10 110	//F3
#define LIST_FORMAT_1x14 114	//F3
#define LIST_FORMAT_2x1 201		//L2
#define LIST_FORMAT_2x2 202		//L2
#define LIST_FORMAT_2x3 203		//L, L2
#define LIST_FORMAT_2x4 204		//L
#define LIST_FORMAT_3x1 301		//L2
#define LIST_FORMAT_3x2 302		//L2
#define LIST_FORMAT_3x3 303		//L, L2
#define LIST_FORMAT_3x4 304		//L
#define LIST_FORMAT_4x1 401		//L2
#define LIST_FORMAT_4x3 403		//L2
#define LIST_FORMAT_4x4 404		//L, L2
#define LIST_FORMAT_4x5 405		//L, L2
#define LIST_FORMAT_4x6 406		//L
#define LIST_FORMAT_5x1 501		//L2
#define LIST_FORMAT_5x2 502		//L2
#define LIST_FORMAT_5x3 503		//F
#define LIST_FORMAT_5x4 504		//L, L2
#define LIST_FORMAT_5x5 505		//L2
#define LIST_FORMAT_5x6 506		//F2, L
#define LIST_FORMAT_5x7 507		//L
#define LIST_FORMAT_6x1 601		//L2
#define LIST_FORMAT_6x5 605		//L, L2
#define LIST_FORMAT_6x6 606		//L2
#define LIST_FORMAT_7x1 701		//L2
#define LIST_FORMAT_7x7 707		//L2
#define LIST_FORMAT_8x5 805		//L2
#define LIST_FORMAT_9x6 906		//L2
#define LIST_FORMAT_10x5 1005	//L2
#define LIST_FORMAT_12x5 1205	//L2
#define LIST_FORMAT_12x10 1210	//L, L2
#define LIST_FORMAT_15x12 1512	//L2

//borrowed defines to patch up code
//during development
#define KEY_WIDTH 240	
#define KEY_HEIGHT 240
#define KEY_XSPACE_DISPLAY 2410	
#define KEY_YSPACE_DISPLAY 1600
#define KEY_XSPACE_PRINTER 250
#define KEY_YSPACE_PRINTER 250
#define KEY_XORIGIN 100
#define KEY_YORIGIN 100

#define KEY_MINSIZE 100
#define KEY_MAXSIZE 300
#define KEY_MINSPACE 5
#define KEY_MAXSPACE 100
#define KEY_MINORIGIN 0
#define KEY_MAXORIGIN 999

#define POS_KEY_COUNT 180	
#define SYMBOLID_ECRTEXT 0
#define SYMBOLID_REPTEXT 1

#define ECR_PLUNO_LEN 8

//import files from stockman
#define PAYMENT_COUNT 16

#define COLOURDROPPER_CLOSED 0
#define COLOURDROPPER_LISTKEY 1
#define COLOURDROPPER_COPYKEY 2
#define COLOURDROPPER_COPYSTYLE 3
#define COLOURDROPPER_GROUPKEY 4

#define MAX_BITMAP_FILENAME 19	//NOT INCLUDING EXTENSION

#define VIEWMODE_LIST_ITEM_STANDARD 0
#define VIEWMODE_LIST_ITEM_LARGE 1
#define VIEWMODE_LIST_FUNCTION_STANDARD 2
#define VIEWMODE_LIST_FUNCTION_LARGE 3
#define VIEWMODE_LIST_FUNCTION_BOTTOM 4
#define VIEWMODE_SCREEN 5

/**********************************************************************/
