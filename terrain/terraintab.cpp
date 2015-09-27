#include "terrain/terraintab.h"



TerrainTab::TerrainTab(const TerrainTab& copy):
        height(copy.height),    width(copy.width),  amplitude(copy.amplitude), hauteurMin(copy.hauteurMin), hauteurMax(copy.hauteurMax)
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
}



Eigen::Vector3f TerrainTab::getNormalXY(float x, float y) const
{
    float ha = getHauteurXY(x,y);
    float   rx = 1/largeur,
            ry = 1/longueur;
    float   xg = std::max(x-rx, 0.f),
            xd = std::min(x+rx, 1.f),
            yb = std::min(y+ry, 1.f),
            yh = std::max(y-ry, 0.f);
    float   g = getHauteurXY(xg,y),
            d = getHauteurXY(xd,y),
            b = getHauteurXY(x,yb),
            h = getHauteurXY(x,yh);
    Eigen::Vector3f vg(-1, 0, g-ha),
                    vd(1, 0, d-ha),
                    vb(0, 1, b-ha),
                    vh(0, -1, h-ha);
    float   distg = vg.norm(),
            distd = vd.norm(),
            distb = vb.norm(),
            disth = vh.norm();
    Eigen::Vector3f v1 = vg.cross(vh),
                    v2 = vh.cross(vd),
                    v3 = vd.cross(vb),
                    v4 = vb.cross(vg);
    Eigen::Vector3f normale(0,0,0);
    if(distg*disth > 0)
        normale += v1.normalized()*distg*disth;
    if(disth*distd > 0)
        normale += v2.normalized()*disth*distd;
    if(distd*distb > 0)
        normale += v3.normalized()*distd*distb;
    if(distb*distg > 0)
        normale += v4.normalized()*distb*distg;
    normale.normalize();
    return normale;
}


/*******************************Image********************************/


TerrainTab::TerrainTab(const QImage &img, float longueur, float largeur, float amplitude):
    Terrain(longueur,largeur,amplitude),    height(img.height()), width(img.width()), amplitude(amplitude)
{
    initGrille();
    simpleInitImage(img);
    updateElevation();
}


TerrainTab::TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float longueur, float largeur, float amplitude):
    Terrain(longueur,largeur,amplitude), height(_nbHeight), width(_nbWidth), amplitude(amplitude)
{
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
    updateElevation();
}

/**construit un terrain avec le même nombre de point que le nombre de pixel de l'image*/
void TerrainTab::simpleInitImage(const QImage& img)
{
    for(int j = 0;  j < height; j++)
        for(int i = 0;  i < width; i++)
            grille2d[j][i] = qGray(img.pixel(i,j))/255.0;
}

void TerrainTab::initGrille()
{
    grille = new float[height*width];
    grille2d = new float*[height];
    for(int j = 0;  j < height; j++)
        grille2d[j] = &grille[j*width];
}

/********************************************************************************************/





float TerrainTab::minElevation() const{
    return hauteurMin; //pas bon
}
float TerrainTab::maxElevation() const{
    return hauteurMax; //pas bon
}
