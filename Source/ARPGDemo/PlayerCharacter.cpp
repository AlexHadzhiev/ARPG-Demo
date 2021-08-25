#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnemyCharacter.h"
#include "PlayerGameMode.h"
#include "DrawDebugHelpers.h"
#include "Ability.h"
#include "PlayerCharacterController.h"

APlayerCharacter::APlayerCharacter()
{
	Health = 100.0f;
	Score = 0.0f;
	SpeedModifier = 0.0f;

	bIsUsingAbility = false;
	bIsDead = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetCTW(TEXT("Material'/Game/Blueprints/M_Cursor_Decal_World.M_Cursor_Decal_World'"));
	if (DecalMaterialAssetCTW.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAssetCTW.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	CursorToEnemy = CreateDefaultSubobject<UDecalComponent>("CursorToEnemy");
	CursorToEnemy->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetCTE(TEXT("Material'/Game/Blueprints/M_Cursor_Decal_Enemy.M_Cursor_Decal_Enemy'"));
	if (DecalMaterialAssetCTE.Succeeded())
	{
		CursorToEnemy->SetDecalMaterial(DecalMaterialAssetCTE.Object);
	}
	CursorToEnemy->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToEnemy->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	CursorAbility = CreateDefaultSubobject<UDecalComponent>("CursorAbility");
	CursorAbility->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetCA(TEXT("Material'/Game/Blueprints/M_Cursor_Decal_Ability.M_Cursor_Decal_Ability'"));
	if (DecalMaterialAssetCA.Succeeded())
	{
		CursorAbility->SetDecalMaterial(DecalMaterialAssetCA.Object);
	}
	CursorAbility->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorAbility->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	CursorToWorld->SetVisibility(true, false);
	CursorToEnemy->SetVisibility(false, false);
	CursorAbility->SetVisibility(false, false);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WeaponRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Sphere Collision"));
	AbilityRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Ability Sphere Collision"));

	this->OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::TakeAnyDamage);

	WeaponRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::WeaponInRangeOverlap);
	WeaponRangeSphere->SetupAttachment(RootComponent);
	AbilityRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AbilityInRangeOverlap);
	AbilityRangeSphere->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.Owner = this;
	const FVector WeaponLocationVector = FVector(0.0f, 0.0f, 0.0f);
	const FRotator WeaponRotationRotator = FRotator(0.0f, 0.0f, 0.0f);

	AActor* WeaponActor = GetWorld()->SpawnActor(WeaponClass, &WeaponLocationVector, &WeaponRotationRotator, ActorSpawnParameters);
	if (IsValid(WeaponActor) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::BeginPlay IsValid(WeaponActor) == false"));
		return;
	}

	Weapon = Cast<AWeapon>(WeaponActor);
	if (IsValid(Weapon) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::BeginPlay IsValid(Weapon) == false"));
		return;
	}

	Ability = NewObject<UAbility>(this, AbilityClass);
	if (IsValid(Ability) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::BeginPlay IsValid(Ability) == false"));
		return;
	}

	WeaponRangeSphere->SetSphereRadius(Weapon->MaxRange);
	AbilityRangeSphere->SetSphereRadius(Ability->MaxRange);

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	float MovementMod = Movement->MaxWalkSpeed * SpeedModifier;
	Movement->MaxWalkSpeed += MovementMod;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(CursorToWorld) && IsValid(CursorToEnemy) && IsValid(CursorAbility))
	{
		AController* PC= this->GetController();
		if (IsValid(PC) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::Tick IsValid(PC) == false"));
			return;
		}

		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);

		if (IsValid(PlayerController))
		{
			FHitResult TraceHitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

			AActor* HitActor = TraceHitResult.GetActor();
			if (IsValid(HitActor))
			{
				if (HitActor->ActorHasTag("Enemy"))
				{
					CursorToWorld->SetVisibility(false, false);
					CursorToEnemy->SetVisibility(true, false);
					CursorAbility->SetVisibility(false, false);
				}
				else if (bIsUsingAbility && !(HitActor->ActorHasTag("Enemy")))
				{
					CursorToWorld->SetVisibility(false, false);
					CursorToEnemy->SetVisibility(false, false);
					CursorAbility->SetVisibility(true, false);
				}
				else
				{
					CursorToWorld->SetVisibility(true, false);
					CursorToEnemy->SetVisibility(false, false);
					CursorAbility->SetVisibility(false, false);
				}
			}

			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);

			CursorToEnemy->SetWorldLocation(TraceHitResult.Location);
			CursorToEnemy->SetWorldRotation(CursorR);

			CursorAbility->SetWorldLocation(TraceHitResult.Location);
			CursorAbility->SetWorldRotation(CursorR);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::ActivateAbilty()
{
	if (Ability->bIsInCooldown == false)
	{
		if (bIsUsingAbility == false)
		{
			if (Ability->bIsInstant)
			{
				Ability->UseAbility();
				return;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Ability Activated"));
			}
			bIsUsingAbility = !bIsUsingAbility;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability Deactivated"));
			bIsUsingAbility = !bIsUsingAbility;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability in Cooldown"));
	}
}

void APlayerCharacter::Attack(AActor* Target)
{
	if (IsValid(CursorToWorld) && IsValid(CursorToEnemy) && IsValid(CursorAbility))
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Target);

		if (IsValid(Enemy))
		{
			if (Enemy->ActorHasTag("Enemy"))
			{
				if (bIsUsingAbility)
				{
					Ability->UseAbility(Enemy);
					bIsUsingAbility = !bIsUsingAbility;
				}
				else
				{
					Weapon->Attack(Enemy, this);
				}

				(Cast<APlayerCharacterController>(GetController()))->bIsAttacking = false;
			}
		}
	}
}

void APlayerCharacter::OnDeath()
{
	this->bIsDead = true;

	UE_LOG(LogTemp, Error, TEXT("Player Died"));

	APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>();
	if (IsValid(GameMode) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnDeath IsValid(GameMode) == false"));
		return;
	}

	GameMode->Loss();

	AController* PC = this->GetController();
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnDeath IsValid(PC) == false"));
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnDeath IsValid(PlayerController) == false"));
		return;
	}

	PlayerController->ConsoleCommand("quit");
}

void APlayerCharacter::TakeAnyDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	this->Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Player Took Damage - %.1f"), Damage);

	if (this->Health <= 0)
	{
		this->OnDeath();
	}
}

void APlayerCharacter::WeaponInRangeOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (IsValid(this->EnemyTarget) == false)
		{
			return;
		}

		if (OtherActor == this->EnemyTarget)
		{
			AController* PC = GetController();
			if (IsValid(PC) == false)
			{
				return;
			}

			APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
			if (IsValid(PlayerController) == false)
			{
				return;
			}

			PlayerController->Attack(OtherActor);
		}
		else
		{
			return;
		}
	}
}

void APlayerCharacter::AbilityInRangeOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (IsValid(this->EnemyTarget) == false)
		{
			return;
		}

		if (OtherActor == this->EnemyTarget)
		{
			AController* PC = GetController();
			if (IsValid(PC) == false)
			{
				return;
			}

			APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
			if (IsValid(PlayerController) == false)
			{
				return;
			}

			PlayerController->Attack(OtherActor);
		}
		else
		{
			return;
		}
	}
}