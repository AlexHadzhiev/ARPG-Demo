#include "EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Materials/Material.h"
#include "PlayerGameMode.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsDead = false;
	bIsBoss = false;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	this->OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::TakeAnyDamage);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const FVector WeaponLocationVector = FVector(0.0f, 0.0f, 0.0f);
	const FRotator WeaponRotationRotator = FRotator(0.0f, 0.0f, 0.0f);

	AActor* WeaponActor = GetWorld()->SpawnActor(WeaponClass, &WeaponLocationVector, &WeaponRotationRotator, ActorSpawnParameters);
	if (IsValid(WeaponActor) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::BeginPlay IsValid(WeaponActor) == false"));
		return;
	}

	Weapon = Cast<AWeapon>(WeaponActor);
	if (IsValid(Weapon) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::BeginPlay IsValid(Weapon) == false"));
		return;
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OnDeath(AActor* Attacker)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Attacker);
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::OnDeath IsValid(Player) == false"));
		return;
	}

	Player->Score += this->Score;
	
	bIsDead = true;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Killed"));
	UE_LOG(LogTemp, Warning, TEXT("Score: %.1f"), Player->Score);

	if (bIsBoss == true)
	{
		APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>();
		if (IsValid(GameMode) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::OnDeath IsValid(GameMode) == false"));
			return;
		}

		GameMode->Win();

		AController* PC = Player->GetController();
		if (IsValid(PC) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::OnDeath IsValid(PC) == false"));
			return;
		}

		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
		if (IsValid(PlayerController) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::OnDeath IsValid(PlayerController) == false"));
			return;
		}

		PlayerController->ConsoleCommand("quit");
	}

	this->Destroy();
}

void AEnemyCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	this->Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Took Damage - %.1f"), Damage);

	if (Health <= 0)
	{
		this->OnDeath(DamageCauser);
	}
}