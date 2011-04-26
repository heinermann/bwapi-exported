#include "ScriptEnum.h"
#include <BWAPI.h>

const char *pszUnitName[] =
{
  "marine",
  "ghost",
  "vulture",
  "goliath",
  "goliath_turret",
  "siege_tank",
  "tank_turret",
  "scv",
  "wraith",
  "science_vessel",
  "gui_montag",
  "dropship",
  "battlecruiser",
  "spider_mine",
  "nuke",
  "civilian",
  "sarah_kerrigan",
  "alan_schezar",
  "alan_turret",
  "jim_raynor_vulture",
  "jim_raynor_marine",
  "tom_kazansky",
  "magellan",
  "edmund_duke",
  "duke_turret",
  "edmund_duke_sieged",
  "duke_sieged_turret",
  "arcturus_mengsk",
  "hyperion",
  "norad_2",
  "siege_tank_sieged",
  "tank_sieged_turret",
  "firebat",
  "scanner_sweep",
  "medic",
  "larva",
  "egg",
  "zergling",
  "hydralisk",
  "ultralisk",
  "broodling",
  "drone",
  "overlord",
  "mutalisk",
  "guardian",
  "queen",
  "defiler",
  "scourge",
  "torrasque",
  "matriarch",
  "infested_terran",
  "infested_kerrigan",
  "unclean_one",
  "hunter_killer",
  "devouring_one",
  "kukulza",
  "kukulza_guardian",
  "yggdrasill",
  "valkyrie",
  "cocoon",
  "corsair",
  "dark_templar",
  "devourer",
  "dark_archon",
  "probe",
  "zealot",
  "dragoon",
  "high_templar",
  "archon",
  "shuttle",
  "scout",
  "arbiter",
  "carrier",
  "interceptor",
  "dark_templar_hero",
  "zeratul",
  "tassadar_zeratul",
  "fenix_zealot",
  "fenix_dragoon",
  "tassadar",
  "mojo",
  "warbringer",
  "gantrithor",
  "reaver",
  "observer",
  "scarab",
  "danimoth",
  "aldaris",
  "artanis",
  "rhynadon",
  "bengalaas",
  "cargo_ship",
  "mercenary_gunship",
  "scantid",
  "kakaru",
  "ragnasaur",
  "ursadon",
  "lurker_egg",
  "raszagal",
  "samir_duran",
  "alexei_stukov",
  "map_revealer",
  "gerard_dugalle",
  "lurker",
  "infested_duran",
  "disruption_web",
  "command_center",
  "comsat_station",
  "nuke_silo",
  "supply_depot",
  "refinery",
  "barracks",
  "academy",
  "factory",
  "starport",
  "control_tower",
  "science_facility",
  "covert_ops",
  "physics_lab",
  "starbase",
  "machine_shop",
  "repair_bay",
  "engineering_bay",
  "armory",
  "missile_turret",
  "bunker",
  "crashed_norad_2",
  "ion_cannon",
  "uraj_crystal",
  "khalis_crystal",
  "infested_command_center",
  "hatchery",
  "lair",
  "hive",
  "nydus_canal",
  "hydralisk_den",
  "defiler_mound",
  "greater_spire",
  "queens_nest",
  "evolution_chamber",
  "ultralisk_cavern",
  "spire",
  "spawning_pool",
  "creep_colony",
  "spore_colony",
  "",
  "sunken_colony",
  "overmind_with_shell",
  "overmind",
  "extractor",
  "mature_chrysalis",
  "cerebrate",
  "daggoth",
  "",
  "nexus",
  "robotics_facility",
  "pylon",
  "assimilator",
  "",
  "observatory",
  "gateway",
  "",
  "photon_cannon",
  "citadel_of_adun",
  "cybernetics_core",
  "templar_archives",
  "forge",
  "stargate",
  "stasis_cell",
  "fleet_beacon",
  "arbiter_tribunal",
  "robotics_support",
  "shield_battery",
  "khaydarin_crystal_formation",
  "protoss_temple",
  "xelnaga_temple",
  "mineral_field_1",
  "mineral_field_2",
  "mineral_field_3",
  "cave",
  "cave_in",
  "cantina",
  "mining_platform",
  "independant_command_center",
  "independant_starport",
  "independant_jumpgate",
  "ruins",
  "khaydarin_crystal_formation",
  "vespene_geyser",
  "warp_gate",
  "psi_disrupter",
  "zerg_marker",
  "terran_marker",
  "protoss_marker",
  "zerg_beacon",
  "terran_beacon",
  "protoss_beacon",
  "zerg_flag_beacon",
  "terran_flag_beacon",
  "protoss_flag_beacon",
  "power_generator",
  "overmind_cocoon",
  "dark_swarm",
  "floor_missile_trap",
  "floor_hatch",
  "left_upper_door",
  "right_upper_door",
  "left_pit_door",
  "right_pit_door",
  "floor_gun_trap",
  "left_wall_missile_trap",
  "left_wall_flame_trap",
  "right_wall_missile_trap",
  "right_wall_flame_trap",
  "start_location",
  "flag",
  "young_chrysalis",
  "psi_emitter",
  "data_disc",
  "khaydarin_crystal",
  "mineral_chunk_1",
  "mineral_chunk_2",
  "protoss_gas_chunk_1",
  "protoss_gas_chunk_2",
  "zerg_gas_chunk_1",
  "zerg_gas_chunk_2",
  "terran_gas_chunk_1",
  "terran_gas_chunk_2",
  "unknown"
};

