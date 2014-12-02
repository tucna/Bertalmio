#include <QImage>
#include <QDebug>

#include <qmath.h>

#include "bertalmioprocessing.h"

BertalmioProcessing::BertalmioProcessing()
{
}

QImage BertalmioProcessing::anisotropicDiffusion(QImage &image)
{
    const float KAPPA = 30.0;

    QImage result(image);
    List2DFloat imageFloat;
    QList<float> rowTemp;

    for (int i = 0; i < image.width(); i++)
    {
        rowTemp.append(0);
    }

    imageFloat.r.append(rowTemp);
    imageFloat.g.append(rowTemp);
    imageFloat.b.append(rowTemp);

    for (int o = 1; o < image.height() - 1; o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        for (int i = 1; i < image.width() - 1; i++)
        {
            // nabla
            int nablaN_R = qRed(image.pixel(i-1, o)) - qRed(image.pixel(i, o));
            int nablaS_R = qRed(image.pixel(i+1, o)) - qRed(image.pixel(i, o));
            int nablaE_R = qRed(image.pixel(i, o+1)) - qRed(image.pixel(i, o));
            int nablaW_R = qRed(image.pixel(i, o-1)) - qRed(image.pixel(i, o));

            int nablaN_G = qGreen(image.pixel(i-1, o)) - qGreen(image.pixel(i, o));
            int nablaS_G = qGreen(image.pixel(i+1, o)) - qGreen(image.pixel(i, o));
            int nablaE_G = qGreen(image.pixel(i, o+1)) - qGreen(image.pixel(i, o));
            int nablaW_G = qGreen(image.pixel(i, o-1)) - qGreen(image.pixel(i, o));

            int nablaN_B = qBlue(image.pixel(i-1, o)) - qBlue(image.pixel(i, o));
            int nablaS_B = qBlue(image.pixel(i+1, o)) - qBlue(image.pixel(i, o));
            int nablaE_B = qBlue(image.pixel(i, o+1)) - qBlue(image.pixel(i, o));
            int nablaW_B = qBlue(image.pixel(i, o-1)) - qBlue(image.pixel(i, o));

            // c - gauss
            float cN_R = qExp(-qPow(nablaN_R/KAPPA,2));
            float cS_R = qExp(-qPow(nablaS_R/KAPPA,2));
            float cE_R = qExp(-qPow(nablaE_R/KAPPA,2));
            float cW_R = qExp(-qPow(nablaW_R/KAPPA,2));

            float cN_G = qExp(-qPow(nablaN_G/KAPPA,2));
            float cS_G = qExp(-qPow(nablaS_G/KAPPA,2));
            float cE_G = qExp(-qPow(nablaE_G/KAPPA,2));
            float cW_G = qExp(-qPow(nablaW_G/KAPPA,2));

            float cN_B = qExp(-qPow(nablaN_B/KAPPA,2));
            float cS_B = qExp(-qPow(nablaS_B/KAPPA,2));
            float cE_B = qExp(-qPow(nablaE_B/KAPPA,2));
            float cW_B = qExp(-qPow(nablaW_B/KAPPA,2));

            //rowR.append(valueR);
            //rowG.append(valueG);
            //rowB.append(valueB);
        }

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        imageFloat.r.append(rowR);
        imageFloat.g.append(rowG);
        imageFloat.b.append(rowB);
    }

    imageFloat.r.append(rowTemp);
    imageFloat.g.append(rowTemp);
    imageFloat.b.append(rowTemp);

    return result;
}

