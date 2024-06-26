/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static unsigned int borderpx  = 4;        /* border pixel of windows */
static unsigned int snap      = 4;       /* snap pixel */
static unsigned int gappih    = 4;       /* horiz inner gap between windows */
static unsigned int gappiv    = 4;       /* vert inner gap between windows */
static unsigned int gappoh    = 4;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 4;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static int vertpad            = 4;       /* vertical padding of bar */
static int sidepad            = 4;       /* horizontal padding of bar */
static int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static char *altbarclass      = "Polybar"; /* Alternate bar class name */
static char *altbarcmd        = "$HOME/.scripts/polybar.sh"; /* Alternate bar launch command */
static char font[]            = "Hack Nerd Font Mono:style=Regular:size=10";
static char dmenufont[]       = "Hack Nerd Font Mono:style=Regular:size=10";
static const char *fonts[]    = { font };
static char normbgcol[]             = "#222222";
static char normbordercol[]         = "#444444";
static char normfgcol[]             = "#bbbbbb";
static char selfgcol[]              = "#eeeeee";
static char selbordercol[]          = "#005577";
static char selbgcol[]              = "#005577";

static char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcol, normbgcol, normbordercol },
	[SchemeSel]  = { selfgcol,  selbgcol,  selbordercol  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */  
	/* class    instance    title                 tags mask  isfloating   monitor */
	{ NULL,    "nmtui",     NULL,                 0,         1,           -1 },
	{ NULL,    "spterm",    NULL,                 0,         1,           -1 },
	{ NULL,    "calc",      NULL,                 0,         1,           -1 },
	{ NULL,    "keepassxc", NULL,                 0,         1,           -1 },
	{ NULL,    "bitwarden", NULL,                 0,         1,           -1 },
	{ NULL,    "plexamp",   NULL,                 0,         1,           -1 },
	{ NULL,    NULL,        "Picture-in-Picture", 0,         1,           -1 }
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ NULL,       NULL },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcol, "-nf", normfgcol, "-sb", selbordercol, "-sf", selfgcol, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
       { "vertpad",            INTEGER, &vertpad },
       { "sidepad",            INTEGER, &sidepad },

       { "usealtbar",          INTEGER, &usealtbar },

       { "borderpx",           INTEGER, &borderpx },
       { "snap",               INTEGER, &snap },
       { "gappih",             INTEGER, &gappih },
       { "gappiv",             INTEGER, &gappiv },
       { "gappoh",             INTEGER, &gappoh },
       { "gappov",             INTEGER, &gappov },
       { "smartgaps",          INTEGER, &smartgaps },
       { "showbar",            INTEGER, &showbar },
       { "topbar",             INTEGER, &topbar },
       { "font",               STRING,  &font },
       { "dmenufont",          STRING,  &dmenufont },

       { "normbordercol",      STRING,  &normbordercol },
       { "selbordercol",       STRING,  &selbordercol },
       { "selfgcol",           STRING,  &selfgcol },
       { "selbgcol",           STRING,  &selbgcol },
       { "normfgcol",          STRING,  &normfgcol },
       { "normbgcol",          STRING,  &normbgcol },

       { "mfact",              FLOAT,   &mfact },
       { "nmaster",            INTEGER, &nmaster },
       { "resizehints",        INTEGER, &resizehints },
       { "enablegaps",         INTEGER, &enablegaps },
};

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	{ MODKEY,                       XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,         incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,         incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,         setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_f,         togglecanfocusfloating, {0} },
	{ MODKEY|ShiftMask,             XK_j,         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,         movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return,    zoom,           {0} },
	{ MODKEY,                       XK_minus,     incrgaps,       {.i = -4 } },
	{ MODKEY,                       XK_equal,     incrgaps,       {.i = +4 } },
	{ MODKEY|ShiftMask,             XK_minus,     togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_equal,     defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,       view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,         killclient,     {0} },
	{ MODKEY,                       XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_y,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,         setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_u,         setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,     setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,     togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_p,         togglesticky,   {0} },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Delete,    spawn,          SHCMD("xresources.sh") },
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
	{ MODKEY,                       XK_0,         setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_semicolon, winview,        {0} },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
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
static const Button buttons[] = {
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  togglebar,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

