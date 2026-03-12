#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerState.h"
#include "ChatSystem/Interaction/NumberBaseballInteraction.h"
#include "ChatSystem/System/MainGameModeBase.h"

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
		AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
		if (IsValid(PS))
			PS->bIsWidgetOpen = !(PS->bIsWidgetOpen);
	}
}

void AMainPlayerController::SetCurrentIneractable(ANumberBaseballInteraction* Interactable)
{
	CurrentInteractable =  Interactable;
}

void AMainPlayerController::ServerRPCSetWidgetOpen_Implementation(bool bIsOpen)
{
	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (!IsValid(PS)) return;
	
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GM)) return;
	
	PS->bIsWidgetOpen = bIsOpen;
	
	if (bIsOpen)
		GM->OnPlayerWidgetOpened();
	else
		GM->OnPlayerWidgetClosed(this);
	
}

void AMainPlayerController::ServerRPCSetPlayerName_Implementation(const FString& InName)
{
	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (IsValid(PS))
	{
		PS->SetPlayerName(InName);
		PS->bIsNameSet = true;
	}
}
