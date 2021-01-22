#include "Main.h";

using namespace std;

JacobSeedExecutor::JacobSeedExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(80);
	setMustBeCloseToLocate(true);

	campfirePos = toVector3(2061.37, -1838.93, 40.7533);
	enemiesGroup = new GuardsGroup(campfirePos, 25, true); // Create a new Guards Group. First parameter is the center of the defense area. The second one is the radius. The third is whether to tolerate the player when he gets close or not.

	campfire = NULL;
	horse = NULL;
}

void JacobSeedExecutor::update()
{
	BaseMissionExecutor::update();
	releaseUnnecessaryEntities();
	Ped player = PLAYER::PLAYER_PED_ID();
	Vector3 lastImpactCoords;
	vector<Ped>::iterator pedItr;
	vector<Ped>* enemyPeds = enemiesGroup->peds();
	for (pedItr = enemyPeds->begin(); pedItr != enemyPeds->end(); ++pedItr)
	{
		if (!ENTITY::IS_ENTITY_DEAD(target) && !isPedHogtied(target))
		{
			if (!PED::IS_PED_ON_MOUNT(target) && !PED::_0xAAB0FE202E9FC9F0(horse, -1) && !PED::IS_PED_IN_COMBAT(target, player))
			{
				PED::_0x5337B721C51883A9(*pedItr, true, true);
			}
		}
		if ((ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(*pedItr, player, true, true) || WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(player, &lastImpactCoords) && distanceBetween(ENTITY::GET_ENTITY_COORDS(*pedItr, 1, 0), lastImpactCoords) <= GUARD_SUSPECT_RANGE) && getMissionStage() == BountyMissionStage::LocateTarget)
		{
			nextStage();
		}
	}

	enemiesGroup->update(); // Update the group to keep it working

	if (getMissionStage() == BountyMissionStage::CaptureTarget && !ENTITY::IS_ENTITY_DEAD(target))
	{
		if (distanceBetweenEntities(target, player) > 100)
		{
			showSubtitle("The target is getting too far!");
		}
		if (distanceBetweenEntities(target, player) > 140)
		{
			PED::DELETE_PED(&target);
			PED::DELETE_PED(&horse);
			fail("Bounty failed, target lost");
		}
	}
}

void JacobSeedExecutor::prepareSet()
{
	campfire = createProp("P_CAMPFIRE02X", campfirePos);
	addHorse(horse);
	addHorse("A_C_Horse_KentuckySaddle_Black", toVector3(2075.75, -1817.13, 40.6617));
	addHorse("A_C_Horse_KentuckySaddle_SilverBay", toVector3(2076.22, -1815.83, 40.6673));

	// Now just add the enemies to the group to make them be controlled by it
	RoutineParams routine1;
	routine1.patrolName = "miss_hello57";
	routine1.patrolRoute.push_back(toVector3(2065.37, -1842.76, 40.7511));
	routine1.patrolHeading.push_back(toVector3(2065.27, -1842.41, 40.7488));
	routine1.patrolRoute.push_back(toVector3(2050.63, -1855.54, 40.7495));
	routine1.patrolHeading.push_back(toVector3(2049.79, -1855.86, 40.7705));
	routine1.patrolRoute.push_back(toVector3(2066.75, -1865.27, 40.8109));
	routine1.patrolHeading.push_back(toVector3(2066.44, -1865.89, 40.8223));

	RoutineParams routine2;
	routine2.patrolName = "miss_hello58";
	routine2.patrolRoute.push_back(toVector3(2089.45, -1836.11, 40.8435));
	routine2.patrolHeading.push_back(toVector3(2089.43, -1836.87, 40.8254));
	routine2.patrolRoute.push_back(toVector3(2072.09, -1834.86, 40.546));
	routine2.patrolHeading.push_back(toVector3(2071.75, -1834.9, 40.5498));
	routine2.patrolRoute.push_back(toVector3(2080.32, -1820.5, 40.6918));
	routine2.patrolHeading.push_back(toVector3(2079.95, -1819.81, 40.6836));

	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2087.34, -1817.95, 41.9341), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2089.93, -1818.91, 41.9271), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2091.45, -1815.44, 41.9287), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2089.45, -1836.11, 40.8435), 178), IdlingModifier::Patrol, routine2);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2064.44, -1833.69, 40.7375), 150), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2085.92, -1829.28, 40.7415), 112), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_Y_UniExConfeds_01", toVector3(2065.37, -1842.76, 40.7511), 17), IdlingModifier::Patrol, routine1);
	enemiesGroup->start();
}

