#include "NumberBaseballInteraction.h"
#include "ChatSystem/ChatSystemCharacter.h"
#include "ChatSystem/Player/MainPlayerController.h"
#include "ChatSystem/UI/NumberBaseballWidget.h"
#include "Framework/Application/SlateApplication.h" 
#include "Components/WidgetInteractionComponent.h"
ANumberBaseballInteraction::ANumberBaseballInteraction()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	
	BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	BoxVolume->SetupAttachment(StaticMesh);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("GameWidgetComponent"));
	WidgetComponent->SetupAttachment(StaticMesh);
	
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetDrawSize(FVector2D(800.f, 600.f));
	
	WidgetComponent->SetVisibility(false);
	
	
	
}

void ANumberBaseballInteraction::BeginPlay()
{
	Super::BeginPlay();
	
	WidgetComponent->SetWindowFocusable(true);
	
	BoxVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlap);
	BoxVolume->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBoxEndOverlap);
	
}

void ANumberBaseballInteraction::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Log,TEXT("BeginOverlap"));
	if (ACharacter* Character = Cast<AChatSystemCharacter>(OtherActor))
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(Character->GetController());
		if (PC && PC->IsLocalController())
		{
			InteractController = PC;
			PC->SetCurrentIneractable(this);
		}
	}
}

void ANumberBaseballInteraction::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp,Log,TEXT("EndOverlap"));
	if (ACharacter* Character = Cast<AChatSystemCharacter>(OtherActor))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(InteractController))
		{
			PC->SetCurrentIneractable(nullptr);
		}
		InteractController = nullptr; 
	}
}

void ANumberBaseballInteraction::Interact(APlayerController* InstigatorController)
{
	bool bIsVisible = WidgetComponent->IsVisible();
	WidgetComponent->SetVisibility(!bIsVisible);

	AChatSystemCharacter* Char = Cast<AChatSystemCharacter>(InstigatorController->GetPawn());

	if (!bIsVisible)
	{
		UE_LOG(LogTemp,Log,TEXT("Interact On"));
		
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InstigatorController->SetShowMouseCursor(true);
		InstigatorController->SetInputMode(InputMode);

		if (Char && Char->WidgetInteraction)
		{
			Char->WidgetInteraction->SetActive(true);
		}

		if (UNumberBaseballWidget* Widget = Cast<UNumberBaseballWidget>(WidgetComponent->GetUserWidgetObject()))
		{
			Widget->OwnerInteraction = this;
		}
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("Interact Off"));
		InstigatorController->SetShowMouseCursor(false);
		InstigatorController->SetInputMode(FInputModeGameOnly());

		if (Char && Char->WidgetInteraction)
		{
			Char->WidgetInteraction->SetActive(false);
		}
	}
}


