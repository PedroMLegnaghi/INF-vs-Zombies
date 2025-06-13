    #include "sun.h"
    
    double timeOfLastSun = 0;
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[SIZE_OF_SUN_ARR]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=800;
    //array to track where a sun hits the ground. Each index of the array corresponds to the index of that sun in the sunArray
    float groundOfTheSuns[SIZE_OF_SUN_ARR]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 8.0;   
    //used to spawn sun appropriately
    double timeSpawnSunTracking =0;
   

    //SUN FUNCTIONS---

//AddSunToArray:
//Given an array of suns, the index of the next sun, the array of lawns, the array of grounds of suns and the proprieties of Rectangle, add that sun to the array of suns 
void AddSunToArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR],  int *indexOfNextSun,Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],int rowOfGround,int columnOfGround,float groundOfTheSuns[SIZE_OF_SUN_ARR], int x, int y) {
    array_of_suns[*indexOfNextSun].x = x;
        array_of_suns[*indexOfNextSun].y =  y;
        array_of_suns[*indexOfNextSun].width = SUN_WIDTH;
        array_of_suns[*indexOfNextSun].height = SUN_HEIGHT;
        groundOfTheSuns[*indexOfNextSun] =  lawn_array[rowOfGround][columnOfGround].y;

        *indexOfNextSun+=1;
}

//AddRandomlySunToArr:
//Given an array of suns, the index of the next sun , the array of lawns of the game and the groundOfTheSuns array, add that sun to the array with x and y position of a random lawn of the game 
void AddRandomlySunToArr(Rectangle array_of_suns[SIZE_OF_SUN_ARR],  int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],float groundOfTheSuns[SIZE_OF_SUN_ARR]) {
    if (*indexOfNextSun >= SIZE_OF_SUN_ARR) return;
    //chooses a number beetwen 0 and NUMBER_ROWS_LAWN
    int row = rand() % (NUMBER_ROWS_LAWN);
    //chooses a number beetwen 0 and NUMBER_COLUMN_LAWN
    int column = rand() % (NUMBER_COLUMN_LAWN);

    AddSunToArray(array_of_suns,indexOfNextSun,lawn_array,row,column,groundOfTheSuns,lawn_array[row][column].x,-20);
}

//updateSunsPosition:
//given the array of the suns of the game ,groundOfTheSuns and the index of the next sun, update their position, until they hit their ground
void updateSunsPosition(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfSun,float groundOfTheSuns[SIZE_OF_SUN_ARR]){
    for(int i=0;i<indexOfSun;i++){

        //if the sun hasn't hit it's ground, (we use "lesser than" here to avoid rounding problems)
        if((array_of_suns[i].y<groundOfTheSuns[i]))
            //update its position
            array_of_suns[i].y+=0.5;

        if (array_of_suns[i].y > groundOfTheSuns[i]) {
            array_of_suns[i].y = groundOfTheSuns[i]; // prevent it from falling under the ground
        }
    }
};

//RemoveSunToArray:
//Given an array of suns ,groundOfTheSuns and the index of the sun, remove the last sun of the arrayofsuns and remove its ground and update the array
void RemoveSunOfArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, int indexOfSunRemoved, float groundOfTheSuns[SIZE_OF_SUN_ARR]) {
    if (indexOfSunRemoved < 0 || indexOfSunRemoved >= *indexOfNextSun) return; //checking if valid index

    for (int i = indexOfSunRemoved; i < (*indexOfNextSun) - 1; i++) {
        array_of_suns[i] = array_of_suns[i + 1]; //update the sun array (similar to mmemove())
        groundOfTheSuns[i] = groundOfTheSuns[i + 1]; //update the groundOfTheSuns array (similar to mmemove())
    }
    (*indexOfNextSun)--;

    // make the last index invalid (because it moved from the last position to the (last-1) position)
    array_of_suns[*indexOfNextSun].x = -1;
    array_of_suns[*indexOfNextSun].y = -1;
    groundOfTheSuns[*indexOfNextSun] = 0;
}

//collectSun:
//Given an array of suns, the mousePointer,the quantityofSun of the player, the index of the next sun and the array of the grounds of the suns, returns wheter one sun was collected(1) or not(0) and collects it
int collectSun(Rectangle array_of_suns[SIZE_OF_SUN_ARR],int *indexOfNextSun, float groundOfTheSuns[SIZE_OF_SUN_ARR])
{
    for (int i = 0; i < *indexOfNextSun; i++) {
        
        if (CheckCollisionPointRec(mousePoint, array_of_suns[i])) { //if mouse of player and the sun collided
            if (IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { //and if the player clicked on the sun
                RemoveSunOfArray(array_of_suns, indexOfNextSun, i, groundOfTheSuns); //collect the sun
                return 1;//Sun was collected
                break; // removes only one sun per click
            }
        }
    }
    return 0;//sun was not collected
}
//addSunToStorage:
//given the gamingSunStorage, add the value of one sun to the storage
void addSunToStorage(unsigned int *gamingSunStorage){
    *gamingSunStorage+=VALUE_OF_EACH_SUN;
}

//DrawSuns:
//Given the array of suns of the game and the index of the sun, draw the suns
void DrawSuns(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfNextSun, Texture2D sunTexture){
    Vector2 origin ={0,0};
    Rectangle sunTextureSourceRectangle = {
        .height=sunTexture.height,
        .width = sunTexture.width,
        .x=0,
        .y=0
    };
    for(int i = 0; i < indexOfNextSun; i++){
        if (array_of_suns[i].x != -1 && array_of_suns[i].y != -1) { //if the sun is not an invalid sun
            // DrawRectangle(array_of_suns[i].x, array_of_suns[i].y,
            //             array_of_suns[i].width, array_of_suns[i].height, YELLOW);//draw it
            DrawTexturePro(sunTexture,sunTextureSourceRectangle,array_of_suns[i],origin,0.0f,WHITE);
        }
    }
}

