#ifndef BERTALMIOPROCESSING_H
#define BERTALMIOPROCESSING_H

#include <QImage>

class BertalmioProcessing
{
public:
    BertalmioProcessing();

    QImage anisotropicDiffusion(QImage &image);
    QImage laplace(QImage &image);
    QImage gradient(QImage &image);
};

#endif // BERTALMIOPROCESSING_H
