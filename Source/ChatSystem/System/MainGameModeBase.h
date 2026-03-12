#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class AMainPlayerController;

UENUM(BlueprintType)
enum class ENumberBaseballPhase : uint8
{
	Waiting, //플레이어 대기 중
	Playing, // 게임 진행 중
	GameOver // 게임 종료
};

UCLASS()
class CHATSYSTEM_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainGameModeBase();
	FString GenerateSecretNumber();
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	void OnPlayerWidgetOpened();
	void OnPlayerWidgetClosed(AMainPlayerController* ClosedPlayer);
	
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
private:
	void TryStartGame();
protected:
	TArray<TObjectPtr<AMainPlayerController>> AllPlayerController;
	static constexpr int32 RequirePlayerCount = 2;
private:
	FString SecretNumber;
};
