// Header for main.cpp

const int wolf_hp = 60000; // Health points
const int wolf_birth = 55000; // Birth threshold
const int wolf_feed = rabbit_hp/40; // Energy gain from eating a rabbit

int wolf_regime = 0; // To keep track of wolves movement

class Wolves : public Drawable
{
public:
    
    Wolves(int population = int(mapsize/3))
    {
        start(population);
    }

    // Start the wolves
    void start(int population)
    {
        VertexArray turn_vertices(Quads,4*population); // A turnaround for the vertex array attribute

        for (int i=0;i<population;i++)
            {
                for(int j=0;j<4;j++){turn_vertices[j+i*4].color = wolf_fur;} // Set the color of the vertex

                srand(clock()); // Randomize the seed

                int stx = rand()%mapsize;
                int sty = rand()%mapsize;
                while (ghost[stx][sty] != 0 && field_of_sun[stx][sty] != 0) // Avoid initial position to be already occupied
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
                power.push_back(wolf_hp/2);
                wolvesmap[stx][sty] = i+1;
                ghost[stx][sty]=1;
            }

        // Attributes
        pack=population;
        vertices = turn_vertices;

        // Plot
        timeplot.setPrimitiveType(LinesStrip);
        for (int i=0;i<points_horizontal;i++)
            timeplot.append(Vertex(Vector2f(i+heigth+26,heigth-22),Color::Transparent));

        // Phase plot

        phaseplot.setPrimitiveType(LineStrip);
        phaseplot.resize(phase_length);

        for (int i=0;i<phase_length;i++)
            {phaseplot[i] = Vertex(Vector2f(heigth+26,heigth-104));
             phaseplot[i].color = Color::Transparent;}
    }

