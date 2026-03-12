#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class ANumberBaseballInteraction;

UCLASS()
class CHATSYSTEM_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void TryInteract();
	
	UPROPERTY()
	TObjectPtr<ANumberBaseballInteraction> CurrentInteractable;
public:
	void SetCurrentIneractable(ANumberBaseballInteraction* Interactable);
	
public:
	bool bWidgetOpen = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
};
