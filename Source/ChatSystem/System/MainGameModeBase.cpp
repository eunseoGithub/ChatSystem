#include "MainGameModeBase.h"
#include "MainGameStateBase.h"
#include "ChatSystem/Player/MainPlayerController.h"
#include "ChatSystem/Player/MainPlayerState.h"

AMainGameModeBase::AMainGameModeBase()
{
	GameStateClass = AMainGameStateBase::StaticClass();
}

FString AMainGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

void AMainGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(NewPlayer);
	if (IsValid(PlayerController))
	{
		AllPlayerController.Add(PlayerController);
	}
}

void AMainGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AMainPlayerController* PC = Cast<AMainPlayerController>(Exiting);
	if (IsValid(PC))
	{
		AllPlayerController.Remove(PC);
	}
}

void AMainGameModeBase::OnPlayerWidgetOpened()
{
	int32 OpenCount = 0;
	for (AMainPlayerController* PC : AllPlayerController)
	{
		AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
		if (IsValid(PS) && PS->bIsWidgetOpen)
			OpenCount++;
	}
	if (OpenCount >= RequirePlayerCount)
	{
		AMainGameStateBase* GS = GetGameState<AMainGameStateBase>();
		if (IsValid(GS) && GS->GamePhase == ENumberBaseballPhase::Waiting)
		{
			GS->GamePhase = ENumberBaseballPhase::WriteName;
			GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::WriteName);

		}
	}
}

void AMainGameModeBase::OnPlayerWidgetClosed(AMainPlayerController* ClosedPlayer)
{
	AMainGameStateBase* GS = GetGameState<AMainGameStateBase>();
	if (!IsValid(GS)) return;
	
	if (GS->GamePhase == ENumberBaseballPhase::Playing)
	{
		for (AMainPlayerController* PC : AllPlayerController)
		{
			if (PC != ClosedPlayer)
			{
				AMainPlayerState* WinnerPS = PC->GetPlayerState<AMainPlayerState>();
				//TODO : 게임 승리 , 패배 이름같은거 넘겨주기
			}
		}
		GS->GamePhase = ENumberBaseballPhase::GameOver;
		GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::GameOver);
		
	}
}

void AMainGameModeBase::OnPlayerNameWrite()
{
	int32 WriteName = 0;
	for (AMainPlayerController* PC : AllPlayerController)
	{
		AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
		if (IsValid(PS) && PS->bIsNameSet)
			WriteName++;
	}
	if (WriteName >= RequirePlayerCount)
	{
		AMainGameStateBase* GS = GetGameState<AMainGameStateBase>();
		if (IsValid(GS) && GS->GamePhase == ENumberBaseballPhase::WriteName)
		{
			GS->GamePhase = ENumberBaseballPhase::Playing;
			GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::Playing);
			//시크릿 넘버 생성
			SecretNumber = GenerateSecretNumber();
			UE_LOG(LogTemp,Error, TEXT("SecretNumber : %s"),*SecretNumber);
			
			CurrentTurnIndex = 0;
			for (int32 i = 0; i < AllPlayerController.Num(); i++)
			{
				if (IsValid(AllPlayerController[i]))
				{
					AllPlayerController[i]->ClientRPCSetTurn(i == 0);
				}
			}
			StartTurnTimer();
		}
	}
}

void AMainGameModeBase::BroadcastChatMessage(const FString& SenderName, const FString& Message)
{
	for (AMainPlayerController* PC : AllPlayerController)
	{
		if (IsValid(PC))
		{
			PC->ClientRPCReceiveChatMessage(SenderName,Message);	
		}
	}
}

bool AMainGameModeBase::IsValidGuess(const FString& Input) const
{
	if (Input.Len() != 3) return false; // 3자리
	
	TSet<TCHAR> UsedDigits;
	for (int32 i = 0; i < Input.Len(); i++)
	{
		TCHAR Ch = Input[i];
		if (Ch < '1' || Ch > '9') return false; // 0도 제외
		if (UsedDigits.Contains(Ch)) return false;//중복
		UsedDigits.Add(Ch);
	}
	return true;
}

void AMainGameModeBase::SwitchTurn()
{
	CurrentTurnIndex = (CurrentTurnIndex + 1) % AllPlayerController.Num();
	
	for (int32 i = 0 ; i < AllPlayerController.Num();i++)
	{
		if (IsValid(AllPlayerController[i]))
		{
			AllPlayerController[i]->ClientRPCSetTurn(i == CurrentTurnIndex);
		}
	}
	StartTurnTimer();
}

void AMainGameModeBase::EndGame(const FString& WinnerName)
{
	AMainGameStateBase* GS = GetGameState<AMainGameStateBase>();
	if (!IsValid(GS)) return;
	
	if (WinnerName.IsEmpty())
	{
		BroadcastChatMessage(TEXT("Server"),TEXT("무승부! 모든 기회 소진"));
	}
	else
	{
		FString Msg = FString::Printf(TEXT("%s 승리! 정답 : %s"), *WinnerName, *SecretNumber);
		BroadcastChatMessage(TEXT("Server"),Msg);
	}
	GS->GamePhase = ENumberBaseballPhase::GameOver;
	GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::GameOver);
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
}

bool AMainGameModeBase::CheckAllAttemptsUsed() const
{
	for (AMainPlayerController* PC : AllPlayerController)
	{
		if (!IsValid(PC)) continue;
		AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
		if (IsValid(PS) && PS->AttemptCount > 0)
			return false;
	}
	return true;
}

void AMainGameModeBase::StartTurnTimer()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().SetTimer(
		TurnTimerHandle, this, &ThisClass::OnTurnTimeout, TurnTimeLimit, false);
}

void AMainGameModeBase::OnTurnTimeout()
{
	if (!AllPlayerController.IsValidIndex(CurrentTurnIndex)) return;
	
	AMainPlayerController* CurrentPC = AllPlayerController[CurrentTurnIndex];
	AMainPlayerState* PS = CurrentPC->GetPlayerState<AMainPlayerState>();
	if (!IsValid(PS)) return;
	
	BroadcastChatMessage(TEXT("Server"),TEXT("시간 초과!"));
	
	PS->AttemptCount--;
	CurrentPC->ClientRPCConsumeAttempt(PS->AttemptCount);
	
	if (CheckAllAttemptsUsed())
	{
		BroadcastChatMessage(TEXT("Server"), TEXT("무승부"));
		EndGame(TEXT(""));
		return;
	}
	SwitchTurn();
}

FString AMainGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;
	UE_LOG(LogTemp,Error,TEXT("Secret : %s , Guess : %s"),*InSecretNumberString,*InGuessNumberString);
	for (int32 i = 0; i < 3;++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	return FString::Printf(TEXT("%dS%dB"),StrikeCount, BallCount);
}

