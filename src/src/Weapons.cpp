#include "Main.h";

void giveWeaponToPed(Ped ped, WeaponHash hash, int ammo, bool equipNow)
{
	if (!WEAPON::HAS_PED_GOT_WEAPON(ped, (Hash)hash, 0, 0))
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, (Hash)hash, ammo, false, false);
	}
	
	if (equipNow)
	{
		WEAPON::SET_CURRENT_PED_WEAPON(ped, (Hash)hash, true, 0, false, false);
	}
}

void pedEquipBestWeapon(Ped ped)
{
	auto bow = new Hash;
	int iWeapon = rand() % 2 + 1;
	if (PED::IS_PED_MODEL(ped, 2873015698) && !WEAPON::GET_CURRENT_PED_WEAPON(ped, bow, true, true, true))
	{
		if (iWeapon == 1)
		{
			WEAPON::SET_CURRENT_PED_WEAPON(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), true, 0, false, false);
		}
		else if (iWeapon == 2)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), false, 0);
			giveWeaponToPed(ped, WeaponBow, 0x38E6F55F, true);
		}
	}
	else if ((PED::IS_PED_MODEL(ped, 1057570823) || PED::IS_PED_MODEL(ped, 3939581308) || PED::IS_PED_MODEL(ped, 3666010779) || PED::IS_PED_MODEL(ped, 3204687205) || PED::IS_PED_MODEL(ped, 347599949) || PED::IS_PED_MODEL(ped, 50483426)) && !WEAPON::GET_CURRENT_PED_WEAPON(ped, bow, true, true, true))
	{
		if (iWeapon == 1)
		{
			WEAPON::SET_CURRENT_PED_WEAPON(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), true, 0, false, false);
		}
		else if (iWeapon == 2)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), false, 0);
			giveWeaponToPed(ped, RevolverSchofield, 0x64356159, true);
		}
	}
	else if (PED::IS_PED_MODEL(ped, 265884691) && !WEAPON::GET_CURRENT_PED_WEAPON(ped, bow, true, true, true))
	{
		if (iWeapon == 1)
		{
			giveWeaponToPed(ped, RepeaterWinchester, 0xDC8FB3E9, true);
		}
		else if (iWeapon == 2)
		{
			giveWeaponToPed(ped, RevolverSchofield, 0x64356159, true);
		}
	}
	else
	{
		WEAPON::SET_CURRENT_PED_WEAPON(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), true, 0, false, false);
	}
}