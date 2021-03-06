#include "Main.h";

using namespace std;

ChesterBurnettExecutor::ChesterBurnettExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(75);
	setMustBeCloseToLocate(true);

	campfirePos = toVector3(-1352.869, 2440.57, 307.4282);
	enemiesGroup = new GuardsGroup(campfirePos, 25, true); // Create a new Guards Group. First parameter is the center of the defense area. The second one is the radius. The third is whether to tolerate the player when he gets close or not.

	campfire = NULL;
	horse = NULL;
}

void ChesterBurnettExecutor::update()
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

void ChesterBurnettExecutor::prepareSet()
{
	campfire = createProp("P_CAMPFIRE02X", campfirePos);
	addHorse(horse);
	addHorse("A_C_Horse_KentuckySaddle_Black", toVector3(-1341.81, 2445.77, 308.204));
	addHorse("A_C_Horse_KentuckySaddle_SilverBay", toVector3(-1341.08, 2448.55, 308.184));

	// Now just add the enemies to the group to make them be controlled by it

	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1360.06, 2427.01, 306.921), rand() % 361), IdlingModifier::Rest);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1360.12, 2424.16, 306.917), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1353.54, 2437.52, 307.408), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1348.84, 2439.62, 307.424), rand() % 361), IdlingModifier::Rest);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1323.85, 2439.67, 308.62), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1321.54, 2437.53, 308.6), (rand() % 361)), IdlingModifier::Rest);
	enemiesGroup->add(createPed("G_M_M_UNIMICAHGOONS_01", toVector3(-1316.81, 2437.43, 308.583), (rand() % 361)), IdlingModifier::Scout);
	enemiesGroup->start();
}

Ped ChesterBurnettExecutor::spawnTarget()
{
	RoutineParams routine3;
	this->horse = createPed("A_C_Horse_KentuckySaddle_Grey", toVector3(-334.379, -156.812, 50.0511));
	routine3.Horse = horse;
	routine3.isTarget = true;
	Vector3 targetPos = toVector3(-1349.51, 2445.11, 307.429);
	Ped target = createPed("G_M_M_UNIMICAHGOONS_01", targetPos, rand() % 361);
	enemiesGroup->add(target, IdlingModifier::Scout, routine3);
	return target;
}

void ChesterBurnettExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	enemiesGroup->addBlips();
}

void ChesterBurnettExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false);
	horses.push_back(horse);
}

void ChesterBurnettExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void ChesterBurnettExecutor::releaseUnnecessaryEntities()
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

void ChesterBurnettExecutor::cleanup()
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

const int IDLE_DIST = 0;
const int ALERT_DIST = 0;
const int WARN_DIST = 0;
const int HEARING_RANGE = 0;
const int COMBAT_RANGE = 0;

ChesterBurnettExecutor::ChesterBurnettExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(50);
	setRequiredDistanceToLocateTarget(0);
	setMustBeCloseToLocate(true);
	enemiesStatus = EnemiesMode::IDLE;
	campfirePos = toVector3(-1352.869, 2440.57, 307.4282);
	toleratePlayer = true;
	campfire = NULL;
	horse = NULL;
}

void ChesterBurnettExecutor::update()
{
	BaseMissionExecutor::update();
	releaseUnnecessaryEntities();
	Ped player = PLAYER::PLAYER_PED_ID();
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
	}
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		if (*pedItr != target)
		{
			if (getMissionStage() == BountyMissionStage::LocateTarget && ENTITY::IS_ENTITY_DEAD(*pedItr) && enemiesStatus < EnemiesMode::COMBAT)
			{
				showSubtitle("Bounty Failed, you killed an innocent man.");
				stopwatch.start();
				enterCombatMode();
				AI::_0xFD45175A6DFD7CE9(target, player, 3, 0, -999.0f, -1, 0); // FLEE
			}
			if (getMissionStage() == BountyMissionStage::ArriveToPoliceStation && ENTITY::IS_ENTITY_DEAD(*pedItr) && distanceBetweenEntities(*pedItr, player) <= 250)
			{
				showSubtitle("Bounty Failed, you killed an innocent man.");
				stopwatch.start();
				enterCombatMode();
				AI::_0xFD45175A6DFD7CE9(target, player, 3, 0, -999.0f, -1, 0); // FLEE
			}
			if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(*pedItr, player, false, false))
			{
				AI::TASK_COMBAT_PED(*pedItr, player, 0, 16);
			}
			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), *pedItr))
			{
				AI::TASK_COWER(*pedItr, 9999, 0, 0);
				if (ENTITY::IS_ENTITY_DEAD(target))
				{
					AI::TASK_COWER(*pedItr, 9999, 0, 0);
				}
			}
		}
		else
		{
			if (ENTITY::IS_ENTITY_DEAD(*pedItr))
			{
				setMustBeCloseToLocate(false);
			}
			if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(*pedItr, player, false, false))
			{
				AI::TASK_COMBAT_PED(*pedItr, player, 0, 16);
			}
			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), *pedItr))
			{
				giveWeaponToPed(target, RevolverCattleman, 0x64356159, true);
				AI::TASK_COMBAT_PED(*pedItr, player, 0, 16);
			}
		}
		if (PED::IS_PED_SHOOTING(player))
		{
			AI::TASK_COWER(*pedItr, 9999, 0, 0);
		}
		if (isPedHogtied(target))
		{
			setMustBeCloseToLocate(false);
		}
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

