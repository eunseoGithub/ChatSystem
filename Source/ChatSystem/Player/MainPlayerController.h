#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class CHATSYSTEM_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UNumberBaseballWidget> NumberBaseballWidgetClass;

	UPROPERTY()
	TObjectPtr<UNumberBaseballWidget> NumberBaseballWidget;
};
