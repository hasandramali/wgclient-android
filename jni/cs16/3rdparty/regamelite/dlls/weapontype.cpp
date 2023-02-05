// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

WeaponStruct g_weaponStruct[ MAX_WEAPONS ] =
{
	{ 0, 0, 0, 0, 0 },

	{ WEAPON_P228,		P228_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_SECONDARY,	AMMO_357SIG_PRICE },
	{ WEAPON_SCOUT,		SCOUT_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_762MM_PRICE },
	{ WEAPON_XM1014,	XM1014_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_BUCKSHOT_PRICE },
	{ WEAPON_MAC10,		MAC10_PRICE,		CT,			AUTOBUYCLASS_PRIMARY,	AMMO_45ACP_PRICE },
	{ WEAPON_AUG,		AUG_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_ELITE,		ELITE_PRICE,		CT,			AUTOBUYCLASS_SECONDARY,	AMMO_9MM_PRICE },
	{ WEAPON_FIVESEVEN,	FIVESEVEN_PRICE,	TERRORIST|CT,		AUTOBUYCLASS_SECONDARY,	AMMO_57MM_PRICE },
	{ WEAPON_UMP45,		UMP45_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_45ACP_PRICE },
	{ WEAPON_SG550,		SG550_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_USP,		USP_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_SECONDARY,	AMMO_45ACP_PRICE },
	{ WEAPON_GLOCK18,	GLOCK18_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_SECONDARY,	AMMO_9MM_PRICE },
	{ WEAPON_MP5N,		MP5NAVY_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_9MM_PRICE },
	{ WEAPON_AWP,		AWP_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_338MAG_PRICE },
	{ WEAPON_M249,		M249_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_M3,		M3_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_BUCKSHOT_PRICE },
	{ WEAPON_M4A1,		M4A1_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_TMP,		TMP_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_9MM_PRICE },
	{ WEAPON_G3SG1,		G3SG1_PRICE,		CT,			AUTOBUYCLASS_PRIMARY,	AMMO_762MM_PRICE },
	{ WEAPON_DEAGLE,	DEAGLE_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_SECONDARY,	AMMO_50AE_PRICE },
	{ WEAPON_SG552,		SG552_PRICE,		CT,			AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_AK47,		AK47_PRICE,		CT,			AUTOBUYCLASS_PRIMARY,	AMMO_762MM_PRICE },
	{ WEAPON_P90,		P90_PRICE,		TERRORIST|CT,		AUTOBUYCLASS_PRIMARY,	AMMO_57MM_PRICE },
	{ WEAPON_FAMAS,		FAMAS_PRICE,		TERRORIST,		AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
	{ WEAPON_GALIL,		GALIL_PRICE,		CT,			AUTOBUYCLASS_PRIMARY,	AMMO_556MM_PRICE },
				// TODO: this have bug, the cost of galil $2000, but not $2250

	{ WEAPON_SHIELDGUN,	SHIELDGUN_PRICE,	TERRORIST,		AUTOBUYCLASS_PRIMARY,	0 },

	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }
};

