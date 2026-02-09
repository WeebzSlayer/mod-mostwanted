#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Creature.h"
#include "ScriptedGossip.h"
#include "World.h"

// Global to store the current Most Wanted for each faction (Alliance = 0, Horde = 1)
ObjectGuid currentMostWanted[2];

class MostWantedModule : public PlayerScript
{
public:
    MostWantedModule() : PlayerScript("MostWantedModule") {}

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (!sConfigMgr->GetOption<bool>("MostWanted.Enable", true))
            return;

        UpdateBounty(killer, 1);
        CheckBountyClaim(killer, killed);
    }

    void OnCreatureKill(Player* killer, Creature* killed) override
    {
        if (!sConfigMgr->GetOption<bool>("MostWanted.Enable", true))
            return;

        // NPCs count less than players? (Adjust logic as desired)
        UpdateBounty(killer, 1);
    }

    void UpdateBounty(Player* player, uint32 count)
    {
        uint32 guid = player->GetGUID().GetCounter();
        uint32 faction = (player->GetTeamId() == TEAM_ALLIANCE) ? 0 : 1;
        uint32 now = GameTime::GetGameTime();

        CharacterDatabase.Execute("INSERT INTO most_wanted_stats (guid, kills, last_kill_time, faction) VALUES ({}, {}, {}, {}) "
                                   "ON DUPLICATE KEY UPDATE kills = kills + {}, last_kill_time = {}", 
                                   guid, count, now, faction, count, now);

        CheckForNewLeader(player, faction);
    }

    void CheckForNewLeader(Player* player, uint32 faction)
    {
        QueryResult result = CharacterDatabase.Query("SELECT guid, kills FROM most_wanted_stats WHERE faction = {} ORDER BY kills DESC LIMIT 1", faction);
        if (result)
        {
            Field* fields = result->Fetch();
            ObjectGuid topGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].Get<uint32>());
            
            if (topGuid != currentMostWanted[faction] && fields[1].Get<uint32>() >= sConfigMgr->GetOption<uint32>("MostWanted.MinKills", 10))
            {
                currentMostWanted[faction] = topGuid;
                std::string msg = "|cffff0000[Most Wanted]|r " + player->GetName() + " is now the Most Wanted criminal of the " + (faction == 0 ? "Alliance" : "Horde") + "!";
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str());
            }
        }
    }

    void CheckBountyClaim(Player* killer, Player* victim)
    {
        uint32 victimFaction = (victim->GetTeamId() == TEAM_ALLIANCE) ? 0 : 1;
        if (victim->GetGUID() == currentMostWanted[victimFaction])
        {
            // Reward Gold
            killer->ModifyMoney(sConfigMgr->GetOption<uint32>("MostWanted.RewardGold", 10000000));
            // Reward Item
            killer->AddItem(sConfigMgr->GetOption<uint32>("MostWanted.RewardItemId", 49426), sConfigMgr->GetOption<uint32>("MostWanted.RewardItemCount", 5));
            
            std::string msg = "|cff00ff00[Bounty Claimed]|r " + killer->GetName() + " has slain the Most Wanted: " + victim->GetName() + " and received a massive reward!";
            sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str());

            // Reset victim's status
            CharacterDatabase.Execute("DELETE FROM most_wanted_stats WHERE guid = {}", victim->GetGUID().GetCounter());
            currentMostWanted[victimFaction] = ObjectGuid::Empty;
        }
    }
};

// Hook for Stat Modifiers (Applying the 10% dmg buff and -5% health debuff)
class MostWantedStats : public PlayerScript
{
public:
    MostWantedStats() : PlayerScript("MostWantedStats") {}

    void OnUpdate(Player* player, uint32 /*diff*/) override
    {
        uint32 faction = (player->GetTeamId() == TEAM_ALLIANCE) ? 0 : 1;
        if (player->GetGUID() == currentMostWanted[faction])
        {
            // Note: In a production module, apply a hidden Aura with these mods.
            // For simplicity, we use the core hooks if available or script it here.
            if (!player->HasAura(8257)) // Example spell ID for 10% damage (Bloodlust-like)
                 player->CastSpell(player, 8257, true);
        }
        else
        {
            player->RemoveAura(8257);
        }
    }
};

// NPC Script for listing bounties
class npc_mostwanted_list : public CreatureScript
{
public:
    npc_mostwanted_list() : CreatureScript("npc_mostwanted_list") {}

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "View Alliance Most Wanted", GOSSIP_SENDER_MAIN, 0);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "View Horde Most Wanted", GOSSIP_SENDER_MAIN, 1);
        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) override
    {
        QueryResult result = CharacterDatabase.Query("SELECT guid, kills FROM most_wanted_stats WHERE faction = {} ORDER BY kills DESC LIMIT 5", action);
        if (result)
        {
            std::string list = "Top Bounties for the " + std::string(action == 0 ? "Alliance" : "Horde") + ":\n";
            do {
                Field* f = result->Fetch();
                // Note: Guid to Name lookup would be done here via sObjectMgr or DB
                list += "Kills: " + std::to_string(f[1].Get<uint32>()) + "\n";
            } while (result->NextRow());
            ChatHandler(player->GetSession()).PSendSysMessage(list.c_str());
        }
        CloseGossipMenuFor(player);
        return true;
    }
};

void Addmod_mostwantedScripts()
{
    new MostWantedModule();
    new MostWantedStats();
    new npc_mostwanted_list();
}