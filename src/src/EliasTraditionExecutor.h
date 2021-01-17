#pragma once

class EliasTraditionExecutor : public BaseMissionExecutor
{
private:
	Vector3 campfirePos;
	Object campfire;
	Ped horse;
	GuardsGroup* enemiesGroup;
	vector<Ped> horses;

public:
	EliasTraditionExecutor(BountyMissionData missionData, MapAreasManager* areasMgr);
	void update();

protected:
	void prepareSet();
	Ped spawnTarget();
	void onTargetLocated();
	void cleanup();

private:
	void releaseUnnecessaryEntities();
	void addHorse(Ped horse);
	void addHorse(const char* model, Vector3 pos);
};
//#pragma once
//
//class EliasTraditionExecutor : public BaseMissionExecutor
//{
//private:
//	vector<Ped> enemies;
//	Object campfire;
//	Ped horse;
//	Ped killer;
//	Ped victim;
//	bool executedVictim;
//	bool executionCleanedUp;
//	bool enemiesAlerted;
//
//public:
//	EliasTraditionExecutor(BountyMissionData missionData, MapAreasManager* areasMgr);
//	void update();
//
//protected:
//	void prepareSet();
//	Ped spawnTarget();
//
//private:
//	void onTargetLocated();
//	void createEnemyBlips();
//	Ped addGuard(Vector3 position);
//	void addGuard(Ped guard);
//	void updateEnemies();
//	void playVictimExecution();
//	void cleanUpExecution();
//	void releaseUnnecessaryEntities();
//	void cleanup();
//};