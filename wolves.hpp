#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int wolf_variance = 10;
const int wolf_speed = 20;
const int wolf_hp = 500000;
const int wolf_birth = 300000;

int wolf_regime = 0;

class Wolves : public Drawable
{
public:
    
    Wolves(int population = 1)
    {
        VertexArray turn_vertices(Quads,4*population); // A turnaround for the vertex array attribute

        for (int i=0;i<population;i++)
            {
                for(int j=0;j<4;j++){turn_vertices[j+i*4].color = Color::Black;} // Set the color of the vertex

                srand(clock()); // Randomize the seed

                int stx = rand()%mapsize;
                int sty = rand()%mapsize;
                while (wolvesmap[stx][sty] != 0)
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
                power.push_back(wolf_hp/2);
                wolvesmap[stx][sty] = i+1;
            }

        // Attributes
        pack=population;
        vertices = turn_vertices;
    }

    void move(Rabbits &zhoverch)
    {
        for(int k=0;k<pack;k++)
        {
            int x = position[k].first; // Current position x
            int y = position[k].second; // Current position y
            int temp;

            for (int i=-1; i<2; i++)
                for(int j=-1; j<2; j++)
                    if(wolvesmap[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0 && field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                        searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,((position[k].second+j+mapsize)%mapsize)));

            if (searching.size() == 0)
                continue;

            for (int i=0; i<searching.size(); i++)
                if (zhoverch.rabbitsmap[searching[i].first][searching[i].second] > 0)
                    hunting.push_back(make_pair(searching[i].first,searching[i].second));

            if (hunting.size() == 0)
                {
                    int i = rand() % size(searching);
                    x = searching[i].first;
                    y = searching[i].second;
                }
            else
                {
                    int i = rand() % size(hunting);
                    x = hunting[i].first;
                    y = hunting[i].second;

                    feed(k,zhoverch,x,y);
                    zhoverch.kill(zhoverch.rabbitsmap[x][y]);

                }

            temp=wolvesmap[x][y];
            wolvesmap[x][y] = wolvesmap[position[k].first][position[k].second];
            wolvesmap[position[k].first][position[k].second] = temp;

            position[k] = make_pair(x,y);


            vertices[0+k*4].position = Vector2f(x*rang,y*rang);
            vertices[1+k*4].position = Vector2f((x+1)*rang,y*rang);
            vertices[2+k*4].position = Vector2f((x+1)*rang,(y+1)*rang);
            vertices[3+k*4].position = Vector2f(x*rang,(y+1)*rang);

            power[k]-=2*wolf_speed; // Cost of life
            searching.clear();
            hunting.clear();        
        }

    }

    void kill(int id)
        {
            if (id == 0)
                return;

            id--;
            pack--;

            for (int i=id*4;i<pack*4;i++)
                vertices[i] = vertices[i+4];

            vertices.resize(pack*4);

            wolvesmap[position[id].first][position[id].second] = 0;

            for(int i=0;i<mapsize;i++)
                for(int j=0;j<mapsize;j++)
                    if (wolvesmap[i][j] > id)
                        wolvesmap[i][j]--;
        
            position.erase(position.begin() + id);

            power.erase(power.begin() + id);
        }

    void feed(int id, Rabbits &zhoverch, int x, int y)
    {
        power[id]+=zhoverch.power[zhoverch.rabbitsmap[x][y]];
        power[id]=min(power[id],wolf_hp); // Upper bound of energy

        if (power[id] < 0)
            kill(id); 
    }

    void birth()
        {
            for (int k=0; k<pack; k++)
                if (power[k] > wolf_birth)
                    {
                        for (int i=-1; i<2; i++)
                            for(int j=-1; j<2; j++)
                                if(wolvesmap[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0 && field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize])
                                    searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                        
                        if (size(searching) == 0)
                            continue;

                        int i = rand() % size(searching);
                        int x = searching[i].first;
                        int y = searching[i].second;
                        
                        power[k]=power[k]/2;

                        pack++;
                        wolvesmap[x][y] = pack;
                        vertices.resize(pack*4);

                        vertices[pack*4-4].position = Vector2f(x*rang,y*rang);
                        vertices[pack*4-3].position = Vector2f((x+1)*rang,y*rang);
                        vertices[pack*4-2].position = Vector2f((x+1)*rang,(y+1)*rang);
                        vertices[pack*4-1].position = Vector2f(x*rang,(y+1)*rang);

                        vertices[pack*4-4].color = Color::Black;
                        vertices[pack*4-3].color = Color::Black;
                        vertices[pack*4-2].color = Color::Black;
                        vertices[pack*4-1].color = Color::Black;

                        position.push_back(make_pair(x,y));
                        srand(clock());
                        power.push_back(wolf_hp/4+(rand()%(wolf_hp/2))); // expected = 15000+(0,30000)

                        searching.clear();
                    }

        }

    void update(Rabbits &zhoverch)
    {
        // Wipe the wolf that stepted on the sun
        for(int i=0;i<mapsize;i++)
            for(int j=0;j<mapsize;j++)
                if (field_of_sun[i][j] > 0 && wolvesmap[i][j] > 0)
                    kill(wolvesmap[i][j]);
        move(zhoverch);
        birth();
    }

public:

    int pack; // Number of wolves
    vector<int> power; // Energy power of wolves
    int wolvesmap[mapsize][mapsize]={0}; // 0 - no wolf, >0 - wolf id

private:

    VertexArray vertices; // Vertex array for drawing
    vector<pair<int,int>> position; // Positions of wolves

    vector<pair<int,int>> searching; // Searching vector for all means
    vector<pair<int,int>> hunting; // Hunting vector for all means
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};