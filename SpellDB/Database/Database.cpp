#include "Database.h"

#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include "../Library/http.h"
#include "../Library/json.h"
#include "Helper.h"

using namespace nlohmann;

void Database::get_staging_champions()
{
	std::string get_version = http->Request("GET", "https://ddragon.leagueoflegends.com/api/versions.json");
	std::string version = json::parse(get_version)[0];

	std::cout << "> Analyzing patch " + version << std::endl;

	std::string get_champions = http->Request("GET", "http://ddragon.leagueoflegends.com/cdn/" + version + "/data/en_US/champion.json");
	auto json_champions = json::parse(get_champions)["data"];

	for (auto& champion : json_champions) {
		champions.emplace_back(champion.at("id").get<std::string>());
	}

	std::cout << "> Aquired " << champions.size() << " champions" << std::endl;
}

void Database::analyze_champion_spells(std::string champion_name)
{
	std::cout << std::endl  << "> Analyzing " << champion_name << " spells" << std::endl;

	std::string url = "https://raw.communitydragon.org/latest/game/data/characters/" + to_lower(champion_name) + "/" + to_lower(champion_name) + ".bin.json";
	std::string get_data = http->Request("GET", url);
	json json_data = json::parse(get_data);

	for (json::iterator it = json_data.begin(); it != json_data.end(); it++)
	{
		if (!it.key().find("Spells/"))
			continue;

		if (!it.value()["mScriptName"].is_string())
			continue;

		if (it.value().find("mSpell") == it.value().end())
			continue;

		/* Filter spell by name */
		std::string m_script_name = it.value()["mScriptName"];
		if (m_script_name.find("BasicAttack") != std::string::npos || m_script_name.find("CritAttack") != std::string::npos)
		{
			spells_to_ignore.emplace_back(m_script_name);
			continue;
		}

		/* Filter spell by field */
		auto root = it.value()["mSpell"];
		if (root.find("mTargetingTypeData") != root.end())
		{
			if (root["mTargetingTypeData"]["__type"] == "Self" || root["mTargetingTypeData"]["__type"] == "SelfAoe")
			{
				spells_to_ignore.emplace_back(m_script_name);
				continue;
			}
		}
		else
		{
			spells_to_ignore.emplace_back(m_script_name);
			continue;
		}

		spells_to_parse++;
	}

	std::cout << "> Found " << spells_to_parse << " valid & " << spells_to_ignore.size() << " invalid spells" << std::endl;
}

void Database::generate_spelldb(std::string champion_name_exclusive)
{
	get_staging_champions();
	bool exclusive = champion_name_exclusive != "None";

	for (auto& champion_name : champions)
	{
		if (exclusive && champion_name != champion_name_exclusive) continue;
		analyze_champion_spells(champion_name);

		std::string url = "https://raw.communitydragon.org/latest/game/data/characters/" + to_lower(champion_name) + "/" + to_lower(champion_name) + ".bin.json";
		std::string get_data = http->Request("GET", url);
		json json_data = json::parse(get_data);

		std::ofstream out;
		system(std::format("del {}.json", champion_name).c_str()); // Delete previous file
		
		out.open(std::format("{}.json", champion_name), std::ios::app);
		out << '{' << std::endl;

		for (json::iterator it = json_data.begin(); it != json_data.end(); it++)
		{
			if (!it.key().find("Spells/"))
				continue;

			if (!it.value()["mScriptName"].is_string())
				continue;

			if (it.value().find("mSpell") == it.value().end())
				continue;

			bool is_first_property = true;
			bool should_skeep = false;
			std::string m_script_name = it.value()["mScriptName"];
			auto root = it.value()["mSpell"];

			for (auto& spell_name : spells_to_ignore)
			{
				if (spell_name == m_script_name)
				{
					should_skeep = true;
					break;
				}
			}

			if (should_skeep)
				continue;

			// Generate JSON
			out << "\t" + add_quotes(m_script_name) + ": {";

			if (root.find("missileSpeed") != root.end())
			{
				float missile_speed = root["missileSpeed"];
				out << (is_first_property ? "" : ", ") << add_quotes("missileSpeed") + ": " << missile_speed;
				is_first_property = false;
			}

			if (root.find("mLineWidth") != root.end())
			{
				float missile_line_width = root["mLineWidth"];
				out << (is_first_property ? "" : ", ") << add_quotes("mLineWidth") + ": " << missile_line_width;
				is_first_property = false;
			}

			if (root.find("mTargetingTypeData") != root.end())
			{
				out << (is_first_property ? "" : ", ") << add_quotes("mTargetingTypeData") + ": " << root["mTargetingTypeData"]["__type"];
				is_first_property = false;
			}

			spells_parsed++;

			if (spells_parsed != spells_to_parse)
				out << "},\n";
			else
				out << "}\n";
		}

		out << "}";
		std::cout << "> Generated " << champion_name << ".json" << std::endl;
	}
}