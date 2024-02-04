// Header file for the sun.hpp
// Holds most of the parameters. Responsible for actively renewing matrix field_of_sun.

using namespace std;

int seeds = 50; // Amount of seeds. Seeds are the points on field_of_sun where the sun units are generated.
int chance = 99; // Chance of spawning a child seed.
int maxshine = 20; // Maximum possible amount of sun units added to the field of sun at once.

int day_duration = 20000; // Duration of the day. Field_of_sun is being enriched during the day.
int night_duration = 20000; // Duration of the night. Field_of_sun is empty during the night.

unsigned int field_of_sun[mapsize][mapsize]={}; // Matrix of the sun units.
unsigned int shine; // Amount of sun units added to the field of sun.

int enhancer = 0; // Extends the sunseed vector.
int expected = round(450*(maxshine+1)*seeds/(100-chance)); // Expected value of total sun units on the field_of_sun. Mathematically calculated.
int actual; // Actual value of total sun units on the field_of_sun.
int degree; // Bar of the sungradient. It is used to determine the colour.
int day=0; // For days cycling (simulation starts with the daytime -> nighttime -> daytime). So at the beginning day starts at 0 ms.
int night=day_duration; // For nights cycling (first night starts after the first day have passed, so = day_duration ms.)
int duration = day_duration; // For clock arrow rotation. Arrow rotates from 0 to duration ms of the day/night.

vector<pair<int,int>> sunseed; // Vector of the sun seeds.

// This function shines on the field_of_sun at coordinate (x,y) and fills the 3x3 square around it with some amount of sun units.
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
        sunseed.push_back(make_pair(rand() % mapsize,rand() % mapsize));

    // Run through the vector of seeds
    for (int j=0;j<seeds+enhancer;j++)
        {
            getlight(sunseed[j].first,sunseed[j].second);

            // Spawn the child seed (child seed is an adjuscent cell to the parent seed)
            if (chance > rand() % 100)
                {
                    enhancer++;
                    sunseed.push_back(make_pair(sunseed[j].first-1+(rand()%3),sunseed[j].second-1+(rand()%3)));
                }
        }

    // Reset enhancer
    enhancer = 0;

    // Reset expected value
    expected = round(450*(maxshine+1)*seeds/(100-chance));

    // Reset actual and degree parameters
    actual=0;
    for(int i=0;i<mapsize;i++)
        for(int j=0;j<mapsize;j++)
            {
                actual+=field_of_sun[i][j];
                if(degree<field_of_sun[i][j])
                    degree = field_of_sun[i][j];
            }

    // Normalize the degree parameter: max -> max/n (n is the number of colours in the sungradient)
    degree = max(int(degree/size(sungradient)),1);
}

// Clears the field of sun
void sunset()
{
    // Clear the field of sun
    memset(field_of_sun,0,sizeof(field_of_sun));

    // Clear the sunseed
    sunseed.clear();
}