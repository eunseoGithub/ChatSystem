#include "MainPlayerController.h"

#include "ChatSystem/UI/NumberBaseballWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	if (!NumberBaseballWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("NumberBaseballWidgetClass is not set."));
		return;
	}

	NumberBaseballWidget = CreateWidget<UNumberBaseballWidget>(this, NumberBaseballWidgetClass);
	if (!NumberBaseballWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create NumberBaseballWidget."));
		return;
	}

	NumberBaseballWidget->AddToViewport();

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(NumberBaseballWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;
}
