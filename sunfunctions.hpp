const int mapsize = 150;
const int seeds = 20; // amount of seeds
const int chance = 99; // chance of spawning a child seed
const int maxshine = 8; // maximum possible amount of sun units added to the field of sun

unsigned int field_of_sun[mapsize][mapsize]={0};
unsigned int shine;

int enhancer = 0; // extends the suna and sunb vectors
int expected = round(450*(maxshine+1)*seeds/(100-chance)); // expected value of total sun units on the field of sun
float grad = round(100*expected/pow(mapsize,2))/100; // expected value per cell
int regime = 5000;

std::vector<int> suna (seeds); // x-coordinate of the sun seeds
std::vector<int> sunb (seeds); // y-coordinate of the sun seeds

// This function shines on the field of sun at coordinate (x,y) and fills the 3x3 square around with some amount of sun
void getlight(int x, int y)
{
    shine = rand() % maxshine +1;
    for (int j=0; j<3;j++)
        for (int k=0; k<3; k++)
            field_of_sun[(y+j+mapsize-1) % mapsize][(x+k+mapsize-1) % mapsize]+= shine;
}

// Enriches the field of sun
void sunrise()
{
    srand(clock());

    // Set the parent seeds
    for (int i=0;i<seeds;i++)
        {
            suna[i] = rand() % mapsize;
            sunb[i] = rand() % mapsize;
        }

    // Run through the vector pairs (suna,sunb)
    for (int j=0;j<seeds+enhancer;j++)
        {
            getlight(suna[j],sunb[j]);

            // Spawn the child seed
            if (chance > rand() % 100)
                {
                    enhancer++;

                    // Determine the coordinates of the child seed
                    switch (rand() % 8){
                        case 0:
                            {
                                suna.push_back(suna[j]-1);
                                sunb.push_back(sunb[j]);
                            }
                            break;
                        case 1:
                            {
                                suna.push_back(suna[j]-1);
                                sunb.push_back(sunb[j]+1);
                            }
                            break;
                        case 2:
                            {
                                suna.push_back(suna[j]);
                                sunb.push_back(sunb[j]+1);
                            }
                            break;
                        case 3:
                            {
                                suna.push_back(suna[j]+1);
                                sunb.push_back(sunb[j]+1);
                            }
                            break;
                        case 4:
                            {
                                suna.push_back(suna[j]+1);
                                sunb.push_back(sunb[j]);
                            }
                            break;
                        case 5:
                            {
                                suna.push_back(suna[j]+1);
                                sunb.push_back(sunb[j]-1);
                            }
                            break;
                        case 6:
                            {
                                suna.push_back(suna[j]);
                                sunb.push_back(sunb[j]-1);
                            }
                            break;
                        case 7:
                            {
                                suna.push_back(suna[j]-1);
                                sunb.push_back(sunb[j]-1);
                            }
                            break;
                    }
                }
        }

    // Reset enhancer
    enhancer = 0;
}

// Clears the field of sun
void sunset()
{
    // Clear the field of sun
    for(int i=0;i<mapsize;i++)
        for(int j=0;j<mapsize;j++)
            field_of_sun[i][j]=0;

    // Clear the suna and sunb
    suna.clear();
    sunb.clear();

    // Reset the suna and sunb
    for (int i=0; i<seeds; i++)
        {
            suna.push_back(0);
            sunb.push_back(0);
        }
}