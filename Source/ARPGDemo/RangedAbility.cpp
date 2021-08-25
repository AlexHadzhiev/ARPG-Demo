#include "RangedAbility.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "PlayerCharacterController.h"

URangedAbility::URangedAbility()
{
	bIsInCooldown = false;
}

void URangedAbility::UseAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("Instant Ranged Ability Used"));
}

void URangedAbility::UseAbility(AActor* Target)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);
	if (IsValid(Enemy) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("URangedAbility::UseAbility IsValid(Enemy) == false"));
		return;
	}

	UObject* PlayerObject = this->GetOuter();
	if (IsValid(PlayerObject) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("URangedAbility::UseAbility IsValid(PlayerObject) == false"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerObject);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("URangedAbility::UseAbility IsValid(Player) == false"));
		return;
	}

	AController* Controller = Player->GetController();
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("URangedAbility::UseAbility IsValid(Controller) == false"));
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("URangedAbility::UseAbility IsValid(PlayerController) == false"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ranged Ability Used"));

	

	FDamageEvent DamageEvent(UDamageType::StaticClass());
	Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);
}

void URangedAbility::SetInCooldown()
{
	bIsInCooldown = !bIsInCooldown;

	if (bIsInCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Set in Cooldown"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Out of Cooldown"));
	}
}