AutoBuyInfoStruct g_autoBuyInfo[] =
{
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"galil",	"weapon_galil" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"ak47",		"weapon_ak47" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SNIPERRIFLE,	"scout",	"weapon_scout" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"sg552",	"weapon_sg552" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SNIPERRIFLE,	"awp",		"weapon_awp" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SNIPERRIFLE,	"g3sg1",	"weapon_g3sg1" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"famas",	"weapon_famas" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"m4a1",		"weapon_m4a1" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_RIFLE,		"aug",		"weapon_aug" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SNIPERRIFLE,	"sg550",	"weapon_sg550" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"glock",	"weapon_glock18" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"usp",		"weapon_usp" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"p228",		"weapon_p228" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"deagle",	"weapon_deagle" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"elites",	"weapon_elite" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_PISTOL,		"fn57",		"weapon_fiveseven" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SHOTGUN,		"m3",		"weapon_m3" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SHOTGUN,		"xm1014",	"weapon_xm1014" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SMG,		"mac10",	"weapon_mac10" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SMG,		"tmp",		"weapon_tmp" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SMG,		"mp5",		"weapon_mp5navy" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SMG,		"ump45",	"weapon_ump45" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SMG,		"p90",		"weapon_p90" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_MACHINEGUN,		"m249",		"weapon_m249" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_AMMO,		"primammo",	"primammo" },
	{ AUTOBUYCLASS_SECONDARY|AUTOBUYCLASS_AMMO,		"secammo",	"secammo" },
	{ AUTOBUYCLASS_ARMOR,					"vest",		"item_kevlar" },
	{ AUTOBUYCLASS_ARMOR,					"vesthelm",	"item_assaultsuit" },
	{ AUTOBUYCLASS_GRENADE,					"flash",	"weapon_flashbang" },
	{ AUTOBUYCLASS_GRENADE,					"hegren",	"weapon_hegrenade" },
	{ AUTOBUYCLASS_GRENADE,					"sgren",	"weapon_smokegrenade"},
	{ AUTOBUYCLASS_NIGHTVISION,				"nvgs",		"nvgs" },
	{ AUTOBUYCLASS_DEFUSER,					"defuser",	"defuser" },
	{ AUTOBUYCLASS_PRIMARY|AUTOBUYCLASS_SHIELD,		"shield",	"shield" },
	{ AUTOBUYCLASS_NONE, NULL, NULL }
};

WeaponAliasInfo weaponAliasInfo[] =
{
	{ "p228",	WEAPON_P228 },
	{ "???",	WEAPON_GLOCK },
	{ "scout",	WEAPON_SCOUT },
	{ "hegren",	WEAPON_HEGRENADE },
	{ "xm1014",	WEAPON_XM1014 },
	{ "c4",		WEAPON_C4 },
	{ "mac10",	WEAPON_MAC10 },
	{ "aug",	WEAPON_AUG },
	{ "sgren",	WEAPON_SMOKEGRENADE },
	{ "elites",	WEAPON_ELITE },
	{ "fn57",	WEAPON_FIVESEVEN },
	{ "ump45",	WEAPON_UMP45 },
	{ "sg550",	WEAPON_SG550 },
	{ "galil",	WEAPON_GALIL },
	{ "famas",	WEAPON_FAMAS },
	{ "usp",	WEAPON_USP },
	{ "glock",	WEAPON_GLOCK18 },
	{ "awp",	WEAPON_AWP },
	{ "mp5",	WEAPON_MP5N },
	{ "m249",	WEAPON_M249 },
	{ "m3",		WEAPON_M3 },
	{ "m4a1",	WEAPON_M4A1 },
	{ "tmp",	WEAPON_TMP },
	{ "g3sg1",	WEAPON_G3SG1 },
	{ "flash",	WEAPON_FLASHBANG },
	{ "deagle",	WEAPON_DEAGLE },
	{ "sg552",	WEAPON_SG552 },
	{ "ak47",	WEAPON_AK47 },
	{ "knife",	WEAPON_KNIFE },
	{ "p90",	WEAPON_P90 },
	{ "shield",	WEAPON_SHIELDGUN },
	{ "none",	WEAPON_NONE },
	{ "grenade",	WEAPON_HEGRENADE },
	{ "hegrenade",	WEAPON_HEGRENADE },
	{ "glock18",	WEAPON_GLOCK18 },
	{ "elite",	WEAPON_ELITE },
	{ "fiveseven",	WEAPON_FIVESEVEN },
	{ "mp5navy",	WEAPON_MP5N },
	{ NULL,		WEAPON_NONE }
};

