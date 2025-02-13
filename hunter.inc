// Copyright © 2017 Vaqtincha
/*

■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ clcmds ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

server_cmd("endround") // end round
server_cmd("mp_freeforall 1") // kill anyone including team mates
erver_cmd("mp_round_infinite 0") // not infinite


// M/Change team
	register_clcmd("chooseteam", "clcmd_changeteam")
	register_clcmd("jointeam", "clcmd_changeteam")

public clcmd_changeteam(id) { // Block Team Change

	// either you can use this or
	static team; team = fm_cs_get_user_team(id)
	if(team == FM_CS_TEAM_SPECTATOR || team == FM_CS_TEAM_UNASSIGNED) return PLUGIN_CONTINUE; // Unless it's a spectator joining the game

	// alternatively you can use this too

	if (!is_user_connected(id)) // auto team join plugin will be needed
		return PLUGIN_CONTINUE

	show_menu_game(id) // Pressing 'M' (chooseteam) ingame should show the main menu instead
	return PLUGIN_HANDLED;
}

// G/Drop weapon
	register_clcmd("drop", "clcmd_drop")

public clcmd_drop(id) { // block damage
	if(is_user_alive(id)) return PLUGIN_HANDLED

	show_menu_game(id) // Pressing 'G' (drop) ingame should show the main menu instead
	return PLUGIN_CONTINUE;
}

// impulse bind

	register_impulse(100, "impulse_flashlight")

public impulse_flashlight()
{
    if(!is_user_alive(id)) return PLUGIN_CONTINUE; // remove check to not allow at all
    
    show_menu_game(id) // Pressing 'F' (flashlight) ingame should show the main menu instead
    return PLUGIN_HANDLED;
}

	register_impulse(201, "impulse_spray")

public impulse_spray()
{
    if(!is_user_alive(id)) return PLUGIN_CONTINUE; // remove check to not allow at all
    
    show_menu_game(id) // Pressing 'F' (flashlight) ingame should show the main menu instead
    return PLUGIN_HANDLED;
}

put calls in plugin_init and functions are like public xyz
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Forward call and functions - hamsandwich & others ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// spawn
RegisterHam(Ham_Spawn, "player", "fw_PlayerSpawn_Post", 1, true)

public fw_PlayerSpawn_Post(id) 
{

	if(!is_user_alive(id))
		return HAM_IGNORED;

return HAM_IGNORED;
}

// player killed

RegisterHam(Ham_Killed, "player", "fw_PlayerKilled", 0, true)

public fw_PlayerKilled(victim, attacker, shouldgib) {

	if (is_user_alive(iVictim))
		return

}

// player killed post

RegisterHam(Ham_Killed, "player", "fw_PlayerKilled_Post", 1, true)

public fw_PlayerKilled_Post(victim, attacker, shouldgib) {

	if(!is_user_valid_alive(victim))
		return;

}

// player take damage

RegisterHam(Ham_TakeDamage, "player", "fw_TakeDamage", 0, true)

public fw_TakeDamage(victim, inflictor, attacker, Float:damage, damage_type) { // Ham Take Damage Forward
	
	if(!is_user_valid_alive(victim))
		return HAM_IGNORED;
	if(!is_user_valid_connected(attacker) && pev_valid(attacker)) {
		return HAM_IGNORED;
	}

	if(is_user_alive(attacker))
	{
		SetHamParamFloat(4, damage * 2.5)
	}

	return HAM_SUPERCEDE;
}

// player take damage post

RegisterHam(Ham_TakeDamage, "player", "fw_TakeDamage_Post", 1, true)

public fw_TakeDamage_Post(victim, inflictor, attacker, Float:damage, damage_type) { // Ham Take Damage Post Forward
	if(!is_user_valid_alive(victim))
		return;

}

// trace attack

RegisterHam(Ham_TraceAttack, "player", "fw_TraceAttack", 0, true)

public fw_TraceAttack(victim, attacker, Float:damage, Float:direction[3], tracehandle, damage_type) { // Ham Trace Attack Forward
	if(victim == attacker || !is_user_valid_connected(attacker) || !is_user_valid_connected(victim)) return HAM_IGNORED; // Non-player damage or self damage

	if(g_newround)
		return HAM_SUPERCEDE;

	return HAM_IGNORED;
}

// client kill/suicide - registering forwards

register_forward(FM_ClientKill, "fw_ClientKill")

public fw_ClientKill() { // Client Kill/Suicide Forward
	if(get_pcvar_num(cvar_blocksuicide)) return FMRES_SUPERCEDE; // Prevent players from killing themselves?

	return FMRES_IGNORED;
}


// client disconnected
register_forward(FM_ClientDisconnect, "fw_ClientDisconnect")

public fw_ClientDisconnect(id) { // Client leaving

	// examples
	save_stats(id)
	remove_task(id+TASK_BLOOD)
	remove_task(id+TASK_AURA)

	g_isconnected[id] = false
	g_isalive[id] = false
}


// client disconnected post
register_forward(FM_ClientDisconnect, "fw_ClientDisconnect_Post", 1)

public fw_ClientDisconnect_Post() {
	fnCheckSomeoneElseForHisRole(); // example if there is any function to replace the leaver
}

fnCheckSomeoneElseForHisRole() { // Last T Check -check for last t and set its flag-
	static id, NumT, NumCt
	NumT = fnGetT();
	NumCt = fnGetCt();
	for(id = 1; id <= MaxClients; id++) {
		if(!is_user_alive[id])
			continue;

		if(NumT == 1) { // Last t
			if(!g_lastt[id]) {
				fm_set_user_health(id, fm_get_user_health(id) + 100) // Reward extra hp
			}
			g_lastt[id] = true
		}
		else g_lastt[id] = false

		if(NumCt == 1) { // Last ct
			if(!g_lastct[id]) {
				fm_set_user_health(id, fm_get_user_health(id) + 100) // Reward extra hp
			}
			g_lastct[id] = true
		}
		else g_lastct[id] = false
	}
}

fnGetT() { // Get T -returns alive t number-
	static iT, id; iT = 0
	for(id = 1; id <= MaxClients; id++) if(is_user_alive[id] && g_t[id]) iT++
	return iT;
}
fnGetCt() { // Get Ct -returns alive ct number-
	static iCt, id; iCt = 0
	for(id = 1; id <= MaxClients; id++) if(g_isalive[id] && !g_zombie[id]) iCts++

	return iCt;
}

// client put in the server - events

public client_putinserver(id) { // Client joins the game
	
}

// event round start

register_event("HLTV", "event_round_start", "a", "1=0", "2=0")

public event_round_start() { // Event Round Start

	g_freezetime = true // Freezetime begins

	for(new id = 1; i <= MaxClients; id++)
		if(is_user_alive(id))
		{
			fm_set_user_health(id, fm_get_user_health(id) + 100)
		}
}

// event round start once freeze time ends

register_logevent("logevent_round_start",2, "1=Round_Start")

public logevent_round_start() {
	g_freezetime = false // Freezetime ends
}

// event round end

register_logevent("logevent_round_end", 2, "1=Round_End")

public logevent_round_end() {
	for(id = 1; id <= MaxClients; id++)
	{
		save_stats(id)
	}
	remove_task(TASK_WELCOMEMSG)
}

// event map ended (maybe changing)

register_event("30", "event_intermission", "a")

public event_intermission()
{
	remove_task(TASK_AMBIENCESOUNDS); // Remove ambience sounds task
}

// fakemta teams

enum { // CS Teams
	FM_CS_TEAM_UNASSIGNED = 0,
	FM_CS_TEAM_T,
	FM_CS_TEAM_CT,
	FM_CS_TEAM_SPECTATOR
};

	if(fm_cs_get_user_team(id) != FM_CS_TEAM_CT || fm_cs_get_user_team(id) == FM_CS_TEAM_CT) { // if not ct make ct
		fm_cs_set_user_team(id, FM_CS_TEAM_CT)
		fm_cs_get_user_team(id) = FM_CS_TEAM_CT
	}


■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Forward call and functions - reapi ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// spawn post
RegisterHookChain(RG_CBasePlayer_Spawn, "fw_PlayerSpawn_Post", 1)

public fw_PlayerSpawn_Post(const id)
{
	if (!is_user_alive(id))
		return

}

// player killed post
RegisterHookChain(RG_CBasePlayer_Killed, "fw_PlayerKilled_Post", 1)

public fw_PlayerKilled_Post(const iVictim, const iAttacker, const iGibs)
{

	if (is_user_alive(iVictim))
		return

}

// player take damage post

RegisterHookChain(RG_CBasePlayer_TakeDamage, "fw_TakeDamage_Post", 1)

public fw_TakeDamage_Post(const iVictim, const iInflector, const iAttacker, const Float:flDamage, const bitsDamageType)
{
	// Damage himself?
	if (iVictim == iAttacker || !is_user_connected(iVictim))
		return
	if(is_user_alive(iAttacker))
	{
		SetHookChainArg(4, ATYPE_FLOAT, flDamage * 2.5)
	}
}

// trace attack pre

RegisterHookChain(RG_CBasePlayer_TraceAttack, "fw_TraceAttack_Pre", 0)

public fw_TraceAttack_Pre(const iVictim, const iAttacker, const Float:flDamage, const Float:vDirecton[3], const pTrace, bitsDamageType)
{
	// Damage himself or player not on game?
	if (iVictim == iAttacker || !is_user_connected(iVictim) || !is_user_connected(iAttacker))
		return HC_CONTINUE

	// Round Ended!
	if (g_bRoundEnd)
		return HC_SUPERCEDE

	// Teammate?
	if (flag_get_boolean(g_bitsIsZombie, iVictim) == flag_get_boolean(g_bitsIsZombie, iAttacker))
		return HC_CONTINUE

	return HC_CONTINUE
}

*/

