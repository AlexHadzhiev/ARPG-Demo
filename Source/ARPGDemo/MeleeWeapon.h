#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "EnemyCharacter.h"

#include "MeleeWeapon.generated.h"


UCLASS()
class ARPGDEMO_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Attack(AActor* Target, AActor* PlayerActor) override;
};
