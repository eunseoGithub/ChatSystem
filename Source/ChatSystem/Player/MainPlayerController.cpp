#include "MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "ChatSystem/Interaction/NumberBaseballInteraction.h"
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::TryInteract);
	}
}

void AMainPlayerController::TryInteract()
{
	UE_LOG(LogTemp, Log, TEXT("TryInteract"));
	if (!IsLocalController()) return;
	
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}

void AMainPlayerController::SetCurrentIneractable(ANumberBaseballInteraction* Interactable)
{
	CurrentInteractable =  Interactable;
}
