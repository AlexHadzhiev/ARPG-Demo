#include "Ability.h"

UAbility::UAbility()
{
	bIsInCooldown = false;
	
}

void UAbility::UseAbility()
{

}

void UAbility::UseAbility(AActor* Target)
{

}

void UAbility::SetInCooldown()
{
	bIsInCooldown = !bIsInCooldown;

}