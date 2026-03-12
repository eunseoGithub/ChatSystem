#include "NumberBaseballWidget.h"
#include "ChatSystem/Player/MainPlayerController.h"
#include "ChatSystem/Player/MainPlayerState.h"
#include "ChatSystem/System/MainGameStateBase.h"
#include "Components/TextBlock.h"

void UNumberBaseballWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AMainGameStateBase* GS = GetWorld()->GetGameState<AMainGameStateBase>();
	if (IsValid(GS))
	{
		GS->OnGamePhaseChanged.AddDynamic(this, &UNumberBaseballWidget::OnGamePhaseChanged);
	}
	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this,&ThisClass::OnInputCommitted);
	}
}

void UNumberBaseballWidget::NativeDestruct()
{
	Super::NativeDestruct();
	AMainGameStateBase* GS = GetWorld()->GetGameState<AMainGameStateBase>();
	if (IsValid(GS))
	{
		GS->OnGamePhaseChanged.RemoveDynamic(this, &UNumberBaseballWidget::OnGamePhaseChanged);
	}
}


void UNumberBaseballWidget::OnGamePhaseChanged(ENumberBaseballPhase NewPhase)
{
	UE_LOG(LogTemp, Log, TEXT("BroadCast is on"));
	switch (NewPhase)
	{
	case ENumberBaseballPhase::Waiting:
		ChatInput->SetIsEnabled(false);
		TurnText->SetText(FText::FromString("Waiting..."));
		break;
	case ENumberBaseballPhase::Playing:
		ChatInput->SetIsEnabled(true);
		TurnText->SetText(FText::FromString("GameStart"));
		break;
	case ENumberBaseballPhase::GameOver:
		ChatInput->SetIsEnabled(false);
		TurnText->SetText(FText::FromString("Game End"));
		break;
	}
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
	UTextBlock* NewText = NewObject<UTextBlock>(this);
	if (!NewText) return;
	UE_LOG(LogTemp, Log, TEXT("이름을 입력해주세요."));
	NewText->SetText(FText::FromString("Write your name!!"));
	ChatBox->AddChild(NewText);
	ChatBox->ScrollToEnd();
}

void UNumberBaseballWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString Input = ChatInput->GetText().ToString().TrimStartAndEnd();
		if (Input.IsEmpty()) return;
		
		//플레이어 이름 넘겨주기
		AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwningPlayer());
		if (!IsValid(PC)) return;
		
		AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
		if (!IsValid(PS)) return;
		
		FString ChatText = "";
		if (!PS->bIsNameSet)
		{
			PS->bIsNameSet = true;
			PC->ServerRPCSetPlayerName(Input);
			
			UTextBlock* NewText = NewObject<UTextBlock>(this);
			if (!NewText) return;
			
			ChatText = Input + " is your name.";
			
		}
		else
		{
			ChatText = Input;
		}
		
		UTextBlock* NewText = NewObject<UTextBlock>(this);
		if (!NewText) return;
				
		NewText->SetText(FText::FromString(ChatText));
		ChatBox->AddChild(NewText);
		ChatBox->ScrollToEnd();
			
		ChatInput->SetText(FText::GetEmpty());
	}
}