#if defined _reapi_stocks_included
	#endinput
#endif

#define _reapi_stocks_included

#if !defined _reapi_included
	#include <reapi>
#endif

const GRENADES_BIT_SUM = ((1<<_:WEAPON_HEGRENADE)|(1<<_:WEAPON_SMOKEGRENADE)|(1<<_:WEAPON_FLASHBANG))
const NOCLIP_WEAPONS_BIT_SUM = (GRENADES_BIT_SUM|(1<<_:WEAPON_KNIFE)|(1<<_:WEAPON_C4))
const SECONDARY_WEAPONS_BIT_SUM = ((1<<_:WEAPON_P228)|(1<<_:WEAPON_ELITE)|(1<<_:WEAPON_FIVESEVEN)|(1<<_:WEAPON_USP)|(1<<_:WEAPON_GLOCK18)|(1<<_:WEAPON_DEAGLE))
const PRIMARY_WEAPONS_BIT_SUM = 
(
	(1<<_:WEAPON_SCOUT)|(1<<_:WEAPON_XM1014)|(1<<_:WEAPON_MAC10)|(1<<_:WEAPON_AUG)|(1<<_:WEAPON_UMP45)|(1<<_:WEAPON_SG550)
	|(1<<_:WEAPON_GALIL)|(1<<_:WEAPON_FAMAS)|(1<<_:WEAPON_AWP)|(1<<_:WEAPON_MP5N)|(1<<_:WEAPON_M249)|(1<<_:WEAPON_M3)
	|(1<<_:WEAPON_M4A1)|(1<<_:WEAPON_TMP)|(1<<_:WEAPON_G3SG1)|(1<<_:WEAPON_SG552)|(1<<_:WEAPON_AK47)|(1<<_:WEAPON_P90)
)


