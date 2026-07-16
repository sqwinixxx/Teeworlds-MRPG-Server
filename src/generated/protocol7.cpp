#include <engine/shared/protocol.h>
#include <engine/message.h>
#include <generated/protocol7.h>
namespace protocol7 {
CNetObjHandler::CNetObjHandler()
{
	m_pMsgFailedOn = "";
	m_pObjFailedOn = "";
	m_NumObjFailures = 0;
}

const char *CNetObjHandler::FailedObjOn() const { return m_pObjFailedOn; }
int CNetObjHandler::NumObjFailures() const { return m_NumObjFailures; }
const char *CNetObjHandler::FailedMsgOn() const { return m_pMsgFailedOn; }




static const int max_int = 0x7fffffff;

bool CNetObjHandler::CheckInt(const char *pErrorMsg, int Value, int Min, int Max)
{
	if(Value < Min) { Value = Min; }
	if(Value > Max) { Value = Max; }
	return true;
}

bool CNetObjHandler::CheckFlag(const char *pErrorMsg, int Value, int Mask)
{
	if((Value&Mask) != Value) { m_pObjFailedOn = pErrorMsg; m_NumObjFailures++; return false; }
	return true;
}

const char *CNetObjHandler::ms_apObjNames[] = {
	"invalid",
	"PlayerInput",
	"Projectile",
	"Laser",
	"Pickup",
	"Flag",
	"GameData",
	"GameDataTeam",
	"GameDataFlag",
	"CharacterCore",
	"Character",
	"PlayerInfo",
	"SpectatorInfo",
	"De_ClientInfo",
	"De_GameInfo",
	"De_TuneParams",
	"Common",
	"Explosion",
	"Spawn",
	"HammerHit",
	"Death",
	"SoundWorld",
	"Damage",
	"PlayerInfoRace",
	"GameDataRace",
	"EffectMmo",
	"TextEffect",
	"Mmo_ClientInfo",
	"MmoPickup",
	"MmoProj",
	""
};

int CNetObjHandler::ms_aObjSizes[] = {
	0,
	sizeof(CNetObj_PlayerInput),
	sizeof(CNetObj_Projectile),
	sizeof(CNetObj_Laser),
	sizeof(CNetObj_Pickup),
	sizeof(CNetObj_Flag),
	sizeof(CNetObj_GameData),
	sizeof(CNetObj_GameDataTeam),
	sizeof(CNetObj_GameDataFlag),
	sizeof(CNetObj_CharacterCore),
	sizeof(CNetObj_Character),
	sizeof(CNetObj_PlayerInfo),
	sizeof(CNetObj_SpectatorInfo),
	sizeof(CNetObj_De_ClientInfo),
	sizeof(CNetObj_De_GameInfo),
	sizeof(CNetObj_De_TuneParams),
	sizeof(CNetEvent_Common),
	sizeof(CNetEvent_Explosion),
	sizeof(CNetEvent_Spawn),
	sizeof(CNetEvent_HammerHit),
	sizeof(CNetEvent_Death),
	sizeof(CNetEvent_SoundWorld),
	sizeof(CNetEvent_Damage),
	sizeof(CNetObj_PlayerInfoRace),
	sizeof(CNetObj_GameDataRace),
	sizeof(CNetEvent_EffectMmo),
	sizeof(CNetEvent_TextEffect),
	sizeof(CNetObj_Mmo_ClientInfo),
	sizeof(CNetObj_MmoPickup),
	sizeof(CNetObj_MmoProj),
	0
};

const char *CNetObjHandler::ms_apMsgNames[] = {
	"invalid",
	"Sv_Motd",
	"Sv_Broadcast",
	"Sv_Chat",
	"Sv_Team",
	"Sv_KillMsg",
	"Sv_TuneParams",
	"Sv_ExtraProjectile",
	"Sv_ReadyToEnter",
	"Sv_WeaponPickup",
	"Sv_Emoticon",
	"Sv_VoteClearOptions",
	"Sv_VoteOptionListAdd",
	"Sv_VoteOptionAdd",
	"Sv_VoteOptionRemove",
	"Sv_VoteSet",
	"Sv_VoteStatus",
	"Sv_ServerSettings",
	"Sv_ClientInfo",
	"Sv_GameInfo",
	"Sv_ClientDrop",
	"Sv_GameMsg",
	"De_ClientEnter",
	"De_ClientLeave",
	"Cl_Say",
	"Cl_SetTeam",
	"Cl_SetSpectatorMode",
	"Cl_StartInfo",
	"Cl_Kill",
	"Cl_ReadyChange",
	"Cl_Emoticon",
	"Cl_Vote",
	"Cl_CallVote",
	"Sv_SkinChange",
	"Cl_SkinChange",
	"Sv_RaceFinish",
	"Sv_Checkpoint",
	"Sv_CommandInfo",
	"Sv_CommandInfoRemove",
	"Cl_Command",
	"Cl_IsMmoServer",
	"Cl_ClientAuth",
	"Cl_TalkInteractive",
	"Sv_AfterIsMmoServer",
	"Sv_EquipItems",
	"Sv_VoteMmoOptionAdd",
	"Sv_ClientProgressAuth",
	"Sv_TalkText",
	"Sv_ClearTalkText",
	"Sv_AddQuestingProcessing",
	"Sv_ClearQuestingProcessing",
	"Sv_ProgressBar",
	"Sv_WorldMusic",
	"Sv_SendGuiInformationBox",
	"Cl_MailLetterActions",
	"Sv_SendMailLetterInfo",
	"Cl_SendMailLetterTo",
	"DialogCommon",
	"Sv_Dialog_Msgbox",
	"Sv_Dialog_Input",
	"Sv_Dialog_List",
	"Sv_Dialog_List_AddItem",
	""
};

const char *CNetObjHandler::GetObjName(int Type) const
{
	if(Type < 0 || Type >= NUM_NETOBJTYPES) return "(out of range)";
	return ms_apObjNames[Type];
};

int CNetObjHandler::GetObjSize(int Type) const
{
	if(Type < 0 || Type >= NUM_NETOBJTYPES) return 0;
	return ms_aObjSizes[Type];
};

const char *CNetObjHandler::GetMsgName(int Type) const
{
	if(Type < 0 || Type >= NUM_NETMSGTYPES) return "(out of range)";
	return ms_apMsgNames[Type];
};

int CNetObjHandler::ValidateObj(int Type, const void *pData, int Size)
{
	switch(Type)
	{
	case NETOBJTYPE_PLAYERINPUT:
	{
		const CNetObj_PlayerInput *pObj = (const CNetObj_PlayerInput *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Direction", pObj->m_Direction, -1, 1)) return -1;
		if(!CheckInt("m_Jump", pObj->m_Jump, 0, 1)) return -1;
		if(!CheckInt("m_Hook", pObj->m_Hook, 0, 1)) return -1;
		if(!CheckFlag("m_PlayerFlags", pObj->m_PlayerFlags, PLAYERFLAG_ADMIN|PLAYERFLAG_CHATTING|PLAYERFLAG_SCOREBOARD|PLAYERFLAG_READY|PLAYERFLAG_DEAD|PLAYERFLAG_WATCHING|PLAYERFLAG_BOT)) return -1;
		if(!CheckInt("m_WantedWeapon", pObj->m_WantedWeapon, 0, NUM_WEAPONS-1)) return -1;
		return 0;
	}

	case NETOBJTYPE_PROJECTILE:
	{
		const CNetObj_Projectile *pObj = (const CNetObj_Projectile *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Type", pObj->m_Type, 0, NUM_WEAPONS-1)) return -1;
		if(!CheckInt("m_StartTick", pObj->m_StartTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_LASER:
	{
		const CNetObj_Laser *pObj = (const CNetObj_Laser *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_StartTick", pObj->m_StartTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_PICKUP:
	{
		const CNetObj_Pickup *pObj = (const CNetObj_Pickup *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Type", pObj->m_Type, 0, 7)) return -1;
		return 0;
	}

	case NETOBJTYPE_FLAG:
	{
		const CNetObj_Flag *pObj = (const CNetObj_Flag *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Team", pObj->m_Team, TEAM_RED, TEAM_BLUE)) return -1;
		return 0;
	}

	case NETOBJTYPE_GAMEDATA:
	{
		const CNetObj_GameData *pObj = (const CNetObj_GameData *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_GameStartTick", pObj->m_GameStartTick, 0, max_int)) return -1;
		if(!CheckFlag("m_GameStateFlags", pObj->m_GameStateFlags, GAMESTATEFLAG_WARMUP|GAMESTATEFLAG_SUDDENDEATH|GAMESTATEFLAG_ROUNDOVER|GAMESTATEFLAG_GAMEOVER|GAMESTATEFLAG_PAUSED|GAMESTATEFLAG_STARTCOUNTDOWN)) return -1;
		if(!CheckInt("m_GameStateEndTick", pObj->m_GameStateEndTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_GAMEDATATEAM:
	{
		const CNetObj_GameDataTeam *pObj = (const CNetObj_GameDataTeam *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETOBJTYPE_GAMEDATAFLAG:
	{
		const CNetObj_GameDataFlag *pObj = (const CNetObj_GameDataFlag *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_FlagCarrierRed", pObj->m_FlagCarrierRed, FLAG_MISSING, MAX_CLIENTS-1)) return -1;
		if(!CheckInt("m_FlagCarrierBlue", pObj->m_FlagCarrierBlue, FLAG_MISSING, MAX_CLIENTS-1)) return -1;
		if(!CheckInt("m_FlagDropTickRed", pObj->m_FlagDropTickRed, 0, max_int)) return -1;
		if(!CheckInt("m_FlagDropTickBlue", pObj->m_FlagDropTickBlue, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_CHARACTERCORE:
	{
		const CNetObj_CharacterCore *pObj = (const CNetObj_CharacterCore *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Tick", pObj->m_Tick, 0, max_int)) return -1;
		if(!CheckInt("m_Direction", pObj->m_Direction, -1, 1)) return -1;
		if(!CheckInt("m_Jumped", pObj->m_Jumped, 0, 3)) return -1;
		if(!CheckInt("m_HookedPlayer", pObj->m_HookedPlayer, -1, MAX_CLIENTS-1)) return -1;
		if(!CheckInt("m_HookState", pObj->m_HookState, -1, 5)) return -1;
		if(!CheckInt("m_HookTick", pObj->m_HookTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_CHARACTER:
	{
		const CNetObj_Character *pObj = (const CNetObj_Character *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Health", pObj->m_Health, 0, 10)) return -1;
		if(!CheckInt("m_Armor", pObj->m_Armor, 0, 10)) return -1;
		if(!CheckInt("m_Weapon", pObj->m_Weapon, 0, NUM_WEAPONS-1)) return -1;
		if(!CheckInt("m_Emote", pObj->m_Emote, 0, 5)) return -1;
		if(!CheckInt("m_AttackTick", pObj->m_AttackTick, 0, max_int)) return -1;
		if(!CheckFlag("m_TriggeredEvents", pObj->m_TriggeredEvents, COREEVENTFLAG_GROUND_JUMP|COREEVENTFLAG_AIR_JUMP|COREEVENTFLAG_HOOK_ATTACH_PLAYER|COREEVENTFLAG_HOOK_ATTACH_GROUND|COREEVENTFLAG_HOOK_HIT_NOHOOK)) return -1;
		if(!CheckInt("m_Tick", pObj->m_Tick, 0, max_int)) return -1;
		if(!CheckInt("m_Direction", pObj->m_Direction, -1, 1)) return -1;
		if(!CheckInt("m_Jumped", pObj->m_Jumped, 0, 3)) return -1;
		if(!CheckInt("m_HookedPlayer", pObj->m_HookedPlayer, -1, MAX_CLIENTS-1)) return -1;
		if(!CheckInt("m_HookState", pObj->m_HookState, -1, 5)) return -1;
		if(!CheckInt("m_HookTick", pObj->m_HookTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_PLAYERINFO:
	{
		const CNetObj_PlayerInfo *pObj = (const CNetObj_PlayerInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckFlag("m_PlayerFlags", pObj->m_PlayerFlags, PLAYERFLAG_ADMIN|PLAYERFLAG_CHATTING|PLAYERFLAG_SCOREBOARD|PLAYERFLAG_READY|PLAYERFLAG_DEAD|PLAYERFLAG_WATCHING|PLAYERFLAG_BOT)) return -1;
		return 0;
	}

	case NETOBJTYPE_SPECTATORINFO:
	{
		const CNetObj_SpectatorInfo *pObj = (const CNetObj_SpectatorInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_SpecMode", pObj->m_SpecMode, 0, NUM_SPECMODES-1)) return -1;
		if(!CheckInt("m_SpectatorID", pObj->m_SpectatorID, -1, MAX_CLIENTS-1)) return -1;
		return 0;
	}

	case NETOBJTYPE_DE_CLIENTINFO:
	{
		const CNetObj_De_ClientInfo *pObj = (const CNetObj_De_ClientInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Local", pObj->m_Local, 0, 1)) return -1;
		if(!CheckInt("m_Team", pObj->m_Team, TEAM_SPECTATORS, TEAM_BLUE)) return -1;
		if(!CheckInt("m_aUseCustomColors[0]", pObj->m_aUseCustomColors[0], 0, 1)) return -1;
		if(!CheckInt("m_aUseCustomColors[1]", pObj->m_aUseCustomColors[1], 0, 1)) return -1;
		if(!CheckInt("m_aUseCustomColors[2]", pObj->m_aUseCustomColors[2], 0, 1)) return -1;
		if(!CheckInt("m_aUseCustomColors[3]", pObj->m_aUseCustomColors[3], 0, 1)) return -1;
		if(!CheckInt("m_aUseCustomColors[4]", pObj->m_aUseCustomColors[4], 0, 1)) return -1;
		if(!CheckInt("m_aUseCustomColors[5]", pObj->m_aUseCustomColors[5], 0, 1)) return -1;
		return 0;
	}

	case NETOBJTYPE_DE_GAMEINFO:
	{
		const CNetObj_De_GameInfo *pObj = (const CNetObj_De_GameInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckFlag("m_GameFlags", pObj->m_GameFlags, GAMEFLAG_TEAMS|GAMEFLAG_FLAGS|GAMEFLAG_SURVIVAL|GAMEFLAG_RACE)) return -1;
		if(!CheckInt("m_ScoreLimit", pObj->m_ScoreLimit, 0, max_int)) return -1;
		if(!CheckInt("m_TimeLimit", pObj->m_TimeLimit, 0, max_int)) return -1;
		if(!CheckInt("m_MatchNum", pObj->m_MatchNum, 0, max_int)) return -1;
		if(!CheckInt("m_MatchCurrent", pObj->m_MatchCurrent, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_DE_TUNEPARAMS:
	{
		const CNetObj_De_TuneParams *pObj = (const CNetObj_De_TuneParams *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_COMMON:
	{
		const CNetEvent_Common *pObj = (const CNetEvent_Common *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_EXPLOSION:
	{
		const CNetEvent_Explosion *pObj = (const CNetEvent_Explosion *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_SPAWN:
	{
		const CNetEvent_Spawn *pObj = (const CNetEvent_Spawn *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_HAMMERHIT:
	{
		const CNetEvent_HammerHit *pObj = (const CNetEvent_HammerHit *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_DEATH:
	{
		const CNetEvent_Death *pObj = (const CNetEvent_Death *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_ClientID", pObj->m_ClientID, 0, MAX_CLIENTS-1)) return -1;
		return 0;
	}

	case NETEVENTTYPE_SOUNDWORLD:
	{
		const CNetEvent_SoundWorld *pObj = (const CNetEvent_SoundWorld *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETEVENTTYPE_DAMAGE:
	{
		const CNetEvent_Damage *pObj = (const CNetEvent_Damage *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_ClientID", pObj->m_ClientID, 0, MAX_CLIENTS-1)) return -1;
		if(!CheckInt("m_HealthAmount", pObj->m_HealthAmount, 0, 9)) return -1;
		if(!CheckInt("m_ArmorAmount", pObj->m_ArmorAmount, 0, 9)) return -1;
		if(!CheckInt("m_Self", pObj->m_Self, 0, 1)) return -1;
		return 0;
	}

	case NETOBJTYPE_PLAYERINFORACE:
	{
		const CNetObj_PlayerInfoRace *pObj = (const CNetObj_PlayerInfoRace *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_RaceStartTick", pObj->m_RaceStartTick, 0, max_int)) return -1;
		return 0;
	}

	case NETOBJTYPE_GAMEDATARACE:
	{
		const CNetObj_GameDataRace *pObj = (const CNetObj_GameDataRace *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_BestTime", pObj->m_BestTime, -1, max_int)) return -1;
		if(!CheckInt("m_Precision", pObj->m_Precision, 0, 3)) return -1;
		if(!CheckFlag("m_RaceFlags", pObj->m_RaceFlags, RACEFLAG_HIDE_KILLMSG|RACEFLAG_FINISHMSG_AS_CHAT|RACEFLAG_KEEP_WANTED_WEAPON)) return -1;
		return 0;
	}

	case NETEVENTTYPE_EFFECTMMO:
	{
		const CNetEvent_EffectMmo *pObj = (const CNetEvent_EffectMmo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_EffectID", pObj->m_EffectID, 0, 1)) return -1;
		return 0;
	}

	case NETEVENTTYPE_TEXTEFFECT:
	{
		const CNetEvent_TextEffect *pObj = (const CNetEvent_TextEffect *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}

	case NETOBJTYPE_MMO_CLIENTINFO:
	{
		const CNetObj_Mmo_ClientInfo *pObj = (const CNetObj_Mmo_ClientInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Local", pObj->m_Local, 0, 1)) return -1;
		if(!CheckInt("m_MoodType", pObj->m_MoodType, 0, 6)) return -1;
		if(!CheckInt("m_WorldType", pObj->m_WorldType, 0, 1)) return -1;
		if(!CheckInt("m_ActiveQuest", pObj->m_ActiveQuest, 0, 1)) return -1;
		return 0;
	}

	case NETOBJTYPE_MMOPICKUP:
	{
		const CNetObj_MmoPickup *pObj = (const CNetObj_MmoPickup *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Type", pObj->m_Type, 0, 6)) return -1;
		return 0;
	}

	case NETOBJTYPE_MMOPROJ:
	{
		const CNetObj_MmoProj *pObj = (const CNetObj_MmoProj *)pData;
		if(sizeof(*pObj) != Size) return -1;
		if(!CheckInt("m_Weapon", pObj->m_Weapon, 0, NUM_WEAPONS-1)) return -1;
		if(!CheckInt("m_StartTick", pObj->m_StartTick, 0, max_int)) return -1;
		return 0;
	}

	}
	return -1;
};

void *CNetObjHandler::SecureUnpackMsg(int Type, CUnpacker *pUnpacker)
{
	m_pMsgFailedOn = 0;
	m_pObjFailedOn = 0;
	switch(Type)
	{
	case NETMSGTYPE_SV_MOTD:
	{
		CNetMsg_Sv_Motd *pMsg = (CNetMsg_Sv_Motd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMessage = pUnpacker->GetString();
	} break;

	case NETMSGTYPE_SV_BROADCAST:
	{
		CNetMsg_Sv_Broadcast *pMsg = (CNetMsg_Sv_Broadcast *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMessage = pUnpacker->GetString();
	} break;

	case NETMSGTYPE_SV_CHAT:
	{
		CNetMsg_Sv_Chat *pMsg = (CNetMsg_Sv_Chat *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Mode = pUnpacker->GetInt();
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_TargetID = pUnpacker->GetInt();
		pMsg->m_pMessage = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(!CheckInt("m_Mode", pMsg->m_Mode, 0, NUM_CHATS-1)) break;
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, -1, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_TargetID", pMsg->m_TargetID, -1, MAX_CLIENTS-1)) break;
	} break;

	case NETMSGTYPE_SV_TEAM:
	{
		CNetMsg_Sv_Team *pMsg = (CNetMsg_Sv_Team *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Team = pUnpacker->GetInt();
		pMsg->m_Silent = pUnpacker->GetInt();
		pMsg->m_CooldownTick = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, -1, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Team", pMsg->m_Team, TEAM_SPECTATORS, TEAM_BLUE)) break;
		if(!CheckInt("m_Silent", pMsg->m_Silent, 0, 1)) break;
		if(!CheckInt("m_CooldownTick", pMsg->m_CooldownTick, 0, max_int)) break;
	} break;

	case NETMSGTYPE_SV_KILLMSG:
	{
		CNetMsg_Sv_KillMsg *pMsg = (CNetMsg_Sv_KillMsg *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Killer = pUnpacker->GetInt();
		pMsg->m_Victim = pUnpacker->GetInt();
		pMsg->m_Weapon = pUnpacker->GetInt();
		pMsg->m_ModeSpecial = pUnpacker->GetInt();
		if(!CheckInt("m_Killer", pMsg->m_Killer, -2, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Victim", pMsg->m_Victim, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Weapon", pMsg->m_Weapon, -3, NUM_WEAPONS-1)) break;
	} break;

	case NETMSGTYPE_SV_TUNEPARAMS:
	{
		CNetMsg_Sv_TuneParams *pMsg = (CNetMsg_Sv_TuneParams *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_EXTRAPROJECTILE:
	{
		CNetMsg_Sv_ExtraProjectile *pMsg = (CNetMsg_Sv_ExtraProjectile *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_READYTOENTER:
	{
		CNetMsg_Sv_ReadyToEnter *pMsg = (CNetMsg_Sv_ReadyToEnter *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_WEAPONPICKUP:
	{
		CNetMsg_Sv_WeaponPickup *pMsg = (CNetMsg_Sv_WeaponPickup *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Weapon = pUnpacker->GetInt();
		if(!CheckInt("m_Weapon", pMsg->m_Weapon, 0, NUM_WEAPONS-1)) break;
	} break;

	case NETMSGTYPE_SV_EMOTICON:
	{
		CNetMsg_Sv_Emoticon *pMsg = (CNetMsg_Sv_Emoticon *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Emoticon = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Emoticon", pMsg->m_Emoticon, 0, 15)) break;
	} break;

	case NETMSGTYPE_SV_VOTECLEAROPTIONS:
	{
		CNetMsg_Sv_VoteClearOptions *pMsg = (CNetMsg_Sv_VoteClearOptions *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_VOTEOPTIONLISTADD:
	{
		CNetMsg_Sv_VoteOptionListAdd *pMsg = (CNetMsg_Sv_VoteOptionListAdd *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_VOTEOPTIONADD:
	{
		CNetMsg_Sv_VoteOptionAdd *pMsg = (CNetMsg_Sv_VoteOptionAdd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_VOTEOPTIONREMOVE:
	{
		CNetMsg_Sv_VoteOptionRemove *pMsg = (CNetMsg_Sv_VoteOptionRemove *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_VOTESET:
	{
		CNetMsg_Sv_VoteSet *pMsg = (CNetMsg_Sv_VoteSet *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Type = pUnpacker->GetInt();
		pMsg->m_Timeout = pUnpacker->GetInt();
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pReason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, -1, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Type", pMsg->m_Type, 0, 6)) break;
		if(!CheckInt("m_Timeout", pMsg->m_Timeout, 0, 60)) break;
	} break;

	case NETMSGTYPE_SV_VOTESTATUS:
	{
		CNetMsg_Sv_VoteStatus *pMsg = (CNetMsg_Sv_VoteStatus *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Yes = pUnpacker->GetInt();
		pMsg->m_No = pUnpacker->GetInt();
		pMsg->m_Pass = pUnpacker->GetInt();
		pMsg->m_Total = pUnpacker->GetInt();
		if(!CheckInt("m_Yes", pMsg->m_Yes, 0, MAX_CLIENTS)) break;
		if(!CheckInt("m_No", pMsg->m_No, 0, MAX_CLIENTS)) break;
		if(!CheckInt("m_Pass", pMsg->m_Pass, 0, MAX_CLIENTS)) break;
		if(!CheckInt("m_Total", pMsg->m_Total, 0, MAX_CLIENTS)) break;
	} break;

	case NETMSGTYPE_SV_SERVERSETTINGS:
	{
		CNetMsg_Sv_ServerSettings *pMsg = (CNetMsg_Sv_ServerSettings *)m_aMsgData;
		(void)pMsg;
		pMsg->m_KickVote = pUnpacker->GetInt();
		pMsg->m_KickMin = pUnpacker->GetInt();
		pMsg->m_SpecVote = pUnpacker->GetInt();
		pMsg->m_TeamLock = pUnpacker->GetInt();
		pMsg->m_TeamBalance = pUnpacker->GetInt();
		pMsg->m_PlayerSlots = pUnpacker->GetInt();
		if(!CheckInt("m_KickVote", pMsg->m_KickVote, 0, 1)) break;
		if(!CheckInt("m_KickMin", pMsg->m_KickMin, 0, MAX_CLIENTS)) break;
		if(!CheckInt("m_SpecVote", pMsg->m_SpecVote, 0, 1)) break;
		if(!CheckInt("m_TeamLock", pMsg->m_TeamLock, 0, 1)) break;
		if(!CheckInt("m_TeamBalance", pMsg->m_TeamBalance, 0, 1)) break;
		if(!CheckInt("m_PlayerSlots", pMsg->m_PlayerSlots, 0, MAX_CLIENTS)) break;
	} break;

	case NETMSGTYPE_SV_CLIENTINFO:
	{
		CNetMsg_Sv_ClientInfo *pMsg = (CNetMsg_Sv_ClientInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Local = pUnpacker->GetInt();
		pMsg->m_Team = pUnpacker->GetInt();
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pClan = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Country = pUnpacker->GetInt();
		pMsg->m_apSkinPartNames[0] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[1] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[2] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[3] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[4] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[5] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_aUseCustomColors[0] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[1] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[2] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[3] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[4] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[5] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[0] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[1] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[2] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[3] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[4] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[5] = pUnpacker->GetInt();
		pMsg->m_Silent = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Local", pMsg->m_Local, 0, 1)) break;
		if(!CheckInt("m_Team", pMsg->m_Team, TEAM_SPECTATORS, TEAM_BLUE)) break;
		if(!CheckInt("m_aUseCustomColors[0]", pMsg->m_aUseCustomColors[0], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[1]", pMsg->m_aUseCustomColors[1], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[2]", pMsg->m_aUseCustomColors[2], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[3]", pMsg->m_aUseCustomColors[3], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[4]", pMsg->m_aUseCustomColors[4], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[5]", pMsg->m_aUseCustomColors[5], 0, 1)) break;
		if(!CheckInt("m_Silent", pMsg->m_Silent, 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_GAMEINFO:
	{
		CNetMsg_Sv_GameInfo *pMsg = (CNetMsg_Sv_GameInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_GameFlags = pUnpacker->GetInt();
		pMsg->m_ScoreLimit = pUnpacker->GetInt();
		pMsg->m_TimeLimit = pUnpacker->GetInt();
		pMsg->m_MatchNum = pUnpacker->GetInt();
		pMsg->m_MatchCurrent = pUnpacker->GetInt();
		if(!CheckFlag("m_GameFlags", pMsg->m_GameFlags, GAMEFLAG_TEAMS|GAMEFLAG_FLAGS|GAMEFLAG_SURVIVAL|GAMEFLAG_RACE)) break;
		if(!CheckInt("m_ScoreLimit", pMsg->m_ScoreLimit, 0, max_int)) break;
		if(!CheckInt("m_TimeLimit", pMsg->m_TimeLimit, 0, max_int)) break;
		if(!CheckInt("m_MatchNum", pMsg->m_MatchNum, 0, max_int)) break;
		if(!CheckInt("m_MatchCurrent", pMsg->m_MatchCurrent, 0, max_int)) break;
	} break;

	case NETMSGTYPE_SV_CLIENTDROP:
	{
		CNetMsg_Sv_ClientDrop *pMsg = (CNetMsg_Sv_ClientDrop *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_pReason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Silent = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Silent", pMsg->m_Silent, 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_GAMEMSG:
	{
		CNetMsg_Sv_GameMsg *pMsg = (CNetMsg_Sv_GameMsg *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_DE_CLIENTENTER:
	{
		CNetMsg_De_ClientEnter *pMsg = (CNetMsg_De_ClientEnter *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Team = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, -1, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Team", pMsg->m_Team, TEAM_SPECTATORS, TEAM_BLUE)) break;
	} break;

	case NETMSGTYPE_DE_CLIENTLEAVE:
	{
		CNetMsg_De_ClientLeave *pMsg = (CNetMsg_De_ClientLeave *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_pReason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, -1, MAX_CLIENTS-1)) break;
	} break;

	case NETMSGTYPE_CL_SAY:
	{
		CNetMsg_Cl_Say *pMsg = (CNetMsg_Cl_Say *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Mode = pUnpacker->GetInt();
		pMsg->m_Target = pUnpacker->GetInt();
		pMsg->m_pMessage = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(!CheckInt("m_Mode", pMsg->m_Mode, 0, NUM_CHATS-1)) break;
		if(!CheckInt("m_Target", pMsg->m_Target, -1, MAX_CLIENTS-1)) break;
	} break;

	case NETMSGTYPE_CL_SETTEAM:
	{
		CNetMsg_Cl_SetTeam *pMsg = (CNetMsg_Cl_SetTeam *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Team = pUnpacker->GetInt();
		if(!CheckInt("m_Team", pMsg->m_Team, TEAM_SPECTATORS, TEAM_BLUE)) break;
	} break;

	case NETMSGTYPE_CL_SETSPECTATORMODE:
	{
		CNetMsg_Cl_SetSpectatorMode *pMsg = (CNetMsg_Cl_SetSpectatorMode *)m_aMsgData;
		(void)pMsg;
		pMsg->m_SpecMode = pUnpacker->GetInt();
		pMsg->m_SpectatorID = pUnpacker->GetInt();
		if(!CheckInt("m_SpecMode", pMsg->m_SpecMode, 0, NUM_SPECMODES-1)) break;
		if(!CheckInt("m_SpectatorID", pMsg->m_SpectatorID, -1, MAX_CLIENTS-1)) break;
	} break;

	case NETMSGTYPE_CL_STARTINFO:
	{
		CNetMsg_Cl_StartInfo *pMsg = (CNetMsg_Cl_StartInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pClan = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Country = pUnpacker->GetInt();
		pMsg->m_apSkinPartNames[0] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[1] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[2] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[3] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[4] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[5] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_aUseCustomColors[0] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[1] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[2] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[3] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[4] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[5] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[0] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[1] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[2] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[3] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[4] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[5] = pUnpacker->GetInt();
		if(!CheckInt("m_aUseCustomColors[0]", pMsg->m_aUseCustomColors[0], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[1]", pMsg->m_aUseCustomColors[1], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[2]", pMsg->m_aUseCustomColors[2], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[3]", pMsg->m_aUseCustomColors[3], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[4]", pMsg->m_aUseCustomColors[4], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[5]", pMsg->m_aUseCustomColors[5], 0, 1)) break;
	} break;

	case NETMSGTYPE_CL_KILL:
	{
		CNetMsg_Cl_Kill *pMsg = (CNetMsg_Cl_Kill *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_CL_READYCHANGE:
	{
		CNetMsg_Cl_ReadyChange *pMsg = (CNetMsg_Cl_ReadyChange *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_CL_EMOTICON:
	{
		CNetMsg_Cl_Emoticon *pMsg = (CNetMsg_Cl_Emoticon *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Emoticon = pUnpacker->GetInt();
		if(!CheckInt("m_Emoticon", pMsg->m_Emoticon, 0, 15)) break;
	} break;

	case NETMSGTYPE_CL_VOTE:
	{
		CNetMsg_Cl_Vote *pMsg = (CNetMsg_Cl_Vote *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Vote = pUnpacker->GetInt();
		if(!CheckInt("m_Vote", pMsg->m_Vote, -1, 1)) break;
	} break;

	case NETMSGTYPE_CL_CALLVOTE:
	{
		CNetMsg_Cl_CallVote *pMsg = (CNetMsg_Cl_CallVote *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Type = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Value = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Reason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Force = pUnpacker->GetInt();
		if(!CheckInt("m_Force", pMsg->m_Force, 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_SKINCHANGE:
	{
		CNetMsg_Sv_SkinChange *pMsg = (CNetMsg_Sv_SkinChange *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_apSkinPartNames[0] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[1] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[2] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[3] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[4] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[5] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_aUseCustomColors[0] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[1] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[2] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[3] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[4] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[5] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[0] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[1] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[2] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[3] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[4] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[5] = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_aUseCustomColors[0]", pMsg->m_aUseCustomColors[0], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[1]", pMsg->m_aUseCustomColors[1], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[2]", pMsg->m_aUseCustomColors[2], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[3]", pMsg->m_aUseCustomColors[3], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[4]", pMsg->m_aUseCustomColors[4], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[5]", pMsg->m_aUseCustomColors[5], 0, 1)) break;
	} break;

	case NETMSGTYPE_CL_SKINCHANGE:
	{
		CNetMsg_Cl_SkinChange *pMsg = (CNetMsg_Cl_SkinChange *)m_aMsgData;
		(void)pMsg;
		pMsg->m_apSkinPartNames[0] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[1] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[2] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[3] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[4] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_apSkinPartNames[5] = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_aUseCustomColors[0] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[1] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[2] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[3] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[4] = pUnpacker->GetInt();
		pMsg->m_aUseCustomColors[5] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[0] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[1] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[2] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[3] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[4] = pUnpacker->GetInt();
		pMsg->m_aSkinPartColors[5] = pUnpacker->GetInt();
		if(!CheckInt("m_aUseCustomColors[0]", pMsg->m_aUseCustomColors[0], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[1]", pMsg->m_aUseCustomColors[1], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[2]", pMsg->m_aUseCustomColors[2], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[3]", pMsg->m_aUseCustomColors[3], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[4]", pMsg->m_aUseCustomColors[4], 0, 1)) break;
		if(!CheckInt("m_aUseCustomColors[5]", pMsg->m_aUseCustomColors[5], 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_RACEFINISH:
	{
		CNetMsg_Sv_RaceFinish *pMsg = (CNetMsg_Sv_RaceFinish *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Time = pUnpacker->GetInt();
		pMsg->m_Diff = pUnpacker->GetInt();
		pMsg->m_RecordPersonal = pUnpacker->GetInt();
		pMsg->m_RecordServer = pUnpacker->GetIntOrDefault(0);
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_Time", pMsg->m_Time, -1, max_int)) break;
		if(!CheckInt("m_RecordPersonal", pMsg->m_RecordPersonal, 0, 1)) break;
		if(!CheckInt("m_RecordServer", pMsg->m_RecordServer, 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_CHECKPOINT:
	{
		CNetMsg_Sv_Checkpoint *pMsg = (CNetMsg_Sv_Checkpoint *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Diff = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_SV_COMMANDINFO:
	{
		CNetMsg_Sv_CommandInfo *pMsg = (CNetMsg_Sv_CommandInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Name = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_ArgsFormat = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_HelpText = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_COMMANDINFOREMOVE:
	{
		CNetMsg_Sv_CommandInfoRemove *pMsg = (CNetMsg_Sv_CommandInfoRemove *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Name = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_CL_COMMAND:
	{
		CNetMsg_Cl_Command *pMsg = (CNetMsg_Cl_Command *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Name = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Arguments = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_CL_ISMMOSERVER:
	{
		CNetMsg_Cl_IsMmoServer *pMsg = (CNetMsg_Cl_IsMmoServer *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Version = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_CL_CLIENTAUTH:
	{
		CNetMsg_Cl_ClientAuth *pMsg = (CNetMsg_Cl_ClientAuth *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Login = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Password = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_SelectRegister = pUnpacker->GetInt();
		if(!CheckInt("m_SelectRegister", pMsg->m_SelectRegister, 0, 1)) break;
	} break;

	case NETMSGTYPE_CL_TALKINTERACTIVE:
	{
		CNetMsg_Cl_TalkInteractive *pMsg = (CNetMsg_Cl_TalkInteractive *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_AFTERISMMOSERVER:
	{
		CNetMsg_Sv_AfterIsMmoServer *pMsg = (CNetMsg_Sv_AfterIsMmoServer *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_EQUIPITEMS:
	{
		CNetMsg_Sv_EquipItems *pMsg = (CNetMsg_Sv_EquipItems *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_EquipID[0] = pUnpacker->GetInt();
		pMsg->m_EquipID[1] = pUnpacker->GetInt();
		pMsg->m_EquipID[2] = pUnpacker->GetInt();
		pMsg->m_EquipID[3] = pUnpacker->GetInt();
		pMsg->m_EquipID[4] = pUnpacker->GetInt();
		pMsg->m_EquipID[5] = pUnpacker->GetInt();
		pMsg->m_EquipID[6] = pUnpacker->GetInt();
		pMsg->m_EquipID[7] = pUnpacker->GetInt();
		pMsg->m_EquipID[8] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[0] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[1] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[2] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[3] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[4] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[5] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[6] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[7] = pUnpacker->GetInt();
		pMsg->m_EnchantItem[8] = pUnpacker->GetInt();
		if(!CheckInt("m_ClientID", pMsg->m_ClientID, 0, MAX_CLIENTS-1)) break;
		if(!CheckInt("m_EnchantItem[0]", pMsg->m_EnchantItem[0], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[1]", pMsg->m_EnchantItem[1], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[2]", pMsg->m_EnchantItem[2], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[3]", pMsg->m_EnchantItem[3], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[4]", pMsg->m_EnchantItem[4], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[5]", pMsg->m_EnchantItem[5], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[6]", pMsg->m_EnchantItem[6], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[7]", pMsg->m_EnchantItem[7], 0, 1)) break;
		if(!CheckInt("m_EnchantItem[8]", pMsg->m_EnchantItem[8], 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_VOTEMMOOPTIONADD:
	{
		CNetMsg_Sv_VoteMmoOptionAdd *pMsg = (CNetMsg_Sv_VoteMmoOptionAdd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pHexColor = pUnpacker->GetInt();
		pMsg->m_pIcon[0] = pUnpacker->GetInt();
		pMsg->m_pIcon[1] = pUnpacker->GetInt();
		pMsg->m_pIcon[2] = pUnpacker->GetInt();
		pMsg->m_pIcon[3] = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_SV_CLIENTPROGRESSAUTH:
	{
		CNetMsg_Sv_ClientProgressAuth *pMsg = (CNetMsg_Sv_ClientProgressAuth *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Code = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_SV_TALKTEXT:
	{
		CNetMsg_Sv_TalkText *pMsg = (CNetMsg_Sv_TalkText *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pText = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_ConversationWithClientID = pUnpacker->GetInt();
		pMsg->m_Emote = pUnpacker->GetInt();
		pMsg->m_Flag = pUnpacker->GetInt();
		if(!CheckInt("m_Emote", pMsg->m_Emote, 0, 5)) break;
	} break;

	case NETMSGTYPE_SV_CLEARTALKTEXT:
	{
		CNetMsg_Sv_ClearTalkText *pMsg = (CNetMsg_Sv_ClearTalkText *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_ADDQUESTINGPROCESSING:
	{
		CNetMsg_Sv_AddQuestingProcessing *pMsg = (CNetMsg_Sv_AddQuestingProcessing *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pText = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pIcon[0] = pUnpacker->GetInt();
		pMsg->m_pIcon[1] = pUnpacker->GetInt();
		pMsg->m_pIcon[2] = pUnpacker->GetInt();
		pMsg->m_pIcon[3] = pUnpacker->GetInt();
		pMsg->m_pRequiresNum = pUnpacker->GetInt();
		pMsg->m_pHaveNum = pUnpacker->GetInt();
		pMsg->m_pGivingTable = pUnpacker->GetInt();
		if(!CheckInt("m_pGivingTable", pMsg->m_pGivingTable, 0, 1)) break;
	} break;

	case NETMSGTYPE_SV_CLEARQUESTINGPROCESSING:
	{
		CNetMsg_Sv_ClearQuestingProcessing *pMsg = (CNetMsg_Sv_ClearQuestingProcessing *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_PROGRESSBAR:
	{
		CNetMsg_Sv_ProgressBar *pMsg = (CNetMsg_Sv_ProgressBar *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pText = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pCount = pUnpacker->GetInt();
		pMsg->m_pRequires = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_SV_WORLDMUSIC:
	{
		CNetMsg_Sv_WorldMusic *pMsg = (CNetMsg_Sv_WorldMusic *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pSoundID = pUnpacker->GetInt();
		pMsg->m_pVolume = pUnpacker->GetInt();
		if(!CheckInt("m_pVolume", pMsg->m_pVolume, 1, 10)) break;
	} break;

	case NETMSGTYPE_SV_SENDGUIINFORMATIONBOX:
	{
		CNetMsg_Sv_SendGuiInformationBox *pMsg = (CNetMsg_Sv_SendGuiInformationBox *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMsg = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_CL_MAILLETTERACTIONS:
	{
		CNetMsg_Cl_MailLetterActions *pMsg = (CNetMsg_Cl_MailLetterActions *)m_aMsgData;
		(void)pMsg;
		pMsg->m_MailLetterID = pUnpacker->GetInt();
		pMsg->m_MailLetterFlags = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_SV_SENDMAILLETTERINFO:
	{
		CNetMsg_Sv_SendMailLetterInfo *pMsg = (CNetMsg_Sv_SendMailLetterInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_MailLetterID = pUnpacker->GetInt();
		pMsg->m_pTitle = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pFrom = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pMsg = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_IsRead = pUnpacker->GetInt();
		pMsg->m_pJsonAttachementItem = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(!CheckInt("m_IsRead", pMsg->m_IsRead, 0, 1)) break;
	} break;

	case NETMSGTYPE_CL_SENDMAILLETTERTO:
	{
		CNetMsg_Cl_SendMailLetterTo *pMsg = (CNetMsg_Cl_SendMailLetterTo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pTitle = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pMsg = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pPlayer = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_FromClientID = pUnpacker->GetInt();
	} break;

	case NETMSGTYPE_DIALOGCOMMON:
	{
		CNetMsg_DialogCommon *pMsg = (CNetMsg_DialogCommon *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pTitle = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pButton1 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pButton2 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_DIALOG_MSGBOX:
	{
		CNetMsg_Sv_Dialog_Msgbox *pMsg = (CNetMsg_Sv_Dialog_Msgbox *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMsg = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_DIALOG_INPUT:
	{
		CNetMsg_Sv_Dialog_Input *pMsg = (CNetMsg_Sv_Dialog_Input *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMsg = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	case NETMSGTYPE_SV_DIALOG_LIST:
	{
		CNetMsg_Sv_Dialog_List *pMsg = (CNetMsg_Sv_Dialog_List *)m_aMsgData;
		(void)pMsg;
	} break;

	case NETMSGTYPE_SV_DIALOG_LIST_ADDITEM:
	{
		CNetMsg_Sv_Dialog_List_AddItem *pMsg = (CNetMsg_Sv_Dialog_List_AddItem *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pTitle = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;

	default:
		m_pMsgFailedOn = "(type out of range)";
		break;
	}

	if(pUnpacker->Error())
		m_pMsgFailedOn = "(unpack error)";

	if(m_pMsgFailedOn || m_pObjFailedOn) {
		if(!m_pMsgFailedOn)
			m_pMsgFailedOn = "";
		if(!m_pObjFailedOn)
			m_pObjFailedOn = "";
		return 0;
	}
	m_pMsgFailedOn = "";
	m_pObjFailedOn = "";
	return m_aMsgData;
};

} // namespace protocol7
