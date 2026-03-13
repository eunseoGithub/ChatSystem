#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/UI/NumberBaseballWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "NumberBaseballInteraction.generated.h"

UCLASS()
class CHATSYSTEM_API ANumberBaseballInteraction : public AActor
{
	GENERATED_BODY()

public:
	ANumberBaseballInteraction();
	UNumberBaseballWidget* GetWidget();
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
						   AActor* OtherActor,
						   UPrimitiveComponent* OtherComp,
						   int32 OtherBodyIndex,
						   bool bFromSweep,
						   const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp,
						 AActor* OtherActor,
						 UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex);
	
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxVolume;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	void Interact(APlayerController* InstigatorController);
	
	UPROPERTY()
	TObjectPtr<APlayerController> InteractController;
};