enum GiveAmmoType { GAT_GIVE_AMMO, GAT_SET_AMMO }

enum AmmoTypes
{
	AMMO_NONE,
	AMMO_338MAGNUM,
	AMMO_762NATO,
	AMMO_556NATOBOX,
	AMMO_556NATO,
	AMMO_BUCKSHOT,
	AMMO_45ACP,
	AMMO_57MM,
	AMMO_50AE,
	AMMO_357SIG,
	AMMO_9MM,
	AMMO_FLASHBANG,
	AMMO_HEGRENADE,
	AMMO_SMOKEGRENADE,
	AMMO_C4,

	AMMO_MAX_TYPES
}

enum /* RewardAccount */ 		// from gamerules.h
{
	REWARD_TARGET_BOMB			= 3500,
	REWARD_VIP_ESCAPED			= 3500,
	REWARD_VIP_ASSASSINATED		= 3250,
	REWARD_TERRORISTS_ESCAPED	= 3150,
	REWARD_CTS_PREVENT_ESCAPE	= 3500,
	REWARD_ESCAPING_TT_NEUTRALIZED	= 3250,
	REWARD_BOMB_DEFUSED			= 3250,
	REWARD_BOMB_PLANTED			= 800,
	REWARD_BOMB_EXPLODED		= 3250,
	REWARD_CTS_WIN				= 3000,
	REWARD_TERRORISTS_WIN		= 3000,
	REWARD_ALL_HOSTAGES_RESCUED	= 2500,
	REWARD_TARGET_BOMB_SAVED	= 3250,
	REWARD_HOSTAGE_NOT_RESCUED	= 3250,
	REWARD_VIP_NOT_ESCAPED		= 3250,
	REWARD_LOSER_BONUS_DEFAULT	= 1400,
	REWARD_LOSER_BONUS_MIN		= 1500,
	REWARD_LOSER_BONUS_MAX		= 3000,
	REWARD_LOSER_BONUS_ADD		= 500,
	REWARD_RESCUED_HOSTAGE		= 750,
	// REWARD_KILLED_ENEMY			= 300,
	// REWARD_KILLED_VIP			= 2500,
	// REWARD_VIP_HAVE_SELF_RESCUED = 2500,
	// REWARD_TAKEN_HOSTAGE			= 1000,
	REWARD_TOOK_HOSTAGE_ACC		= 100,
	REWARD_TOOK_HOSTAGE			= 150,
}

native csd_setuserac(id);

#define rg_is_user_steamplus(%1) csd_setuserac(%1)

#define IsValidWeaponID(%1) 		(WEAPON_P228 <= %1 <= WEAPON_P90 && %1 != WEAPON_GLOCK)
#define IsPrimaryWeaponID(%1) 		(PRIMARY_WEAPONS_BIT_SUM & (1 << any:%1))
#define IsSecondaryWeaponID(%1) 	(SECONDARY_WEAPONS_BIT_SUM & (1 << any:%1))
#define IsGrenadeWeaponID(%1) 		(GRENADES_BIT_SUM & (1 << any:%1))
#define IsNoClipWeaponID(%1) 		(NOCLIP_WEAPONS_BIT_SUM & (1 << any:%1))

