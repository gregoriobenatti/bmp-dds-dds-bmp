//
// Created by Gregorio Benatti on 06/11/2018.
//

#ifndef BMP_DDS_DDS_BMP_DDSFILE_H
#define BMP_DDS_DDS_BMP_DDSFILE_H


class DDSFile {
public:
    DDSFile();
    ~DDSFile();

    static void DDSInit();
    void saveAsDDS();
};


#endif //BMP_DDS_DDS_BMP_DDSFILE_H
