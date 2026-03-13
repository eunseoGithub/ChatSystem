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
	
	UFUNCTION(Server, Reliable)
	void ServerRPCSetPlayerName(const FString& InName);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCSetWidgetOpen(bool bIsOpen);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCChatMessage(const FString& Message);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCReceiveChatMessage(const FString& SenderName, const FString& Message);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCSetTurn(bool bIsMyTurn);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCConsumeAttempt(int32 RemainingAttempt);
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
};
