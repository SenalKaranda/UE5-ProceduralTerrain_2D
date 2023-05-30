// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMapGenerator.h"

// Sets default values
AProceduralMapGenerator::AProceduralMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralMapGenerator::BeginPlay()
{
	Super::BeginPlay();

    RegenerateMap(RandomizeSeed);
}

// Called every frame
void AProceduralMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Primary Generation Functions

void AProceduralMapGenerator::LoadMap()
{
    for (int32 i = 0; i < TileActorArray.Num(); i++) { if (TileActorArray.IsValidIndex(i)) { TileActorArray[i]->Destroy(); } }
    TileActorArray.Empty();
    SpawnTiles();
}

void AProceduralMapGenerator::RegenerateMap(bool RandomSeed)
{
    if (RandomSeed) { Seed = GenerateSeed(); }

    Stream = FRandomStream(Seed);

    TotalTiles = MapWidth * MapHeight;

    InitializeTileArray();
    GenerateLand();
    SetBiomes();
    SpawnTiles();
}



void AProceduralMapGenerator::InitializeTileArray()
{
    for (int32 i = 0; i < TileActorArray.Num(); i++) { if (TileActorArray.IsValidIndex(i)) { TileActorArray[i]->Destroy(); } }
    TileActorArray.Empty();
    TileArray.Empty();
    for (int32 i = 0; i < TotalTiles; i++) { TileArray.Add("ocean"); }
}
/* GENERATE LAND ORIGINAL
void AProceduralMapGenerator::GenerateLand()
{
    int32 TotalTilesGenerated = 0;
    int32 TotalLandmasses = 0;
    
    int32 LandTiles = (LandPercentage / 100) * TileArray.Num();

    for (int z = 0; z < MaxIterations; z++)
    {
        if (TotalTilesGenerated < LandTiles)
        {
            if (TotalLandmasses < MaxLandmasses)
            {
                //Choose random point and if it is an ocean, start a landmass at that tile.
                int32 Index = RandomInt(0, TileArray.Num() - 1);
                int32 CurrentLandmassSize = 0;

                for (int i = 0; i < MaxLandmassSize; i++)
                {
                    FIntPoint IndexCoords = IndexToCoordinates(Index);
                    TArray<FIntPoint> SurroundingTiles = GetSurroundingTiles(IndexCoords.X, IndexCoords.Y);
     
                    if (TileArray[Index] == "ocean")
                    {
                        TileArray[Index] = "land";
                        TotalTilesGenerated++;
                        CurrentLandmassSize++;
                    }
                    int32 DirectionTile = RandomInt(0, SurroundingTiles.Num() - 1);
                    Index = SurroundingTiles[DirectionTile].Y * MapWidth + SurroundingTiles[DirectionTile].X;
                }
                TotalLandmasses++;
            }
        }
    }

    for (int32 i = 0; i < TileArray.Num() - 1; i++)
    {
        int32 topleft = 0;
        int32 botright = CoordinatesToIndex(MapWidth - 1, MapHeight - 1);
        int32 topright = CoordinatesToIndex(0, MapHeight - 1);
        int32 botleft = CoordinatesToIndex(MapWidth - 1, 0);
        if (i == topleft || i == botright || i == topright  || i == botleft ) { continue; }
        FIntPoint Point = IndexToCoordinates(i);
        TArray<FIntPoint> SurroundingTiles = GetSurroundingTiles(Point.X, Point.Y);
        int32 SurroundingOceans = GetSurroundingMatchTiles(SurroundingTiles, "ocean");
        if (SurroundingOceans < 4)
        {
            TileArray[i] = "land";
        }
    }
}
*/

