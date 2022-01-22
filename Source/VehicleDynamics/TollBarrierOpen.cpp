// AILEX 2021

#include "TollBarrierOpen.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h" // Needed for the for loop of TotalMassOfActors().
#include "Engine/World.h"
#include "GameFrameWork/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTollBarrierOpen::UTollBarrierOpen() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTollBarrierOpen::BeginPlay() {
	Super::BeginPlay();

	CurrentPitch = GetOwner()->GetActorRotation().Yaw;
	CurrentPitch = InitialPitch;
	OpenAngle += InitialPitch;

	FindPressurePlate();
	FindAudioComponent();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UTollBarrierOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenGate) { // Only when the mass on the pressure plate is more than 50kg then it will open the door. 
		OpenGate(DeltaTime);

		// DoorLastOpened = When the door was open.
		GateLastOpened = GetWorld()->GetTimeSeconds();
	}

	else {
		// If the door has been open longer than DoorCloseDelta...
		if (GetWorld()->GetTimeSeconds() - GateLastOpened > GateCloseDelay) {
			CloseGate(DeltaTime);
		}
	}
}

void UTollBarrierOpen::OpenGate(float DeltaTime) {
	// Set the Rotation.
	CurrentPitch = FMath::Lerp(CurrentPitch, OpenAngle, DeltaTime * GateOpenSpeed); // Just change the rateOpen in order to change the rate of door opening. 
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Pitch = CurrentPitch; // Assign the changing value of CurrentYaw to the DoorRotation.Yaw in order to rotate it. 
	GetOwner()->SetActorRotation(DoorRotation);

	// For the Sound Effects.
	GateCloseSpeed = false;

	if (!AudioComponent) { return; }

	if (!GateOpenSpeed) {
		AudioComponent->Play(); // To play the sound.
		GateOpenSpeed = true;
	}
}


void UTollBarrierOpen::CloseGate(float DeltaTime)
{
	// Set the Rotation.
	CurrentPitch = FMath::Lerp(CurrentPitch, InitialPitch, DeltaTime * GateCloseSpeed); // Just change the rateOpen in order to change the rate of door opening. 
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Pitch = CurrentPitch; // Assign the changing value of CurrentYaw to the DoorRotation.Yaw in order to rotate it. 
	GetOwner()->SetActorRotation(DoorRotation);

	// For the Sound Effects.
	GateOpenSpeed = false; // Reset back the door opening check. 

	if (!AudioComponent) { return; }

	if (!GateCloseSpeed) {
		AudioComponent->Play(); // To play the sound. 
		GateCloseSpeed = true;
	}
}

void UTollBarrierOpen::FindAudioComponent() { // Cannot be const. Otherwise the AudioComponent will have error.
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}


void UTollBarrierOpen::FindPressurePlate() {
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set"), *GetOwner()->GetName());
	}
}

float UTollBarrierOpen::TotalMassOfActors() const {
	float TotalMass = 0.0;

	// Find all overlapping actors. 
	TArray<AActor*> OverlappingActors; // To store things. Its a pointer to that actors.
	if (!PressurePlate) { return TotalMass; } // To prevent crashing. 
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their Masses with for loop.
	for (AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate!"), *Actor->GetName()); 
	}

	return TotalMass;
}