Ped JacobSeedExecutor::spawnTarget()
{
	RoutineParams routine3;
	this->horse = createPed("A_C_Horse_KentuckySaddle_Grey", toVector3(2076.97, -1813.81, 40.6774));
	routine3.Horse = horse;
	routine3.isTarget = true;
	Vector3 targetPos = toVector3(2093.4, -1819.38, 41.82);
	Ped target = createPed("G_M_O_UniExConfeds_01", targetPos, 230);
	enemiesGroup->add(target, IdlingModifier::Scout, routine3);
	return target;
}

void JacobSeedExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	enemiesGroup->addBlips();
}

void JacobSeedExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false);
	horses.push_back(horse);
}

void JacobSeedExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void JacobSeedExecutor::releaseUnnecessaryEntities()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	std::vector<Ped>::iterator it;

	if (getMissionStage() >= BountyMissionStage::ArriveToPoliceStation)
	{
		for (it = horses.begin(); it != horses.end(); it++)
		{
			releaseEntitySafe(&(*it));
		}
	}
}

void JacobSeedExecutor::cleanup()
{
	BaseMissionExecutor::cleanup();

	enemiesGroup->stop();
	releaseEntitySafe(&campfire);

	vector<Ped>::iterator pedItr;
	for (pedItr = horses.begin(); pedItr != horses.end(); pedItr++)
	{
		releaseEntitySafe(&(*pedItr));
	}
	vector<Ped>* enemyPeds = enemiesGroup->peds();
	for (pedItr = enemyPeds->begin(); pedItr != enemyPeds->end(); ++pedItr)
	{
		releaseEntitySafe(&(*pedItr));
	}
}