WeaponBuyAliasInfo weaponBuyAliasInfo[] =
{
	{ "galil",		WEAPON_GALIL,		"#Galil" },
	{ "defender",		WEAPON_GALIL,		"#Galil" },
	{ "ak47",		WEAPON_AK47,		"#AK47" },
	{ "cv47",		WEAPON_AK47,		"#AK47" },
	{ "scout",		WEAPON_SCOUT,		NULL },
	{ "sg552",		WEAPON_SG552,		"#SG552" },
	{ "krieg552",		WEAPON_SG552,		"#SG552" },
	{ "awp",		WEAPON_AWP,		NULL },
	{ "magnum",		WEAPON_AWP,		NULL },
	{ "g3sg1",		WEAPON_G3SG1,		"#G3SG1" },
	{ "d3au1",		WEAPON_G3SG1,		"#G3SG1" },
	{ "famas",		WEAPON_FAMAS,		"#Famas" },
	{ "clarion",		WEAPON_FAMAS,		"#Famas" },
	{ "m4a1",		WEAPON_M4A1,		"#M4A1" },
	{ "aug",		WEAPON_AUG,		"#Aug" },
	{ "bullpup",		WEAPON_AUG,		"#Aug" },
	{ "sg550",		WEAPON_SG550,		"#SG550" },
	{ "krieg550",		WEAPON_SG550,		"#SG550" },
	{ "glock",		WEAPON_GLOCK18,		NULL },
	{ "9x19mm",		WEAPON_GLOCK18,		NULL },
	{ "usp",		WEAPON_USP,		NULL },
	{ "km45",		WEAPON_USP,		NULL },
	{ "p228",		WEAPON_P228,		NULL },
	{ "228compact",		WEAPON_P228,		NULL },
	{ "deagle",		WEAPON_DEAGLE,		NULL },
	{ "nighthaw",		WEAPON_DEAGLE,		NULL },
	{ "elites",		WEAPON_ELITE,		"#Beretta96G" },
	{ "fn57",		WEAPON_FIVESEVEN,	"#FiveSeven" },
	{ "fiveseven",		WEAPON_FIVESEVEN,	"#FiveSeven" },
	{ "m3",			WEAPON_M3,		NULL },
	{ "12gauge",		WEAPON_M3,		NULL },
	{ "xm1014",		WEAPON_XM1014,		NULL },
	{ "autoshotgun",	WEAPON_XM1014,		NULL },
	{ "mac10",		WEAPON_MAC10,		"#Mac10" },
	{ "tmp",		WEAPON_TMP,		"#tmp" },
	{ "mp",			WEAPON_TMP,		"#tmp" },
	{ "mp5",		WEAPON_MP5N,		NULL },
	{ "smg",		WEAPON_MP5N,		NULL },
	{ "ump45",		WEAPON_UMP45,		NULL },
	{ "p90",		WEAPON_P90,		NULL },
	{ "c90",		WEAPON_P90,		NULL },
	{ "m249",		WEAPON_M249,		NULL },
	{ NULL,			WEAPON_NONE,		NULL }
};

