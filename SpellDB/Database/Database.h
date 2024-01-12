#pragma once

#include <vector>
#include <string>

class Database {
private:
	std::vector<std::string> champions;
	std::vector<std::string> spells_to_ignore;

	int spells_to_parse;
	int spells_parsed;

	void get_staging_champions();
	void analyze_champion_spells(std::string champion_name);

public:
	void generate_spelldb(std::string champion_name_exclusive = "None");

};