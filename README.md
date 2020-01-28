# emWin.Untouchable

The version 5.50 of emWin (GUI-library by Segger for embedded applications) has a new function WM_SetUntouchable. 
This function enables you to make window "untouchable" for PID-messages. This is very helpful if you create windows like screen-keypads to avoid context switch to bottom placed windows.

But this function is absent in previous versions of emWin.

And it was a reason to write this small library, allows to create "untouchable" windows/widgets for emWin versions before 5.50.

It can be used also if you have only precompiled emWin-library instead of its source code.

You can easily extend this lib with absent widgets (some of standard emWin-widgets are missed, because I do not use them in my project).

For every "create"-function like FRAMEWIN_CreateEx or WINDOW_CreateEx, BUTTON_CreateEx, EDIT_CreateEx etc. there is correspondent FRAMEWIN_CreateUntouchable or WINDOW_CreateUntouchable, BUTTON_CreateUntouchable, EDIT_CreateUntouchable etc.

And if you used them, you can use the only one call of function setUntouchable(WM_HWIN hWin, U8 OnOff) to enable/disable "untouchable"-behaviour for some parent-window including all its childs.

Also you can use correspondent indirect creators like WINDOW_CreateUntouchableIndirect or DROPDOWN_CreateUntouchableIndirect etc. instead of WINDOW_CreateIndirect or DROPDOWN_CreateIndirect etc.