void AProceduralMapGenerator::GenerateLand()
{
    int32 TotalTilesGenerated = 0;
    int32 TotalLandmasses = 0;

    int32 LandTiles = (LandPercentage / 100) * TileArray.Num();

    for (int z = 0; z < MaxIterations; z++)
    {
        if (TotalTilesGenerated < LandTiles)
        {
            if (TotalLandmasses < MaxLandmasses)
            {
                //Choose random point and if it is an ocean, start a landmass at that tile.
                int32 Index = RandomInt(0, TileArray.Num() - 1);
                int32 CurrentLandmassSize = 0;

                for (int i = 0; i < MaxLandmassSize; i++)
                {
                    FIntPoint IndexCoords = IndexToCoordinates(Index);
                    TArray<FIntPoint> SurroundingTiles = GetSurroundingTiles(IndexCoords.X, IndexCoords.Y);

                    if (TileArray[Index] == "ocean")
                    {
                        TileArray[Index] = "land";
                        TotalTilesGenerated++;
                        CurrentLandmassSize++;
                    }
                    int32 DirectionTile = RandomInt(0, SurroundingTiles.Num() - 1);
                    Index = SurroundingTiles[DirectionTile].Y * MapWidth + SurroundingTiles[DirectionTile].X;
                }
                TotalLandmasses++;
            }
        }
    }

    for (int32 i = 0; i < TileArray.Num() - 1; i++)
    {
        int32 topleft = 0;
        int32 botright = CoordinatesToIndex(MapWidth - 1, MapHeight - 1);
        int32 topright = CoordinatesToIndex(0, MapHeight - 1);
        int32 botleft = CoordinatesToIndex(MapWidth - 1, 0);
        if (i == topleft || i == botright || i == topright || i == botleft) { continue; }
        FIntPoint Point = IndexToCoordinates(i);
        TArray<FIntPoint> SurroundingTiles = GetSurroundingTiles(Point.X, Point.Y);
        int32 SurroundingOceans = GetSurroundingMatchTiles(SurroundingTiles, "ocean");
        if (SurroundingOceans < 4) { TileArray[i] = "land"; }
    }
}

void AProceduralMapGenerator::SetBiomes()
{
    for (int32 x = 0; x < MapWidth; x++)
    {
        for (int32 y = 0; y < MapHeight; y++)
        {
            int32 Index = CoordinatesToIndex(x, y);

            if (TileArray[Index] == "land")
            {
                FString Biome = DetermineBiome(x, y);
                TileArray[Index] = Biome;
            }
            else
            {
                if (GetSurroundingMatchTiles(GetSurroundingTiles(x, y), "land") > 5) 
                {
                    FString Biome = DetermineBiome(x, y);
                    TileArray[Index] = Biome;
                }
            }
        }
    }
}

void AProceduralMapGenerator::SpawnTiles()
{
    for (int32 x = 0; x < MapWidth; x++)
    {
        for (int32 y = 0; y < MapHeight; y++)
        {
            int32 Index = y * MapWidth + x;

            FVector Location = FVector(x * TileSize, y * TileSize, 0);
            FRotator Rotation = FRotator(0.f);
            ATileActor* NewTile = GetWorld()->SpawnActor<ATileActor>(TileBP, Location, Rotation);
            NewTile->TileType = TileArray[Index];
            TileActorArray.Add(NewTile);
        }
    }
}

// Helper Functions
int32 AProceduralMapGenerator::GenerateSeed()
{
    return FMath::Rand();
}

FString AProceduralMapGenerator::DetermineBiome(int32 X, int32 Y)
{
    float Distance = DistanceFromCenter(X, Y);
    float TundraRadius = (MapHeight * TileSize) * .375;
    float DesertRadius = (MapHeight * TileSize) * .125;
    
    bool OceanAdjacent = false;
    bool TundraAdjacent = false;
    bool IceAdjacent = false;

    if (GetSurroundingMatchTiles(GetSurroundingTiles(X, Y), "ocean") > 0) { OceanAdjacent = true; }
    if (GetSurroundingMatchTiles(GetSurroundingTiles(X, Y), "tundra") > 0) { TundraAdjacent = true; }
    if (GetSurroundingMatchTiles(GetSurroundingTiles(X, Y), "ice") > 0) { IceAdjacent = true; }

    if (OceanAdjacent) 
    { 
        if (Distance > TundraRadius && (TundraAdjacent || IceAdjacent)) { return "ice"; }
        else { return "beach"; }
    }
    else if (Distance > TundraRadius) { return "tundra"; }
    else if (Distance < DesertRadius) { return "desert"; }
    else { return "forest"; }
}

