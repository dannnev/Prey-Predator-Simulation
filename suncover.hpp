// Root header file for the sun.hpp

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Suncover: public Drawable
{
public:

    Suncover()
    {
        VertexArray turn_vertices(Quads, mapsize * mapsize * 4); // A turnaround for the vertex array attribute
        vertices = turn_vertices;
    };

    void enrich(unsigned int field_of_sun[][mapsize])
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
                            Color colo(sungradient[min(15,div(field_of_sun[i][j],15).quot)]);
    
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
    }

    ~Suncover() {};

private:

    VertexArray vertices;
    
    virtual void draw(RenderTarget& target, RenderStates states) const
    {
        target.draw(vertices);
    }
};