// AILEX 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h" // For the trigger volume. 
#include "TollBarrierOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLEDYNAMICS_API UTollBarrierOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTollBarrierOpen();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenGate(float DeltaTime);
	void CloseGate(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	// For ensuring the sound effect only occurs for once. 
	bool GateOpenAlready = false;
	bool GateCloseAlready = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float InitialPitch;
	float CurrentPitch;

	UPROPERTY(EditAnywhere)
	float MassToOpenGate = 50.0;

	UPROPERTY(EditAnywhere) // So we can see TargetYaw in editor. 
	float OpenAngle = 75.0;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;

	float GateLastOpened = 0.0; // Please initialize this otherwise it will only be assigned when player enters the trigger volume. 

	UPROPERTY(EditAnywhere)
	float GateCloseDelay = 0.5;

	UPROPERTY(EditAnywhere)
	float GateOpenSpeed = 0.8;

	UPROPERTY(EditAnywhere)
	float GateCloseSpeed = 2.0;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

};
