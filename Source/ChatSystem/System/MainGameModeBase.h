#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class AMainPlayerController;

UENUM(BlueprintType)
enum class ENumberBaseballPhase : uint8
{
	Waiting, //플레이어 대기 중
	WriteName, // 이름 입력 시간
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
	
	//Player Login/Logout
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	//Widget Open/Close
	void OnPlayerWidgetOpened();
	void OnPlayerWidgetClosed(AMainPlayerController* ClosedPlayer);
	
	//플레이어 모두 이름 입력했으면 phase 변경
	void OnPlayerNameWrite();
	
	//Server에서 다시 채팅을 뿌려주는 함수
	void BroadcastChatMessage(const FString& SenderName, const FString& Message);
	
	//입력이 잘못되었나 확인
	bool IsValidGuess(const FString& Input) const;
	
	//숫자 판단
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	const FString& GetSecretNumber() const {return SecretNumber;}
	//턴 체인지
	void SwitchTurn();
protected:
	TArray<TObjectPtr<AMainPlayerController>> AllPlayerController;
	static constexpr int32 RequirePlayerCount = 2;
private:
	FString SecretNumber;
	int32 CurrentTurnIndex = 0;
};

