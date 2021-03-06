#include "pch.h"
#include "selhero.h"

int selhero_SaveCount = 0;
_uiheroinfo heros[MAX_CHARACTERS];
_uiheroinfo heroInfo;
char listItems[6][16];
char textStats[5][4];
char title[32];
char selhero_Lable[32];
char selhero_Description[256];
int selhero_result;
bool selhero_endMenu;
bool isMultiPlayer;

BOOL(__stdcall *gfnHeroStats)
(unsigned int, _uidefaultstats *);
BOOL(__stdcall *gfnHeroCreate)
(_uiheroinfo *);

UI_Item SELHERO_DIALOG[] = {
	{ { 0, 0, 640, 480 }, UI_IMAGE, 0, 0, NULL, &ArtBackground },
	{ { 24, 161, 590, 35 }, UI_TEXT, UIS_CENTER | UIS_BIG, 0, title },
	{ { 30, 211, 180, 76 }, UI_IMAGE, 0, UI_NUM_CLASSES, NULL, &ArtHero },
	{ { 39, 323, 110, 21 }, UI_TEXT, UIS_RIGHT, 0, "Level:" },
	{ { 159, 323, 40, 21 }, UI_TEXT, UIS_CENTER, 0, textStats[0] },
	{ { 39, 358, 110, 21 }, UI_TEXT, UIS_RIGHT, 0, "Strength:" },
	{ { 159, 358, 40, 21 }, UI_TEXT, UIS_CENTER, 0, textStats[1] },
	{ { 39, 380, 110, 21 }, UI_TEXT, UIS_RIGHT, 0, "Magic:" },
	{ { 159, 380, 40, 21 }, UI_TEXT, UIS_CENTER, 0, textStats[2] },
	{ { 39, 401, 110, 21 }, UI_TEXT, UIS_RIGHT, 0, "Dexterity:" },
	{ { 159, 401, 40, 21 }, UI_TEXT, UIS_CENTER, 0, textStats[3] },
	{ { 39, 422, 110, 21 }, UI_TEXT, UIS_RIGHT, 0, "Vitality:" },
	{ { 159, 422, 40, 21 }, UI_TEXT, UIS_CENTER, 0, textStats[4] },
};