#define IsValidSlotType(%1) 		(PRIMARY_WEAPON_SLOT <= %1 <= C4_SLOT)
#define IsValidArmoury(%1) 			(ARMOURY_MP5NAVY <= %1 <= ARMOURY_DEAGLE)

#define IsValidRuleID(%1) 			(RR_CTS_WIN <= %1 < RR_END)
#define IsValidAmmoID(%1) 			(AMMO_338MAGNUM <= %1 < AMMO_MAX_TYPES)
#define IsValidTeam(%1) 			(TEAM_TERRORIST <= get_member(%1, m_iTeam) <= TEAM_CT)


/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Other stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


// by s1lent
stock rg_map_buy_status(bool:enabled)
{
	new pBuyzone = NULLENT, count
	while((pBuyzone = rg_find_ent_by_class(pBuyzone, "func_buyzone", .useHashTable = true)))
	{
		set_entvar(pBuyzone, var_solid, enabled ? SOLID_TRIGGER : SOLID_NOT)
		count++
	}

	// old
	if(!count) 
	{
		set_member_game(m_bCTCantBuy, enabled)
		set_member_game(m_bTCantBuy, enabled)
	}
}


// change roundtime
stock rg_set_round_time(const newTime) 
{
	new iRoundTime = clamp(newTime, 0, 999)

	set_member_game(m_fRoundStartTime, get_gametime())
	set_member_game(m_iRoundTimeSecs, iRoundTime)

	static gmsgShowTimer, gmsgRoundTime
	if(gmsgShowTimer || (gmsgShowTimer = get_user_msgid("ShowTimer")))
	{
		message_begin(MSG_ALL, gmsgShowTimer)
		message_end()
	}
	
	if(gmsgRoundTime || (gmsgRoundTime = get_user_msgid("RoundTime")))
	{
		message_begin(MSG_ALL, gmsgRoundTime)
		write_short(iRoundTime)
		message_end()
	}
}

stock bool:rg_freezetime_ended(){
	return !get_member_game(m_bFreezePeriod)
}

stock bool:rg_map_has_bombtarget(){
	return get_member_game(m_bMapHasBombTarget)
}

stock bool:rg_buytime_expired() 
{
	static mp_buytime
	if(mp_buytime || (mp_buytime = get_cvar_pointer("mp_buytime")))
	{
		new Float:flBuyTime = get_pcvar_float(mp_buytime)
		if(flBuyTime == -1.0) {  		// no limit
			return false
		}
		if(flBuyTime == 0.0 || (get_gametime() - Float:get_member_game(m_fRoundStartTime) > (flBuyTime * 60))) {
			return true
		}
	}

	return false
}

stock rg_set_round_winner(const WinStatus:st, const iAddPoints = 1)
{
	new ScenarioEventEndRound:Event, ctScore, ttScore
	switch(st)
	{
		case WINSTATUS_CTS: {
			Event = ROUND_CTS_WIN
			ctScore = iAddPoints
		}
		case WINSTATUS_TERRORISTS: {
			Event = ROUND_TERRORISTS_WIN
			ttScore = iAddPoints
		}
		case WINSTATUS_DRAW: {
			Event = ROUND_NONE
		}
		default:return
	}

	rg_round_end(5.0, st, Event)

	if(Event != ROUND_NONE && (ctScore || ttScore)) {
		rg_update_teamscores(ctScore, ttScore)
	}
}

