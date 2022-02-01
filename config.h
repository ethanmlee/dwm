/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#define TERM "urxvt"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 15;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "CozetteVector:size=9::antialias=false" };
static const char dmenufont[]       = "CozetteVector:size=9:antialias=false";
static const char col_black[]       = "#040404";
static const char col_gray3[]       = "#a1a1a1";
static const char col_gray5[]       = "#595959";
static const char col_orange[]      = "#f59542";
static const char *colors[][3]      = {
	/*               fg           bg          border   */
	[SchemeNorm] = { col_gray3,  col_black,  col_gray5 },
	[SchemeSel]  = { col_black,  col_orange, col_orange},
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"urxvt", "-tn", "rxvt-unicode", "-name", "spterm", NULL };
const char *spcmd2[] = {"keepassxc", NULL };
const char *spcmd3[] = {"bitwarden", NULL };
const char *spcmd4[] = {"plexamp", NULL };
const char *spcmd5[] = {"urxvt", "-tn", "rxvt-unicode", "-name", "calc", "-e", "calc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"keepassxc",   spcmd2},
	{"bitwarden",   spcmd3},
	{"plexamp",     spcmd4},
	{"calc",        spcmd5},
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
	{ NULL,    "spterm",    NULL,           SPTAG(0),  1,           -1 },
	{ NULL,    "keepassxc", NULL,           SPTAG(1),  1,           -1 },
	{ NULL,    "bitwarden", NULL,           SPTAG(2),  1,           -1 },
	{ NULL,    "plexamp",   NULL,           SPTAG(3),  1,           -1 },
	{ NULL,    "calc",      NULL,           SPTAG(4),  1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_gray3, "-sb", col_orange, "-sf", col_black, NULL };
static const char *termcmd[]    = { TERM, NULL };
static const char *bigtermcmd[] = { TERM, "-fn", "xft:CozetteVector:size=18:antialias=true", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_backslash,spawn,        {.v = bigtermcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_s,      togglescratch,  {.ui = 0 } }, /* spterm */
	{ MODKEY,                       XK_r,      togglescratch,  {.ui = 1 } }, /* keepassxc */
	{ MODKEY|ShiftMask,             XK_r,      togglescratch,  {.ui = 2 } }, /* bitwarden */
	{ MODKEY,                       XK_p,      togglescratch,  {.ui = 3 } }, /* plexamp */
	{ MODKEY,                       XK_a,      togglescratch,  {.ui = 4 } }, /* calc */
        { MODKEY,                       XK_n,      spawn,          SHCMD("kill -44 $(pidof nmtui) & urxvt -tn rxvt-unicode -name nmtui -e nmtui") },
        { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("arandr") },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
        { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("$PWBROWSER") },
        { MODKEY,                       XK_e,      spawn,          SHCMD("thunderbird") },
        { MODKEY,                       XK_v,      spawn,          SHCMD("vscode") },
        { MODKEY,                       XK_f,      spawn,          SHCMD("spacefm") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("cd ~/pix/scrot && scrot") },
	{ MODKEY|ShiftMask,             XK_Print,  spawn,          SHCMD("cd ~/pix/scrot && scrot -s") },
	{ MODKEY|ShiftMask,             XK_Delete, spawn,          SHCMD("kill -44 $(cat /tmp/dwmbarpid.tmp)") },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer -u; pamixer --allow-boost -i 5;refreshbar.sh") },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer -u; pamixer --allow-boost -d 5;refreshbar.sh") },
	{ 0,                            XF86XK_AudioMute,        spawn, SHCMD("pamixer -t;refreshbar.sh") },
	{ 0,                            XF86XK_AudioMicMute,     spawn, SHCMD("pamixer --source alsa_input.pci-0000_00_1b.0.analog-stereo -t") },
	{ 0,                            XF86XK_Sleep,            spawn, SHCMD("lock.sh & systemctl suspend") },
	{ 0,                            XF86XK_ScreenSaver,      spawn, SHCMD("lock.sh") },
	{ 0,                            XF86XK_Display,          spawn, SHCMD("desktop_mode.sh") },
        { ShiftMask,                    XF86XK_Display,          spawn, SHCMD("autorandr --change") },
	{ 0,                            XF86XK_Launch1,          spawn, SHCMD("bluetooth toggle") },

	{ MODKEY|ShiftMask,             XK_Delete, quit,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