BertalmioProcessing::List2DInt BertalmioProcessing::laplace_7(QImage &image)
{
    List2DInt result;
    QList<int> rowTemp;

    /*
     * 0  1  0
     * 1 -4  1
     * 0  1  0
     */    

    for (int i = 0; i < image.width(); i++)
    {
        rowTemp.append(0);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < image.height() - 1; o++)
    {
        QList<int> rowR;
        QList<int> rowG;
        QList<int> rowB;

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        for (int i = 1; i < image.width() - 1; i++)
        {
            int valueR = qRed(image.pixel(i, o-1)) + qRed(image.pixel(i-1, o)) + qRed(image.pixel(i+1, o)) + qRed(image.pixel(i, o+1)) - 4 * qRed(image.pixel(i, o));
            int valueG = qGreen(image.pixel(i, o-1)) + qGreen(image.pixel(i-1, o)) + qGreen(image.pixel(i+1, o)) + qGreen(image.pixel(i, o+1)) - 4 * qGreen(image.pixel(i, o));
            int valueB = qBlue(image.pixel(i, o-1)) + qBlue(image.pixel(i-1, o)) + qBlue(image.pixel(i+1, o)) + qBlue(image.pixel(i, o+1)) - 4 * qBlue(image.pixel(i, o));

            rowR.append(valueR);
            rowG.append(valueG);
            rowB.append(valueB);
        }

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    return result;
}

BertalmioProcessing::GradientLaplace BertalmioProcessing::gradientLaplace_6(List2DInt &laplace)
{
    GradientLaplace result;    
    QList<Element> rowTemp;

    for (int i = 0; i < laplace.r[0].count(); i++)
    {
        rowTemp.append(Element(0,0));
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < laplace.r.count() - 1; o++)
    {
        QList<Element> rowR;
        QList<Element> rowG;
        QList<Element> rowB;

        rowR.append(Element(0,0));
        rowG.append(Element(0,0));
        rowB.append(Element(0,0));

        for (int i = 1; i < laplace.r[0].count() - 1; i++)
        {
            int xR = laplace.r[o][i+1] - laplace.r[o][i-1];
            int yR = laplace.r[o+1][i] - laplace.r[o-1][i];

            int xG = laplace.g[o][i+1] - laplace.g[o][i-1];
            int yG = laplace.g[o+1][i] - laplace.g[o-1][i];

            int xB = laplace.b[o][i+1] - laplace.b[o][i-1];
            int yB = laplace.b[o+1][i] - laplace.b[o-1][i];

            rowR.append(Element(xR, yR));
            rowG.append(Element(xG, yG));
            rowB.append(Element(xB, yB));
        }

        rowR.append(Element(0,0));
        rowG.append(Element(0,0));
        rowB.append(Element(0,0));

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    return result;
}

BertalmioProcessing::IsophoteDirection BertalmioProcessing::isophoteDirection_8(QImage &image)
{
    // Right difference

    IsophoteDirection result;
    QList<ElementFloat> rowTemp;

    for (int i = 0; i < image.width(); i++)
    {
        rowTemp.append(ElementFloat(0,0));
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < image.height() - 1; o++)
    {
        QList<ElementFloat> rowR;
        QList<ElementFloat> rowG;
        QList<ElementFloat> rowB;

        rowR.append(ElementFloat(0,0));
        rowG.append(ElementFloat(0,0));
        rowB.append(ElementFloat(0,0));

        for (int i = 1; i < image.width() - 1; i++)
        {
            int xUpR = qRed(image.pixel(i, o+1)) - qRed(image.pixel(i, o)); // Here have to be added minus
            int yUpR = qRed(image.pixel(i+1, o)) - qRed(image.pixel(i, o));
            float downR = qSqrt(xUpR * xUpR + yUpR * yUpR);
            float resultXR = downR != 0 ? -(xUpR/downR) : 0;
            float resultYR = downR != 0 ? yUpR/downR : 0;

            int xUpG = qGreen(image.pixel(i, o+1)) - qGreen(image.pixel(i, o)); // Here have to be added minus
            int yUpG = qGreen(image.pixel(i+1, o)) - qGreen(image.pixel(i, o));
            float downG = qSqrt(xUpG * xUpG + yUpG * yUpG);
            float resultXG = downG != 0 ? -(xUpG/downG) : 0;
            float resultYG = downG != 0 ? yUpG/downG : 0;

            int xUpB = qBlue(image.pixel(i, o+1)) - qBlue(image.pixel(i, o)); // Here have to be added minus
            int yUpB = qBlue(image.pixel(i+1, o)) - qBlue(image.pixel(i, o));
            float downB = qSqrt(xUpB * xUpB + yUpB * yUpB);
            float resultXB = downB != 0 ? -(xUpB/downB) : 0;
            float resultYB = downB != 0 ? yUpB/downB : 0;

            rowR.append(ElementFloat(resultXR, resultYR));
            rowG.append(ElementFloat(resultXG, resultYG));
            rowB.append(ElementFloat(resultXB, resultYB));
        }

        rowR.append(ElementFloat(0,0));
        rowG.append(ElementFloat(0,0));
        rowB.append(ElementFloat(0,0));

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    return result;
}

BertalmioProcessing::List2DInt BertalmioProcessing::beta_9(BertalmioProcessing::GradientLaplace &gradient, BertalmioProcessing::IsophoteDirection &isophote)
{
    List2DInt result;

    for (int o = 0; o < gradient.r.count(); o++)
    {
        QList<int> rowR;
        QList<int> rowG;
        QList<int> rowB;

        for (int i = 0; i < gradient.r[0].count(); i++)
        {
            int valueR = qRound(gradient.r[o][i].x * isophote.r[o][i].x + gradient.r[o][i].y * isophote.r[o][i].y);
            int valueG = qRound(gradient.g[o][i].x * isophote.g[o][i].x + gradient.g[o][i].y * isophote.g[o][i].y);
            int valueB = qRound(gradient.b[o][i].x * isophote.b[o][i].x + gradient.b[o][i].y * isophote.b[o][i].y);

            rowR.append(valueR);
            rowG.append(valueG);
            rowB.append(valueB);
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::gradientInput_10(QImage &image, List2DInt &beta)
{
    List2DFloat result;
    QList<float> rowTemp;

    for (int i = 0; i < image.width(); i++)
    {
        rowTemp.append(0);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < image.height() - 1; o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        for (int i = 1; i < image.width() - 1; i++)
        {
            int xbR = qRed(image.pixel(i, o)) - qRed(image.pixel(i-1, o));
            int xfR = qRed(image.pixel(i+1, o)) - qRed(image.pixel(i, o));
            int ybR = qRed(image.pixel(i, o)) - qRed(image.pixel(i, o-1));
            int yfR = qRed(image.pixel(i, o+1)) - qRed(image.pixel(i, o));

            int xbG = qGreen(image.pixel(i, o)) - qGreen(image.pixel(i-1, o));
            int xfG = qGreen(image.pixel(i+1, o)) - qGreen(image.pixel(i, o));
            int ybG = qGreen(image.pixel(i, o)) - qGreen(image.pixel(i, o-1));
            int yfG = qGreen(image.pixel(i, o+1)) - qGreen(image.pixel(i, o));

            int xbB = qBlue(image.pixel(i, o)) - qBlue(image.pixel(i-1, o));
            int xfB = qBlue(image.pixel(i+1, o)) - qBlue(image.pixel(i, o));
            int ybB = qBlue(image.pixel(i, o)) - qBlue(image.pixel(i, o-1));
            int yfB = qBlue(image.pixel(i, o+1)) - qBlue(image.pixel(i, o));

            // Red
            if (beta.r[o][i] > 0)
            {
                xbR = qMin(0, xbR);
                xfR = qMax(0, xfR);
                ybR = qMin(0, ybR);
                yfR = qMax(0, yfR);
            }
            else if (beta.r[o][i] < 0)
            {
                xbR = qMax(0, xbR);
                xfR = qMin(0, xfR);
                ybR = qMax(0, ybR);
                yfR = qMin(0, yfR);
            }
            else
            {
                //qDebug() << "beta R is equal 0 - not handled";
            }

            // Green
            if (beta.g[o][i] > 0)
            {
                xbG = qMin(0, xbG);
                xfG = qMax(0, xfG);
                ybG = qMin(0, ybG);
                yfG = qMax(0, yfG);
            }
            else if (beta.g[o][i] < 0)
            {
                xbG = qMax(0, xbG);
                xfG = qMin(0, xfG);
                ybG = qMax(0, ybG);
                yfG = qMin(0, yfG);
            }
            else
            {
                //qDebug() << "beta G is equal 0 - not handled";
            }

            // Blue
            if (beta.b[o][i] > 0)
            {
                xbB = qMin(0, xbB);
                xfB = qMax(0, xfB);
                ybB = qMin(0, ybB);
                yfB = qMax(0, yfB);
            }
            else if (beta.b[o][i] < 0)
            {
                xbB = qMax(0, xbB);
                xfB = qMin(0, xfB);
                ybB = qMax(0, ybB);
                yfB = qMin(0, yfB);
            }
            else
            {
                //qDebug() << "beta B is equal 0 - not handled";
            }

            // Result
            float valueR = qSqrt(xbR * xbR + xfR * xfR + ybR * ybR + yfR * yfR);
            float valueG = qSqrt(xbG * xbG + xfG * xfG + ybG * ybG + yfG * yfG);
            float valueB = qSqrt(xbB * xbB + xfB * xfB + ybB * ybB + yfB * yfB);

            rowR.append(valueR);
            rowG.append(valueG);
            rowB.append(valueB);
        }

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::partialResult_5(BertalmioProcessing::List2DInt &beta, BertalmioProcessing::List2DFloat &gradient)
{
    List2DFloat result;

    for (int o = 0; o < beta.r.count(); o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        for (int i = 0; i < beta.r[0].count(); i++)
        {
            float valueR = beta.r[o][i] * gradient.r[o][i];
            float valueG = beta.g[o][i] * gradient.g[o][i];
            float valueB = beta.b[o][i] * gradient.b[o][i];

            rowR.append(valueR);
            rowG.append(valueG);
            rowB.append(valueB);
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    return result;
}
