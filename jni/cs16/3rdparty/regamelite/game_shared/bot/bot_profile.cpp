// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"
#include "game_shared/simple_checksum.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

BotProfileManager *TheBotProfiles = NULL;
char *BotDifficultyName[] = { "EASY", "NORMAL", "HARD", "EXPERT", NULL };

#endif

// Generates a filename-decorated skin name
const char *GetDecoratedSkinName(const char *name, const char *filename)
{
	const int BufLen = MAX_PATH + 64;
	static char buf[BufLen];
	Q_snprintf(buf, BufLen, "%s/%s", filename, name);
	return buf;
}

const char *BotProfile::GetWeaponPreferenceAsString(int i) const
{
	if (i < 0 || i >= m_weaponPreferenceCount)
		return NULL;

	return WeaponIDToAlias(m_weaponPreference[i]);
}

// Return true if this profile has a primary weapon preference
bool BotProfile::HasPrimaryPreference() const
{
	for (int i = 0; i < m_weaponPreferenceCount; ++i)
	{
		int weaponClass = AliasToWeaponClass(WeaponIDToAlias(m_weaponPreference[i]));

		if (weaponClass == WEAPONCLASS_SUBMACHINEGUN ||
				weaponClass == WEAPONCLASS_SHOTGUN ||
				weaponClass == WEAPONCLASS_MACHINEGUN ||
				weaponClass == WEAPONCLASS_RIFLE ||
				weaponClass == WEAPONCLASS_SNIPERRIFLE)
			return true;
	}

	return false;
}

// Return true if this profile has a pistol weapon preference
bool BotProfile::HasPistolPreference() const
{
	for (int i = 0; i < m_weaponPreferenceCount; ++i)
	{
		if (AliasToWeaponClass(WeaponIDToAlias(m_weaponPreference[i])) == WEAPONCLASS_PISTOL)
			return true;
	}

	return false;
}

// Return true if this profile is valid for the specified team
bool BotProfile::IsValidForTeam(BotProfileTeamType team) const
{
	return (team == BOT_TEAM_ANY || m_teams == BOT_TEAM_ANY || team == m_teams);
}

BotProfileManager::BotProfileManager()
{
	m_nextSkin = 0;
	for (int i = 0; i < NumCustomSkins; ++i)
	{
		m_skins[i] = NULL;
		m_skinFilenames[i] = NULL;
		m_skinModelnames[i] = NULL;
	}
}

