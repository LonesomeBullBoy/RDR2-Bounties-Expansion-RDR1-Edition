#include "Main.h";

using namespace std;

AndrewClarkExecutor::AndrewClarkExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(100);
	setRequiredDistanceToLocateTarget(75);
	setMustBeCloseToLocate(true);

	campfirePos = toVector3(-333.478, -144.551, 49.0278);
	enemiesGroup = new GuardsGroup(campfirePos, 25, true); // Create a new Guards Group. First parameter is the center of the defense area. The second one is the radius. The third is whether to tolerate the player when he gets close or not.

	campfire = NULL;
	horse = NULL;
}

void AndrewClarkExecutor::update()
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

void AndrewClarkExecutor::prepareSet()
{
	campfire = createProp("P_CAMPFIRE02X", campfirePos);
	addHorse(horse);
	addHorse("A_C_Horse_KentuckySaddle_Black", toVector3(-332.809, -157.876, 50.4296));
	addHorse("A_C_Horse_KentuckySaddle_SilverBay", toVector3(-336.251, -155.543, 49.7358));

	// Now just add the enemies to the group to make them be controlled by it
	RoutineParams routine1;
	routine1.patrolName = "miss_hello53";
	routine1.patrolRoute.push_back(toVector3(-315.557, -151.732, 51.1304));
	routine1.patrolHeading.push_back(toVector3(-315.046, -152.307, 51.2733));
	routine1.patrolRoute.push_back(toVector3(-328.284, -162.227, 51.708));
	routine1.patrolHeading.push_back(toVector3(-329.186, -162.743, 51.8057));
	routine1.patrolRoute.push_back(toVector3(-336.818, -149.052, 49.4045));
	routine1.patrolHeading.push_back(toVector3(-337.332, -148.165, 49.3222));
	routine1.patrolRoute.push_back(toVector3(-331.05, -137.693, 48.4476));
	routine1.patrolHeading.push_back(toVector3(-330.699, -136.948, 48.4058));

	RoutineParams routine2;
	routine2.patrolName = "miss_hello54";
	routine2.patrolRoute.push_back(toVector3(-342.646, -126.223, 48.037));
	routine2.patrolHeading.push_back(toVector3(-343.241, -126.248, 47.961));
	routine2.patrolRoute.push_back(toVector3(-347.87, -140.826, 47.1577));
	routine2.patrolHeading.push_back(toVector3(-348.867, -141.175, 46.9923));
	routine2.patrolRoute.push_back(toVector3(-353.417, -132.43, 46.3223));
	routine2.patrolHeading.push_back(toVector3(-353.916, -131.616, 46.2376));

	enemiesGroup->add(createPed("G_M_M_UniDuster_01", toVector3(-327.237, -148.022, 50.0784), rand() % 361), IdlingModifier::Rest);
	enemiesGroup->add(createPed("G_M_M_UniDuster_02", toVector3(-325.412, -151.986, 50.0784), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UniDuster_01", toVector3(-344.713, -157.153, 49.6692), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UniDuster_02", toVector3(-341.713, -153.949, 49.6787), 343), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UniDuster_01", toVector3(-319.058, -133.953, 50.7173), rand() % 361), IdlingModifier::Scout);
	enemiesGroup->add(createPed("G_M_M_UniDuster_02", toVector3(-315.557, -151.732, 51.1304), (rand() % 361)), IdlingModifier::Patrol, routine1);
	enemiesGroup->add(createPed("G_M_M_UniDuster_01", toVector3(-342.646, -126.223, 48.037), (rand() % 361)), IdlingModifier::Patrol, routine2);
	enemiesGroup->start();
}

Ped AndrewClarkExecutor::spawnTarget()
{
	RoutineParams routine3;
	this->horse = createPed("A_C_Horse_KentuckySaddle_Grey", toVector3(-334.379, -156.812, 50.0511));
	routine3.Horse = horse;
	routine3.isTarget = true;
	Vector3 targetPos = toVector3(-329.429, -151.443, 50.0784);
	Ped target = createPed("G_M_M_UniDuster_02", targetPos, rand() % 361);
	enemiesGroup->add(target, IdlingModifier::Rest, routine3);
	return target;
}

void AndrewClarkExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	enemiesGroup->addBlips();
}

void AndrewClarkExecutor::addHorse(Ped horse)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	PED::_0xD3A7B003ED343FD9(horse, 0x8FFCF06B, true, false, false);
	horses.push_back(horse);
}

