#include "Main.h";

using namespace std;

EliasTraditionExecutor::EliasTraditionExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(75);
	setMustBeCloseToLocate(true);

	campfirePos = toVector3(-2070.24, -1900.76, 111.898);
	enemiesGroup = new GuardsGroup(campfirePos, 25, true); // Create a new Guards Group. First parameter is the center of the defense area. The second one is the radius. The third is whether to tolerate the player when he gets close or not.

	campfire = NULL;
	horse = NULL;
}

void EliasTraditionExecutor::update()
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

void EliasTraditionExecutor::prepareSet()
{
	campfire = createProp("P_CAMPFIRE02X", campfirePos);
	addHorse(horse);
	addHorse("A_C_Horse_KentuckySaddle_Black", toVector3(-2078.73, -1880.09, 111.949));
	addHorse("A_C_Horse_KentuckySaddle_SilverBay", toVector3(-2076.44, -1879.77, 111.838));

	// Now just add the enemies to the group to make them be controlled by it
	RoutineParams routine1;
	routine1.patrolName = "miss_hello50";
	routine1.patrolRoute.push_back(toVector3(-2035.56, -1906.09, 109.124));
	routine1.patrolHeading.push_back(toVector3(-2034.5, -1906.35, 109.103));
	routine1.patrolRoute.push_back(toVector3(-2056.46, -1917.68, 112.019));
	routine1.patrolHeading.push_back(toVector3(-2057.24, -1918.32, 112.09));
	routine1.patrolRoute.push_back(toVector3(-2077.67, -1904.75, 112.313));
	routine1.patrolHeading.push_back(toVector3(-2078.75, -1904.13, 112.363));
	routine1.patrolRoute.push_back(toVector3(-2053.82, -1895.88, 110.446));
	routine1.patrolHeading.push_back(toVector3(-2052.73, -1896.03, 110.345));

	RoutineParams routine2;
	routine2.patrolName = "miss_hello51";
	routine2.patrolRoute.push_back(toVector3(-2074.99, -1882.89, 111.71));
	routine2.patrolHeading.push_back(toVector3(-2075.76, -1882.94, 111.789));
	routine2.patrolRoute.push_back(toVector3(-2087.34, -1892.93, 112.611));
	routine2.patrolHeading.push_back(toVector3(-2087.99, -1893.9, 112.662));
	routine2.patrolRoute.push_back(toVector3(-2077.17, -1899.15, 112.219));
	routine2.patrolHeading.push_back(toVector3(-2076.48, -1899.31, 112.188));
	routine2.patrolRoute.push_back(toVector3(-2064.54, -1888.72, 111.395));
	routine2.patrolHeading.push_back(toVector3(-2064.29, -1887.96, 111.365));

	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2035.81, -1899.38, 109.275), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2062.25, -1925.62, 112.307), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2075.1, -1891.69, 112.461), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2035.56, -1906.09, 109.124), (rand() % 361)), IdlingModifier::Patrol, routine1);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2074.99, -1882.89, 111.71), (rand() % 361)), IdlingModifier::Patrol, routine2);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2071.35, -1895.33, 111.858), 183), IdlingModifier::Rest);
	enemiesGroup->add(createPed("g_m_m_unimountainmen_01", toVector3(-2040.35, -1896.23, 109.198), 84), IdlingModifier::Rest);
	enemiesGroup->start();
}

Ped EliasTraditionExecutor::spawnTarget()
{
	RoutineParams routine3;
	this->horse = createPed("A_C_Horse_KentuckySaddle_Grey", toVector3(-2073.25, -1879.32, 111.744));
	routine3.Horse = horse;
	routine3.isTarget = true;
	Vector3 targetPos = toVector3(-2054.91, -1891.52, 110.513);
	Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos, (rand() % 361));
	enemiesGroup->add(target, IdlingModifier::Scout, routine3);
	return target;
}

void EliasTraditionExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	enemiesGroup->addBlips();
}

void EliasTraditionExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false);
	horses.push_back(horse);
}

void EliasTraditionExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void EliasTraditionExecutor::releaseUnnecessaryEntities()
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

void EliasTraditionExecutor::cleanup()
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
#include "ExecuteVictimInteraction.h";

const Vector3 VICTIMS_POS = toVector3(-2033.07, -1908.37, 110.075);
const int COMBAT_RANGE = 18;
const int EXECUTION_RANGE = 35;
const int HEARING_RANGE = 40;

EliasTraditionExecutor::EliasTraditionExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(130);
	setMustBeCloseToLocate(true);
	executionCleanedUp = false;
	enemiesAlerted = false;
}

void EliasTraditionExecutor::update()
{
	BaseMissionExecutor::update();

	Ped player = PLAYER::PLAYER_PED_ID();
	updateEnemies();

	if (!executedVictim && 
		(distanceBetweenEntities(target, player) < EXECUTION_RANGE) && 
		ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(player, victim, 0))
	{
		executedVictim = true;
		playVictimExecution();
	}
	
	if (executedVictim && victim && ENTITY::DOES_ENTITY_EXIST(victim) && ENTITY::IS_ENTITY_DEAD(victim) && !executionCleanedUp)
	{
		executionCleanedUp = true;
		cleanUpExecution();
	}
		
	if (enemiesAlerted)
	{
		vector<Ped>::iterator it;

		for (it = enemies.begin(); it != enemies.end(); ++it)
		{
			AI::TASK_COMBAT_PED(*it, player, 0, 16);
		}
	}

	releaseUnnecessaryEntities();
}