float AProceduralMapGenerator::DistanceFromCenter(int32 X, int32 Y)
{
    int32 CenterX = (MapWidth * TileSize) / 2;
    int32 CenterY = (MapHeight * TileSize) / 2;
    float DeltaX = (X * TileSize) - CenterX;
    float DeltaY = (Y * TileSize) - CenterY;
    //return FMath::Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
    return FMath::Sqrt(DeltaX * DeltaX);
}

float AProceduralMapGenerator::RandomFloat()
{
    //FRandomStream Stream(Seed);
    return Stream.FRandRange(0.0f, 1.0f);
}

int32 AProceduralMapGenerator::RandomInt(int32 Min, int32 Max)
{
    //FRandomStream Stream(Seed);
    return Stream.RandRange(Min, Max);
}

TArray<FIntPoint> AProceduralMapGenerator::GetSurroundingTiles(int32 x, int32 y)
{
    TArray<FIntPoint> surroundingTiles;

    // Check the tile to the left
    if (x > 0) { surroundingTiles.Add(FIntPoint(x - 1, y)); }

    // Check the tile to the right
    if (x < MapWidth - 1) { surroundingTiles.Add(FIntPoint(x + 1, y)); }

    // Check the tile above
    if (y > 0) { surroundingTiles.Add(FIntPoint(x, y - 1)); }

    // Check the tile below
    if (y < MapHeight - 1) { surroundingTiles.Add(FIntPoint(x, y + 1)); }

    // Check the tile to the top left
    if (x > 0 && y > 0) { surroundingTiles.Add(FIntPoint(x - 1, y - 1)); }

    // Check the tile to the top right
    if (x < MapWidth - 1 && y > 0) { surroundingTiles.Add(FIntPoint(x + 1, y - 1)); }

    // Check the tile to the bottom left
    if (x > 0 && y < MapHeight - 1) {  surroundingTiles.Add(FIntPoint(x - 1, y + 1)); }

    // Check the tile to the bottom right
    if (x < MapWidth - 1 && y < MapHeight - 1) { surroundingTiles.Add(FIntPoint(x + 1, y + 1)); }

    return surroundingTiles;
}

int32 AProceduralMapGenerator::GetSurroundingMatchTiles(TArray<FIntPoint> Tiles, FString Type)
{
    int32 TempNum = 0;
    for (int32 i = 0; i < Tiles.Num(); i++)
    {
        int32 Index = CoordinatesToIndex(Tiles[i].X, Tiles[i].Y);
        if (Type == "land")
        {
            if (TileArray[Index] == "forest" || TileArray[Index] == "desert" || TileArray[Index] == "tundra" || TileArray[Index] == "beach" || TileArray[Index] == "ice") { TempNum++; }
        }
        else
        {
            if (TileArray[Index] == Type) { TempNum++; }
        }
    }
    return TempNum;
}

bool AProceduralMapGenerator::MatchTileType(FIntPoint Point, FString Type)
{
    int32 Index = CoordinatesToIndex(Point.X, Point.Y);
    if (TileArray[Index] == Type) { return true; }
    else { return false; }
}

FIntPoint AProceduralMapGenerator::IndexToCoordinates(int32 index)
{
    int32 x = index % MapWidth;
    int32 y = index / MapWidth;
    return FIntPoint(x, y);
}

int32 AProceduralMapGenerator::CoordinatesToIndex(int32 X, int32 Y)
{
    return Y * MapWidth + X;
}