#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerState.h"
#include "ChatSystem/Interaction/NumberBaseballInteraction.h"
#include "ChatSystem/System/MainGameModeBase.h"
#include "ChatSystem/UI/NumberBaseballWidget.h"

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


void AMainPlayerController::ClientRPCConsumeAttempt_Implementation(int32 RemainingAttempt)
{
	if (!IsValid(CurrentInteractable)) return;
	UNumberBaseballWidget* Widget = CurrentInteractable->GetWidget();
	if (IsValid(Widget))
	{
		Widget->ConsumeAttempt(RemainingAttempt);
	}
}

void AMainPlayerController::ClientRPCSetTurn_Implementation(bool bIsMyTurn)
{
	if (!IsValid(CurrentInteractable)) return;
	UNumberBaseballWidget* Widget= CurrentInteractable->GetWidget();
	if (IsValid(Widget))
	{
		Widget->SetMyTurn(bIsMyTurn);
	}
}

void AMainPlayerController::ServerRPCChatMessage_Implementation(const FString& Message)
{
	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (!IsValid(PS)) return;
	
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GM)) return;
	
	FString SenderName = PS->GetPlayerName();
	
	GM->BroadcastChatMessage(SenderName, Message);
	
	if (!GM->IsValidGuess(Message))
	{
		ClientRPCReceiveChatMessage(TEXT("Server"),TEXT("다시 입력하세요."));
		return;
	}
	
	FString Result = GM->JudgeResult(GM->GetSecretNumber(),Message);
	GM->BroadcastChatMessage(TEXT("Server"),Result);
	
	PS->AttemptCount--;
	ClientRPCConsumeAttempt(PS->AttemptCount);
	
	if (Result == TEXT("3S0B"))
	{
		FString WinMessage = FString::Printf(TEXT("%s 승리!"), *SenderName);
		GM->BroadcastChatMessage(TEXT("Server"),WinMessage);
		GM->EndGame(SenderName);
		return;
	}
	
	if (GM->CheckAllAttemptsUsed())
	{
		GM->BroadcastChatMessage(TEXT("Server"), TEXT("무승부"));
		GM->EndGame(TEXT(""));
		return;
	}
	
	GM->SwitchTurn();
}

void AMainPlayerController::ClientRPCReceiveChatMessage_Implementation(const FString& SenderName,
	const FString& Message)
{
	if (!IsValid(CurrentInteractable)) return;
	UNumberBaseballWidget* Widget = CurrentInteractable->GetWidget();
	if (IsValid(Widget))
	{
		Widget->AddChatMessage(SenderName,Message);
	}
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
	
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GM)) return;
	
	if (IsValid(PS))
	{
		PS->SetPlayerName(InName);
		PS->bIsNameSet = true;
	}
	
	GM->OnPlayerNameWrite();
}