Ped ChesterBurnettExecutor::spawnTarget()
{
	Vector3 targetPos = toVector3(-1349.408, 2446.22, 308.4254);
	Ped target = createPed("G_M_M_UNIMICAHGOONS_01", targetPos);
	giveWeaponToPed(target, RevolverCattleman, 0x64356159, false);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(target, GAMEPLAY::GET_HASH_KEY("REL_CRIMINALS"));
	return target;
}

void ChesterBurnettExecutor::enterIdleMode()
{
	char* scenarioName;
	Ped player = PLAYER::PLAYER_PED_ID();
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		Object seq;
		AI::OPEN_SEQUENCE_TASK(&seq);
		AI::TASK_TURN_PED_TO_FACE_COORD(*pedItr, campfirePos.x, campfirePos.y, campfirePos.z, 0);
		AI::CLOSE_SEQUENCE_TASK(seq);
		AI::TASK_PERFORM_SEQUENCE(*pedItr, seq);
		AI::CLEAR_SEQUENCE_TASK(&seq);
	}

	enemiesStatus = EnemiesMode::IDLE;
}

void ChesterBurnettExecutor::enterAlertMode()
{
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		PED::_0xFE07FF6495D52E2A(*pedItr, 0, 0, 0);
	}

	//if (enemiesStatus == EnemiesMode::IDLE)
	{
		playAmbientSpeech(target, "GET_LOST");
	}
	//else if (enemiesStatus == EnemiesMode::WARNING)
	{
		playAmbientSpeech(target, "WON_DISPUTE");
	}

	enemiesStatus = EnemiesMode::ALERTED;
}

void ChesterBurnettExecutor::enterWarningMode()
{
	vector<Ped>::iterator pedItr;
	for (pedItr = enemies.begin(); pedItr != enemies.end(); pedItr++)
	{
		pedEquipBestWeapon(*pedItr);
	}

	playAmbientSpeech(target, "FINAL_WARNING");
	enemiesStatus = EnemiesMode::WARNING;
}

void ChesterBurnettExecutor::enterCombatMode()
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
				AI::TASK_COMBAT_PED(0, player, 0, 16);
				AI::CLOSE_SEQUENCE_TASK(seq);

				AI::CLEAR_PED_TASKS(target, 1, 1);
				AI::TASK_PERFORM_SEQUENCE(target, seq);
				AI::CLEAR_SEQUENCE_TASK(&seq);
				playAmbientSpeech(target, "ITS_MALE_EXTREME");
			}
		}
		else
		{
			if (ENTITY::IS_ENTITY_DEAD(*pedItr))
			{
				AI::_0xFD45175A6DFD7CE9(target, player, 3, 0, -999.0f, -1, 0); // FLEE
			}
			Object seq;
			AI::OPEN_SEQUENCE_TASK(&seq);
			AI::_0xFD45175A6DFD7CE9(0, player, 3, 0, -999.0f, -1, 0); // FLEE
			AI::CLOSE_SEQUENCE_TASK(seq);

			AI::CLEAR_PED_TASKS(*pedItr, 1, 1);
			AI::TASK_PERFORM_SEQUENCE(*pedItr, seq);
			AI::CLEAR_SEQUENCE_TASK(&seq);
		}
	}
	if (stopwatch.getElapsedSecondsRealTime() >= 4)
	{
		fail();
	}
}

void ChesterBurnettExecutor::prepareSet()
{
	toleratePlayer = true;
	campfire = createProp("P_CAMPFIRE02X", campfirePos);

	addEnemy(target);
	addEnemy(toVector3(-1351.776, 2436.312, 308.4017));
	addEnemy(toVector3(-1347.818, 2439.917, 308.449));
	addEnemy(toVector3(-1357.465, 2438.535, 308.4151));

	enterIdleMode();
}

void ChesterBurnettExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
}

void ChesterBurnettExecutor::createEnemyBlips()
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

void ChesterBurnettExecutor::releaseUnnecessaryEntities()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	std::vector<Ped>::iterator it;

	if (getMissionStage() >= BountyMissionStage::ArriveToPoliceStation)
	{
		releaseEntitySafe(&horse);

		for (it = enemies.begin(); it != enemies.end(); ++it)
		{
			if (distanceBetweenEntities(*it, player) > 250)
			{
				releaseEntitySafe(&(*it));
			}
		}
	}
}

void ChesterBurnettExecutor::addEnemy(Vector3 pos)
{
	Ped enemyPed = createPed("G_M_M_UNIMICAHGOONS_01", pos);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(target, GAMEPLAY::GET_HASH_KEY("REL_CRIMINALS"));
	addEnemy(enemyPed);
}

void ChesterBurnettExecutor::addEnemy(Ped ped)
{
	enemies.push_back(ped);

	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
	AI::CLEAR_PED_TASKS(ped, true, true);
}

void ChesterBurnettExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void ChesterBurnettExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false); // give saddle
	horses.push_back(horse);
}

void ChesterBurnettExecutor::cleanup()
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