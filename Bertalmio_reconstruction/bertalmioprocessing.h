#ifndef BERTALMIOPROCESSING_H
#define BERTALMIOPROCESSING_H

#include <QImage>
#include <QList>

class BertalmioProcessing
{
public:
    BertalmioProcessing();

    struct Element
    {
        int x;
        int y;

        Element(int x_h, int y_h) : x(x_h), y(y_h) {}
    };

    struct GradientLaplace
    {
        QList< QList<Element> > r;
        QList< QList<Element> > g;
        QList< QList<Element> > b;
    };

    QImage anisotropicDiffusion(QImage &image);
    QImage laplace(QImage &image);
    QImage gradientInput(QImage &image);

    GradientLaplace gradientLaplace(QImage &image);
};

#endif // BERTALMIOPROCESSING_H
