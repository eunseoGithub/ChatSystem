#include "MainPlayerState.h"
#include "Net/UnrealNetwork.h"
void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerState, bIsWidgetOpen);
	DOREPLIFETIME(AMainPlayerState, bIsNameSet);
}