// RR_END  = reset all rules
stock rg_reset_account_rules(RewardRules:iRulesID = RR_END)
{
	static const rgRewardAccountRules[any:RR_END] = {
		REWARD_CTS_WIN,						// RR_CTS_WIN
		REWARD_TERRORISTS_WIN,				// RR_TERRORISTS_WIN
		REWARD_TARGET_BOMB,					// RR_TARGET_BOMB
		REWARD_VIP_ESCAPED,					// RR_VIP_ESCAPED
		REWARD_VIP_ASSASSINATED,			// RR_VIP_ASSASSINATED
		REWARD_TERRORISTS_ESCAPED,			// RR_TERRORISTS_ESCAPED
		REWARD_CTS_PREVENT_ESCAPE,			// RR_CTS_PREVENT_ESCAPE
		REWARD_ESCAPING_TT_NEUTRALIZED,		// RR_ESCAPING_TERRORISTS_NEUTRALIZED
		REWARD_BOMB_DEFUSED,				// RR_BOMB_DEFUSED
		REWARD_BOMB_PLANTED,				// RR_BOMB_PLANTED
		REWARD_BOMB_EXPLODED,				// RR_BOMB_EXPLODED
		REWARD_ALL_HOSTAGES_RESCUED,		// RR_ALL_HOSTAGES_RESCUED
		REWARD_TARGET_BOMB_SAVED,			// RR_TARGET_BOMB_SAVED
		REWARD_HOSTAGE_NOT_RESCUED,			// RR_HOSTAGE_NOT_RESCUED
		REWARD_VIP_NOT_ESCAPED,				// RR_VIP_NOT_ESCAPED
		REWARD_LOSER_BONUS_DEFAULT,			// RR_LOSER_BONUS_DEFAULT
		REWARD_LOSER_BONUS_MIN,				// RR_LOSER_BONUS_MIN
		REWARD_LOSER_BONUS_MAX,				// RR_LOSER_BONUS_MAX
		REWARD_LOSER_BONUS_ADD,				// RR_LOSER_BONUS_ADD
		REWARD_RESCUED_HOSTAGE,				// RR_RESCUED_HOSTAGE
		REWARD_TOOK_HOSTAGE_ACC,			// RR_TOOK_HOSTAGE_ACC
		REWARD_TOOK_HOSTAGE					// RR_TOOK_HOSTAGE
	}

	if(IsValidRuleID(iRulesID))
	{
		rg_set_account_rules(iRulesID, rgRewardAccountRules[iRulesID])
	}
	else if(iRulesID == RR_END)
	{
		for(iRulesID = RR_CTS_WIN; iRulesID < RR_END; iRulesID++)
		{
			rg_set_account_rules(iRulesID, rgRewardAccountRules[iRulesID])
		}
	}
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Player stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

stock rg_set_user_noclip(const player, bool:noclip) {
	set_entvar(player, var_movetype, noclip ? MOVETYPE_NOCLIP : MOVETYPE_WALK)
}

/* 
*/
stock rg_set_user_godmode(const player, bool:godmode) {
	set_entvar(player, var_takedamage, godmode ? DAMAGE_NO : DAMAGE_AIM);
}

stock Float:rg_get_user_maxspeed(const player) {
	return Float:get_entvar(player, var_maxspeed)
}
/* Sets player max. speed. 
*/
stock rg_set_user_maxspeed(const player, Float:speed = -1.0)
{
	if(speed != -1) // reset
		set_entvar(player, var_maxspeed, speed)
	else
		rg_reset_maxspeed(player)		
}

/* Sets player gravity.
*/
stock rg_set_user_gravity(const player, Float:gravity = 1.0) {
	set_entvar(player, var_gravity, gravity)
}

/* Returns in bitwise (see enum SignalState) form if the user is in a specific map zone.
*/
stock SignalState:rg_get_user_mapzones(const player)
{
	new iSignals[UnifiedSignals]
	get_member(player, m_signals, iSignals)

	return SignalState:iSignals[US_State]	
}

stock bool:rg_user_in_buyzone(const player) {
	return bool:(rg_get_user_mapzones(player) & SIGNAL_BUY)
}

stock bool:rg_user_in_bombzone(const player) {
	return bool:((rg_get_user_mapzones(player) & SIGNAL_BOMB) == SIGNAL_BOMB)
}

stock bool:rg_user_in_water(const player) {
	return bool:(get_entvar(player, var_waterlevel) > 0)
}

stock bool:rg_user_on_ladder(const player) {
	return bool:(get_entvar(player, var_movetype) == MOVETYPE_FLY)
}

stock bool:rg_get_user_invisibility(const player) {
	return bool:(get_entvar(player, var_effects) & EF_NODRAW))
}

stock rg_set_user_invisibility(const player, bool:hide = true) {
	new iFlags = get_entvar(player, var_effects)
	set_entvar(player, var_effects, hide ? (iFlags |= EF_NODRAW) : (iFlags &= ~EF_NODRAW))
}

stock rg_get_user_money(const player) {
	return get_member(player, m_iAccount)
}

stock rg_get_user_next_spraytime(const player, Float:time) {
	set_member(player, m_flNextDecalTime, get_gametime() + time)
}

stock rg_set_user_sprayframe(const player, frames = -1) {
	set_member(player, m_nCustomSprayFrames, frames)
}

// Adidasman
stock rg_skip_user_vguimenu(const player) {
	set_member(player, m_bForceShowMenu, true)
}

stock rg_get_user_deaths(const player) {
	return get_member(player, m_iDeaths)
}

stock bool:rg_is_user_joined(const player) {
	return bool:(get_member(player, m_iJoiningState) == JOINED)
}

/* Returns true user has blinded.
*/
stock bool:rg_is_user_blinded(const player) {
	return bool:(Float:get_member(player, m_blindStartTime) + Float:get_member(player, m_blindFadeTime) >= get_gametime())
}

// original code by ConnorMcLeod 2011 (see: https://forums.alliedmods.net/showpost.php?p=1612876&postcount=2)