void AndrewClarkExecutor::addHorse(const char* model, Vector3 pos)
{
	Ped horse = createPed((char*)model, pos);
	addHorse(horse);
}

void AndrewClarkExecutor::releaseUnnecessaryEntities()
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

void AndrewClarkExecutor::cleanup()
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

const char ROBBERY_HINT[] = "Hint: Andrew was found guilty of stealing money from the bank he had been working for...";
const char ROBBERY_ESCALATE_HINT[] = "Try to make Andrew understand that you're serious.";
const int OFFSET_FROM_TARGET = 4;
const int ROBBERY_DISTANCE = OFFSET_FROM_TARGET + 2;

AndrewClarkExecutor::AndrewClarkExecutor(BountyMissionData missionData, MapAreasManager* areasMgr)
	: BaseMissionExecutor(missionData, areasMgr)
{
	setTargetAreaRadius(120);
	setRequiredDistanceToLocateTarget(15);
	setMustBeCloseToLocate(true);
	isTargetAlerted = false;
	isTargetScared = false;
	robberyAttempts = 0;
	robberyProgress = RobberyProgress::NONE;
	stash = NULL;
	Vector3 stashPos = toVector3(-345.296, 1362.1, 158.634);
	campProps.push_back(stash);
	campProps.push_back(createProp("p_moneystack01x", stashPos));
	campProps.push_back(createProp("p_moneystack01x", stashPos));
}

void AndrewClarkExecutor::update()
{
	BaseMissionExecutor::update();
	Ped player = PLAYER::PLAYER_PED_ID();

	if (getMissionStage() >= BountyMissionStage::LocateTarget)
	{
		if (distanceBetweenEntities(player, target) < 12 && !isTargetAlerted)
		{
			isTargetAlerted = true;
			PED::_0xFE07FF6495D52E2A(target, 0, 0, 0);
			WAIT(500);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(target, player, -1, 0, 0, 0);
			playAmbientSpeech(target, "GET_LOST");
		}

		if (isTargetAlerted)
		{
			Hash playerCurrWeapon;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &playerCurrWeapon, true, 0, false);

			if (getMissionStage() == BountyMissionStage::CaptureTarget && playerCurrWeapon != GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO"))
			{
				playTargetRobbery();
			}
			else
			{
				threatPrompt->hide();
			}
		}
	}
}

Ped AndrewClarkExecutor::spawnTarget()
{
	Ped target = createPed("A_M_M_GriFancyTravellers_01", toVector3(-311.059, 1355.05, 158.291), 283.609);
	AUDIO::SET_AMBIENT_VOICE_NAME(target, "0974_A_M_M_ROUGHTRAVELLERS_WHITE_05");
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(target, GAMEPLAY::GET_HASH_KEY("REL_CRIMINALS"));
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(target, true);
	return target;
}

