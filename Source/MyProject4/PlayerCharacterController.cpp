#include "PlayerCharacterController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnemyCharacter.h"
#include "Weapon.h"
#include "Ability.h"

APlayerCharacterController::APlayerCharacterController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APlayerCharacterController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APlayerCharacterController::OnSetDestinationReleased);
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacterController::MoveToMouseCursor);
	InputComponent->BindAction("Ability", IE_Pressed, this, &APlayerCharacterController::ActivateAbilty);
	InputComponent->BindAction("Cancel", IE_Pressed, this, &APlayerCharacterController::CancelAttackOrAbility);
}

void APlayerCharacterController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	bool bIsEnemy = false;

	AActor* EnemyActor = Hit.GetActor();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyActor);

	if (IsValid(Enemy))
	{
		if (Enemy->ActorHasTag("Enemy"))
		{
			bIsEnemy = true;
		}
	}

	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::MoveToMouseCursor IsValid(PlayerPawn) == false"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (IsValid(PlayerCharacter) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::MoveToMouseCursor IsValid(PlayerCharacter) == false"));
		return;
	}

	if (Hit.bBlockingHit)
	{
		if (bIsEnemy)
		{
			float MaxDist;

			if (PlayerCharacter->bIsUsingAbility)
			{
				MaxDist = PlayerCharacter->Ability->MaxRange;
			}
			else
			{
				MaxDist = PlayerCharacter->Weapon->MaxRange;
			}

			float Dist = FVector::Dist(PlayerCharacter->GetActorLocation(), Enemy->GetActorLocation());

			if (Dist > MaxDist)
			{
				FVector Direction = Enemy->GetActorLocation() - PlayerCharacter->GetActorLocation();
				Direction.Normalize();
				
				FVector Destination = PlayerCharacter->GetActorLocation() + (Direction * ((Dist - (MaxDist)) + 50.0f));

				PlayerCharacter->EnemyTarget = Enemy;

				SetNewMoveDestination(Destination, bIsEnemy, Enemy);
			}
			else
			{
				this->Attack(Enemy);
			}
		}
		else
		{
			SetNewMoveDestination(Hit.ImpactPoint, bIsEnemy, nullptr);
		}
	}
}

void APlayerCharacterController::SetNewMoveDestination(const FVector DestLocation, bool bIsEnemy, AActor* Target)
{
	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::SetNewMoveDestination IsValid(PlayerPawn) == false"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (IsValid(PlayerCharacter) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::SetNewMoveDestination IsValid(PlayerCharacter) == false"));
		return;
	}

	if (bIsEnemy)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FRotator PlayerRotation = PlayerCharacter->GetActorRotation();

		FVector EnemyLocation = Target->GetActorLocation();

		FRotator NewPlayerRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EnemyLocation);
		FRotator ChangedRotation(PlayerRotation.Pitch, NewPlayerRotation.Yaw, PlayerRotation.Roll);

		PlayerCharacter->SetActorRotation(ChangedRotation);

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
	else
	{
		float const Distance = FVector::Dist(DestLocation, PlayerCharacter->GetActorLocation());

		if (Distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APlayerCharacterController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void APlayerCharacterController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void APlayerCharacterController::CancelAttackOrAbility()
{
	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::CancelAttackOrAbility IsValid(PlayerPawn) == false"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (IsValid(PlayerCharacter) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::CancelAttackOrAbility IsValid(PlayerCharacter) == false"));
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		if (PlayerCharacter->bIsUsingAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability Canceled"));
			PlayerCharacter->bIsUsingAbility = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack Canceled"));
		}

		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void APlayerCharacterController::ActivateAbilty()
{
	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::ActivateAbilty IsValid(PlayerPawn) == false"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (IsValid(PlayerCharacter) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::ActivateAbilty IsValid(PlayerCharacter) == false"));
		return;
	}

	PlayerCharacter->ActivateAbilty();
}

void APlayerCharacterController::Attack(AActor* Target)
{
	APawn* PlayerPawn = GetPawn();
	if (IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::Attack IsValid(PlayerPawn) == false"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (IsValid(PlayerCharacter) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::Attack IsValid(PlayerCharacter) == false"));
		return;
	}

	if (IsValid(Target) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacterController::Attack IsValid(Target) == false"));
		return;
	}

	bool bToSetInCooldown;

	float AttackTime;

	if (PlayerCharacter->bIsUsingAbility)
	{
		AttackTime = PlayerCharacter->Ability->CastingTime;
		bToSetInCooldown = true;
	}
	else
	{
		AttackTime = PlayerCharacter->Weapon->AttackSpeed;
		bToSetInCooldown = false;
	}

	if (!(GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle)))
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FRotator PlayerRotation = PlayerCharacter->GetActorRotation();

		FVector EnemyLocation = Target->GetActorLocation();

		FRotator NewPlayerRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EnemyLocation);
		FRotator ChangedRotation(PlayerRotation.Pitch, NewPlayerRotation.Yaw, PlayerRotation.Roll);

		PlayerCharacter->SetActorRotation(ChangedRotation);

		FTimerDelegate AttackDelegate;
		AttackDelegate.BindUFunction(PlayerCharacter, FName("Attack"), Target);

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackDelegate, AttackTime, false);

		if (bToSetInCooldown)
		{
			FTimerDelegate SetInCooldownDelegate;
			FTimerDelegate SetOutOfCooldownDelegate;

			SetInCooldownDelegate.BindUFunction(PlayerCharacter->Ability, FName("SetInCooldown"));
			SetOutOfCooldownDelegate.BindUFunction(PlayerCharacter->Ability, FName("SetInCooldown"));

			GetWorld()->GetTimerManager().SetTimer(PlayerCharacter->Ability->SetInCooldownHandle, SetInCooldownDelegate,
				PlayerCharacter->Ability->CastingTime, false);

			GetWorld()->GetTimerManager().SetTimer(PlayerCharacter->Ability->SetOutOfCooldownHandle, SetOutOfCooldownDelegate,
				(PlayerCharacter->Ability->Cooldown + PlayerCharacter->Ability->CastingTime), false);
		}
	}
}