#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "bertalmioprocessing.h"

const int A = 15; // 15
const int B = 2; // 2

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BertalmioProcessing bertalmioParts;

    QImage input(QCoreApplication::applicationDirPath() + "/lena.png");
    QImage mask(QCoreApplication::applicationDirPath() + "/mask.png");
    QImage mask_full(QCoreApplication::applicationDirPath() + "/mask_full.png");

    if (input.isNull() || mask.isNull() || mask_full.isNull())
    {
        qDebug() << "Image or mask not found!";
    }
    else
    {
        qDebug() << "-Process started!-";
        qDebug() << "==================";

        qDebug() << "Initialization...";

        bool stable = false;
        int iteration = 0;

        BertalmioProcessing::List2DFloat gradientInput;
        BertalmioProcessing::List2DFloat anisotropic;
        BertalmioProcessing::List2DFloat partialResult;
        BertalmioProcessing::GradientLaplace gradientLaplace;
        BertalmioProcessing::IsophoteDirection isophoteDirection;
        BertalmioProcessing::List2DFloat beta;
        BertalmioProcessing::List2DFloat laplace;
        BertalmioProcessing::List2DFloat inputFloat;

        inputFloat = bertalmioParts.imageToFloat(input);

        qDebug() << "----------ok----------";
        qDebug() << "Anisotropic diffusion...";

        // Whole image should goes thru anisotropic diffusion
        anisotropic = bertalmioParts.anisotropicDiffusion_3(inputFloat);

        qDebug() << "----------ok----------";

        // ...after that go to the loop
        while (!stable)
        {
            for(int i = 0; i < A; i++)
            {
                //...few iterations of inpainting
                qDebug() << i << ". inpainting iteration...";

                laplace = bertalmioParts.laplace_7(anisotropic);
                gradientLaplace = bertalmioParts.gradientLaplace_6(anisotropic);
                isophoteDirection = bertalmioParts.isophoteDirection_8(anisotropic);
                beta = bertalmioParts.beta_9(gradientLaplace, isophoteDirection);
                gradientInput = bertalmioParts.gradientInput_10(anisotropic, beta);
                partialResult = bertalmioParts.partialResult_5(beta, gradientInput);

                bertalmioParts.updateImage_4(inputFloat, partialResult, mask);
                bertalmioParts.updateImage_4(anisotropic, partialResult, mask);

                //qDebug() << "Gradient laplace: " << gradientLaplace.r[122][191].x << gradientLaplace.r[122][191].y;
                //qDebug() << "Isophote: " << isophoteDirection.r[122][191].x << gradientLaplace.r[122][191].y;
                //qDebug() << "Laplace: " << laplace.r[10];
                //qDebug() << "Beta: " << beta.r[10];
                //qDebug() << "gradientInput: " << gradientInput.r[10];
                //qDebug() << "partialResult: " << partialResult.r[10];
                //qDebug() << "inputFloat: " << inputFloat.r[10];

                //QImage result;
                //result = bertalmioParts.floatToImage(inputFloat);
                //result.save("result_" + QString::number(iteration) + ".png");
                //result = bertalmioParts.floatToImage(laplace);
                //result.save("laplace_" + QString::number(iteration) + ".png");
                //result = bertalmioParts.floatToImage(beta);
                //result.save("beta_" + QString::number(iteration) + ".png");
                //iteration++;
            }

            qDebug() << "----------ok----------";

            for(int i = 0; i < B; i++)
            {
                // ...few iterations of anisotropic diffusion.
                qDebug() << i << ". anisotropic filtering iteration...";

                anisotropic = bertalmioParts.anisotropicDiffusion_3(anisotropic);
            }

            qDebug() << "----------ok----------";

            stable = bertalmioParts.stabilityTest(partialResult);

            QImage result;
            result = bertalmioParts.floatToImage(anisotropic);
            result.save("result_" + QString::number(iteration) + ".png");
            iteration++;
        }

        qDebug() << "-==Ended==-";
    }

    /*
    QImage result;

    BertalmioProcessing::List2DFloat inputFloat;
    BertalmioProcessing::List2DFloat anisotropic;

    inputFloat = bertalmioParts.imageToFloat(input);
    anisotropic = bertalmioParts.anisotropicDiffusion_3(inputFloat);
    result = bertalmioParts.floatToImage(anisotropic);

    result.save("testoOutput.png");    
    */

    /*
    QImage result;
    BertalmioProcessing::List2DFloat inputFloat;
    BertalmioProcessing::List2DFloat laplace;

    inputFloat = bertalmioParts.imageToFloat(input);
    laplace = bertalmioParts.laplace_7(inputFloat);
    result = bertalmioParts.floatToImage(laplace);

    result.save("testLapla.png");
    */

    //qDebug() << gradientLaplace.r[9][10].x << gradientLaplace.r[9][10].y;
    //qDebug() << isophoteDirection.r[4][4].x << isophoteDirection.r[4][4].y;
    //qDebug() << isophoteDirection.g[4][4].x << isophoteDirection.g[4][4].y;
    //qDebug() << isophoteDirection.b[4][4].x << isophoteDirection.b[4][4].y;

    return a.exec();
}
