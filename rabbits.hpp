#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int rabbit_variance = 10;
const int rabbit_speed = 20;
const int rabbit_hp = 120000;
const int rabbit_birth = 110000;

int rabbit_regime = 0;

class Rabbits : public Drawable
{
public:

    Rabbits(int population = 1)
    {
        VertexArray turn_vertices(Quads,4*population); // A turnaround for the vertex array attribute

        for (int i=0;i<population;i++)
            {
                for(int j=0;j<4;j++){turn_vertices[j+i*4].color = Color::White;} // Set the color of the vertex

                srand(clock()); // Randomize the seed

                int stx = rand()%mapsize;
                int sty = rand()%mapsize;
                while (rabbitsmap[stx][sty] != 0)
                {
                    stx = rand()%mapsize;
                    sty = rand()%mapsize;
                }

                turn_vertices[0+i*4].position = Vector2f(stx*rang,sty*rang);
                turn_vertices[1+i*4].position = Vector2f((stx+1)*rang,sty*rang);
                turn_vertices[2+i*4].position = Vector2f((stx+1)*rang,(sty+1)*rang);
                turn_vertices[3+i*4].position = Vector2f(stx*rang,(sty+1)*rang);

                // Attributes
                position.push_back(make_pair(stx,sty));
                power.push_back(rabbit_hp/2);
                rabbitsmap[stx][sty] = i+1;
            }

        // Attributes
        fluffle=population;
        vertices = turn_vertices;
    }

    void move()
    {
        for (int k=0;k<fluffle;k++)
        {
            int x = position[k].first; // Current position x
            int y = position[k].second; // Current position y
            int best = field_of_sun[x][y]; // Set the best value to be current cell value
            int n;
            int temp;

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++) 
                    // If the cell is empty add the coordinates to the searching vector
                    if (rabbitsmap[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                        {
                            searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                            if (field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] > best)
                                {
                                    best = field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize];
                                    x = (position[k].first + i + mapsize)%mapsize;
                                    y = (position[k].second + j + mapsize)%mapsize;
                                }
                        }

            // No free space around the rabbit
            if (size(searching) == 0)
                continue;

            if (best == 0 || (best == field_of_sun[position[k].first][position[k].second] && (rabbit_variance > rand()%100)))
                {
                    n = rand() % size(searching);
                    x = searching[n].first;
                    y = searching[n].second;
                }

            temp=rabbitsmap[x][y];
            rabbitsmap[x][y] = rabbitsmap[position[k].first][position[k].second];
            rabbitsmap[position[k].first][position[k].second] = temp;

            position[k] = make_pair(x,y);

            vertices[0+k*4].position = Vector2f(x*rang,y*rang);
            vertices[1+k*4].position = Vector2f((x+1)*rang,y*rang);
            vertices[2+k*4].position = Vector2f((x+1)*rang,(y+1)*rang);
            vertices[3+k*4].position = Vector2f(x*rang,(y+1)*rang);

            searching.clear();
        }
    }

    // Kill the rabbit and do cosmetics on the class attributes
    void kill(int id)
    {
        if (id == 0)
            return;

        id--;
        fluffle--;

        for (int i=id*4;i<fluffle*4;i++)
            vertices[i] = vertices[i+4];

        vertices.resize(fluffle*4);

        rabbitsmap[position[id].first][position[id].second] = 0;

        for(int i=0;i<mapsize;i++)
            for(int j=0;j<mapsize;j++)
                if (rabbitsmap[i][j] > id)
                    rabbitsmap[i][j]--;
    
        position.erase(position.begin() + id);

        power.erase(power.begin() + id);
    }

    // Feed the rabbit from the sun
    void synthes()
    {
        for(int i=0;i<fluffle;i++)
            {
                power[i]+=field_of_sun[position[i].first][position[i].second]; // Add energy fron sun
                power[i]=min(power[i],rabbit_hp); // Upper bound of energy
                power[i]-=rabbit_speed; // Cost of life
                if (power[i] < 0)
                    kill(i); 
            }

    }

    // Birth of the rabbit
    void birth()
    {
        for (int k=0; k<fluffle; k++)
            if (power[k] > rabbit_birth)
                {
                    for (int i=-1; i<2; i++)
                        for(int j=-1; j<2; j++)
                            if(rabbitsmap[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                                searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                    
                    if (size(searching) == 0)
                        continue;

                    int i = rand() % size(searching);
                    int x = searching[i].first;
                    int y = searching[i].second;
                    
                    power[k]=power[k]/2;

                    fluffle++;
                    rabbitsmap[x][y] = fluffle;
                    vertices.resize(fluffle*4);

                    vertices[fluffle*4-4].position = Vector2f(x*rang,y*rang);
                    vertices[fluffle*4-3].position = Vector2f((x+1)*rang,y*rang);
                    vertices[fluffle*4-2].position = Vector2f((x+1)*rang,(y+1)*rang);
                    vertices[fluffle*4-1].position = Vector2f(x*rang,(y+1)*rang);

                    vertices[fluffle*4-4].color = Color::White;
                    vertices[fluffle*4-3].color = Color::White;
                    vertices[fluffle*4-2].color = Color::White;
                    vertices[fluffle*4-1].color = Color::White;

                    position.push_back(make_pair(x,y));
                    srand(clock());
                    power.push_back(15000+(rand()%(rabbit_hp/2))); // expected = 15000+(0,30000)

                    searching.clear();
                }

    }

    // Combine all the functions
    void update()
    {
        move();
        synthes();
        birth();
    }

    ~Rabbits() {};
public:

    int fluffle; // Number of rabbits
    vector<int> power; // Energy power of rabbits
    int rabbitsmap[mapsize][mapsize]={0}; // 0 - no rabbit, >0 - id of rabbit

private:

    VertexArray vertices; // Vertex array for drawing
    vector<pair<int,int>> position; // Positions of rabbits

    vector<pair<int,int>> searching; // Searching vector for all means
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};