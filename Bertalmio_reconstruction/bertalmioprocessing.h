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

    struct List2DInt
    {
        QList< QList<int> > r;
        QList< QList<int> > g;
        QList< QList<int> > b;
    };

    struct List2DFloat
    {
        QList< QList<float> > r;
        QList< QList<float> > g;
        QList< QList<float> > b;
    };

    BertalmioProcessing();

    QImage anisotropicDiffusion(QImage &image);

    List2DFloat gradientInput_10(QImage &image, List2DInt &beta);
    List2DFloat partialResult_5(List2DInt &beta, List2DFloat &gradient);
    List2DInt laplace_7(QImage &image);
    GradientLaplace gradientLaplace_6(List2DInt &laplace);
    IsophoteDirection isophoteDirection_8(QImage &image);
    List2DInt beta_9(GradientLaplace &gradient, IsophoteDirection &isophote);
};

#endif // BERTALMIOPROCESSING_H
