#include "NumberBaseballWidget.h"

#include "Components/TextBlock.h"

void UNumberBaseballWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextComitted) == false)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextComitted);
	}
}

void UNumberBaseballWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (ChatInput->OnTextCommitted.IsAlreadyBound(this,&ThisClass::OnChatInputTextComitted) == true)
	{
		ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextComitted);
	}
}

void UNumberBaseballWidget::OnChatInputTextComitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString Message = Text.ToString().TrimStartAndEnd();
		if (Message.IsEmpty())
		{
			return;
		}
		
		AddChatLog(Message);
		if (ChatInput)
		{
			ChatInput->SetText(FText::GetEmpty());
		}
	}
	
}

void UNumberBaseballWidget::AddChatLog(const FString& Message)
{
	if (!ChatBox) return;
	
	UTextBlock* NewText = NewObject<UTextBlock>(this);
	if (!NewText) return;
	
	NewText->SetText(FText::FromString(Message));
	ChatBox->AddChild(NewText);
	ChatBox->ScrollToEnd();
}
