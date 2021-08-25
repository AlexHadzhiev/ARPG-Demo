#include "FireballAbility.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "PlayerCharacterController.h"
#include "Engine/World.h"

UFireballAbility::UFireballAbility()
{
	bIsInCooldown = false;

}

void UFireballAbility::UseAbility(AActor* Target)
{
	UObject* PlayerObject = GetOuter();
	if (IsValid(PlayerObject) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(PlayerObject) == false"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerObject);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(Player) == false"));
		return;
	}

	AController* PC = Player->GetController();
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(PC) == false"));
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(PlayerController) == false"));
		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);
	if (IsValid(Enemy) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(Enemy) == false"));
		return;
	}

	FVector FireLocation;
	FVector FireLocationOffset = FVector(50.0f, 0.0f, 0.0f);
	FRotator FireRotation;

	FVector PlayerLocation = Player->GetActorLocation();
	FRotator PlayerRotation = Player->GetActorRotation();

	FVector FireLocatonOffsetRotated = FTransform(PlayerRotation).TransformVector(FireLocationOffset);
	FireLocation = PlayerLocation + FireLocatonOffsetRotated;

	FireRotation = PlayerRotation;

	FTransform Transform(FireRotation);
	Transform.SetLocation(FireLocation);

	Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Player, Player, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(Projectile) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UFireballAbility::UseAbility IsValid(Projectile) == false"));
		return;
	}

	Projectile->SetTarget(Enemy);

	Projectile->FinishSpawning(Transform);

	FVector FireDirection = FireRotation.Vector();
	Projectile->FireInDirection(FireDirection);
}

void UFireballAbility::SetInCooldown()
{
	bIsInCooldown = !bIsInCooldown;

	if (bIsInCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Set in Cooldown for %.3f"), Cooldown);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Out of Cooldown"));
	}
}