#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

UCLASS()
class CHATSYSTEM_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	bool bIsNameSet = false;
	
	UPROPERTY(Replicated)
	bool bIsWidgetOpen = false;
	
	UPROPERTY(Replicated)
	int32 AttemptCount = 3;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
