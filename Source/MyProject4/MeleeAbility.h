#pragma once

#include "CoreMinimal.h"
#include "Ability.h"

#include "MeleeAbility.generated.h"

UCLASS()
class MYPROJECT4_API UMeleeAbility : public UAbility
{
	GENERATED_BODY()

public:
	UMeleeAbility();

	virtual void UseAbility() override;

	virtual void UseAbility(AActor* Target) override;

	virtual void SetInCooldown() override;
};
