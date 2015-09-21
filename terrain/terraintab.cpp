#include "terrain/terraintab.h"



TerrainTab::TerrainTab(const TerrainTab& copy):
        height(copy.height),    width(copy.width),  amplitude(copy.amplitude)
{
    longueur = copy.longueur;
    largeur = copy.largeur;
    box = copy.box;
    grille = new float[height*width];
    for(int i = 0;  i < height*width;   i++)
        grille[i] = copy.grille[i];
    grille2d = new float*[height];
    for(int j = 0;  j < height; j++)
        grille2d[j] = &grille[j*width];
}

TerrainTab::~TerrainTab()
{
    if(grille != nullptr)
    {
        delete[] grille;
        for(int j = 0;  j < height; j++)
            delete[] grille2d;
    }
}

inline float TerrainTab::get(int x, int y) const
{
    return grille2d[y][x];
}

float TerrainTab::getHauteurXY(float x, float y) const
{
    if(x < 0 || y < 0 || x > 1 || y > 1)
        return HAUTEUR_HORS_MAP;


    x *= (width-1),    //largeur: 1m et 5 points: (1.0f*(5-1))/1 = 4.0f donc regarder l'indice 4
    y *= (height-1);

    int x1 = floorf(x),
        y1 = floorf(y),
        x2 = ceilf(x),
        y2 = ceilf(y);

    const float h11 = get(x1, y1);

    if(y1 == y2)    {
        if(x1 == x2 )
            return h11*amplitude;
        return interp::interp_linear1D(x, h11, get(x2, y1))*amplitude;
    }
    else    {
        float h12 = get(x1, y2);
        if(x1 == x2 )
            return interp::interp_linear1D(x, h11, h12)*amplitude;
        return interp::interp_linear2D(x, y, x1, y1, x2, y2,
                                       h11, h12, get(x2, y1), get(x2, y2))*amplitude;
    }

    getNormal(x,y);
}

/*******************************Image********************************/


TerrainTab::TerrainTab(const QImage &img, float amplitude):
    Terrain2(1000,1000),    height(img.height()), width(img.width()), amplitude(amplitude)
{
    box = Box(Vector3f(0,0,0),Vector3f(1000,1000,amplitude));
    initGrille();
    simpleInitImage(img);
}


TerrainTab::TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float _amplitude):
    Terrain2(1000,1000), height(_nbHeight), width(_nbWidth), amplitude(_amplitude)
{
    box = Box(Vector3f(0,0,0),Vector3f(1000,1000,_amplitude));
    initGrille();
    if(_nbHeight == img.height() && _nbWidth == img.width())
        simpleInitImage(img);
    else
    {
        for(int j = 0;  j < _nbHeight;   j++)
        {
            float y = j*(float)(img.height()-1)/(_nbHeight-1);
            int y1 = floorf(y), y2 = ceilf(y);
            for(int i = 0;  i < _nbWidth;    i++)
            {
                float x = i*(float)(img.width()-1)/(_nbWidth-1);
                int x1 = floorf(x), x2 = ceilf(x);
                float z = interp::interp_hermite2D(x,y,
                                                  x1,y1,x2,y2,
                                                  qGray(img.pixel(x1, y1))/255.0,
                                                  qGray(img.pixel(x1, y2))/255.0,
                                                  qGray(img.pixel(x2, y1))/255.0,
                                                  qGray(img.pixel(x2, y2))/255.0
                                                  );
                grille2d[j][i] = z;
            }
        }
    }
}

/**construit un terrain avec le même nombre de point que le nombre de pixel de l'image*/
void TerrainTab::simpleInitImage(const QImage& img)
{
    for(int i = 0;  i < height*width; i++)
        grille[i] = qGray(img.color(i))/255.0;
}

void TerrainTab::initGrille()
{
    grille = new float[height*width];
    grille2d = new float*[height];
    for(int j = 0;  j < height; j++)
        grille2d[j] = &grille[j*width];
}


void TerrainTab::getNormal(float pointX, float pointY) const
{
    float x = pointX * (width-1),    //largeur: 1m et 5 points: (1.0f*(5-1))/1 = 4.0f donc regarder l'indice 4
          y = pointY * (height-1);

    const float p11 = get(floorf(x), floorf(y)),
                p22 = get(ceilf(x), ceilf(y));
                            /*& normal11 = getN(floorf(x), floorf(y)),
                            & normal12 = getN(floorf(x), ceilf(y)),
                            & normal21 = getN(ceilf(x), floorf(y)),
                            & normal22 = getN(ceilf(x), ceilf(y));

    Eigen::Vector3f normal;
    normal(0) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(0), normal12(0), normal21(0), normal22(0));
    normal(1) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(1), normal12(1), normal21(1), normal22(1));
    normal(2) = interp::interp_linear2D(pointX, pointY, p11(0), p11(1), p22(0), p22(1),
                                   normal11(2), normal12(2), normal21(2), normal22(2));

    normal.normalize();

    return normal;*/
}
