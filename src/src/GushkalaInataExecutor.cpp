#include "Main.h";

using namespace std;

GushkalaInataExecutor::GushkalaInataExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(60);
	setMustBeCloseToLocate(true);

	campfirePos = toVector3(-2369.87, -1597.39, 152.826);
	enemiesGroup = new GuardsGroup(campfirePos, 25, true); // Create a new Guards Group. First parameter is the center of the defense area. The second one is the radius. The third is whether to tolerate the player when he gets close or not.

	campfire = NULL;
	horse = NULL;
}

void GushkalaInataExecutor::update()
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
		if (distanceBetweenEntities(target, player) > 80)
		{
			showSubtitle("The target is getting too far!");
		}
		if (distanceBetweenEntities(target, player) > 120)
		{
			PED::DELETE_PED(&target);
			PED::DELETE_PED(&horse);
			fail("Bounty failed, target lost");
		}
	}
}

void GushkalaInataExecutor::prepareSet()
{
	campfire = createProp("P_CAMPFIRE02X", campfirePos);
	addHorse(horse);
	addHorse("A_C_Horse_KentuckySaddle_Black", toVector3(-2388.5, -1595.79, 153.245));
	addHorse("A_C_Horse_KentuckySaddle_SilverBay", toVector3(-2388.79, -1594.31, 153.25));

	// Now just add the enemies to the group to make them be controlled by it
	RoutineParams routine1;
	routine1.patrolName = "miss_hello52";
	routine1.patrolRoute.push_back(toVector3(-2376.88, -1582.15, 153.182));
	routine1.patrolHeading.push_back(toVector3(-2377.47, -1581.64, 153.186));
	routine1.patrolRoute.push_back(toVector3(-2385.41, -1590.58, 153.181));
	routine1.patrolHeading.push_back(toVector3(-2385.84, -1591.27, 153.183));
	routine1.patrolRoute.push_back(toVector3(-2376.21, -1599.85, 153.273));
	routine1.patrolHeading.push_back(toVector3(-2375.7, -1600.16, 153.271));
	routine1.patrolRoute.push_back(toVector3(-2367.12, -1590.19, 152.656));
	routine1.patrolHeading.push_back(toVector3(-2366.61, -1589.77, 152.61));

	RoutineParams routine2;
	routine2.patrolName = "miss_hello59";
	routine2.patrolRoute.push_back(toVector3(-2386.16, -1604.21, 153.68));
	routine2.patrolHeading.push_back(toVector3(-2386.8, -1604.84, 153.637));
	routine2.patrolRoute.push_back(toVector3(-2374.48, -1613.81, 150.837));
	routine2.patrolHeading.push_back(toVector3(-2373.85, -1614.25, 150.735));
	routine2.patrolRoute.push_back(toVector3(-2369.76, -1601.2, 152.73));
	routine2.patrolHeading.push_back(toVector3(-2369.78, -1600.77, 152.75));

	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2373.24, -1595.4, 153.004), 256), IdlingModifier::Rest);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2372.19, -1591.21, 152.99), rand() % 306 + 165), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2375.83, -1572.4, 153.468), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2386.16, -1604.21, 153.68), 134), IdlingModifier::Patrol, routine2);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2376.88, -1582.15, 153.182), rand() % 361), IdlingModifier::Patrol, routine1);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2373.92, -1587.23, 153.122), 329), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2392.55, -1590.97, 153.303), 90), IdlingModifier::Rest);
	enemiesGroup->start();
}

Ped GushkalaInataExecutor::spawnTarget()
{
	RoutineParams routine3;
	this->horse = createPed("A_C_Horse_KentuckySaddle_Grey", toVector3(-2389.18, -1592.39, 153.229));
	routine3.Horse = horse;
	routine3.isTarget = true;
	Vector3 targetPos = toVector3(-2379.75, -1591.5, 153.157);
	Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos, 146);
	enemiesGroup->add(target, IdlingModifier::Rest, routine3);
	return target;
}

void GushkalaInataExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	enemiesGroup->addBlips();
}

void GushkalaInataExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false);
	horses.push_back(horse);
}

void GushkalaInataExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void GushkalaInataExecutor::releaseUnnecessaryEntities()
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

void GushkalaInataExecutor::cleanup()
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

using namespace std;

const int IDLE_DIST = 3;
const int ALERT_DIST = 3;
const int WARN_DIST = 3;
const int HEARING_RANGE = 3;
const int COMBAT_RANGE = 3;

GushkalaInataExecutor::GushkalaInataExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(3);
	setMustBeCloseToLocate(true);
	enemiesStatus = EnemiesMode::IDLE;
	campfirePos = toVector3(2126.519, 166.7494, 75.02033);
	toleratePlayer = true;
	campfire = NULL;
	horse = NULL;
}

void GushkalaInataExecutor::update()
{
	BaseMissionExecutor::update();
	releaseUnnecessaryEntities();
	Ped player = PLAYER::PLAYER_PED_ID();
	float distanceToTarget = distanceBetweenEntities(target, player);
	if (getMissionStage() == BountyMissionStage::CaptureTarget)
	{
		float distanceToTarget = distanceBetweenEntities(target, player);
		switch (enemiesStatus)
		{
		case EnemiesMode::IDLE:
			if (distanceToTarget <= ALERT_DIST)
			{
				if (toleratePlayer)
				{
					stopwatch.start();
					enterAlertMode();
				}
				else
				{
					enterCombatMode();
				}
			}
			break;
		case EnemiesMode::ALERTED:
			if (stopwatch.getElapsedSecondsRealTime() >= 5)
			{
				if (toleratePlayer)
				{
					stopwatch.start();
					enterWarningMode();
				}
				else
				{
					enterCombatMode();
				}
			}
			else if (distanceToTarget >= IDLE_DIST)
			{
				stopwatch.stop();
				enterIdleMode();
			}
			break;

		case EnemiesMode::WARNING:
			if (stopwatch.getElapsedSecondsRealTime() >= 4)
			{
				if (distanceToTarget <= WARN_DIST)
				{
					enterCombatMode();
				}
				else if (distanceToTarget >= ALERT_DIST)
				{
					toleratePlayer = false;
					stopwatch.stop();
					enterAlertMode();
				}
			}
			break;
		}

		if (enemiesStatus < EnemiesMode::COMBAT && PED::IS_PED_ON_FOOT(player) && distanceToTarget <= HEARING_RANGE)
		{
			if (distanceToTarget <= COMBAT_RANGE || PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()) || PED::IS_PED_SHOOTING(player))
			{
				enterCombatMode();
			}
		}
	}
	if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY((PLAYER::PLAYER_ID()), target) && distanceToTarget <= 10)
	{
		enterCombatMode();
	}
	if (getMissionStage() == BountyMissionStage::LocateTarget && PED::IS_PED_SHOOTING(player) && enemiesStatus < EnemiesMode::COMBAT && distanceToTarget <= 8)
	{
		enterCombatMode();
	}
	if (getMissionStage() == BountyMissionStage::CaptureTarget && enemiesStatus >= EnemiesMode::ALERTED && !ENTITY::IS_ENTITY_DEAD(target))
	{
		if (distanceBetweenEntities(target, player) > 80)
		{
			showSubtitle("The target is getting too far!");
		}
		if (distanceBetweenEntities(target, player) > 120)
		{
			fail("Bounty failed, target lost");
		}
	}

}

Ped GushkalaInataExecutor::spawnTarget()
{
	int iSecret;
	iSecret = rand() % 3 + 1;
	if (iSecret == 1) 
	{
		Vector3 targetPos = toVector3(-2296.886, -1476.857, 134.9669);
		Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos);
		return target;
	}
	if (iSecret == 2)
	{
		Vector3 targetPos = toVector3(-2326.244, -1418.903, 142.3623);
		Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos);
		return target;
	}
	if (iSecret == 3)
	{
		Vector3 targetPos = toVector3(-2329.595, -1459.724, 142.6416);
		Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos);
		return target;
	}
}

