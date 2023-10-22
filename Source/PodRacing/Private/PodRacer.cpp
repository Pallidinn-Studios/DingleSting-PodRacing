// Fill out your copyright notice in the Description page of Project Settings.


#include "PodRacer.h"

#include "EnhancedInputComponent.h"
#include "HoverComponent.h"
#include "MovieSceneSequenceID.h"
#include "ToolContextInterfaces.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PodRacing/PodRacingGameMode.h"
#include "ProfilingDebugging/BootProfiling.h"

// Sets default values
APodRacer::APodRacer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionParams.AddIgnoredActor(this);
	
	//Create root
	PodRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = PodRoot;

	//Cretae visual pod mesh
	PodMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pod"));
	PodMesh->SetupAttachment(PodRoot);

	//Create left engine
	LeftEngineParent = CreateDefaultSubobject<USceneComponent>(TEXT("Left engine parent"));
	LeftEngineParent->SetupAttachment(PodRoot);
	LeftEngine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left engine"));
	LeftEngine->SetupAttachment(LeftEngineParent);

	//Create right engine 
	RightEngineParent = CreateDefaultSubobject<USceneComponent>(TEXT("Right engine parent"));
	RightEngineParent->SetupAttachment(PodRoot);
	RightEngine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right engine"));
	RightEngine->SetupAttachment(RightEngineParent);
}

// Called when the game starts or when spawned
void APodRacer::BeginPlay() {
	Super::BeginPlay();
	InitTransform = GetActorTransform();
}

// Called every frame
void APodRacer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	PodSpeed = GetVelocity().Length() * 0.036;
	Hover();

	

	//GEngine->AddOnScreenDebugMessage(1, GetWorld()->DeltaTimeSeconds, FColor::Blue, FString::FromInt(APodRacingGameMode::LapTime));
}

// Called to bind functionality to input
void APodRacer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//Sets the player back to the start position
void APodRacer::RestartGame() {
	//Resets position and rotation
	PodRoot->SetPhysicsLinearVelocity(FVector::Zero());
	SetActorTransform(InitTransform);

	//Resets timerS
	LapTimes.Empty();
	CurrentLapTime = GetWorld()->GetTimeSeconds();
	GameStartTime = GetWorld()->GetTimeSeconds();

}

//Keeps the pod at the correct height above the ground
void APodRacer::Hover() {
	//Calculate start and end pos
	Start = FVector(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z - 150);
	End = Start + (FVector(0,0,-RideHeight + 100));
	
	//Do line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams)) {

		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);

		PodRoot->SetLinearDamping(GroundDrag);
		PodRoot->SetAngularDamping(GroundAngularDrag);
		PodRoot->SetEnableGravity(false);
		IsGrounded = true;

		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.Location.Z + RideHeight), false, nullptr, ETeleportType::ResetPhysics);
	}
	else {
		PodRoot->SetLinearDamping(AirDrag);
		PodRoot->SetAngularDamping(AirAngularDrag);
		PodRoot->SetEnableGravity(true);
		IsGrounded = false;
	}
}

//Adds the pods movement in all 4 directions
void APodRacer::PodMovement(FVector2D YawThrottleInput, FVector2D RollPitchInput) {

	CalculatedVelocity =
		(RollPitchInput.X * JoystickForce * GetActorRightVector() +
		RollPitchInput.Y * JoystickForce * GetActorForwardVector() +
		YawThrottleInput.Y * ForwardForce * GetActorForwardVector()) *
		GetWorld()->DeltaTimeSeconds;
	
	PodRoot->AddImpulse(IsGrounded ? CalculatedVelocity : FVector::Zero());
}

//Tilts the pod on mesh on the pitch and roll axis
void APodRacer::TiltPod(FVector2D RollPitchInput) {

	TiltRotation = FMath::RInterpTo(
		PodMesh->GetRelativeRotation(),
		FRotator(RollPitchInput.Y * PodPitch * -1,0,RollPitchInput.X * PodRoll),
		GetWorld()->GetDeltaSeconds(),
		3);

	PodMesh->SetRelativeRotation(TiltRotation, false, nullptr, ETeleportType::ResetPhysics);
}

//Controls the pods yaw 
void APodRacer::YawControl(FVector2D YawThrottleInput, FVector2D RollPitchInput) {
	
	PodRoot->AddAngularImpulseInDegrees(FVector(
		0,
		0,
		(YawThrottleInput.X + RollPitchInput.X) * GlobalControlSensitivity),
		"None", true);
}

//Adds a new lap time to the list
void APodRacer::AddLapTime() {
	LapTimes.Add(GetWorld()->GetTimeSeconds() - CurrentLapTime);

	CurrentLapTime = GetWorld()->GetTimeSeconds();
}








