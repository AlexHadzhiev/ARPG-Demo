#include "MeleeWeapon.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "PlayerCharacterController.h"

AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bShootsProjectiles = false;
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeapon::Attack(AActor* Target, AActor* PlayerActor)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);
	if (IsValid(Enemy) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::Attack IsValid(Enemy) == false"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerActor);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::Attack IsValid(Player) == false"));
		return;
	}

	AController* Controller = Player->GetController();
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::Attack IsValid(Controller) == false"));
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::Attack IsValid(PlayerController) == false"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Attacked with %s"), *WeaponName);

	FDamageEvent DamageEvent(UDamageType::StaticClass());
	Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);
}