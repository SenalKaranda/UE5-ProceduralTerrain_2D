// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileActor.h"
#include "ProceduralMapGenerator.generated.h"

UCLASS()
class CPP_HEXMAPS_API AProceduralMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    // The array of tiles (types & coordinates) that make up the map.
    UPROPERTY(BlueprintReadWrite, Category = "Map Generation")
        TArray<FString> TileArray;

    // The blueprint class for a tile actor.
    UPROPERTY(EditAnywhere, Category = "Map Generation")
        TSubclassOf<ATileActor> TileBP;
    
    // The width of the map, in tiles.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 MapWidth;

    // The height of the map, in tiles.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 MapHeight;

    // The size of a tile, should be uniform x and y.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Generation")
        float TileSize;

    // Used to randomize seed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        bool RandomizeSeed = true;

    // The seed used for generating the map.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 Seed;

    // The percentage chance of a tile being "land".
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        float LandPercentage = 50.0f;
    
    // Max possible landmasses generated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 MaxLandmasses = 1;

    // Max size per landmass, in tiles.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 MaxLandmassSize = 10;

    // Max iterations for land tile generation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
        int32 MaxIterations = 2500;

    // Used to hold total tiles for array init and other functions.
    int32 TotalTiles;

    // Random stream for seeding.
    FRandomStream Stream;

private:
    // The array of tile actors that make up the map.
    TArray<ATileActor*> TileActorArray;

    // Generates a random seed if none is specified.
    int32 GenerateSeed();

    // Initializes the tile array with "ocean" tiles.
    void InitializeTileArray();

    // Fills in the "land" tiles based on the LandPercentage variable.
    void GenerateLand();

    // Sets the biome for each tile.
    void SetBiomes();

    // Spawns tiles.
    void SpawnTiles();

    // Determines the biome of a given tile.
    FString DetermineBiome(int32 X, int32 Y);

    // Determines the distance from the center of the map to a given tile.
    float DistanceFromCenter(int32 X, int32 Y);

    // Returns a random float between 0 and 1 based on the given seed.
    float RandomFloat();

    // Returns a random integer between Min and Max based on the given seed.
    int32 RandomInt(int32 Min, int32 Max);

    // Returns an array of points of surrounding tiles.
    TArray<FIntPoint> GetSurroundingTiles(int32 x, int32 y);

    // Returns number of surrounding ocean tiles, takes array of points as input.
    int32 GetSurroundingMatchTiles(TArray<FIntPoint> Tiles, FString Match);

    // Return bool for matching a specific tile to the requested type.
    bool MatchTileType(FIntPoint Point, FString Type);

    // Convert 1D Array index to grid coordinates.
    FIntPoint IndexToCoordinates(int32 index);

    // Convert grid coordinates to 1D Array index.
    int32 CoordinatesToIndex(int32 X, int32 Y);

    UFUNCTION(BlueprintCallable)
    // Destroy all tile cctors, reset tile arrays, regenerate.
    void RegenerateMap(bool RandomSeed);

    UFUNCTION(BlueprintCallable)
    // Generate map from save.
    void LoadMap();

    /*
    * Cool Seeds:
    * 8343
    * 16565
    * 5101
    * 12287
    * 18757
    * 26864
    */
};
