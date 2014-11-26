#ifndef BERTALMIOPROCESSING_H
#define BERTALMIOPROCESSING_H

#include <QImage>
#include <QList>

class BertalmioProcessing
{
public:
    struct Element
    {
        int x;
        int y;

        Element(int x_h, int y_h) : x(x_h), y(y_h) {}
    };

    struct ElementFloat
    {
        float x;
        float y;

        ElementFloat(float x_h, float y_h) : x(x_h), y(y_h) {}
    };

    struct GradientLaplace
    {
        QList< QList<Element> > r;
        QList< QList<Element> > g;
        QList< QList<Element> > b;
    };

    struct IsophoteDirection
    {
        QList< QList<ElementFloat> > r;
        QList< QList<ElementFloat> > g;
        QList< QList<ElementFloat> > b;
    };

    BertalmioProcessing();

    QImage anisotropicDiffusion(QImage &image);
    QImage laplace(QImage &image);
    QImage gradientInput(QImage &image);

    GradientLaplace gradientLaplace(QImage &image);
    IsophoteDirection isophoteDirection(QImage &image);
};

#endif // BERTALMIOPROCESSING_H