WeaponClassAliasInfo weaponClassAliasInfo[] =
{
	{ "p228",	WEAPONCLASS_PISTOL },
	{ "???",	WEAPONCLASS_PISTOL },
	{ "scout",	WEAPONCLASS_SNIPERRIFLE },
	{ "hegren",	WEAPONCLASS_GRENADE },
	{ "xm1014",	WEAPONCLASS_SHOTGUN },
	{ "c4",		WEAPONCLASS_GRENADE },
	{ "mac10",	WEAPONCLASS_SUBMACHINEGUN },
	{ "aug",	WEAPONCLASS_RIFLE },
	{ "sgren",	WEAPONCLASS_GRENADE },
	{ "elites",	WEAPONCLASS_PISTOL },
	{ "fn57",	WEAPONCLASS_PISTOL },
	{ "ump45",	WEAPONCLASS_SUBMACHINEGUN },
	{ "sg550",	WEAPONCLASS_SNIPERRIFLE },
	{ "galil",	WEAPONCLASS_RIFLE },
	{ "famas",	WEAPONCLASS_RIFLE },
	{ "usp",	WEAPONCLASS_PISTOL },
	{ "glock",	WEAPONCLASS_PISTOL },
	{ "awp",	WEAPONCLASS_SNIPERRIFLE },
	{ "mp5",	WEAPONCLASS_SUBMACHINEGUN },
	{ "m249",	WEAPONCLASS_MACHINEGUN },
	{ "m3",		WEAPONCLASS_SHOTGUN },
	{ "m4a1",	WEAPONCLASS_RIFLE },
	{ "tmp",	WEAPONCLASS_SUBMACHINEGUN },
	{ "g3sg1",	WEAPONCLASS_SNIPERRIFLE },
	{ "flash",	WEAPONCLASS_GRENADE },
	{ "deagle",	WEAPONCLASS_PISTOL },
	{ "sg552",	WEAPONCLASS_RIFLE },
	{ "ak47",	WEAPONCLASS_RIFLE },
	{ "knife",	WEAPONCLASS_KNIFE },
	{ "p90",	WEAPONCLASS_SUBMACHINEGUN },
	{ "shield",	WEAPONCLASS_PISTOL },
	{ "grenade",	WEAPONCLASS_GRENADE },
	{ "hegrenade",	WEAPONCLASS_GRENADE },
	{ "glock18",	WEAPONCLASS_PISTOL },
	{ "elite",	WEAPONCLASS_PISTOL },
	{ "fiveseven",	WEAPONCLASS_PISTOL },
	{ "mp5navy",	WEAPONCLASS_SUBMACHINEGUN },
	{ "grenade",	WEAPONCLASS_GRENADE },
	{ "pistol",	WEAPONCLASS_PISTOL },
	{ "SMG",	WEAPONCLASS_SUBMACHINEGUN },
	{ "machinegun",	WEAPONCLASS_MACHINEGUN },
	{ "shotgun",	WEAPONCLASS_SHOTGUN },
	{ "rifle",	WEAPONCLASS_RIFLE },
	{ "sniper",	WEAPONCLASS_SNIPERRIFLE },
	{ "none",	WEAPONCLASS_NONE },
	{ NULL,		WEAPONCLASS_NONE }
};

