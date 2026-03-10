#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include  "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "NumberBaseballWidget.generated.h"

UCLASS()
class CHATSYSTEM_API UNumberBaseballWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:
	UFUNCTION()
	void OnChatInputTextComitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void AddChatLog(const FString& Message);
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatBox; 
};