stock rg_get_user_blind_percent(const player)
{
	static const BLINDED_FULLY = 255
	#define fPercent(%1,%2) 	100.0 - (((%1) * 100.0) / %2)

	new Float:flCurTime = get_gametime(),
		Float:flBlindStartTime = Float:get_member(player, m_blindStartTime),
		Float:flBlindHoldTime = Float:get_member(player, m_blindHoldTime),
		Float:flBlindFadeTime = Float:get_member(player, m_blindFadeTime),
		Float:flEndTime = flBlindFadeTime + flBlindHoldTime

	if((flCurTime - flBlindStartTime) > flEndTime)
		return 0

	if(get_member(player, m_blindAlpha) == BLINDED_FULLY)
	{
		if((flBlindStartTime + flBlindHoldTime) >= flCurTime)
			return 100 		// 100 percent

		return floatround(fPercent(flCurTime - (flBlindStartTime + flBlindHoldTime), flBlindFadeTime))
	}
	
	return floatround(fPercent(flCurTime - flBlindStartTime, flEndTime))
}

/* return enum ModelName
*/
stock ModelName:rg_get_user_class(const player) {
	return get_member(player, m_iModelName)
}

stock TeamName:rg_get_user_team(const player, &{ModelName,_}:model = MODEL_UNASSIGNED) 
{
	model = rg_get_user_class(player)
	return TeamName:get_member(player, m_iTeam)
}


/* Does user have night vision goggles?
 */
stock rg_get_user_nvg(const player) {
	return get_member(player, m_bHasNightVision)
}

/* Set nvgoggles to 1 to give night vision goggles to index. Set it to 0 to remove them.
 */
stock rg_set_user_nvg(const player, bool:nvgoggles = true) {
	set_member(player, m_bHasNightVision, nvgoggles)
}

stock bool:rg_user_has_defuser(const player) {
	return bool:(get_member(player, m_bHasDefuser))
}

/* return enum HitBoxGroup 
*/
stock HitBoxGroup:rg_get_user_last_hitgroup(const player) {
	return get_member(player, m_LastHitGroup)
}

stock bool:rg_user_killed_by_headshot(const player) {
	return bool:(rg_get_user_last_hitgroup(player) == HITGROUP_HEAD || get_member(player, m_bHeadshotKilled))
}

/* Returns true if user has a shield, else false.
*/
stock bool:rg_user_has_shield(const player, &bool:bShieldDrawn = false)
{
	if(get_member(player, m_bOwnsShield)) 
	{
		bShieldDrawn = get_member(player, m_bShieldDrawn)
		return true
	}

	return false
}

stock bool:rg_user_has_secondary(const player) {
	return bool:(get_member(player, m_rgpPlayerItems, PISTOL_SLOT) > 0)
}

/* Returns 1 when user has a primary weapon OR a shield in inventory, else 0.
*/
stock bool:rg_user_has_primary(const player) {
	return bool:((get_member(player, m_rgpPlayerItems, PRIMARY_WEAPON_SLOT) > 0) || get_member(player, m_bHasPrimary))
}

stock rg_set_user_can_shot(const player, bool:can = true) {
	set_member(player, m_bIsDefusing, can)
}

stock rg_set_user_team_changed(const player, bool:changed = true) {
	set_member(player, m_bTeamChanged, changed)
}

stock rg_set_user_radio_message(const player, num) {
	set_member(player, m_iRadioMessages, num)
}

stock rg_set_user_ignore_radio(const player, bool:ignore = true) {
	set_member(player, m_bIgnoreRadio, !ignore)
}

stock WeaponIdType:rg_get_user_weapon(const player, InventorySlotType:iSlot, &pWeapon = NULLENT)
{
	if(!IsValidSlotType(iSlot))
		return WEAPON_NONE

	return ((pWeapon = get_member(player, m_rgpPlayerItems, iSlot)) > 0) ? rg_get_weapon_id(pWeapon) : WEAPON_NONE
}

stock WeaponIdType:rg_get_user_active_weapon(const player, &pWeapon = NULLENT) {
	return ((pWeapon = get_member(player, m_pActiveItem)) > 0) ? rg_get_weapon_id(pWeapon) : WEAPON_NONE
}

// Max = 0   default max (works with GAT_GIVE_AMMO)
stock rg_user_add_ammo(const player, AmmoTypes:AmmoId, Amount, Max = 0, GiveAmmoType:type = GAT_SET_AMMO, bool:bShowPickup = true)
{
	if(!IsValidAmmoID(AmmoId))
		return

	static const iMaxBpAmmo[AMMO_MAX_TYPES] = {
		0, 30, 90, 200, 90, 32, 100, 100, 35, 52, 120, 2, 1, 1, 1
	}
	
	new finalAmount, userAmmo, finalMax = (Max > 0 ? Max : iMaxBpAmmo[AmmoId])
	
	if(type == GAT_SET_AMMO)
	{
		set_member(player, m_rgAmmo, Amount, AmmoId)
		return
	}
	else if(type == GAT_GIVE_AMMO)
	{
		if(Amount <= 0 || ((userAmmo = get_member(player, m_rgAmmo, AmmoId)) >= finalMax))
			return

		set_member(player, m_rgAmmo, userAmmo + (finalAmount = min(finalMax, Amount)), AmmoId)
	}

	static gmsgAmmoPickup
	if(finalAmount && bShowPickup && (gmsgAmmoPickup || (gmsgAmmoPickup = get_user_msgid("AmmoPickup"))))
	{
		message_begin(MSG_ONE, gmsgAmmoPickup, .player = player)
		write_byte(any:AmmoId)
		write_byte(finalAmount)
		message_end()
	}
}

