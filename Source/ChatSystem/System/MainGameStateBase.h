#pragma once

#include "CoreMinimal.h"
#include "MainGameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameStateBase.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, ENumberBaseballPhase, NewPhase);
UCLASS()
class CHATSYSTEM_API AMainGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase)
	ENumberBaseballPhase GamePhase = ENumberBaseballPhase::Waiting;
	
	UPROPERTY(BlueprintAssignable)
	FOnGamePhaseChanged OnGamePhaseChanged;
	
	UFUNCTION()
	void OnRep_GamePhase(); // 서버에서 GamePhase 값이 바뀌어서 클라이언트에 복제되었을때 호출되는 콜백함수
	
	//어떤 변수를 복제할건지 등록하는 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostNetInit() override;
};
