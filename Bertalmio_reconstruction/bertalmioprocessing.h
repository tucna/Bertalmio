#ifndef BERTALMIOPROCESSING_H
#define BERTALMIOPROCESSING_H

#include <QImage>
#include <QList>

class BertalmioProcessing
{
public:
    struct ElementFloat
    {
        float x;
        float y;

        ElementFloat(float x_h, float y_h) : x(x_h), y(y_h) {}
    };

    struct GradientLaplace
    {
        QList< QList<ElementFloat> > r;
        QList< QList<ElementFloat> > g;
        QList< QList<ElementFloat> > b;
    };

    struct IsophoteDirection
    {
        QList< QList<ElementFloat> > r;
        QList< QList<ElementFloat> > g;
        QList< QList<ElementFloat> > b;
    };

    struct List2DFloat
    {
        QList< QList<float> > r;
        QList< QList<float> > g;
        QList< QList<float> > b;
    };

    BertalmioProcessing();

    void anisotropicDiffusion_3(List2DFloat &imageFloat);
    void updateImage_4(List2DFloat &imageFloat, const List2DFloat &partialResult);

    List2DFloat gradientInput_10(const List2DFloat &imageFloat, const List2DFloat &beta);
    List2DFloat partialResult_5(const List2DFloat &beta, const List2DFloat &gradient);
    List2DFloat laplace_7(const List2DFloat &imageFloat);
    GradientLaplace gradientLaplace_6(const List2DFloat &laplace);
    IsophoteDirection isophoteDirection_8(const List2DFloat &imageFloat);
    List2DFloat beta_9(const GradientLaplace &gradient, const IsophoteDirection &isophote);

    bool stabilityTest(const List2DFloat &partialResult);
    List2DFloat imageToFloat(const QImage &image);
    QImage floatToImage(const List2DFloat &imageFloat);
};

#endif // BERTALMIOPROCESSING_H