/*#include "Main.h";

const float COMBAT_RANGE = 18.0f;
const int LEMOYNE_RAIDERS_MODELS_COUNT = 2;
char* const LEMOYNE_RAIDERS_MODELS[LEMOYNE_RAIDERS_MODELS_COUNT] = {"g_m_o_uniexconfeds_01", "g_m_y_uniexconfeds_01"/*, "g_m_y_uniexconfeds_02"*//*};
const Hash GATLING_POLICE_WAGON_VEHICLE = 0xB31F8075;
const float FAIL_DISTANCE = 110.0f;

JacobSeedExecutor::JacobSeedExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(140);
	enemiesAlerted = false;
	finishedAlertingEnemies = false;
}

void JacobSeedExecutor::update()
{
	BaseMissionExecutor::update();
	releaseUnnecessaryEntities();
	updateEnemies();
	Ped player = PLAYER::PLAYER_PED_ID();
	vector<Ped>::iterator it;
	float distanceToTarget;


	if (enemiesAlerted && !finishedAlertingEnemies)
	{
		enemyShootGatling(shooter);
		enemyDriveGatling(target);

		playAmbientSpeech(target, "ITS_THEM_EXTREME");

		for (it = enemies.begin(); it != enemies.end(); ++it)
		{
			if (*it != shooter && *it != target)
			{
				PED::_0xFE07FF6495D52E2A(*it, 0, 0, 0);
				AI::TASK_COMBAT_PED(*it, player, 0, 16);
			}
		}

		finishedAlertingEnemies = true;
	}

	distanceToTarget = distanceBetweenEntities(player, target);
	if (enemiesAlerted && !ENTITY::IS_ENTITY_DEAD(target))
	{
		if (distanceToTarget > FAIL_DISTANCE)
		{
			fail("Bounty failed, target lost");
		}
		else if (distanceToTarget > FAIL_DISTANCE * 0.8)
		{
			showSubtitle("The target is getting away!");
		}
	}
}

Ped JacobSeedExecutor::spawnTarget()
{
	Ped target = createPed("g_m_o_uniexconfeds_01", toVector3(1552.53, -369.602, 80.6158), 163.664);
	addEnemy(target);
	return target;
}

void JacobSeedExecutor::prepareSet()
{
	enemiesAlerted = false;
	finishedAlertingEnemies = false;
	campfire = createProp("P_CAMPFIRE02X", toVector3(1551.14, -372.55, 80.5242));
	gatlingWagon = createVehicle(GATLING_POLICE_WAGON_VEHICLE, toVector3(1550.35, -363.854, 80.3371), 63.1261);

	shooter = addEnemy(toVector3(1546.45, -369.089, 79.9711), 157.155);
	Ped scouter = addEnemy(toVector3(1560.44, -365.805, 80.0124), 275.14);
	Ped campEnemy1 = addEnemy(toVector3(1550.92, -375.835, 80.4239), 6.18465);
	Ped campEnemy2 = addEnemy(toVector3(1547.77, -373.901, 80.2731), 306.579);

	enemyScout(shooter, 157.155);
	enemyScout(scouter, 275.14);
	enemySitCamp(target);
	enemySitCamp(campEnemy1);
	enemySitCamp(campEnemy2);
}

void JacobSeedExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();

	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		if (*pedItr != target)
		{
			createBlip(*pedItr, BLIP_STYLE_ENEMY);
		}
	}
}

void JacobSeedExecutor::updateEnemies()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	std::vector<Ped>::iterator it;

	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(*it, player, 1) &&
			distanceBetweenEntities(*it, player) < COMBAT_RANGE)
		{
			enemiesAlerted = true;
		}
	}
}

void JacobSeedExecutor::releaseUnnecessaryEntities()
{
	Ped player = PLAYER::PLAYER_PED_ID();

	if (getMissionStage() >= BountyMissionStage::ArriveToPoliceStation)
	{
		vector<Ped>::iterator pedItr;

		for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
		{
			if (distanceBetweenEntities(*pedItr, player) > 250 || ENTITY::IS_ENTITY_DEAD(*pedItr))
			{
				releaseEntitySafe(&(*pedItr));
			}
		}
	}
}

void JacobSeedExecutor::cleanup()
{
	BaseMissionExecutor::cleanup();

	releaseEntitySafe(&campfire);
	releaseEntitySafe(&gatlingWagon);

	vector<Ped>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		releaseEntitySafe(&(*it));
	}
}

Ped JacobSeedExecutor::addEnemy(Vector3 pos, float heading)
{
	char* model = LEMOYNE_RAIDERS_MODELS[enemies.size() % LEMOYNE_RAIDERS_MODELS_COUNT];
	Ped enemy = createPed(model, pos);
	addEnemy(enemy);
	return enemy;
}

void JacobSeedExecutor::addEnemy(Ped enemy)
{
	enemies.push_back(enemy);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(enemy, true);
}

void JacobSeedExecutor::enemySitCamp(Ped ped)
{
	AI::_0x524B54361229154F(ped, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND"), -1, true, true, 0, true); // PLAY SCENARIO
}

void JacobSeedExecutor::enemyScout(Ped enemy, float heading)
{
	Vector3 position = ENTITY::GET_ENTITY_COORDS(enemy, 1, 0);
	AI::TASK_STAND_GUARD(enemy, position.x, position.y, position.z, heading, "WORLD_HUMAN_GUARD_SCOUT");
}

void JacobSeedExecutor::enemyShootGatling(Ped enemy)
{
	Ped player = PLAYER::PLAYER_PED_ID();

	if (ENTITY::IS_ENTITY_DEAD(enemy) || ENTITY::IS_ENTITY_DEAD(gatlingWagon))
	{
		return;
	}

	int seat = -2;
	VEHICLE::_0xFF5791B7639C2A46(gatlingWagon, (Any*)&seat);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_ENTER_VEHICLE(0, gatlingWagon, 20000, seat, 2, 1, 0);
	AI::TASK_VEHICLE_SHOOT_AT_PED(0, player, 20);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(enemy, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);
}

void JacobSeedExecutor::enemyDriveGatling(Ped enemy)
{
	Ped player = PLAYER::PLAYER_PED_ID();

	if (ENTITY::IS_ENTITY_DEAD(enemy) || ENTITY::IS_ENTITY_DEAD(gatlingWagon))
	{
		return;
	}

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_ENTER_VEHICLE(0, gatlingWagon, 20000, -1, 3.0f, 1, 0);
	AI::_0xFD45175A6DFD7CE9(0, player, 3, 0, -999.0f, -1, 0); // FLEE
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(enemy, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);
}*/