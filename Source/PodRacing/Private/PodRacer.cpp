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


	//Create blaster locations
	BlasterLocations.Add(CreateDefaultSubobject<USceneComponent>("Blaster location 1"));
	BlasterLocations[0]->SetupAttachment(PodMesh);
	BlasterLocations.Add(CreateDefaultSubobject<USceneComponent>("Blaster location 2"));
	BlasterLocations[1]->SetupAttachment(PodMesh);
	
	//Cretae visual pod mesh
	PodMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pod"));
	PodMesh->SetupAttachment(PodRoot);
}

//Called before tick
void APodRacer::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	//Sets engine positions
	LeftEngineParent->SetRelativeLocation(FVector(EngineParentOffset.X, EngineParentOffset.Y,EngineParentOffset.Z));
	LeftEngine->SetRelativeLocation(FVector(EngineOffset.X, -EngineOffset.Y,EngineOffset.Z));
	
	RightEngineParent->SetRelativeLocation(FVector(EngineParentOffset.X, -EngineParentOffset.Y,EngineParentOffset.Z));
	RightEngine->SetRelativeLocation(FVector(EngineOffset.X, EngineOffset.Y,EngineOffset.Z));

}


// Called when the game starts or when spawned
void APodRacer::BeginPlay() {
	Super::BeginPlay();
	InitTransform = GetActorTransform();
	APawn* MyPawn = Cast<APawn>(GetOwner());
}

// Called every frame
void APodRacer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	PodSpeed = GetVelocity().Length() * 0.036;
	Hover();
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
	PodRoot->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
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
	FRotator TargetRotation;
	
	//Do line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams)) {
		//Debug line
		//DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);

		//Sets physics settings
		PodRoot->SetLinearDamping(GroundDrag);
		PodRoot->SetAngularDamping(GroundAngularDrag);
		PodRoot->SetEnableGravity(false);
		IsGrounded = true;

		//Calculates and sets new rotation
		TargetRotation = FMath::RInterpTo(GetActorRotation(), FRotationMatrix::MakeFromZX(HitResult.Normal, GetActorForwardVector()).Rotator(), GetWorld()->DeltaTimeSeconds, 3);

		//sets new hovering position
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.Location.Z + RideHeight), false, nullptr, ETeleportType::ResetPhysics);
	}
	else {
		//Sets physics settings
		PodRoot->SetLinearDamping(AirDrag);
		PodRoot->SetAngularDamping(AirAngularDrag);
		PodRoot->SetEnableGravity(true);
		IsGrounded = false;

		//Calculates and sets new rotation
		TargetRotation = FMath::RInterpTo(GetActorRotation(), FRotationMatrix::MakeFromZX(FVector::UpVector, GetActorForwardVector()).Rotator(), GetWorld()->DeltaTimeSeconds, 3);
	}

	//Sets the actor rotation 
	SetActorRotation(TargetRotation, ETeleportType::ResetPhysics);
}

//Adds the pods movement in all 4 directions
void APodRacer::PodMovement(FVector2D YawThrottleInput, FVector2D RollPitchInput) {

	if(!CanMove) return;
	
	CalculatedVelocity =
		(RollPitchInput.X * JoystickForce * GetActorRightVector() +
		RollPitchInput.Y * JoystickForce * GetActorForwardVector() +
		YawThrottleInput.Y * ForwardForce * GetActorForwardVector()) *
		GetWorld()->DeltaTimeSeconds;
	
	PodRoot->AddImpulse(IsGrounded ? CalculatedVelocity : FVector::Zero());
}

//Tilts the pod on mesh on the pitch and roll axis
void APodRacer::TiltPod(FVector2D RollPitchInput) {

	if(CanMove && Cast<APodRacingGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SpectatorModeEnabled) return;

	TiltRotation = FMath::RInterpTo(
		PodMesh->GetRelativeRotation(),
		FRotator(RollPitchInput.Y * PodPitch * -1,0,RollPitchInput.X * PodRoll),
		GetWorld()->GetDeltaSeconds(),
		3);

	PodMesh->SetRelativeRotation(TiltRotation, false, nullptr, ETeleportType::ResetPhysics);
}

//Controls the pods yaw 
void APodRacer::YawControl(FVector2D YawThrottleInput, FVector2D RollPitchInput) {

	if(!CanMove) return;

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

//Uses the blasters if equiped
void APodRacer::UseBlasters() {
	
}

void APodRacer::ChangeCamera(int NewCameraIndex)
{
	
}
