struct TNavy
{
	TNavy();
	wchar_t	mask[MAX_REG_LEN], path[MAX_STR_LEN], suffixes[MAX_STR_LEN];
	int		pos, rect[4];
	bool	viewer;
};

TNavy::TNavy ()
{
	wcscpy (this->mask, L"");
	wcscpy (this->path, L"");
	wcscpy (this->suffixes, L"");
	this->pos = 0;
	this->rect[0] = -1;
	this->rect[1] = -1;
	this->rect[2] = -1;
	this->rect[3] = -1;
	this->viewer = false;
}

static void NavyInsert (const wchar_t *file, const wchar_t *suffixes, const int rect[4], bool viewer, TCollection &coll)
{
	struct nav
	{
		wchar_t	*file;
		int		rect[4];
		bool	viewer;
	};

	const wchar_t	*cursuffixes = suffixes;
	wchar_t				tmp[2] = L"x";
	wchar_t				filename[2 * _MAX_PATH + 1];
	wchar_t				filenamex[2 * _MAX_PATH + 1];
	wcscpy (filename, file);
	GetFullPathName (filename, 2 * _MAX_PATH + 1, filenamex, NULL);
	wcscpy (filename, filenamex);
	do
	{
		if (IsFile (filename))
		{
			for (unsigned i = 0; i < coll.getCount (); i++)
				if (_wcsicmp (((nav *) (coll[i]))->file, filename) == 0) return ;

			nav *tmp = new nav;
			tmp->rect[0] = rect[0];
			tmp->rect[1] = rect[1];
			tmp->rect[2] = rect[2];
			tmp->rect[3] = rect[3];
			tmp->viewer = viewer;
			tmp->file = new wchar_t[wcslen (filename) + 1];
			wcscpy (tmp->file, filename);
			coll.insert (tmp);
		}

		wcscpy (filename, (*cursuffixes) ? file : L"");
		while (*cursuffixes)
		{
			if ((*cursuffixes == L';') || (*cursuffixes == L'\0'))
			{
				cursuffixes++;
				break;
			}
			else
			{
				tmp[0] = *cursuffixes;
				wcscat (filename, tmp);
				GetFullPathName (filename, 2 * _MAX_PATH + 1, filenamex, NULL);
				wcscpy (filename, filenamex);
				cursuffixes++;
			}
		}
	} while (*filename);
}

static void NavyFind (const wchar_t	*path, const wchar_t *file, const wchar_t *suffixes, const int rect[4], bool viewer, TCollection &coll)
{
	wchar_t	testFile[2 * NM];
	wcscpy(testFile, path);
	wcscat(testFile, file);
	NavyInsert (testFile, suffixes, rect, viewer, coll);

	int n = Info.AdvControl (&MainGuid, ACTL_GETWINDOWCOUNT, NULL, NULL);
	for (int i = 0; i < n; i++)
	{
		wchar_t	witypename[NM];
		wchar_t	winame[NM];
		WindowInfoEx wi;
		wi.TypeName = witypename;
		wi.TypeNameSize = NM;
		wi.Name = winame;
		wi.NameSize = NM;
		wi.Pos = i;
		Info.AdvControl (&MainGuid, ACTL_GETWINDOWINFO, 0, (void *) &wi);
		if (wi.Type == WTYPE_EDITOR)
		{
			wchar_t	*p = (wchar_t *) wcsrchr (wcscpy (testFile, wi.Name), L'\\');
			if (p)
			{
				wcscpy (p + 1, file);
				NavyInsert (testFile, suffixes, rect, viewer, coll);
			}
		}
	}
}

static void NavyCollect
(
	const wchar_t	*path,
	const wchar_t	*filename,
	const wchar_t	*includepath,
	const wchar_t	*suffixes,
	const int		rect[4],
	bool			viewer,
	TCollection &coll
)
{
	wchar_t				curdir[2 * _MAX_PATH + 1];
	wchar_t				curfile[2 * _MAX_PATH + 1];
	wchar_t				*tmpDir = curdir;
	const wchar_t	*tmpPath = includepath;

	NavyInsert (filename, suffixes, rect, viewer, coll);
	while (*tmpPath)
	{
		if (*tmpPath == L';')
		{
			*tmpDir = L'\0';
			*tmpPath++;
			tmpDir = curdir;
			if ((curdir[0] == L'.') && (curdir[1] == L'\0'))
				NavyFind (path, filename, suffixes, rect, viewer, coll);
			else
			{
				wcscpy (curfile, curdir);
				wcscat (curfile, L"\\");
				wcscat (curfile, filename);
				NavyInsert (curfile, suffixes, rect, viewer, coll);
			}
		}
		else
			*tmpDir++ = *tmpPath++;
	}

	*tmpDir = L'\0';
	tmpDir = curdir;
	if ((curdir[0] == L'.') && (curdir[1] == L'\0'))
		NavyFind (path, filename, suffixes, rect, viewer, coll);
	else
	{
		wcscpy (curfile, curdir);
		wcscat (curfile, L"\\");
		wcscat (curfile, filename);
		NavyInsert (curfile, suffixes, rect, viewer, coll);
	}
}

