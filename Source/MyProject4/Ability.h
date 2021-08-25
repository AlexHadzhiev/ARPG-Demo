#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"

#include "Ability.generated.h"

UCLASS(Blueprintable)
class MYPROJECT4_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CastingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	bool bIsInstant;

	UPROPERTY()
	bool bIsInCooldown;

	UPROPERTY()
	FTimerHandle SetInCooldownHandle;

	UPROPERTY()
	FTimerHandle SetOutOfCooldownHandle;

	virtual void UseAbility();

	virtual void UseAbility(AActor* Target);

	UFUNCTION()
	virtual void SetInCooldown();
};
