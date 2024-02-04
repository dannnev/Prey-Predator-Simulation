// Header for main.cpp

const int rabbit_hp = 60000; // Health points
const int rabbit_birth = 55000; // Birth threshold
const int rabbit_variance = 10; // Variance of movement

int rabbit_regime = 0; // To keep track of rabbits movement 
int gif_regime=0;

int speed = 20; // Speed of animation
int gif_speed=5*speed;
int frame=8;

/* Here the definition of ghost map is redundant as Wolves and Rabbits classes posses
the similar attribute (wolvesmap and rabbitsmap). However to efectively use them, classes
would have to point to each other, which results in either forward declaration loop or
inconsistent code. Can be improved in the future.
*/
int ghost[mapsize][mapsize] = {}; // Ghost array for the rabbits and wolves (0 - no animal, 1 - animal)

Text graphtop, graphright; // Top of the graph

class Rabbits : public Drawable
{
public:

    Rabbits(int population = int(mapsize/2))
    {
        start(population);
    }

    // Start the rabbits
    void start(int population)
    {
        VertexArray turn_vertices(Quads,4*population); // A turnaround variable for passing to the vertex array attribute

        for (int i=0;i<population;i++)
            {
                for(int j=0;j<4;j++){turn_vertices[j+i*4].color = rabbit_fur;} // Set the color of the vertex

                srand(clock()); // Randomize the seed

                int stx = rand()%mapsize;
                int sty = rand()%mapsize;
                while (ghost[stx][sty] != 0) // Avoid initial position to be already occupied
                {
                    stx = rand()%mapsize;
                    sty = rand()%mapsize;
                }

                // The quad represents a cell on the screen with the size of rang
                turn_vertices[0+i*4].position = Vector2f(stx*rang,sty*rang);
                turn_vertices[1+i*4].position = Vector2f((stx+1)*rang,sty*rang);
                turn_vertices[2+i*4].position = Vector2f((stx+1)*rang,(sty+1)*rang);
                turn_vertices[3+i*4].position = Vector2f(stx*rang,(sty+1)*rang);

                // Attributes
                position.push_back(make_pair(stx,sty));
                power.push_back(rabbit_hp/2);
                rabbitsmap[stx][sty] = i+1;
                ghost[stx][sty]=1;
            }

        // Attributes
        fluffle=population;
        vertices = turn_vertices;

        // Plot
        timeplot.setPrimitiveType(LinesStrip);
        for (int i=0;i<points_horizontal;i++)
            timeplot.append(Vertex(Vector2f(i+heigth+26,heigth-22),Color::Transparent));        
    }

