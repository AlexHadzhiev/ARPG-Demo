#include "SplitArrowsAbility.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "PlayerCharacterController.h"
#include "Engine/World.h"

USplitArrowsAbility::USplitArrowsAbility()
{
	bIsInCooldown = false;

}

void USplitArrowsAbility::UseAbility(AActor* Target)
{
	UObject* PlayerObject = GetOuter();
	if (IsValid(PlayerObject) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(PlayerObject) == false"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerObject);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(Player) == false"));
		return;
	}

	AController* PC = Player->GetController();
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(PC) == false"));
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(PlayerController) == false"));
		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);
	if (IsValid(Enemy) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(Enemy) == false"));
		return;
	}

	FVector ForwardArrowLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 50.0f;
	FRotator PlayerRotation = Player->GetActorRotation();

	FVector RotatedLeftVector = PlayerRotation.RotateVector(FVector(0.0f, -50.0f, 0.0f));
	FVector RotatedRightVector = PlayerRotation.RotateVector(FVector(0.0f, 50.0f, 0.0f));

	FVector LeftArrowLocation = ForwardArrowLocation + RotatedLeftVector;
	FVector RightArrowLocation = ForwardArrowLocation + RotatedRightVector;

	FTransform ForwardTransform(PlayerRotation);
	ForwardTransform.SetLocation(ForwardArrowLocation);

	FTransform LeftTransform(PlayerRotation);
	LeftTransform.SetLocation(LeftArrowLocation);

	FTransform RightTransform(PlayerRotation);
	RightTransform.SetLocation(RightArrowLocation);

	ProjectileForward = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, ForwardTransform, Player, Player, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	ProjectileLeft = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, LeftTransform, Player, Player, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	ProjectileRight = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, RightTransform, Player, Player, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (IsValid(ProjectileForward) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(ProjectileForward) == false"));
		return;
	}
	if (IsValid(ProjectileLeft) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(ProjectileLeft) == false"));
		return;
	}
	if (IsValid(ProjectileRight) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USplitArrowsAbility::UseAbility IsValid(ProjectileRight) == false"));
		return;
	}

	ProjectileForward->SetTarget(Enemy);
	ProjectileLeft->SetTarget(Enemy);
	ProjectileRight->SetTarget(Enemy);

	ProjectileForward->FinishSpawning(ForwardTransform);
	ProjectileLeft->FinishSpawning(LeftTransform);
	ProjectileRight->FinishSpawning(RightTransform);

	FVector FireDirection = PlayerRotation.Vector();
	ProjectileForward->FireInDirection(FireDirection);
	ProjectileLeft->FireInDirection(FireDirection);
	ProjectileRight->FireInDirection(FireDirection);
}

void USplitArrowsAbility::SetInCooldown()
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