void AndrewClarkExecutor::prepareSet()
{
	isTargetAlerted = false;
	isTargetScared = false;
	robberyAttempts = 0;
	robberyProgress = RobberyProgress::NONE;
	threatPrompt = new Prompt("Where Is The Money?", 0x9FA5AD07);
	wagon = createVehicle(VehicleHash::Wagon02X, toVector3(-310.107, 1360.19, 158.084), 94.2345);
	setVehicleCargo(wagon, VehicleCargoHash::CampCargo1);

	Object campfire = createProp("p_campfire05x", toVector3(-308.143, 1355.59, 158.132));
	campProps.push_back(campfire);

	float tentHeading = 193.621;
	Vector3 tentPos = toVector3(-307.686, 1351.84, 158.326);
	campProps.push_back(createProp("p_amb_tent01x", tentPos, tentHeading));
	campProps.push_back(createProp("p_bedrollopen03x", tentPos, tentHeading));
	*/
	/*Vector3 stashPos = toVector3(-408.529, 1264.26, 159.21);
	stash = createProp("p_boxmeddeposit01x", stashPos, 141.608);*//*
	Vector3 stashPos = toVector3(-345.296, 1362.1, 158.634);
	stash = createProp("p_boxmeddeposit01x", stashPos, 335.257);
	campProps.push_back(stash);

	AI::_0x524B54361229154F(target, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND"), -1, true, true, 0, true);
}

void AndrewClarkExecutor::onTargetLocated()
{
	BaseMissionExecutor::onTargetLocated();
	Ped player = PLAYER::PLAYER_PED_ID();

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	if (PED::IS_PED_ON_MOUNT(player))
	{
		AI::_0x48E92D3DDE23C23A(0, PED::GET_MOUNT(player), 0, 0, 0, 0);
	}
	else if (PED::IS_PED_ON_VEHICLE(player, 0))
	{
		AI::TASK_LEAVE_ANY_VEHICLE(0, 0, 0);
	}
	AI::TASK_GO_TO_ENTITY(0, target, 10000, OFFSET_FROM_TARGET, 1, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(player, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);

	setHelpMessage(ROBBERY_HINT);
}

void AndrewClarkExecutor::cleanup()
{
	BaseMissionExecutor::cleanup();
	threatPrompt->remove();
	releaseEntitySafe(&wagon);

	vector<Object>::iterator propsItr;
	for (propsItr = campProps.begin(); propsItr != campProps.end(); propsItr++)
	{
		releaseEntitySafe(&(*propsItr));
	}
}

void AndrewClarkExecutor::playTargetRobbery()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	Conversation robberyInteraction;

	if (robberyProgress == RobberyProgress::NONE)
	{
		if (distanceBetweenEntities(player, target) < OFFSET_FROM_TARGET)
		{
			threatPrompt->show();
		}
		else
		{
			threatPrompt->hide();
		}

		if (threatPrompt->isActivatedByPlayer())
		{
			robberyProgress = RobberyProgress::TARGET_RESISTING;
			threatPrompt->hide();
		}
	}
	else if (robberyProgress == RobberyProgress::TARGET_RESISTING)
	{
		// conv.addLine(player, "RE_GP_MNT_V2_GREET_ROB_POST");
		robberyInteraction.addLine(new RobberyAimAtVictim(player, target));
		robberyInteraction.addLine(target, "WHOA");
		robberyInteraction.addLine(player, "RE_AMD_LWL_V2_ROB_DEALERS");
		robberyInteraction.addLine(target, "INTIMIDATED_ROB");
		robberyInteraction.play();
		robberyProgress = RobberyProgress::WAITING_FOR_INTIMIDATION;
		robberyAttempts = 0;
	}
	else if (robberyProgress == RobberyProgress::WAITING_FOR_INTIMIDATION)
	{
		threatPrompt->setText("Give Me The Money Or Else...");
		threatPrompt->show();

		if (threatPrompt->isActivatedByPlayer())
		{
			robberyAttempts++;
			threatPrompt->hide();
			if (robberyAttempts == 1)
			{
				robberyInteraction.addLine(player, "ROB_THREATEN");
				robberyInteraction.addLine(target, "RT_INTIMIDATED_ROB_NOT_INTIMIDATED");
			}
			else
			{
				robberyInteraction.play();robberyInteraction.addLine(player, "ROB_AGAIN_THREATEN");
				robberyInteraction.addLine(target, "STOP_THAT");
			}
			robberyInteraction.play();
			threatPrompt->show();

			if (robberyAttempts > 2)
			{
				setHelpMessage(ROBBERY_ESCALATE_HINT);
			}
		}

		if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(target, player, 0, 0))
		{
			robberyProgress = RobberyProgress::TARGET_GAVE_UP;
		}
	}
	else if (robberyProgress == RobberyProgress::TARGET_GAVE_UP)
	{
		if (threatPrompt->isActivatedByPlayer())
		{
			threatPrompt->hide();
			robberyInteraction.addLine(player, "ROB_PULL_GUN");
			robberyInteraction.addLine(target, "HAND_OVER_MONEY");
			robberyInteraction.play();
			goToStash();
			robberyProgress = RobberyProgress::FINISHED;
		}
	}
}

void AndrewClarkExecutor::goToStash()
{
	if (ENTITY::IS_ENTITY_DEAD(target))
	{
		return;
	}

	string msg = string("Follow ").append(getMissionData()->targetName).append(" to the stash.");
	showSubtitle(msg.c_str());
	
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_GO_TO_ENTITY(0, stash, 30000, 0.3f, 2, 0, 0);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, stash, 6000, 0, 0, 0);
	AI::_0xFD45175A6DFD7CE9(0, PLAYER::PLAYER_PED_ID(), 3, 0, -999.0f, -1, 0); // FLEE
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::CLEAR_PED_TASKS(target, 1, 1);
	AI::TASK_PERFORM_SEQUENCE(target, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);
}*/
