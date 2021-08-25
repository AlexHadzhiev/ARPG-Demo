#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"


#include "PlayerCharacterController.generated.h"

UCLASS()
class ARPGDEMO_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	APlayerCharacterController();

	UFUNCTION()
	void Attack(AActor* Target);

	UPROPERTY()
	bool bIsAttacking;

private:
	uint32 bMoveToMouseCursor : 1;

	FTimerHandle AttackTimerHandle;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void MoveToMouseCursor();

	UFUNCTION()
	void SetNewMoveDestination(const FVector Destination, bool bIsEnemy, AActor* Target);

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void ActivateAbilty();

	UFUNCTION()
	void CancelAttackOrAbility();
};
