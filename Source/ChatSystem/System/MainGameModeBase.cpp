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
	TryStartGame();
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
			GS->GamePhase = ENumberBaseballPhase::Playing;
			GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::Playing);
			SecretNumber = GenerateSecretNumber();
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

void AMainGameModeBase::TryStartGame()
{
	if (AllPlayerController.Num() < RequirePlayerCount) return;
	//
	// AMainGameStateBase* GS = GetGameState<AMainGameStateBase>();
	// if (!IsValid(GS)) return;
	// SecretNumber = GenerateSecretNumber();
	//
	// UE_LOG(LogTemp, Log, TEXT("Game Started. SecretNumber : %s"), *SecretNumber);
	//
	// if (GS->GamePhase == ENumberBaseballPhase::Waiting)
	// {
	// 	GS->GamePhase = ENumberBaseballPhase::Playing;
	// 	GS->OnGamePhaseChanged.Broadcast(ENumberBaseballPhase::Playing);
	// }
}

FString AMainGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;
	
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

