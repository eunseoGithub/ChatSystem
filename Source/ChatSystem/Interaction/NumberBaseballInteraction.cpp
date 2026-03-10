#include "NumberBaseballInteraction.h"


ANumberBaseballInteraction::ANumberBaseballInteraction()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	
	BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	BoxVolume->SetupAttachment(StaticMesh);
}

void ANumberBaseballInteraction::BeginPlay()
{
	Super::BeginPlay();
	
}


