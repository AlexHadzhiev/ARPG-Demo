#include "RangedWeapon.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "PlayerCharacterController.h"
#include "Engine/World.h"

ARangedWeapon::ARangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bShootsProjectiles = true;
}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangedWeapon::Attack(AActor* Target, AActor* PlayerActor)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);
	if (IsValid(Enemy) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangedWeapon::Attack IsValid(Enemy) == false"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerActor);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangedWeapon::Attack IsValid(Player) == false"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Attacked with %s"), *WeaponName);

	FVector FireLocation;
	FVector FireLocationOffset = FVector(80.0f, 0.0f, 0.0f);
	FRotator FireRotation;

	FVector PlayerLocation = Player->GetActorLocation();
	FRotator PlayerRotation = Player->GetActorRotation();

	FVector FireLocatonOffsetRotated = FTransform(PlayerRotation).TransformVector(FireLocationOffset);
	FireLocation = PlayerLocation + FireLocatonOffsetRotated;
	
	FireRotation = PlayerRotation;

	FTransform Transform(FireRotation);
	Transform.SetLocation(FireLocation);
	Transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Player, Player, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(Projectile) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ARangedWeapon::Attack IsValid(Projectile) == false"));
		return;
	}

	Projectile->SetTarget(Enemy);

	Projectile->FinishSpawning(Transform);

	FVector FireDirection = FireRotation.Vector();
	Projectile->FireInDirection(FireDirection);
}