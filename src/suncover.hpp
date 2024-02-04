// Header file for the sun.hpp

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

VertexArray act(Quads,4);

class Suncover: public Drawable
{
public:

    Suncover()
    {
        VertexArray turn_vertices(Quads, mapsize * mapsize * 4); // A turnaround for the vertex array attribute
        vertices = turn_vertices;
    };

    void enrich() // Updates the vertex array, which then can be drawn.
    {
        for (int i = 0; i < mapsize; i++)
            for (int j = 0; j < mapsize; j++)
                {
                    sf::Vertex* quad = &vertices[(i + j * mapsize) * 4]; // A pointer to the current quad

                    // Determine the color of the quad
                    if (field_of_sun[i][j] == 0)
                        {
                            Color colo(Color::Transparent);
                            
                            quad[0].color = colo;
                            quad[1].color = colo;
                            quad[2].color = colo;
                            quad[3].color = colo;
                        }
                    else
                        {
                            Color colo(sungradient[min(int(size(sungradient)-1),div(field_of_sun[i][j],degree).quot)]); // sungradient is a list of colours
    
                            quad[0].color = colo;
                            quad[1].color = colo;
                            quad[2].color = colo;
                            quad[3].color = colo;
                        }

                    quad[0].position = Vector2f(i*rang,j*rang);
                    quad[1].position = Vector2f((i+1)*rang,j*rang);
                    quad[2].position = Vector2f((i+1)*rang,(j+1)*rang);
                    quad[3].position = Vector2f(i*rang,(j+1)*rang);
                }

        // Plot actual value bar
        act[2].position = Vector2f(heigth+200,489-120*actual/expected);
        act[3].position = Vector2f(heigth+150,489-120*actual/expected);

        int n = int(19*actual/expected);

        act[0].color = sungradient[n];
        act[1].color = sungradient[n];
        act[2].color = sungradient[n];
        act[3].color = sungradient[n];

    }

    void lessen()
    {
        for (int i = 0; i < 4*mapsize*mapsize; i++)
                vertices[i].color = Color::Transparent;
    
    }

    ~Suncover() {};

private:

    VertexArray vertices;
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};