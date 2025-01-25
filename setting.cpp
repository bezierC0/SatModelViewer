#include "setting.h"
#include <stdlib.h>

char Setting::INI_FILE_NAME[255] = "\0";
bool Setting::IS_DISPALY_CLICKED_PMI = false;
bool Setting::IS_DISPALY_FACE = true;
float Setting::Line_Width = 1.0f;
float Setting::Point_Size = 1.0f;
float Setting::Face_Alpha = 1.0f;
float Setting::Line_Alpha = 1.0f;
float Setting::Point_Alpha = 1.0f;
float Setting::COLOR_FACE[3] = { 0.0,0.0,0.0 };
float Setting::COLOR_EDGE[3] = { 0.0,0.0,0.0 };
void Setting::ReadIniFile()
{
    char Path[MAX_PATH + 1];
    if (0 != GetModuleFileNameA(NULL, Path, MAX_PATH))
    {// ���s�t�@�C���̊��S�p�X���擾

        char drive[MAX_PATH + 1]
            , dir[MAX_PATH + 1]
            , fname[MAX_PATH + 1]
            , ext[MAX_PATH + 1];

        _splitpath_s(Path, drive, dir, fname, ext);//�p�X�����\���v�f�ɕ������܂�
        strcat_s(INI_FILE_NAME, drive);
        strcat_s(INI_FILE_NAME, dir);
        strcat_s(INI_FILE_NAME, "setting.ini");
    }

    const int BUFF_SIZE = 255;
    char buf[BUFF_SIZE] = "\0";
    int bufInt = 0;
    bufInt = GetPrivateProfileIntA("DISPLAY", "DisplayClickedPmi", 0, INI_FILE_NAME);
    Setting::IS_DISPALY_CLICKED_PMI = (bufInt == 1 ? true : false);

    bufInt = GetPrivateProfileIntA("DISPLAY", "DisplayFace", 1, INI_FILE_NAME);
    Setting::IS_DISPALY_FACE = (bufInt == 1 ? true : false);

    // Line�T�C�Y
    GetPrivateProfileStringA("DISPLAY", "LineWidth", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::Line_Width = (float)atof(buf);

    // ���_�T�C�Y
    GetPrivateProfileStringA("DISPLAY", "PointSize", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::Point_Size = (float)atof(buf);

    // Face�����x
    GetPrivateProfileStringA("DISPLAY", "FaceAlpha", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::Face_Alpha = (float)atof(buf);

    // Line�����x
    GetPrivateProfileStringA("DISPLAY", "LineAlpha", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::Line_Alpha = (float)atof(buf);

    // ���_�����x
    GetPrivateProfileStringA("DISPLAY", "PointAlpha", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::Point_Alpha = (float)atof(buf);

    // EDGE�̐F�iR�j
    GetPrivateProfileStringA("DISPLAY", "ColorEdgeR", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::COLOR_EDGE[0] = (float)atof(buf);
    GetPrivateProfileStringA("DISPLAY", "ColorEdgeG", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::COLOR_EDGE[1] = (float)atof(buf);
    GetPrivateProfileStringA("DISPLAY", "ColorEdgeB", "1.0", buf, BUFF_SIZE, INI_FILE_NAME);
    Setting::COLOR_EDGE[2] = (float)atof(buf);

}