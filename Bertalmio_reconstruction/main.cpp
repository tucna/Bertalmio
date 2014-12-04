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

    if (input.isNull())
    {
        qDebug() << "Image not found!";
    }
    else
    {
        qDebug() << "-Process started!-";
        qDebug() << "==================";

        qDebug() << "Initialization...";

        bool stable = false;
        int iteration = 0;

        BertalmioProcessing::List2DFloat gradientInput;
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
        bertalmioParts.anisotropicDiffusion_3(inputFloat);

        qDebug() << "----------ok----------";

        // ...after that go to the loop
        while (!stable)
        {
            for(int i = 0; i < A; i++)
            {
                //...few iterations of inpainting
                qDebug() << i << ". inpainting iteration...";

                laplace = bertalmioParts.laplace_7(inputFloat);
                gradientLaplace = bertalmioParts.gradientLaplace_6(inputFloat);
                isophoteDirection = bertalmioParts.isophoteDirection_8(inputFloat);
                beta = bertalmioParts.beta_9(gradientLaplace, isophoteDirection);
                gradientInput = bertalmioParts.gradientInput_10(inputFloat, beta);
                partialResult = bertalmioParts.partialResult_5(beta, gradientInput);

                bertalmioParts.updateImage_4(inputFloat, partialResult);

                QImage result;
                result = bertalmioParts.floatToImage(partialResult);
                result.save("input_" + QString::number(iteration) + ".png");
                //result = bertalmioParts.floatToImage(laplace);
                //result.save("laplace_" + QString::number(iteration) + ".png");
                //result = bertalmioParts.floatToImage(beta);
                //result.save("beta_" + QString::number(iteration) + ".png");
                iteration++;
            }

            qDebug() << "----------ok----------";

            for(int i = 0; i < B; i++)
            {
                // ...few iterations of anisotropic diffusion.
                qDebug() << i << ". anisotropic filtering iteration...";

                bertalmioParts.anisotropicDiffusion_3(inputFloat);
            }

            qDebug() << "----------ok----------";

            stable = bertalmioParts.stabilityTest(partialResult);
        }
    }

    /*
    QImage result;
    BertalmioProcessing::List2DFloat inputFloat;

    inputFloat = bertalmioParts.imageToFloat(input);
    bertalmioParts.anisotropicDiffusion_3(inputFloat);
    result = bertalmioParts.floatToImage(inputFloat);

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
