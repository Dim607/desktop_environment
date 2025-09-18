/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h> /* to read XF86 codes for keys*/

// Applied patches
// dwm-statuscmd-20210405-67d76bd.diff --- execeute commands when status bar is clicked (for dwmblocks)

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "monospace:size=10" };
//static const char *fonts[]          = { "JetBrains Mono:size=10" };
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "󰈹","",  "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      		instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     		NULL,       NULL,       0,            1,           -1 },
	{ "firefox",    	NULL,       NULL,       1 << 0,       0,           -1 },
	{ "kitty",  		  NULL,       NULL,       1 << 1,       0,           -1 },
	// { "spotify-launcher",  	NULL,       NULL,       1 << 2,       0,           -1 },
	// { "discord",  		NULL,       NULL,       1 << 3,       0,           -1 },

	
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
//#define MODKEY Mod1Mask
#define MODKEY Mod4Mask		// use super key as modekey instead of alt
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *termcmd[]  = { "st", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *inc_refreshrate[] = {"xrandr", "--output", "eDP-1", "--mode", "1920x1080", "--rate", "144.03"};
static const char *dec_refreshrate[] = {"xrandr", "--output", "eDP-1", "--mode", "1920x1080", "--rate", "60.01"};
/* command to cycle through battery modes */
static const char *chg_bat_mode[] = {"pkill", "-RTMIN+4", "dwmblocks"};
/* command to toggle between languages */
//static const char *change_keyboard[] = { "setxkbmap", "-layout", "us, gr", "-option", "grp:toggle", NULL };
//static const char *change_keyboard[] = { "setxkbmap", "-layout", "us,gr", "-option", "grp:toggle", NULL };


static const Key keys[] = {
	/* modifier                     key        			function        argument */
	{ MODKEY,                       XK_p,      			spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, 			spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      			togglebar,      {0} },
	{ MODKEY,                       XK_j,      			focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      			focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      			incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      			incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      			setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      			setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, 			zoom,           {0} },
	{ MODKEY,                       XK_Tab,    			view,           {0} },
	{ MODKEY,                       XK_c,      			killclient,     {0} },
	{ MODKEY,                       XK_t,      			setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      			setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      			setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  			setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  			togglefloating, {0} },
	{ MODKEY,                       XK_0,      			view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      			tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  			focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 			focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  			tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 			tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      			0)
	TAGKEYS(                        XK_2,                      			1)
	TAGKEYS(                        XK_3,                      			2)
  TAGKEYS(                        XK_4,                      			3)
	TAGKEYS(                        XK_5,                      			4)
	TAGKEYS(                        XK_6,                      			5)
	TAGKEYS(                        XK_7,                      			6)
	TAGKEYS(                        XK_8,                      			7)
	TAGKEYS(                        XK_9,                      			8)
	{ MODKEY,                       XK_q,      			quit,           {0} },

  // Creating a command array when sending mulitple shell commands does not work
  // beacuse of how spawn works (calls execvp which does not understand ;)
  // call SHCMD() insted
  /* 						volume keys					*/
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 2%+; pkill -RTMIN+6 dwmblocks") },
	{ 0,				XF86XK_AudioLowerVolume,	spawn,		 SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 2%-; pkill -RTMIN+6 dwmblocks")},
	{ 0,				XF86XK_AudioMute,		spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; pkill -RTMIN+6 dwmblocks") },
	/* 						brightness keys					*/
	{ 0,				XF86XK_MonBrightnessUp,		spawn,		SHCMD("brightnessctl set +2% ; pkill -RTMIN+5 dwmblocks") },
	{ 0,				XF86XK_MonBrightnessDown,	spawn,		SHCMD("brightnessctl set 2%- ; pkill -RTMIN+5 dwmblocks") },
	
	/* 						refresh rate keys				*/
	{ Mod1Mask,			XF86XK_MonBrightnessUp,		spawn,		{.v = inc_refreshrate} },
	{ Mod1Mask,			XF86XK_MonBrightnessDown,	spawn,		{.v = dec_refreshrate} },
  /* cycle through battery modes */
  {MODKEY|ShiftMask,        XK_b,     spawn,                {.v = chg_bat_mode }}

	/*						to change language				*/
//	{ Mod4Mask,			XK_space,			spawn,		{.v = change_keyboard} },


};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