Ped EliasTraditionExecutor::spawnTarget()
{
	Vector3 targetPos = toVector3(-2033.78, -1914.03, 109.976);
	Ped target = createPed(SKINNER_BROTHER_MODEL, targetPos);
	addGuard(target);
	return target;
}

void EliasTraditionExecutor::prepareSet()
{
	executionCleanedUp = false;
	enemiesAlerted = false;
	Ped player = PLAYER::PLAYER_PED_ID();
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(player, true, false);
	vector<Ped>::iterator enemiesItr;
	int victimNo;

	campfire = createProp("P_CAMPFIRE02X", toVector3(-2031.62, -1921.61, 108.276));
	horse = createPed("A_C_Horse_KentuckySaddle_Black", toVector3(-2038.82, -1912.17, 110.337));
	giveSaddleToHorse(horse, HorseSaddleHashes::CHARRO_01_STOCK_NEW_SADDLE_000);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);

	killer = addGuard(toVector3(-2029.45, -1907.8, 110.041));
	addGuard(toVector3(-2023.57, -1905.58, 110.068));
	addGuard(toVector3(-2039.7, -1906.49, 109.158));

	victim = createPed(F_LOWER_TOWN_FOLK, VICTIMS_POS);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(victim, true);
	AI::CLEAR_PED_TASKS(victim, 1, 1);
	AI::TASK_HANDS_UP(victim, 99999, killer, -1, 0);
	
	AI::TASK_TURN_PED_TO_FACE_ENTITY(killer, victim, -1, 0, 0, 0);
}

void EliasTraditionExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	createEnemyBlips();
}

void EliasTraditionExecutor::createEnemyBlips()
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

void EliasTraditionExecutor::updateEnemies()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	std::vector<Ped>::iterator it;
	bool justAlerted = false;

	if (!enemiesAlerted)
	{
		for (it = enemies.begin(); it != enemies.end(); ++it)
		{
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(*it, player, 1) &&
				distanceBetweenEntities(*it, player) < COMBAT_RANGE)
			{
				enemiesAlerted = true;
				justAlerted = true;
			}
		}

		if (PED::IS_PED_SHOOTING(player) && distanceBetweenEntities(target, player) < HEARING_RANGE)
		{
			enemiesAlerted = true;
			justAlerted = true;
		}
	}

	if (enemiesAlerted && justAlerted)
	{
		playAmbientSpeech(target, "ITS_THEM_EXTREME");
	}
}

Ped EliasTraditionExecutor::addGuard(Vector3 position)
{
	Ped guard = createPed(SKINNER_BROTHER_MODEL, position);
	addGuard(guard);
	return guard;
}

void EliasTraditionExecutor::addGuard(Ped guard)
{
	Vector3 position = ENTITY::GET_ENTITY_COORDS(guard, 1, 0);
	enemies.push_back(guard);

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_STAND_GUARD(0, position.x, position.y, position.z, 0, "WORLD_HUMAN_GUARD_SCOUT");
	AI::CLOSE_SEQUENCE_TASK(seq);

	AI::CLEAR_PED_TASKS(guard, true, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(guard, true);
	AI::TASK_PERFORM_SEQUENCE(guard, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);
}

void EliasTraditionExecutor::playVictimExecution()
{
	Vector3 killerPos = ENTITY::GET_ENTITY_COORDS(killer, 1, 0);
	giveWeaponToPed(killer, WeaponHash::MeleeMachete, 0, true);
	
	std::vector<Ped>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		if (*it != killer)
		{
			AI::CLEAR_PED_TASKS(*it, 1, 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(*it, victim, -1, 0, 0, 0);
		}
	}

	Conversation conv;
	conv.addLine(target, "CALLOUT_JUST_KILL_EM_NEUTRAL");
	conv.addLine(new ExecuteVictimInteraction(victim, killer));
	conv.addLine(victim, "PLEAD");
	conv.addLine(victim, "PANIC_HELP");
	conv.play();
}

void EliasTraditionExecutor::cleanUpExecution()
{
	std::vector<Ped>::iterator it;
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_GO_TO_ENTITY(0, campfire, 20000, 1.5f, 1, 0, 0);
	AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND_COFFEE_DRINK"), -1, true, true, 0, true); // PLAY SCENARIO
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::CLEAR_PED_TASKS(killer, 1, 1);
	AI::TASK_PERFORM_SEQUENCE(killer, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);

	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		if (*it != killer)
		{
			Vector3 position = ENTITY::GET_ENTITY_COORDS(*it, 1, 1);
			AI::CLEAR_PED_TASKS(*it, 1, 1);
			AI::TASK_STAND_GUARD(*it, position.x, position.y, position.z, 0, "WORLD_HUMAN_GUARD_SCOUT");
		}
	}
}

void EliasTraditionExecutor::releaseUnnecessaryEntities()
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

void EliasTraditionExecutor::cleanup()
{
	BaseMissionExecutor::cleanup();
	releaseEntitySafe(&horse);
	releaseEntitySafe(&victim);
	vector<Ped>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); ++it)
	{
		releaseEntitySafe(&(*it));
	}
}*/