WeaponInfoStruct weaponInfo[31];
WeaponInfoStruct weaponInfo_default[] =
{
	{ WEAPON_P228,		P228_PRICE,		AMMO_357SIG_PRICE,	AMMO_357SIG_BUY,	P228_MAX_CLIP,		MAX_AMMO_357SIG,	AMMO_357SIG,		"weapon_p228",		"ammo_357sig" },
#ifdef REGAMEDLL_FIXES
																						// fix collision with glock18
	{ WEAPON_GLOCK,		GLOCK18_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		GLOCK18_MAX_CLIP,	MAX_AMMO_9MM,		AMMO_9MM,		"weapon_glock",		"ammo_9mm" },
#else
	{ WEAPON_GLOCK,		GLOCK18_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		GLOCK18_MAX_CLIP,	MAX_AMMO_9MM,		AMMO_9MM,		"weapon_glock18",	"ammo_9mm" },
#endif
	{ WEAPON_GLOCK18,	GLOCK18_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		GLOCK18_MAX_CLIP,	MAX_AMMO_9MM,		AMMO_9MM,		"weapon_glock18",	"ammo_9mm" },
	{ WEAPON_SCOUT,		SCOUT_PRICE,		AMMO_762MM_PRICE,	AMMO_762NATO_BUY,	SCOUT_MAX_CLIP,		MAX_AMMO_762NATO,	AMMO_762NATO,		"weapon_scout",		"ammo_762nato" },
	{ WEAPON_XM1014,	XM1014_PRICE,		AMMO_BUCKSHOT_PRICE,	AMMO_BUCKSHOT_BUY,	XM1014_MAX_CLIP,	MAX_AMMO_BUCKSHOT,	AMMO_BUCKSHOT,		"weapon_xm1014",	"ammo_buckshot" },
	{ WEAPON_MAC10,		MAC10_PRICE,		AMMO_45ACP_PRICE,	AMMO_45ACP_BUY,		MAC10_MAX_CLIP,		MAX_AMMO_45ACP,		AMMO_45ACP,		"weapon_mac10",		"ammo_45acp" },
	{ WEAPON_AUG,		AUG_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	AUG_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_aug",		"ammo_556nato" },
	{ WEAPON_ELITE,		ELITE_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		ELITE_MAX_CLIP,		MAX_AMMO_9MM,		AMMO_9MM,		"weapon_elite",		"ammo_9mm" },
	{ WEAPON_FIVESEVEN,	FIVESEVEN_PRICE,	AMMO_57MM_PRICE,	AMMO_57MM_BUY,		FIVESEVEN_MAX_CLIP,	MAX_AMMO_57MM,		AMMO_57MM,		"weapon_fiveseven",	"ammo_57mm" },
	{ WEAPON_UMP45,		UMP45_PRICE,		AMMO_45ACP_PRICE,	AMMO_45ACP_BUY, 	UMP45_MAX_CLIP,		MAX_AMMO_45ACP,		AMMO_45ACP,		"weapon_ump45",		"ammo_45acp" },
	{ WEAPON_SG550,		SG550_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	SG550_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_sg550",		"ammo_556nato" },
	{ WEAPON_GALIL,		GALIL_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	GALIL_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_galil",		"ammo_556nato" },
	{ WEAPON_FAMAS,		FAMAS_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	FAMAS_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_famas",		"ammo_556nato" },
	{ WEAPON_USP,		USP_PRICE,		AMMO_45ACP_PRICE,	AMMO_45ACP_BUY,		USP_MAX_CLIP,		MAX_AMMO_45ACP,		AMMO_45ACP,		"weapon_usp",		"ammo_45acp" },
	{ WEAPON_AWP,		AWP_PRICE,		AMMO_338MAG_PRICE,	AMMO_338MAG_BUY,	AWP_MAX_CLIP,		MAX_AMMO_338MAGNUM,	AMMO_338MAGNUM,		"weapon_awp",		"ammo_338magnum" },
	{ WEAPON_MP5N,		MP5NAVY_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		MP5N_MAX_CLIP,		MAX_AMMO_9MM,		AMMO_9MM,		"weapon_mp5navy",	"ammo_9mm" },
	{ WEAPON_M249,		M249_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATOBOX_BUY,	M249_MAX_CLIP,		MAX_AMMO_556NATOBOX,	AMMO_556NATOBOX,	"weapon_m249",		"ammo_556natobox" },
	{ WEAPON_M3,		M3_PRICE,		AMMO_BUCKSHOT_PRICE,	AMMO_BUCKSHOT_BUY,	M3_MAX_CLIP,		MAX_AMMO_BUCKSHOT,	AMMO_BUCKSHOT,		"weapon_m3",		"ammo_buckshot" },
	{ WEAPON_M4A1,		M4A1_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	M4A1_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_m4a1",		"ammo_556nato" },
	{ WEAPON_TMP,		TMP_PRICE,		AMMO_9MM_PRICE,		AMMO_9MM_BUY,		TMP_MAX_CLIP,		MAX_AMMO_9MM,		AMMO_9MM,		"weapon_tmp",		"ammo_9mm" },
	{ WEAPON_G3SG1,		G3SG1_PRICE,		AMMO_762MM_PRICE,	AMMO_762NATO_BUY,	G3SG1_MAX_CLIP,		MAX_AMMO_762NATO,	AMMO_762NATO,		"weapon_g3sg1",		"ammo_762nato" },
	{ WEAPON_DEAGLE,	DEAGLE_PRICE,		AMMO_50AE_PRICE,	AMMO_50AE_BUY,		DEAGLE_MAX_CLIP,	MAX_AMMO_50AE,		AMMO_50AE,		"weapon_deagle",	"ammo_50ae" },
	{ WEAPON_SG552,		SG552_PRICE,		AMMO_556MM_PRICE,	AMMO_556NATO_BUY,	SG552_MAX_CLIP,		MAX_AMMO_556NATO,	AMMO_556NATO,		"weapon_sg552",		"ammo_556nato" },
	{ WEAPON_AK47,		AK47_PRICE,		AMMO_762MM_PRICE,	AMMO_762NATO_BUY,	AK47_MAX_CLIP,		MAX_AMMO_762NATO,	AMMO_762NATO,		"weapon_ak47",		"ammo_762nato" },
	{ WEAPON_P90,		P90_PRICE,		AMMO_57MM_PRICE,	AMMO_57MM_BUY,		P90_MAX_CLIP,		MAX_AMMO_57MM,		AMMO_57MM,		"weapon_p90",		"ammo_57mm" },

#ifdef REGAMEDLL_ADD
	{ WEAPON_C4,		0,					0,			0,			0,			0,			AMMO_C4,		"weapon_c4",		nullptr },
	{ WEAPON_KNIFE,		0,					0,			0,			0,			0,			AMMO_NONE,		"weapon_knife",		nullptr },
	{ WEAPON_HEGRENADE,	(WeaponCostType)HEGRENADE_PRICE,	0,			0,			0,			MAX_AMMO_HEGRENADE,	AMMO_HEGRENADE,		"weapon_hegrenade",	nullptr },
	{ WEAPON_SMOKEGRENADE,	(WeaponCostType)SMOKEGRENADE_PRICE,	0,			0,			0,			MAX_AMMO_SMOKEGRENADE,	AMMO_SMOKEGRENADE,	"weapon_smokegrenade",	nullptr },
	{ WEAPON_FLASHBANG,	(WeaponCostType)FLASHBANG_PRICE,	0,			0,			0,			MAX_AMMO_FLASHBANG,	AMMO_FLASHBANG,		"weapon_flashbang",	nullptr },
#endif

	{ WEAPON_SHIELDGUN,	SHIELDGUN_PRICE,			0,			0,			0,			0,			AMMO_NONE,		"weapon_shield",	nullptr },
#ifndef REGAMEDLL_ADD
	{ 0,			0,					0,			0,			0,			0,			AMMO_NONE,		nullptr,		nullptr }
#endif
};

#endif // HOOK_GAMEDLL

WeaponSlotInfo weaponSlotInfo[] = {
	{ WEAPON_C4,		C4_SLOT,		"weapon_c4" },
	{ WEAPON_KNIFE,		KNIFE_SLOT,		"weapon_knife" },
	{ WEAPON_P228,		PISTOL_SLOT,		"weapon_p228" },
	{ WEAPON_GLOCK,		PISTOL_SLOT,		"weapon_glock" },
	{ WEAPON_ELITE,		PISTOL_SLOT,		"weapon_elite" },
	{ WEAPON_FIVESEVEN,	PISTOL_SLOT,		"weapon_fiveseven" },
	{ WEAPON_USP,		PISTOL_SLOT,		"weapon_usp" },
	{ WEAPON_GLOCK18,	PISTOL_SLOT,		"weapon_glock18" },
	{ WEAPON_DEAGLE,	PISTOL_SLOT,		"weapon_deagle" },
	{ WEAPON_HEGRENADE,	GRENADE_SLOT,		"weapon_hegrenade" },
	{ WEAPON_SMOKEGRENADE,	GRENADE_SLOT,		"weapon_smokegrenade" },
	{ WEAPON_FLASHBANG,	GRENADE_SLOT,		"weapon_flashbang" },
	{ WEAPON_SCOUT,		PRIMARY_WEAPON_SLOT,	"weapon_scout" },
	{ WEAPON_XM1014,	PRIMARY_WEAPON_SLOT,	"weapon_xm1014" },
	{ WEAPON_MAC10,		PRIMARY_WEAPON_SLOT,	"weapon_mac10" },
	{ WEAPON_AUG,		PRIMARY_WEAPON_SLOT,	"weapon_aug" },
	{ WEAPON_UMP45,		PRIMARY_WEAPON_SLOT,	"weapon_ump45" },
	{ WEAPON_SG550,		PRIMARY_WEAPON_SLOT,	"weapon_sg550" },
	{ WEAPON_GALIL,		PRIMARY_WEAPON_SLOT,	"weapon_galil" },
	{ WEAPON_FAMAS,		PRIMARY_WEAPON_SLOT,	"weapon_famas" },
	{ WEAPON_AWP,		PRIMARY_WEAPON_SLOT,	"weapon_awp" },
	{ WEAPON_MP5N,		PRIMARY_WEAPON_SLOT,	"weapon_mp5navy" },
	{ WEAPON_M249,		PRIMARY_WEAPON_SLOT,	"weapon_m249" },
	{ WEAPON_M3,		PRIMARY_WEAPON_SLOT,	"weapon_m3" },
	{ WEAPON_M4A1,		PRIMARY_WEAPON_SLOT,	"weapon_m4a1" },
	{ WEAPON_TMP,		PRIMARY_WEAPON_SLOT,	"weapon_tmp" },
	{ WEAPON_G3SG1,		PRIMARY_WEAPON_SLOT,	"weapon_g3sg1" },
	{ WEAPON_SG552,		PRIMARY_WEAPON_SLOT,	"weapon_sg552" },
	{ WEAPON_AK47,		PRIMARY_WEAPON_SLOT,	"weapon_ak47" },
	{ WEAPON_P90,		PRIMARY_WEAPON_SLOT,	"weapon_p90" },
	{ WEAPON_SHIELDGUN,	NONE_SLOT,		"weapon_shield" }
};

// Given an alias, return the associated weapon ID
WeaponIdType AliasToWeaponID(const char *alias)
{
	if (alias != NULL)
	{
		for (int i = 0; weaponAliasInfo[i].alias != NULL; ++i)
		{
			if (!Q_stricmp(weaponAliasInfo[i].alias, alias))
				return weaponAliasInfo[i].id;
		}
	}

	return WEAPON_NONE;
}

const char *BuyAliasToWeaponID(const char *alias, WeaponIdType &id)
{
	if (alias)
	{
		for (int i = 0; weaponBuyAliasInfo[i].alias != NULL; ++i)
		{
			if (!Q_stricmp(weaponBuyAliasInfo[i].alias, alias))
			{
				id = weaponBuyAliasInfo[i].id;
				return weaponBuyAliasInfo[i].failName;
			}
		}
	}

	id = WEAPON_NONE;
	return NULL;
}

// Given a weapon ID, return its alias
const char *WeaponIDToAlias(int id)
{
	for (int i = 0; weaponAliasInfo[i].alias != NULL; ++i)
	{
		if (weaponAliasInfo[i].id == id)
			return weaponAliasInfo[i].alias;
	}

	return NULL;
}

WeaponClassType AliasToWeaponClass(const char *alias)
{
	if (alias != NULL)
	{
		for (int i = 0; weaponClassAliasInfo[i].alias != NULL; ++i)
		{
			if (!Q_stricmp(weaponClassAliasInfo[i].alias, alias))
				return weaponClassAliasInfo[i].id;
		}
	}

	return WEAPONCLASS_NONE;
}

WeaponClassType WeaponIDToWeaponClass(int id)
{
	return AliasToWeaponClass(WeaponIDToAlias(id));
}

// Return true if given weapon ID is a primary weapon
bool IsPrimaryWeapon(int id)
{
	switch (id)
	{
	case WEAPON_SCOUT:
	case WEAPON_XM1014:
	case WEAPON_MAC10:
	case WEAPON_AUG:
	case WEAPON_UMP45:
	case WEAPON_SG550:
	case WEAPON_GALIL:
	case WEAPON_FAMAS:
	case WEAPON_AWP:
	case WEAPON_MP5N:
	case WEAPON_M249:
	case WEAPON_M3:
	case WEAPON_M4A1:
	case WEAPON_TMP:
	case WEAPON_G3SG1:
	case WEAPON_SG552:
	case WEAPON_AK47:
	case WEAPON_P90:
	case WEAPON_SHIELDGUN:
		return true;
	default:
		break;
	}

	return false;
}

// Return true if given weapon ID is a secondary weapon
bool IsSecondaryWeapon(int id)
{
	switch (id)
	{
	case WEAPON_P228:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_USP:
	case WEAPON_GLOCK18:
	case WEAPON_DEAGLE:
		return true;
	default:
		break;
	}

	return false;
}

WeaponInfoStruct* GetWeaponInfo(int weaponID)
{
	for (auto& info : weaponInfo) {
		if (info.id == weaponID) {
			return &info;
		}
	}

	return nullptr;
}

WeaponInfoStruct* GetWeaponInfo(const char* weaponName)
{
	for (auto& info : weaponInfo) {
		if (!Q_stricmp(info.entityName, weaponName)) {
			return &info;
		}
	}

	return nullptr;
}

void WeaponInfoReset()
{
#ifndef HOOK_GAMEDLL
	Q_memcpy(weaponInfo, weaponInfo_default, sizeof(weaponInfo));
#endif
}

WeaponSlotInfo* GetWeaponSlot(WeaponIdType weaponID)
{
	for (auto& infoSlot : weaponSlotInfo) {
		if (infoSlot.id == weaponID) {
			return &infoSlot;
		}
	}

	return nullptr;
}

WeaponSlotInfo* GetWeaponSlot(const char* weaponName)
{
	for (auto& infoSlot : weaponSlotInfo) {
		if (!Q_stricmp(infoSlot.weaponName, weaponName)) {
			return &infoSlot;
		}
	}

	return nullptr;
}

bool CanBuyWeaponByMaptype(int playerTeam, WeaponIdType weaponID, bool useAssasinationRestrictions)
{
	if (useAssasinationRestrictions)
	{
		if (playerTeam == CT)
		{
			switch (weaponID)
			{
			case WEAPON_P228:
			case WEAPON_XM1014:
			case WEAPON_AUG:
			case WEAPON_FIVESEVEN:
			case WEAPON_UMP45:
			case WEAPON_SG550:
			case WEAPON_FAMAS:
			case WEAPON_USP:
			case WEAPON_GLOCK18:
			case WEAPON_MP5N:
			case WEAPON_M249:
			case WEAPON_M3:
			case WEAPON_M4A1:
			case WEAPON_TMP:
			case WEAPON_DEAGLE:
			case WEAPON_P90:
			case WEAPON_SHIELDGUN:
				return true;
			default:
				return false;
			}
		}
		else if (playerTeam == TERRORIST)
		{
			switch (weaponID)
			{
			case WEAPON_P228:
			case WEAPON_MAC10:
			case WEAPON_ELITE:
			case WEAPON_UMP45:
			case WEAPON_GALIL:
			case WEAPON_USP:
			case WEAPON_GLOCK18:
			case WEAPON_AWP:
			case WEAPON_DEAGLE:
			case WEAPON_AK47:
				return true;
			default:
				return false;
			}
		}

		return false;
	}
	if (playerTeam == CT)
	{
		switch (weaponID)
		{
		case WEAPON_P228:
		case WEAPON_SCOUT:
		case WEAPON_XM1014:
		case WEAPON_AUG:
		case WEAPON_FIVESEVEN:
		case WEAPON_UMP45:
		case WEAPON_SG550:
		case WEAPON_FAMAS:
		case WEAPON_USP:
		case WEAPON_GLOCK18:
		case WEAPON_AWP:
		case WEAPON_MP5N:
		case WEAPON_M249:
		case WEAPON_M3:
		case WEAPON_M4A1:
		case WEAPON_TMP:
		case WEAPON_DEAGLE:
		case WEAPON_P90:
		case WEAPON_SHIELDGUN:
			return true;
		default:
			return false;
		}
	}
	else if (playerTeam == TERRORIST)
	{
		switch (weaponID)
		{
		case WEAPON_P228:
		case WEAPON_SCOUT:
		case WEAPON_XM1014:
		case WEAPON_MAC10:
		case WEAPON_ELITE:
		case WEAPON_UMP45:
		case WEAPON_GALIL:
		case WEAPON_USP:
		case WEAPON_GLOCK18:
		case WEAPON_AWP:
		case WEAPON_MP5N:
		case WEAPON_M249:
		case WEAPON_M3:
		case WEAPON_G3SG1:
		case WEAPON_DEAGLE:
		case WEAPON_SG552:
		case WEAPON_AK47:
		case WEAPON_P90:
			return true;
		default:
			return false;
		}
	}

	return false;
}
