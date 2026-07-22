// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

class USpringArmComponent;
class UCameraComponent;

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	bUseControllerRotationYaw = false;	//컨트롤러 움직일 때 같이 회전되는 것 방지

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터 이동방향으로 바라보게 만들기
	CameraSpringArmComponent->bUsePawnControlRotation = true;	//스프링암은 컨트롤러 입력에 맞게 회전되기
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AActionCharacter::OnBoostAction);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AActionCharacter::OnBoostEnd);
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("TestAction 실행"));

}

void AActionCharacter::OnMoveAction(const FInputActionValue& Value)
{

	FVector2D MoveAxis = Value.Get<FVector2D>();

	FVector WorldDirection = FVector(MoveAxis.X, MoveAxis.Y, 0).GetSafeNormal();

	//카메라의 Yaw회전각(Degree)를 Radian으로 변경
	float YawRadian = FMath::DegreesToRadians(GetControlRotation().Yaw);

	//좌우 회전만 할꺼라 UPVector를 기준축으로 Yaw회전각 만큼 돌리는 회전 만들기
	FQuat ControlYawRoation(FVector::UpVector, YawRadian);

	//입력된 방향에 회전 적용(=카메라 Yaw회전 만큼 입력 방향을 회전 시키기)
	WorldDirection = ControlYawRoation.RotateVector(WorldDirection);

	AddMovementInput(WorldDirection);

	UE_LOG(LogTemp, Log, TEXT("(%.1f,%.1f)"), WorldDirection.X, WorldDirection.Y);


	//FRotator Rotator = GetControlRotation();
	//FRotator Rotator_Yaw(0,Rotator.Yaw,0);

	//// 그 방향 기준 앞쪽 / 오른쪽 벡터
	//FVector Forward = FRotationMatrix(Rotator_Yaw).GetScaledAxis(EAxis::X);                         // 앞 방향
	//FVector Right = FRotationMatrix(Rotator_Yaw).GetScaledAxis(EAxis::Y);  // 오른쪽 방향

	//AddMovementInput(Forward, MoveAxis.Y);  // W/S
	//AddMovementInput(Right, MoveAxis.X);  // A/D

	//UE_LOG(LogTemp, Log, TEXT("(%.1f,%.1f)"), MoveAxis.X, MoveAxis.Y);




}

void AActionCharacter::OnBoostAction(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
}

void AActionCharacter::OnBoostEnd(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