void GushkalaInataExecutor::enterIdleMode()
{
	char* scenarioName;
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		Object seq;
		AI::OPEN_SEQUENCE_TASK(&seq);
		AI::_0x17293C633C8AC019(target, true, 0, true);
		AI::CLOSE_SEQUENCE_TASK(seq);
		AI::TASK_PERFORM_SEQUENCE(*pedItr, seq);
		AI::CLEAR_SEQUENCE_TASK(&seq);
	}

	enemiesStatus = EnemiesMode::IDLE;
}

void GushkalaInataExecutor::enterAlertMode()
{
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		PED::_0xFE07FF6495D52E2A(*pedItr, 0, 0, 0);
		AI::TASK_TURN_PED_TO_FACE_ENTITY(*pedItr, PLAYER::PLAYER_PED_ID(), -1, 0, 0, 0);
	}
	enemiesStatus = EnemiesMode::ALERTED;
}

void GushkalaInataExecutor::enterWarningMode()
{
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		pedEquipBestWeapon(*pedItr);
	}
	enemiesStatus = EnemiesMode::WARNING;
}

void GushkalaInataExecutor::enterCombatMode()
{
	enemiesStatus = EnemiesMode::COMBAT;

	Ped player = PLAYER::PLAYER_PED_ID();
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		PED::_0xFE07FF6495D52E2A(*pedItr, 0, 0, 0);
		WEAPON::SET_CURRENT_PED_WEAPON(*pedItr, WEAPON::GET_BEST_PED_WEAPON(*pedItr, 0, 0), true, 0, false, false);

		if (*pedItr == target)
		{
			{
				Object seq;
				AI::OPEN_SEQUENCE_TASK(&seq);
				AI::_0xFD45175A6DFD7CE9(0, player, 3, 0, -999.0f, -1, 0); // FLEE
				AI::CLOSE_SEQUENCE_TASK(seq);

				AI::CLEAR_PED_TASKS(target, 1, 1);
				AI::TASK_PERFORM_SEQUENCE(target, seq);
				AI::CLEAR_SEQUENCE_TASK(&seq);
			}
		}
	}
}

void GushkalaInataExecutor::prepareSet()
{
	toleratePlayer = true;
	addEnemy(target);

	enterIdleMode();
}

void GushkalaInataExecutor::onTargetLocated()
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

void GushkalaInataExecutor::createEnemyBlips()
{
	std::vector<Ped>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		if (*it != target)
		{
			createBlip(*it, BLIP_STYLE_ENEMY);
		}
	}
}

void GushkalaInataExecutor::releaseUnnecessaryEntities()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	std::vector<Ped>::iterator it;

	if (getMissionStage() >= BountyMissionStage::ArriveToPoliceStation)
	{
		releaseEntitySafe(&horse);

		for (it = enemies.begin(); it != enemies.end(); ++it)
		{
			if (distanceBetweenEntities(*it, player) > 250 ||  ENTITY::IS_ENTITY_DEAD(*it))
			{
				releaseEntitySafe(&(*it));
			}
		}
	}
}

void GushkalaInataExecutor::addEnemy(Vector3 pos)
{
	Ped enemyPed = createPed("G_M_M_UniCriminals_01", pos);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(enemyPed, GAMEPLAY::GET_HASH_KEY("REL_CRIMINALS"));
	addEnemy(enemyPed);
}

void GushkalaInataExecutor::addEnemy(Ped ped)
{
	enemies.push_back(ped);

	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
	AI::CLEAR_PED_TASKS(ped, true, true);
}

void GushkalaInataExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void GushkalaInataExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false); // give saddle
	horses.push_back(horse);
}

void GushkalaInataExecutor::cleanup()
{
	BaseMissionExecutor::cleanup();
	releaseEntitySafe(&campfire);

	vector<Ped>::iterator pedItr;
	for (pedItr = horses.begin(); pedItr != horses.end(); pedItr++)
	{
		releaseEntitySafe(&(*pedItr));
	}

	for (pedItr = enemies.begin(); pedItr != enemies.end(); ++pedItr)
	{
		releaseEntitySafe(&(*pedItr));
	}
}*/