    // Move the rabbit
    void move()
    {
        for (int k=0;k<fluffle;k++)
        {
            int x = position[k].first; // Current position x
            int y = position[k].second; // Current position y
            int best = field_of_sun[x][y]; // Set the best value to be current cell value
            int n; // Index of the searching vector
            int temp; // Temporary variable for swapping

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++) 
                    // If the cell is empty add the coordinates to the searching vector
                    if (ghost[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                        {
                            searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                            
                            // If the cell has a higher value than the best, set the best to be the value of the cell
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

            // If the best cell is the same as the current cell && variance of movement, or best=0, do random movement
            if (best == 0 || (best == field_of_sun[position[k].first][position[k].second] && (rabbit_variance > rand()%100)))
                {
                    n = rand() % size(searching);
                    x = searching[n].first;
                    y = searching[n].second;
                }

            // Swap the rabbits and ghost maps
            temp = rabbitsmap[x][y];
            rabbitsmap[x][y] = rabbitsmap[position[k].first][position[k].second];
            rabbitsmap[position[k].first][position[k].second] = temp;

            temp = ghost[x][y];
            ghost[x][y] = ghost[position[k].first][position[k].second];
            ghost[position[k].first][position[k].second] = temp;

            // Renew attributes
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
        // All the rabbits that posses the id higher than the killed rabbit, decrease their id by 1
        for(int i=id+1;i<fluffle;i++)
            rabbitsmap[position[i].first][position[i].second]--;

        // Killed rabbits id changes to 0
        rabbitsmap[position[id].first][position[id].second] = 0;
        ghost[position[id].first][position[id].second] = 0;

        // Decrease population
        fluffle--;
        
        // Starting from the killed rabbit, move all the vertices by 4 to the left
        for (int i=id*4;i<fluffle*4;i++)
            vertices[i] = vertices[i+4];

        // Resize the vertices (so new vertices are 4 vertexes shorter than the previous one)
        vertices.resize(fluffle*4);
    
        // Remove the killed rabbit from the position and power vectors
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
                power[i]-=speed; // Cost of life
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
                    // Make a searching vector for all the free cells around the rabbit
                    for (int i=-1; i<2; i++)
                        for(int j=-1; j<2; j++)
                            if(ghost[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                                searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                    
                    if (size(searching) == 0)
                        continue;

                    int i = rand() % size(searching); // Random cell from the searching vector
                    int x = searching[i].first;
                    int y = searching[i].second;
                    
                    power[k]=power[k]/2; // Half the energy

                    fluffle++; // Increase the population
                    rabbitsmap[x][y] = fluffle; // Set the rabbitsmap coordinate to new rabbits id
                    ghost[x][y]=1; // Set the ghost coordinate to 1
                    vertices.resize(fluffle*4); // Resize the vertices

                    // Set the new quad of the rabbit
                    vertices[fluffle*4-4].position = Vector2f(x*rang,y*rang);
                    vertices[fluffle*4-3].position = Vector2f((x+1)*rang,y*rang);
                    vertices[fluffle*4-2].position = Vector2f((x+1)*rang,(y+1)*rang);
                    vertices[fluffle*4-1].position = Vector2f(x*rang,(y+1)*rang);

                    vertices[fluffle*4-4].color = rabbit_fur;
                    vertices[fluffle*4-3].color = rabbit_fur;
                    vertices[fluffle*4-2].color = rabbit_fur;
                    vertices[fluffle*4-1].color = rabbit_fur;

                    // Store the position and power of the new rabbit
                    position.push_back(make_pair(x,y));
                    power.push_back(rabbit_hp/2);

                    searching.clear();
                }

    }

    // Plot the population graph
    void plotter()
    {
        if (fluffle > absolute_y)
            graphtop.setString(to_string(fluffle));
        else
            graphtop.setString(to_string(absolute_y));

        // Move the plot to the left
        for (int i=0;i<points_horizontal-1;i++)
            {timeplot[i].position.y = timeplot[i+1].position.y;
            timeplot[i].color = timeplot[i+1].color;}

        timeplot[points_horizontal-1] = Vertex(Vector2f(heigth+26+points_horizontal,heigth-22-min(points_vertical*fluffle/absolute_y,points_vertical)),rabbit_plot);
    }

    // Complete update of the rabbits
    void update()
    {
        move();
        synthes();
        birth();
        plotter();
    }

    // Restart the rabbits
    void restart(int population = int(mapsize/2))
    { 
        // Erase everything
        vertices.clear();
        position.clear();
        searching.clear();
        timeplot.clear();
        power.clear();

        memset(rabbitsmap,0,sizeof(rabbitsmap));
        memset(ghost,0,sizeof(ghost));

        // Start again
        start(population);
    }

    ~Rabbits() {};
public:

    VertexArray timeplot;
    vector<int> power; // Energy power of rabbits
    int fluffle; // Population number of rabbits
    int rabbitsmap[mapsize][mapsize]={0}; // 0 - no rabbit, >0 - id of rabbit

private:

    VertexArray vertices; // Vertex array for drawing
    vector<pair<int,int>> position; // Positions of rabbits
    vector<pair<int,int>> searching; // Searching vector for means of movement and birth
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};