// Copyright Andrei Bondarenko 2023


#include "Characters/Enemies/Zombie.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"

AZombie::AZombie()
{
	DefaultAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("BaseAttributeSet");
}
