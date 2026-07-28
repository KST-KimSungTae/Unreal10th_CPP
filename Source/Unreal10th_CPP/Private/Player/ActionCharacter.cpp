// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/StatComponent.h"
#include "AnimNotify/MyAnimNotifyState_SectionJump.h"
#include "Components/CapsuleComponent.h"
#include "Unreal10th_CPP/Unreal10th_CPP.h"

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

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));

	bUseControllerRotationYaw = false;	//컨트롤러 움직일 때 같이 회전되는 것 방지

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터 이동방향으로 바라보게 만들기
	CameraSpringArmComponent->bUsePawnControlRotation = true;	//스프링암은 컨트롤러 입력에 맞게 회전되기

	// 무기(ECC_Weapon)에 맞을 수 있게 (플레이어·적 공통)
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Overlap);
}

void AActionCharacter::SetSectionJumpNotify(UMyAnimNotifyState_SectionJump* InSectionJumpNotify)
{
	SectionJumpNotify = InSectionJumpNotify;
	bComboReady = SectionJumpNotify.IsValid();
}


// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	if (StatComponent)
	{
		FAutoRecoveryData Data = FAutoRecoveryData(
			StaminaRecoveryCoolTime,
			StaminaAutoRecoveryInterval,
			StaminaAutoRecoveryPerTick);
		StatComponent->InitializeStat(Data);
	}


	//GetCurrentStamina();	//실행했을 때 C++에 구현된 내용만 호출한다.
	//IStaminaInterface::Execute_GetCurrentStamina(this);	//실행했을때 블루프린트 구현으로 호출한다.
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpendSprintStamina(DeltaTime);
	//StaminaAutoRecovery(DeltaTime);
}


// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);

		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Started, this, &AActionCharacter::OnBoostAction);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AActionCharacter::OnBoostEnd);

		EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Started, this, &AActionCharacter::OnRollAction);
	
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AActionCharacter::OnAttackAction);
	
	}
}

UStatComponent* AActionCharacter::GetStatComponent_Implementation() const
{
	return StatComponent.Get();
}

void AActionCharacter::OnWeaponAttackState(bool bEnable)
{
	OnOnWeaponAttackStateChanged.ExecuteIfBound(bEnable);
}

float AActionCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UStatComponent* Stat = IStatInterface::Execute_GetStatComponent(this);
	if (Stat)
	{
		IStatInterface::Execute_Damaged(Stat, DamageAmount);   // OnHealthChange / OnDie 자동 브로드캐스트
	}
	return DamageAmount;
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

	//UE_LOG(LogTemp, Log, TEXT("(%.1f,%.1f)"), WorldDirection.X, WorldDirection.Y);


	//FRotator Rotator = GetControlRotation();
	//FRotator Rotator_Yaw(0,Rotator.Yaw,0);

	//// 그 방향 기준 앞쪽 / 오른쪽 벡터
	//FVector Forward = FRotationMatrix(Rotator_Yaw).GetScaledAxis(EAxis::X);                         // 앞 방향
	//FVector Right = FRotationMatrix(Rotator_Yaw).GetScaledAxis(EAxis::Y);  // 오른쪽 방향

	//AddMovementInput(Forward, MoveAxis.Y);  // W/S
	//AddMovementInput(Right, MoveAxis.X);  // A/D

	//UE_LOG(LogTemp, Log, TEXT("(%.1f,%.1f)"), MoveAxis.X, MoveAxis.Y);

}

void AActionCharacter::OnAttackAction(const FInputActionValue& Value)
{
	if (AnimInstance && IStaminaInterface::Execute_GetCurrentStamina(IStatInterface::Execute_GetStatComponent(this))>AttackCost)
	{
		OnWeaponAttackState(false);

		if (!AnimInstance->IsAnyMontagePlaying())
		{
			//첫번쨰 콤보 공격
			PlayAnimMontage(AttackMontage);
			IStaminaInterface::Execute_ConsumeStamina(IStatInterface::Execute_GetStatComponent(this), AttackCost);
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)
		{
			SectionJumpForCombo();
		}
	}
}

void AActionCharacter::OnBoostAction()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	bSprintMode = true;

}

void AActionCharacter::OnBoostEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	bSprintMode = false;


}

void AActionCharacter::OnRollAction(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("OnRollAction"));
	if (!RollMontage)return;
	//UE_LOG(LogTemp, Log, TEXT("OnRollAction - MontageValid"));


	if (!AnimInstance)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}

	if (IStaminaInterface::Execute_ConsumeStamina(StatComponent, RollStaminaCost))	//스테미너 소비 시도 후 소비되면 구르기 실행.
	{
		if (AnimInstance && !AnimInstance->IsAnyMontagePlaying())
		{
			if (!GetLastMovementInputVector().IsNearlyZero())	//이동 입력 중이면
			{
				SetActorRotation(GetLastMovementInputVector().Rotation());	//입력방향으로 즉신 회전해서 구르기
			}

			PlayAnimMontage(RollMontage.Get());
		}
	}
}

void AActionCharacter::SpendSprintStamina(float DeltaTime)
{
	//달리기 모드이고, 이동하고 있고, 몽타주 재생중이 아니면
	if (bSprintMode && !GetVelocity().IsNearlyZero() &&
		(AnimInstance && !AnimInstance->IsAnyMontagePlaying()))
	{
		if (!IStaminaInterface::Execute_ConsumeStamina(StatComponent, SprintStaminaCostPerSec * DeltaTime))
		{
			OnBoostEnd();
			UE_LOG(LogTemp, Log, TEXT("부스트 끝"));
		}
	}
}

void AActionCharacter::SectionJumpForCombo()
{
	if (SectionJumpNotify.IsValid() && bComboReady)
	{
		UAnimMontage* Current = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_SetNextSection(	//섹션을 변경한다.
			AnimInstance->Montage_GetCurrentSection(Current),	//이 섹션에서(from)
			SectionJumpNotify->GetNextSectionName(),			//이 섹션으로 변경(to)
			Current	//적용할 몽타주
		);

		IStaminaInterface::Execute_ConsumeStamina(IStatInterface::Execute_GetStatComponent(this), AttackCost);
		bComboReady = false;	//중복실행 방지
	}
}
