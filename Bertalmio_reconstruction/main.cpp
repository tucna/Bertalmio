#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "bertalmioprocessing.h"

const int A = 15;
const int B = 2;

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
        QImage anisotropic;
        bool stable = false;

        BertalmioProcessing::List2DFloat gradientInput;
        BertalmioProcessing::List2DFloat partialResult;
        BertalmioProcessing::GradientLaplace gradientLaplace;
        BertalmioProcessing::IsophoteDirection isophoteDirection;
        BertalmioProcessing::List2DInt beta;
        BertalmioProcessing::List2DInt laplace;

        // Whole imahe should goes thru anisotropic diffusion
        anisotropic = bertalmioParts.anisotropicDiffusion(input);

        // ...after that go to the loop
        while (!stable)
        {
            for(int i = 0; i < A; i++)
            {
                //...few iterations of inpainting
                laplace = bertalmioParts.laplace_7(input);
                isophoteDirection = bertalmioParts.isophoteDirection_8(input);
                beta = bertalmioParts.beta_9(gradientLaplace, isophoteDirection);
                gradientInput = bertalmioParts.gradientInput_10(input, beta);
            }

            for(int i = 0; i < B; i++)
            {
                // ...few iterations of anisotropic diffusion.
                anisotropic = bertalmioParts.anisotropicDiffusion(input);
            }
        }

        /*
        laplace = bertalmioParts.laplace_7(input);
        gradientLaplace = bertalmioParts.gradientLaplace_6(laplace);
        isophoteDirection = bertalmioParts.isophoteDirection_8(input);
        beta = bertalmioParts.beta_9(gradientLaplace, isophoteDirection);
        gradientInput = bertalmioParts.gradientInput_10(input, beta);
        partialResult = bertalmioParts.partialResult_5(beta, gradientInput);
        */

        //qDebug() << gradientLaplace.r[9][10].x << gradientLaplace.r[9][10].y;
        //qDebug() << isophoteDirection.r[4][4].x << isophoteDirection.r[4][4].y;
        //qDebug() << isophoteDirection.g[4][4].x << isophoteDirection.g[4][4].y;
        //qDebug() << isophoteDirection.b[4][4].x << isophoteDirection.b[4][4].y;
    }

    return a.exec();
}
