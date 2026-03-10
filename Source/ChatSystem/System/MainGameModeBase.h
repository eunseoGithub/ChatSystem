#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class AMainPlayerController;

UCLASS()
class CHATSYSTEM_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	FString GenerateSecretNumber();
	
	void OnPostLogin(AController* NewPlayer) override;
	
protected:
	TArray<TObjectPtr<AMainPlayerController>> AllPlayerController;
};