    // Move the wolf
    void move(Rabbits &zho)
    {
        for(int k=0;k<pack;k++)
        {
            int x = position[k].first; // Current position x
            int y = position[k].second; // Current position y
            int temp; // Temporary variable for swapping

            for (int i=-1; i<2; i++)
                for(int j=-1; j<2; j++)
                    // If the cell is empty and not in the sun add it to the searching vector
                    if(wolvesmap[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0 && field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                        searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,((position[k].second+j+mapsize)%mapsize)));

            if (searching.size() == 0)
                continue;

            // Determine the hunting vector
            for (int i=0; i<searching.size(); i++)
                if (zho.rabbitsmap[searching[i].first][searching[i].second] > 0)
                    hunting.push_back(make_pair(searching[i].first,searching[i].second));

            // If there are no rabbits in the hunting vector, move randomly
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
                    feed(k); // Feed the wolf
                    /*Kill the rabbit (note rabbits id in the rabbitsmap are (1,fluffle), but in attributes id is in (0,fluffle-1)*/
                    zho.kill(zho.rabbitsmap[x][y]-1);
                }

            // Swap the wolvesmap and ghost
            temp = wolvesmap[x][y];
            wolvesmap[x][y] = wolvesmap[position[k].first][position[k].second];
            wolvesmap[position[k].first][position[k].second] = temp;

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
            hunting.clear();        
        }

    }

    // Kill the wolf and do cosmetics on the class attributes
    void kill(int id)
    {
        // All wolves with id> dead wolf id, decrease their id by 1
        for(int i=id+1;i<pack;i++)
            wolvesmap[position[i].first][position[i].second]--;

        // Dead wolf id appears as 0 in the wolvesmap and ghost
        wolvesmap[position[id].first][position[id].second] = 0;
        ghost[position[id].first][position[id].second] = 0;

        // Decrease the number of wolves
        pack--;

        // Move all the vertices by 4 to the left
        for (int i=id*4;i<pack*4;i++)
            vertices[i] = vertices[i+4];

        vertices.resize(pack*4);
    
        // Remove the dead wolf from the position and power vectors
        position.erase(position.begin() + id);
        power.erase(power.begin() + id);
    }

    // Feed the wolf
    void feed(int id)
    {
        power[id]+=wolf_feed;
        power[id]=min(power[id],wolf_hp); // Upper bound of energy
    }

    // Birth of the wolf
    void birth()
    {
        for (int k=0; k<pack; k++)
            if (power[k] > wolf_birth)
                {
                    for (int i=-1; i<2; i++)
                        for(int j=-1; j<2; j++)
                            // If the cell is empty and not in the sun add it to the searching vector
                            if(ghost[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0 && field_of_sun[(position[k].first+i+mapsize)%mapsize][(position[k].second+j+mapsize)%mapsize] == 0)
                                searching.push_back(make_pair((position[k].first+i+mapsize)%mapsize,(position[k].second+j+mapsize)%mapsize));
                    
                    if (size(searching) == 0)
                        continue;

                    int i = rand() % size(searching); // Randomly choose the cell
                    int x = searching[i].first;
                    int y = searching[i].second;
                    
                    power[k]=power[k]/2; // Half the energy
                    pack++; // Increase the number of wolves
                    wolvesmap[x][y] = pack; // Set the wolvesmap
                    ghost[x][y]=1; // Set the ghost

                    vertices.resize(pack*4);

                    vertices[pack*4-4].position = Vector2f(x*rang,y*rang);
                    vertices[pack*4-3].position = Vector2f((x+1)*rang,y*rang);
                    vertices[pack*4-2].position = Vector2f((x+1)*rang,(y+1)*rang);
                    vertices[pack*4-1].position = Vector2f(x*rang,(y+1)*rang);

                    vertices[pack*4-4].color = wolf_fur;
                    vertices[pack*4-3].color = wolf_fur;
                    vertices[pack*4-2].color = wolf_fur;
                    vertices[pack*4-1].color = wolf_fur;

                    // Store the position and power of the new wolf
                    position.push_back(make_pair(x,y));
                    power.push_back(wolf_hp/2);

                    searching.clear();
                }

    }

    // Plot the population graph
    void plotter(Rabbits &zho)
    {
        // Move the plot to the left
        for (int i=0;i<points_horizontal-1;i++)
            {timeplot[i].position.y = timeplot[i+1].position.y;
            timeplot[i].color = timeplot[i+1].color;}

        // Plot the last point
        timeplot[points_horizontal-1] = Vertex(Vector2f(heigth+26+points_horizontal,heigth-22-min((points_vertical-1)*pack/absolute_y,points_vertical-1)),wolf_plot);
    
        // Move the phase plot "snake"
        for (int i=0;i<phase_length-1;i++)
            {phaseplot[i] = phaseplot[i+1];
            phaseplot[i].color = phase_color;}
        
        phaseplot[phase_length-1] = Vertex(Vector2f(heigth+27+min(points_horizontal-1,(points_horizontal-1)*pack/absolute_x), heigth-105-min(points_horizontal-1,(points_horizontal-1)*zho.fluffle/absolute_y)));
    
        if (pack > absolute_x)
            graphright.setString(to_string(pack));
        else
            graphright.setString(to_string(absolute_x));
    }

    // Complete update of the wolves
    void update(Rabbits &zho)
    {
        // Wipe the wolf that stepted on the sun
        for(int i=0;i<pack;i++)
            {
                power[i]-=speed; // Cost of life
                power[i]=min(power[i],wolf_hp); // Upper bound of energy
                if (field_of_sun[position[i].first][position[i].second] > 0 || power[i] < 0)
                    kill(i);
            }
        move(zho);
        birth();
        plotter(zho);
    }

    // Restart the wolves
    void restart(int population = int(mapsize/3))
    {
        // Erase everything
        vertices.clear();
        position.clear();
        searching.clear();
        hunting.clear();
        timeplot.clear();
        phaseplot.clear();
        power.clear();

        memset(wolvesmap,0,sizeof(wolvesmap));

        // Start again
        start(population);
    }
public:

    VertexArray timeplot;
    VertexArray phaseplot; // Phase plot for the rabbits and wolves
    vector<int> power; // Energy power of wolves
    int pack; // Number of wolves
    int wolvesmap[mapsize][mapsize]={0}; // 0 - no wolf, >0 - wolf id

private:

    VertexArray vertices; // Vertex array for drawing
    vector<pair<int,int>> position; // Positions of wolves
    vector<pair<int,int>> searching; // Searching vector for means of movement and birth
    vector<pair<int,int>> hunting; // Hunting vector for means of finding a rabbit
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};