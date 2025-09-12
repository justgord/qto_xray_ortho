#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;


//   USAGE :    cat xyz.csv | ./qto_xray_ortho  xmin xmax ymin ymax  output_image.png  ds 

//   BUILD :    g++ -std=c++0x qto_xray_ortho.cpp -o qto_xray_ortho output_image.png

//   RUN   :    cat xyz.csv | ./qto_xray_ortho -26.00 -8.00  -7 9   ortho_001.png  0.010


bool endsWith(const std::string& fullString, const std::string& ending) 
{
    if (ending.length() > fullString.length()) {
        return false;
    }
    return fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0;
}

void usage()
{
    cerr << "USAGE : ./qto_xray_ortho  xmin xmax ymin ymax   ortho_image.png  ds_m_per_pixel" << endl;
    exit(-1);
}


int main(int argc, char* argv[])
{

    if (argc<5)
    {
        usage();
    }

    float ll = stof(argv[1]);
    float rr = stof(argv[2]);
    float bb = stof(argv[3]);
    float tt = stof(argv[4]);

    cerr << "l r b t   : " << fixed << setprecision(3) << ll << " " << rr << " " << bb << " " << tt << endl;

    float ww = (rr-ll);
    float hh = (tt-bb);
    cerr << "ww x hh m : " << ww << " x " << hh << endl;

    string img_file_png = "ortho.png";
    if (argc>5)
    {
        string sfil = argv[5];
        if (!endsWith(sfil, ".png"))
        {
            cerr << "bad output file name : " << sfil << endl;
            usage();
        }
        img_file_png = sfil; 
    }

    float m_per_pix = 0.010;   // 1cm pixels by default

    if (argc>6) 
    {   
        float mpp = stof(argv[6]);
        if (mpp<0.0001 || mpp > 100.00)
        {
            cerr << "meters per pixel : out of range" << endl;
            usage();
        }
        m_per_pix = mpp;
        
    }
    const float pix_per_m = 1.0 / m_per_pix;

    cerr << "pix_per_m : " << fixed << setprecision(3) << pix_per_m << endl;
    cerr << "m_per_pix : " << fixed << setprecision(6) << m_per_pix << endl;

    int W = ceil(ww * pix_per_m);
    int H = ceil(hh * pix_per_m);

    cerr << "W x H pix : " << W << " x " << H << endl;

    // stream and count pts per x y pixel bin

    vector<uint16_t> buf(W*H, 0);

    {

    string line;
    int n=0;
    int oob=0;
    while (getline(cin, line)) 
    {
        stringstream ss(line);
        string value;
        vector<float> row;
        n++;

        while (getline(ss, value, ',')) 
        {
            row.push_back(stof(value));
        }
    
        float x = row[0];
        float y = row[1];
        //cerr << "x y : " << fixed << setprecision(3) << x << " " << y << endl;

        if (x<ll || x>rr || y<bb || y>tt)
        {
            oob++;
        }
        else
        {
            int i = round((x-ll) * pix_per_m );
            int j = round((y-bb) * pix_per_m);

            if (i<0 || j<0 || i>=W || j>=H)
                cerr << "BAD index" << endl;
            
            // cerr << "i j : " << i << " " << j << endl; 
            
            int ix = j*W+i;
            // cerr << " ix : " << ix << endl;

            buf[ix]++;
        }

    }
    if (oob)
        cerr << "ignored oobs : " << oob << endl;
        
    }

    // get max count so we can normalize to 0..255 color range

    uint16_t npmax=0;
    {
    for (int j=0;j<H;j++)
    {
        for (int i=0;i<W;i++)
        {
            int ix = j*W+i;
            uint16_t np = buf[ix];
            if (np>npmax)
                npmax=np;
        }
    }
    cerr << "max count : " << npmax << endl;
    }

    // normalize [0..npmax] -> 0..255 in some color gradient scheme, grayscale for now
    // write rgb array, save as image


    /// write rgb buffer, save to png file

{
    int width = W;
    int height = H;
    std::vector<unsigned char> data(width * height * 3); // R, G, B for each pixel

    for (int j=0;j<H;j++)
    {
        for (int i=0;i<W;i++)
        {
            int ix = j*W+i;
            uint16_t np = buf[ix];

            int cg = floor(255*np/npmax);

            int jj = H-j-1;                     // image y inverted
            int cix = 3*(jj*W+i);

            data[cix+0]=cg;
            data[cix+1]=cg;
            data[cix+2]=cg;
        }
    }

    cerr << "writing   : " << img_file_png << endl;

    stbi_write_png(img_file_png.c_str(), width, height, 3, data.data(), width * 3);
}
}

        



