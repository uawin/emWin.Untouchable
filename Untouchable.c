/*
***************************************************
	created by Volodymyr Vyshenskyy

	This module contains all widgets that
	can be untouchable and touchable back
***************************************************
*/

#include "Untouchable.h"

#ifndef IS_EMWIN_5_50

#define	UNTOUCH_OFF					WM_USER + 0x7AB
#define	UNTOUCH_ON					UNTOUCH_OFF + 1

typedef int USER_DATA_FUNC(WM_HWIN hObj, void * pDest, int NumBytes);

static void _cb_CheckPID(WM_MESSAGE* pMsg, USER_DATA_FUNC* getData, USER_DATA_FUNC* setData) {
	UntouchInfo_t info;
	int bytes = getData(pMsg->hWin, &info, sizeof(info));

	switch (pMsg->MsgId) {

	case UNTOUCH_OFF:
	case UNTOUCH_ON:
		info.isUntouchable = (pMsg->MsgId == UNTOUCH_ON);
		bytes = setData(pMsg->hWin, &info, sizeof(UntouchInfo_t));
		break;

	case WM_TOUCH:
	case WM_TOUCH_CHILD:
	case WM_PID_STATE_CHANGED:

		if (info.isUntouchable) {
			WM_SendMessage(WM_GetParent(pMsg->hWin), pMsg);
			return;
		}

	default:
		info.cb(pMsg);
		break;
	}
}

//			UNTOUCHABLE calbacks		//

static void cb_FRAMEWIN_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, FRAMEWIN_GetUserData, FRAMEWIN_SetUserData); }

static void cb_WINDOW_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, WINDOW_GetUserData, WINDOW_SetUserData); }

static void cb_BUTTON_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, BUTTON_GetUserData, BUTTON_SetUserData); }

static void cb_DROPDOWN_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, DROPDOWN_GetUserData, DROPDOWN_SetUserData); }

static void cb_EDIT_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, EDIT_GetUserData, EDIT_SetUserData); }

static void cb_LISTVIEW_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, LISTVIEW_GetUserData, LISTVIEW_SetUserData); }

static void cb_RADIO_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, RADIO_GetUserData, RADIO_SetUserData); }

static void cb_MULTIPAGE_untouchable(WM_MESSAGE* pMsg) { _cb_CheckPID(pMsg, MULTIPAGE_GetUserData, MULTIPAGE_SetUserData); }

static WM_CALLBACK* prev_cb_MULTIPAGE_ClientWindow_untouchable;
static void cb_MULTIPAGE_ClientlWindow_untouchable(WM_MESSAGE* pMsg) {
	switch (pMsg->MsgId) {

	case WM_TOUCH:
	case WM_TOUCH_CHILD:
	case WM_PID_STATE_CHANGED:
		WM_SendMessage(WM_GetParent(pMsg->hWin), pMsg);
		return;

	default:
		prev_cb_MULTIPAGE_ClientWindow_untouchable(pMsg);
		break;
	}
}

static void change_MULTIPAGE_ClientWindow_callback(WM_HWIN hMultipage) {
	WM_HWIN clientWin = WM_GetClientWindow(hMultipage);
	prev_cb_MULTIPAGE_ClientWindow_untouchable = WM_GetCallback(clientWin);
	WM_SetCallback(clientWin, cb_MULTIPAGE_ClientlWindow_untouchable);
}





static void _sendUntouchableMessage(WM_HWIN hWin, void* pData) {
	U8 OnOff = *(U8 *)pData;
	WM_SendMessage(hWin, &(WM_MESSAGE){.MsgId = (OnOff ? UNTOUCH_ON : UNTOUCH_OFF) });
}

static void _setDataAndCallback(WM_HWIN hWin, USER_DATA_FUNC* setData, WM_CALLBACK* cbUntouchable) {
	int bytes = setData(hWin, &(UntouchInfo_t){.cb = WM_GetCallback(hWin), }, sizeof(UntouchInfo_t));
	if (bytes == sizeof(UntouchInfo_t)) {
		WM_SetCallback(hWin, cbUntouchable);
	}
}

static WM_HWIN _createUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb,
	GUI_WIDGET_CREATE_FUNC* createFn, USER_DATA_FUNC* setData, WM_CALLBACK* cbUntouchable) {

	WM_HWIN hWin = createFn(pCreateInfo, hWinParent, x0, y0, cb);
	if (pCreateInfo->NumExtraBytes >= sizeof(UntouchInfo_t)) {
		_setDataAndCallback(hWin, setData, cbUntouchable);
	}
	return hWin;
}


#endif // IS_EMWIN_5_50

/************************************/
/************************************/
/*									*/
/*			PUBLIC SECTION			*/
/*									*/
/************************************/
/************************************/

int setUntouchable(WM_HWIN hWin, U8 OnOff) {
	if (WM_IsWindow(hWin)) {
	#ifdef IS_EMWIN_5_50
			WM_SetUntouchable(hWin, OnOff);
	#else		
			WM_ForEachDesc(hWin, _sendUntouchableMessage, (void*)& OnOff);
			_sendUntouchableMessage(hWin, (void*)& OnOff);
	#endif
	}
}

