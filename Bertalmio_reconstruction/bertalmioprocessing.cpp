#include <QImage>

#include "bertalmioprocessing.h"

BertalmioProcessing::BertalmioProcessing()
{
}

QImage BertalmioProcessing::anisotropicDiffusion(QImage &image)
{
    QImage result(image);

    return result;
}

QImage BertalmioProcessing::laplace(QImage &image)
{
    QImage result(image);

    /*
     * 0  1  0
     * 1 -4  1
     * 0  1  0
     */

    for (int i=1; i < image.width()-1; i++) // ignore boundaries
    {
        for (int o=1; o < image.height()-1; o++) // ignore boundaries
        {
            int valueR = qRed(image.pixel(i, o-1)) + qRed(image.pixel(i-1, o)) + qRed(image.pixel(i+1, o)) + qRed(image.pixel(i, o+1)) - 4 * qRed(image.pixel(i, o));
            int valueG = qGreen(image.pixel(i, o-1)) + qGreen(image.pixel(i-1, o)) + qGreen(image.pixel(i+1, o)) + qGreen(image.pixel(i, o+1)) - 4 * qGreen(image.pixel(i, o));
            int valueB = qBlue(image.pixel(i, o-1)) + qBlue(image.pixel(i-1, o)) + qBlue(image.pixel(i+1, o)) + qBlue(image.pixel(i, o+1)) - 4 * qBlue(image.pixel(i, o));

            valueR = qMin(255, valueR); valueR = qMax(0, valueR);
            valueG = qMin(255, valueG); valueG = qMax(0, valueG);
            valueB = qMin(255, valueB); valueB = qMax(0, valueB);

            result.setPixel(i, o, qRgb(valueR, valueG, valueB));
        }
    }

    return result;
}

BertalmioProcessing::GradientLaplace BertalmioProcessing::gradientLaplace(QImage &image)
{
    GradientLaplace result;

    for (int o=1; o < image.height()-1; o++) // ignore boundaries
    {
        QList<Element> rowR;
        QList<Element> rowG;
        QList<Element> rowB;

        for (int i=1; i < image.width()-1; i++) // ignore boundaries
        {
            int xR = qRed(image.pixel(i+1, o)) - qRed(image.pixel(i-1, o));
            int yR = qRed(image.pixel(i, o+1)) - qRed(image.pixel(i, o-1));
            int xG = qGreen(image.pixel(i+1, o)) - qGreen(image.pixel(i-1, o));
            int yG = qGreen(image.pixel(i, o+1)) - qGreen(image.pixel(i, o-1));
            int xB = qBlue(image.pixel(i+1, o)) - qBlue(image.pixel(i-1, o));
            int yB = qBlue(image.pixel(i, o+1)) - qBlue(image.pixel(i, o-1));

            rowR.append(Element(xR, yR));
            rowG.append(Element(xG, yG));
            rowB.append(Element(xB, yB));
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    return result;
}

QImage BertalmioProcessing::gradientInput(QImage &image)
{
    QImage result(image);

    return result;
}
