# Spell Database Parser
This is a tool to create custom spell database from datadragon for League of Legends script.
Basically it parses the pages, filters unnecessary stuff out and generates .json file for every champion it can find for latest patch of the game.

I didn't implement any configuration file so you have to recompile it each time if you want to generate .json file for one particular champion.

To generate spell database for every champion call:
```cpp
database.generate_spelldb("None");
```

For particular champion call with champion name:
```cpp
database.generate_spelldb("Lux");
```