/************************************/
/*			DIRECT CREATORS			*/
/************************************/

WM_HWIN FRAMEWIN_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char* pTitle, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return FRAMEWIN_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, pTitle, cb);
#else
	WM_HWIN hWin = FRAMEWIN_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, pTitle, cb, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, FRAMEWIN_SetUserData, cb_FRAMEWIN_untouchable);
	return hWin; 
#endif
}

WM_HWIN WINDOW_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return WINDOW_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, cb);
#else
	WM_HWIN hWin = WINDOW_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, cb, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, WINDOW_SetUserData, cb_WINDOW_untouchable);
	return hWin;
#endif
}

WM_HWIN BUTTON_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) {
#ifdef IS_EMWIN_5_50
	return BUTTON_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id);
#else
	WM_HWIN hWin = BUTTON_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, BUTTON_SetUserData, cb_BUTTON_untouchable);
	return hWin;
#endif
}

WM_HWIN DROPDOWN_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) {
#ifdef IS_EMWIN_5_50
	return DROPDOWN_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id);
#else
	WM_HWIN hWin = DROPDOWN_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, DROPDOWN_SetUserData, cb_DROPDOWN_untouchable);
	return hWin;
#endif
}

WM_HWIN EDIT_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int MaxLen) {
#ifdef IS_EMWIN_5_50
	return EDIT_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, MaxLen);
#else
	WM_HWIN hWin = EDIT_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, MaxLen, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, EDIT_SetUserData, cb_EDIT_untouchable);
	return hWin;
#endif
}

WM_HWIN LISTVIEW_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) {
#ifdef IS_EMWIN_5_50
	return LISTVIEW_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id);
#else
	WM_HWIN hWin = LISTVIEW_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, LISTVIEW_SetUserData, cb_LISTVIEW_untouchable);
	return hWin;
#endif
}

WM_HWIN RADIO_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumItems, int Spacing) {
#ifdef IS_EMWIN_5_50
	return RADIO_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, NumItems, Spacing);
#else
	WM_HWIN hWin = RADIO_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, NumItems, Spacing, sizeof(UntouchInfo_t));
	_setDataAndCallback(hWin, RADIO_SetUserData, cb_RADIO_untouchable);
	return hWin;
#endif
}

WM_HWIN MULTIPAGE_CreateUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) {
#ifdef IS_EMWIN_5_50
	return MULTIPAGE_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id);
#else
	WM_HWIN hWin = MULTIPAGE_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, sizeof(UntouchInfo_t));
	change_MULTIPAGE_ClientWindow_callback(hWin);
	_setDataAndCallback(hWin, MULTIPAGE_SetUserData, cb_MULTIPAGE_untouchable);
	return hWin;
#endif
}



// with user data

WM_HWIN BUTTON_CreateUserUntouchable(int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes) {
#ifdef IS_EMWIN_5_50
	return BUTTON_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, NumExtraBytes);
#else
	WM_HWIN hWin = BUTTON_CreateUser(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, sizeof(UntouchInfo_t) + NumExtraBytes);
	_setDataAndCallback(hWin, BUTTON_SetUserData, cb_BUTTON_untouchable);
	return hWin;
#endif
}


/************************************/
/*			INDIRECT CREATORS		*/
/************************************/

WM_HWIN FRAMEWIN_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return FRAMEWIN_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, FRAMEWIN_CreateIndirect, FRAMEWIN_SetUserData, cb_FRAMEWIN_untouchable);
#endif
}

WM_HWIN WINDOW_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return WINDOW_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, WINDOW_CreateIndirect, WINDOW_SetUserData, cb_WINDOW_untouchable);
#endif
}

WM_HWIN BUTTON_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return BUTTON_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, BUTTON_CreateIndirect, BUTTON_SetUserData, cb_BUTTON_untouchable);
#endif
}

WM_HWIN DROPDOWN_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return DROPDOWN_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, DROPDOWN_CreateIndirect, DROPDOWN_SetUserData, cb_DROPDOWN_untouchable);
#endif
}

WM_HWIN EDIT_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return EDIT_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, EDIT_CreateIndirect, EDIT_SetUserData, cb_EDIT_untouchable);
#endif
}

WM_HWIN LISTVIEW_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return LISTVIEW_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, LISTVIEW_CreateIndirect, LISTVIEW_SetUserData, cb_LISTVIEW_untouchable);
#endif
}


WM_HWIN RADIO_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return RADIO_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	return _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, RADIO_CreateIndirect, RADIO_SetUserData, cb_RADIO_untouchable);
#endif
}


WM_HWIN MULTIPAGE_CreateUntouchableIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
#ifdef IS_EMWIN_5_50
	return MULTIPAGE_CreateIndirect(pCreateInfo, hWinParent, x0, y0, cb);
#else
	WM_HWIN hWin = _createUntouchableIndirect(pCreateInfo, hWinParent, x0, y0, cb, MULTIPAGE_CreateIndirect, MULTIPAGE_SetUserData, cb_MULTIPAGE_untouchable);
	change_MULTIPAGE_ClientWindow_callback(hWin);
	return hWin;
#endif
}
