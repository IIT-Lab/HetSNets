//
// Created by lee on 17-7-28.
//

#ifndef HETSNETS_IMPORTEXPORT_H
#define HETSNETS_IMPORTEXPORT_H

#include <iostream>
#include <fstream>
#include "Interface.h"

using namespace std;

class ImportExport
{
private:
    static ImportExport* _Instance;
protected:
    ImportExport();                         //Constructor
public:
    static ImportExport& GetInstance();
    void GenerateFileName();
    void GenerateSenseFile();
    void system_config();
    void SetScene();
    void PushSceneToMySQL();
    void SetSceneByMySQL();
    void SetPtr2Bus(Interface* _InterfaceTemp);
    static ofstream fout;                 //将要写的文件定义为静态变量，方便多个类对文件进行写操作
};

#endif //HETSNETS_IMPORTEXPORT_H