static void Navigate (TLang *lng, const wchar_t *path, wchar_t *realLine, DWORD pos, DWORD start, DWORD end)
{
	wchar_t				filename[2 * _MAX_PATH + 1];
	TCollection found;
	for (unsigned i = 0; i < lng->navyColl.getCount (); i++)
	{
		TNavy *nav = (TNavy *) (lng->navyColl[i]);
		int		bounds[1][2] = { { 0, 0 } };
		if (start != -1)
		{
			int len = min (end - start, _MAX_PATH);
			wcsncpy (filename, realLine + start, len);
            filename[len] = 0;
			NavyCollect (path, filename, nav->path, nav->suffixes, nav->rect, nav->viewer, found);
		}
		else if (strMatch (realLine, nav->mask, L"/^\\s*", (lng->ignoreCase ? L"\\s*$/i" : L"\\s*$/"), 1, bounds, &nav->pos))
		{
			if (((int)pos >= bounds[0][0]) && ((int)pos <= bounds[0][1]))
			{
				int len = min (bounds[0][1] - bounds[0][0], _MAX_PATH);
				wcsncpy (filename, realLine + bounds[0][0], len);
                filename[len] = 0;
				NavyCollect (path, filename, nav->path, nav->suffixes, nav->rect, nav->viewer, found);
			}
		}
	}

	struct nav
	{
		wchar_t	*file;
		int		rect[4];
		bool	viewer;
	};

	FarMenuItemEx *mMenu = NULL;
	if (found.getCount () > 1) mMenu = new FarMenuItemEx[found.getCount ()];
	for (unsigned i = 0; ((i < found.getCount ()) && (found.getCount () > 1)); i++)
	{
		mMenu[i].Text = ((nav *) (found[i]))->file;
	}

	FarKey BreakKeys[2];
	BreakKeys[0].VirtualKeyCode = VK_F3;
	BreakKeys[0].ControlKeyState = 0;
	BreakKeys[1].VirtualKeyCode = 0;
	BreakKeys[1].ControlKeyState = 0;

	intptr_t BreakCode;
	int ExitCode = 0;
	while (found.getCount () > 1)
	{
		ExitCode = Info.Menu
			(
				&MainGuid,
				&NavigateGuid,
				-1,
				-1,
				0,
				FMENU_WRAPMODE,
				GetMsg (MNavyMenu),
				GetMsg (MNavyMenuBottom),
				NULL,
				BreakKeys,
				&BreakCode,
				(const FarMenuItemEx *) mMenu,
				found.getCount ()
			);
		if (ExitCode >= 0)
		{
			if (BreakCode == 0)
			{
				const wchar_t	*Msg[5];
				Msg[0] = GetMsg (MNavigation);
				Msg[1] = L"\1";
				Msg[2] = ((nav *) (found[ExitCode]))->file;
				Msg[3] = L"\1";
				Msg[4] = GetMsg (MOK);
				Info.Message (&MainGuid, &NavigateMessageGuid, FMSG_LEFTALIGN, NULL, Msg, sizeof (Msg) / sizeof (Msg[0]), 1);
				continue;
			}
			else
				break;
		}
		else
			break;
	}

	if (found.getCount () > 1) delete[] mMenu;
	if ((found.getCount ()) && (ExitCode >= 0))
	{
		CONSOLE_SCREEN_BUFFER_INFO	ConBuffInfo;
		HANDLE	ConHnd = GetStdHandle (STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo (ConHnd, &ConBuffInfo);

		wchar_t	navRectLeft[32];
		wchar_t	navRectRight[32];
		wchar_t	navRectTop[32];
		wchar_t	navRectBottom[32];

		PluginSettings settings(MainGuid, ::Info.SettingsControl);
		settings.Get(0,L"NavigationRectLeft", navRectLeft, 32, L"0");
		settings.Get(0,L"NavigationRectTop", navRectTop, 32, L"50");
		settings.Get(0,L"NavigationRectRight", navRectRight, 32, L"100");
		settings.Get(0,L"NavigationRectBottom", navRectBottom, 32, L"100");

		WORD	x1, y1, x2, y2;
		x1 = ((((nav *) (found[ExitCode]))->rect[0] >= 0) && (((nav *) (found[ExitCode]))->rect[0] <= 100)) ? ((nav *) (found[ExitCode]))->rect[0] : FSF.atoi (navRectLeft);
		y1 = ((((nav *) (found[ExitCode]))->rect[1] >= 0) && (((nav *) (found[ExitCode]))->rect[1] <= 100)) ? ((nav *) (found[ExitCode]))->rect[1] : FSF.atoi (navRectTop);
		x2 = ((((nav *) (found[ExitCode]))->rect[2] >= 0) && (((nav *) (found[ExitCode]))->rect[2] <= 100)) ? ((nav *) (found[ExitCode]))->rect[2] : FSF.atoi (navRectRight);
		y2 = ((((nav *) (found[ExitCode]))->rect[3] >= 0) && (((nav *) (found[ExitCode]))->rect[3] <= 100)) ? ((nav *) (found[ExitCode]))->rect[3] : FSF.atoi (navRectBottom);
		x1 = ConBuffInfo.dwSize.X * x1 / 100;
		y1 = ConBuffInfo.dwSize.Y * y1 / 100;
		x2 = ConBuffInfo.dwSize.X * x2 / 100;
		y2 = ConBuffInfo.dwSize.Y * y2 / 100;
		if (((nav *) (found[ExitCode]))->viewer)
		{
			Info.Viewer
				(
					((nav *) (found[ExitCode]))->file,
					NULL,
					x1,
					y1,
					x2,
					y2,
					VF_NONMODAL | VF_IMMEDIATERETURN | VF_DISABLEHISTORY | VF_ENABLE_F6,
					CP_DEFAULT
				);
		}
		else
		{
			Info.Editor
				(
					((nav *) (found[ExitCode]))->file,
					NULL,
					x1,
					y1,
					x2,
					y2,
					EF_NONMODAL | EF_IMMEDIATERETURN | EF_DISABLEHISTORY,
					0,
					1,
					CP_DEFAULT
				);
		}
	}

	for (unsigned i = 0; i < found.getCount (); i++) delete[]((nav *) (found[i]))->file;
	found.removeAll ();
}

static void SelectNavigation (TEInfo *te, const wchar_t* path)
{
	if (te)
	{
		TLang *lng = (TLang *) (langColl[te->lang]);
		if (lng)
		{
			wchar_t				line[MAX_STR_LEN];
			EditorInfoEx	ei;
			Info.EditorControl (-1, ECTL_GETINFO, 0, &ei);

			EditorGetStringEx gs;
			TEditorPos			epos = EditorGetPos ();
			EditorGetStr (&gs);
			wcscpy(line, gs.StringText);
			Navigate (lng, path, line, epos.Col, gs.SelStart, gs.SelEnd);
		}
	}
}

static void SelectNavigationList (TEInfo *te, const wchar_t* path)
{
	if (te)
	{
		TLang *lng = (TLang *) (langColl[te->lang]);
		if (lng)
		{
			wchar_t				line[MAX_STR_LEN];
			wchar_t				filename[2 * _MAX_PATH + 1];
			TCollection found;
			EditorInfoEx	ei;
			Info.EditorControl (-1, ECTL_GETINFO, 0, &ei);
			for (int l = 0; l < ei.TotalLines; l++)
			{
				EditorGetStringEx egs;
				EditorGetStr (&egs, l);
				wcsncpy (line, egs.StringText, egs.StringLength + 1);
				for (unsigned i = 0; i < lng->navyColl.getCount (); i++)
				{
					TNavy *nav = (TNavy *) (lng->navyColl[i]);
					int		bounds[1][2] = { { 0, 0 } };
					if (strMatch (line, nav->mask, L"/^\\s*", (lng->ignoreCase ? L"\\s*$/i" : L"\\s*$/"), 1, bounds, &nav->pos))
					{
						int len = min (bounds[0][1] - bounds[0][0], _MAX_PATH);
						wcsncpy (filename, line + bounds[0][0], len);
						filename[len] = 0;
						NavyCollect (path, filename, nav->path, nav->suffixes, nav->rect, nav->viewer, found);
					}
				}
			}

			struct nav
			{
				wchar_t	*file;
				int		rect[4];
				bool	viewer;
			};

			FarMenuItemEx *mMenu = NULL;
			if (found.getCount () > 0) mMenu = new FarMenuItemEx[found.getCount ()];
			for (unsigned i = 0; ((i < found.getCount ()) && (found.getCount () > 1)); i++)
			{
				mMenu[i].Text = ((nav *) (found[i]))->file;
			}

			FarKey BreakKeys[2];
			BreakKeys[0].VirtualKeyCode = VK_F3;
			BreakKeys[0].ControlKeyState = 0;
			BreakKeys[1].VirtualKeyCode = 0;
			BreakKeys[1].ControlKeyState = 0;	

			intptr_t BreakCode;
			int ExitCode = 0;
			while (found.getCount () > 0)
			{
				ExitCode = Info.Menu
					(
						&MainGuid,
						&SelNavigateGuid,
						-1,
						-1,
						0,
						FMENU_WRAPMODE,
						GetMsg (MNavyMenu),
						GetMsg (MNavyMenuBottom),
						NULL,
						BreakKeys,
						&BreakCode,
						(const FarMenuItemEx *) mMenu,
						found.getCount ()
					);
				if (ExitCode >= 0)
				{
					if (BreakCode == 0)
					{
						const wchar_t	*Msg[5];
						Msg[0] = GetMsg (MNavigation);
						Msg[1] = L"\1";
						Msg[2] = ((nav *) (found[ExitCode]))->file;
						Msg[3] = L"\1";
						Msg[4] = GetMsg (MOK);
						Info.Message (&MainGuid, &SelNavigateMessageGuid, FMSG_LEFTALIGN, NULL, Msg, sizeof (Msg) / sizeof (Msg[0]), 1);
						continue;
					}
					else
						break;
				}
				else
					break;
			}

			if (found.getCount () > 0) delete[] mMenu;
			if ((found.getCount ()) && (ExitCode >= 0))
			{
				CONSOLE_SCREEN_BUFFER_INFO	ConBuffInfo;
				HANDLE	ConHnd = GetStdHandle (STD_OUTPUT_HANDLE);
				GetConsoleScreenBufferInfo (ConHnd, &ConBuffInfo);

				wchar_t	navRectLeft[32];
				wchar_t	navRectRight[32];
				wchar_t	navRectTop[32];
				wchar_t	navRectBottom[32];

				PluginSettings settings(MainGuid, ::Info.SettingsControl);
				settings.Get(0,L"NavigationRectLeft", navRectLeft, 32, L"0");
				settings.Get(0,L"NavigationRectTop", navRectTop, 32, L"50");
				settings.Get(0,L"NavigationRectRight", navRectRight, 32, L"100");
				settings.Get(0,L"NavigationRectBottom", navRectBottom, 32, L"100");

				WORD	x1, y1, x2, y2;
				x1 = ((((nav *) (found[ExitCode]))->rect[0] >= 0) && (((nav *) (found[ExitCode]))->rect[0] <= 100)) ? ((nav *) (found[ExitCode]))->rect[0] : FSF.atoi (navRectLeft);
				y1 = ((((nav *) (found[ExitCode]))->rect[1] >= 0) && (((nav *) (found[ExitCode]))->rect[1] <= 100)) ? ((nav *) (found[ExitCode]))->rect[1] : FSF.atoi (navRectTop);
				x2 = ((((nav *) (found[ExitCode]))->rect[2] >= 0) && (((nav *) (found[ExitCode]))->rect[2] <= 100)) ? ((nav *) (found[ExitCode]))->rect[2] : FSF.atoi (navRectRight);
				y2 = ((((nav *) (found[ExitCode]))->rect[3] >= 0) && (((nav *) (found[ExitCode]))->rect[3] <= 100)) ? ((nav *) (found[ExitCode]))->rect[3] : FSF.atoi (navRectBottom);
				x1 = ConBuffInfo.dwSize.X * x1 / 100;
				y1 = ConBuffInfo.dwSize.Y * y1 / 100;
				x2 = ConBuffInfo.dwSize.X * x2 / 100;
				y2 = ConBuffInfo.dwSize.Y * y2 / 100;
				if (((nav *) (found[ExitCode]))->viewer)
				{
					Info.Viewer
						(
							((nav *) (found[ExitCode]))->file,
							NULL,
							x1,
							y1,
							x2,
							y2,
							VF_NONMODAL | VF_IMMEDIATERETURN | VF_DISABLEHISTORY | VF_ENABLE_F6,
							CP_DEFAULT
						);
				}
				else
				{
					Info.Editor
						(
							((nav *) (found[ExitCode]))->file,
							NULL,
							x1,
							y1,
							x2,
							y2,
							EF_NONMODAL | EF_IMMEDIATERETURN | EF_DISABLEHISTORY,
							0,
							1,
							CP_DEFAULT
						);
				}
			}

			for (unsigned i = 0; i < found.getCount (); i++) delete[]((nav *) (found[i]))->file;
			found.removeAll ();
		}
	}
}