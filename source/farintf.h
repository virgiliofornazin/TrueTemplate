#ifndef __FARINTF_H
#define __FARINTF_H
#define _FAR_USE_FARFINDDATA
#include <plugin.hpp>

struct WindowInfoEx : WindowInfo
{
	WindowInfoEx() : WindowInfo()
	{
		ZeroMemory(this, sizeof(WindowInfoEx));
		StructSize = sizeof(WindowInfoEx);
	}
};

struct MacroSendMacroTextEx : MacroSendMacroText
{
	MacroSendMacroTextEx() : MacroSendMacroText()
	{
		ZeroMemory(this, sizeof(MacroSendMacroTextEx));
		StructSize = sizeof(MacroSendMacroTextEx);
	}
};

struct FarMenuItemEx : FarMenuItem
{
	FarMenuItemEx() : FarMenuItem()
	{
		ZeroMemory(this, sizeof(FarMenuItemEx));
	}
};

struct FarDialogItemDataEx : FarDialogItemData
{
	FarDialogItemDataEx() : FarDialogItemData()
	{
		ZeroMemory(this, sizeof(FarDialogItemDataEx));
		StructSize = sizeof(FarDialogItemDataEx);
	}
};

struct ViewerInfoEx : ViewerInfo
{
	ViewerInfoEx() : ViewerInfo()
	{
		ZeroMemory(this, sizeof(ViewerInfoEx));
		StructSize = sizeof(ViewerInfoEx);
	}
};

struct EditorInfoEx : EditorInfo
{
	EditorInfoEx() : EditorInfo()
	{
		ZeroMemory(this, sizeof(EditorInfoEx));
		StructSize = sizeof(EditorInfoEx);
	}
};

struct EditorGetStringEx : EditorGetString
{
	EditorGetStringEx() : EditorGetString()
	{
		ZeroMemory(this, sizeof(EditorGetStringEx));
		StructSize = sizeof(EditorGetStringEx);
	}
};

struct EditorSetPositionEx : EditorSetPosition
{
	EditorSetPositionEx() : EditorSetPosition()
	{
		ZeroMemory(this, sizeof(EditorSetPositionEx));
		StructSize = sizeof(EditorSetPositionEx);
	}
};

struct EditorSetParameterEx : EditorSetParameter
{
	EditorSetParameterEx() : EditorSetParameter()
	{
		ZeroMemory(this, sizeof(EditorSetParameterEx));
		StructSize = sizeof(EditorSetParameterEx);
	}
};

struct EditorSetStringEx : EditorSetString
{
	EditorSetStringEx() : EditorSetString()
	{
		ZeroMemory(this, sizeof(EditorSetStringEx));
		StructSize = sizeof(EditorSetStringEx);
	}
};

struct EditorSelectEx : EditorSelect
{
	EditorSelectEx() : EditorSelect()
	{
		ZeroMemory(this, sizeof(EditorSelectEx));
		StructSize = sizeof(EditorSelectEx);
	}
};

extern struct PluginStartupInfo Info;
extern struct FarStandardFunctions FSF;
extern wchar_t PluginRootKey[80];

struct InitDialogItem
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned int Selected;
  unsigned int Flags;
  wchar_t *Data;
};

struct InitDialogItemEx
{
 int Type;
 int X1,Y1,X2,Y2;
 int Focus;
 int Selected;
 DWORD Flags;
 int DefaultButton;
 const wchar_t *Data;
 const wchar_t *History;
};

inline const wchar_t *GetMsg(int MsgId)
{
  return Info.GetMsg(&MainGuid, MsgId);
}

extern void InitDialogItems(struct InitDialogItem*, struct FarDialogItem*, int);
extern void InitDialogItemsEx(const struct InitDialogItemEx*, struct FarDialogItem*,int);

#endif