stock rg_send_weapon_anim(const player, const anim, const Float:fAnimTime, weapon = NULLENT) 
{
	if(weapon > 0 || (weapon = get_member(player, m_pActiveItem)) > 0)
	{
		set_entvar(player, var_weaponanim, anim)

		message_begin(MSG_ONE, SVC_WEAPONANIM, .player = player)
		write_byte(anim)
		write_byte(get_entvar(weapon, var_body))
		message_end()

		set_member(weapon, m_Weapon_flTimeWeaponIdle, fAnimTime)
	}	
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ WeaponBox stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

stock rg_kill_weaponbox(const weaponbox)
{
	if(!get_member(weaponbox, m_WeaponBox_bIsBomb))
	{
		set_entvar(weaponbox, var_nextthink, get_gametime() + 0.1)
		return
	}
	
	new pWeapon = get_member(weaponbox, m_WeaponBox_rgpPlayerItems, C4_SLOT)
	if(!is_nullent(pWeapon)) {
		set_entvar(pWeapon, var_flags, FL_KILLME)
	}

	set_entvar(weaponbox, var_flags, FL_KILLME)

	static gmsgBombPickup
	if(gmsgBombPickup || (gmsgBombPickup = get_user_msgid("BombPickup")))
	{
		message_begin(MSG_ALL, gmsgBombPickup)
		message_end()
	}
}
	
// alternate native WeaponIdType:rg_get_weaponbox_id(const entity)

stock WeaponIdType:rg_get_weaponbox_weapon(const weaponbox, InventorySlotType:iSlot, &pWeapon = NULLENT)
{
	if(!IsValidSlotType(iSlot))
		return WEAPON_NONE

	return ((pWeapon = get_member(weaponbox, m_WeaponBox_rgpPlayerItems, iSlot)) > 0) ? rg_get_weapon_id(pWeapon) : WEAPON_NONE
}

stock rg_get_weaponbox_ammo(const weaponbox) {
	return get_member(weaponbox, m_WeaponBox_rgAmmo, IsGrenadeWeaponID(rg_get_weaponbox_id(weaponbox)) ? 1 : 0)
}

stock rg_set_weaponbox_ammo(const weaponbox, const AmmoTypes:AmmoId, const iCount)
{
	if(!IsValidAmmoID(AmmoId))
		return

	static const szAmmoTypes[AMMO_MAX_TYPES][] = {
		"", "338Magnum", "762Nato", "556NatoBox", "556Nato", "buckshot", "45acp", "57mm", 
		"50AE", "357SIG", "9mm", "Flashbang", "HEGrenade", "SmokeGrenade", "C4"
	}

	new element = IsGrenadeWeaponID(rg_get_weaponbox_id(weaponbox)) ? 1 : 0

	set_member(weaponbox, m_WeaponBox_rgiszAmmo, szAmmoTypes[AmmoId], element)
	set_member(weaponbox, m_WeaponBox_rgAmmo, (iCount < 0) ? 0 : iCount, element)
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Weapon stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

// rg_get_weapon_info() WI_SLOT
stock InventorySlotType:rg_get_weapon_slot(const weapon) {
	return (weapon > 0) ? rg_get_weaponid_slot(rg_get_weapon_id(weapon)) : NONE_SLOT
}

stock InventorySlotType:rg_get_weaponid_slot(const WeaponIdType:weaponid) 
{
	static const InventorySlotType:iWeaponsSlots[any:WEAPON_P90 + 1] = {
		NONE_SLOT,
		PISTOL_SLOT,
		NONE_SLOT,
		PRIMARY_WEAPON_SLOT,
		GRENADE_SLOT,
		PRIMARY_WEAPON_SLOT,
		C4_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		GRENADE_SLOT,
		PISTOL_SLOT,
		PISTOL_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PISTOL_SLOT,
		PISTOL_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		GRENADE_SLOT,
		PISTOL_SLOT,
		PRIMARY_WEAPON_SLOT,
		PRIMARY_WEAPON_SLOT,
		KNIFE_SLOT,
		PRIMARY_WEAPON_SLOT
	}
	
	return IsValidWeaponID(weaponid) ? iWeaponsSlots[weaponid] : NONE_SLOT
}

stock WeaponIdType:rg_get_weapon_id(const weapon) {
	return get_member(weapon, m_iId)
}

stock WeaponState:rg_get_weapon_state(const weapon) {
	return get_member(weapon, m_Weapon_iWeaponState)
}

stock WeaponState:rg_get_weapon_owner(const weapon) {
	return get_member(weapon, m_pPlayer)
}

stock rg_set_weapon_can_shot(const weapon, bool:can = true) {
	set_member(weapon, m_Weapon_iWeaponState, (can ? (any:0) : WPNSTATE_SHIELD_DRAWN))
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Armoury entity stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

// -1 = all armoury items
stock rg_invisibility_armoury(const ArmouryItemPack:itemID = any:-1, bool:visible = false)
{
	new pArmoury = NULLENT
	while((pArmoury = rg_find_ent_by_class(pArmoury, "armoury_entity", .useHashTable = true)))
	{
		if(itemID != any:-1 && itemID != get_member(pArmoury, m_Armoury_iItem))
			continue

		if(!visible)
		{
			set_entvar(pArmoury, var_effects, get_entvar(pArmoury, var_effects) | EF_NODRAW)
			set_entvar(pArmoury, var_solid, SOLID_NOT)
		}
		else
		{
			set_entvar(pArmoury, var_effects, get_entvar(pArmoury, var_effects) & ~EF_NODRAW)
			set_entvar(pArmoury, var_solid, SOLID_TRIGGER)
		}
	}
}

/* To get ArmouryItem ID from armoury_entity
*
* @param entity		armoury_entity
* @param count		count item
* @return			return enum's of ArmouryItemPack
*/
stock ArmouryItemPack:rg_get_armoury_info(const entity, &count = 1)
{
	new ArmouryItemPack:item = get_member(entity, m_Armoury_iItem)
	if(!IsValidArmoury(item))
		return any:-1

	count = get_member(entity, m_Armoury_iCount)
	return item
}

/* Set an armoury_entity to be of specified type.
*/
stock bool:rg_set_armoury_type(const entity, const ArmouryItemPack:item)
{
	if(!IsValidArmoury(item))
		return false

	set_member(entity, m_Armoury_iItem, item)
	return true
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ Other entity stocks ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

/* Sets the submodel setting of the entity.
* If this is 1, then the user has a backpack or defuser on their model (depending on team)
* 0 removes it.
*/
stock rg_set_entity_submodel(const entity, const value) {
	set_entvar(entity, var_body, value)
}

stock rg_get_entity_submodel(const entity, const value) {
	return get_entvar(entity, var_body)
}

/* Sets player/entity rendering mode. 
*/
stock rg_set_entity_rendering(const entity, fx = kRenderFxNone, Float:color[] = {255.0, 255.0, 255.0}, render = kRenderNormal, amount = 16) 
{
	set_entvar(entity, var_renderfx, fx)
	set_entvar(entity, var_rendercolor, color)
	set_entvar(entity, var_rendermode, render)
	set_entvar(entity, var_renderamt, float(amount))
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ ReAPI native fixes ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

// fix shield/elite drop
stock rg_give_item_fix(const player, const pszName[], GiveType:type = GT_APPEND)
{
	if(type != GT_APPEND)
	{
		new WeaponIdType:iId = rg_get_weapon_info(pszName, WI_ID)

		if((IsPrimaryWeaponID(iId) || iId == WEAPON_ELITE) && get_member(player, m_bOwnsShield))
		{
			rg_remove_item(player, "weapon_shield")
		}
		else if(iId == WEAPON_SHIELDGUN && user_has_weapon(player, any:WEAPON_ELITE))
		{
			rg_remove_item(player, "weapon_elite")
		}
	}

	return rg_give_item(player, pszName, type)
}

/**■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ PurposeLess ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

stock rg_get_user_button(const id)
{
	return get_entvar(id, var_button);
}

stock rg_get_user_oldbutton(const id)
{
	return get_entvar(id, var_oldbuttons);
}

stock rg_get_user_flags(const id)
{
	return get_entvar(id, var_flags);
}

stock rg_remove_entity(const index)
{
	if(!is_nullent(index))
	{
		set_entvar(index, var_flags, FL_KILLME);
	}
}

stock rg_user_kill(const victim, const attacker, bool:givefrag = true)
{
	static DeathMsg;

	if(DeathMsg || (DeathMsg = get_user_msgid("DeathMsg")))
	{
		user_silentkill(victim);

		message_begin(MSG_BROADCAST, DeathMsg, {0,0,0}, victim);
		write_byte(attacker);
		write_byte(victim);
		write_byte(0);
		message_end();

		if(givefrag)
		{
			static ScoreInfo;

			if(ScoreInfo || (ScoreInfo = get_user_msgid("ScoreInfo")))
			{
				if(!is_user_connected(attacker))
				{
					return;
				}

				new Float:frags = get_entvar(id, var_frags) + 1.0;
				set_entvar(id, var_frags, frags);

				message_begin(MSG_BROADCAST, ScoreInfo);
				write_short(floatround(frags));
				write_byte(id);
				write_short(get_member(id, m_iDeaths));
				write_short(0);
				write_short(get_member(id, m_iTeam));
				message_end();
			}
		}
	}
}

