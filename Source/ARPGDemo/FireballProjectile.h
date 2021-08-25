#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"

#include "FireballProjectile.generated.h"

UCLASS()
class ARPGDEMO_API AFireballProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AFireballProjectile();

	UPROPERTY(EditAnywhere)
	bool bOverpenetrates;

	UPROPERTY(EditAnywhere)
	float Lifetime;

	UPROPERTY()
	FTimerHandle DeathTimerHandle;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnBeginOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
