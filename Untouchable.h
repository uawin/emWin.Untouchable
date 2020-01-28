#ifndef __UNTOUCHABLE__
#define __UNTOUCHABLE__

/*
***************************************************
	created by Volodymyr Vyshenskyy

	This module contains all widgets that
	can be untouchable and touchable back
***************************************************
*/

#include "DIALOG.h"

typedef struct {
	WM_CALLBACK * cb;
	U8 isUntouchable;
} UntouchInfo_t;

int setUntouchable(WM_HWIN hWin, U8 OnOff);		// This function makes a window and all its descendants untouchable or touchable back.


												/*			DIRECT CREATORS							*/

WM_HWIN FRAMEWIN_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pTitle, WM_CALLBACK * cb);
WM_HWIN WINDOW_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK * cb);
WM_HWIN BUTTON_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
WM_HWIN DROPDOWN_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
WM_HWIN EDIT_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int MaxLen);
WM_HWIN LISTVIEW_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
WM_HWIN RADIO_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumItems, int Spacing);
WM_HWIN MULTIPAGE_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);

												/*			DIRECT CREATORS	WITH USER DATA			*/
WM_HWIN BUTTON_CreateUserUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);



												/*			INDIRECT CREATORS						*/

// to create DIALOGs using GUI_CreateDialogBox and GUI_WIDGET_CREATE_INFO array 
// you should set NumExtraBytes as sizeof(UntouchInfo_t) for each array element

WM_HWIN FRAMEWIN_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN WINDOW_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN BUTTON_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN DROPDOWN_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN EDIT_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN LISTVIEW_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN RADIO_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
WM_HWIN MULTIPAGE_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

#endif