UI_Item SELLIST_DIALOG[] = {
	{ { 264, 211, 320, 33 }, UI_TEXT, UIS_CENTER | UIS_BIG, 0, "Select Hero" },
	{ { 265, 256, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 0, listItems[0] },
	{ { 265, 282, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 1, listItems[1] },
	{ { 265, 308, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 2, listItems[2] },
	{ { 265, 334, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 3, listItems[3] },
	{ { 265, 360, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 4, listItems[4] },
	{ { 265, 386, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 5, listItems[5] },
	{ { 239, 429, 120, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "OK", (void *)UiFocusNavigationSelect },
	{ { 364, 429, 120, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_DISABLED, 0, "Delete" },
	{ { 489, 429, 120, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "Cancel", (void *)UiFocusNavigationEsc },
};

UI_Item SELCLASS_DIALOG[] = {
	{ { 264, 211, 320, 33 }, UI_TEXT, UIS_CENTER | UIS_BIG, 0, "Choose Class" },
	{ { 264, 285, 320, 33 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, UI_WARRIOR, "Warrior" },
	{ { 264, 318, 320, 33 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, UI_ROGUE, "Rogue" },
	{ { 264, 352, 320, 33 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, UI_SORCERER, "Sorcerer" },
	{ { 279, 429, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "OK", (void *)UiFocusNavigationSelect },
	{ { 429, 429, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "Cancel", (void *)UiFocusNavigationEsc },
};

UI_Item ENTERNAME_DIALOG[] = {
	{ { 264, 211, 320, 33 }, UI_TEXT, UIS_CENTER | UIS_BIG, 0, "Enter Name" },
	{ { 265, 317, 320, 33 }, UI_EDIT, UIS_LIST | UIS_MED | UIS_GOLD, 15, heroInfo.name },
	{ { 279, 429, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "OK", (void *)UiFocusNavigationSelect },
	{ { 429, 429, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_BIG | UIS_GOLD, 0, "Cancel", (void *)UiFocusNavigationEsc },
};

UI_Item SELLOAD_DIALOG[] = {
	{ { 264, 211, 320, 33 }, UI_TEXT, UIS_CENTER | UIS_BIG, 0, "Save File Exists" },
	{ { 265, 285, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 0, "Load Game" },
	{ { 265, 318, 320, 26 }, UI_LIST, UIS_CENTER | UIS_MED | UIS_GOLD, 1, "New Game" },
	{ { 279, 427, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_VCENTER | UIS_BIG | UIS_GOLD, 0, "OK", (void *)UiFocusNavigationSelect },
	{ { 429, 427, 140, 35 }, UI_BUTTON, UIS_CENTER | UIS_VCENTER | UIS_BIG | UIS_GOLD, 0, "Cancel", (void *)UiFocusNavigationEsc },
};

void selhero_Free()
{
	mem_free_dbg(ArtBackground.data);
	ArtBackground.data = NULL;
	memset(listItems, 0, sizeof(listItems));
}

void selhero_SetStats()
{
	SELHERO_DIALOG[2].value = heroInfo.heroclass;
	sprintf(textStats[0], "%d", heroInfo.level);
	sprintf(textStats[1], "%d", heroInfo.strength);
	sprintf(textStats[2], "%d", heroInfo.magic);
	sprintf(textStats[3], "%d", heroInfo.dexterity);
	sprintf(textStats[4], "%d", heroInfo.vitality);
}

void selhero_List_Init()
{
	UiInitList(0, selhero_SaveCount, selhero_List_Focus, selhero_List_Select, selhero_List_Esc, SELLIST_DIALOG, size(SELLIST_DIALOG));
	int i;
	for (i = 0; i < selhero_SaveCount && i < 6; i++) {
		sprintf(listItems[i], heros[i].name);
	}
	if (i < 6)
		sprintf(listItems[i], "New Hero");

	sprintf(title, "Single Player Characters");
	if (isMultiPlayer) {
		sprintf(title, "Multi Player Characters");
	}
}

void selhero_List_Focus(int value)
{
	if (selhero_SaveCount && value < selhero_SaveCount) {
		memcpy(&heroInfo, &heros[value], sizeof(heroInfo));
		selhero_SetStats();
		return;
	}

	SELHERO_DIALOG[2].value = UI_NUM_CLASSES;
	sprintf(textStats[0], "--");
	sprintf(textStats[1], "--");
	sprintf(textStats[2], "--");
	sprintf(textStats[3], "--");
	sprintf(textStats[4], "--");
}

void selhero_List_Select(int value)
{
	if (value == selhero_SaveCount) {
		UiInitList(0, 2, selhero_ClassSelector_Focus, selhero_ClassSelector_Select, selhero_ClassSelector_Esc, SELCLASS_DIALOG, size(SELCLASS_DIALOG));
		memset(&heroInfo.name, 0, sizeof(heroInfo.name));
		sprintf(title, "New Single Player Hero");
		if (isMultiPlayer) {
			sprintf(title, "New Multi Player Hero");
		}
		return;
	} else if (heroInfo.hassaved) {
		UiInitList(0, 1, selhero_Load_Focus, selhero_Load_Select, selhero_List_Init, SELLOAD_DIALOG, size(SELLOAD_DIALOG), true);
		sprintf(title, "Single Player Characters");
		return;
	}

	UiInitList(0, 0, NULL, NULL, NULL, NULL, 0);
	selhero_endMenu = true;
}

void selhero_List_Esc()
{
	UiInitList(0, 0, NULL, NULL, NULL, NULL, 0);
	selhero_endMenu = true;
	selhero_result = EXIT_MENU;
}

void selhero_ClassSelector_Focus(int value)
{
	_uidefaultstats defaults;
	gfnHeroStats(value, &defaults);

	heroInfo.level = 1;
	heroInfo.heroclass = value;
	heroInfo.strength = defaults.strength;
	heroInfo.magic = defaults.magic;
	heroInfo.dexterity = defaults.dexterity;
	heroInfo.vitality = defaults.vitality;

	selhero_SetStats();
}

void selhero_ClassSelector_Select(int value)
{
	sprintf(title, "New Single Player Hero");
	if (isMultiPlayer) {
		sprintf(title, "New Multi Player Hero");
	}
	memset(heroInfo.name, '\0', sizeof(heroInfo.name));
	UiInitList(0, 0, NULL, selhero_Name_Select, selhero_Name_Esc, ENTERNAME_DIALOG, size(ENTERNAME_DIALOG));
}

void selhero_ClassSelector_Esc()
{
	if (selhero_SaveCount) {
		selhero_List_Init();
		return;
	}

	selhero_List_Esc();
}

void selhero_Name_Select(int value)
{
	UiInitList(0, 0, NULL, NULL, NULL, NULL, 0);
	gfnHeroCreate(&heroInfo);
	selhero_endMenu = true;
}

void selhero_Name_Esc()
{
	selhero_List_Select(selhero_SaveCount);
}

void selhero_Load_Focus(int value)
{
}

void selhero_Load_Select(int value)
{
	UiInitList(0, 0, NULL, NULL, NULL, NULL, 0);
	selhero_endMenu = true;
	if (value == 0) {
		selhero_result = LOAD_GAME;
		return;
	}

	selhero_result = NEW_GAME;
}

BOOL __stdcall SelHero_GetHeroInfo(_uiheroinfo *pInfo)
{
	heros[selhero_SaveCount] = *pInfo;
	selhero_SaveCount++;

	return TRUE;
}

BOOL UiSelHeroDialog(
    BOOL(__stdcall *fninfo)(BOOL(__stdcall *fninfofunc)(_uiheroinfo *)),
    BOOL(__stdcall *fncreate)(_uiheroinfo *),
    BOOL(__stdcall *fnstats)(unsigned int, _uidefaultstats *),
    int *dlgresult,
    char *name)
{
	selhero_result = *dlgresult;
	gfnHeroStats = fnstats;
	gfnHeroCreate = fncreate;
	LoadBackgroundArt("ui_art\\selhero.pcx");

	selhero_SaveCount = 0;
	fninfo(SelHero_GetHeroInfo);

	if (selhero_SaveCount) {
		selhero_List_Init();
	} else {
		selhero_List_Select(selhero_SaveCount);
	}

	selhero_endMenu = false;
	while (!selhero_endMenu) {
		UiRenderItems(SELHERO_DIALOG, size(SELHERO_DIALOG));
		UiRender();
	}
	BlackPalette();
	selhero_Free();

	strcpy(name, heroInfo.name);

	*dlgresult = selhero_result;
	return TRUE;
}

BOOL __stdcall UiSelHeroSingDialog(
    BOOL(__stdcall *fninfo)(BOOL(__stdcall *fninfofunc)(_uiheroinfo *)),
    BOOL(__stdcall *fncreate)(_uiheroinfo *),
    BOOL(__stdcall *fnremove)(_uiheroinfo *),
    BOOL(__stdcall *fnstats)(unsigned int, _uidefaultstats *),
    int *dlgresult,
    char *name,
    int *difficulty)
{
	isMultiPlayer = false;
	return UiSelHeroDialog(fninfo, fncreate, fnstats, dlgresult, name);
}

BOOL __stdcall UiSelHeroMultDialog(
    BOOL(__stdcall *fninfo)(BOOL(__stdcall *fninfofunc)(_uiheroinfo *)),
    BOOL(__stdcall *fncreate)(_uiheroinfo *),
    BOOL(__stdcall *fnremove)(_uiheroinfo *),
    BOOL(__stdcall *fnstats)(unsigned int, _uidefaultstats *),
    int *dlgresult,
    BOOL *hero_is_created,
    char *name)
{
	isMultiPlayer = true;
	return UiSelHeroDialog(fninfo, fncreate, fnstats, dlgresult, name);
}
