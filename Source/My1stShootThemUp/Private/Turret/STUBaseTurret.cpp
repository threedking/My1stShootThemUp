// My 1st Unreal Shoot Them Up Game

#include "Turret/STUBaseTurret.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/STUProjectile.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "STUUtils.h"
#include "STUCoreTypes.h"
#include "Math/UnrealMathVectorCommon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseTurret, All, All);

ASTUBaseTurret::ASTUBaseTurret()
{
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");    
    CollisionComponent->InitSphereRadius(3000.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

    TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretBodyMesh");
    TurretHead = CreateDefaultSubobject<USceneComponent>("TurretHead");
    TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretHeadMesh");
    
    TurretBodyMesh->SetupAttachment(GetRootComponent());
    TurretHead->SetupAttachment(TurretBodyMesh);
    TurretHeadMesh->SetupAttachment(TurretHead);

    InitialSpeed = ASTUProjectile::GetInitialSpeed();
}

void ASTUBaseTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTUBaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    static float TimeElapsed = 0;

    if(TurretHeadMesh->GetComponentRotation() != TargetRotator)
    {
        if(TimeElapsed < LerpDuration)
        {
            TurretHeadMesh->SetWorldRotation(FMath::LerpRange<float>(TurretHeadMesh->GetComponentRotation(), TargetRotator, 0.1f));
            if(TurretHeadMesh->GetComponentRotation() == TargetRotator)
            {
                TimeElapsed = 0.0f;
            }
        }
    }
}

void ASTUBaseTurret::FireByBoomData() 
{
    auto SpawnPoint = TurretHeadMesh->GetComponentLocation();
    //SpawnPoint.Z += 500;
    FTransform SpawnTransform(FRotator::ZeroRotator, SpawnPoint);
    
    
    UE_LOG(LogBaseTurret, Display, TEXT("SpawnPoint : %s"), *SpawnPoint.ToCompactString());
    UE_LOG(LogBaseTurret, Display, TEXT("SpawnTrnsf : %s"), *SpawnTransform.GetTranslation().ToCompactString());

    
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        //UE_LOG(LogBaseTurret, Display, TEXT("BoomData.Direction : %s"), *BoomData.Direction.ToCompactString());
        
        Projectile->SetOwner(this);
        Projectile->SetShotDirection(BoomData.Direction2nd);
        Projectile->SetInitialSpeed(InitialSpeed);
        //Projectile->SetInitialSpeed(0);
        Projectile->FinishSpawning(SpawnTransform);
        
        UE_LOG(LogBaseTurret, Display, TEXT("SpawnPoint : %s"), *Projectile->GetActorLocation().ToCompactString());
    }
}

void ASTUBaseTurret::NotifyActorBeginOverlap(AActor* OtherActor) 
{
    //auto LookAtRotator = UKismetMathLibrary::FindLookAtRotation(TurretHeadMesh->GetComponentLocation(), OtherActor->GetActorLocation());
    //TurretHeadMesh->SetWorldRotation(LookAtRotator);
    UE_LOG(LogBaseTurret, Display, TEXT("NotifyActorBeginOverlap with : %s"), *OtherActor->GetName());

    auto EnemyProjectile = Cast<ASTUProjectile>(OtherActor);
    if(EnemyProjectile)
    {
        EnemyProjectile->UpdateComponentTransforms();
        if (!EnemyProjectile->GetOwner() || EnemyProjectile->GetOwner()->IsA(ASTUBaseTurret::StaticClass())) return;
        
        float CalculationTime = GetWorld()->GetRealTimeSeconds();        
        if(STUUtils::PointToHorizon::GetBoomData(BoomData, TurretHeadMesh->GetComponentLocation(), EnemyProjectile, InitialSpeed))
        {
            CalculationTime = GetWorld()->GetRealTimeSeconds() - CalculationTime;

            //TurretHeadMesh->SetWorldRotation(BoomData.Direction2nd.ToOrientationRotator());
            TargetRotator = BoomData.Direction2nd.ToOrientationRotator();
            LerpDuration = BoomData.Time1st - BoomData.Time2nd - CalculationTime;

            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTUBaseTurret ::FireByBoomData, LerpDuration, false);
        }
    }
}

