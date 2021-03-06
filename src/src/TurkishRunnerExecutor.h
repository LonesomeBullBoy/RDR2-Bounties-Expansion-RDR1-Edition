#pragma once

class TurkishRunnerExecutor : public BaseMissionExecutor
{
private:
	Vector3 campfirePos;
	Object campfire;
	Ped horse;
	GuardsGroup* enemiesGroup;
	vector<Ped> horses;

public:
	TurkishRunnerExecutor(BountyMissionData missionData, MapAreasManager* areasMgr);
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

/*#pragma once

enum EnemiesMode
{
	IDLE,
	ALERTED,
	WARNING,
	COMBAT
};

class TurkishRunnerExecutor : public BaseMissionExecutor
{
private:
	Vector3 campfirePos;
	Object campfire;
	Ped horse;
	vector<Ped> enemies;
	vector<Ped> horses;
	EnemiesMode enemiesStatus;
	GameStopwatch stopwatch;
	bool toleratePlayer;

public:
	TurkishRunnerExecutor(BountyMissionData missionData, MapAreasManager* areasMgr);
	void update();

protected:
	void prepareSet();
	Ped spawnTarget();
	void onTargetLocated();
	void cleanup();

private:
	void releaseUnnecessaryEntities();
	void addEnemy(Vector3 pos);
	void addEnemy(Ped ped);
	void addHorse(const char* model, Vector3 pos);
	void addHorse(Ped horse);
	void enterIdleMode();
	void enterAlertMode();
	void enterWarningMode();
	void enterCombatMode();
};*/