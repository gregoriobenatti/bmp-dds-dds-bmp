//
// Created by Gregorio Benatti on 06/11/2018.
//

#ifndef BMP_DDS_DDS_BMP_BMPFILE_H
#define BMP_DDS_DDS_BMP_BMPFILE_H


class BMPFile {
public:
    BMPFile();
    ~BMPFile();

    static void BMPInit();
    void saveAsBMP();
};


#endif //BMP_DDS_DDS_BMP_BMPFILE_H
