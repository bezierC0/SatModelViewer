#include <Windows.h>
#include <stdlib.h>

class Setting
{
public:
    static char INI_FILE_NAME[255];
    static bool IS_DISPALY_CLICKED_PMI;
    static bool IS_DISPALY_FACE;
    static float Line_Width;
    static float Point_Size;
    static float Face_Alpha;
    static float Line_Alpha;
    static float Point_Alpha;
    static float COLOR_FACE[3];
    static float COLOR_EDGE[3];
    static void ReadIniFile();
};
