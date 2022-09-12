#pragma once
#include "Player/STUPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "STUCoreTypes.h"

class STUUtils
{
public:
	template<typename T> 
	static T* GetSTUPlayerComponent(AActor* PlayerPawn) 
	{
        if(!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
	}

	static bool AreEnemies(AController* Controller1, AController* Controller2)
	{ 
		if(!Controller1 || !Controller2 || Controller1 == Controller2) return false;
	
		const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}

	class PointToHorizon
    {
    public:
        static const float G() 
        {
            return -UPhysicsSettings::Get()->DefaultGravityZ;
        }
        static FVector GetPredictPoint(float FlyTime, float InitialSpeed, FVector StartPoint, FVector EndPoint)
        {
            if (FlyTime < 0.001f)
                return StartPoint;

            FVector StraightDirection = (EndPoint - StartPoint).GetSafeNormal();
            StraightDirection *= InitialSpeed;

            FVector PredictPoint = StartPoint;
            PredictPoint.X += StraightDirection.X * FlyTime;
            PredictPoint.Y += StraightDirection.Y * FlyTime;
            PredictPoint.Z += StraightDirection.Z * FlyTime - (G() * FlyTime * FlyTime) / 2.0f;

            return PredictPoint;
        }

        static FVector GetPredictPoint(float FlyTime, FVector Velocity, FVector StartPoint)
        {
            if (FlyTime < 0.001f)
                return StartPoint;

            FVector PredictPoint = StartPoint;
            PredictPoint.X = Velocity.X * FlyTime;
            PredictPoint.Y = Velocity.Y * FlyTime;
            PredictPoint.Z = Velocity.Z * FlyTime - (G() * FlyTime * FlyTime) / 2.0f;

            return PredictPoint;
        }
                

        static FVector GetVelocityToPoint(FVector StartPoint, float AngleZ, float InitialSpeed, float FlyTime, float EndPointX, float EndPointY)
        {
            
            FVector EndPoint;
            EndPoint.X = EndPointX;
            EndPoint.Y = EndPointY;
            EndPoint.Z = StartPoint.Z + (InitialSpeed * UKismetMathLibrary::DegSin(AngleZ)) * FlyTime - (G() * FlyTime * FlyTime) / 2.0f;

            auto VelocityXY = InitialSpeed * UKismetMathLibrary::DegCos(AngleZ);
            auto DirectionStraight = (EndPoint - StartPoint).GetSafeNormal();

            float XCosJ = DirectionStraight.X / DirectionStraight.Size2D();
            float YSinJ = DirectionStraight.Y / DirectionStraight.Size2D();

            FVector Velocity;
            Velocity.X = VelocityXY * XCosJ;
            Velocity.Y = VelocityXY * YSinJ;
            Velocity.Z = InitialSpeed * UKismetMathLibrary::DegSin(AngleZ);

            return Velocity;
        }

        static bool GetAngleZAndFlyTime(float& AngleZ, float& FlyTime, float ChekingTime, float DistanceToBoomPointXY, float DistanceToBoomPointZ, float InitialSpeed)
        {

            float Tolerance = 0.01;
            float AlphaTimeMultiplier = 0.0f;
            float AlphaTimeAddentum = 0.1f;
            float LastDiff = 360.0f;
            float AngleZByXY{}, AngleZByZ{};
            int OutOfRangeCount = 0;

            while(true)
            {
                if(AlphaTimeMultiplier >= 1.0f)
                    break;
                FlyTime = UKismetMathLibrary::Abs(DistanceToBoomPointXY) / (InitialSpeed - InitialSpeed * AlphaTimeMultiplier);
                if(FlyTime > ChekingTime)
                    break;

                float CosAngleXY = DistanceToBoomPointXY / (InitialSpeed * FlyTime);
                float SinAngleZ = (DistanceToBoomPointZ + (G() * FlyTime * FlyTime) / 2.0f) / (InitialSpeed * FlyTime);
                if(CosAngleXY <= 1.0f && CosAngleXY >= -1.0f && SinAngleZ <= 1.0f && SinAngleZ >= -1.0f)
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
                if(Diff < Tolerance)
                {
                    AngleZ = AngleZByZ >= 0.0f ? (AngleZByXY + AngleZByZ) / 2.0f : AngleZByZ;

                    return true;
                }
                else
                {
                    if(Diff > LastDiff)
                    {
                        AlphaTimeAddentum *= -0.5f;
                    }
                    if (UKismetMathLibrary::Abs(AlphaTimeAddentum) < 0.0001f)
                    {
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
            return false;
        }

        static bool GetBoomData(FBoomData& BoomData, FVector ShottingActorLocation, AActor* FlyingActor, float InitialSpeed)
        {
            auto TargetVelocityVector = FlyingActor->GetVelocity();

            float AngleZ{};
            float FlyTime{};

            float FarTime = 2.0f;
            float AdditionalTime = FarTime * 0.05f;
            for(float ChekingTime = FarTime * 0.1f; ChekingTime > 0.0f && ChekingTime <= FarTime; ChekingTime += AdditionalTime)
            {
                auto BoomPoint = FlyingActor->GetActorLocation();
                BoomPoint.X += TargetVelocityVector.X * ChekingTime;
                BoomPoint.Y += TargetVelocityVector.Y * ChekingTime;
                BoomPoint.Z += TargetVelocityVector.Z * ChekingTime - (G() * ChekingTime * ChekingTime) / 2.0f;

                float DistanceToBoomPointXY = (BoomPoint - ShottingActorLocation).Size2D();
                float DistanceToBoomPointZ = BoomPoint.Z - ShottingActorLocation.Z;

                if(GetAngleZAndFlyTime(AngleZ, FlyTime, ChekingTime, DistanceToBoomPointXY, DistanceToBoomPointZ, InitialSpeed))
                {
                    FVector VelocityToPoint = GetVelocityToPoint(ShottingActorLocation, AngleZ, InitialSpeed, FlyTime, BoomPoint.X, BoomPoint.Y);

                    BoomData.Time1st = ChekingTime;
                    BoomData.Time2nd = FlyTime;
                    BoomData.Point = BoomPoint;
                    BoomData.Direction2nd = VelocityToPoint.GetSafeNormal();

                    return true;
                }
            }
            return false;
        }
    };
};