bool ASTUBaseTurret::GetMyBoomData(ASTUProjectile* Projectile)
{
    /*
    if(!GetWorld()) return false;

    const float G = -UPhysicsSettings::Get()->DefaultGravityZ;
    auto TargetVelocityVector = Projectile->GetVelocity();
    

    float FarTime = 2.0f;
    float AdditionalTime = FarTime * 0.05f;
    for (float ChekingTime = FarTime * 0.1f; ChekingTime > 0.0f && ChekingTime <= FarTime; ChekingTime += AdditionalTime)
    {
        BoomData.Time1st = ChekingTime;
        
        BoomData.Point = Projectile->GetActorLocation();
        BoomData.Point.X += TargetVelocityVector.X * BoomData.Time1st;
        BoomData.Point.Y += TargetVelocityVector.Y * BoomData.Time1st;
        BoomData.Point.Z += TargetVelocityVector.Z * BoomData.Time1st - (G * BoomData.Time1st * BoomData.Time1st) / 2.0f;
        
        float DistanceToBoomPointXY = (BoomData.Point - TurretHeadMesh->GetComponentLocation()).Size2D();
        float DistanceToBoomPointZ = BoomData.Point.Z - TurretHeadMesh->GetComponentLocation().Z;
        
        float Tolerance = 0.01;
        float AlphaTimeMultiplier = 0.0f;
        float AlphaTimeAddentum = 0.1f;
        float LastDiff = 360.0f;
        float AngleZByXY{}, AngleZByZ{};
        int OutOfRangeCount = 0;

        while (true)
        {
            if(AlphaTimeMultiplier >= 1.0f) break;
            float MyXYTime = UKismetMathLibrary::Abs(DistanceToBoomPointXY) / (InitialSpeed - InitialSpeed * AlphaTimeMultiplier);
            float MyZTime = MyXYTime;
            if(MyXYTime > ChekingTime) break;
            
            float CosAngleXY = DistanceToBoomPointXY / (InitialSpeed * MyXYTime);            
            float SinAngleZ = (DistanceToBoomPointZ + (G * MyZTime * MyZTime) / 2.0f) / (InitialSpeed * MyZTime);
            if (CosAngleXY <= 1.0f && CosAngleXY >= -1.0f && SinAngleZ <= 1.0f && SinAngleZ >= -1.0f)
            {
                AngleZByXY = UKismetMathLibrary::DegAcos(CosAngleXY);
                AngleZByZ = UKismetMathLibrary::DegAsin(SinAngleZ);
            }
            else
            {
                if(CosAngleXY > 1.0f || CosAngleXY < -1.0f)
                {
                    if(OutOfRangeCount < 5)
                    {
                        OutOfRangeCount++;
                        AlphaTimeAddentum *= -1.0f;
                    }
                    else
                    {
                        break;
                    }
                }
                AlphaTimeMultiplier += AlphaTimeAddentum;
                continue;
            }

            float Diff = UKismetMathLibrary::Abs(AngleZByXY - UKismetMathLibrary::Abs(AngleZByZ));            
            if (Diff < Tolerance)
            {

                BoomData.AngleZ = AngleZByZ >= 0.0f ? (AngleZByXY + AngleZByZ) / 2.0f : AngleZByZ;

                FBoomData BoomData2;
                BoomData2.Point = TurretHeadMesh->GetComponentLocation();
                BoomData2.Point.X = BoomData.Point.X;
                BoomData2.Point.Y = BoomData.Point.Y;
                BoomData2.Point.Z += (UKismetMathLibrary::DegSin(BoomData.AngleZ) * InitialSpeed - 0.0f * G * MyXYTime) * MyXYTime - (G * MyXYTime * MyXYTime) / 2.0f;
                
                auto MyXYVelocity = InitialSpeed * UKismetMathLibrary::DegCos(BoomData.AngleZ);
                auto DirectionStraight = (BoomData2.Point - TurretHeadMesh->GetComponentLocation()).GetSafeNormal();
                
                float XCosJ = DirectionStraight.X / DirectionStraight.Size2D();
                float YSinJ = DirectionStraight.Y / DirectionStraight.Size2D();

                FVector MyVelocity;
                MyVelocity.X = MyXYVelocity * XCosJ;
                MyVelocity.Y = MyXYVelocity * YSinJ;
                MyVelocity.Z = UKismetMathLibrary::DegSin(BoomData.AngleZ) * InitialSpeed - 0.0f * G * MyXYTime;

                FVector BoomPointCheck;
                BoomPointCheck.X = MyVelocity.X * MyXYTime + TurretHeadMesh->GetComponentLocation().X;
                BoomPointCheck.Y = MyVelocity.Y * MyXYTime + TurretHeadMesh->GetComponentLocation().Y;
                BoomPointCheck.Z = MyVelocity.Z * MyXYTime + TurretHeadMesh->GetComponentLocation().Z - (G * MyXYTime * MyXYTime) / 2.0f;
    */            
                /* CheckingDistance = FVector::DistXY(BoomData.Point, BoomPointCheck);
                
                if(CheckingDistance > 2.0f * Projectile->GetCollisionSphereRadius())
                {   
                    UE_LOG(LogBaseTurret, Display, TEXT("BAD CheckingDistance   : %f"), CheckingDistance);
                    if (CheckingDistance >= LastCheckingDistance)
                    {
                        AdditionalTime *= -0.5f;//infinity loop...
                        if (UKismetMathLibrary::Abs(AdditionalTime) < 0.001f)
                        {
                            if(CheckingDistance > 3.0f * Projectile->GetCollisionSphereRadius())
                            {
                                break;
                            }
                        }
                    }
                    LastCheckingDistance = CheckingDistance;
                    break;
                }*/
    /*
                DrawDebugSphere(this->GetWorld(), BoomData.Point, 30, 8, FColor::Green, false, 10.0f);
                DrawDebugSphere(this->GetWorld(), BoomPointCheck, 30, 8, FColor::Yellow, false, 10.0f);
                UE_LOG(LogBaseTurret, Display, TEXT("BoomData      : %s"), *BoomData.Point.ToCompactString());
                UE_LOG(LogBaseTurret, Display, TEXT("BoomData2     : %s"), *BoomData2.Point.ToCompactString());
                UE_LOG(LogBaseTurret, Display, TEXT("BoomPointCheck : %s"), *BoomPointCheck.ToCompactString());
                //UE_LOG(LogBaseTurret, Display, TEXT("CheckingDistance   : %f"), CheckingDistance);                
                UE_LOG(LogBaseTurret, Display, TEXT("MyVelocity : %f, InitialSpeed : %f"), MyVelocity.Size(), InitialSpeed);

                BoomData.Time2nd = MyXYTime;
                BoomData.Direction = MyVelocity.GetSafeNormal();
                
                return true;
            }
            else
            {
                if (Diff > LastDiff)
                {
                    AlphaTimeAddentum *= -0.5f;
                }
                if (UKismetMathLibrary::Abs(AlphaTimeAddentum) < 0.0001f)
                {
                    UE_LOG(LogBaseTurret, Display, TEXT("BAD Angles Diff : %f"), Diff);
                    if (Diff < 1.0f)
                    {
                        Tolerance = 1.0f;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                
                AlphaTimeMultiplier += AlphaTimeAddentum;
                LastDiff = Diff;
            }
        }
    }
    */
    return false;
    
}