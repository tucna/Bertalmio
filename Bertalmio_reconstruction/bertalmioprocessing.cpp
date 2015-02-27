#include <QImage>
#include <QRgb>
#include <QDebug>

#include <qmath.h>

#include "bertalmioprocessing.h"

BertalmioProcessing::BertalmioProcessing()
{
}

BertalmioProcessing::List2DFloat BertalmioProcessing::anisotropicDiffusion_3(List2DFloat &imageFloat)
{
    const float KAPPA = 30.0f;
    const float DELTA_T = 1.0f/7.0f;
    const int NUM_ITER = 15;

    List2DFloat result;

    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    // copy because of more iterations
    for (int o = 0; o < height; o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        for (int i = 0; i < width; i++)
        {
            rowR.append(imageFloat.r[o][i]);
            rowG.append(imageFloat.g[o][i]);
            rowB.append(imageFloat.b[o][i]);
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    // process
    for (int t = 0; t < NUM_ITER; t++)
    {
        for (int o = 1; o < height - 1; o++)
        {
            for (int i = 1; i < width - 1; i++)
            {
                // nabla
                float nablaN_R = result.r[o][i-1] - result.r[o][i];
                float nablaS_R = result.r[o][i+1] - result.r[o][i];
                float nablaE_R = result.r[o+1][i] - result.r[o][i];
                float nablaW_R = result.r[o-1][i] - result.r[o][i];

                float nablaN_G = result.g[o][i-1] - result.g[o][i];
                float nablaS_G = result.g[o][i+1] - result.g[o][i];
                float nablaE_G = result.g[o+1][i] - result.g[o][i];
                float nablaW_G = result.g[o-1][i] - result.g[o][i];

                float nablaN_B = result.b[o][i-1] - result.b[o][i];
                float nablaS_B = result.b[o][i+1] - result.b[o][i];
                float nablaE_B = result.b[o+1][i] - result.b[o][i];
                float nablaW_B = result.b[o-1][i] - result.b[o][i];

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

                /*
                // c - second one
                float cN_R = 1.0f / (1 + qPow(nablaN_R/KAPPA,2));
                float cS_R = 1.0f / (1 + qPow(nablaS_R/KAPPA,2));
                float cE_R = 1.0f / (1 + qPow(nablaE_R/KAPPA,2));
                float cW_R = 1.0f / (1 + qPow(nablaW_R/KAPPA,2));

                float cN_G = 1.0f / (1 + qPow(nablaN_G/KAPPA,2));
                float cS_G = 1.0f / (1 + qPow(nablaS_G/KAPPA,2));
                float cE_G = 1.0f / (1 + qPow(nablaE_G/KAPPA,2));
                float cW_G = 1.0f / (1 + qPow(nablaW_G/KAPPA,2));

                float cN_B = 1.0f / (1 + qPow(nablaN_B/KAPPA,2));
                float cS_B = 1.0f / (1 + qPow(nablaS_B/KAPPA,2));
                float cE_B = 1.0f / (1 + qPow(nablaE_B/KAPPA,2));
                float cW_B = 1.0f / (1 + qPow(nablaW_B/KAPPA,2));
                */

                // application
                float valueR = result.r[o][i] + DELTA_T * (nablaN_R * cN_R + nablaS_R * cS_R + nablaE_R * cE_R + nablaW_R * cW_R);
                float valueG = result.g[o][i] + DELTA_T * (nablaN_G * cN_G + nablaS_G * cS_G + nablaE_G * cE_G + nablaW_G * cW_G);
                float valueB = result.b[o][i] + DELTA_T * (nablaN_B * cN_B + nablaS_B * cS_B + nablaE_B * cE_B + nablaW_B * cW_B);

                valueR = qIsFinite(valueR) ? valueR : 0;
                valueG = qIsFinite(valueG) ? valueG : 0;
                valueB = qIsFinite(valueB) ? valueB : 0;

                result.r[o][i] = valueR;
                result.g[o][i] = valueG;
                result.b[o][i] = valueB;
            }
        }
    }

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::laplace_7(const List2DFloat &imageFloat)
{
    List2DFloat result;
    QList<float> rowTemp;

    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    /*
     * 0  1  0
     * 1 -4  1
     * 0  1  0
     */    

    for (int i = 0; i < width; i++)
    {
        rowTemp.append(0);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < height - 1; o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        for (int i = 1; i < width - 1; i++)
        {
            float valueR = imageFloat.r[o-1][i] + imageFloat.r[o][i-1] + imageFloat.r[o][i+1] + imageFloat.r[o+1][i] - 4 * imageFloat.r[o][i];
            float valueG = imageFloat.g[o-1][i] + imageFloat.g[o][i-1] + imageFloat.g[o][i+1] + imageFloat.g[o+1][i] - 4 * imageFloat.g[o][i];
            float valueB = imageFloat.b[o-1][i] + imageFloat.b[o][i-1] + imageFloat.b[o][i+1] + imageFloat.b[o+1][i] - 4 * imageFloat.b[o][i];

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

BertalmioProcessing::Gradient BertalmioProcessing::gradientLaplace_6(const List2DFloat &laplace)
{
    Gradient result;
    QList<ElementFloat> rowTemp;

    for (int i = 0; i < laplace.r[0].count(); i++)
    {
        rowTemp.append(ElementFloat(0.0f, 0.0f));
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < laplace.r.count() - 1; o++)
    {
        QList<ElementFloat> rowR;
        QList<ElementFloat> rowG;
        QList<ElementFloat> rowB;

        rowR.append(ElementFloat(0.0f, 0.0f));
        rowG.append(ElementFloat(0.0f, 0.0f));
        rowB.append(ElementFloat(0.0f, 0.0f));

        for (int i = 1; i < laplace.r[0].count() - 1; i++)
        {
            float xR = laplace.r[o][i+1] - laplace.r[o][i-1];
            float yR = laplace.r[o+1][i] - laplace.r[o-1][i];

            float xG = laplace.g[o][i+1] - laplace.g[o][i-1];
            float yG = laplace.g[o+1][i] - laplace.g[o-1][i];

            float xB = laplace.b[o][i+1] - laplace.b[o][i-1];
            float yB = laplace.b[o+1][i] - laplace.b[o-1][i];

            xR = qIsFinite(xR) ? xR : 0;
            yR = qIsFinite(yR) ? yR : 0;

            xG = qIsFinite(xG) ? xG : 0;
            yG = qIsFinite(yG) ? yG : 0;

            xB = qIsFinite(xB) ? xB : 0;
            yB = qIsFinite(yB) ? yB : 0;

            if (o == 92 && i == 69)
            {
                qDebug() << "gradientLaplace_6:"<< laplace.r[o][i+1] << laplace.r[o][i-1] << laplace.r[o+1][i] << laplace.r[o-1][i] << xR << yR;
            }

            rowR.append(ElementFloat(xR, yR));
            rowG.append(ElementFloat(xG, yG));
            rowB.append(ElementFloat(xB, yB));
        }

        rowR.append(ElementFloat(0.0f, 0.0f));
        rowG.append(ElementFloat(0.0f, 0.0f));
        rowB.append(ElementFloat(0.0f, 0.0f));

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    return result;
}

BertalmioProcessing::Gradient BertalmioProcessing::isophoteDirection_8(const List2DFloat &imageFloat)
{
    Gradient result;
    QList<ElementFloat> rowTemp;

    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    for (int i = 0; i < width; i++)
    {
        rowTemp.append(ElementFloat(0,0));
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < height - 1; o++)
    {
        QList<ElementFloat> rowR;
        QList<ElementFloat> rowG;
        QList<ElementFloat> rowB;

        rowR.append(ElementFloat(0,0));
        rowG.append(ElementFloat(0,0));
        rowB.append(ElementFloat(0,0));

        // central differencies are better (probably)

        for (int i = 1; i < width - 1; i++)
        {           
            float xUpR = imageFloat.r[o][i+1] - imageFloat.r[o][i-1]; // Here have to be added minus
            float yUpR = imageFloat.r[o+1][i] - imageFloat.r[o-1][i];
            float downR = qSqrt(xUpR * xUpR + yUpR * yUpR);
            float resultXR = -(yUpR/downR);
            float resultYR = xUpR/downR;

            float xUpG = imageFloat.g[o][i+1] - imageFloat.g[o][i-1]; // Here have to be added minus
            float yUpG = imageFloat.g[o+1][i] - imageFloat.g[o-1][i];
            float downG = qSqrt(xUpG * xUpG + yUpG * yUpG);
            float resultXG = -(yUpG/downG);
            float resultYG = xUpG/downG;

            float xUpB = imageFloat.b[o][i+1] - imageFloat.b[o][i-1]; // Here have to be added minus
            float yUpB = imageFloat.b[o+1][i] - imageFloat.b[o-1][i];
            float downB = qSqrt(xUpB * xUpB + yUpB * yUpB);
            float resultXB = -(yUpB/downB);
            float resultYB = xUpB/downB;

            resultXR = qIsFinite(resultXR) ? resultXR : 0;
            resultYR = qIsFinite(resultYR) ? resultYR : 0;

            resultXG = qIsFinite(resultXG) ? resultXG : 0;
            resultYG = qIsFinite(resultYG) ? resultYG : 0;

            resultXB = qIsFinite(resultXB) ? resultXB : 0;
            resultYB = qIsFinite(resultYB) ? resultYB : 0;

            if (o == 92 && i == 69)
            {
                qDebug() << "isophoteDirection_8:"<< resultXR << resultYR;
            }

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

BertalmioProcessing::Gradient BertalmioProcessing::gradient(const BertalmioProcessing::List2DFloat &imageFloat)
{
    Gradient result;
    QList<ElementFloat> rowTemp;

    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    for (int i = 0; i < width; i++)
    {
        rowTemp.append(ElementFloat(0,0));
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < height - 1; o++)
    {
        QList<ElementFloat> rowR;
        QList<ElementFloat> rowG;
        QList<ElementFloat> rowB;

        rowR.append(ElementFloat(0,0));
        rowG.append(ElementFloat(0,0));
        rowB.append(ElementFloat(0,0));

        for (int i = 1; i < width - 1; i++)
        {
            float xR = (imageFloat.r[o][i+1] - imageFloat.r[o][i-1]) / 2.0f;
            float yR = (imageFloat.r[o+1][i] - imageFloat.r[o-1][i]) / 2.0f;

            float xG = (imageFloat.g[o][i+1] - imageFloat.g[o][i-1]) / 2.0f;
            float yG = (imageFloat.g[o+1][i] - imageFloat.g[o-1][i]) / 2.0f;

            float xB = (imageFloat.b[o][i+1] - imageFloat.b[o][i-1]) / 2.0f;
            float yB = (imageFloat.b[o+1][i] - imageFloat.b[o-1][i]) / 2.0f;

            rowR.append(ElementFloat(xR, yR));
            rowG.append(ElementFloat(xG, yG));
            rowB.append(ElementFloat(xB, yB));
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

    // fixes
    for (int i = 1; i < width - 1; i++)
    {
        result.r[0][i].x = (imageFloat.r[0][i+1] - imageFloat.r[0][i-1]) / 2.0f;
        result.r[height-1][i].x = (imageFloat.r[height-1][i+1] - imageFloat.r[height-1][i-1]) / 2.0f;
    }

    for (int o = 1; o < height - 1; o++)
    {
        result.r[o][0].x = (imageFloat.r[o+1][0] - imageFloat.r[o-1][0]) / 2.0f;
        result.r[o][width-1].x = (imageFloat.r[o+1][width-1] - imageFloat.r[o-1][width-1]) / 2.0f;
    }

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::beta_9(const Gradient &laplace, const Gradient &isophote)
{
    List2DFloat result;

    for (int o = 0; o < laplace.r.count(); o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        for (int i = 0; i < laplace.r[0].count(); i++)
        {
            float valueR = (-laplace.r[o][i].x * isophote.r[o][i].y + laplace.r[o][i].y * isophote.r[o][i].x) /
                           (qSqrt(isophote.r[o][i].x*isophote.r[o][i].x + isophote.r[o][i].y*isophote.r[o][i].y) + 0.001f);
            float valueG = (-laplace.g[o][i].x * isophote.g[o][i].y + laplace.g[o][i].y * isophote.g[o][i].x) /
                           (qSqrt(isophote.g[o][i].x*isophote.g[o][i].x + isophote.g[o][i].y*isophote.g[o][i].y) + 0.001f);
            float valueB = (-laplace.b[o][i].x * isophote.b[o][i].y + laplace.b[o][i].y * isophote.b[o][i].x) /
                           (qSqrt(isophote.b[o][i].x*isophote.b[o][i].x + isophote.b[o][i].y*isophote.b[o][i].y) + 0.001f);

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

void BertalmioProcessing::updateImage_4(List2DFloat &imageFloat, const List2DFloat &partialResult, const float dt)
{
    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    for (int o = 0; o < height; o++)
    {
        for (int i = 0; i < width; i++)
        {
            float valueR = imageFloat.r[o][i] + dt * partialResult.r[o][i];
            float valueG = imageFloat.g[o][i] + dt * partialResult.g[o][i];
            float valueB = imageFloat.b[o][i] + dt * partialResult.b[o][i];

            imageFloat.r[o][i] = valueR;
            imageFloat.g[o][i] = valueG;
            imageFloat.b[o][i] = valueB;
        }
    }
}

bool BertalmioProcessing::stabilityTest(const BertalmioProcessing::List2DFloat &partialResult)
{
    // TEST VERSION!

    bool result = true;

    return false;

    int height = partialResult.r.count();
    int width = partialResult.r[0].count();

    for (int o = 0; o < height; o++)
    {
        for (int i = 0; i < width; i++)
        {
            int valueR = qRound(partialResult.r[o][i]);
            int valueG = qRound(partialResult.g[o][i]);
            int valueB = qRound(partialResult.b[o][i]);

            if (valueR != 0 || valueG != 0 || valueB != 0)
            {
                return false;
            }
        }
    }

    //return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::imageToFloat(const QImage &image)
{
    List2DFloat result;

    for (int o = 0; o < image.height(); o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        for (int i = 0; i < image.width(); i++)
        {
            rowR.append(qRed(image.pixel(i, o)));
            rowG.append(qGreen(image.pixel(i, o)));
            rowB.append(qBlue(image.pixel(i, o)));
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::array2DToFloat(const float image[][21], int N)
{
    List2DFloat result;

    for (int o = 0; o < N; o++)
    {
        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        for (int i = 0; i < 21; i++)
        {
            rowR.append(image[o][i]);
            rowG.append(image[o][i]);
            rowB.append(image[o][i]);
        }

        result.r.append(rowR);
        result.g.append(rowG);
        result.b.append(rowB);
    }

    return result;
}

QImage BertalmioProcessing::floatToImage(const List2DFloat &imageFloat)
{
    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    QImage result(width, height, QImage::Format_ARGB32);

    for (int o = 0; o < height; o++)
    {
        for (int i = 0; i < width; i++)
        {
            int valueR = qRound(imageFloat.r[o][i]);
            int valueG = qRound(imageFloat.g[o][i]);
            int valueB = qRound(imageFloat.b[o][i]);

            if (o == 92 && i == 69)
            {
                qDebug() << "floatToImage:"<< valueR;
            }

            valueR = qMin(valueR, 255);
            valueG = qMin(valueG, 255);
            valueB = qMin(valueB, 255);

            valueR = qMax(valueR, 0);
            valueG = qMax(valueG, 0);
            valueB = qMax(valueB, 0);                        

            result.setPixel(i, o, qRgb(valueR, valueG, valueB));
        }
    }

    return result;
}

BertalmioProcessing::List2DFloat BertalmioProcessing::gradientInput_10(const List2DFloat &imageFloat, const List2DFloat &beta, const float mask[][21])
{
    List2DFloat result;
    QList<float> rowTemp;

    QList<float> tempDifference;
    tempDifference.clear();


    int height = imageFloat.r.count();
    int width = imageFloat.r[0].count();

    for (int i = 0; i < width; i++)
    {
        rowTemp.append(0);
        tempDifference.append(0);
    }

    result.r.append(rowTemp);
    result.g.append(rowTemp);
    result.b.append(rowTemp);

    for (int o = 1; o < height - 1; o++)
    {
        qDebug() << tempDifference;

        tempDifference.clear();

        QList<float> rowR;
        QList<float> rowG;
        QList<float> rowB;

        rowR.append(0);
        rowG.append(0);
        rowB.append(0);

        for (int i = 1; i < width - 1; i++)
        {
            float xbR = imageFloat.r[o][i] - imageFloat.r[o][i-1];
            float xfR = imageFloat.r[o][i+1] - imageFloat.r[o][i];
            float ybR = imageFloat.r[o][i] - imageFloat.r[o-1][i];
            float yfR = imageFloat.r[o+1][i] - imageFloat.r[o][i];

            float xbG = imageFloat.g[o][i] - imageFloat.g[o][i-1];
            float xfG = imageFloat.g[o][i+1] - imageFloat.g[o][i];
            float ybG = imageFloat.g[o][i] - imageFloat.g[o-1][i];
            float yfG = imageFloat.g[o+1][i] - imageFloat.g[o][i];

            float xbB = imageFloat.b[o][i] - imageFloat.b[o][i-1];
            float xfB = imageFloat.b[o][i+1] - imageFloat.b[o][i];
            float ybB = imageFloat.b[o][i] - imageFloat.b[o-1][i];
            float yfB = imageFloat.b[o+1][i] - imageFloat.b[o][i];

            tempDifference.append(xfR);


            // Red
            if (beta.r[o][i] > 0)
            {
                xbR = qMin(0.0f, xbR);
                xfR = qMax(0.0f, xfR);
                ybR = qMin(0.0f, ybR);
                yfR = qMax(0.0f, yfR);
            }
            else if (beta.r[o][i] < 0)
            {
                xbR = qMax(0.0f, xbR);
                xfR = qMin(0.0f, xfR);
                ybR = qMax(0.0f, ybR);
                yfR = qMin(0.0f, yfR);
            }
            else
            {
                //qDebug() << "beta R is equal 0 - not handled";
            }

            // Green
            if (beta.g[o][i] > 0)
            {
                xbG = qMin(0.0f, xbG);
                xfG = qMax(0.0f, xfG);
                ybG = qMin(0.0f, ybG);
                yfG = qMax(0.0f, yfG);
            }
            else if (beta.g[o][i] < 0)
            {
                xbG = qMax(0.0f, xbG);
                xfG = qMin(0.0f, xfG);
                ybG = qMax(0.0f, ybG);
                yfG = qMin(0.0f, yfG);
            }
            else
            {
                //qDebug() << "beta G is equal 0 - not handled";
            }

            // Blue
            if (beta.b[o][i] > 0)
            {
                xbB = qMin(0.0f, xbB);
                xfB = qMax(0.0f, xfB);
                ybB = qMin(0.0f, ybB);
                yfB = qMax(0.0f, yfB);
            }
            else if (beta.b[o][i] < 0)
            {
                xbB = qMax(0.0f, xbB);
                xfB = qMin(0.0f, xfB);
                ybB = qMax(0.0f, ybB);
                yfB = qMin(0.0f, yfB);
            }
            else
            {
                //qDebug() << "beta B is equal 0 - not handled";
            }

            // Result
            float valueR = qSqrt(mask[o][i] * (xbR * xbR + xfR * xfR + ybR * ybR + yfR * yfR));
            float valueG = qSqrt(mask[o][i] * (xbG * xbG + xfG * xfG + ybG * ybG + yfG * yfG));
            float valueB = qSqrt(mask[o][i] * (xbB * xbB + xfB * xfB + ybB * ybB + yfB * yfB));

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

BertalmioProcessing::List2DFloat BertalmioProcessing::partialResult_5(const List2DFloat &beta, const List2DFloat &gradient)
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
