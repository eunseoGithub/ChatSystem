#include "NumberBaseballWidget.h"
#include "Components/TextBlock.h"

void UNumberBaseballWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this,&ThisClass::OnInputCommitted);
	}
}

void UNumberBaseballWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UNumberBaseballWidget::AddGuessResult(const FString& Guess, int32 Strike, int32 Ball, bool bIsOut)
{
	if (!ChatBox) return;
	FString Result;
	
	if (bIsOut)
		Result = FString::Printf(TEXT("%s -> OUT"),*Guess);
	
	else
		Result = FString::Printf(TEXT("%s -> %dS %dB"), *Guess, Strike, Ball);
	UTextBlock* NewText = NewObject<UTextBlock>(this);
	if (!NewText) return;
	
	NewText->SetText(FText::FromString(Result));
	ChatBox->AddChild(NewText);
	ChatBox->ScrollToEnd();
	
}

void UNumberBaseballWidget::UpdateTimer(float RemainingTime)
{
	if (!TimerText) return;
	
	FString Time = FString::Printf(TEXT("%.0f초"), RemainingTime);
	TimerText->SetText(FText::FromString(Time));
}

void UNumberBaseballWidget::SetMyTurn(bool bIsMyTurn)
{
	if (TurnText)
	{
		TurnText->SetText(FText::FromString(bIsMyTurn ? TEXT("My Turn") : TEXT("Other Turn")));
	}
	
	if (ChatInput) ChatInput->SetIsEnabled(bIsMyTurn);
}

void UNumberBaseballWidget::OnWidgetOpened()
{
	if (!ChatInput) return;
	
	ChatInput->SetKeyboardFocus();
	
	TSharedPtr<SWidget> SlateWidget = ChatInput->GetCachedWidget();
	if (SlateWidget.IsValid())
	{
		FSlateApplication::Get().SetKeyboardFocus(SlateWidget, EFocusCause::SetDirectly);
	}
}

void UNumberBaseballWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString Input = ChatInput->GetText().ToString().TrimStartAndEnd();
		if (Input.IsEmpty()) return;
		
		//TODO : Test용 코드
		UTextBlock* NewText = NewObject<UTextBlock>(this);
		if (!NewText) return;
	
		NewText->SetText(FText::FromString(Input));
		ChatBox->AddChild(NewText);
		ChatBox->ScrollToEnd();
		
		ChatInput->SetText(FText::GetEmpty());
	}
}


