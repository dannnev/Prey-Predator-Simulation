// Header file for the sun.hpp

const int seeds = 100; // amount of seeds
const int chance = 98; // chance of spawning a child seed
const int maxshine = 1000; // maximum possible amount of sun units added to the field of sun

unsigned int field_of_sun[mapsize][mapsize]={0};
unsigned int shine;

int enhancer = 0; // extends the suna and sunb vectors
int expected = round(450*(maxshine+1)*seeds/(100-chance)); // expected value of total sun units on the field of sun
int actual; // actual value of total sun units on the field of sun
int gradus; // bar of the sungradient
int sunset_regime = 30000; // time between sunrises
int sunrise_regime = 0; // time between sunrises

std::vector<int> suna (seeds); // x-coordinate of the sun seeds
std::vector<int> sunb (seeds); // y-coordinate of the sun seeds

// This function shines on the field of sun at coordinate (x,y) and fills the 3x3 square around with some amount of sun
void getlight(int x, int y)
{
    shine = rand() % maxshine +1;
    for (int j=-1; j<2;j++)
        for (int k=-1; k<2; k++)
            field_of_sun[(x+j+mapsize) % mapsize][(y+k+mapsize) % mapsize]+= shine;
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

    // Reset actual and gradus parameters
    actual=0;
    for(int i=0;i<mapsize;i++)
        for(int j=0;j<mapsize;j++)
            {
                actual+=field_of_sun[i][j];
                if(gradus<field_of_sun[i][j])
                    gradus = field_of_sun[i][j];
            }

    // Normalize the gradus parameter: max -> max/16 (16 is the number of colours in the sungradient)
    gradus = int(gradus/16);

    // If gradus is zero, set it to 1
    if(gradus == 0)
        gradus = 1;
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