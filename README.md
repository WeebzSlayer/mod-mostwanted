# 🏹 mod-mostwanted (Bounty Hunter System)

[![AzerothCore](https://img.shields.io/badge/AzerothCore-Latest-blue.svg)](https://www.azerothcore.org/)
[![Status](https://img.shields.io/badge/Status-Active-brightgreen.svg)]()
[![Type](https://img.shields.io/badge/Type-PvP%20%2F%20PvE-orange.svg)]()

---

A dynamic **Most Wanted / Bounty Hunter System** for AzerothCore. This module tracks player activity and identifies the most dangerous player in each faction, turning them into a high-value target for everyone else!

---

## 🌟 Features

*   ⚔️ **Dynamic Tracking:** Automatically tracks Player and NPC kills within a configurable time window.
*   🚩 **Faction Leaders:** Maintains one "Most Wanted" criminal for the **Horde** and one for the **Alliance**.
*   ⚖️ **Risk vs Reward:** 
    *   **Buff:** The Most Wanted player gains **+10% Damage** (to stay dangerous).
    *   **Debuff:** The Most Wanted player suffers **-5% Health and Defense** (to make them vulnerable).
*   💰 **Bounty Rewards:** Slaying a Most Wanted player grants Gold and End-Game currency/items (fully configurable).
*   📢 **Global Announcements:** The whole server is notified when a new Most Wanted is crowned or when a bounty is claimed.
*   📜 **Gossip NPC:** A dedicated NPC lists the current top bounties for both factions.

---

## 🛠️ Installation

1. **Download the module.**

2. **Database Setup :**
   *  Apply the provided SQL file to your database. You have to run this query to create the new table.

3. **Run CMake.**

4. **Compile**

---

## ⚙️ Configuration (`diablo.conf`)

1. Go to your server module folder.
   *  Copy mod_mostwanted.conf.dist to your conf folder.
   *  Edit values to your liking!

| Variable | Default | Description |
| :--- | :---: | :--- |
| `Diablo.Enable` | **1** | Toggle the entire module. |
| `Diablo.MovementSpeed.Boost` | **1.2** | 1.2	1.2 = 20% faster run speed. |
| `Diablo.Density.Chance` | **25** | Chance (%) for mobs to clone. |
| `Diablo.Champion.Chance` | **5** | Chance (%) Chance (%) for a mob to be a Champion. |
| `Diablo.Cleave.Chance` | **30** | Chance (%) for damage to trigger AOE. |
| `Diablo.Potion.HealPercent` | **35** | % of Max HP restored by Super Potions. |

2. **NPC Setup (Optional)**

   * Create a basic NPC template or update an existing one `UPDATE creature_template SET ScriptName = 'npc_mostwanted_list' WHERE entry = 99000;`

   * To use the leaderboard NPC, run this on your World database and spawn him in-game with `.npc add 99000`

---

## 📜 Credits

   *  [WeebzSlayer](https://github.com/WeebzSlayer)

---

Made for AzerothCore. ✨ If you like this module, feel free to give it a ⭐ on GitHub.

---
  
