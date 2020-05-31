/*
				 ______        ____  __ 
 _ __ ___  _   _|  _ \ \      / /  \/  |
| '_ ` _ \| | | | | | \ \ /\ / /| |\/| |
| | | | | | |_| | |_| |\ V  V / | |  | |
|_| |_| |_|\__, |____/  \_/\_/  |_|  |_|
		   |___/                        
*/
/* See LICENSE file for copyright and license details. */
#include<X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 36;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     	/* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 22;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 22;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "JetBrains Mono Medium:size=18", "Noto Sans CJK SC:size=18" };
static const char dmenufont[]       = "JetBrains Mono Medium:size=24";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]      = "#42A5F5";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "0", "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Stack",     tile    },    /* first entry is default */
	{ "Float",     NULL    },    /* no layout function means floating behavior */
	{ "Full",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]     = { "st", NULL                       };
static const char *browsercmd[]  = { "chromium", NULL            };
static const char *musicmd[]     = { "netease-cloud-music", NULL             };
static const char *upvol[]       = {"amixer", "set", "Master", "2%+", NULL   };
static const char *downvol[]     = {"amixer", "set", "Master", "2%-", NULL   };
static const char *mutevol[]     = {"amixer", "set", "Master", "toggle", NULL};

static Key keys[] = {
	/* modifier                     key                      function        			   argument */
	{ MODKEY,                       XK_d,                    spawn,          			   {.v = dmenucmd } },
	{ MODKEY,			            XK_Return,               spawn,          			   {.v = termcmd } },
	{ MODKEY,              			XK_c,                    spawn,          			   {.v = browsercmd } },
	{ MODKEY|ShiftMask,				XK_m, 	                 spawn,		     			   {.v = musicmd } },
	{ MODKEY,                       XK_b,                    togglebar,      			   {0} },
	{ 0,                            XF86XK_AudioLowerVolume, spawn,          			   {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,        spawn,          			   {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn,          			   {.v = upvol   } },
	{ MODKEY,                       XK_s,                    spawn,                        {.v = downvol } },
	{ MODKEY,                       XK_n,                    spawn,                        {.v = mutevol } },
	{ MODKEY,                       XK_w,                    spawn,                        {.v = upvol   } },
	{ MODKEY|ShiftMask,             XK_h,                    rotatestack,                  {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,                    rotatestack,                  {.i = -1 } },
	{ MODKEY,                       XK_h,                    focusstack,                   {.i = +1 } },
	{ MODKEY,                       XK_l,                    focusstack,                   {.i = -1 } },
	{ MODKEY,                       XK_i,                    incnmaster,                   {.i = +1 } },
	{ MODKEY,                       XK_u,                    incnmaster,                   {.i = -1 } },
	{ MODKEY,                       XK_j,                    setmfact,                     {.f = -0.01} },
	{ MODKEY,                       XK_k,                    setmfact,                     {.f = +0.01} },
	{ MODKEY|Mod4Mask,              XK_h,                    incrgaps,                     {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_l,                    incrgaps,                     {.i = -1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_h,                    incrogaps,                    {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_l,                    incrogaps,                    {.i = -1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_h,                    incrigaps,                    {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_l,                    incrigaps,                    {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,                    togglegaps,                   {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,                    defaultgaps,                  {0} },
	{ MODKEY,                       XK_y,                    incrihgaps,                   {.i = +1 } },
	{ MODKEY,                       XK_o,                    incrihgaps,                   {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,                    incrivgaps,                   {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,                    incrivgaps,                   {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_y,                    incrohgaps,                   {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_o,                    incrohgaps,                   {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,                    incrovgaps,                   {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,                    incrovgaps,                   {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return,               zoom,                         {0} },
	{ MODKEY,                       XK_Tab,                  view,                         {0} },
	{ MODKEY,                       XK_q,                    killclient,                   {0} },
	{ MODKEY,                       XK_t,                    setlayout,                    {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                    setlayout,                    {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                    setlayout,                    {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                setlayout,                    {0} },
	{ MODKEY|ShiftMask,             XK_space,                togglefloating,               {0} },
	{ MODKEY,                       XK_0,                    view,                         {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                    tag,                          {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                focusmon,                     {.i = -1 } },
	{ MODKEY,                       XK_period,               focusmon,                     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                tagmon,                       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,               tagmon,                       {.i = +1 } },
	TAGKEYS(                        XK_1,                                                  0)
	TAGKEYS(                        XK_2,                                                  1)
	TAGKEYS(                        XK_3,                                                  2)
	TAGKEYS(                        XK_4,                                                  3)
	TAGKEYS(                        XK_5,                                                  4)
	TAGKEYS(                        XK_6,                                                  5)
	TAGKEYS(                        XK_7,                                                  6)
	TAGKEYS(                        XK_8,                                                  7)
	TAGKEYS(                        XK_9,                                                  8)
	{ MODKEY|ShiftMask,             XK_q,                    quit,                         {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
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

