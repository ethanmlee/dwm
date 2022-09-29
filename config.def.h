/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font Mono:style=Regular:size=10" };
static const char dmenufont[]       = "Hack Nerd Font Mono:style=Regular:size=10";
static const char col_black[]       = "#000000";
static const char col_white[]       = "#ffffff";
static const char col_gray[]        = "#696969";
static const char col_magenta[]     = "#f746fa";
static const char col_green[]       = "#55fa55";
static const char col_cyan[]        = "#46e8fa";
static const char *colors[][4]      = {
	/*                   fg           bg           border       float   */
	[SchemeNorm]     = { col_white,   col_black,   col_gray,    col_gray  },
	[SchemeSel]      = { col_black,   col_magenta, col_magenta, col_cyan  },
	[SchemeStatus]   = { col_gray,    col_black,   "#000000",   "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_black,   col_magenta, "#000000",   "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm] = { col_gray,    col_black,   "#000000",   "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_white,   col_black,   "#000000",   "#000000" }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm] = { col_white,   col_black,   "#000000",   "#000000" }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class   instance     title           tags mask  isfloating   monitor */
	{ "Gimp",  NULL,        NULL,           0,         1,           -1 },
	{ "feh",   NULL,        NULL,           0,         1,           -1 },
	{ NULL,    "urxvt",     NULL,           0,         0,           -1 },
	{ NULL,    "nmtui",     NULL,           0,         1,           -1 },
	{ NULL,    NULL,        "Event Tester", 0,         0,           -1 }, /* xev */
	{ NULL,    "spterm",    NULL,           0,         1,           -1 },
	{ NULL,    "calc",      NULL,           0,         1,           -1 },
	{ NULL,    "keepassxc", NULL,           0,         1,           -1 },
	{ NULL,    "bitwarden", NULL,           0,         1,           -1 },
	{ NULL,    "plexamp",   NULL,           0,         1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 4;/* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_white, "-sb", col_magenta, "-sf", col_black, NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key           function        argument */
	{ MODKEY,                       XK_d,         spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	{ MODKEY,                       XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,         incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,         incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,         setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,         setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_p,         setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_p,         resetlayout,    {0} },
	{ MODKEY|ShiftMask,             XK_j,         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,         movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return,    zoom,           {0} },
	{ MODKEY,                       XK_Tab,       view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,         killclient,     {0} },
	{ MODKEY,                       XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_u,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,         setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,     setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,     togglefloating, {0} },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Delete,    quit,           {0} },
	{ MODKEY,                       XK_semicolon, winview,        {0} },
	{ MODKEY,                       XK_1,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_2,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_3,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_4,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_5,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_6,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_7,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_8,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_9,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_0,         spawn,          SHCMD("tdrop hide_all") },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_0,         setlayout,      {.v = &layouts[3]} },
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