// Load the bot profile database
void BotProfileManager::Init(const char *filename, unsigned int *checksum)
{
	int dataLength;
	char *dataPointer = (char *)LOAD_FILE_FOR_ME(const_cast<char *>(filename), &dataLength);
	const char *dataFile = dataPointer;

	if (dataFile == NULL)
	{
		if (g_bIsCzeroGame)
		{
			CONSOLE_ECHO("WARNING: Cannot access bot profile database '%s'\n", filename);
		}

		return;
	}

	// compute simple checksum
	if (checksum)
	{
		*checksum = ComputeSimpleChecksum((const unsigned char *)dataPointer, dataLength);
	}

	// keep list of templates used for inheritance
	BotProfileList templateList;
	BotProfile defaultProfile;

	// Parse the BotProfile.db into BotProfile instances
	while (true)
	{
		dataFile = SharedParse(dataFile);
		if (!dataFile)
			break;

		char *token = SharedGetToken();

		bool isDefault = (!Q_stricmp(token, "Default"));
		bool isTemplate = (!Q_stricmp(token, "Template"));
		bool isCustomSkin = (!Q_stricmp(token, "Skin"));

		if (isCustomSkin)
		{
			const int BufLen = 64;
			char skinName[BufLen];

			// get skin name
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected skin name\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();
			Q_snprintf(skinName, BufLen, "%s", token);

			// get attribute name
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected 'Model'\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();
			if (Q_stricmp(token, "Model") != 0)
			{
				CONSOLE_ECHO("Error parsing %s - expected 'Model'\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			// eat '='
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected '='\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();
			if (Q_strcmp(token, "=") != 0)
			{
				CONSOLE_ECHO("Error parsing %s - expected '='\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			// get attribute value
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected attribute value\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();

			const char *decoratedName = GetDecoratedSkinName(skinName, filename);
			bool skinExists = GetCustomSkinIndex(decoratedName) > 0;
			if (m_nextSkin < NumCustomSkins && !skinExists)
			{
				// decorate the name
				m_skins[ m_nextSkin ] = CloneString(decoratedName);

				// construct the model filename
				m_skinModelnames[ m_nextSkin ] = CloneString(token);
				m_skinFilenames[ m_nextSkin ] = new char[ Q_strlen(token) * 2 + Q_strlen("models/player//.mdl") + 1 ];
				Q_sprintf(m_skinFilenames[ m_nextSkin ], "models/player/%s/%s.mdl", token, token);
				++m_nextSkin;
			}

			// eat 'End'
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected 'End'\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();
			if (Q_strcmp(token, "End") != 0)
			{
				CONSOLE_ECHO("Error parsing %s - expected 'End'\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			// it's just a custom skin - no need to do inheritance on a bot profile, etc.
			continue;
		}

		// encountered a new profile
		BotProfile *profile;
		if (isDefault)
		{
			profile = &defaultProfile;
		}
		else
		{
			profile = new BotProfile;
			// always inherit from Default
			*profile = defaultProfile;
		}

		// do inheritance in order of appearance
		if (!isTemplate && !isDefault)
		{
			const BotProfile *inherit = NULL;

			// template names are separated by "+"
			while (true)
			{
				char *c = Q_strchr(token, '+');
				if (c)
					*c = '\0';

				// find the given template name
				for (BotProfileList::iterator iter = templateList.begin(); iter != templateList.end(); ++iter)
				{
					if (!Q_stricmp((*iter)->GetName(), token))
					{
						inherit = (*iter);
						break;
					}
				}

				if (inherit == NULL)
				{
					CONSOLE_ECHO("Error parsing '%s' - invalid template reference '%s'\n", filename, token);
					FREE_FILE(dataPointer);
					return;
				}

				// inherit the data
				profile->Inherit(inherit, &defaultProfile);

				if (c == NULL)
					break;

				token = c + 1;
			}
		}

		// get name of this profile
		if (!isDefault)
		{
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing '%s' - expected name\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			profile->m_name = CloneString(SharedGetToken());

			// HACK HACK
			// Until we have a generalized means of storing bot preferences, we're going to hardcode the bot's
			// preference towards silencers based on his name.
			if (profile->m_name[0] % 2)
			{
				profile->m_prefersSilencer = true;
			}
		}

		// read attributes for this profile
		bool isFirstWeaponPref = true;
		while (true)
		{
			// get next token
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected 'End'\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();

			// check for End delimiter
			if (!Q_stricmp(token, "End"))
				break;

			// found attribute name - keep it
			char attributeName[64];
			Q_strcpy(attributeName, token);

			// eat '='
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected '='\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();
			if (Q_strcmp(token, "=") != 0)
			{
				CONSOLE_ECHO("Error parsing %s - expected '='\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			// get attribute value
			dataFile = SharedParse(dataFile);
			if (!dataFile)
			{
				CONSOLE_ECHO("Error parsing %s - expected attribute value\n", filename);
				FREE_FILE(dataPointer);
				return;
			}

			token = SharedGetToken();

			// store value in appropriate attribute
			if (!Q_stricmp("Aggression", attributeName))
			{
				profile->m_aggression = Q_atof(token) / 100.0f;
			}
			else if (!Q_stricmp("Skill", attributeName))
			{
				profile->m_skill = Q_atof(token) / 100.0f;
			}
			else if (!Q_stricmp("Skin", attributeName))
			{
				profile->m_skin = Q_atoi(token);

				if (profile->m_skin == 0)
				{
					// Q_atoi() failed - try to look up a custom skin by name
					profile->m_skin = GetCustomSkinIndex(token, filename);
				}
			}
			else if (!Q_stricmp("Teamwork", attributeName))
			{
				profile->m_teamwork = Q_atof(token) / 100.0f;
			}
			else if (!Q_stricmp("Cost", attributeName))
			{
				profile->m_cost = Q_atoi(token);
			}
			else if (!Q_stricmp("VoicePitch", attributeName))
			{
				profile->m_voicePitch = Q_atoi(token);
			}
			else if (!Q_stricmp("VoiceBank", attributeName))
			{
				profile->m_voiceBank = FindVoiceBankIndex(token);
			}
			else if (!Q_stricmp("WeaponPreference", attributeName))
			{
				// weapon preferences override parent prefs
				if (isFirstWeaponPref)
				{
					isFirstWeaponPref = false;
					profile->m_weaponPreferenceCount = 0;
				}

				if (!Q_stricmp(token, "none"))
				{
					profile->m_weaponPreferenceCount = 0;
				}
				else
				{
					if (profile->m_weaponPreferenceCount < BotProfile::MAX_WEAPON_PREFS)
					{
						profile->m_weaponPreference[ profile->m_weaponPreferenceCount++ ] = AliasToWeaponID(token);
					}
				}
			}
			else if (!Q_stricmp("ReactionTime", attributeName))
			{
				profile->m_reactionTime = Q_atof(token);

#ifndef GAMEUI_EXPORTS
				// subtract off latency due to "think" update rate.
				// In GameUI, we don't really care.
				profile->m_reactionTime -= g_flBotFullThinkInterval;
#endif

			}
			else if (!Q_stricmp("AttackDelay", attributeName))
			{
				profile->m_attackDelay = Q_atof(token);
			}
			else if (!Q_stricmp("Difficulty", attributeName))
			{
				// override inheritance
				profile->m_difficultyFlags = 0;

				// parse bit flags
				while (true)
				{
					char *c = Q_strchr(token, '+');
					if (c)
						*c = '\0';

					for (int i = 0; i < NUM_DIFFICULTY_LEVELS; ++i)
					{
						if (!Q_stricmp(BotDifficultyName[i], token))
							profile->m_difficultyFlags |= (1 << i);
					}

					if (c == NULL)
						break;

					token = c + 1;
				}
			}
			else if (!Q_stricmp("Team", attributeName))
			{
				if (!Q_stricmp(token, "T"))
				{
					profile->m_teams = BOT_TEAM_T;
				}
				else if (!Q_stricmp(token, "CT"))
				{
					profile->m_teams = BOT_TEAM_CT;
				}
				else
				{
					profile->m_teams = BOT_TEAM_ANY;
				}
			}
			else
			{
				CONSOLE_ECHO("Error parsing %s - unknown attribute '%s'\n", filename, attributeName);
			}
		}

		if (!isDefault)
		{
			if (isTemplate)
			{
				// add to template list
				templateList.push_back(profile);
			}
			else
			{
				// add profile to the master list
				m_profileList.push_back(profile);
			}
		}
	}

	FREE_FILE(dataPointer);

	// free the templates
	for (BotProfileList::iterator iter = templateList.begin(); iter != templateList.end(); ++iter)
		delete (*iter);

	templateList.clear();
}

BotProfileManager::~BotProfileManager()
{
	Reset();

	for (VoiceBankList::iterator it = m_voiceBanks.begin(); it != m_voiceBanks.end(); ++it)
		delete[] (*it);

	m_voiceBanks.clear();
}

// Free all bot profiles
void BotProfileManager::Reset()
{
	for (BotProfileList::iterator iter = m_profileList.begin(); iter != m_profileList.end(); ++iter)
		delete (*iter);

	m_profileList.clear();

	for (int i = 0; i < NumCustomSkins; ++i)
	{
		if (m_skins[i])
		{
			delete[] m_skins[i];
			m_skins[i] = NULL;
		}
		if (m_skinFilenames[i])
		{
			delete[] m_skinFilenames[i];
			m_skinFilenames[i] = NULL;
		}
		if (m_skinModelnames[i])
		{
			delete[] m_skinModelnames[i];
			m_skinModelnames[i] = NULL;
		}
	}
}

// Returns custom skin name at a particular index
const char *BotProfileManager::GetCustomSkin(int index)
{
	if (index < FirstCustomSkin || index > LastCustomSkin)
	{
		return NULL;
	}

	return m_skins[ index - FirstCustomSkin ];
}

// Returns custom skin filename at a particular index
const char *BotProfileManager::GetCustomSkinFname(int index)
{
	if (index < FirstCustomSkin || index > LastCustomSkin)
	{
		return NULL;
	}

	return m_skinFilenames[ index - FirstCustomSkin ];
}

// Returns custom skin modelname at a particular index
const char *BotProfileManager::GetCustomSkinModelname(int index)
{
	if (index < FirstCustomSkin || index > LastCustomSkin)
	{
		return NULL;
	}

	return m_skinModelnames[ index - FirstCustomSkin ];
}

// Looks up a custom skin index by filename-decorated name (will decorate the name if filename is given)
int BotProfileManager::GetCustomSkinIndex(const char *name, const char *filename)
{
	const char *skinName = name;
	if (filename != NULL)
	{
		skinName = GetDecoratedSkinName(name, filename);
	}

	for (int i = 0; i < NumCustomSkins; ++i)
	{
		if (m_skins[i])
		{
			if (!Q_stricmp(skinName, m_skins[i]))
			{
				return FirstCustomSkin + i;
			}
		}
	}

	return 0;
}

// return index of the (custom) bot phrase db, inserting it if needed
int BotProfileManager::FindVoiceBankIndex(const char *filename)
{
	int index = 0;
	for (VoiceBankList::const_iterator it = m_voiceBanks.begin(); it != m_voiceBanks.end(); ++it, ++index)
	{
		if (!Q_stricmp(filename, *it))
		{
			return index;
		}
	}

	m_voiceBanks.push_back(CloneString(filename));
	return index;
}

// Return random unused profile that matches the given difficulty level
const BotProfile *BotProfileManager::GetRandomProfile(BotDifficultyType difficulty, BotProfileTeamType team) const
{
#ifdef RANDOM_LONG
	BotProfileList::const_iterator iter;

	// count up valid profiles
	int validCount = 0;
	for (iter = m_profileList.begin(); iter != m_profileList.end(); ++iter)
	{
		const BotProfile *profile = (*iter);

		if (profile->IsDifficulty(difficulty) && !UTIL_IsNameTaken(profile->GetName()) && profile->IsValidForTeam(team))
			++validCount;
	}

	if (validCount == 0)
		return NULL;

	// select one at random
	int which = RANDOM_LONG(0, validCount - 1);

	for (iter = m_profileList.begin(); iter != m_profileList.end(); ++iter)
	{
		const BotProfile *profile = (*iter);

		if (profile->IsDifficulty(difficulty) && !UTIL_IsNameTaken(profile->GetName()) && profile->IsValidForTeam(team))
		{
			if (which-- == 0)
				return profile;
		}
	}

	return NULL;
#else
	// we don't need random profiles when we're not in the game dll
	return NULL;
#endif // RANDOM_LONG
}
