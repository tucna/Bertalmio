#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "bertalmioprocessing.h"

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
        QImage laplace;
        QImage gradientInput;

        BertalmioProcessing::GradientLaplace gradientLaplace;
        BertalmioProcessing::IsophoteDirection isophoteDirection;

        laplace = bertalmioParts.laplace(input);
        gradientLaplace = bertalmioParts.gradientLaplace(laplace);        
        isophoteDirection = bertalmioParts.isophoteDirection(input);

        anisotropic = bertalmioParts.anisotropicDiffusion(input);
        gradientInput = bertalmioParts.gradientInput(input);

        laplace.save("laplace.png");

        // GradientLaplace starts on 1,1 !!
        //qDebug() << gradientLaplace.r[9][10].x << gradientLaplace.r[9][10].y;
        qDebug() << isophoteDirection.r[4][4].x << isophoteDirection.r[4][4].y;
        qDebug() << isophoteDirection.g[4][4].x << isophoteDirection.g[4][4].y;
        qDebug() << isophoteDirection.b[4][4].x << isophoteDirection.b[4][4].y;
    }

    return a.exec();
}