const char *pszOpcodeName[] =
{
  "goto",
  "notowns_jump",
  "wait",
  "start_town",
  "start_areatown",
  "expand",
  "build",
  "upgrade",
  "tech",
  "wait_build",
  "wait_buildstart",
  "attack_clear",
  "attack_add",
  "attack_prepare",
  "attack_do",
  "wait_secure",
  "capt_expand",
  "build_bunkers",
  "wait_bunkers",
  "defensebuild_gg",
  "defensebuild_ag",
  "defensebuild_ga",
  "defensebuild_aa",
  "defenseuse_gg",
  "defenseuse_ag",
  "defenseuse_ga",
  "defenseuse_aa",
  "defenseclear_gg",
  "defenseclear_ag",
  "defenseclear_ga",
  "defenseclear_aa",
  "send_suicide",
  "player_enemy",
  "player_ally",
  "default_min",
  "defaultbuild_off",
  "stop",
  "switch_rescue",
  "move_dt",
  "debug",
  "fatal_error",
  "enter_bunker",
  "value_area",
  "transports_off",
  "check_transports",
  "nuke_rate",
  "max_force",
  "clear_combatdata",
  "random_jump",
  "time_jump",
  "farms_notiming",
  "farms_timing",
  "build_turrets",
  "wait_turrets",
  "default_build",
  "harass_factor",
  "start_campaign",
  "race_jump",
  "region_size",
  "get_oldpeons",
  "groundmap_jump",
  "place_guard",
  "wait_force",
  "guard_resources",
  "call",
  "return",
  "eval_harass",
  "creep",
  "panic",
  "player_need",
  "do_morph",
  "wait_upgrades",
  "multirun",
  "rush",
  "scout_with",
  "define_max",
  "train",
  "target_expansion",
  "wait_train",
  "set_attacks",
  "set_gencmd",
  "make_patrol",
  "give_money",
  "prep_down",
  "resources_jump",
  "enter_transport",
  "exit_transport",
  "sharedvision_on",
  "sharedvision_off",
  "nuke_location",
  "harass_location",
  "implode",
  "guard_all",
  "enemyowns_jump",
  "enemyresources_jump",
  "if_dif",
  "easy_attack",
  "kill_thread",
  "killable",
  "wait_finishattack",
  "quick_attack",
  "junkyard_dog",
  "fake_nuke",
  "disruption_web",
  "recall_location",
  "set_randomseed",
  "if_owned",
  "create_nuke",
  "create_unit",
  "nuke_pos",
  "help_iftrouble",
  "allies_watch",
  "try_townpoint",
  "unknown"
};

namespace AISCRIPT
{
  const char *getOpcodeName(int iOpcode)
  {
    if ( iOpcode > LAST || iOpcode < 0 ) 
      return pszOpcodeName[LAST];
    return pszOpcodeName[iOpcode];
  }
  const char *getUnitName(int iUnitType)
  {
    if ( iUnitType > BWAPI::UnitTypes::None || iUnitType < 0 )
      return pszUnitName[BWAPI::UnitTypes::None];
    return pszUnitName[iUnitType];
  }
}
