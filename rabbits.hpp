#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int rabbit_variance = 10;
const int rabbit_speed = 100;
int rabbit_regime = 3000;

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
                Vector2f pos(stx*rang,sty*rang); // random starting position

                turn_vertices[0+i*4].position = Vector2f(stx*rang+rang,sty*rang+rang);
                turn_vertices[1+i*4].position = Vector2f(stx*rang+2*rang,sty*rang+rang);
                turn_vertices[2+i*4].position = Vector2f(stx*rang+2*rang,sty*rang+2*rang);
                turn_vertices[3+i*4].position = Vector2f(stx*rang+rang,sty*rang+2*rang);

                // Attributes
                position.push_back(Vector2i(stx*rang,sty*rang));
                power.push_back(0);
                ghostmap[stx][sty] = 1;
            }

        // Attributes
        fluffle=population;
        vertices = turn_vertices;
    }

    void move(unsigned int field_of_sun[][mapsize])
    {
        for (int k=0;k<fluffle;k++)
        {
            int x = position[k].x;
            int y = position[k].y;
            int best = field_of_sun[int(x/rang)][int(y/rang)];
            ghostmap[int(x/rang)][int(y/rang)] = 0; // Temporary remove the rabbit from the map

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++) 
                    if (ghostmap[(int(position[k].x/rang)+i+mapsize)%mapsize][(int(position[k].y/rang)+j+mapsize)%mapsize] == 0 && field_of_sun[(int(position[k].x/rang)+i+mapsize)%mapsize][(int(position[k].y/rang)+j+mapsize)%mapsize] > best)
                        {
                            best = field_of_sun[(int(position[k].x/rang)+i+mapsize)%mapsize][(int(position[k].y/rang)+j+mapsize)%mapsize];
                            x = (position[k].x + i*rang+heigth)%heigth;
                            y = (position[k].y + j*rang+heigth)%heigth;
                        }

            if (best == 0 || (best == field_of_sun[int(x/rang)][int(y/rang)] && rabbit_variance > rand() % 100))
                {
                    int i = rand() % 3 -1;
                    int j = rand() % 3 -1;
                    if(ghostmap[(int(position[k].x/rang)+i+mapsize)%mapsize][(int(position[k].y/rang)+j+mapsize)%mapsize] == 0)
                        {
                            x = (position[k].x + i*rang+heigth)%heigth;
                            y = (position[k].y + j*rang+heigth)%heigth;
                        }
                }

            position[k] = Vector2i(x,y);

            ghostmap[int(x/rang)][int(y/rang)] = 1;

            vertices[0+k*4].position = Vector2f(x,y);
            vertices[1+k*4].position = Vector2f(x+rang,y);
            vertices[2+k*4].position = Vector2f(x+rang,y+rang);
            vertices[3+k*4].position = Vector2f(x,y+rang);
        }
    }

    void kill(int id)
    {
        for (int i=id*4;i<(fluffle-1)*4;i++)
            vertices[i] = vertices[i+4];

        fluffle--;

        vertices.resize(fluffle*4);

        ghostmap[int(position[id].x/rang)][int(position[id].y/rang)] = 0;

        position.erase(position.begin() + id);

        power.erase(power.begin() + id);
    }

    void synthes()
    {
        for(int i=0;i<size(position);i++)
            power[i]+=field_of_sun[int(position[i].x/rang)][int(position[i].y/rang)];

    }

    ~Rabbits() {};
public:

    int fluffle; // Number of rabbits

private:

    VertexArray vertices; // Vertex array for drawing
    vector<Vector2i> position; // Positions of rabbits
    int ghostmap[mapsize][mapsize]={0}; // 0 - no rabbit, 1 - rabbit
    vector<int> power; // Energy power of rabbits
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};