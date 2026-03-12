#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatSystem/System/MainGameModeBase.h"
#include  "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "NumberBaseballWidget.generated.h"

class ANumberBaseballInteraction;
UCLASS()
class CHATSYSTEM_API UNumberBaseballWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable)
	void AddGuessResult(const FString& Guess, int32 Strike, int32 Ball, bool bIsOut);
	
	UFUNCTION(BlueprintCallable)
	void UpdateTimer(float RemainingTime);
	
	UFUNCTION(BlueprintCallable)
	void SetMyTurn(bool bIsMyTurn);
	
	void OnWidgetOpened();
	
	UPROPERTY()
	TObjectPtr<ANumberBaseballInteraction> OwnerInteraction;
private:
	UFUNCTION()
	void OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnGamePhaseChanged(ENumberBaseballPhase NewPhase); // callback
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatBox; 
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TurnText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AttemptImage_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AttemptImage_2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AttemptImage_3;
private:
	ENumberBaseballPhase CachedGamePhase = ENumberBaseballPhase::Waiting;
};
