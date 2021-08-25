#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameMode.generated.h"

UCLASS()
class ARPGDEMO_API APlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APlayerGameMode();

	void Win();

	void Loss();
};
