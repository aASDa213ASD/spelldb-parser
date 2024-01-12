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

Example of `lux.json`:
```json
{
	"LuxLightBinding": {"missileSpeed": 1200, "mLineWidth": 80, "mTargetingTypeData": "Location"},
	"LuxLightBindingDummy": {"missileSpeed": 1200, "mLineWidth": 50, "mTargetingTypeData": "Direction"},
	"LuxLightBindingMis": {"missileSpeed": 1200, "mLineWidth": 70, "mTargetingTypeData": "Direction"},
	"LuxLightStrikeKugel": {"missileSpeed": 1300, "mTargetingTypeData": "Area"},
	"LuxPrismaticWave": {"missileSpeed": 1200, "mLineWidth": 150, "mTargetingTypeData": "Location"},
	"LuxPrismaticWaveMissile": {"missileSpeed": 2200, "mLineWidth": 110, "mTargetingTypeData": "Location"},
	"LuxPrismaticWaveReturnDead": {"missileSpeed": 50, "mLineWidth": 110, "mTargetingTypeData": "Direction"},
	"LuxR": {"missileSpeed": 3000, "mLineWidth": 190, "mTargetingTypeData": "Location"},
	"LuxRVfxMis": {"missileSpeed": 3600, "mLineWidth": 60, "mTargetingTypeData": "Location"},
	"LuxRMis": {"missileSpeed": 3000, "mLineWidth": 250, "mTargetingTypeData": "Direction"}
}
```