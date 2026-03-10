#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxVolume;

	
	
};
