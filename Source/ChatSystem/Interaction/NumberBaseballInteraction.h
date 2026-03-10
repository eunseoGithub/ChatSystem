#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NumberBaseballInteraction.generated.h"

UCLASS()
class CHATSYSTEM_API ANumberBaseballInteraction : public AActor
{
	GENERATED_BODY()

public:
	ANumberBaseballInteraction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
