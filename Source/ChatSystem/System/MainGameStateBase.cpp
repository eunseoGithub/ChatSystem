#include "MainGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AMainGameStateBase::OnRep_GamePhase()
{
	UE_LOG(LogTemp,Log,TEXT("OnRep_GamePhase"));
	OnGamePhaseChanged.Broadcast(GamePhase);
}

void AMainGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainGameStateBase, GamePhase);
}

void AMainGameStateBase::PostNetInit()
{
	Super::PostNetInit();
	UE_LOG(LogTemp,Error,TEXT("GameState 복제됨"));
	OnGamePhaseChanged.Broadcast(GamePhase);
}

